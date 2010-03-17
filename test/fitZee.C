#include "ZeeMassFitter.C"

void fitZee()
{
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.1);
  tdrStyle->SetPadTopMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);


  const int NGenPtBins=21;
  const double GenPt[NGenPtBins+1] = {0,15,20,30,50,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,2600,3000,3500,-1};
  TString ptBin[NGenPtBins];
  TString basename = "/uscms_data/d1/kalanand/trash/CSA07-ZeeJets_Pt_";
  TFile* f[NGenPtBins];
  const double lumi = 100.0;



  double crosssection[NGenPtBins];
  crosssection[0]  =  104400.0 * 0.98;
  crosssection[1]  =  353.1          ;
  crosssection[2]  =  326.7          ;
  crosssection[3]  =  227.0          ;
  crosssection[4]  =  93.17          ;
  crosssection[5]  =  31.48          ;
  crosssection[6]  =  9.63           ;
  crosssection[7]  =  2.92           ;
  crosssection[8]  =  0.8852         ;
  crosssection[9]  =  0.2936         ;
  crosssection[10] =  0.1025         ;
  crosssection[11] =  0.04242        ;
  crosssection[12] =  0.01443        ;
  crosssection[13] =  0.002859       ;
  crosssection[14] =  0.00094        ;
  crosssection[15] =  0.00009536     ;
  crosssection[16] =  0.00001232     ;
  crosssection[17] =  0.000001839    ;
  crosssection[18] =  0.0000002881   ;
  crosssection[19] =  0.00000004764  ;
  crosssection[20] =  0.000000004515 ;


  for(int i=0; i<NGenPtBins; i++) {
    ptBin[i] = Form("%d_%d", (int) GenPt[i], (int) GenPt[i+1] );
    f[i] = new TFile( basename+ptBin[i]+TString(".root") );
  }



  TH1D Zmass("Zmass", "", 40, 60, 120);
 



  for(int i=0; i<NGenPtBins; i++) {
    TTree* t = (TTree*) f[i]->Get("ZJet");
    int nEvents  = (int) t->GetEntries();
    double coeff = lumi * crosssection[i]  /  nEvents;
    TString wt = Form("%f", coeff);  
    //  if(i==1) wt = Form("%f*(JetGenPt[0][0]<500.0)", coeff); 


    TString cutZCut =  wt+"*(ePlusPt>20.0 && eMinusPt>20.0 && ((fabs(ePlusEta)<1.4442) || (fabs(ePlusEta)>1.560 && fabs(ePlusEta)<2.5)) && ((fabs(eMinusEta)<1.4442) || (fabs(eMinusEta)>1.560 && fabs(eMinusEta)<2.5)))";


//     TString cutZCut =  "(ePlusPt>20.0 && eMinusPt>20.0 && ((fabs(ePlusEta)<1.4442) || (fabs(ePlusEta)>1.560 && fabs(ePlusEta)<2.5)) && ((fabs(eMinusEta)<1.4442) || (fabs(eMinusEta)>1.560 && fabs(eMinusEta)<2.5)))";


    t->Draw("mZee>>+Zmass", cutZCut, "goff");

  }





  ///////////////////////////////////////
  /////////////////////////////////////



  // TCanvas can("can", "", 600, 600);
  // gStyle->SetOptStat(0);
  // Zmass->Draw("e");

  // TString plotname = "NormRes_Zpt_20GeVcut";
  // can7.SaveAs( plotname+TString(".eps") );
  // can7.SaveAs( plotname+TString(".gif") );
  // can7.SaveAs( plotname+TString(".root") );


  ZeeMassFitter(Zmass);
}
