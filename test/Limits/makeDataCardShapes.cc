/*
makeLimitFile

Some important constants are set at the top of the file.
*/
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TDirectory.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TKey.h"
#include "TClass.h"

using namespace std;

#include "dcutils.C"

#include "hwwinputs.h"
//#include "hwwinputs_rectcut2.h"
#include "hwwutils.C"
//#include "mjjinputs.h"

//================================================================================

int
tokNameGetInfo(const TString& hname,
	       TString& procname,
	       TString& systname,
	       int&     ichan,
	       int&     imass)
{
  char name[128];

  strcpy(name,hname.Data());

  // use "strtok" because I can stop tokenizing in mid-string
  //
  char *tok = strtok(name,"_");  assert(tok);

  procname = TString(tok);

  TString channame;
  if (!procname.CompareTo("data")) {
    strtok(NULL,"_"); // skip "obs"
  }
  tok = strtok(NULL,"_");  assert(tok);
  channame = TString(tok);

  for (ichan=0; ichan<NUMCHAN; ichan++)
    if (!strcmp(channames[ichan],channame.Data())) break;
  if (ichan>=NUMCHAN) {
    cerr << "channel " << channame << "not found, skipping" << endl;
    return 0;
  }

  // if there's a mass embedded in the name it has to be now:
  if (hname.Contains("Mass")) {
    tok = strtok(NULL,"_");    assert(tok); assert(!strcmp(tok,"Mass"));
    tok = strtok(NULL,"_");    assert(tok); int massgev = atoi(tok);

    for (imass=0;massgev!=masspts[imass] && imass<NUMMASSPTS; imass++) ;
    if (imass>=NUMMASSPTS) {
      cerr << "mass point M=" << massgev << " GeV not found, skipping" << endl;
      return 0;
    }
  }

  tok = strtok(NULL,""); // the rest of the string may be systematic

  if (tok) {
    TString test(tok);
    if (test.EndsWith("Up")) { // This is a histogram with a systematic applied
      systname = test.Chop().Chop();
    }
  }

  cout << "Read process=" << setw(10) << procname << ", channel=" << channame << ", mass=" << masspts[imass];
  if (systname.Length()) cout << ", systname = " << systname;

  return 1;
}                                                                // tokNameGetInfo

//================================================================================

CardData_t 
makeNewCard(TH1           *inhist,
	    const TString& procname,
	    const TString& systname,
	    const int      imass,
	    const int      ichanref,
	    const int      ichan,
	    const int      nchan
	    )
{
  CardData_t card;

  card.nbackproc = 0;
  card.nsigproc  = 0;

  cout << "\tmake new card ";

  const pair<double,double> zeropair(0.0,0.0);

  // all cards have these by default; these are the non-shape-based systematics
  //
  TString bckgrdsyst = "CMS_"+TString(channames[ichanref])+"_norm_back"; // channel-dependent
  TString signalsyst = "CMS_"+TString(channames[ichanref])+"_eff_sig";   // channel-dependent

  char elormu = channames[ichanref][0];
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);

  card.systematics[bckgrdsyst]     = "lnN";
  card.systematics[trigsyst]       = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]       = "lnN"; // common across same-flavor channels
  card.systematics["lumi"]         = "lnN"; // common across channels
#ifdef ISHWW
  card.systematics[signalsyst]     = "lnN";
  card.systematics["pdf_gg"]       = "lnN"; // common across channels for ggF process
  card.systematics["pdf_qqbar"]    = "lnN"; // common across channels for VBF process
  
  card.systematics["QCDscale_ggH"] = "lnN"; // common across channels for ggF process
  card.systematics["QCDscale_qqH"] = "lnN"; // common across channels for VBF process

  if (masspts[imass] >= 300) {
    // add "heavy Higgs" uncertainty
    // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
    card.systematics["theoryUncXS_HighMH"] = "lnN";
  }
