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
#include <climits>
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
// routine reads a text table with yields as follows:
// headerline proc1 proc2 proc3...
//  xx        1.23  4.56  7.89 ...
//
// and "transposes" it into a vector of column vectors headed by the process names
// xx must be expendable, will be overwritten with the channel name.
// 
// This routine can be called multiple times to fill the same 2d-vector
// as long as the text files have identical formatting.
//
void
readTextTable(FILE *fp, 
	      TString channame,
	      std::vector<std::vector<TString> >& columns) /* columns are processes,
							      rows are channels */
{
  char instr[128];

  // convert table to vector of column vectors, preserves ordering
  for (int iline=0; !feof(fp) && fgets(instr,128,fp); iline++) {
    TString line(instr); line.Chop(); // remove '\n'
    TObjArray *tokens = line.Tokenize(" ");

    if (!columns.size()) { // first line must contain process names

      assert(tokens->GetEntriesFast() >= 2);
      for (int i=0; i<tokens->GetEntriesFast(); i++) {
	std::vector<TString> colvec;
	TString pname = ((TObjString *)(*tokens)[i])->GetString();
	//cout << pname << endl;
	colvec.push_back(pname);
	columns.push_back(colvec);
      }

    } else if (iline) {
      //cout << tokens->GetEntriesFast() << " " << columns.size() << endl;
      assert(tokens->GetEntriesFast() == (int)columns.size());
      columns[0].push_back(channame);
      for (int i=1; i<tokens->GetEntriesFast(); i++) {
	TString val = ((TObjString *)(*tokens)[i])->GetString();
	columns[i].push_back(val);
      }
    }

    delete tokens;
  }
  fclose(fp);

  assert(columns.at(0).size() > 1); // should have at least one data row

}                                                                 // readTextTable

//================================================================================

CardData_t 
makeNewCard(double         yield,
	    const TString& procname,
	    const int      imass,
	    const bool     isinterp, // interpolated mass point
	    const int      ichanref,
	    const int      ichan,
	    const int      nchan
	    )
{
  CardData_t card;

  card.nbackproc = 0;
  card.nsigproc  = 0;

  TString channame(channames[ichanref]);
  int massgev = isinterp ? interpolatedmasspts[imass] : masspts[imass];

  cout<<"Read process="<<setw(10)<<procname<<", channel="<<channame<<", mass="<<massgev;
  if (isinterp) cout << " (interpolated)";

  cout << "\tmake new card ";

  const pair<double,double> zeropair(0.0,0.0);

  // all cards have these by default
  //

  char elormu = channame[ELORMUCHAR];
  //TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  double leptsystrate = 1+sqrt(siglepteffunc*siglepteffunc + sigtrigeffunc*sigtrigeffunc);

  //card.systematics[trigsyst]       = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]       = "lnN"; // common across same-flavor channels
  card.systematics["lumi"]         = "lnN"; // common across channels
#ifdef ISHWW
  card.systematics["pdf_gg"]       = "lnN"; // common across channels for ggF process
#ifdef SEVENTEV
  card.systematics["pdf_qqbar"]    = "lnN"; // common across channels for VBF process
#endif
  card.systematics["QCDscale_ggH"]    = "lnN"; // common across channels for ggF process 2j bin
  card.systematics["QCDscale_ggH1in"] = "lnN"; // common across channels for ggF process 2j/3j bins
  card.systematics["QCDscale_ggH2in"] = "lnN"; // common across channels for ggF process 3j bin
  card.systematics["UEPS"]            = "lnN"; // common across channels for ggF process 2j/3j bins
#ifdef SEVENTEV
  card.systematics["QCDscale_qqH"]  = "lnN"; // common across channels for VBF process
#endif


  if (massgev >= 300) {
    // add "heavy Higgs" uncertainty
    // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
    card.systematics["theoryUncXS_HighMH"] = "lnN";
  }
#endif

  ProcData_t pd;

  pair<TString,double> channel;

  pd.name        = procname;
  channel.first  = channame;
  channel.second = yield;

  pd.channels.insert(channel);

  if( procname.Contains("data", TString::kIgnoreCase) ) {
    card.data = pd; // done.
  } else {

    if( procname.Contains("ignal")
#ifdef ISHWW
	|| (procname.Contains("ggH") ||
	    procname.Contains("qq") )
#endif
	) {                              // signal

      card.nsigproc++;

      pd.procindex = 1-card.nsigproc; /* process index for signal processes required to be distinct,
					 as well as 0 or negative */

      // insert signal lumi and xsec systematics now
#ifdef ISHWW

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
	pd.systrates["QCDscale_ggH"][ichan]    = scaleunc0;
	pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc1;
	pd.systrates["UEPS"][ichan]            = ueps0;
      }

      if (massgev >= 300) {
	// add "heavy Higgs" uncertainty
	// https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
	pair<double,double> highmassunc( 0.0, 1.0 + (1.5*pow(massgev/1000.,3)) );
	
	pd.systrates["theoryUncXS_HighMH"].resize(nchan,highmassunc);
      }
#endif
      pair<double,double> lumipair(0.0, 1+siglumiunc);

      pd.systrates["lumi"].resize(nchan,lumipair);

      //pd.systrates[trigsyst].resize(nchan,zeropair);
      //pd.systrates[trigsyst][ichan].second = 1+sigtrigeffunc;
      pd.systrates[leptsyst].resize(nchan,zeropair);

      pd.systrates[leptsyst][ichan].second = leptsystrate;

    } else {                                                        //background
      card.nbackproc++;
      pd.procindex = card.nbackproc;

      // cross-section uncertainty applies to all processes (except QCD) and channels 
      TString bckgrdsyst = Form("CMS_%s_xs_unc_%dTeV",procname.Data(),beamcomenergytev); // channel-dependent;
      card.systematics[bckgrdsyst] = "lnN";

      double xsunc=1.0;
      if      (!procname.CompareTo("WJet"))      xsunc = 1.0+ (1558./31314.);
      else if (!procname.CompareTo("TTbar"))     xsunc = 1.0+ (10./165.);
      else if (!procname.CompareTo("Diboson"))   xsunc = 1.0+ (0.7/18.2);
      else if (!procname.CompareTo("SingleTop")) xsunc = 1.0+ (3.5/79.8);
      else if (!procname.CompareTo("ZJet"))      xsunc = 1.0+ (132./3048.);
      else if (!procname.CompareTo("QCD"))       xsunc = 1.5;

      const pair<double,double> xsuncpair(0.0,xsunc);

      pd.systrates[bckgrdsyst].resize(nchan,xsuncpair);

      if (xsunc > 1.0) {      // lumi and eff's also apply:
	pair<double,double> lumipair(0.0, 1+siglumiunc);
	pd.systrates["lumi"].resize(nchan,lumipair);
	pd.systrates[leptsyst].resize(nchan,zeropair);
	pd.systrates[leptsyst][ichan].second = leptsystrate;
      }
    }
      
    card.processes.push_back(pd);
    card.pname2index[procname]= 0;

  } // else not data


  return card;
}                                                                   // makeNewCard

