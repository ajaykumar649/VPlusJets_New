#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "card.h"

#include "hwwinputs.h"
#include "hwwutils.h"

using namespace std;

//================================================================================

Card::Card(double         procchanyield,
	   const TString& procname,
	   const TString& systname,
	   const TString& channame
	   )
{
  nbackproc = 0;
  nsigproc  = 0;

  cout << "\tmake new card ";

  ProcData_t pd;

  pair<TString,double> channel;

  pd.name          = procname;
  channel.first    = channame;
  channel.second   = procchanyield; // the yield for this channel

  pd.channels.insert(channel);

  if( procname.Contains("data") ) {
    data = pd; // done.
  } else {

    if( procname.Contains("ignal")
#ifdef ISHWW
	|| (procname.Contains("ggH") ||
	    procname.Contains("qq") )
#endif //ISHWW
	) {                              // signal

      assert(!systname.Length());     // don't expect to encounter shape systematic first.

      nsigproc++;
      pd.procindex = 1-nsigproc; /* process index for signal processes required to be distinct,
				    as well as 0 or negative */
    } else {                                                        //background
      nbackproc++;
      pd.procindex = nbackproc;
    }
      
    processes.push_back(pd);
    pname2index[procname]= 0;

  } // else not data

  channels.insert(channame);
}                                                                   // Card::Card

//================================================================================

void
Card::addToCard(double         procchanyield,// process/channel yield
		const TString& procname,     // process name 
		const TString& systname,     // name of (shape) systematic applied
		const int      ichanref,     // channel reference index
		const int      ichan,        // channel index
		const int      nchan         // number of channels in card
	  )
{
  cout << "\tadd to card ";

  // With each input histogram one must:
  // 1. If a new process, update the process info in the card: "card.processes", "card.pname2index",
  // 2. Update new channel information "processes[procindex].channels" (channel name/rate pairs)
  // 3. Update the systematics information as necessary:
  //    if systname.length, add shape systematic information to
  //    systematics (if new systematic), processes[procindex].systrates[systname][ichan]
  // 
  pair<TString,double> channel;

  const pair<double,double> zeropair(0.0,0.0);

  TString channame(channames[ichanref]);

  channels.insert(channame);

  channel.first  = channame;
  channel.second = procchanyield;
  
  if( procname.Contains("data") )         // data observation

    if (!data.name.Length()) {             // first channel of data encountered
      ProcData_t pd;
      pd.name = procname;
      pd.channels.insert(channel);
      data = pd;
    }else{                                      // another channel for data, possibly
      assert(!data.name.CompareTo(procname)); // must be the same
      data.channels.insert(channel);
    }

  else if( procname.Contains("ignal")
#ifdef ISHWW
	    || (procname.Contains("ggH") ||
		procname.Contains("qq") )
#endif //ISHWW
	   ) { // signal expectation


    map<TString,int>::iterator pit;
    pit = pname2index.find(procname);
    if (pit == pname2index.end()) {    // new signal process, first channel encountered
      nsigproc++;

      assert(!systname.Length());     // don't expect to encounter shape systematic first

      ProcData_t pd;
      pd.name      = procname;
      pd.procindex = 1-nsigproc; /* process index for signal processes required to be distinct,
					 as well as 0 or negative */

      pd.channels.insert(channel);

      // put new signal in front, have to adjust the mapped indices for all the rest.
      // This maintains the proper ordering of processes in the datacard (for LandS)
      //
      for (pit = pname2index.begin(); pit != pname2index.end(); pit++)
	pit->second++;

      processes.push_front(pd);

      pname2index[pd.name] = 0;

    } else {                        // another channel for signal, or a shape systematic

      ProcData_t& pd = processes[pit->second];
      assert(pd.name.Length());             // process should exist here
      assert(!pd.name.CompareTo(procname)); // must be the same

      if (systname.Length()) {
#ifdef ISHWW
	if (procname.Contains("ggH")) {
	  // In this case particularly we do *not* pay attention to the systematics name, but
	  // assume this is the common interference shape systematic for ggH signal
	  //
	  const pair<double,double> onepair(0.0,1.0);

	  pd.systrates["interf_ggH"].resize(nchan,onepair); /* shape-based systematic,
							       assume all channels CORRELATED,
							       so all channels get factor 1.0, */
	}
#else
	systematics[systname] = "shape";

	pd.systrates[systname].resize(nchan,zeropair); /* shape-based systematic,
							  assume all channels uncorrelated,
							  so all channels get factor 0.0, */
	pd.systrates[systname][ichan].second = 1.0;     // except the current channel
#endif

      } else {

	pd.channels.insert(channel);

      } // nominal signal histo
    } // signal histo for another channel, or systematic

  } else {                               // background process

    map<TString,int>::iterator pit;
    pit = pname2index.find(procname);
    if (pit == pname2index.end()) {    // first channel of this background process encountered
      nbackproc++;

      ProcData_t pd;
      pd.name = procname;
      pd.procindex = nbackproc;
      pd.channels.insert(channel);

      pname2index[pd.name] = (int)processes.size();
      processes.push_back(pd);

    } else {                                /* either another channel, or a shape
					       systematic for an existing channel */
      ProcData_t& pd = processes[pit->second];

      if (systname.Length()) { // a shape-based limit, all channels get a factor of 0.0
	systematics[systname] = "shape";
	pd.systrates[systname].resize(nchan,zeropair);
	pd.systrates[systname][ichan].second = 1.0; // except the current channel
      }
      else {
	assert(pd.name.Length());             // process should exist here
	assert(!pd.name.CompareTo(procname)); // and be named the same
	pd.channels.insert(channel);
      }
    }
  }
}                                                               // Card::addToCard

