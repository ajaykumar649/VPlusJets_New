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

#include "card.h"
#include "atgcinputs.h"


//================================================================================
#if 0
bool calcEstimatedLimit(const Card& card)
{
  double totback = 0.0, totsig=0.0;

  std::map<TString,int>::const_iterator it;
  for (it= card.pname2index.begin(); it != card.pname2index.end(); it++)  {
    const ProcData_t& pd = card.processes[it->second];
    std::map<TString,double>::const_iterator itchan;
    if (pd.procindex > 0) { // background
      for (itchan = pd.channels.begin(); itchan != pd.channels.end(); itchan++)
	totback += itchan->second;
    } else {
      for (itchan = pd.channels.begin(); itchan != pd.channels.end(); itchan++) {
	if (itchan->second < 0.005) {
	  cerr << "signal contribution for channel " << itchan->first << " is too small, skipping" << endl;
	  return false;
	}
	totsig += itchan->second;
      }
    }
  }

  double estlimit = 1.92 * sqrt(totback)/totsig;

  printf("%7.1f %7.2f %5.2f %5.2f\n", 
	 totback,totsig,estlimit,estlimit*3);

  return (true);
}                                                            // calcEstimatedLimit
#endif

//================================================================================

Card *
makeDataCardContent(TFile *fp,
		    const TString& fname,
		    const TString& channame,
		    const TString& signame,
		    const TString& sigaltname)
{
  Card *card;

  TH1 *datahist;
  TH1 *backhist;
  TH1 *shapehist;

  char elormu = channame[ELORMUCHAR];
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString sigXSsyst  = "sigUncXS";

  datahist = (TH1 *)fp->Get(dataobjname);
  if (!datahist) {
    cerr << "Couldn't get data histogram from file for channel " << channame << endl;
    exit(-1);
  }

  backhist = (TH1 *)fp->Get(bkgdobjname);
  if (!backhist) {
    cerr << "Couldn't get background histogram from file for channel " << channame << endl;
    exit(-1);
  }

  TString shapesystname = Form("%s_backshape",channame.Data());

  shapehist = (TH1 *)fp->Get("background_"+shapesystname+"Up");
  if (!shapehist) {
    cerr << "Couldn't get background shapeUp histogram from file for channel " << channame << endl;
    exit(-1);
  }

  TH1 *sighist = (TH1 *)fp->Get(signame);

  if (!sighist) {
    cerr<<"Couldn't get signal histogram "<<signame<<" from file for channel "<<channame<<endl;
    //exit(-1);
    return NULL;
  }

  TH1 *sigalthist = (TH1 *)fp->Get(sigaltname);

  if (!sigalthist) {
    cerr<<"Couldn't get alt signal histogram "<<sigaltname<<" from file for channel "<<channame<<endl;
    //exit(-1);
    return NULL;
  }

  card = new Card(-1,dataobjname,channame,"",false);
  card->addProcessChannel(sighist->Integral(),"sigwv",channame,"",true);
  card->addProcessChannel(sigalthist->Integral(),"sigwv_ALT",channame,"",true);
  card->addProcessChannel(backhist->Integral(),bkgdobjname,channame,shapesystname,false);

  // (non-shape) Systematics:
  card->addSystematic(leptsyst,"sigwv",channame,1+siglepteffunc);
  card->addSystematic(trigsyst,"sigwv",channame,1+sigtrigeffunc);
  card->addSystematic("lumi_8TeV","sigwv",channame,1+siglumiunc);
  card->addSystematic("sigXSsyst","sigwv",channame,1+signal_xs_unc);

  card->addSystematic(leptsyst,"sigwv_ALT",channame,1+siglepteffunc);
  card->addSystematic(trigsyst,"sigwv_ALT",channame,1+sigtrigeffunc);
  card->addSystematic("lumi_8TeV","sigwv_ALT",channame,1+siglumiunc);
  card->addSystematic("sigXSsyst","sigwv_ALT",channame,1+signal_xs_unc);

  card->addShapesFile(ShapesFile_t("data_obs",channame,fname,"data_obs"));
  card->addShapesFile(ShapesFile_t("sigwv",channame,fname,signame));
  card->addShapesFile(ShapesFile_t("sigwv_ALT",channame,fname,sigaltname));
  card->addShapesFile(ShapesFile_t("background",channame,fname,
				   "background","background_$SYSTEMATIC"));

  return card;
}                                                           // makeDataCardContent

//================================================================================

void
make_LZ_DKG_files(TFile *fp,
		  const TString& fname,
		  const TString& channame)
{
  TString cfgtag = Form(signalfmtstr_lzvsdkg,0.0,0.0);
  TString sigSM  = "signal_"+cfgtag;

  // loop through objects in the input root file and find histograms
  // that are shape inputs into the limit setting data card
  //
  for (float lambdaz=LAMBDAZ_MIN;
       lambdaz<=LAMBDAZ_MAX+LAMBDAZ_INC/1000;
       lambdaz+= LAMBDAZ_INC) {

    for (float deltaKappaGamma=dKG_MIN;
	 deltaKappaGamma<=dKG_MAX+dKG_INC/1000;
	 deltaKappaGamma += dKG_INC) {
      
      //+INC/1000 to avoid truncation
      cfgtag = Form(signalfmtstr_lzvsdkg,
		    lambdaz+LAMBDAZ_INC/1000.,
		    deltaKappaGamma+dKG_INC/1000.);
      TString signame = "signal_"+cfgtag;
      
      Card *card = makeDataCardContent(fp,fname,channame,sigSM,signame);

      //if (calcEstimatedLimit(card))
      if (card)
      {
	//+INC/1000 to avoid truncation
	cfgtag = Form("lz_%.3f_dkg_%.2f_%s",lambdaz+LAMBDAZ_INC/1000.,
		      deltaKappaGamma+dKG_INC/1000.,channame.Data());
	TString dcardname("./datacard_"+cfgtag+".txt");
	card->Print(dcardname);
      }
      delete card;

    } // dKG loop
  } // lZ loop
}                                                            // make_LZ_DKG_files

