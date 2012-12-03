/*
makeLimitFile

Some important constants are set at the top of the file.
*/
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"

#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"

#include "card.h"
#include "hwwinputs.h"
#include "hwwutils.h"

using namespace std;

//================================================================================

Card *
makeDataCardContent(TFile *fp,
		    int imass,
		    bool isinterp,
		    int ichan)
{
  Card *card;

  TString channame(channames[ichan]);
  int massgev=isinterp ? interpolatedmasspts[imass] : masspts[imass];

  RooWorkspace *w = (RooWorkspace *)gDirectory->Get("w");

  //w->Print();

  // The dataset

  if (!w->data("data_obs")) {
    cerr << "Couldn't find data_obs in workspace" << endl;
    exit(-1);
  }

  card = new Card(-1,"data_obs","",channame);

  cout<<endl;

  // PDFs
  RooArgSet pdfs = w->allPdfs();
  TIterator* pit = pdfs.createIterator();
  

  for(RooAbsPdf* pdf = 0;
      (pdf=(RooAbsPdf*)pit->Next());){

    if (pdf->IsA() == RooProdPdf::Class()) {
      //pdf->Print();

      TString pdfname(pdf->GetName());

      if (pdfname.EndsWith("Down")) continue; // skip the "Down"s, expect a matching "Up"

      // tokenize the pdf name to determine
      // 1. process name
      // 2. any systematic applied
      //
      TString procname(""),systname("");

      if (pdfname.EndsWith("Up")) { // This is a histogram with a systematic applied
	procname = TString(pdfname(0,pdfname.First('_')-1));
	systname = TString(pdfname(pdfname.First('_')+1,pdfname.Length()-3));
      } else
	procname = pdfname;

      RooRealVar * pdfint = w->var("n_"+procname);

      if (!pdfint) {
	cerr << "can't find integral for " << procname << endl;
	continue;
      }

      //pdfint->Print();

      cout<<"Read process="<<setw(10)<<procname<<", channel="<<channame<<", mass="<<massgev;
      if (isinterp) cout << " (interpolated)";
      if (systname.Length()) cout << ", systname = " << systname;

      card->addToCard(pdfint->getValV(),procname,systname,ichan,0,1);

      cout<<endl;
    }
  }

  std::vector<int> channellist(1,ichan);

  card->addSystematics(massgev,channellist);

  return card;

}                                                           // makeDataCardContent

//================================================================================

void
makeDataCardFiles(int imass, bool isinterp, char*nametag)
{
  TString cfgtag(nametag);

  int massgev=isinterp ? interpolatedmasspts[imass] : masspts[imass];

  cout << "cfgtag = " << cfgtag << endl;

#ifdef ISHWW
  // needed for theoretical uncertainties
  readHxsTable   (Form("ggHtable%dtev.txt",beamcomenergytev));
  readHxsTable   (Form("vbfHtable%dtev.txt",beamcomenergytev)); // , scalefrom7to8tev);
  readJetBinErrTable("jetbinerrtable.txt");
#endif //ISHWW

  
  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    TString fname = TString(dir) + "/" + Form(inputfilesfmtstr[ichan],massgev);
    cout << "Reading files from directory: " << dir << endl;
    TFile *fp = new TFile(fname.Data());

    if (fp->IsZombie()) {
      cerr << "Couldn't find root input file " << fname << ", skipping..." << endl;
      continue;
    }

    cout << "Reading root input file " << fname << endl;
    
    Card *card = makeDataCardContent(fp,imass,isinterp,ichan);

    card->addShapeFiles(ShapeFiles_t("*","*",fname,
				     "w:$PROCESS",
				     "w:$PROCESS_$SYSTEMATIC")
			);

    card->Print(massgev,cfgtag);

    delete card;

  } // channel loop
}                                                             // makeDataCardFiles

#ifdef MAIN
//================================================================================

#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s nametag\n",argv[0]);
    return 1;
  }
#ifdef DEBUG
  for (int i=0; i<argc; i++) printf("%s ", argv[i]);
  printf ("\n");
#endif

  for (int imass=0; imass<NUMMASSPTS; imass++) {
    makeDataCardFiles(imass, false, argv[1]);
  }
#ifdef DO_INTERP
  for (int imass=0; ; imass++) {
    int mass = interpolatedmasspts[imass];
    if (mass < 0) break;
    makeDataCardFiles(imass, true, argv[1]);
  }
#endif

  return 0;
}
#endif //MAIN
