#include <iostream>
#include <vector>
#include "TLatex.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "THStack.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TCanvas.h"
//=====================================================================================
// SYNOPSIS:
//   1. Prepare "InData" and "OutDir" directories; e.g., "ln -s . OutDir" to go to current dir
//   2. root [0] .x mkControlPlots.C(0) for electron data, or
//      root [0] .x mkControlPlots.C(1) for muon data
// This macro makes control plots for the MVA selection efficiencies. Check the input to get the chosen final state and selection!
// ====================================================================================
// Self Function
// ====================================================================================
/*

*/
void cmspre(double intlumifbinv)
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.85,0.93,"#sqrt{s} = 8 TeV");
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.65,0.93,Form("#int #font[12]{L} dt = %.1f fb^{-1}", (float)intlumifbinv));
//  latex.DrawLatex(0.65,0.93,Form("2012A", (float)intlumifbinv));
//  latex.DrawLatex(0.65,0.93,Form("Runs 193701-194400", (float)intlumifbinv));
//  latex.DrawLatex(0.65,0.93,Form("Runs 194401-194800", (float)intlumifbinv));
//  latex.DrawLatex(0.65,0.93,Form("Runs 194801-195552", (float)intlumifbinv));

  latex.SetTextAlign(11); // align left
//  latex.DrawLatex(0.15,0.93,"CMS,  #sqrt{s} = 7 TeV");//preliminary 2011");
  latex.DrawLatex(0.15,0.96,"CMS preliminary");

}
#if 0
  PlotVar_t(char *inpv,double inmaxr,double inminr,int innbin,int inslog,char *inxl,char *inoutf,char *inoutf2,double inamax,double inamin,int inanb,int inhp,int indl) :
    plotvar(inpv),
    MAXRange(inmaxr),MINRange(inminr),NBINS(innbin),
    slog(inslog),xlabel(inxl),outfile(inoutf),outfile2(inoutf2),
    AMAXRange(inamax),AMINRange(inamin),ANBINS(inanb),
    hplot(inhp,drawleg(indl) {}
#endif
struct plotVar_t {
  char* plotvar;
  double MINRange;
  double MAXRange;
  int    NBINS;
  int    slog;
  char* xlabel;
  char* outfile;
  double AMINRange;
  double AMAXRange;
  int    ANBINS;
  int    mva_in;
  int    hplot;
  int  drawleg;
};

void mkMVACutFlowControlPlotsErr(bool domu=true,bool do3j=false, bool doblind=false, bool dosignal=false, bool dotop=false)
{
  gROOT->ProcessLine(".L tdrstyle.C");
  double intLUMI = 11900.;
  if ( domu ) intLUMI = 11500.;
  float minValue = 1000;

  std::vector<float> scale;
  scale.push_back( 36257.2* intLUMI/18353019); // V
  scale.push_back( 54.838   * intLUMI/9450414); // V
  scale.push_back( 32.3161   * intLUMI/10000267); // V
  scale.push_back( 8.059   * intLUMI/9702850); // V
  scale.push_back( 3503.71  * intLUMI/30209426); // V
  scale.push_back( 225.197 * intLUMI/6893735); // V
  scale.push_back( 3.89394  * intLUMI/259960); // top
  scale.push_back( 55.531  * intLUMI/3758221); //V
  scale.push_back( 11.1773  * intLUMI/497657); // V
  scale.push_back( 1.75776  * intLUMI/139974); // V anti-top
  scale.push_back( 30.0042  * intLUMI/1935066); // V
  scale.push_back( 11.1773  * intLUMI/493458); // V

  std::vector<int> colors;
  colors.push_back( kRed ); // V
  colors.push_back( kAzure+8 ); // V
  colors.push_back( kAzure+8 ); // V
  colors.push_back( kAzure+8 ); // V
  colors.push_back( kYellow ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V
  colors.push_back( kGreen+2 ); // V

  //stuff useful for QCD normalization
  float QCD_scale_init =  1. ; // V
  std::string QCDCut = "effwt*puwt*((ggdevt==2) && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>0.8)";
  if ( domu ) QCDCut = "effwt*puwt*((ggdevt==2) && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>0.8)";
  const double QCD_frac_ele = 0.14; //fit with fixed background a part from Wjets
  const double QCD_frac_mu  = 0.00001;

  //list of working point to be checked
  std::vector<std::string> mvaCuts;
  //el 2j
  if ( !domu && !do3j ) {
    mvaCuts.push_back("mva2j170el> 0.300");
    mvaCuts.push_back("mva2j180el> 0.550");
    mvaCuts.push_back("mva2j190el> 0.550");
    mvaCuts.push_back("mva2j200el> 0.600");
    mvaCuts.push_back("mva2j250el> 0.650");
    mvaCuts.push_back("(mva2j300el> 0.600)");
    mvaCuts.push_back("mva2j350el> 0.600");
    mvaCuts.push_back("(mva2j400el> 0.550)");
    mvaCuts.push_back("mva2j450el> 0.500");
    mvaCuts.push_back("mva2j500interferencenominalel> 0.500");
    mvaCuts.push_back("mva2j550interferencenominalel> 0.550");
    mvaCuts.push_back("mva2j600interferencenominalel> 0.600");
  }

  //el 3j
  else if ( !domu && do3j ) {
    mvaCuts.push_back("mva3j170el> 0.300");
    mvaCuts.push_back("mva3j180el> 0.300");
    mvaCuts.push_back("mva3j190el> 0.300");
    mvaCuts.push_back("mva3j200el> 0.400");
    mvaCuts.push_back("mva3j250el> 0.400");
    mvaCuts.push_back("(mva3j300el> 0.500)");
    mvaCuts.push_back("mva3j350el> 0.650");
    mvaCuts.push_back("(mva3j400el> 0.600)");
    mvaCuts.push_back("mva3j450el> 0.550");
    mvaCuts.push_back("mva3j500el> 0.600");
    mvaCuts.push_back("mva3j550el> 0.500");
    mvaCuts.push_back("mva3j600el> 0.600");
  }

  //mu 2j
  else if ( domu && !do3j ) {
    mvaCuts.push_back("mva2j170mu> 0.300");
    mvaCuts.push_back("mva2j180mu> 0.600");
    mvaCuts.push_back("mva2j190mu> 0.600");
    mvaCuts.push_back("mva2j200mu> 0.600");
    mvaCuts.push_back("mva2j250mu> 0.650");
    mvaCuts.push_back("(mva2j300mu> 0.600)");
    mvaCuts.push_back("mva2j350mu> 0.600");
    mvaCuts.push_back("(mva2j400mu> 0.550)");
    mvaCuts.push_back("mva2j450mu> 0.600");
    mvaCuts.push_back("mva2j500interferencenominalmu> 0.500");
    mvaCuts.push_back("mva2j550interferencenominalmu> 0.550");
    mvaCuts.push_back("mva2j600interferencenominalmu> 0.650");
  }

  //mu 2j
  else if ( domu && do3j ) {
    mvaCuts.push_back("mva3j170mu> 0.300");
    mvaCuts.push_back("mva3j180mu> 0.300");
    mvaCuts.push_back("mva3j190mu> 0.300");
    mvaCuts.push_back("mva3j200mu> 0.300");
    mvaCuts.push_back("mva3j250mu> 0.400");
    mvaCuts.push_back("(mva3j300mu> 0.600)");
    mvaCuts.push_back("mva3j350mu> 0.650");
    mvaCuts.push_back("(mva3j400mu> 0.500)");
    mvaCuts.push_back("mva3j450mu> 0.550");
    mvaCuts.push_back("mva3j500mu> 0.650");
    mvaCuts.push_back("mva3j550mu> 0.500");
    mvaCuts.push_back("mva3j600mu> 0.600");
  }

  std::vector<std::string> cutNames;
  cutNames.push_back("m_{H} = 170");
  cutNames.push_back("m_{H} = 180");
  cutNames.push_back("m_{H} = 190");
  cutNames.push_back("m_{H} = 200");
  cutNames.push_back("m_{H} = 250");
  cutNames.push_back("m_{H} = 300");
  cutNames.push_back("m_{H} = 350");
  cutNames.push_back("m_{H} = 400");
  cutNames.push_back("m_{H} = 450");
  cutNames.push_back("m_{H} = 500");
  cutNames.push_back("m_{H} = 550");
  cutNames.push_back("m_{H} = 600");
  
  std::string cutName;
  if (domu && !do3j)         cutName = "Mu 2j Data";
  else if ( !domu && !do3j ) cutName = "Ele 2j Data,";
  else if ( domu && do3j )   cutName = "Mu 3j Data,";
  else if ( !domu && do3j )  cutName = "Ele 3j Data,";

  //flat cuts
  std::string flatCut;
  if (domu) flatCut = "((fit_status==0) && (W_mt > 30) && (abs(JetPFCor_dphiMET[0])>0.4) && (abs(W_muon_eta) < 2.1)";
  else      flatCut = "((fit_status==0) && (W_mt > 30) && (abs(JetPFCor_dphiMET[0])>0.8) && (W_electron_pt > 35)";
  if ( do3j && !dotop )      flatCut  += "&& (ggdevt == 3)";
  else if ( !do3j && !dotop ) flatCut += "&& (ggdevt == 2)";
  
  std::string flatCutWeight = ")*effwt*puwt";
  
  //mjj cuts
  std::string mjjCut = "(Mass2j_PFCor > 0)";
  if ( doblind )                  mjjCut = "(Mass2j_PFCor > 95 || Mass2j_PFCor < 65)"; 
  else if ( !doblind && dosignal )mjjCut = "(Mass2j_PFCor < 95 && Mass2j_PFCor > 65)";

  //top cuts
  std::string  topCut = "(TopWm >= 0)";
  if ( dotop ) topCut = "(TopWm > 0)"; 

  //file names
  TFile *fin2;
  TFile *fQCD;
  std::vector<TFile*> MC_files;

  if (domu) {
    fin2            = new TFile("InData/RD_WmunuJets_DataAll_GoldenJSON_11p5invfb.root", "read");
    fQCD            = new TFile("InData/RD_mu_WpJ_CMSSW532.root", "read");
    MC_files.push_back( new TFile("InData/RD_mu_WpJ_CMSSW532.root","READ") );
    MC_files.push_back( new TFile("InData/RD_mu_WW_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_WZ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_ZZ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_ZpJ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_TTbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopS_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopT_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopTW_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopS_Tbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopT_Tbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_mu_STopTW_Tbar_CMSSW532.root", "READ") );

  } else { // electrons

    fin2            = new TFile("InData/RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_11p9invfb.root", "READ");
    fQCD            = new TFile("InData/RDQCD_WenuJets_Isog0p3NoElMVA_11p9invfb.root", "READ") ; //temporary: for muons take the Wjets as QCD proxy: WRONG!
    MC_files.push_back( new TFile("InData/RD_el_WpJ_CMSSW532.root","READ") );
    MC_files.push_back( new TFile("InData/RD_el_WW_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_WZ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_ZZ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_ZpJ_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_TTbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopS_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopT_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopTW_T_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopS_Tbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopT_Tbar_CMSSW532.root", "READ") );
    MC_files.push_back( new TFile("InData/RD_el_STopTW_Tbar_CMSSW532.root", "READ") );

  }

  // Prepare the trees
  TTree* t_data = (TTree*) fin2 -> Get("WJet");
  TTree* t_qcd  = (TTree*) fQCD -> Get("WJet");
  t_data -> SetBranchStatus("*", 0);
  t_data -> SetBranchStatus("fit_status", 1);
  t_data -> SetBranchStatus("ggdevt", 1);
  t_data -> SetBranchStatus("W_electron_pt", 1);
  t_data -> SetBranchStatus("W_muon_eta", 1);
  t_data -> SetBranchStatus("W_mt", 1);
  t_data -> SetBranchStatus("TopWm", 1);
  t_data -> SetBranchStatus("event_met_pfmet", 1);
  t_data -> SetBranchStatus("JetPFCor_dphiMET*", 1);
  t_data -> SetBranchStatus("numPFCorJets", 1);
  t_data -> SetBranchStatus("Mass2j_PFCor", 1);
  t_data -> SetBranchStatus("mva*", 1);
  t_data -> SetBranchStatus("effwt", 1);
  t_data -> SetBranchStatus("puwt", 1);

  t_qcd -> SetBranchStatus("*", 0);
  t_qcd -> SetBranchStatus("fit_status", 1);
  t_qcd -> SetBranchStatus("ggdevt", 1);
  t_qcd -> SetBranchStatus("W_electron_pt", 1);
  t_qcd -> SetBranchStatus("W_muon_eta", 1);
  t_qcd -> SetBranchStatus("W_mt", 1);
  t_qcd -> SetBranchStatus("TopWm", 1);
  t_qcd -> SetBranchStatus("event_met_pfmet", 1);
  t_qcd -> SetBranchStatus("JetPFCor_dphiMET*", 1);
  t_qcd -> SetBranchStatus("numPFCorJets", 1);
  t_qcd -> SetBranchStatus("Mass2j_PFCor", 1);
  t_qcd -> SetBranchStatus("mva*", 1);
  t_qcd -> SetBranchStatus("effwt", 1);
  t_qcd -> SetBranchStatus("puwt", 1);

  std::vector<TTree*> MC_trees;
  for ( int iFile = 0; iFile < MC_files.size(); iFile++ ) { 
    MC_trees.push_back((TTree*) MC_files[iFile] -> Get("WJet"));
    MC_trees[iFile] -> SetBranchStatus("*", 0);
    MC_trees[iFile] -> SetBranchStatus("fit_status", 1);
    MC_trees[iFile] -> SetBranchStatus("ggdevt", 1);
    MC_trees[iFile] -> SetBranchStatus("W_electron_pt", 1);
    MC_trees[iFile] -> SetBranchStatus("W_muon_eta", 1);
    MC_trees[iFile] -> SetBranchStatus("W_mt", 1);
    MC_trees[iFile] -> SetBranchStatus("TopWm", 1);
    MC_trees[iFile] -> SetBranchStatus("event_met_pfmet", 1);
    MC_trees[iFile] -> SetBranchStatus("numPFCorJets", 1);
    MC_trees[iFile] -> SetBranchStatus("JetPFCor_dphiMET*", 1);
    MC_trees[iFile] -> SetBranchStatus("Mass2j_PFCor", 1);
    MC_trees[iFile] -> SetBranchStatus("mva*", 1);
    MC_trees[iFile] -> SetBranchStatus("effwt", 1);
    MC_trees[iFile] -> SetBranchStatus("puwt", 1);
  }
  // Prepare the histos
  const int nBins = cutNames.size();
  const float binMin = 0.;
  const float binMax = (float) nBins;
  
  TH1F* h_data = new TH1F ("h_data", "h_data", nBins, binMin, binMax);
  TH1F* h_qcd  = new TH1F ("h_qcd",  "h_qcd",  nBins, binMin, binMax);
  h_data -> Sumw2();
  h_qcd  -> Sumw2();
    
  std::vector<TH1F*> MC_histos;
  for ( int iFile = 0; iFile < MC_files.size(); iFile++ ) {
    TString sampleNumber = "";
    sampleNumber += iFile;
    TString sampleName = "sample";
    sampleName += sampleNumber;
    TH1F* temp = new TH1F (sampleName,sampleName, nBins, binMin, binMax);
    temp -> Sumw2();
    MC_histos.push_back(temp);
  }
     
  TH1D* tmpHist = new TH1D("tmpHist","tmpHist",1,0,10);
  // Fix the QCD scale
  t_data->Draw(TString("numPFCorJets")+TString(">>tmpHist"), QCDCut.c_str(), "goff");
  double nDataQCD = tmpHist -> Integral();    
  tmpHist -> Reset();
  t_qcd ->Draw(TString("numPFCorJets")+TString(">>tmpHist"), QCDCut.c_str(), "goff");
  double nQCD     = tmpHist -> Integral();
  tmpHist -> Reset();
  float QCD_scale = 1.;
  QCD_scale = nDataQCD/nQCD * QCD_frac_ele;
  if (domu) QCD_scale = nDataQCD/nQCD * QCD_frac_mu;
  if (dotop) QCD_scale = 0.;
  
  
  // Fill the histos
  for ( int iCut = 0; iCut < cutNames.size(); iCut++ ) {
   
    float nData = 0;
    float nMC = 0; 
    std::string thisCut = flatCut + " && " + mjjCut + " && " + topCut + " && " + mvaCuts[iCut] + flatCutWeight; 
    std::cout << thisCut << std::endl;

    t_data -> Draw(TString("numPFCorJets")+TString(">>tmpHist"), thisCut.c_str(), "goff");
    h_data -> SetBinContent(iCut+1, tmpHist -> Integral());
    h_data -> SetBinError(iCut+1,sqrt(tmpHist -> Integral()));
    nData += tmpHist -> Integral();
    tmpHist -> Reset();

    t_qcd -> Draw(TString("numPFCorJets")+TString(">>tmpHist"), thisCut.c_str(), "goff");
    h_qcd -> SetBinContent(iCut+1, tmpHist -> Integral());
    h_qcd -> SetBinError(iCut+1, sqrt(tmpHist -> Integral()));
    nMC += tmpHist -> Integral() * QCD_scale; 
    tmpHist -> Reset();
    
    //cycle on MC samples
    for ( int iMC = 0; iMC < MC_histos.size(); iMC++ ) {

      MC_trees[iMC]  -> Draw(TString("numPFCorJets")+TString(">>tmpHist"), thisCut.c_str(), "goff");
      MC_histos[iMC] -> SetBinContent(iCut+1, tmpHist -> Integral());
      MC_histos[iMC] -> SetBinError(iCut+1, sqrt(tmpHist -> Integral()));
      nMC += tmpHist -> Integral() * scale[iMC]; 
      tmpHist -> Reset();

    }
    
    std::cout << "data/MC " << nData/nMC << std::endl; 
    
  }
      
  // Setup the canvas
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.05);

  tdrStyle->SetLegendBorderSize(0);

  TCanvas* c1 = new TCanvas("events","events",10,10, 1050, 800);
  TPad *d1, *d2;

  c1->Divide(1,2,0,0);
  d1 = (TPad*)c1->GetPad(1);
  d1->SetPad(0.01,0.30,0.95,0.99);
  d1->SetLogy(1);
  d2 = (TPad*)c1->GetPad(2);
  d2->SetPad(0.01,0.02,0.95,0.30);

  // Prepare the err box  
  TBox *errbox = new TBox(binMin,0.95,binMax,1.05);
  errbox->SetFillColor(kYellow);

  // Prepare the stack
  THStack* hs = new THStack("hs","MC contribution");
  
  // Fill the stack
  h_qcd -> SetFillColor(kGray+1);
  h_qcd -> SetLineColor(kGray+1);
  h_qcd -> SetLineWidth(0);
  h_qcd -> Scale(QCD_scale);
  hs    -> Add(h_qcd); 
  //cycle on MC samples
  for ( int iMC = 0; iMC < MC_histos.size(); iMC++ ) {
    MC_histos[iMC] -> SetFillColor(colors[iMC]);
    MC_histos[iMC] -> SetLineColor(colors[iMC]);
    MC_histos[iMC] -> SetLineWidth(0);
    MC_histos[iMC] -> Scale(scale[iMC]);
    hs             -> Add(MC_histos[iMC]); 
  }

  // Sum all the backgrounds
  TH1F *th1tot = (TH1F*) h_qcd->Clone("th1tot");
  th1tot->Reset();
  th1tot->Add(h_qcd);
  for ( int iMC = 0; iMC < MC_histos.size(); iMC++ ) th1tot->Add(MC_histos[iMC]);

  d1->cd();
  gPad->SetBottomMargin(0.0);
  gPad->SetTopMargin(0.1);
  gPad->SetRightMargin(0.20);
  gPad->SetLeftMargin(0.12);
  
  // Set up the legend

  float  legX0=0.795, legX1=0.999, legY0=0.4, legY1=0.88;
  TLegend * Leg = new TLegend( legX0, legY0, legX1, legY1);
  Leg->SetFillColor(0);
  Leg->SetFillStyle(0);
  Leg->SetTextSize(0.04);
  Leg->AddEntry(h_data,  cutName.c_str(),  "PLE");
  Leg->AddEntry(h_qcd,  "multijet",  "f");
  Leg->AddEntry(MC_histos[0],  "W+jets",  "f");
  Leg->AddEntry(MC_histos[1],  "WW/WZ/ZZ",  "f");
  Leg->AddEntry(MC_histos[4],  "Z+Jets",  "f");
  Leg->AddEntry(MC_histos[5],  "top",  "f");
  Leg->SetFillColor(0);

  TH1* th1totempty = new TH1D("th1totempty", "th1totempty", nBins, binMin, binMax);
  h_data->SetMarkerStyle(20);
  h_data->SetMarkerSize(1.25);
  h_data->SetLineWidth(2);

  th1tot->SetFillStyle(1);
  th1tot->SetFillColor(1);
  th1tot->SetLineColor(1);
  th1tot->SetMarkerStyle(0);

  char tmpc[100];    sprintf(tmpc,"Events");
  th1totempty->SetYTitle(tmpc);
  th1totempty->GetYaxis()->SetTitleOffset(1.0);
  th1totempty->GetYaxis()->SetLabelOffset(0.01);
  th1totempty->GetYaxis()->SetLabelSize(0.04);

  
  th1tot->SetMinimum(minValue);
  int maxbin = h_data->GetMaximumBin();
  float maxval = h_data->GetBinContent(maxbin);
  th1totempty->SetMaximum(100*maxval);
  th1totempty->SetMinimum(minValue);
  //th1totempty->GetXaxis()->SetRangeUser(step_min,step_max+step_extra);
  h_data->SetMinimum(minValue);
  
  // Draw it all
  th1totempty->Draw();
  h_data->Draw("esame");
  hs->Draw("samehist");
//  th1tot->Draw("e2same");

  h_data->Draw("esame");
  cmspre(intLUMI/1000.0);
  Leg->Draw();  
  gPad->RedrawAxis();
  d2->cd();

  TH1F    * hhratio    = (TH1F*) h_data->Clone("hhratio")  ;
  hhratio->Sumw2();
 
  gPad->SetLeftMargin(0.12);
  gPad->SetTopMargin(0);
  gPad->SetRightMargin(0.20);
  gPad->SetFrameBorderSize(0);
  gPad->SetBottomMargin(0.3);
  gPad->SetTickx();

  hhratio->SetMarkerSize(1.25);
  hhratio->GetYaxis()->SetRangeUser(0.3,1.7);
  hhratio->GetXaxis()->SetTitleOffset(0.9);
  hhratio->GetXaxis()->SetTitleSize(0.15);
  hhratio->GetXaxis()->SetLabelSize(0.15);
  hhratio->SetYTitle("Ratio Data/MC");
  hhratio->GetYaxis()->SetTitleSize(0.1);
  hhratio->GetYaxis()->SetTitleOffset(0.5);
  hhratio->GetYaxis()->CenterTitle(true);
  hhratio->GetYaxis()->SetLabelSize(0.1);
  hhratio->Divide(th1tot);
  double binError(0.0), mcbinentry(0.0), mcerror(0.0);
  for(int i=1; i<=hhratio->GetNbinsX(); ++i) {
    binError = hhratio->GetBinError(i);
    mcerror = th1tot->GetBinError(i);
    mcbinentry = th1tot->GetBinContent(i);
    if(mcbinentry>0.) mcerror /= mcbinentry;
    else mcerror = 0.0;
    hhratio->SetBinError(i, binError);
  }
  TH1D *th1emptyclone = new TH1D("th1emptyclone", "th1emptyclone", nBins, binMin, binMax);
  th1emptyclone->GetYaxis()->SetRangeUser(0.4,1.6);
  //th1emptyclone->GetXaxis()->SetRangeUser(step_min,step_max+step_extra);
  th1emptyclone->GetXaxis()->SetLabelOffset(0.015);
  th1emptyclone->GetXaxis()->SetTitleSize(0.15);
  th1emptyclone->GetXaxis()->SetLabelSize(0.11);
  th1emptyclone->SetYTitle("Ratio Data/MC");
  th1emptyclone->GetYaxis()->SetTitleSize(0.1);
  th1emptyclone->GetXaxis()->SetNdivisions(505);
  th1emptyclone->GetYaxis()->SetNdivisions(505);
  th1emptyclone->GetYaxis()->SetTitleOffset(0.5);
  th1emptyclone->GetYaxis()->CenterTitle(true);
  th1emptyclone->GetYaxis()->SetLabelSize(0.1);
  for ( int iBin = 1; iBin <= nBins; iBin++ ) th1emptyclone -> GetXaxis() -> SetBinLabel(iBin, cutNames[iBin-1].c_str());
  th1emptyclone->Draw();
  errbox->Draw();
  hhratio->Draw("esame");
  TLine *line; line = new TLine(0.,1.0,nBins,1.0);
  line->SetLineStyle(1);
  line->SetLineWidth(1);
  line->SetLineColor(1);
  line->Draw();

  TString outfile = TString("OutDir/")+(domu?TString("mu_"):TString("el_"))+(do3j?TString("3j_"):TString("2j_"))+(dotop?TString("top_"):TString(""))+TString("MVAevents");
  
  c1->Print(outfile+".png");
  c1->Print(outfile+".C");
  c1->Modified();
  c1->Update();
  c1->SaveAs(outfile+".pdf"); 

}