//================================================================================

void
Card::addSystematics(int massgev,
		     const vector<int>& channellist)
{
  int nchan = (int)channels.size();

  const pair<double,double> zeropair(0.0,0.0);

  // per-channel systematics:
  for (std::set<TString>::iterator it=channels.begin(); it!=channels.end(); it++) {
    TString channame = *it;

    // all cards have these by default; these are the non-shape-based systematics
    //
    TString bckgrdsyst = Form("CMS_%s_norm_back_%dTeV",channame.Data(),beamcomenergytev); // channel-dependent
    TString signalsyst = Form("CMS_%s_eff_sig_%dTeV",channame.Data(),beamcomenergytev);   // channel-dependent

    char elormu = channame[ELORMUCHAR];
    //TString trigsyst   = "CMS_trigger_"+TString(elormu);
    TString leptsyst   = "CMS_eff_"+TString(elormu);

    systematics[bckgrdsyst]   = "lnN";
#ifdef ISHWW
    systematics[signalsyst]   = "lnN";
#endif //ISHWW
    //systematics[trigsyst]     = "lnN"; // common across same-flavor channels
    systematics[leptsyst]     = "lnN"; // common across same-flavor channels
  }

  // common systematics
  systematics["lumi_8TeV"]    = "lnN"; // common across channels

#ifdef ISHWW
  systematics["pdf_gg"]       = "lnN"; // common across channels for ggF process
#ifdef SEVENTEV
  systematics["pdf_qqbar"]    = "lnN"; // common across channels for VBF process
#endif
  systematics["QCDscale_ggH"]    = "lnN"; // common across channels for ggF process 2j bin
  systematics["QCDscale_ggH1in"] = "lnN"; // common across channels for ggF process 2j/3j bins
  systematics["QCDscale_ggH2in"] = "lnN"; // common across channels for ggF process 3j bin
  systematics["UEPS"]            = "lnN"; // common across channels for ggF process 2j/3j bins
  systematics["interf_ggH"]      = "shape"; // common across channels for ggF process 2j/3j bins
#ifdef SEVENTEV
  systematics["QCDscale_qqH"]  = "lnN"; // common across channels for VBF process
#endif
#endif // ISHWW

  std::map<TString,int>::const_iterator it;
  for (it=pname2index.begin(); it != pname2index.end(); it++) {
    TString procname = it->first;
    int index = it->second;

    ProcData_t& pd = processes[index];

    if( procname.Contains("data") ) return;

    if( procname.Contains("ignal")
#ifdef ISHWW
	|| (procname.Contains("ggH") ||
	    procname.Contains("qq") )
#endif //ISHWW
	) {                              // signal

      // insert signal lumi and xsec systematics now
#ifdef ISHWW

      pair<double,double> lumipair(0.0, 1+siglumiunc);

      pd.systrates["lumi_8TeV"].resize(nchan,lumipair);

      // down/up pairs to put in card
      pair<double,double> pdfunc,scaleunc0,scaleunc1,scaleunc2,scaleunc3,ueps0,ueps1; 
    
      makeTheoretUncert4Sig(massgev,procname,pdfunc,scaleunc0,scaleunc1,scaleunc2,scaleunc3,ueps0,ueps1);

      if (procname.Contains("qq") ) { // VBF process
	
	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc0);
      
      } else { // default gg fusion

	pd.systrates["pdf_gg"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_ggH"].resize(nchan,zeropair);
	pd.systrates["QCDscale_ggH1in"].resize(nchan,zeropair);
	pd.systrates["QCDscale_ggH2in"].resize(nchan,zeropair);
	pd.systrates["UEPS"].resize(nchan,zeropair);
      }
#endif //ISHWW

      int ichan=-1;
      for (int ichanref=0; ichanref<NUMCHAN; ichanref++) { //  depends on channames being lexically ordered!

	if (channels.find(channames[ichanref]) == channels.end()) {
	  //cout << "...skipping."  << endl;
	  continue;  // check if on the list of channels to be done
	}
      
	ichan++;

	char elormu = channames[ichanref][ELORMUCHAR];
	//TString trigsyst   = "CMS_trigger_"+TString(elormu);
	TString leptsyst   = "CMS_eff_"+TString(elormu);

	if (ichanref & 1) { // odd channel, 3jet bin
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc2;
	  pd.systrates["QCDscale_ggH2in"][ichan] = scaleunc3;
	  pd.systrates["UEPS"][ichan]            = ueps1;
	} else { // even channel, 2jet bin
	  pd.systrates["QCDscale_ggH"][ichan]    = scaleunc0;
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc1;
	  pd.systrates["UEPS"][ichan]            = ueps0;
	}

	//pd.systrates[trigsyst].resize(nchan,zeropair);
	//pd.systrates[trigsyst][ichan].second = 1+sigtrigeffunc;
	if (!pd.systrates[leptsyst].size())
	  pd.systrates[leptsyst].resize(nchan,zeropair);
	pd.systrates[leptsyst][ichan].second = 1+sqrt(siglepteffunc*siglepteffunc + sigtrigeffunc*sigtrigeffunc);

#ifdef ISHWW
	TString signalsyst = Form("CMS_%s_eff_sig_%dTeV",  channames[ichanref],beamcomenergytev);
	if (!pd.systrates[signalsyst].size())
	  pd.systrates[signalsyst].resize(nchan,zeropair);
	pd.systrates[signalsyst][ichan].second =
#ifdef SEVENTEV
	  1.0 + (massgev < 500 ? sigselefferrpctlomass : sigselefferrpcthimass)/100.;
#else
	  1.0 + (sigselefferrpct8tev)/100.;
#endif
#endif //ISHWW

      } // channel loop

    } else {                                                        //background
    
      int imass2use = findMasspt2use(massgev);

      int ichan=-1;
      for (int ichanref=0; ichanref<NUMCHAN; ichanref++) { //  depends on channames being lexically ordered!

	if (channels.find(channames[ichanref]) == channels.end()) {
	  //cout << "...skipping."  << endl;
	  continue;  // check if on the list of channels to be done
	}
      
	ichan++;

	TString bckgrdsyst = Form("CMS_%s_norm_back_%dTeV",channames[ichanref],beamcomenergytev);
    
	if ( pd.name.Contains("kgr") ) {
	  if (!pd.systrates[bckgrdsyst].size())
	    pd.systrates[bckgrdsyst].resize(nchan,zeropair);
	  int index = imass2use*NUMCHAN+ichanref;
	  //cout << imass2use << " " << ichanref << " " << ichan << " " << index << endl;
	  pd.systrates[bckgrdsyst][ichan].second = backnormerr[index];
	}
      } // channel loop

    } // if is background

  } // process loop

}                                                                // addSystematics

