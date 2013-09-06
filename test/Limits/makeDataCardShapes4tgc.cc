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

#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooATGCSemiAnalyticPdf.h"
#include "card.h"
#include "atgcinputs.h"


//================================================================================

Card *
makeDataCardContent(const TString& fname,
		    const TString& channame)
{
  Card *card;

  RooWorkspace *w = (RooWorkspace *)gDirectory->Get(channame);

  RooDataHist *backhist;
  RooDataHist *shapehist;
  RooDataHist *sighist;
  RooDataHist *datahist;

  char elormu = tolower(channame[ELORMUCHAR]);
  TString leptsyst   = "CMS_eff_"+TString(elormu);
  TString trigsyst   = "CMS_trigger_"+TString(elormu);
  TString sigXSsyst  = "sigUncXS";

  TString bkgprocname = TString(bkgdobjprefix)+"_"+channame;

  datahist = (RooDataHist *)w->data(dataobjname);
  if (!datahist) {
    cerr << "Couldn't find "<<dataobjname<<" histogram in workspace " << channame << endl;
    exit(-1);
  }

  backhist = (RooDataHist *)w->obj(bkgprocname);
  if (!backhist) {
    cerr << "Couldn't find "<<bkgprocname<<" histogram in workspace " << channame << endl;
    exit(-1);
  }

  TString shapesystname = channame+"_backshape";

  shapehist = (RooDataHist *)w->obj(TString(bkgdobjprefix)+"_"+shapesystname+"Up");
  if (!shapehist) {
    cerr << "Couldn't find background backshapeUp histogram in workspace " << channame << endl;
    exit(-1);
  }

  // The sigpdfname is the name of the object that we tell combine to use,
  // but the integral (signal yield) comes from sighist (default SM value)
  // 
  TString sigpdfname = TString("ATGCPdf_")+signame+TString("_")+channame;
  TString sighistname = TString("SM_")+channame+TString("_rawshape");
  sighist = (RooDataHist *)w->obj(sighistname);

  if (!sighist) {
    cerr<<"Couldn't find signal histogram "<<sighistname<<" in workspace "<<channame<<endl;
    //exit(-1);
    return NULL;
  }

  card = new Card(datahist->sumEntries(),dataobjname,channame,"",false);
  card->addProcessChannel(sighist->sumEntries(),signame,channame,"",true);
  card->addProcessChannel(backhist->sumEntries(),bkgprocname,channame,shapesystname,false);

  // (non-shape) Systematics:
  card->addSystematic(leptsyst,signame,channame,1+siglepteffunc);
  card->addSystematic(trigsyst,signame,channame,1+sigtrigeffunc);
  card->addSystematic("lumi_8TeV",signame,channame,1+siglumiunc);
  card->addSystematic("sigXSsyst",signame,channame,1+signal_xs_unc);

  // assumes the channel filenames are in the same order as the channels!!
  card->addShapesFile(ShapesFile_t("data_obs",channame,fname,channame+":data_obs"));
  card->addShapesFile(ShapesFile_t(signame,channame,fname,channame+":"+sigpdfname));
  card->addShapesFile(ShapesFile_t(bkgprocname,channame,fname,
				   channame+":"+bkgprocname,
				   channame+":"+bkgdobjprefix+"_$SYSTEMATIC"));

  return card;
}                                                           // makeDataCardContent

//================================================================================

void
makeDataCardFiles(const char *rootfn,
		  const char *nametag)
{
  TFile *fp;

  TString cfgtag(nametag);

#if 0
  if (argc != NUMCHAN+1) {
    cerr << "Need " << NUMCHAN << " input root files, one for each channel" << endl;
    exit(-1);
  }
#endif

  int ichan = (rootfn[0]=='m'); // 0=false=electron, 1=true=muon

  TString fname(rootfn);
  fp = new TFile(fname);

  TString channame(channames[ichan]);

  if (fp->IsZombie()) {
    cerr << "Couldn't open file " << fname << endl;
    exit(-1);
  }

  cout << "Reading root input file " << fname << endl;

  Card *card = makeDataCardContent(fname,channame);

  if (card)
  {
    TString dcardname("./datacard_8TeV_ATGC_"+channame+"_"+cfgtag+".txt");
    card->Print(dcardname);
  }

}                                                             // makeDataCardFiles

//================================================================================

#ifdef MAIN
#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s rootfile nametag\n",argv[0]);
    return 1;
  }
#ifdef DEBUG
  for (int i=0; i<argc; i++) printf("%s ", argv[i]);
  printf ("\n");
#endif

  makeDataCardFiles(argv[1], argv[2]);

  return 0;
}
#endif
