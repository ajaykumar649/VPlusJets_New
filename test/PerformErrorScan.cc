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
 * Description: A few macros to scan over Matching and Scale values
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
#include <TH2.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom3.h>

using namespace std;

const char* defaultlogprefix="/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/logs/WideScan_";
const char* defaultsummaryfile="/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/WideScanSummary_3j.root";


// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void PerformErrorScan(int NJets, double scaleUpStart, double scaleUpStep, int NscaleUpSteps, double matchingUpStart, double matchingUpStep, int NmatchingUpSteps, const char* logPrefix = defaultlogprefix ) {
  TString Command;
  double fMU_Val=matchingUpStart;
  double fSU_Val=scaleUpStart;
  char fMU_char[10];
  char fSU_char[10];
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);
  //TString NJ_str=NJ_char;


  for (Int_t is=0; is<NscaleUpSteps; is++) {
    for (Int_t jm=0; jm<NmatchingUpSteps; jm++) {
      sprintf(fMU_char,"%8.6f",fMU_Val);
      sprintf(fSU_char,"%8.6f",fSU_Val);
      Command=".log";
      Command=fMU_char+Command;
      Command="fMU"+Command;
      Command=fSU_char+Command;
      Command="j_fSU"+Command;
      Command=NJ_char+Command;
      Command="_"+Command;
      Command=logPrefix+Command;

      Command = "Jets.txt -m MjjOptimizeConfig_scanfSUfMU > "+Command;
      Command = NJ_char + Command;
      Command = " -i MjjNominal" + Command;
      Command = NJ_char + Command;
      Command = " -j " + Command;
      Command=fMU_char+Command;
      Command = " --fMU " + Command;
      Command = fSU_char+Command;
      Command = "python runMjjFitter.py --fSU " + Command;

//       if ( NJets==2 ) {
// 	Command=",0,0.0,true,false,false,2,\"Nominal2JetWjjFitParams.txt\")\' > "+Command;
//       } else {
// 	if ( NJets==3 ) {
// 	  Command=",0,0.0,true,false,false,3,\"Nominal3JetBinInit.txt\")\' > "+Command;
// 	} else {
// 	Command=",0,0.0,true,false,false,0,\"Nominal2JetWjjFitParams.txt\")\' > "+Command;
// 	}
//       }
//       Command=fMU_char+Command;
//       Command=","+Command;
//       Command=fSU_char+Command;
//       Command="root -l -b -q \'RooWjjFitterNew_ErrorScan.C+("+Command;

      cout << "Command=" << Command << endl;
      system(Command);
      fMU_Val=fMU_Val+matchingUpStep;
    }
    fMU_Val=matchingUpStart;
    fSU_Val=fSU_Val+scaleUpStep;
  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

void processLogs(int NJets, double scaleUpStart, double scaleUpStep, int NscaleUpSteps, double matchingUpStart, double matchingUpStep, int NmatchingUpSteps, const char* logPrefix=defaultlogprefix, const char* outFileName = defaultsummaryfile)
//// Processes the logs and produces an output file.
{
  TString inLogFileName, sPar1, sPar2, sPar3, sPar4;
  //  bool dofill=true;
  char logline[2000];
  double fMU_Val=matchingUpStart;
  double fSU_Val=scaleUpStart;
  char fMU_char[10];
  char fSU_char[10];
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);

  double dPar,dErr,numDiboson,numWjets,errDiboson,errWjets,nll;//,chi2log,chi2,chi2corr;
  double numTotal=0;
  int Convergence;

//   double ndof=14;
//   double NMC_WpJ = 37703;
//   double NMC_SU = 2960;
//   double NMC_SD = 8315;
//   double NMC_MU = 4715;
//   double NMC_MD = 4345;
//   if ( is3Jbin ) {
//     NMC_WpJ = 9260;
//     NMC_SU = 630;
//     NMC_SD = 2228;
//     NMC_MU = 1104;
//     NMC_MD = 1087;
//   }
  
//   double C_WpJ, C_SU, C_MU, w_WpJ, w_SU, w_MU;
//   //double k_WpJ, k_SU, k_MU, w_WpJ, w_SU, w_MU;



  TFile *outfile = new TFile(outFileName, "RECREATE");
  //TFile *outfile = new TFile(outFileName, "UPDATE");
  TTree *OutTree = new TTree("SummaryTree","SummaryTree");

  //  cout << "1" << endl;
  OutTree->Branch("numDiboson",&numDiboson,"numDiboson/D");
  OutTree->Branch("errDiboson",&errDiboson,"errDiboson/D");
  OutTree->Branch("numWjets",&numWjets,"numWjets/D");
  OutTree->Branch("errWjets",&errWjets,"errWjets/D");
  OutTree->Branch("numTotal",&numTotal,"numTotal/D");
  OutTree->Branch("fMU_Val",&fMU_Val,"fMU_Val/D");
  OutTree->Branch("fSU_Val",&fSU_Val,"fSU_Val/D");
  OutTree->Branch("nll",&nll,"nll/D");
  //  OutTree->Branch("chi2",&chi2,"chi2/D");
  //  OutTree->Branch("chi2corr",&chi2corr,"chi2corr/D");
  OutTree->Branch("Convergence",&Convergence,"Convergence/I");

  //  cout << "2" << endl;
  for (Int_t is=0; is<NscaleUpSteps; is++) {
    for (Int_t jm=0; jm<NmatchingUpSteps; jm++) {
      sprintf(fMU_char,"%8.6f",fMU_Val);
      sprintf(fSU_char,"%8.6f",fSU_Val);
      inLogFileName=".log";
      inLogFileName=fMU_char+inLogFileName;
      inLogFileName="fMU"+inLogFileName;
      inLogFileName=fSU_char+inLogFileName;
      inLogFileName="j_fSU"+inLogFileName;
      inLogFileName=NJ_char+inLogFileName;
      inLogFileName="_"+inLogFileName;
      inLogFileName=logPrefix+inLogFileName;
      cout << "inLogFileName = " << inLogFileName << endl;
      ifstream inLogFile(inLogFileName);
      //cout << "4" << endl;
      // Parse the log file and extract the desired parameters
      while ( inLogFile.good() ) {
	inLogFile.getline(logline,2000);
	istrstream str(logline);
	//cout << "5" << endl;
	str >> sPar1 >> sPar2 >> sPar3 >> sPar4;
	if ( sPar3=="**HESSE" ) {
	  for (Int_t dummy=0; dummy<3; dummy++) {
	    inLogFile.getline(logline,2000);
	  }
	  istrstream str1(logline);
	  str1 >> sPar1 >> sPar2 >> sPar3 >> sPar4;
	  //cout << "i=" << i << " sPar4 = " << sPar4 << endl;
	  if ( sPar4=="STATUS=OK" ) {
	    Convergence=1;
	  } else {
	    Convergence=0;
	  }
	}
	
	//get the parameters and errors returned by the fit
	if ( (sPar1=="Floating")&&(sPar2=="Parameter")&&(sPar3=="InitialValue") ) {
	  inLogFile.getline(logline,2000);
	  inLogFile.getline(logline,2000);
	  istrstream str1(logline);
	  str1 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=dPar;
	  numDiboson=dPar;
	  errDiboson=dErr;
	  inLogFile.getline(logline,2000);
	  istrstream str2(logline);
	  str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=numTotal+dPar;//add QCD
	  inLogFile.getline(logline,2000);
	  istrstream str3(logline);
	  str3 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=numTotal+dPar;//add SingleTop
	  inLogFile.getline(logline,2000);
	  istrstream str4(logline);
	  str4 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=numTotal+dPar;//add TTbar
	  inLogFile.getline(logline,2000);
	  istrstream str5(logline);
	  str5 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=numTotal+dPar;//add Wjets
	  numWjets=dPar;
	  errWjets=dErr;
	  inLogFile.getline(logline,2000);
	  istrstream str6(logline);
	  str6 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	  numTotal=numTotal+dPar;//add Zjets

	}

	//get the Log likelihood value
	if ( (sPar1=="*****")&&(sPar2=="nll") ) {
	  istrstream str1(logline);
	  str1 >> sPar1 >> sPar2 >> sPar3 >> dPar;
	  nll=dPar;
	}

// 	//get the chi2log value
// 	if ( sPar2=="chi^2fitdirect" ) {
// 	  // cout << "recording chi^2/dof" << endl;
// 	  istrstream str1(logline);
// 	  //cout << "logline=" << logline << endl;
// 	  str1 >> sPar1 >> sPar2 >> sPar3 >> dPar;
// 	  chi2log=dPar;
// 	  //cout << "chi2=" << chi2 << endl;
// 	}
      }

//       //Compute the desired chi2's
//       if ( fSU_Val>0 ) {
// 	w_SU=fSU_Val;
// 	C_SU=1/(1+w_SU*numWjets/NMC_SU);
// 	//k_SU=NMC_SU/numWjets;
//       } else {
// 	w_SU=-fSU_Val;
// 	C_SU=1/(1+w_SU*numWjets/NMC_SD);
// 	//k_SU=NMC_SD/numWjets;
//       }
      
//       if ( fMU_Val>0 ) {
// 	w_MU=fMU_Val;
// 	C_MU=1/(1+w_MU*numWjets/NMC_MU);
// 	//k_MU=NMC_MU/numWjets;
//       } else {
// 	w_MU=-fMU_Val;
// 	C_MU=1/(1+w_MU*numWjets/NMC_MD);
// 	//k_MU=NMC_MD/numWjets;
//       }
//       w_WpJ=(1-w_SU-w_MU);
//       if ( w_WpJ<0 ) {
// 	dofill=false;
//       }

//       C_WpJ=1/(1+w_WpJ*numWjets/NMC_WpJ);
//       //k_WpJ=NMC_WpJ/numWjets;

//       chi2=chi2log*ndof;
//       chi2corr=chi2*( (w_WpJ*C_WpJ+w_SU*C_SU+w_MU*C_MU)*numWjets/numTotal + 1-numWjets/numTotal);

      //fill the tree
      //      if ( dofill ) {
      OutTree->Fill();
      //}
      // reset and go a step up
      //dofill=true;
      numTotal=0;
      fMU_Val=fMU_Val+matchingUpStep;
    }
    fMU_Val=matchingUpStart;
    fSU_Val=fSU_Val+scaleUpStep;
  }
  
  outfile->Write();
  outfile->Close();

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void getContourExtrema(double nll_Contour, const char* inFileName = defaultsummaryfile)
//// Get the fMU, fSU corresponding to the minimum and maxumum numDiboson on the contour (technically anywhere on the surface) defined by nll<nll_Contour
{
 
  double fMU_Val,fSU_Val,numDiboson,numWjets,errDiboson,errWjets,numTotal,nll;//,chi2corr;

  TFile* f = new TFile(inFileName);
  TTree* InTree = (TTree*)f->Get("SummaryTree");
  InTree->SetBranchAddress("numDiboson",&numDiboson);
  InTree->SetBranchAddress("errDiboson",&errDiboson);
  InTree->SetBranchAddress("numWjets",&numWjets);
  InTree->SetBranchAddress("errWjets",&errWjets);
  InTree->SetBranchAddress("numTotal",&numTotal);
  InTree->SetBranchAddress("fMU_Val",&fMU_Val);
  InTree->SetBranchAddress("fSU_Val",&fSU_Val);
  InTree->SetBranchAddress("nll",&nll);

  double numWjets_Min=100000.0;
  double numWjets_Max=-1.0;
  double fMU_Min, fMU_Max, fSU_Min, fSU_Max, nll_Min, nll_Max;

  int nEntries;
  nEntries=InTree->GetEntries();
  cout << "nEntries=" << nEntries << endl;

  for (Int_t i=0; i<nEntries; i++) {
    InTree->GetEntry(i);
    if ( nll<nll_Contour ) {
      if ( numWjets<numWjets_Min ) {
	numWjets_Min=numWjets;
	fMU_Min=fMU_Val;
	fSU_Min=fSU_Val;
	nll_Min=nll;
      }
      if ( numWjets>numWjets_Max ) {
	numWjets_Max=numWjets;
	fMU_Max=fMU_Val;
	fSU_Max=fSU_Val;
	nll_Max=nll;
      }
    }
  }

  cout << "Maximum : numWjets=" << numWjets_Max << ", fMU=" << fMU_Max << ", fSU=" << fSU_Max << " (nll=" << nll_Max << ")" << endl;
  cout << "Minimum : numWjets=" << numWjets_Min << ", fMU=" << fMU_Min << ", fSU=" << fSU_Min << " (nll=" << nll_Min << ")" << endl;

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void DrawNLLSurface(double scaleUpStart, double scaleUpStep, int NscaleUpSteps, double matchingUpStart, double matchingUpStep, int NmatchingUpSteps, double nllMin, const char* TitleName, const char* SaveNamePrefix = 0, const char* SaveNameSuffix = 0, const char* inFileName  = defaultsummaryfile, char* AddRest=0)
//// Draw nll+nllMin vs fMU_Val:fSU_Val
{
  double matchingMin, matchingMax, scaleMin, scaleMax;
  TString varString;
  char nllMin_char[15];
  TFile* f = new TFile(inFileName);
  TTree* InTree = (TTree*)f->Get("SummaryTree");

  //  TH1D* hist = new TH1D("hist","hist",25,VarMin,VarMax);
  // SummaryTree->Draw("fMU_Val:fSU_Val>>hs","nll+172750","lego")

  matchingMin=matchingUpStart-matchingUpStep/2;
  matchingMax=matchingUpStart+matchingUpStep*NmatchingUpSteps-matchingUpStep/2;
  scaleMin=scaleUpStart-scaleUpStep/2;
  scaleMax=scaleUpStart+scaleUpStep*NscaleUpSteps-scaleUpStep/2;

  cout << "matchingMin=" << matchingMin << " matchingMax=" << matchingMax << " scaleMin=" << scaleMin << " scaleMax=" << scaleMax << endl;

  TH2D* hist = new TH2D("hist","hist",NscaleUpSteps,scaleMin,scaleMax,NmatchingUpSteps,matchingMin,matchingMax);
  TCanvas *cnv = new TCanvas("cnv","cnv",10,10,900,600);

  sprintf(nllMin_char,"%8.6f",-nllMin);
  varString=nllMin_char;
  varString="nll+"+varString;
  if (AddRest!=0) {
    varString = "(" + varString;
    varString = varString + ")*(";
    varString = varString + AddRest;
    varString = varString + ")";
  }
  
  cout << "varString=" << varString << endl;
  InTree->Draw("fMU_Val:fSU_Val>>hist",varString,"surf2z");

  gStyle->SetOptStat(0);
  gStyle->SetTitleX(0.3);
  gStyle->SetTitleY(0.96);
  hist->SetXTitle("fSU");
  hist->GetXaxis()->SetTitleOffset(1.4);
  hist->SetYTitle("fMU");
  hist->GetYaxis()->SetTitleOffset(1.7);
  hist->SetZTitle("nll-nll_{min}");
  hist->SetTitle(TitleName);

  /// Save the output
  if ( SaveNamePrefix!=0 ) {
    TString SaveName;
    //Surface Plot
    SaveName=SaveNameSuffix;
    SaveName="_surf"+SaveName;
    SaveName=SaveNamePrefix+SaveName;
    cnv->SaveAs(SaveName);

    //Contour Plot
    hist->GetXaxis()->SetTitleOffset(1.0);
    hist->GetYaxis()->SetTitleOffset(1.0);
    hist->Draw("cont4z");
    SaveName=SaveNameSuffix;
    SaveName="_cont"+SaveName;
    SaveName=SaveNamePrefix+SaveName;
    cnv->SaveAs(SaveName);

  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