//================================================================================

void
addToCard(CardData_t&    card,
	  double         yield,
	  const TString& procname, // process name 
	  const int      imass,    // mass index
	  const bool     isinterp, // interpolated mass point
	  const int      ichanref, // channel reference index
	  const int      ichan,    // channel index
	  const int      nchan     // number of channels in card
	  )
{
  TString channame(channames[ichanref]);
  int massgev = isinterp ? interpolatedmasspts[imass] : masspts[imass];

  cout<<"Read process="<<setw(10)<<procname<<", channel="<<channame<<", mass="<<massgev;
  if (isinterp) cout << " (interpolated)";

  cout << "\tadd to card ";

  // With each invocation one must:
  // 1. If a new process, update the process info in the card: "card.processes", "card.pname2index",
  // 2. Update new channel information "card.processes[procindex].channels" (channel name/rate pairs)
  // 
  pair<TString,double> channel;

  const pair<double,double> zeropair(0.0,0.0);

  channel.first  = channame;
  channel.second = yield;

  char elormu = channame[ELORMUCHAR];
  //TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  double leptsystrate = 1+sqrt(siglepteffunc*siglepteffunc + sigtrigeffunc*sigtrigeffunc);
  
  //card.systematics[trigsyst]     = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]     = "lnN"; // common across same-flavor channels

  if( procname.Contains("data", TString::kIgnoreCase) )         // data observation

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

    pair<double,double> pdfunc,scaleunc0,scaleunc1,scaleunc2,scaleunc3,ueps0,ueps1; 
#ifdef ISHWW
    makeTheoretUncert4Sig(massgev,procname,pdfunc,scaleunc0,scaleunc1,scaleunc2,scaleunc3,ueps0,ueps1);
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

      pd.systrates["lumi"].resize(nchan,lumipair);
      //pd.systrates[trigsyst].resize(nchan,zeropair);
      //pd.systrates[trigsyst][ichan].second = 1.0+sigtrigeffunc;
      pd.systrates[leptsyst].resize(nchan,zeropair);
      pd.systrates[leptsyst][ichan].second = leptsystrate;

      if (procname.Contains("qq") ) {
	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc0);
      } else {
	pd.systrates["pdf_gg"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_ggH"].resize(nchan,zeropair);
	pd.systrates["QCDscale_ggH1in"].resize(nchan,zeropair);
	pd.systrates["QCDscale_ggH2in"].resize(nchan,zeropair);
	pd.systrates["UEPS"].resize(nchan,zeropair);
	if (ichanref & 1) { // odd channel, 3jet bin
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc2;
	  pd.systrates["QCDscale_ggH2in"][ichan] = scaleunc3;
	  pd.systrates["UEPS"][ichan]            = ueps1;
	} else { // even channel, 2jet bin
	  pd.systrates["QCDscale_ggH"][ichan]    = scaleunc0;
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc1;
	  pd.systrates["UEPS"][ichan]            = ueps0;
	}
      }

      if (massgev >= 300) {
	// add "heavy Higgs" uncertainty
	// https://twiki.cern.ch/twiki/bin/view/LHCPhysics/HeavyHiggs
	pair<double,double> highmassunc( 0.0, 1.0 + (1.5*pow(massgev/1000.,3)) );
	
	pd.systrates["theoryUncXS_HighMH"].resize(nchan,highmassunc);
      }

      // put new signal in front, have to adjust the mapped indices for all the rest.
      // This maintains the proper ordering of processes in the datacard (for LandS)
      //
      for (pit = card.pname2index.begin(); pit != card.pname2index.end(); pit++)
	pit->second++;

      card.processes.push_front(pd);

      card.pname2index[pd.name] = 0;

    } else {                                // another channel for signal, presumably
      ProcData_t& pd = card.processes[pit->second];
      assert(pd.name.Length());             // process should exist here
      assert(!pd.name.CompareTo(procname)); // must be the same
      pd.channels.insert(channel);

#if 0
      // has this signal systematic been initialized for this process?
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(trigsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[trigsyst].resize(nchan,zeropair);
      }
      pd.systrates[trigsyst][ichan].second   = 1.0 + sigtrigeffunc;
#endif
      // has this signal systematic been initialized for this process?
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(leptsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[leptsyst].resize(nchan,zeropair);
      }
      pd.systrates[leptsyst][ichan].second = leptsystrate;

      if (procname.Contains("qq") ) { // vbf process
	pd.systrates["pdf_qqbar"].resize(nchan,pdfunc);
	pd.systrates["QCDscale_qqH"].resize(nchan,scaleunc0);
      } else {
	if (ichanref & 1) { // odd channel, 3jet bin
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc2;
	  pd.systrates["QCDscale_ggH2in"][ichan] = scaleunc3;
	  pd.systrates["UEPS"][ichan]            = ueps1;
	} else { // even channel, 2jet bin
	  pd.systrates["QCDscale_ggH"][ichan]    = scaleunc0;
	  pd.systrates["QCDscale_ggH1in"][ichan] = scaleunc1;
	  pd.systrates["UEPS"][ichan]            = ueps0;
	}
      }
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

      // cross-section uncertainty applies to all processes (except QCD) and channels 
      TString bckgrdsyst = Form("CMS_%s_xs_unc_%dTeV",procname.Data(),beamcomenergytev); // channel-dependent;
      card.systematics[bckgrdsyst] = "lnN";

      double xsunc=1.0;
      if      (!procname.CompareTo("WJet"))      xsunc = 1.0+ (1558./31314.);
      else if (!procname.CompareTo("TTbar"))     xsunc = 1.0+ (10./165.);
      else if (!procname.CompareTo("Diboson"))   xsunc = 1.0+ (0.7/18.2);
      else if (!procname.CompareTo("SingleTop")) xsunc = 1.0+ (3.5/79.8);
      else if (!procname.CompareTo("ZJet"))      xsunc = 1.0+ (132./3048.);
      else if (!procname.CompareTo("QCD"))       xsunc = 1.5;

      const pair<double,double> xsuncpair(0.0,xsunc);

      pd.systrates[bckgrdsyst].resize(nchan,xsuncpair);

      if (xsunc > 1.0) {      // lumi and eff's also apply:
	pair<double,double> lumipair(0.0, 1+siglumiunc);
	pd.systrates["lumi"].resize(nchan,lumipair);
	pd.systrates[leptsyst].resize(nchan,zeropair);
	pd.systrates[leptsyst][ichan].second = leptsystrate;
      }

      card.pname2index[pd.name] = (int)card.processes.size();
      card.processes.push_back(pd);

    } else {                                /* either another channel, or a shape
					       systematic for an existing channel */
      ProcData_t& pd = card.processes[pit->second];

      assert(pd.name.Length());             // process should exist here
      assert(!pd.name.CompareTo(procname)); // and be named the same
      pd.channels.insert(channel);

      // has the efficiency systematic been initialized for this process?
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(leptsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[leptsyst].resize(nchan,zeropair);
      }
      pd.systrates[leptsyst][ichan].second = leptsystrate;

    }
  }
}                                                                     // addToCard

