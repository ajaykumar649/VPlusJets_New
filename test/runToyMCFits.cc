/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Osipenkov, Ilya, Texas A&M - ilyao@fnal.gov
 *
 * Description: Use to constuct and fit toy datasets
 *
 ********************************************************************/

#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TString.h>
#include <TH1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom3.h>

using namespace std;

const char* logdir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_logs/";
const char* mcSetprefix = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSets/MCSet";
const char* mcNameprefix = "MCSet";
const char* resultsdir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_results/";
const char* logprefix = "1KSetToy";

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void runToyMCFits(int NJets, int NStart, int NEnd) {
  TString Command;
  char I_char[5];
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);
  TString NJ_str=NJ_char;

  if ( NStart==0 ) {
    TString resultsdir_str=resultsdir;
    TFile *fAll = new TFile(resultsdir_str+"FitSummary_1K_"+NJ_str+"j.root", "RECREATE");
    fAll->Close();
  }

  for (Int_t i=NStart; i<(NEnd+1); i++) {
    sprintf(I_char,"%i",i);
    Command=".log";
    Command=I_char+Command;
    Command="j_Sample"+Command;
    Command=NJ_char+Command;
    Command="_"+Command;
    Command=logprefix+Command;
    Command=logdir+Command;

    Command = "Jets.txt -m MjjOptimizeConfig_toydataFits > "+Command;
    Command = NJ_char + Command;
    Command = " -i MjjNominal" + Command;
    Command = NJ_char + Command;
    Command = " -j " + Command;
    //set precision
    Command = " -p 2 " + Command;

    Command = ".root" + Command;
    Command=I_char+Command;
    Command = mcNameprefix + Command;
    Command = " --TD " + Command;

    Command = "python newOptimize.py " + Command;

 //    Command=".root\")\' > /uscms_data/d1/ilyao/KinematicFitterS11/WjjSimulation_log/WjjSim1Klog"+Command;
//     Command=I_char+Command;
//     Command="root -b -q \'RooWjjFitterNarrow_ToyGen.C(0,\"/uscms_data/d1/ilyao/KinematicFitterS11/WjjSimulation_root/mjjSim1K"+Command;
    cout << "Command=" << Command << endl;
    //system(Command);
  }

}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void createTestDatasets(int NJets, int InitRand, int NStart=0, int NEnd=999, const char* outFileName = mcSetprefix)
//// Combines the input yields in appropriate amounts (randomized according to a Poisson distribution) and saves them in a file of the form form mcSetprefix*.root with *=NStart-(NEnd+1)