#endif

  ProcData_t pd;

  pair<TString,double> channel;

  pd.name        = procname;
  channel.first  = channames[ichanref];
  channel.second = inhist->Integral(); // the yield for this channel

  pd.channels.insert(channel);

  if( procname.Contains("data") ) {
    card.data = pd; // done.
  } else {
    int index=0;

    if( procname.Contains("ignal")
#ifdef ISHWW
	|| (procname.Contains("ggH") ||
	    procname.Contains("qq") )
#endif
	) {                              // signal

      card.processes.resize(1);
      card.nsigproc++;

      pd.procindex = 1-card.nsigproc; /* process index for signal processes required to be distinct,
					 as well as 0 or negative */

      assert(!systname.Length()); // shape based systematics for signal not implemented yet

      // insert signal lumi and xsec systematics now
#ifdef ISHWW

      pair<double,double> pdfunc, scaleunc;   // down/up pair to put in card

      makeTheoretUncert4Sig(imass, procname, pdfunc, scaleunc);

      if (procname.Contains("qq") ) { // VBF process

	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc);

      } else { // default gg fusion

	pd.systrates["pdf_gg"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_ggH"].resize(nchan,scaleunc);
      }

      int massgev = masspts[imass];

      if (massgev >= 300) {
	// add "heavy Higgs" uncertainty
	// https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
	pair<double,double> highmassunc( 0.0, 1.0 + (1.5*pow(massgev/1000.,3)) );
	
	pd.systrates["theoryUncXS_HighMH"].resize(nchan,highmassunc);
      }
#endif
      pair<double,double> lumipair(0.0, 1+siglumiunc);

      pd.systrates["lumi"].resize(nchan,lumipair);
      pd.systrates[trigsyst].resize(nchan,zeropair);
      pd.systrates[trigsyst][ichan].second = 1+sigtrigeffunc;
      pd.systrates[leptsyst].resize(nchan,zeropair);
      pd.systrates[leptsyst][ichan].second = 1+siglepteffunc;
#ifdef ISHWW
      pd.systrates[signalsyst].resize(nchan,zeropair);
      pd.systrates[signalsyst][ichan].second =
	1.0 + (sigselefferrpct[imass*NUMCHAN+ichanref]/100.);
#endif

    } else {                                                        //background
      card.nbackproc++;
      pd.procindex = card.nbackproc;

      // must reserve index 0 for signal,
      // so give this process index 1
      index = 1;
      card.processes.resize(2);

      if ( pd.name.Contains("kgr") ) {
	pd.systrates[bckgrdsyst].resize(nchan,zeropair);
	pd.systrates[bckgrdsyst][ichan].second = backnormerr[imass*NUMCHAN+ichanref];
      }
    }
	
    if (systname.Length()) {
      card.systematics[systname] = "shape";

      pd.systrates[systname].resize(nchan,zeropair); /* shape-based systematic,
							  assume all channels uncorrelated,
							  so all channels get factor 0.0, */
      pd.systrates[systname][ichan].second = 1.0;     // except the current channel
    }
    card.pname2index[procname]= index;
    card.processes[index]    = pd;

  } // else not data

  return card;
}                                                                   // makeNewCard

//================================================================================

void
addToCard(CardData_t&    card,
	  TH1           *inhist,   // input histogram
	  const TString& procname, // process name 
	  const TString& systname, // name of (shape) systematic applied
	  const int      imass,    // mass index
	  const int      ichanref, // channel reference index
	  const int      ichan,    // channel index
	  const int      nchan     // number of channels in card
	  )
{
  cout << "\tadd to card ";

  // With each input histogram one must:
  // 1. If a new process, update the process info in the card: "card.processes", "card.pname2index",
  // 2. Update new channel information "card.processes[procindex].channels" (channel name/rate pairs)
  // 3. Update the systematics information as necessary:
  //    if systname.length, add shape systematic information to
  //    card.systematics (if new systematic), card.processes[procindex].systrates[systname][ichan]
  // 
  pair<TString,double> channel;

  const pair<double,double> zeropair(0.0,0.0);

  TString channame(channames[ichanref]);

  channel.first  = channame;
  channel.second = inhist->Integral();

  TString bckgrdsyst = "CMS_"+channame+"_norm_back"; // channel-dependent
  TString signalsyst = "CMS_"+channame+"_eff_sig";   // channel-dependent

  char elormu = channame[0];
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);

  card.systematics[bckgrdsyst]   = "lnN";
#ifdef ISHWW
  card.systematics[signalsyst]   = "lnN";