//================================================================================

// see documentation for higgs combine tool, data card file specification
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit
//
void Card::Print(int massgev,
		 const TString& cfgtag)
{
  using namespace std;

  int nbins= processes[0].channels.size();
  int jmax = processes.size()-1;

#if 0
  TString outdir;
  if (shapesfile.Contains("/"))
    outdir=shapesfile(0,shapesfile.Last('/'));
  else
    outdir=TString(".");
#endif

  TString fname;
  if (nbins==1) {
    TString channame = processes[0].channels.begin()->first;
    fname = Form("./datacard_%s_%s.txt",
		 //outdir.Data(),
		 channame.Data(),cfgtag.Data());
  } else
    fname = Form("./datacard_%s.txt",
		 //outdir.Data(),
		 cfgtag.Data());

  cout << "Writing "<<fname<<endl;

  FILE* dcFile=fopen(fname.Data(),"wt");
  //FILE* dcFile=stdout;

  assert (dcFile);

  fprintf(dcFile,"imax %d  # number of channels\n",nbins);
  fprintf(dcFile,"jmax %d  # number of processes-1\n",jmax);
  fprintf(dcFile,"kmax *  # number of nuisance parameters\n");

  fprintf(dcFile,"--------------------\n");

  // these are the data loops
  fprintf(dcFile,"bin                                ");
  for (map<TString,double>::const_iterator it=data.channels.begin();
       it != data.channels.end();
       it++) {
    fprintf(dcFile,"%15s",it->first.Data());
  }
  fprintf(dcFile,"\nobservation                        ");
  for (map<TString,double>::const_iterator it=data.channels.begin();
       it != data.channels.end();
       it++) {
    //fprintf(dcFile,"%15d",(int)it->second);
    fprintf(dcFile,"%15.5f",it->second);
  }
  
  fprintf(dcFile,"\n--------------------\n");

  if (shapespecs.size()) {
    for (size_t i = 0; i < shapespecs.size(); i++)
      fprintf(dcFile,"shapes %s %s %s %s %s\n",
	      shapespecs[i].process.Data(),
	      shapespecs[i].channel.Data(),
	      shapespecs[i].file.Data(),
	      shapespecs[i].histo.Data(),
	      shapespecs[i].histo_with_syst.Data());

    fprintf(dcFile,"--------------------\n");
  }

  /****************************************
   * YIELDS PER BIN/PROCESS
   ****************************************/

  //--------------------------------------------------------------------------------
#ifdef PROC_MAJOR_CHAN_MINOR_ORDERING

  // Example:
  //--------------------
  //bin           chan1    chan2    chan1   chan2   chan1    chan2
  //process       Signal   Signal   WJets   WJets     WW       WW
  //process         0        0        1       1        2        2  
  //--------------------

  // these are the signal and background loops
  fprintf(dcFile,"bin                                ");
  for (int j=0; j<=jmax; j++)
    for (map<TString,double>::const_iterator it=processes[j].channels.begin();
	 it != processes[j].channels.end();
	 it++)
      fprintf(dcFile,"%15s",it->first.Data());
  fprintf(dcFile,"\n");
  
  fprintf(dcFile,"process                            ");
  for (int j=0; j<=jmax; j++)
    for (int k=0; k<nbins; k++)
      fprintf(dcFile,"%15s",processes[j].name.Data());
  fprintf(dcFile,"\n");

  fprintf(dcFile,"process                            ");
  for (int j=0; j<=jmax; j++)
    for (int k=0; k<nbins; k++)
      fprintf(dcFile,"%15d",processes[j].procindex);
  fprintf(dcFile,"\n");

  fprintf(dcFile,"rate                               ");
  for (int j=0; j<=jmax; j++) 
    for (map<TString,double>::const_iterator it=processes[j].channels.begin();
	 it != processes[j].channels.end();
	 it++)
      fprintf(dcFile,"%15.2f",max(it->second,0.001));
  fprintf(dcFile,"\n");

  fprintf(dcFile,"--------------------\n");

  /****************************************
   * SYSTEMATICS
   ****************************************/

  map<TString,TString>::const_iterator sit;
  for (sit  = systematics.begin();
       sit != systematics.end();
       sit++) {
    const TString& name = sit->first;
    const TString& pdf  = sit->second;
    fprintf(dcFile,"%-32s %5s",name.Data(),pdf.Data());
    for (int j=0; j<=jmax; j++) {
      const ProcData_t& pd = processes[j];
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(sit->first);
      if (rit == pd.systrates.end()) {
	for (int k=0; k<nbins; k++)
	  fprintf(dcFile,"        -      ");
      } else {
	const vector<pair<double,double> >& rates = rit->second;
	for (int k=0; k<nbins; k++) {
	  pair<double,double> rate = rates.at(k);
	  if (rate.second == 0.0)        // expect the UP (second) to contain single-sided error values
	    fprintf(dcFile,"        -      ");
	  else if (rate.first == 0.0)
	    fprintf(dcFile,"  %10.4g   ",rate.second);
	  else
	    fprintf(dcFile," %g/%g ",rate.first,rate.second);
	}
      }
    }
    fprintf(dcFile,"\n");
  }

  //--------------------------------------------------------------------------------
#else // CHAN_MAJOR_PROC_MINOR_ORDERING

  // all processes must have the same channels!

  // Example:
  //--------------------
  //bin           chan1    chan1    chan1   chan2    chan2    chan2
  //process       Signal   WJets      WW    Signal   WJets      WW
  //process         0        1         2      0        1         2  
  //--------------------

  // these are the signal and background loops
  fprintf(dcFile,"bin                                ");
  for (map<TString,double>::const_iterator it=processes[0].channels.begin();
       it != processes[0].channels.end();
       it++)
    for (int j=0; j<=jmax; j++)
      fprintf(dcFile,"%15s",it->first.Data());
  fprintf(dcFile,"\n");
  
  fprintf(dcFile,"process                            ");
  for (int k=0; k<nbins; k++)
    for (int j=0; j<=jmax; j++)
      fprintf(dcFile,"%15s",processes[j].name.Data());
  fprintf(dcFile,"\n");

  fprintf(dcFile,"process                            ");
  for (int k=0; k<nbins; k++)
    for (int j=0; j<=jmax; j++)
      fprintf(dcFile,"%15d",processes[j].procindex);
  fprintf(dcFile,"\n");

  fprintf(dcFile,"rate                               ");
  for (map<TString,double>::const_iterator chit=processes[0].channels.begin();
       chit != processes[0].channels.end();
       chit++) {
    TString channame = chit->first;
    for (int j=0; j<=jmax; j++) {
      map<TString,double>::const_iterator it=processes[j].channels.find(channame);
      if (it==processes[j].channels.end()) {
	cerr << "couldn't find channel " << channame << " for process " << j << endl;
	exit(-1);
      }
      fprintf(dcFile,"%15.2f",max(it->second,0.001));
    }
  }
  fprintf(dcFile,"\n");

  fprintf(dcFile,"--------------------\n");

  /****************************************
   * SYSTEMATICS
   ****************************************/

  map<TString,TString>::const_iterator sit;
  for (sit  = systematics.begin();
       sit != systematics.end();
       sit++) {
    const TString& name = sit->first;
    const TString& pdf  = sit->second;
    fprintf(dcFile,"%-32s %5s",name.Data(),pdf.Data());
    for (int k=0; k<nbins; k++) {
      for (int j=0; j<=jmax; j++) {
	const ProcData_t& pd = processes[j];
	map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(sit->first);
	if (rit == pd.systrates.end()) {
	  fprintf(dcFile,"        -      ");
	} else {
	  const vector<pair<double,double> >& rates = rit->second;
	  pair<double,double> rate = rates.at(k);
	  if (rate.second == 0.0)        // expect the UP (second) to contain single-sided error values
	    fprintf(dcFile,"        -      ");
	  else if (rate.first == 0.0)
	    fprintf(dcFile,"  %10.4g   ",rate.second);
	  else
	    fprintf(dcFile," %g/%g ",rate.first,rate.second);
	}
      }
    }
    fprintf(dcFile,"\n");
  }

#endif

  fclose(dcFile);
}                                                               // fmtDataCardFile