{
  TRandom Rr(InitRand); 
  char I_char[5];
  double mjj, mjj_out;
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);
  TString NJ_str=NJ_char;

  const int NProcesses=8;
  //Set the values from the fit for each process
  Int_t NDiboson=1001;
  Int_t Nqcd=2829;
  Int_t Ntt=3156;
  Int_t NsingleTop=1204;
  Int_t Nzjets=799;
  Int_t NWjjDef=13833;
  Int_t NWjjSU=7384;//fSU=0.213
  Int_t NWjjMU=13450;//fMU=0.388

  if ( NJets==3 ) {
    NDiboson=241;
    Nqcd=1305;
    Ntt=5277;
    NsingleTop=653;
    Nzjets=260;
    NWjjDef=7425;
    NWjjSU=437;//fSU=0.05
    NWjjMU=874;//fMU=0.10
  }

  Int_t Nevt[NProcesses];
  Int_t Nentry[NProcesses];
  for (Int_t j=0; j<NProcesses;j++) {
    Nentry[j]=0;
  }

  const char* sourcedir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSource/";
  TFile *f[NProcesses];
  TString sourcedir_str=sourcedir;
  f[0]= new TFile(sourcedir_str+"Diboson_"+ NJ_str+"j_GenMC.root", "READ");
  f[1]= new TFile(sourcedir_str+"qcd_"+ NJ_str+"j_GenMC.root", "READ");
  f[2]= new TFile(sourcedir_str+"tt_"+ NJ_str+"j_GenMC.root", "READ");
  f[3]= new TFile(sourcedir_str+"singleTop_"+ NJ_str+"j_GenMC.root", "READ");
  f[4]= new TFile(sourcedir_str+"zjets_"+ NJ_str+"j_GenMC.root", "READ");
  f[5]= new TFile(sourcedir_str+"WjjDef_"+ NJ_str+"j_GenMC.root", "READ");
  f[6]= new TFile(sourcedir_str+"WjjSU_"+ NJ_str+"j_GenMC.root", "READ");
  f[7]= new TFile(sourcedir_str+"WjjMU_"+ NJ_str+"j_GenMC.root", "READ");

  TTree *InTree[NProcesses];
  InTree[0] = (TTree*) f[0]->Get("dibosonPdfData");
  InTree[1] = (TTree*) f[1]->Get("qcdPdfData");
  InTree[2] = (TTree*) f[2]->Get("ttPdfData");
  InTree[3] = (TTree*) f[3]->Get("stPdfData");
  InTree[4] = (TTree*) f[4]->Get("ZpJPdfData");
  InTree[5] = (TTree*) f[5]->Get("WpJPdfData");
  InTree[6] = (TTree*) f[6]->Get("WpJPdfData");
  InTree[7] = (TTree*) f[7]->Get("WpJPdfData");

  InTree[0]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[1]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[2]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[3]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[4]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[5]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[6]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[7]->SetBranchAddress("Mass2j_PFCor",&mjj);

  TString outfilename;
  for (Int_t k=NStart; k<(NEnd+1);k++) {
    //    cout << "Generating File #" << k << endl;
    outfilename=".root";
    sprintf(I_char,"%i",k);
    outfilename=I_char+outfilename;
    outfilename="j_"+outfilename;
    outfilename=NJ_char+outfilename;
    outfilename="_"+outfilename;
    outfilename=outFileName+outfilename;
    TFile *fout = new TFile(outfilename, "RECREATE");
    TTree* TMCTree = new TTree("WJet", "Tree from toy MC");
    TMCTree->Branch("Mass2j_PFCor",&mjj_out,"Mass2j_PFCorr/D");
    TMCTree->Branch("evtNJ",&NJets,"evtNJ/I");

    Nevt[0]=Rr.Poisson(NDiboson);
    Nevt[1]=Rr.Poisson(Nqcd);
    Nevt[2]=Rr.Poisson(Ntt);
    Nevt[3]=Rr.Poisson(NsingleTop); 
    Nevt[4]=Rr.Poisson(Nzjets);
    Nevt[5]=Rr.Poisson(NWjjDef);
    Nevt[6]=Rr.Poisson(NWjjSU);
    Nevt[7]=Rr.Poisson(NWjjMU);

    for (Int_t j=0; j<NProcesses;j++) {
      for (Int_t i=Nentry[j]; i<(Nentry[j]+Nevt[j]);i++) {
	InTree[j]->GetEntry(i);
	mjj_out=mjj;
	TMCTree->Fill();
      }
      Nentry[j]=Nentry[j]+Nevt[j];
    }
    fout->Write();
    fout->Close();
    cout << "Generated File " << k << "  NEvts=" << Nevt[0]+Nevt[1]+Nevt[2]+Nevt[3]+Nevt[4]+Nevt[5]+Nevt[6]+Nevt[7] << endl;
  }

}

// // -----------------------------------------------------------------------------------------------------------------------------------------------------------//
// void runTestDatasetFits(int NStart, int NEnd) {
//   TString Command;
//   char I_char[5];

//   if ( NStart==0 ) {
//     TFile *fAll = new TFile("/uscms_data/d1/ilyao/KinematicFitterS11/ToySimulation_root/ToyDataset1KFitSummary.root", "RECREATE");
//     fAll->Close();
//   }