//================================================================================

void
make_LZ_DG1_files(TFile *fp,
		  const TString& fname,
		  const TString& channame)
{
  TString cfgtag = Form(signalfmtstr_lzvsdg1,0.0,0.0);
  TString sigSM  = "signal_"+cfgtag;

  // loop through objects in the input root file and find histograms
  // that are shape inputs into the limit setting data card
  //
  for (float lambdaz=LAMBDAZ_MIN;
       lambdaz<=LAMBDAZ_MAX+LAMBDAZ_INC/1000;
       lambdaz+= LAMBDAZ_INC) {

    for (float deltaG1=dg1_MIN;
	 deltaG1<=dg1_MAX+dg1_INC/1000;
	 deltaG1 += dg1_INC) {

      //+INC/1000 to avoid truncation
      cfgtag = Form(signalfmtstr_lzvsdg1,
		    lambdaz+LAMBDAZ_INC/1000.,
		    deltaG1+dg1_INC/1000.);
      TString signame = "signal_"+cfgtag;
	
      Card *card = makeDataCardContent(fp,fname,channame,sigSM,signame);

      //if (calcEstimatedLimit(card))
      if (card) {
	//+INC/1000 to avoid truncation
	cfgtag = Form("lz_%.3f_dg1_%.3f_%s", lambdaz+LAMBDAZ_INC/1000.,
		      deltaG1+dg1_INC/1000., channame.Data());
	TString dcardname("./datacard_"+cfgtag+".txt");
	card->Print(dcardname);
      }
      delete card;

    } // dg1 loop
  } // lambdaz loop
}                                                            // make_LZ_DG1_files

//================================================================================

void
make_DKG_DG1_files(TFile *fp,
		   const TString& fname,
		   const TString& channame)
{
  TString cfgtag = Form(signalfmtstr_dkgvsdg1,0.0,0.0);
  TString sigSM  = "signal_"+cfgtag;

  // loop through objects in the input root file and find histograms
  // that are shape inputs into the limit setting data card
  //
  for (float deltaKappaGamma=dKG_MIN;
       deltaKappaGamma<=dKG_MAX+dKG_INC/1000;
       deltaKappaGamma += dKG_INC) { 

    for (float deltaG1=dg1_MIN;
	 deltaG1<=dg1_MAX+dg1_INC/1000;
	 deltaG1 += dg1_INC) { 

      //+INC/1000 to avoid truncation
      cfgtag = Form(signalfmtstr_dkgvsdg1,
			    deltaKappaGamma+dKG_INC/1000.,
			    deltaG1+dg1_INC/1000.);
      TString signame = "signal_"+cfgtag;
      
      Card *card = makeDataCardContent(fp,fname,channame,sigSM,signame);
      
      //if (calcEstimatedLimit(card))
      if (card)
      {
	//+INC/1000 to avoid truncation
	cfgtag = Form("dkg_%.2f_dg1_%.3f_%s", deltaKappaGamma+dKG_INC/1000.,
		      deltaG1+dg1_INC/1000., channame.Data());
	TString dcardname("./datacard_"+cfgtag+".txt");
	card->Print(dcardname);
      }
      delete card;
      
    } // dg1 loop
  } // dKG loop
  
}                                                            // make_DKG_DG1_files

//================================================================================

void
makeDataCardFiles(bool doshape) // int argc, char*argv[])
{
  TFile *fp;
  TString fname;

#if 0
  if (argc != NUMCHAN+1) {
    cerr << "Need " << NUMCHAN << " input root files, one for each channel" << endl;
    exit(-1);
  }
#endif

  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    fname = TString(dir)+"/"+TString(inputfiles[ichan]); // TString(argv[ichan+1]);
    fp = new TFile(fname);

    TString channame(channames[ichan]);

    if (fp->IsZombie()) {
      cerr << "Couldn't open file " << fname << endl;
      exit(-1);
    }

    cout << "Reading root input file " << fname << endl;

    make_LZ_DKG_files(fp, fname, channame);
    //make_LZ_DG1_files(fp, fname, channame);
    make_DKG_DG1_files(fp, fname, channame);

    fp->Close();
    delete fp;

  } // channel loop

}                                                             // makeDataCardFiles

//================================================================================

#ifdef MAIN
#define DEBUG 1

int main(int argc, char* argv[]) {

#if 0
  if (argc<2) {
    printf("Usage: %s [do-cut-and-count]\n",argv[0]);
    return 1;
  }
#endif
#ifdef DEBUG
  for (int i=0; i<argc; i++) printf("%s ", argv[i]);
  printf ("\n");
#endif

  // exactly one arg is nominal,
  // more than one means suppress shape info in the card
  // for a cut-and-count limit
  //
  makeDataCardFiles(argc <= 1);
  return 0;
}
#endif
