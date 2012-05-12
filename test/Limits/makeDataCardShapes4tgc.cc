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

#include "atgcinputs.h"

//================================================================================

CardData_t 
makeNewCard(TH1           *inhist,
	    const TString& procname,
	    const TString& systname,
	    const int      ichan,
	    const int      nchan
	    )
{
  CardData_t card;

  card.nbackproc = 0;
  card.nsigproc  = 0;

  cout << "\tmake new card ";

  const pair<double,double> zeropair(0.0,0.0);

  char elormu = channames[ichan][0];
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  TString sigXSsyst  = "sigUncXS";

  card.systematics[trigsyst]       = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]       = "lnN"; // common across same-flavor channels
  card.systematics["lumi"]         = "lnN"; // common across channels
  card.systematics[sigXSsyst]      = "lnN";

  ProcData_t pd;

  pair<TString,double> channel;

  pd.name        = procname;
  channel.first  = channames[ichan];
  channel.second = inhist->Integral(); // the yield for this channel

  pd.channels.insert(channel);

  if( procname.Contains("data") ) {
    card.data = pd; // done.
  } else {
    int index=0;

    if( procname.Contains("ignal") ) {                              // signal

      card.processes.resize(1);
      card.nsigproc++;

      pd.procindex = 1-card.nsigproc; /* process index for signal processes required to be distinct,
					 as well as 0 or negative */

      assert(!systname.Length()); // shape based systematics for signal not implemented yet

      // insert signal lumi and xsec systematics now

      pair<double,double> lumipair(0.0, 1+siglumiunc);

      pd.systrates["lumi"].resize(nchan,lumipair);
      pd.systrates[trigsyst].resize(nchan,zeropair);
      pd.systrates[trigsyst][ichan].second = 1+sigtrigeffunc;
      pd.systrates[leptsyst].resize(nchan,zeropair);
      pd.systrates[leptsyst][ichan].second = 1+siglepteffunc;
      pd.systrates[sigXSsyst].resize(nchan,zeropair);
      pd.systrates[sigXSsyst][ichan].second = 1+signal_xs_unc;

    } else {                                                        //background
      card.nbackproc++;
      pd.procindex = card.nbackproc;

      // must reserve index 0 for signal,
      // so give this process index 1
      index = 1;
      card.processes.resize(2);
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

   TString channame(channames[ichan]);

  channel.first  = channame;
  channel.second = inhist->Integral();

  char elormu = channame[0];
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  TString sigXSsyst  = "sigUncXS";

  card.systematics[trigsyst]     = "lnN"; // common across same-flavor channels
  card.systematics[leptsyst]     = "lnN"; // common across same-flavor channels
  card.systematics[sigXSsyst]    = "lnN";

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

  else if( procname.Contains("ignal") ) { // signal expectation

    assert(card.processes.size());  // space for index 0 s/b reserved by now
    assert(!systname.Length());     // shape based systematics for signal not implemented yet

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
      pd.systrates[sigXSsyst].resize(nchan,zeropair);
      pd.systrates[sigXSsyst][ichan].second = 1+signal_xs_unc;

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

      // has this signal systematic been initialized for this process?
      rit = pd.systrates.find(sigXSsyst);
      if (rit == pd.systrates.end()) { //  no
	pd.systrates[sigXSsyst].resize(nchan,zeropair);
      }
      pd.systrates[sigXSsyst][ichan].second   = 1.0 + signal_xs_unc;
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

      card.processes.push_back(pd);

    } else {                                /* either another channel, or a shape
					       systematic for an existing channel */
      ProcData_t& pd = card.processes[pit->second];

      assert(pd.name.Length());             // process should exist here
      assert(!pd.name.CompareTo(procname)); // and be named the same
      pd.channels.insert(channel);
    }
  }
}                                                                     // addToCard

//================================================================================

void
makeDataCardFiles(int argc, char*argv[])
{
  TFile *fps[NUMCHAN];

  if (argc != NUMCHAN+1) {
    cerr << "Need " << NUMCHAN << " input root files, one for each channel" << endl;
    exit(-1);
  }

  for (int i=0; i<NUMCHAN; i++) {
    fps[i] = new TFile(argv[i+1]);
    if (fps[i]->IsZombie()) {
      cerr << "Couldn't open file " << string(argv[i+1]) << endl;
      exit(-1);
    }
  }

  TH1 *datahists[NUMCHAN];
  TH1 *backhists[NUMCHAN];

  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    datahists[ichan] = (TH1 *)fps[ichan]->Get("data_obs");
    if (!datahists[ichan]) {
      cerr << "Couldn't get data histogram from file for channel " << ichan << endl;
      exit(-1);
    }
    backhists[ichan] = (TH1 *)fps[ichan]->Get("background");
    if (!backhists[ichan]) {
      cerr << "Couldn't get background histogram from file for channel " << ichan << endl;
      exit(-1);
    }
  }

  // loop through objects in the input root file and find histograms
  // that are shape inputs into the limit setting data card
  //
  for (float lambdaz=LAMBDAZ_MIN; lambdaz<=LAMBDAZ_MAX; lambdaz+= LAMBDAZ_INC) {
    for (float deltaKappaGamma=dKG_MIN; deltaKappaGamma<=dKG_MAX; deltaKappaGamma += dKG_INC) {

      TString cfgtag = Form("lambdaZ_%.1f_deltaKappaGamma_%.1f",lambdaz+0.001,deltaKappaGamma+0.001);
      TString signame = "signal_"+cfgtag;

      CardData_t card = makeNewCard(datahists[0],"data","",0,NUMCHAN);

      for (int ichan=0; ichan<NUMCHAN; ichan++) {
	TH1 *sighist = (TH1 *)fps[ichan]->Get(signame);

	if (!sighist) {
	  cerr << "Couldn't get signal histogram " << signame << " from file for channel "<<ichan<<endl;
	  exit(-1);
	}

	// assumes the channel filenames are in the same order as the channels!!
	card.shapespecs.push_back(ShapeFiles_t("signal",channames[ichan],TString(argv[ichan+1]),signame));
	card.shapespecs.push_back(ShapeFiles_t("data_obs",channames[ichan],TString(argv[ichan+1]),"data_obs"));
	card.shapespecs.push_back(ShapeFiles_t("background",channames[ichan],TString(argv[ichan+1]),"background"));

	if (ichan)
	  addToCard(card,datahists[ichan],"data","",ichan,NUMCHAN);

	addToCard(card,backhists[ichan],"background","",ichan,NUMCHAN);
	addToCard(card,sighist,"signal","",ichan,NUMCHAN);
      }


      fmtDataCardFile(0,card,cfgtag);

    } // dKG loop
  } // lambdaz loop

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

  makeDataCardFiles(argc, argv);
  return 0;
}
#endif
