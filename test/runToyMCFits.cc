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
 * Description: Use to constuct, fit and analyze toy datasets
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
#include <TVector.h>
#include <TMatrix.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TH1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom3.h>

using namespace std;

////const char* resultsdir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_results/";
const char* mcSetprefix = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSets/MCSet_";
const char* mcNameprefix = "MCSet_";
const char* logprefix = "1KSetToy";
// //Locally
//const char* logDir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidationLocal_logs/";
//Via CONDOR:
const char* logDir = "./";

const int NProcesses=8;

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void runToyMCFits(int NJets, int NStart, int NEnd, const char* leptonPrefix = "mu_", bool runNoOptLocal = false , const char* logdir=logDir) {
  TString Command,LogName,ConfigName,ParamfileName;
  char I_char[5];
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);
  TString NJ_str=NJ_char;
  TString L_str=leptonPrefix;

  for (Int_t i=NStart; i<(NEnd+1); i++) {
    //make the log file
    sprintf(I_char,"%i",i);
    LogName=".log";
    LogName=I_char+LogName;
    LogName="Sample"+LogName;
    LogName="j_"+LogName;
    LogName=NJ_char+LogName;
    LogName=L_str+LogName;
    LogName="_"+LogName;
    LogName=logprefix+LogName;

//     if ( !runNoOptLocal ) {
//       ///Run The Optimization
//       Command="Optlog_"+LogName;
//       Command=logdir+Command;
//       Command="CondorEOS > "+Command;
//       Command = L_str+Command;
//       Command = "Jets.txt -m MjjOptimizeConfig_toydataFits"+Command;//Typical config is of the form MjjOptimizeConfig_toydataFitsmu_CondorEOS.py
//       Command = NJ_char + Command;
//       Command = L_str+Command;
//       Command = " -i MjjNominal" + Command;
//       Command = NJ_char + Command;//Typical nominal input param file is of the form MjjNominalmu_2Jets.txt
//       Command = " -j " + Command;
//       //set precision
//       Command = " -p 2 " + Command;
//       //provide the file to fit on
//       Command = ".root" + Command;
//       Command = I_char+Command;
//       Command = "j_"+Command;
//       Command = NJ_char+Command;
//       Command = L_str+Command;
//       Command = mcNameprefix + Command;
//       Command = " --TD " + Command;
//       Command = "python newOptimize.py " + Command;
//       cout << "Command=" << Command << endl;
//       system(Command);
//     }

    ///Run The Fit
    Command="Fitlog_"+LogName;
    Command=logdir+Command;
    if ( !runNoOptLocal ) {
      Command = "CondorEOS > "+Command;
      Command = L_str+Command;
      Command = "Jets.txt -m MjjOptimizeConfig_toydataFits"+Command;
    } else {
      Command = "Local > "+Command;
      Command = L_str+Command;
      Command = "Jets.txt -m MjjOptimizeConfig_toydataFits"+Command;
    }

    Command = NJ_char + Command;
    Command = L_str+Command;
    Command = " -i MjjNominal" + Command;
    Command = NJ_char + Command;
    Command = " -j " + Command;
    Command = ".root" + Command;
    Command = I_char+Command;
    Command = "j_"+Command;
    Command = NJ_char+Command;
    Command = L_str+Command;
    Command = mcNameprefix + Command;
    Command = " --TD " + Command;
    Command = "python runMjjFitter.py " + Command;
    cout << "Command=" << Command << endl;
    system(Command);

//     if ( !runNoOptLocal ) {
//       ///Copy the default parameter file to the one used to perform the fit (only needed if reoptimizing)
//       Command = "Jets.txt";
//       Command = NJ_char + Command;
//       Command = L_str+Command; 
//       Command = "Jets_def.txt MjjNominal" + Command;
//       Command = NJ_char + Command;
//       Command = L_str+Command;
//       Command = "cp MjjNominal" + Command;
//       cout << "Command=" << Command << endl;
//       system(Command);
//     }

  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

void GetCovMatrixAndCountsFromLog(const char* logfileName, TMatrixDSym & Cov, TVectorD& MeanPar) {
  TString sPar1, sPar2, sPar3, sPar4;
  double dParV[NProcesses];
  char logline[2000];
  ifstream inLogFile(logfileName);
  //cout << "4" << endl;
  while ( inLogFile.good() ) {
    inLogFile.getline(logline,2000);
    istrstream str(logline);
    //cout << "5" << endl;
    str >> sPar1 >> sPar2 >> sPar3 >> sPar4;

    ///Check the Convergence Status from HESSE
    if ( sPar3=="**HESSE" ) {
      for (Int_t dummy=0; dummy<3; dummy++) {
	inLogFile.getline(logline,2000);
      }
      istrstream str1(logline);
      str1 >> sPar1 >> sPar2 >> sPar3 >> sPar4;
      cout << " HESSE_Convergence = " << sPar4 << endl;

      /// Get the Covariance Matrix
      for (Int_t j=0; j<5+NProcesses; j++) {
	inLogFile.getline(logline,2000);
      }
      for (Int_t j=0; j<NProcesses; j++) {
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str2 >> dParV[0] >> dParV[1] >> dParV[2] >> dParV[3] >> dParV[4] >> dParV[5] >> dParV[6] >> dParV[7];
	for (Int_t k=0; k<NProcesses; k++) {
	  Cov(j,k)=dParV[k];
	}
      }
      /// Get the Mean Values
      for (Int_t j=0; j<8+NProcesses; j++) {
	inLogFile.getline(logline,2000);
      }
      for (Int_t j=0; j<NProcesses; j++) {
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str2 >> sPar1 >> dParV[1] >> dParV[2];
	MeanPar(j)=dParV[2];
      }
    }

  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

void createTestDatasets(int NJets, int InitRand, const char* leptonPrefix = "", int NStart=0, int NEnd=999, const char* outFileName = mcSetprefix)
//// Combines the input yields in appropriate amounts (randomized according to a Poisson distribution) and saves them in a file of the form form mcSetprefix+leptonPrefix+#j_*.root with *=NStart-(NEnd+1),#=NJets
//// leptonPrefix = "", mu_, el_
{
  TRandom PRand(InitRand);
  TRandom GRand(InitRand*137-2011);
  char I_char[5];
  double mjj, mjj_out;
  char NJ_char[5];
  sprintf(NJ_char,"%i",NJets);
  TString NJ_str=NJ_char;
  TString L_str=leptonPrefix;
  TMatrixDSym FullCov(NProcesses);//The Full Covariance Matrix 
  TVectorD Par(NProcesses);//Yields and fractions
  //TMatrixDSym YieldCov(6);
  TString fitLogPrefix="Fit";
  TString fitLogName="j.log";
  fitLogName=NJ_str+fitLogName;
  fitLogName=L_str+fitLogName;
  fitLogName=fitLogPrefix+fitLogName;
  cout << "fitLogName=" << fitLogName << endl;
  //GetCovMatrixAndCountsFromLog("Fit2j.log",FullCov,Par);
  GetCovMatrixAndCountsFromLog(fitLogName,FullCov,Par);
  const int WjIdx=6;
  TMatrixDSymEigen CM(FullCov);
  double covErr=0.0;
  cout << "Covariance Matrix = " << endl;
  for (Int_t j=2; j<NProcesses;j++) {
    for (Int_t k=2; k<NProcesses;k++) {
      cout << FullCov(j,k) << " ";
      covErr=covErr+FullCov(j,k);
    }
    cout << endl;
  }
  cout << "Total Error=" << pow(covErr,0.5) << endl;

  cout << "Expected Yields = " << endl;
  for (Int_t j=0; j<NProcesses;j++) {
      cout << Par(j) << " ";
  }
  cout << endl;

  TVectorD sigsqVal(NProcesses);
  sigsqVal = CM.GetEigenValues();
  TVectorD errVal(NProcesses);
  TMatrixD EVec = CM.GetEigenVectors();
  for (Int_t j=0; j<NProcesses;j++) {
    errVal(j)=pow(sigsqVal(j),0.5);
  }
  TVectorD RErr(NProcesses);//Errors (to be generated) in the Rotated System
  TVectorD UnRErr(NProcesses);//Errors in the physical coordinate system

//   ///Set the values and the errors from the fit for each process for 4.7fb fit
//   //Total=95904
//   Int_t NDiboson=2459;
//   Int_t Nqcd=2636;
//   Int_t Ntt=5823;
//   Int_t NsingleTop=2346;
//   Int_t Nzjets=2137;
//   Int_t NWjjDef=61826;//NWjjTot=80503
//   Int_t NWjjSU=16101;
//   Double_t fSU=0.200;
//   Int_t NWjjMU=2576;
//   Double_t fMU=0.032;
//   Int_t ErrDiboson=232;
//   Int_t Errqcd=1191;
//   Int_t Errtt=349;
//   Int_t ErrsingleTop=115;
//   Int_t Errzjets=92;
//   Int_t ErrWjjTot=1275;
//   Double_t WjjTotGausErr=0.0;

//   if ( NJets==3 ) {
//     //Total=37563
//     NDiboson=553;
//     Nqcd=436;
//     Ntt=12310;
//     NsingleTop=1415;
//     Nzjets=770;
//     NWjjDef=17023;//NWjjTot=22079
//     NWjjSU=2716;
//     fSU=0.123;
//     NWjjMU=2340;
//     fMU=0.106;
//     ErrDiboson=79;
//     Errqcd=192;
//     Errtt=483;
//     ErrsingleTop=69;
//     Errzjets=33;
//     ErrWjjTot=511;
//   }

//   ///Set the values and the errors from the fit for each process for 2.1fb fit
//   //Total=43643
//   Int_t NDiboson=1004;
//   Int_t Nqcd=1368;
//   Int_t Ntt=3120;
//   Int_t NsingleTop=1182;
//   Int_t Nzjets=797;
//   Int_t NWjjDef=14903;//NWjjTot=36172
//   Int_t NWjjSU=7488;
//   Double_t fSU=0.207;
//   Int_t NWjjMU=13781;
//   Double_t fMU=0.381;
//   Int_t ErrDiboson=128;
//   Int_t Errqcd=588;
//   Int_t Errtt=188;
//   Int_t ErrsingleTop=59;
//   Int_t Errzjets=34;
//   Int_t ErrWjjTot=664;
//   Double_t WjjTotGausErr=0.0;

//   if ( NJets==3 ) {
//     //Total=16452
//     NDiboson=242;
//     Nqcd=159;
//     Ntt=5308;
//     NsingleTop=652;
//     Nzjets=260;
//     NWjjDef=8465;//NWjjTot=9831
//     NWjjSU=737;
//     fSU=0.075;
//     NWjjMU=629;
//     fMU=0.064;
//     ErrDiboson=36;
//     Errqcd=86;
//     Errtt=274;
//     ErrsingleTop=32;
//     Errzjets=11;
//     ErrWjjTot=296;
//   }

  Int_t Nevt[NProcesses+2];
  Int_t Nentry[NProcesses+2];
  for (Int_t j=0; j<(NProcesses+2);j++) {
    Nentry[j]=0;
  }

  const char* sourcedir = "/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSource/";
  TFile *f[NProcesses+2];
  TString sourcedir_str=sourcedir;

  f[0]= new TFile(sourcedir_str+"WjjMU_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[1]= new TFile(sourcedir_str+"WjjSU_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[2]= new TFile(sourcedir_str+"Diboson_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[3]= new TFile(sourcedir_str+"qcd_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[4]= new TFile(sourcedir_str+"singleTop_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[5]= new TFile(sourcedir_str+"tt_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[6]= new TFile(sourcedir_str+"WjjDef_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[7]= new TFile(sourcedir_str+"zjets_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[8]= new TFile(sourcedir_str+"WjjMD_"+ L_str + NJ_str +"j_GenMC.root", "READ");
  f[9]= new TFile(sourcedir_str+"WjjSD_"+ L_str + NJ_str +"j_GenMC.root", "READ");

  TTree *InTree[NProcesses+2];
  InTree[0] = (TTree*) f[0]->Get("WpJPdfData");
  InTree[1] = (TTree*) f[1]->Get("WpJPdfData");
  InTree[2] = (TTree*) f[2]->Get("dibosonPdfData");
  InTree[3] = (TTree*) f[3]->Get("qcdPdfData");
  InTree[4] = (TTree*) f[4]->Get("stPdfData");
  InTree[5] = (TTree*) f[5]->Get("ttPdfData");
  InTree[6] = (TTree*) f[6]->Get("WpJPdfData");
  InTree[7] = (TTree*) f[7]->Get("ZpJPdfData");
  InTree[8] = (TTree*) f[8]->Get("WpJPdfData");
  InTree[9] = (TTree*) f[9]->Get("WpJPdfData");

  InTree[0]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[1]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[2]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[3]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[4]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[5]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[6]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[7]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[8]->SetBranchAddress("Mass2j_PFCor",&mjj);
  InTree[9]->SetBranchAddress("Mass2j_PFCor",&mjj);

  TString outfilename;
  for (Int_t k=NStart; k<(NEnd+1);k++) {
    //    cout << "Generating File #" << k << endl;
    outfilename=".root";
    sprintf(I_char,"%i",k);
    outfilename=I_char+outfilename;
    outfilename="j_"+outfilename;
    outfilename=NJ_char+outfilename;
    outfilename=L_str+outfilename;
    //    outfilename="_"+outfilename;
    outfilename=outFileName+outfilename;
    TFile *fout = new TFile(outfilename, "RECREATE");
    TTree* TMCTree = new TTree("WJet", "Tree from toy MC");
    TMCTree->Branch("Mass2j_PFCor",&mjj_out,"Mass2j_PFCorr/D");
    TMCTree->Branch("evtNJ",&NJets,"evtNJ/I");

    for (Int_t j=0; j<NProcesses;j++) {
      RErr(j)=GRand.Gaus(0,errVal(j));
    }
    UnRErr=EVec*RErr;
    for (Int_t j=0; j<NProcesses;j++) {
      //Indices for fMU, fSU are 0&1 and WjIdx for WJets
      if ( (j==0)||(j==1) ) {
	Nevt[j]=(Par(j)+UnRErr(j))*(Par(WjIdx)+UnRErr(WjIdx));
      } else {
	if ( j==WjIdx ) {
	  Nevt[j]=(1-Par(0)-UnRErr(0)-Par(1)-UnRErr(1))*(Par(WjIdx)+UnRErr(WjIdx));
	} else {
	  Nevt[j]=Par(j)+UnRErr(j);
	}
      }
    }

    /// Perform the Poisson Smearing
    for (Int_t j=0; j<NProcesses;j++) {
      if ( (Nevt[j]<0)&&((j==0)||(j==1)) ) {
	//If we need to use MatchingDown or ScaleDown events:
	Nevt[NProcesses+j]=PRand.Poisson(-Nevt[j]);
	Nevt[j]=-Nevt[NProcesses+j];
      } else {
	Nevt[j]=PRand.Poisson(Nevt[j]);
      }
    }

//     Nevt[0]=PRand.Poisson(NDiboson)+GRand.Gaus(0,ErrDiboson);
//     Nevt[1]=PRand.Poisson(Nqcd)+GRand.Gaus(0,Errqcd);
//     Nevt[2]=PRand.Poisson(Ntt)+GRand.Gaus(0,Errtt);
//     Nevt[3]=PRand.Poisson(NsingleTop)+GRand.Gaus(0,ErrsingleTop); 
//     Nevt[4]=PRand.Poisson(Nzjets)+GRand.Gaus(0,Errzjets);
//     WjjTotGausErr=GRand.Gaus(0,ErrWjjTot);
//     Nevt[5]=PRand.Poisson(NWjjDef)+WjjTotGausErr*(1.0-fSU-fMU);
//     Nevt[6]=PRand.Poisson(NWjjSU)+WjjTotGausErr*fSU;
//     Nevt[7]=PRand.Poisson(NWjjMU)+WjjTotGausErr*fMU;

    for (Int_t j=0; j<NProcesses;j++) {
      if ( (Nevt[j]<0)&&((j==0)||(j==1)) ) {
	//If we need to use MatchingDown or ScaleDown events:
	for (Int_t i=Nentry[NProcesses+j]; i<(Nentry[NProcesses+j]+Nevt[NProcesses+j]);i++) {
	  InTree[j]->GetEntry(i);
	  mjj_out=mjj;
	  TMCTree->Fill();
	}
	Nentry[NProcesses+j]=Nentry[NProcesses+j]+Nevt[NProcesses+j];
      } else {
	for (Int_t i=Nentry[j]; i<(Nentry[j]+Nevt[j]);i++) {
	  InTree[j]->GetEntry(i);
	  mjj_out=mjj;
	  TMCTree->Fill();
	}
	Nentry[j]=Nentry[j]+Nevt[j];
      }

    }
    fout->Write();
    fout->Close();
    cout << "Generated File " << k << "  Event Counts:" << endl;
    cout << "NMU=" << Nevt[0] << " NSU=" << Nevt[1] << " NDiboson=" << Nevt[2] << " NQCD =" << Nevt[3] << " NSingleTop=" << Nevt[4] << " NTTbar=" << Nevt[5] << " NWjetsDef=" << Nevt[6] << " NZjets=" << Nevt[7] << " NTotal=" << Nevt[1]+Nevt[2]+Nevt[3]+Nevt[4]+Nevt[5]+Nevt[6]+Nevt[7] << endl;
  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------//

void processLogs(int NLogs=1000, const char* processlogPrefix="/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_logs/Fitlog_1KSetToy_mu_3j_Sample", const char* outFileName="/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_results/1KDataSetFit_mu_3j_LogSummary.root")
//// Processes the logs and produces an output file. Log files are of the form processlogPrefix*.log, where *=0-(NLogs-1)
{
  TString inLogFileName, sPar1, sPar2, sPar3, sPar4;
  char logline[2000];
  char I_char[5];
  double dPar,dErr;
  //  double dParV[6];
  double numDiboson,errDiboson,numQCD,errQCD,numSingleTop,errSingleTop,numTTbar,errTTbar,numWjets,errWjets,numZjets,errZjets,numTotal,errTotal,errTotalCov,nll,numfSU,numfMU,errfSU,errfMU;
  int Convergence;
  TMatrixDSym FullCov(NProcesses);
  TVectorD Par(NProcesses);
  //cout << "0" << endl;

  TFile *outfile = new TFile(outFileName, "RECREATE");
  TTree *OutTree = new TTree("OutTree","OutTree");

  //  cout << "1" << endl;
  OutTree->Branch("numDiboson",&numDiboson,"numDiboson/D");
  OutTree->Branch("errDiboson",&errDiboson,"errDiboson/D");
  OutTree->Branch("numQCD",&numQCD,"numQCD/D");
  OutTree->Branch("errQCD",&errQCD,"errQCD/D");
  OutTree->Branch("numSingleTop",&numSingleTop,"numSingleTop/D");
  OutTree->Branch("errSingleTop",&errSingleTop,"errSingleTop/D");
  OutTree->Branch("numTTbar",&numTTbar,"numTTbar/D");
  OutTree->Branch("errTTbar",&errTTbar,"errTTbar/D");
  OutTree->Branch("numWjets",&numWjets,"numWjets/D");
  OutTree->Branch("errWjets",&errWjets,"errWjets/D");
  OutTree->Branch("numZjets",&numZjets,"numZjets/D");
  OutTree->Branch("errZjets",&errZjets,"errZjets/D");
  OutTree->Branch("numTotal",&numTotal,"numTotal/D");
  OutTree->Branch("errTotal",&errTotal,"errTotal/D");
  OutTree->Branch("errTotalCov",&errTotalCov,"errTotalCov/D");//Sum of the covariance matrix elements.
  OutTree->Branch("nll",&nll,"nll/D");
  OutTree->Branch("numfSU",&numfSU,"numfSU/D");
  OutTree->Branch("numfMU",&numfMU,"numfMU/D");
  OutTree->Branch("errfSU",&errfSU,"errfSU/D");
  OutTree->Branch("errfMU",&errfMU,"errfMU/D");
  OutTree->Branch("Convergence",&Convergence,"Convergence/I");

  //  cout << "2" << endl;
  for (Int_t i=0; i<NLogs; i++) {
    inLogFileName=".log";
    sprintf(I_char,"%i",i);
    inLogFileName=I_char+inLogFileName;
    inLogFileName=processlogPrefix+inLogFileName;
    //  cout << "3" << endl;
    ifstream inLogFile(inLogFileName);
    //cout << "4" << endl;
    while ( inLogFile.good() ) {
      inLogFile.getline(logline,2000);
      istrstream str(logline);
      //cout << "5" << endl;
      str >> sPar1 >> sPar2 >> sPar3 >> sPar4;
//       if ( sPar3=="**MIGRAD" ) {
// 	for (Int_t dummy=0; dummy<14; dummy++) {
// 	  inLogFile.getline(logline,2000);
// 	}
// 	istrstream str1(logline);
// 	str1 >> sPar1 >> sPar2 >> sPar3 >> sPar4;
// 	cout << "i=" << i << " sPar4 = " << sPar4 << endl;
// 	if ( sPar4=="STATUS=CONVERGED" ) {
// 	  Convergence=1;
// 	} else {
// 	  Convergence=0;
// 	}
//       }

      ///Get the Convergence Status from HESSE
      if ( sPar3=="**HESSE" ) {
	for (Int_t dummy=0; dummy<3; dummy++) {
	  inLogFile.getline(logline,2000);
	}
	istrstream str1(logline);
	str1 >> sPar1 >> sPar2 >> sPar3 >> sPar4;
	cout << "i=" << i << " sPar4 = " << sPar4 << endl;
	if ( sPar4=="STATUS=OK" ) {
	  Convergence=1;
	} else {
	  Convergence=0;
	}
// 	/// Get the sum of the covariance matrix elements
// 	errTotalCov=0.0;
// 	for (Int_t j=0; j<13; j++) {
// 	  inLogFile.getline(logline,2000);
// 	}
// 	for (Int_t j=0; j<8; j++) {
// 	  inLogFile.getline(logline,2000);
// 	  istrstream str2(logline);
// 	  str2 >> dParV[0] >> dParV[1] >> dParV[2] >> dParV[3] >> dParV[4] >> dParV[5];
// 	  for (Int_t k=0; k<8; k++) {
// 	    errTotalCov=errTotalCov+dParV[k];
// 	  }
// 	}
// 	errTotalCov=pow(errTotalCov,0.5);
      }

      ///Get the parameters and errors returned by the fit
      if ( (sPar1=="Floating")&&(sPar2=="Parameter")&&(sPar3=="FinalValue") ) {
// 	inLogFile.getline(logline,2000);
// 	inLogFile.getline(logline,2000);
// 	istrstream str1(logline);
// 	str1 >> sPar1 >> dPar;
// 	if ( fabs(dPar)<0.0001 ) {
// 	  inLogFile.getline(logline,2000);
// 	  istrstream st(logline);
// 	  st >> sPar1 >> dPar;
// 	  fMU=dPar;
// 	} else {
// 	  fMU=-dPar;
// 	  inLogFile.getline(logline,2000);
// 	}
// 	inLogFile.getline(logline,2000);
// 	istrstream str2(logline);
// 	str2 >> sPar1 >> dPar;
// 	if ( fabs(dPar)<0.0001 ) {
// 	  inLogFile.getline(logline,2000);
// 	  istrstream st(logline);
// 	  st >> sPar1 >> dPar;
// 	  fSU=dPar;
// 	} else {
// 	  inLogFile.getline(logline,2000);
// 	  fSU=-dPar;
// 	}
// 	inLogFile.getline(logline,2000);
//	inLogFile.getline(logline,2000);

	inLogFile.getline(logline,2000);
	inLogFile.getline(logline,2000);
	istrstream str1(logline);
	str1 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numfMU=dPar;
	errfMU=dErr;
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str1 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numfSU=dPar;
	errfSU=dErr;
	inLogFile.getline(logline,2000);
	istrstream str3(logline);
	str3 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numDiboson=dPar;
	errDiboson=dErr;
	inLogFile.getline(logline,2000);
	istrstream str4(logline);
	str4 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numQCD=dPar;
	errQCD=dErr;
	inLogFile.getline(logline,2000);
	istrstream str5(logline);
	str5 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numSingleTop=dPar;
	errSingleTop=dErr;
	inLogFile.getline(logline,2000);
	istrstream str6(logline);
	str6 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numTTbar=dPar;
	errTTbar=dErr;
	inLogFile.getline(logline,2000);
	istrstream str7(logline);
	str7 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numWjets=dPar;
	errWjets=dErr;
	inLogFile.getline(logline,2000);
	istrstream str8(logline);
	str8 >> sPar1 >>  dPar >> sPar2 >> dErr;
	numZjets=dPar;
	errZjets=dErr;
	//	cout << "nWjets = " << numWjets << " +/- " << errWjets << endl;
      }

      ///Get the nll and Total Yield
      if ( (sPar1=="*****")&&(sPar2=="nll") ) {
	istrstream str1(logline);
	str1 >> sPar1 >> sPar2 >> sPar3 >> dPar;
	nll=dPar;
	inLogFile.getline(logline,2000);
	inLogFile.getline(logline,2000);
	istrstream str2(logline);
	str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
	numTotal=dPar;
	errTotal=dErr;
	//cout << "nll = " << nll << endl;
      }

//       //get the errors returned by the fit
//       if ( (sPar1=="Floating")&&(sPar2=="Parameter") ) {
// 	inLogFile.getline(logline,2000);
// 	inLogFile.getline(logline,2000);
// 	istrstream str1(logline);
// 	str1 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
// 	//numDiboson=dPar;
// 	errDiboson=dErr;
// 	inLogFile.getline(logline,2000);
// 	istrstream str2(logline);
// 	str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
// 	//numWjets=dPar;
// 	errWjets=dErr;
//       }

//       if ( (sPar2=="Printing")&&(sPar3=="yields") ) {
// 	inLogFile.getline(logline,2000);
// 	istrstream str1(logline);
// 	str1 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
// 	numDiboson=dPar;
// 	//errDiboson=dErr;
// 	inLogFile.getline(logline,2000);
// 	istrstream str2(logline);
// 	str2 >> sPar1 >> sPar2 >> dPar >> sPar3 >> dErr;
// 	numWjets=dPar;
// 	//errWjets=dErr;
// 	inLogFile.getline(logline,2000);
// 	istrstream str3(logline);
// 	str3 >> sPar1 >> sPar2 >> dPar;
// 	numTotal=dPar;
//       }



    }

    GetCovMatrixAndCountsFromLog(inLogFileName,FullCov,Par);
    errTotalCov=0.0;
    for (Int_t j=2; j<NProcesses;j++) {
      for (Int_t k=2; k<NProcesses;k++) {
	errTotalCov=errTotalCov + FullCov(j,k);
      }
    }
    errTotalCov=pow(errTotalCov,0.5);

    cout << "errTotal=" << errTotal << " errTotalCov=" << errTotalCov << endl;
    OutTree->Fill();
  }
  
  outfile->Write();
  outfile->Close();

}