//   for (Int_t i=NStart; i<(NEnd+1); i++) {
//     sprintf(I_char,"%i",i);
//     Command=".log";
//     Command=I_char+Command;
//     Command=".root\")\' > /uscms_data/d1/ilyao/KinematicFitterS11/ToySimulation_log/Sim1KDatasetFit"+Command;
//     Command=I_char+Command;
//     Command="root -b -q \'RooWjjFitterNarrow_Validation.C(0,\"/uscms_data/d1/ilyao/KinematicFitterS11/ToySimulation_root/SimDataset"+Command;
//     cout << "Command=" << Command << endl;
//     system(Command);
//   }

// }

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

void processLogs(int NLogs=1000, const char* logPrefix="/uscms_data/d1/ilyao/KinematicFitterS11/ToySimulation_log/Sim1KDatasetFit", const char* outFileName="/uscms_data/d1/ilyao/KinematicFitterS11/ToySimulation_root/1KDataSetFit_LogSummary.root")
//// Processes the logs and produces an output file. Log files are of the form logPrefix*.log, where *=0-NLogs
{
  TString inLogFileName, sPar1, sPar2, sPar3, sPar4;
  char logline[2000];
  char I_char[5];
  double dPar,dErr,numDiboson,numWjets,errDiboson,errWjets,numTotal;
  int Convergence;
  //cout << "0" << endl;

  TFile *outfile = new TFile(outFileName, "RECREATE");
  TTree *OutTree = new TTree("OutTree","OutTree");

  //  cout << "1" << endl;
  OutTree->Branch("numDiboson",&numDiboson,"numDiboson/D");
  OutTree->Branch("errDiboson",&errDiboson,"errDiboson/D");
  OutTree->Branch("numWjets",&numWjets,"numWjets/D");
  OutTree->Branch("errWjets",&errWjets,"errWjets/D");
  OutTree->Branch("numTotal",&numTotal,"numTotal/D");
  OutTree->Branch("Convergence",&Convergence,"Convergence/I");

  //  cout << "2" << endl;
  for (Int_t i=0; i<NLogs; i++) {
    inLogFileName=".log";
    sprintf(I_char,"%i",i);
    inLogFileName=I_char+inLogFileName;
    inLogFileName=logPrefix+inLogFileName;
    //  cout << "3" << endl;
    ifstream inLogFile(inLogFileName);
    //cout << "4" << endl;
    while ( inLogFile.good() ) {
      inLogFile.getline(logline,2000);
      istrstream str(logline);
      //cout << "5" << endl;
      str >> sPar1 >> sPar2 >> sPar3 >> sPar4;
      if ( sPar3=="**MIGRAD" ) {
	for (Int_t dummy=0; dummy<14; dummy++) {
	  inLogFile.getline(logline,2000);
	}
	istrstream str1(logline);
	str1 >> sPar1 >> sPar2 >> sPar3 >> sPar4;
	cout << "i=" << i << " sPar4 = " << sPar4 << endl;
	if ( sPar4=="STATUS=CONVERGED" ) {
	  Convergence=1;
	} else {
	  Convergence=0;
	}
      }

      //get the errors returned by the fit
      if ( (sPar1=="Floating")&&(sPar2=="Parameter") ) {
	inLogFile.getline(logline,2000);
	inLogFile.getline(logline,2000);
	istrstream str1(logline);
	str1 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	//numDiboson=dPar;
	errDiboson=dErr;
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	//numWjets=dPar;
	errWjets=dErr;
      }

      if ( (sPar2=="Printing")&&(sPar3=="yields") ) {
	inLogFile.getline(logline,2000);
	istrstream str1(logline);
	str1 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	numDiboson=dPar;
	//errDiboson=dErr;
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	numWjets=dPar;
	//errWjets=dErr;
	inLogFile.getline(logline,2000);
	istrstream str3(logline);
	str3 >> sPar1 >> sPar2 >> dPar;
	numTotal=dPar;
      }
    }
    OutTree->Fill();
  }
  
  outfile->Write();
  outfile->Close();

}