#endif
  card.systematics[trigsyst]     = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]     = "lnN"; // common across same-flavor channels

  int index=0;
  if( procname.Contains("data") )         // data observation

    if (!card.data.name.Length()) {             // first channel of data encountered
      ProcData_t pd;
      pd.name = procname;
      pd.channels.insert(channel);
      card.data = pd;
    }else{                                      // another channel for data, possibly
      assert(!card.data.name.CompareTo(procname)); // must be the same
      card.data.channels.insert(channel);
    }

  else if( procname.Contains("ignal")
#ifdef ISHWW
	    || (procname.Contains("ggH") ||
		procname.Contains("qq") )
#endif
	   ) { // signal expectation

    assert(card.processes.size());  // space for index 0 s/b reserved by now
    assert(!systname.Length());     // shape based systematics for signal not implemented yet

    pair<double,double> pdfunc,scaleunc;
#ifdef ISHWW
      makeTheoretUncert4Sig(imass, procname, pdfunc, scaleunc);
#endif
    pair<double,double> lumipair(0.0, 1+siglumiunc);

    map<TString,int>::iterator pit;
    pit = card.pname2index.find(procname);
    if (pit == card.pname2index.end()) {    // new signal process, first channel encountered
      card.nsigproc++;

      ProcData_t pd;
      pd.name      = procname;
      pd.procindex = 1-card.nsigproc; /* process index for signal processes required to be distinct,
					 as well as 0 or negative */

      pd.channels.insert(channel);
      index = card.processes.size();
      card.pname2index[pd.name] = index;

      pd.systrates["lumi"].resize(nchan,lumipair);
      pd.systrates[trigsyst].resize(nchan,zeropair);
      pd.systrates[trigsyst][ichan].second = 1.0+sigtrigeffunc;
      pd.systrates[leptsyst].resize(nchan,zeropair);
      pd.systrates[leptsyst][ichan].second = 1+siglepteffunc;
#ifdef ISHWW
      pd.systrates[signalsyst].resize(nchan,zeropair);
      pd.systrates[signalsyst][ichan].second = 1.0 + (sigselefferrpct[imass*NUMCHAN+ichanref]/100.);

      if (procname.Contains("qq") ) {
	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc);
      } else {
	pd.systrates["pdf_gg"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_ggH"].resize(nchan,scaleunc);
      }

      int massgev=masspts[imass];
      if (massgev >= 300) {
	// add "heavy Higgs" uncertainty
	// https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
	pair<double,double> highmassunc( 0.0, 1.0 + (1.5*pow(massgev/1000.,3)) );
	
	pd.systrates["theoryUncXS_HighMH"].resize(nchan,highmassunc);
      }
#endif

      card.processes.push_back(pd);

    }else {                                // another channel for signal, presumably
      ProcData_t& pd = card.processes[pit->second];
      assert(pd.name.Length());             // process should exist here
      assert(!pd.name.CompareTo(procname)); // must be the same
      pd.channels.insert(channel);

      // has this signal systematic been initialized for this process?
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(trigsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[trigsyst].resize(nchan,zeropair);
      }
      pd.systrates[trigsyst][ichan].second   = 1.0 + sigtrigeffunc;

      // has this signal systematic been initialized for this process?
      rit = pd.systrates.find(leptsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[leptsyst].resize(nchan,zeropair);
      }
      pd.systrates[leptsyst][ichan].second   = 1.0 + siglepteffunc;

#ifdef ISHWW
      // has this signal systematic been initialized for this process?
      rit = pd.systrates.find(signalsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[signalsyst].resize(nchan,zeropair);
      }
      pd.systrates[signalsyst][ichan].second = 1.0 + (sigselefferrpct[imass*NUMCHAN+ichanref]/100.);

      if (procname.Contains("qq") ) { // vbf process
	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc);
      } else {
	pd.systrates["pdf_gg"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_ggH"].resize(nchan,scaleunc);
      }
#endif
    }

  } else {                               // background process

    map<TString,int>::iterator pit;
    pit = card.pname2index.find(procname);
    if (pit == card.pname2index.end()) {    // first channel of this background process encountered
      card.nbackproc++;

      ProcData_t pd;
      pd.name = procname;
      pd.procindex = card.nbackproc;
      pd.channels.insert(channel);
      index = card.processes.size();
      card.pname2index[pd.name] = index;

      if ( pd.name.Contains("kgr") ) {
	pd.systrates[bckgrdsyst].resize(nchan,zeropair);
	pd.systrates[bckgrdsyst][ichan].second = backnormerr[imass*NUMCHAN+ichanref];
      }

      card.processes.push_back(pd);

    } else {                                /* either another channel, or a shape
					       systematic for an existing channel */
      ProcData_t& pd = card.processes[pit->second];

      if (systname.Length()) { // a shape-based limit, all channels get a factor of 0.0
	card.systematics[systname] = "shape";
	pd.systrates[systname].resize(nchan,zeropair);
	pd.systrates[systname][ichan].second = 1.0; // except the current channel
      }
      else {
	assert(pd.name.Length());             // process should exist here
	assert(!pd.name.CompareTo(procname)); // and be named the same
	pd.channels.insert(channel);

	if ( pd.name.Contains("kgr") ) {
	  // has this background systematic been initialized for this process?
	  map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(bckgrdsyst);
	  if (rit == pd.systrates.end()) { //  no
	    pd.systrates[bckgrdsyst].resize(nchan,zeropair);
	  }
	  pd.systrates[bckgrdsyst][ichan].second = backnormerr[imass*NUMCHAN+ichanref];
	}
      }
    }
  }
}                                                                     // addToCard

//================================================================================