//================================================================================

void
makeDataCardContent(int massgev,
		    const std::vector<std::vector<TString> >& columns,
		    const vector<int>& channellist,
		    map<int,CardData_t>& m_cards)
{

  cout << "Looking for channels ";
  for (size_t i=0; i<channellist.size(); i++) {
    cout << channames[channellist[i]] << " ";
  }
  cout << endl;

  int imass=0;
  bool isinterp;
  for (imass=0;massgev!=masspts[imass] && imass<NUMMASSPTS; imass++) ;
  if (imass<NUMMASSPTS) {
    isinterp=false;
  } else {
    for (imass=0;massgev!=interpolatedmasspts[imass]; imass++) 
      if (interpolatedmasspts[imass] < 0) {
	cerr << "mass point M=" << massgev << " GeV not found, skipping" << endl;
	return;
      }
    isinterp=true;
  }

  // Now convert vector of vectors to meaningful card data.

  // First column (after first row) assumed to be channel names
  // First row (after first column) assumed to be process names
  //
  for (size_t ic=1; ic < columns.size(); ic++) { // iterate over process names

    TString procname = columns.at(ic).at(0);

    if (procname.Contains("total",TString::kIgnoreCase))
      continue;

    // get the channel names and event counts from all the rows

    for (size_t ir=1; ir < columns.at(0).size(); ir++) {
      TString channame=columns.at(0).at(ir);

      double yield = columns.at(ic).at(ir).Atof(); // event count

      int ichanref;
      for (ichanref=0; ichanref<NUMCHAN; ichanref++)
	if (!channame.CompareTo(channames[ichanref])) break;
      if (ichanref>=NUMCHAN) {
	cerr << "channel " << channame << "not found, skipping" << endl;
	return;
      }
      size_t  ichan;
      for (ichan=0; ichan<channellist.size(); ichan++)
	if (ichanref == channellist[ichan])
	  break;
      if (ichan==channellist.size()) {
	cout << "...skipping."  << endl;
	continue;  // check if on the list of channels to be done
      }

      // insert into existing cards data map:

      map<int,CardData_t>::iterator it = m_cards.find(massgev);
      if (it == m_cards.end()) {                                    // new masspoint

	m_cards[massgev] = makeNewCard(yield,procname,
				       imass,isinterp,ichanref,ichan,channellist.size());

      } else {                                                 // existing masspoint

	addToCard(it->second,yield,procname,imass,isinterp,ichanref,ichan,channellist.size());
      }

      cout  << endl;

    } // row loop
  } // column loop
}                                                           // makeDataCardContent

