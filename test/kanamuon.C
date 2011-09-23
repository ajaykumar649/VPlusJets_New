#define kanamuon_cxx
#include "kanamuon.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "Resolution.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleCart.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"

#include "ElectroWeakAnalysis/VPlusJets/interface/AngularVars.h"

#include "ElectroWeakAnalysis/VPlusJets/interface/METzCalculator.h"

void kanamuon::myana(double myflag)
{
  TChain * myChain;
  myflag = 20110000;

  // 2011 data
  if (myflag == 20111000 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/WmunuJets_DataAll_GoldenJSON_2p1invfb.root"); 
    Init(myChain);Loop( 20110000,"./OutData/RD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root");
  }

  // General Background Samples
  if (myflag == 20111002 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopS_Tbar_CMSSW428.root"); 
    Init(myChain);Loop( 20111002,"./OutData/RD_mu_STopS_Tbar_CMSSW428.root");
  }
  if (myflag == 20111003 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopS_T_CMSSW428.root"); 
    Init(myChain);Loop( 20111003,"./OutData/RD_mu_STopS_T_CMSSW428.root");
  }
  if (myflag == 20111004 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopT_Tbar_CMSSW428.root"); 
    Init(myChain);Loop( 20111004,"./OutData/RD_mu_STopT_Tbar_CMSSW428.root");
  }
  if (myflag == 20111005 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopT_T_CMSSW428.root"); 
    Init(myChain);Loop( 20111005,"./OutData/RD_mu_STopT_T_CMSSW428.root");
  }
  if (myflag == 20111006 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopTW_Tbar_CMSSW428.root"); 
    Init(myChain);Loop( 20111006,"./OutData/RD_mu_STopTW_Tbar_CMSSW428.root");
  }
  if (myflag == 20111007 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_STopTW_T_CMSSW428.root"); 
    Init(myChain);Loop( 20111007,"./OutData/RD_mu_STopTW_T_CMSSW428.root");
  }
  if (myflag == 20111008 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_TTbar_MG_CMSSW428.root"); 
    Init(myChain);Loop( 20111008,"./OutData/RD_mu_TTbar_MG_CMSSW428.root");
  }
  if (myflag == 20111009 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJ_CMSSW428.root"); 
    Init(myChain);Loop( 20111009,"./OutData/RD_mu_WpJ_CMSSW428.root");
  }
  if (myflag == 20111010 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJmatchingdown_CMSSW428.root"); 
    Init(myChain);Loop( 20111010,"./OutData/RD_mu_WpJmatchingdown_CMSSW428.root");
  }
  if (myflag == 20111011 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJmatchingup_CMSSW428.root"); 
    Init(myChain);Loop( 20111011,"./OutData/RD_mu_WpJmatchingup_CMSSW428.root");
  }
  if (myflag == 20111012 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJscaledown_CMSSW428.root"); 
    Init(myChain);Loop( 20111012,"./OutData/RD_mu_WpJscaledown_CMSSW428.root");
  }
  if (myflag == 20111013 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJscaleup_CMSSW428.root"); 
    Init(myChain);Loop( 20111013,"./OutData/RD_mu_WpJscaleup_CMSSW428.root");
  }
  if (myflag == 20111014 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WpJsherpa_CMSSW428.root"); 
    Init(myChain);Loop( 20111014,"./OutData/RD_mu_WpJsherpa_CMSSW428.root");
  }
  if (myflag == 20111015 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WW_CMSSW428.root"); 
    Init(myChain);Loop( 20111015,"./OutData/RD_mu_WW_CMSSW428.root");
  }
  if (myflag == 20111016 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_WZ_CMSSW428.root"); 
    Init(myChain);Loop( 20111016,"./OutData/RD_mu_WZ_CMSSW428.root");
  }
  if (myflag == 20111017 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_ZpJ_CMSSW428.root"); 
    Init(myChain);Loop( 20111017,"./OutData/RD_mu_ZpJ_CMSSW428.root");
  }
  if (myflag == 20111018 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_QCDMu_CMSSW428.root"); 
    Init(myChain);Loop( 20111018,"./OutData/RD_mu_QCDMu_CMSSW428.root");
  }
  if (myflag == 20111019 || myflag ==  999){ // set 999 not run!! no need
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_TTbar_powheg_CMSSW428.root"); 
    Init(myChain);Loop( 20111019,"./OutData/RD_mu_TTbar_powheg_CMSSW428.root");
  }

  // Higgs Signal Samples
  if (myflag == 20112120 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH120_CMSSW428.root"); 
    Init(myChain);Loop( 20112120,"./OutData/RD_mu_HWWMH120_CMSSW428.root");
  }
  if (myflag == 20112130 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH130_CMSSW428.root"); 
    Init(myChain);Loop( 20112130,"./OutData/RD_mu_HWWMH130_CMSSW428.root");
  }
  if (myflag == 20112140 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH140_CMSSW428.root"); 
    Init(myChain);Loop( 20112140,"./OutData/RD_mu_HWWMH140_CMSSW428.root");
  }
  if (myflag == 20112150 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH150_CMSSW428.root"); 
    Init(myChain);Loop( 20112150,"./OutData/RD_mu_HWWMH150_CMSSW428.root");
  }
  if (myflag == 20112160 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH160_CMSSW428.root"); 
    Init(myChain);Loop( 20112160,"./OutData/RD_mu_HWWMH160_CMSSW428.root");
  }
  if (myflag == 20112170 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH170_CMSSW428.root"); 
    Init(myChain);Loop( 20112170,"./OutData/RD_mu_HWWMH170_CMSSW428.root");
  }
  if (myflag == 20112180 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH180_CMSSW428.root"); 
    Init(myChain);Loop( 20112180,"./OutData/RD_mu_HWWMH180_CMSSW428.root");
  }
  if (myflag == 20112190 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH190_CMSSW428.root"); 
    Init(myChain);Loop( 20112190,"./OutData/RD_mu_HWWMH190_CMSSW428.root");
  }
  if (myflag == 20112200 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH200_CMSSW428.root"); 
    Init(myChain);Loop( 20112200,"./OutData/RD_mu_HWWMH200_CMSSW428.root");
  }
  if (myflag == 20112250 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH250_CMSSW428.root"); 
    Init(myChain);Loop( 20112250,"./OutData/RD_mu_HWWMH250_CMSSW428.root");
  }
  if (myflag == 20112300 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH300_CMSSW428.root"); 
    Init(myChain);Loop( 20112300,"./OutData/RD_mu_HWWMH300_CMSSW428.root");
  }
  if (myflag == 20112350 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH350_CMSSW428.root"); 
    Init(myChain);Loop( 20112350,"./OutData/RD_mu_HWWMH350_CMSSW428.root");
  }
  if (myflag == 20112400 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH400_CMSSW428.root"); 
    Init(myChain);Loop( 20112400,"./OutData/RD_mu_HWWMH400_CMSSW428.root");
  }
  if (myflag == 20112450 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH450_CMSSW428.root"); 
    Init(myChain);Loop( 20112450,"./OutData/RD_mu_HWWMH450_CMSSW428.root");
  }
  if (myflag == 20112500 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH500_CMSSW428.root"); 
    Init(myChain);Loop( 20112500,"./OutData/RD_mu_HWWMH500_CMSSW428.root");
  }
  if (myflag == 20112550 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH550_CMSSW428.root"); 
    Init(myChain);Loop( 20112550,"./OutData/RD_mu_HWWMH550_CMSSW428.root");
  }
  if (myflag == 20112600 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_HWWMH600_CMSSW428.root"); 
    Init(myChain);Loop( 20112600,"./OutData/RD_mu_HWWMH600_CMSSW428.root");
  }

  // VBF Higgs MC Signal
  if (myflag == 20113120 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH120_CMSSW428.root"); 
    Init(myChain);Loop( 20113120,"./OutData/RD_mu_VBFHWWMH120_CMSSW428.root");
  }
  if (myflag == 20113130 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH130_CMSSW428.root"); 
    Init(myChain);Loop( 20113130,"./OutData/RD_mu_VBFHWWMH130_CMSSW428.root");
  }
  if (myflag == 20113140 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH140_CMSSW428.root"); 
    Init(myChain);Loop( 20113140,"./OutData/RD_mu_VBFHWWMH140_CMSSW428.root");
  }
  if (myflag == 20113150 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH150_CMSSW428.root"); 
    Init(myChain);Loop( 20113150,"./OutData/RD_mu_VBFHWWMH150_CMSSW428.root");
  }
  if (myflag == 20113160 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH160_CMSSW428.root"); 
    Init(myChain);Loop( 20113160,"./OutData/RD_mu_VBFHWWMH160_CMSSW428.root");
  }
  if (myflag == 20113170 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH170_CMSSW428.root"); 
    Init(myChain);Loop( 20113170,"./OutData/RD_mu_VBFHWWMH170_CMSSW428.root");
  }
  if (myflag == 20113180 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH180_CMSSW428.root"); 
    Init(myChain);Loop( 20113180,"./OutData/RD_mu_VBFHWWMH180_CMSSW428.root");
  }
  if (myflag == 20113190 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH190_CMSSW428.root"); 
    Init(myChain);Loop( 20113190,"./OutData/RD_mu_VBFHWWMH190_CMSSW428.root");
  }
  if (myflag == 20113200 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH200_CMSSW428.root"); 
    Init(myChain);Loop( 20113200,"./OutData/RD_mu_VBFHWWMH200_CMSSW428.root");
  }
  if (myflag == 20113250 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH250_CMSSW428.root"); 
    Init(myChain);Loop( 20113250,"./OutData/RD_mu_VBFHWWMH250_CMSSW428.root");
  }
  if (myflag == 20113300 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH300_CMSSW428.root"); 
    Init(myChain);Loop( 20113300,"./OutData/RD_mu_VBFHWWMH300_CMSSW428.root");
  }
  if (myflag == 20113350 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH350_CMSSW428.root"); 
    Init(myChain);Loop( 20113350,"./OutData/RD_mu_VBFHWWMH350_CMSSW428.root");
  }
  if (myflag == 20113400 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH400_CMSSW428.root"); 
    Init(myChain);Loop( 20113400,"./OutData/RD_mu_VBFHWWMH400_CMSSW428.root");
  }
  if (myflag == 20113450 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH450_CMSSW428.root"); 
    Init(myChain);Loop( 20113450,"./OutData/RD_mu_VBFHWWMH450_CMSSW428.root");
  }
  if (myflag == 20113500 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH500_CMSSW428.root"); 
    Init(myChain);Loop( 20113500,"./OutData/RD_mu_VBFHWWMH500_CMSSW428.root");
  }
  if (myflag == 20113550 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH550_CMSSW428.root"); 
    Init(myChain);Loop( 20113550,"./OutData/RD_mu_VBFHWWMH550_CMSSW428.root");
  }
  if (myflag == 20113600 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add(                    "./InData/mu_VBFHWWMH600_CMSSW428.root"); 
    Init(myChain);Loop( 20113600,"./OutData/RD_mu_VBFHWWMH600_CMSSW428.root");
  }
}