void
makeDataCardContent(TFile *fp, map<int,CardData_t>& m_cards,
		    const vector<int>& channellist)
{
  TIter nextkey( gDirectory->GetListOfKeys() );
  TKey *key;

  cout << "Looking for channels ";
  for (size_t i=0; i<channellist.size(); i++) {
    cout << channames[channellist[i]] << " ";
  }
  cout << endl;

  // loop through objects in the input root file and find histograms
  // that are shape inputs into the limit setting data card
  //
  while ( (key = (TKey*)nextkey())) {

    TObject *obj = key->ReadObj();

    if ( !obj->IsA()->InheritsFrom( "TH1" ) )
      continue;

    TH1 *h1 = (TH1*)obj;
    TString hname(h1->GetName());

    if (hname.EndsWith("Down")) continue; // skip the "Down"s, expect a matching "Up"

    // tokenize the histo name to determine
    // 1. process name
    // 2. channel name
    // 3. mass point
    // 4. any systematic applied
    //
    TString procname(""),systname(""),channname("");
    int ichanref=0,imass=0;

    if (!tokNameGetInfo(hname, procname,systname,ichanref,imass)) {
      cout << "...skipping."  << endl;
      continue;
    }

    size_t ichan;
    for (ichan=0; ichan<channellist.size(); ichan++)
      if (ichanref == channellist[ichan])
	break;
    if (ichan==channellist.size()) {
      cout << "...skipping."  << endl;
      continue;  // check if on the list of channels to be done
    }
    int massgev=masspts[imass];

    // insert into existing cards data map:

    map<int,CardData_t>::iterator it = m_cards.find(massgev);
    if (it == m_cards.end()) {                                    // new masspoint

      m_cards[massgev] = makeNewCard(h1,procname,systname,
				     imass,ichanref,ichan,channellist.size());

    } else {                                                 // existing masspoint

      addToCard(it->second,h1,procname,systname,imass,ichanref,ichan,channellist.size());

    } // existing masspoint


    cout  << endl;
  } // key loop
}                                                           // makeDataCardContent

//================================================================================

void
makeDataCardFiles(int argc, char*argv[])
{
  TString infname=TString(argv[1]);//"hww-histo-shapes-putconfiginfohere.root")
  TFile *fp = new TFile(infname);

  TString cfgtag;

#ifdef ISHWW
  if (infname.Contains("hww-histo-shapes-")) {
#else
  if (infname.Contains("mjj-histo-shapes-")) {
#endif
    if (infname.Contains("/"))
      cfgtag = infname(infname.Last('/')+18,infname.Last('.')-infname.Last('/')-18);
    else
      cfgtag = infname(17,infname.Last('.')-17);
    cout << "cfgtag = " << cfgtag << endl;
  }
  
  if (fp->IsZombie()) {
    cerr << "Couldn't open file " << string(infname) << endl;
    exit(-1);
  }

  vector<int> channellist;
  for (int ichan=0; ichan<NUMCHAN; ichan++)
    if (argc > 2) {
      for( int j=2; j<argc; j++ )
	if (!strcmp(argv[j],channames[ichan])) {
	  channellist.push_back(ichan);
	  break;
	}
    } else
      channellist.push_back(ichan);
  
  if (!channellist.size()) {
    cerr << " no known channels found, buh-bye!" << endl;
    exit(-1);
  }

#ifdef ISHWW
  readHxsTable   ("ggHtable7tev.txt");  // to get the signal x-sec uncertainties
  readHxsTable   ("vbfHtable7tev.txt"); // to get the signal x-sec uncertainties
#endif

  map<int,CardData_t> m_cards;
  makeDataCardContent(fp,m_cards,channellist);

  map<int,CardData_t>::iterator it;
  for (it=m_cards.begin();
       it!=m_cards.end();
       it++) {

#ifdef ISHWW
    it->second.shapespecs.push_back
      (ShapeFiles_t("*","*",infname,
		    "$PROCESS_$CHANNEL_Mass_$MASS", 
		    "$PROCESS_$CHANNEL_Mass_$MASS_$SYSTEMATIC")
       );
#else
    it->second->shapespecs.push_back
      (ShapeFiles_t("*","*",infname,
		    "$PROCESS_$CHANNEL",
		    "$PROCESS_$CHANNEL_$SYSTEMATIC")
       );
#endif

    fmtDataCardFile(it->first,it->second,cfgtag);
  }
  
}                                                             // makeDataCardFiles

#ifdef MAIN
//================================================================================

#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc<2) {
    printf("Usage: %s input.root\n",argv[0]);
    return 1;
  }
#ifdef DEBUG
  for (int i=0; i<argc; i++) printf("%s ", argv[i]);
  printf ("\n");
#endif

  //hwwshapes(argv[1],"hww-histo-shapes-TH1.root");

  makeDataCardFiles(argc, argv); // "hww-histo-shapes-TH1.root");
  return 0;
}
#endif