//================================================================================

void
makeDataCardFile(int argc, char *argv[])
{
  int massgev = atoi(argv[1]);

  TString cfgtag(argv[2]);
  vector<int> channellist;
  std::vector<std::vector<TString> > columns;

  for (int iarg=3; iarg<argc; iarg++) {
    TString arg(argv[iarg]);
    TObjArray *tokens = arg.Tokenize("=");
    assert(tokens->GetEntriesFast() == 2);
    TString channame = ((TObjString *)(*tokens)[0])->GetString();
    TString infname  = ((TObjString *)(*tokens)[1])->GetString();

    FILE *fp = fopen(infname,"r");
  
    if (!fp) {
      cerr << "Couldn't open file " << string(infname) << endl;
      exit(-1);
    }

    // since a subset of all channels can be specified, a separate
    // list of the specified channels has to be separately maintained
    // and indexed.
    //
    int ichan;
    for (ichan=0; ichan<NUMCHAN; ichan++)
      if (!channame.CompareTo(channames[ichan])) {
	channellist.push_back(ichan);
	break;
      }
    if (ichan==NUMCHAN) {
      cerr << channame << " not a valid channel identifier" << endl;
      exit(-1);
    }

    // columns are processes, rows are channels
    readTextTable(fp, channame, columns);
  }
  
  assert (channellist.size());

#ifdef ISHWW
  // needed for theoretical uncertainties
  readHxsTable   (Form("ggHtable%dtev.txt",beamcomenergytev));
  readHxsTable   (Form("vbfHtable%dtev.txt",beamcomenergytev)); // , scalefrom7to8tev);
  readJetBinErrTable("jetbinerrtable.txt");
#endif

  map<int,CardData_t> m_cards;
  makeDataCardContent(massgev,columns,channellist,m_cards);

  map<int,CardData_t>::iterator it;
  for (it=m_cards.begin();
       it!=m_cards.end();
       it++) {

    fmtDataCardFile(it->first,it->second,cfgtag);
  }
  
}                                                             // makeDataCardFiles

#ifdef MAIN
//================================================================================

#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc<4) {
    printf("Usage: %s mHinGeV cfgtag channel=input.text [channel=input.text ...]\n",argv[0]);
    return 1;
  }
#ifdef DEBUG
  for (int i=0; i<argc; i++) printf("%s ", argv[i]);
  printf ("\n");
#endif

  makeDataCardFile(argc,argv);
 
  return 0;
}
#endif