void kanamuon::Loop(int wda, const char *outfilename)
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  // Out Put File Here
  TFile fresults = TFile(outfilename,"RECREATE");
  TTree *newtree = fChain->CloneTree();
  char textfn[100]; 
  sprintf(textfn,"%s.txt", outfilename);
  FILE *textfile = fopen(textfn,"w");
  
  Int_t   gdevtt   =0,   evtNJ     =0;

  TBranch *branch_gdevtt= newtree->Branch("gdevtt",    &gdevtt,     "gdevtt/I");
  TBranch *branch_evtNJ = newtree->Branch("evtNJ",     &evtNJ,      "evtNJ/I");
   
  Float_t fit_mu_px=0,   fit_mu_py =0,   fit_mu_pz=0,   fit_mu_e=0;
  Float_t fit_nv_px=0,   fit_nv_py =0,   fit_nv_pz=0,   fit_nv_e=0;
  Float_t fit_aj_px=0,   fit_aj_py =0,   fit_aj_pz=0,   fit_aj_e=0;
  Float_t fit_bj_px=0,   fit_bj_py =0,   fit_bj_pz=0,   fit_bj_e=0;
  Float_t fit_mlvjj=0,   fit_chi2  =999;  
  Int_t   fit_NDF  =999, fit_status=999;
  Float_t fit_mlv  =0,   fit_mjj   =0;

  TBranch *branch_mu_px = newtree->Branch("fit_mu_px", &fit_mu_px,  "fit_mu_px/F");
  TBranch *branch_mu_py = newtree->Branch("fit_mu_py", &fit_mu_py,  "fit_mu_py/F");
  TBranch *branch_mu_pz = newtree->Branch("fit_mu_pz", &fit_mu_pz,  "fit_mu_pz/F");
  TBranch *branch_mu_e  = newtree->Branch("fit_mu_e",  &fit_mu_e,   "fit_mu_e/F");
  	   
  TBranch *branch_nv_px = newtree->Branch("fit_nv_px", &fit_nv_px,  "fit_nv_px/F");
  TBranch *branch_nv_py = newtree->Branch("fit_nv_py", &fit_nv_py,  "fit_nv_py/F");
  TBranch *branch_nv_pz = newtree->Branch("fit_nv_pz", &fit_nv_pz,  "fit_nv_pz/F");
  TBranch *branch_nv_e  = newtree->Branch("fit_nv_e",  &fit_nv_e,   "fit_nv_e/F");
  	   
  TBranch *branch_aj_px = newtree->Branch("fit_aj_px", &fit_aj_px,  "fit_aj_px/F");
  TBranch *branch_aj_py = newtree->Branch("fit_aj_py", &fit_aj_py,  "fit_aj_py/F");
  TBranch *branch_aj_pz = newtree->Branch("fit_aj_pz", &fit_aj_pz,  "fit_aj_pz/F");
  TBranch *branch_aj_e  = newtree->Branch("fit_aj_e",  &fit_aj_e,   "fit_aj_e/F");
  	   
  TBranch *branch_bj_px = newtree->Branch("fit_bj_px", &fit_bj_px,  "fit_bj_px/F");
  TBranch *branch_bj_py = newtree->Branch("fit_bj_py", &fit_bj_py,  "fit_bj_py/F");
  TBranch *branch_bj_pz = newtree->Branch("fit_bj_pz", &fit_bj_pz,  "fit_bj_pz/F");
  TBranch *branch_bj_e  = newtree->Branch("fit_bj_e",  &fit_bj_e,   "fit_bj_e/F");
  
  TBranch *branch_mlvjj = newtree->Branch("fit_mlvjj", &fit_mlvjj,  "fit_mlvjj/F");
  TBranch *branch_mlv   = newtree->Branch("fit_mlv",   &fit_mlv,    "fit_mlv/F");
  TBranch *branch_mjj   = newtree->Branch("fit_mjj",   &fit_mjj,    "fit_mjj/F");
  TBranch *branch_chi2  = newtree->Branch("fit_chi2",  &fit_chi2,   "fit_chi2/F");
  TBranch *branch_NDF   = newtree->Branch("fit_NDF",   &fit_NDF,    "fit_NDF/I");
  TBranch *branch_status= newtree->Branch("fit_status",&fit_status, "fit_status/I");

  Float_t fi2_mu_px=0,   fi2_mu_py =0,   fi2_mu_pz=0,   fi2_mu_e=0;
  Float_t fi2_nv_px=0,   fi2_nv_py =0,   fi2_nv_pz=0,   fi2_nv_e=0;
  Float_t fi2_aj_px=0,   fi2_aj_py =0,   fi2_aj_pz=0,   fi2_aj_e=0;
  Float_t fi2_bj_px=0,   fi2_bj_py =0,   fi2_bj_pz=0,   fi2_bj_e=0;
  Float_t fi2_mlvjj=0,   fi2_chi2  =999;  
  Int_t   fi2_NDF  =999, fi2_status=999;
  Float_t fi2_mlv  =0,   fi2_mjj   =0;

  TBranch *branc2_mu_px = newtree->Branch("fi2_mu_px", &fi2_mu_px,  "fi2_mu_px/F");
  TBranch *branc2_mu_py = newtree->Branch("fi2_mu_py", &fi2_mu_py,  "fi2_mu_py/F");
  TBranch *branc2_mu_pz = newtree->Branch("fi2_mu_pz", &fi2_mu_pz,  "fi2_mu_pz/F");
  TBranch *branc2_mu_e  = newtree->Branch("fi2_mu_e",  &fi2_mu_e,   "fi2_mu_e/F");
  	   
  TBranch *branc2_nv_px = newtree->Branch("fi2_nv_px", &fi2_nv_px,  "fi2_nv_px/F");
  TBranch *branc2_nv_py = newtree->Branch("fi2_nv_py", &fi2_nv_py,  "fi2_nv_py/F");
  TBranch *branc2_nv_pz = newtree->Branch("fi2_nv_pz", &fi2_nv_pz,  "fi2_nv_pz/F");
  TBranch *branc2_nv_e  = newtree->Branch("fi2_nv_e",  &fi2_nv_e,   "fi2_nv_e/F");
  	   
  TBranch *branc2_aj_px = newtree->Branch("fi2_aj_px", &fi2_aj_px,  "fi2_aj_px/F");
  TBranch *branc2_aj_py = newtree->Branch("fi2_aj_py", &fi2_aj_py,  "fi2_aj_py/F");
  TBranch *branc2_aj_pz = newtree->Branch("fi2_aj_pz", &fi2_aj_pz,  "fi2_aj_pz/F");
  TBranch *branc2_aj_e  = newtree->Branch("fi2_aj_e",  &fi2_aj_e,   "fi2_aj_e/F");
  	   
  TBranch *branc2_bj_px = newtree->Branch("fi2_bj_px", &fi2_bj_px,  "fi2_bj_px/F");
  TBranch *branc2_bj_py = newtree->Branch("fi2_bj_py", &fi2_bj_py,  "fi2_bj_py/F");
  TBranch *branc2_bj_pz = newtree->Branch("fi2_bj_pz", &fi2_bj_pz,  "fi2_bj_pz/F");
  TBranch *branc2_bj_e  = newtree->Branch("fi2_bj_e",  &fi2_bj_e,   "fi2_bj_e/F");
  
  TBranch *branc2_mlvjj = newtree->Branch("fi2_mlvjj", &fi2_mlvjj,  "fi2_mlvjj/F");
  TBranch *branc2_mlv   = newtree->Branch("fi2_mlv",   &fi2_mlv,    "fi2_mlv/F");
  TBranch *branc2_mjj   = newtree->Branch("fi2_mjj",   &fi2_mjj,    "fi2_mjj/F");
  TBranch *branc2_chi2  = newtree->Branch("fi2_chi2",  &fi2_chi2,   "fi2_chi2/F");
  TBranch *branc2_NDF   = newtree->Branch("fi2_NDF",   &fi2_NDF,    "fi2_NDF/I");
  TBranch *branc2_status= newtree->Branch("fi2_status",&fi2_status, "fi2_status/I");

  Float_t fi3_mu_px=0,   fi3_mu_py =0,   fi3_mu_pz=0,   fi3_mu_e=0;
  Float_t fi3_nv_px=0,   fi3_nv_py =0,   fi3_nv_pz=0,   fi3_nv_e=0;
  Float_t fi3_aj_px=0,   fi3_aj_py =0,   fi3_aj_pz=0,   fi3_aj_e=0;
  Float_t fi3_bj_px=0,   fi3_bj_py =0,   fi3_bj_pz=0,   fi3_bj_e=0;
  Float_t fi3_mlvjj=0,   fi3_chi2  =999;  
  Int_t   fi3_NDF  =999, fi3_status=999;
  Float_t fi3_mlv  =0,   fi3_mjj   =0;

  TBranch *branc3_mu_px = newtree->Branch("fi3_mu_px", &fi3_mu_px,  "fi3_mu_px/F");
  TBranch *branc3_mu_py = newtree->Branch("fi3_mu_py", &fi3_mu_py,  "fi3_mu_py/F");
  TBranch *branc3_mu_pz = newtree->Branch("fi3_mu_pz", &fi3_mu_pz,  "fi3_mu_pz/F");
  TBranch *branc3_mu_e  = newtree->Branch("fi3_mu_e",  &fi3_mu_e,   "fi3_mu_e/F");
  	   
  TBranch *branc3_nv_px = newtree->Branch("fi3_nv_px", &fi3_nv_px,  "fi3_nv_px/F");
  TBranch *branc3_nv_py = newtree->Branch("fi3_nv_py", &fi3_nv_py,  "fi3_nv_py/F");
  TBranch *branc3_nv_pz = newtree->Branch("fi3_nv_pz", &fi3_nv_pz,  "fi3_nv_pz/F");
  TBranch *branc3_nv_e  = newtree->Branch("fi3_nv_e",  &fi3_nv_e,   "fi3_nv_e/F");
  	   
  TBranch *branc3_aj_px = newtree->Branch("fi3_aj_px", &fi3_aj_px,  "fi3_aj_px/F");
  TBranch *branc3_aj_py = newtree->Branch("fi3_aj_py", &fi3_aj_py,  "fi3_aj_py/F");
  TBranch *branc3_aj_pz = newtree->Branch("fi3_aj_pz", &fi3_aj_pz,  "fi3_aj_pz/F");
  TBranch *branc3_aj_e  = newtree->Branch("fi3_aj_e",  &fi3_aj_e,   "fi3_aj_e/F");
  	   
  TBranch *branc3_bj_px = newtree->Branch("fi3_bj_px", &fi3_bj_px,  "fi3_bj_px/F");
  TBranch *branc3_bj_py = newtree->Branch("fi3_bj_py", &fi3_bj_py,  "fi3_bj_py/F");
  TBranch *branc3_bj_pz = newtree->Branch("fi3_bj_pz", &fi3_bj_pz,  "fi3_bj_pz/F");
  TBranch *branc3_bj_e  = newtree->Branch("fi3_bj_e",  &fi3_bj_e,   "fi3_bj_e/F");
  
  TBranch *branc3_mlvjj = newtree->Branch("fi3_mlvjj", &fi3_mlvjj,  "fi3_mlvjj/F");
  TBranch *branc3_mlv   = newtree->Branch("fi3_mlv",   &fi3_mlv,    "fi3_mlv/F");
  TBranch *branc3_mjj   = newtree->Branch("fi3_mjj",   &fi3_mjj,    "fi3_mjj/F");
  TBranch *branc3_chi2  = newtree->Branch("fi3_chi2",  &fi3_chi2,   "fi3_chi2/F");
  TBranch *branc3_NDF   = newtree->Branch("fi3_NDF",   &fi3_NDF,    "fi3_NDF/I");
  TBranch *branc3_status= newtree->Branch("fi3_status",&fi3_status, "fi3_status/I");

  Float_t TopWm=0;
  TBranch *branc3_TopWm = newtree->Branch("TopWm",     &TopWm,      "TopWm/F");

  // Parameter Setup
  const unsigned int jetsize         = 6;
  const double Jpt                   = 30;    // Jet pt threshold
  const double btssv                 = 1.74;  // BTagging
  const double VBF_MaxEta            = 4.5;   // VBF jet max eta
  // Loop over all events
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //Long64_t ientry = LoadTree(jentry);
    //if (ientry < 0) break;
    nb = newtree->GetEntry(jentry);   nbytes += nb;
    // Cut variable definitions
    double jess    = 1.00; // control the jet energy scale
    double muoniso = (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt;
    double dijetpt = sqrt(JetPFCor_Pt[0]*JetPFCor_Pt[0]+
			  JetPFCor_Pt[1]*JetPFCor_Pt[1]+
			  2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]));

    // Save variable initialization
    gdevtt    = 0;
    evtNJ     = 0;

    fit_mu_px = 0; fit_mu_py = 0; fit_mu_pz = 0;  fit_mu_e  = 0; 
    fit_nv_px = 0; fit_nv_py = 0; fit_nv_pz = 0;  fit_nv_e  = 0; 
    fit_aj_px = 0; fit_aj_py = 0; fit_aj_pz = 0;  fit_aj_e  = 0; 
    fit_bj_px = 0; fit_bj_py = 0; fit_bj_pz = 0;  fit_bj_e  = 0; 
    fit_mlvjj = 0; fit_chi2  =999;fit_NDF   =999; fit_status=999;
    fit_mlv   = 0; fit_mjj   = 0;

    fi2_mu_px = 0; fi2_mu_py = 0; fi2_mu_pz = 0;  fi2_mu_e  = 0; 
    fi2_nv_px = 0; fi2_nv_py = 0; fi2_nv_pz = 0;  fi2_nv_e  = 0; 
    fi2_aj_px = 0; fi2_aj_py = 0; fi2_aj_pz = 0;  fi2_aj_e  = 0; 
    fi2_bj_px = 0; fi2_bj_py = 0; fi2_bj_pz = 0;  fi2_bj_e  = 0; 
    fi2_mlvjj = 0; fi2_chi2  =999;fi2_NDF   =999; fi2_status=999;
    fi2_mlv   = 0; fi2_mjj   = 0;

    fi3_mu_px = 0; fi3_mu_py = 0; fi3_mu_pz = 0;  fi3_mu_e  = 0; 
    fi3_nv_px = 0; fi3_nv_py = 0; fi3_nv_pz = 0;  fi3_nv_e  = 0; 
    fi3_aj_px = 0; fi3_aj_py = 0; fi3_aj_pz = 0;  fi3_aj_e  = 0; 
    fi3_bj_px = 0; fi3_bj_py = 0; fi3_bj_pz = 0;  fi3_bj_e  = 0; 
    fi3_mlvjj = 0; fi3_chi2  =999;fi3_NDF   =999; fi3_status=999;
    fi3_mlv   = 0; fi3_mjj   = 0;

    TopWm     = 0;

    // Good Event Selection Requirement
    bool  isgengdevt = 0;
    if (JetPFCor_Pt[0]>Jpt 
	&& JetPFCor_Pt[1]>Jpt 
        //&& fabs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5
        //&& fabs(JetPFCor_dphiMET[0])>0.4
        && event_met_pfmet>30.
	&& W_mt>40.
	//&& dijetpt>40.
	&& W_muon_pt>25.
	&& muoniso<0.1
	&& fabs(W_muon_d0bsp)<0.02
	&& fabs(W_muon_eta)<2.1
        ) isgengdevt = 1;

    // 2 and 3 jet event for Mjj and Hww 
    if (isgengdevt
	&& fabs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5
        && fabs(JetPFCor_dphiMET[0])>0.4
	&& dijetpt>40.)
      {
	if ( JetPFCor_Pt[1] > Jpt && JetPFCor_Pt[2] < Jpt ) {evtNJ = 2; gdevtt = 1;}
	if ( JetPFCor_Pt[2] > Jpt && JetPFCor_Pt[3] < Jpt ) {evtNJ = 3;}
	if (evtNJ==2||evtNJ==3) { // Do the kinematic fit for all event!!!
	  int Aj = 0, Bj = 1;    TLorentzVector  mup, nvp, ajp, bjp; 
	  mup.SetPtEtaPhiE(W_muon_pt,              W_muon_eta,       W_muon_phi,       W_muon_e               );
	  nvp.SetPxPyPzE(event_met_pfmet * cos(event_met_pfmetPhi), event_met_pfmet * sin(event_met_pfmetPhi), 
			 W_pzNu1, sqrt(event_met_pfmet*event_met_pfmet + W_pzNu1*W_pzNu1)                     );
	  ajp.SetPtEtaPhiE(jess * JetPFCor_Pt[Aj], JetPFCor_Eta[Aj], JetPFCor_Phi[Aj], jess * JetPFCor_E[Aj]  );
	  bjp.SetPtEtaPhiE(jess * JetPFCor_Pt[Bj], JetPFCor_Eta[Bj], JetPFCor_Phi[Bj], jess * JetPFCor_E[Bj]  );
	  
	  TLorentzVector fit_mup(0,0,0,0), fit_nvp(0,0,0,0), fit_ajp(0,0,0,0), fit_bjp(0,0,0,0) ; 
	  doKinematicFit( 1, mup, nvp, ajp, bjp,  fit_mup, fit_nvp, fit_ajp, fit_bjp, fit_chi2, fit_NDF, fit_status);
	  fit_mu_px = fit_mup.Px(); fit_mu_py = fit_mup.Py(); fit_mu_pz = fit_mup.Pz(); fit_mu_e = fit_mup.E(); 
	  fit_nv_px = fit_nvp.Px(); fit_nv_py = fit_nvp.Py(); fit_nv_pz = fit_nvp.Pz(); fit_nv_e = fit_nvp.E(); 
	  fit_aj_px = fit_ajp.Px(); fit_aj_py = fit_ajp.Py(); fit_aj_pz = fit_ajp.Pz(); fit_aj_e = fit_ajp.E(); 
	  fit_bj_px = fit_bjp.Px(); fit_bj_py = fit_bjp.Py(); fit_bj_pz = fit_bjp.Pz(); fit_bj_e = fit_bjp.E(); 
	  fit_mlvjj = (fit_mup+fit_nvp+fit_ajp+fit_bjp).M();
	  fit_mlv   = (fit_mup+fit_nvp).M();
	  fit_mjj   = (fit_ajp+fit_bjp).M(); 
	  
	  TLorentzVector fi2_mup(0,0,0,0), fi2_nvp(0,0,0,0), fi2_ajp(0,0,0,0), fi2_bjp(0,0,0,0) ; 
	  doKinematicFit( 2, mup, nvp, ajp, bjp,  fi2_mup, fi2_nvp, fi2_ajp, fi2_bjp, fi2_chi2, fi2_NDF, fi2_status);
	  fi2_mu_px = fi2_mup.Px(); fi2_mu_py = fi2_mup.Py(); fi2_mu_pz = fi2_mup.Pz(); fi2_mu_e = fi2_mup.E(); 
	  fi2_nv_px = fi2_nvp.Px(); fi2_nv_py = fi2_nvp.Py(); fi2_nv_pz = fi2_nvp.Pz(); fi2_nv_e = fi2_nvp.E(); 
	  fi2_aj_px = fi2_ajp.Px(); fi2_aj_py = fi2_ajp.Py(); fi2_aj_pz = fi2_ajp.Pz(); fi2_aj_e = fi2_ajp.E(); 
	  fi2_bj_px = fi2_bjp.Px(); fi2_bj_py = fi2_bjp.Py(); fi2_bj_pz = fi2_bjp.Pz(); fi2_bj_e = fi2_bjp.E(); 
	  fi2_mlvjj = (fi2_mup+fi2_nvp+fi2_ajp+fi2_bjp).M();
	  fi2_mlv   = (fi2_mup+fi2_nvp).M();
	  fi2_mjj   = (fi2_ajp+fi2_bjp).M(); 

	  TLorentzVector fi3_mup(0,0,0,0), fi3_nvp(0,0,0,0), fi3_ajp(0,0,0,0), fi3_bjp(0,0,0,0) ; 
	  doKinematicFit( 3, mup, nvp, ajp, bjp,  fi3_mup, fi3_nvp, fi3_ajp, fi3_bjp, fi3_chi2, fi3_NDF, fi3_status);

	  // For test the code using same method as Dan.
	  TLorentzVector tamet = nvp + ajp + bjp - fi3_ajp - fi3_bjp;
	  TLorentzVector tbmet; tbmet.SetPxPyPzE(tamet.Px(), tamet.Py(), 0, sqrt(tamet.Px()*tamet.Px() + tamet.Py()*tamet.Py()) );
	  METzCalculator metz;
	  metz.SetMET(tbmet);
	  metz.SetLepton(mup);
	  metz.SetLeptonType("muon");
	  double newnvpz = metz.Calculate();
	  TLorentzVector nownvp; nownvp.SetPxPyPzE(tamet.Px(), tamet.Py(), newnvpz, sqrt(tamet.Px()*tamet.Px() + tamet.Py()*tamet.Py() + newnvpz*newnvpz) );
	  fi3_nvp = nownvp;
	  fi3_mup = mup;
	  // Re calculate the neutrino

	  fi3_mu_px = fi3_mup.Px(); fi3_mu_py = fi3_mup.Py(); fi3_mu_pz = fi3_mup.Pz(); fi3_mu_e = fi3_mup.E(); 
	  fi3_nv_px = fi3_nvp.Px(); fi3_nv_py = fi3_nvp.Py(); fi3_nv_pz = fi3_nvp.Pz(); fi3_nv_e = fi3_nvp.E(); 
	  fi3_aj_px = fi3_ajp.Px(); fi3_aj_py = fi3_ajp.Py(); fi3_aj_pz = fi3_ajp.Pz(); fi3_aj_e = fi3_ajp.E(); 
	  fi3_bj_px = fi3_bjp.Px(); fi3_bj_py = fi3_bjp.Py(); fi3_bj_pz = fi3_bjp.Pz(); fi3_bj_e = fi3_bjp.E(); 
	  fi3_mlvjj = (fi3_mup+fi3_nvp+fi3_ajp+fi3_bjp).M();
	  fi3_mlv   = (fi3_mup+fi3_nvp).M();
	  fi3_mjj   = (fi3_ajp+fi3_bjp).M(); 
	}
      }	 // Only Good event will be performed kinematic fit
    // For Hadronic W in Top sample
    if (isgengdevt)
      {
	if (JetPFCor_Pt[3] > Jpt && JetPFCor_Pt[4] < Jpt){
	  int nbjet = 0;
	  int nbnot = 0;
	  int Aj    = -999;
	  int Bj    = -999;
	  if (JetPFCor_bDiscriminator[0]>btssv) { nbjet++; } else { nbnot++; if (nbnot==1) Aj=0; if (nbnot==2) Bj=0;}
	  if (JetPFCor_bDiscriminator[1]>btssv) { nbjet++; } else { nbnot++; if (nbnot==1) Aj=1; if (nbnot==2) Bj=1;}
	  if (JetPFCor_bDiscriminator[2]>btssv) { nbjet++; } else { nbnot++; if (nbnot==1) Aj=2; if (nbnot==2) Bj=2;}
	  if (JetPFCor_bDiscriminator[3]>btssv) { nbjet++; } else { nbnot++; if (nbnot==1) Aj=3; if (nbnot==2) Bj=3;}
	  
	  if (nbjet==2 && nbnot==2 && Aj!=-999 && Bj!=-999){
	    TLorentzVector  ajp, bjp; 
	    ajp.SetPtEtaPhiE(jess * JetPFCor_Pt[Aj], JetPFCor_Eta[Aj], JetPFCor_Phi[Aj], jess * JetPFCor_E[Aj]  );
	    bjp.SetPtEtaPhiE(jess * JetPFCor_Pt[Bj], JetPFCor_Eta[Bj], JetPFCor_Phi[Bj], jess * JetPFCor_E[Bj]  );
	    TopWm   = (ajp+bjp).M(); 
	  }
	}
      }
    // For VBF Analysis ! Currently Gd Event Selection same as Hww
    if (isgengdevt
	&& fabs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5
        && fabs(JetPFCor_dphiMET[0])>0.4
	&& dijetpt>40.)
      {
	int * gdcjet  = new int[jetsize];
	int * gdfjet  = new int[jetsize];
	int   ngdcjet = 0, ngdfjet = 0;
	for ( size_t ijet=0; ijet < jetsize; ++ijet) {
	  gdcjet[ijet] = 0;
	  gdfjet[ijet] = 0;
	  // Identify B Jet
	  if (JetPFCor_Pt[ijet]>Jpt                        &&
	      JetPFCor_bDiscriminator[ijet]<btssv           ) {gdcjet[ijet] = 1; ngdcjet++;}
	  if (JetPFCorVBFTag_Pt[ijet]>Jpt                  &&
	      JetPFCorVBFTag_bDiscriminator[ijet]<btssv    && 
	      fabs(JetPFCorVBFTag_Eta[ijet])<VBF_MaxEta     ) {gdfjet[ijet] = 1; ngdfjet++;}
	}
	if (ngdcjet>1 && (ngdcjet+ngdfjet)>3) { // Good VBF event has N total jet >3 and N central jet >1
	  // ----- Output txt file for Dan -15 Lepton and -5 MET
	  fprintf(textfile, "%12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f \n",
		  -15.0, W_muon_pt,     W_muon_eta,     W_muon_phi,     0.0, 0.0, 0.0);
	  fprintf(textfile, "%12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f \n",
		  -5.0,  event_met_pfmet, event_met_pfmetPhi, event_met_pfmetsignificance, event_met_pfsumet, (double)event_nPV, 0.0); 
	  // ----- Output txt file for Dan Jet with pT > jetthreshold
	  for ( size_t ijet=0; ijet < jetsize; ++ijet) {
	    if(gdcjet[ijet]==1) 
	      fprintf(textfile, "%12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f \n",
		      JetPFCor_E[ijet], JetPFCor_Pt[ijet], JetPFCor_Eta[ijet], JetPFCor_Phi[ijet], JetPFCor_bDiscriminator[ijet], 0.0, 0.0 );
	    if(gdfjet[ijet]==1) 
	      fprintf(textfile, "%12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f  %12.5f \n",
		      JetPFCorVBFTag_E[ijet], JetPFCorVBFTag_Pt[ijet], JetPFCorVBFTag_Eta[ijet], JetPFCorVBFTag_Phi[ijet], JetPFCorVBFTag_bDiscriminator[ijet], 0.0, 0.0 );
 	  }
	}
	// For VBF Analysis ! Currently Gd Event Selection same as Hww
      }
    branch_gdevtt->Fill();
    branch_evtNJ ->Fill();
    
    branch_mu_px->Fill();
    branch_mu_py->Fill();
    branch_mu_pz->Fill();
    branch_mu_e ->Fill();
    
    branch_nv_px->Fill();
    branch_nv_py->Fill();
    branch_nv_pz->Fill();
    branch_nv_e ->Fill();
    
    branch_aj_px->Fill();
    branch_aj_py->Fill();
    branch_aj_pz->Fill();
    branch_aj_e ->Fill();
    
    branch_bj_px->Fill();
    branch_bj_py->Fill();
    branch_bj_pz->Fill();
    branch_bj_e ->Fill();
   
    branch_mlvjj->Fill();
    branch_mlv  ->Fill();
    branch_mjj  ->Fill();
    branch_chi2 ->Fill();
    branch_NDF  ->Fill();
    branch_status->Fill();

    branc2_mu_px->Fill();
    branc2_mu_py->Fill();
    branc2_mu_pz->Fill();
    branc2_mu_e ->Fill();
    
    branc2_nv_px->Fill();
    branc2_nv_py->Fill();
    branc2_nv_pz->Fill();
    branc2_nv_e ->Fill();
    
    branc2_aj_px->Fill();
    branc2_aj_py->Fill();
    branc2_aj_pz->Fill();
    branc2_aj_e ->Fill();
    
    branc2_bj_px->Fill();
    branc2_bj_py->Fill();
    branc2_bj_pz->Fill();
    branc2_bj_e ->Fill();
   
    branc2_mlvjj->Fill();
    branc2_mlv  ->Fill();
    branc2_mjj  ->Fill();
    branc2_chi2 ->Fill();
    branc2_NDF  ->Fill();
    branc2_status->Fill();

    branc3_mu_px->Fill();
    branc3_mu_py->Fill();
    branc3_mu_pz->Fill();
    branc3_mu_e ->Fill();
    
    branc3_nv_px->Fill();
    branc3_nv_py->Fill();
    branc3_nv_pz->Fill();
    branc3_nv_e ->Fill();
    
    branc3_aj_px->Fill();
    branc3_aj_py->Fill();
    branc3_aj_pz->Fill();
    branc3_aj_e ->Fill();
    
    branc3_bj_px->Fill();
    branc3_bj_py->Fill();
    branc3_bj_pz->Fill();
    branc3_bj_e ->Fill();
   
    branc3_mlvjj->Fill();
    branc3_mlv  ->Fill();
    branc3_mjj  ->Fill();
    branc3_chi2 ->Fill();
    branc3_NDF  ->Fill();
    branc3_status->Fill();

    branc3_TopWm->Fill();
  } // end event loop
  fresults.cd();
  newtree->Write("WJet");
  fresults.Close();
  fclose(textfile);
  std::cout <<  wda << " :: " << outfilename << "    "<< nentries  << std::endl;
}

bool kanamuon::doKinematicFit(Int_t                 fflage,
			      const TLorentzVector     mup, 
			      const TLorentzVector     nvp, 
			      const TLorentzVector     ajp, 
			      const TLorentzVector     bjp, 
			      TLorentzVector     & fit_mup, 
			      TLorentzVector     & fit_nvp,
			      TLorentzVector     & fit_ajp, 
			      TLorentzVector     & fit_bjp, 
			      Float_t            & fit_chi2,
			      Int_t              & fit_NDF, 
			      Int_t              & fit_status)
{

  bool OK                     = false;
  Resolution* resolution      = new Resolution();

  TMatrixD m1(3,3);
  TMatrixD m2(3,3);
  TMatrixD m3(3,3);
  TMatrixD m4(3,3);
  m1.Zero();
  m2.Zero();
  m3.Zero();
  m4.Zero();

  double etRes, etaRes, phiRes;
  // lepton resolution
  const std::string& leptonName = "muon";  const TLorentzVector lepton   = mup;
  if(leptonName == "electron") {
    OK = resolution->electronResolution(lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  } else {
    OK = resolution->muonResolution(    lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  }
  m1(0,0) = resolution->square(etRes);
  m1(1,1) = resolution->square(etaRes);
  m1(2,2) = resolution->square(phiRes);
  // MET resolution
  OK = resolution->PFMETResolution(     nvp.Et(),            etRes, etaRes, phiRes);
  if(!OK) return OK;
  m2(0,0) = resolution->square(etRes);
  m2(1,1) = 0.01; // resolution->square(etaRes)
  m2(2,2) = resolution->square(phiRes);
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( ajp.Et(), ajp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  m3(0,0) = resolution->square(etRes);
  m3(1,1) = resolution->square(etaRes);
  m3(2,2) = resolution->square(phiRes);
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( bjp.Et(), bjp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  m4(0,0) = resolution->square(etRes);
  m4(1,1) = resolution->square(etaRes);
  m4(2,2) = resolution->square(phiRes);

  TLorentzVector tmp_mup = mup;
  TLorentzVector tmp_nvp = nvp;
  TLorentzVector tmp_ajp = ajp;
  TLorentzVector tmp_bjp = bjp;

  // Fit Particle
  TFitParticleEtEtaPhi* particle1 = new TFitParticleEtEtaPhi( "Lepton",   "Lepton",   &tmp_mup,    &m1 );
  TFitParticleEtEtaPhi* particle2 = new TFitParticleEtEtaPhi( "Neutrino", "Neutrino", &tmp_nvp,    &m2 );
  TFitParticleEtEtaPhi* particle3 = new TFitParticleEtEtaPhi( "Jeta",     "Jeta",     &tmp_ajp,    &m3 );
  TFitParticleEtEtaPhi* particle4 = new TFitParticleEtEtaPhi( "Jetb",     "Jetb",     &tmp_bjp,    &m4 );

  // Constraint
  TFitConstraintM *mCons1 = new TFitConstraintM( "WMassConstrainta", "WMass-Constrainta", 0, 0 , 80.4);
  mCons1->addParticles1( particle1, particle2 );

  TFitConstraintM *mCons2 = new TFitConstraintM( "WMassConstraintb", "WMass-Constraintb", 0, 0 , 80.4);
  mCons2->addParticles1( particle3, particle4 );

  TFitConstraintEp *pxCons = new TFitConstraintEp( "PxConstraint", "Px-Constraint", 0, TFitConstraintEp::pX , (mup+nvp+ajp+bjp).Px() );
  pxCons->addParticles( particle1, particle2, particle3, particle4 );

  TFitConstraintEp *pyCons = new TFitConstraintEp( "PyConstraint", "Py-Constraint", 0, TFitConstraintEp::pY , (mup+nvp+ajp+bjp).Py() );
  pyCons->addParticles( particle1, particle2, particle3, particle4 );

  //Definition of the fitter
  TKinFitter* fitter = new TKinFitter("fitter", "fitter");
  if        (fflage == 1 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addMeasParticle( particle3 );
    fitter->addMeasParticle( particle4 );
    fitter->addConstraint( pxCons );
    fitter->addConstraint( pyCons );
    fitter->addConstraint( mCons1 );
    fitter->addConstraint( mCons2 );
  }else   if(fflage == 2 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addConstraint( mCons1 );
  }else   if(fflage == 3 ){
    fitter->addMeasParticle( particle3 );
    fitter->addMeasParticle( particle4 );
    fitter->addConstraint( mCons2 );
  }else {return false;}

  //Set convergence criteria
  fitter->setMaxNbIter( 500 );
  fitter->setMaxDeltaS( 1e-2 );
  fitter->setMaxF( 1e-2 );
  fitter->setVerbosity(1);
  fitter->fit();

  //Return the kinematic fit results
  fit_status   = fitter->getStatus();
  fit_chi2     = fitter->getS();
  fit_NDF      = fitter->getNDF();
  fit_mup      = *(particle1->getCurr4Vec()); 
  fit_nvp      = *(particle2->getCurr4Vec()); 
  fit_ajp      = *(particle3->getCurr4Vec()); 
  fit_bjp      = *(particle4->getCurr4Vec()); 
  
  if(fitter->getStatus() == 0) { OK = true;  } else { OK = false;  }
  delete resolution;
  delete particle1;
  delete particle2;
  delete particle3;
  delete particle4;
  delete mCons1;
  delete mCons2;
  delete pxCons;
  delete pyCons;
  delete fitter;

  return OK;
}
