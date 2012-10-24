#include <iostream>
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
//
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

void mkCutFlowControlPlotsErr(bool domu=false,bool domva=false,
		    bool dovbf=false)
{
  gROOT->ProcessLine(".L tdrstyle.C");
  double intLUMI = 11900.;
  if ( domu ) intLUMI = 11500.;
  const double H190_scale   = 8.782*0.108*1.5*2.* intLUMI/193737; // V
  const double H500_scale   = 1.439*0.080*1.5*2.* intLUMI/131182 ; // V

  const double WJets_scale   = 36257.2* intLUMI/18353019; // V
  const double W4Jets_scale  = 172.6 * intLUMI/5000700;
  const double WW_scale      = 54.838   * intLUMI/9450414; // V
  const double WZ_scale      = 32.3161   * intLUMI/10000267; // V
  const double ZZ_scale      = 8.059   * intLUMI/9702850; // V

  double QCD_scale     = 1. ; // V
  const double ZJets_scale   = 3503.71  * intLUMI/30209426; // V
  const double ttbar_scale   = 225.197 * intLUMI/6893735; // V
  const double SToppS_scale  = 1.75776  * intLUMI/139974; // V anti-top
  const double SToppT_scale  = 30.0042  * intLUMI/1935066; // V
  const double SToppTW_scale = 11.1773  * intLUMI/493458; // V
  const double STopS_scale   = 3.89394  * intLUMI/259960; // top
  const double STopT_scale   = 55.531  * intLUMI/3758221; //V
  const double STopTW_scale  = 11.1773  * intLUMI/497657; // V

  //Cuts for QCD evaluation
  std::string QCDCut = "effwt*puwt*((ggdevt==2) && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>0.8)";
  if ( domu ) QCDCut = "effwt*puwt*((ggdevt==2) && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>0.8)";
  const double QCD_eff_norm_ele = 1.4463;
//  const double QCD_eff_norm_mu = 0.503766;
  const double QCD_eff_norm_mu = 0.0001;
  const double QCD_frac_ele = 0.14; //fit with fixed background a part from Wjets
//  const double QCD_frac_mu  = 0.02;
  const double QCD_frac_mu  = 0.0001;
  //organize cut flow
  std::string MetCut = "30";
  if ( domu ) MetCut = "25";
  std::string dPhiCut = "0.8";
  if ( domu ) dPhiCut = "0.4";
  
  std::vector<std::string> additional_cuts;
  additional_cuts.push_back("effwt*puwt*(ggdevt >= 2 && event_met_pfmet > 30)");
  additional_cuts.push_back("effwt*puwt*(fit_status == 1 && ggdevt >= 2 && event_met_pfmet > 30)");
  additional_cuts.push_back("effwt*puwt*(fit_status == 1 && ggdevt >= 2 && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>" + dPhiCut + ")");
  additional_cuts.push_back("effwt*puwt*(fit_status == 1 && ggdevt == 2 && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>" + dPhiCut + ")");
  additional_cuts.push_back("effwt*puwt*(fit_status == 1 && ggdevt == 3 && event_met_pfmet > 30 && abs(JetPFCor_dphiMET[0])>" + dPhiCut + ")");
  std::vector<std::string> additional_cuts_name;
  additional_cuts_name.push_back("MET > " + MetCut);
  additional_cuts_name.push_back("Good Kin. Fit");
  additional_cuts_name.push_back("|#phi(WJ1)-#phi(MET)| > " + dPhiCut);
  additional_cuts_name.push_back("evtNJ == 2");
  additional_cuts_name.push_back("evtNJ == 3");
  float step_min = 4;
  float step_max_presel = 7;
  float step_max = 12;
  int   step_extra = additional_cuts.size();
  float minValue = 1000;

  TFile f("cutflow_histo.root", "RECREATE");

 // Get the input trees:

 // Data

  TFile *fin2,*H190_file,*H500_file,*wwShape_file,*zzShape_file,*wzShape_file,*wjetsShape_file,*w4jetShape_file,*ttbar_file,*qcd_file1,*zjets_file,*stops_file,*stopt_file,*stoptW_file;


  if (domu) {
    fin2            = new TFile("InData/RD_WmunuJets_DataAll_GoldenJSON_11p5invfb.root", "read");
    wwShape_file    = new TFile("InData/RD_mu_WW_CMSSW532.root", "READ");
    wzShape_file    = new TFile("InData/RD_mu_WZ_CMSSW532.root", "READ");
    zzShape_file    = new TFile("InData/RD_mu_ZZ_CMSSW532.root", "READ");
    if (dovbf)
      wjetsShape_file = new TFile("InData/RD_mu_W4Jets_CMSSW428.root","READ");
    else
      wjetsShape_file = new TFile("InData/RD_mu_WpJ_CMSSW532.root", "READ");
    ttbar_file      = new TFile("InData/RD_mu_TTbar_CMSSW532.root", "READ");
    qcd_file1       = new TFile("InData/RD_mu_WpJ_CMSSW532.root", "READ"); //temporary: for muons take the Wjets as QCD proxy: WRONG!
    zjets_file      = new TFile("InData/RD_mu_ZpJ_CMSSW532.root", "READ");
    stops_file      = new TFile("InData/RD_mu_STopS_T_CMSSW532.root", "READ");
    stopt_file      = new TFile("InData/RD_mu_STopT_T_CMSSW532.root", "READ");
    stoptW_file     = new TFile("InData/RD_mu_STopTW_T_CMSSW532.root", "READ");

  } else { // electrons

    fin2            = new TFile("InData/RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_11p9invfb.root", "READ");
    wwShape_file    = new TFile("InData/RD_el_WW_CMSSW532.root", "READ");
    wzShape_file    = new TFile("InData/RD_el_WZ_CMSSW532.root", "READ");
    zzShape_file    = new TFile("InData/RD_el_ZZ_CMSSW532.root", "READ");
    if (dovbf)
      wjetsShape_file = new TFile("InData/RD_el_W4Jets_CMSSW428.root","READ");
    else
      wjetsShape_file = new TFile("InData/RD_el_WpJ_CMSSW532.root", "READ");
    ttbar_file      = new TFile("InData/RD_el_TTbar_CMSSW532.root", "READ");
    qcd_file1       = new TFile("InData/RDQCD_WenuJets_Isog0p3NoElMVA_11p9invfb.root", "READ");
    zjets_file      = new TFile("InData/RD_el_ZpJ_CMSSW532.root", "READ");
    stops_file      = new TFile("InData/RD_el_STopS_T_CMSSW532.root", "READ");
    stopt_file      = new TFile("InData/RD_el_STopT_T_CMSSW532.root", "READ");
    stoptW_file     = new TFile("InData/RD_el_STopTW_T_CMSSW532.root", "READ");
  }

  // Prepare the trees
  TTree* treedata = (TTree*) fin2->Get("WJet");
  double nData = treedata->GetEntries();
  std::cout << "ndata =" << nData <<std::endl;

  TTree* treeww    = (TTree*)    wwShape_file->Get("WJet");
  TTree* treewz    = (TTree*)    wzShape_file->Get("WJet");
  TTree* treezz    = (TTree*)    zzShape_file->Get("WJet");
  TTree* treewj    = (TTree*) wjetsShape_file->Get("WJet");
  TTree* treettb   = (TTree*)      ttbar_file->Get("WJet");
  TTree* treeqcd   = (TTree*)       qcd_file1->Get("WJet");
  TTree* treezj    = (TTree*)      zjets_file->Get("WJet");
  TTree* treests   = (TTree*)      stops_file->Get("WJet");
  TTree* treestt   = (TTree*)      stopt_file->Get("WJet");
  TTree* treestw   = (TTree*)     stoptW_file->Get("WJet");
  // trash histo for counting
  TH1D* tmpHist = new TH1D("tmpHist","tmpHist",1,0,10);

  TH1* th1data  = (TH1D*) fin2 -> Get("h_events_weighted");

  TH1* th1data_ext  = new TH1D("th1data_ext","th1data_ext", step_max+step_extra, 0, step_max+step_extra);
  for ( int iBin = 1; iBin <= step_max+1; iBin++ ) th1data_ext -> SetBinContent(iBin, th1data->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treedata->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1data_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1data_ext -> Sumw2();
    
  TBox *errbox = new TBox(step_min,0.95,step_max+step_extra,1.05);
  errbox->SetFillColor(kYellow);

  treedata->Draw(TString("numPFCorJets")+TString(">>tmpHist"), QCDCut.c_str(), "goff");
  double nDataQCD = tmpHist -> Integral();
 
    // Get Signal MC

//    TH1* th1H500 = new TH1D("th1H500", "th1H500", pv.NBINS, pv.MINRange, pv.MAXRange);
//    treeh500->Draw(TString(pv.plotvar)+TString(">>th1H500"), the_cut, "goff");

  // Get WW/WZ/ZZ

  TH1* th1ww = (TH1D*) wwShape_file -> Get("h_events_weighted");
  TH1* th1wz = (TH1D*) wzShape_file -> Get("h_events_weighted");
  TH1* th1zz = (TH1D*) zzShape_file -> Get("h_events_weighted");
  th1ww->Sumw2();
  th1wz->Sumw2();
  th1zz->Sumw2();


  TH1* th1ww_ext  = new TH1D("th1ww_ext","th1ww_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1wz_ext  = new TH1D("th1wz_ext","th1wz_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1zz_ext  = new TH1D("th1zz_ext","th1zz_ext", step_max+step_extra, 0, step_max+step_extra);

  //Scale factor for preselection bins
  double weightSF = 1.;
  weightSF = th1ww -> GetBinContent(step_max_presel+1+1)/((TH1D*) wwShape_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1ww_ext -> SetBinContent(iBin, weightSF*th1ww->GetBinContent(iBin));
  weightSF = th1wz -> GetBinContent(step_max_presel+1+1)/((TH1D*) wzShape_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1wz_ext -> SetBinContent(iBin, weightSF*th1wz->GetBinContent(iBin));
  weightSF = th1zz -> GetBinContent(step_max_presel+1+1)/((TH1D*) zzShape_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1zz_ext -> SetBinContent(iBin, weightSF*th1zz->GetBinContent(iBin));

  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1ww_ext -> SetBinContent(iBin, th1ww->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1wz_ext -> SetBinContent(iBin, th1wz->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1zz_ext -> SetBinContent(iBin, th1zz->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treeww->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1ww_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treewz->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1wz_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treezz->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1zz_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1ww_ext -> Sumw2();
  th1wz_ext -> Sumw2();
  th1zz_ext -> Sumw2();

  // Get WJets

  TH1* th1wjets  = (TH1D*) wjetsShape_file -> Get("h_events_weighted");
  th1wjets->Sumw2();
  TH1* th1wjets_ext  = new TH1D("th1wjets_ext","th1wjets_ext", step_max+step_extra, 0, step_max+step_extra);

  weightSF = th1wjets -> GetBinContent(step_max_presel+1+1)/((TH1D*) wjetsShape_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1wjets_ext -> SetBinContent(iBin, weightSF*th1wjets->GetBinContent(iBin));

  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1wjets_ext -> SetBinContent(iBin, th1wjets->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treewj->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1wjets_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1wjets_ext -> Sumw2();

  // Get ttbar

  TH1* th1Top = (TH1D*) ttbar_file -> Get("h_events_weighted");
  th1Top->Sumw2();
  TH1* th1Top_ext  = new TH1D("th1Top_ext","th1Top_ext", step_max+step_extra, 0, step_max+step_extra);

  weightSF = th1Top -> GetBinContent(step_max_presel+1+1)/((TH1D*) ttbar_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1Top_ext -> SetBinContent(iBin, weightSF*th1Top->GetBinContent(iBin));

  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1Top_ext -> SetBinContent(iBin, th1Top->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treettb->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1Top_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1Top_ext -> Sumw2();

  // Get QCD


  TH1* th1qcd = (TH1D*) qcd_file1 -> Get("h_events_weighted");
  th1qcd->Sumw2();
  TH1* th1qcd_ext  = new TH1D("th1qcd_ext","th1qcd_ext", step_max+step_extra, 0, step_max+step_extra);
  
  treeqcd->Draw(TString("numPFCorJets")+TString(">>tmpHist"), QCDCut.c_str(), "goff");
  double nQCD     = tmpHist -> Integral();
  tmpHist -> Reset();

  //Fix the QCD scale
  QCD_scale = nDataQCD/nQCD * QCD_frac_ele;
  if (domu) QCD_scale = nDataQCD/nQCD * QCD_frac_mu;
  std::cout << " qcd_scale  " << QCD_scale <<std::endl;

  if (!domu) for ( int iBin = 1; iBin <= step_max+1; iBin++ ) th1qcd_ext -> SetBinContent(iBin,QCD_eff_norm_ele/QCD_scale*th1qcd->GetBinContent(iBin));
  else for ( int iBin = 1; iBin <= step_max+1; iBin++ ) th1qcd_ext -> SetBinContent(iBin,QCD_eff_norm_mu/QCD_scale*th1qcd->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treeqcd->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1qcd_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1qcd_ext -> Sumw2();

  th1qcd_ext->Scale(QCD_scale);

  // Get Z+Jets

  TH1* th1zjets = (TH1D*) zjets_file -> Get("h_events_weighted");
  th1zjets->Sumw2();
  TH1* th1zjets_ext  = new TH1D("th1zjets_ext","th1zjets_ext", step_max+step_extra, 0, step_max+step_extra);
  
  weightSF = th1zjets -> GetBinContent(step_max_presel+1+1)/((TH1D*) zjets_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1zjets_ext -> SetBinContent(iBin, weightSF*th1zjets->GetBinContent(iBin));

  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1zjets_ext -> SetBinContent(iBin, th1zjets->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treezj->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1zjets_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1zjets_ext -> Sumw2();

  // Get Single top

  TH1* th1stops = (TH1D*) stops_file -> Get("h_events_weighted");
  TH1* th1stopt = (TH1D*) stopt_file -> Get("h_events_weighted");
  TH1* th1stoptw = (TH1D*) stoptW_file -> Get("h_events_weighted");
  th1stops->Sumw2();
  th1stopt->Sumw2();
  th1stoptw->Sumw2();

  TH1* th1stops_ext  = new TH1D("th1stops_ext","th1stops_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1stopt_ext  = new TH1D("th1stopt_ext","th1stopt_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1stoptw_ext  = new TH1D("th1stopsw_ext","th1stopsw_ext", step_max+step_extra, 0, step_max+step_extra);
  
  weightSF = th1stops -> GetBinContent(step_max_presel+1+1)/((TH1D*) stops_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stops_ext -> SetBinContent(iBin, weightSF*th1stops->GetBinContent(iBin));
  weightSF = th1stopt -> GetBinContent(step_max_presel+1+1)/((TH1D*) stopt_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stopt_ext -> SetBinContent(iBin, weightSF*th1stopt->GetBinContent(iBin));
  weightSF = th1stoptw -> GetBinContent(step_max_presel+1+1)/((TH1D*) stoptW_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stoptw_ext -> SetBinContent(iBin, weightSF*th1stoptw->GetBinContent(iBin));
  
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stops_ext -> SetBinContent(iBin, th1stops->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stopt_ext -> SetBinContent(iBin, th1stopt->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stoptw_ext -> SetBinContent(iBin, th1stoptw->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treests->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stops_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treestt->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stopt_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treestw->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stoptw_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1stops_ext -> Sumw2();
  th1stopt_ext -> Sumw2();
  th1stopsw_ext -> Sumw2();

  TFile* stopps_file =  new TFile("InData/RD_el_STopS_Tbar_CMSSW532.root", "READ");
  TTree* tree64 = (TTree*) stopps_file->Get("WJet");
  TFile* stoppt_file =  new TFile("InData/RD_el_STopT_Tbar_CMSSW532.root", "READ");
  TTree* tree65 = (TTree*) stoppt_file->Get("WJet");
  TFile* stopptW_file =  new TFile("InData/RD_el_STopTW_Tbar_CMSSW532.root", "READ");
  TTree* tree66 = (TTree*) stopptW_file->Get("WJet");
  TH1* th1stopps = (TH1D*) stopps_file -> Get("h_events_weighted");
  TH1* th1stoppt = (TH1D*) stoppt_file -> Get("h_events_weighted");
  TH1* th1stopptw = (TH1D*) stopptW_file -> Get("h_events_weighted");
  th1stopps->Sumw2();
  th1stoppt->Sumw2();
  th1stopptw->Sumw2();

  TH1* th1stopps_ext  = new TH1D("th1stopps_ext","th1stopps_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1stoppt_ext  = new TH1D("th1stoppt_ext","th1stoppt_ext", step_max+step_extra, 0, step_max+step_extra);
  TH1* th1stopptw_ext  = new TH1D("th1stopptw_ext","th1stopptw_ext", step_max+step_extra, 0, step_max+step_extra);
  
  weightSF = th1stopps -> GetBinContent(step_max_presel+1+1)/((TH1D*) stopps_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stopps_ext -> SetBinContent(iBin, weightSF*th1stopps->GetBinContent(iBin));
  weightSF = th1stoppt -> GetBinContent(step_max_presel+1+1)/((TH1D*) stoppt_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stoppt_ext -> SetBinContent(iBin, weightSF*th1stoppt->GetBinContent(iBin));
  weightSF = th1stopptw -> GetBinContent(step_max_presel+1+1)/((TH1D*) stopptW_file -> Get("h_events"))->GetBinContent(step_max_presel+1+1);
  for ( int iBin = 1; iBin <= step_max_presel+1; iBin++ ) th1stopptw_ext -> SetBinContent(iBin, weightSF*th1stopptw->GetBinContent(iBin));

  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stopps_ext -> SetBinContent(iBin, th1stopps->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stoppt_ext -> SetBinContent(iBin, th1stoppt->GetBinContent(iBin));
  for ( int iBin = step_max_presel+1+1; iBin <= step_max+1; iBin++ ) th1stopptw_ext -> SetBinContent(iBin, th1stopptw->GetBinContent(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) {
   treests->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stopps_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treestt->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stoppt_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
   treestw->Draw(TString("numPFCorJets")+TString(">>tmpHist"), additional_cuts[iExtraStep].c_str(), "goff");
   th1stopptw_ext -> SetBinContent(step_max+1+iExtraStep, tmpHist->Integral()); 
   tmpHist -> Reset();
  }
  th1stopps_ext -> Sumw2();
  th1stoppt_ext -> Sumw2();
  th1stopptw_ext -> Sumw2();

  // Setup the canvas

//    gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.05);

  tdrStyle->SetLegendBorderSize(0);

  th1data->Sumw2();

  TCanvas* c1 = new TCanvas("events","events",10,10, 1050, 800);
  TPad *d1, *d2;

  c1->Divide(1,2,0,0);
  d1 = (TPad*)c1->GetPad(1);
  d1->SetPad(0.01,0.30,0.95,0.99);
  d1->SetLogy(1);
  d2 = (TPad*)c1->GetPad(2);
  d2->SetPad(0.01,0.02,0.95,0.30);

  // Setup the stack, scale the histos

  THStack* hs = new THStack("hs","MC contribution");
  th1Top_ext->Scale(ttbar_scale);
  th1Top_ext->SetFillColor(kGreen+2);
  th1Top_ext->SetLineColor(kGreen+2);
  th1Top_ext->SetLineWidth(0);

  th1stops_ext->Scale(STopS_scale);
  th1stops_ext->SetFillColor(7);
  th1stops_ext->SetLineWidth(2);
  th1stopps_ext->Scale(SToppS_scale);
  th1stopps_ext->SetFillColor(7);
  th1stopps_ext->SetLineWidth(2);
  th1stopt_ext->Scale(STopT_scale);
  th1stopt_ext->SetFillColor(13);
  th1stopt_ext->SetLineWidth(2);
  th1stoppt_ext->SetLineWidth(2);
  th1stoppt_ext->Scale(SToppT_scale);
  th1stoppt_ext->SetFillColor(13);
  th1stoptw_ext->Scale(STopTW_scale);
  th1stoptw_ext->SetFillColor(9);
  th1stoptw_ext->SetLineWidth(2);
  th1stopptw_ext->Scale(SToppTW_scale);
  th1stopptw_ext->SetFillColor(9);
  th1stopptw_ext->SetLineWidth(2);

  th1wjets_ext->Scale(dovbf? W4Jets_scale : WJets_scale);
  th1wjets_ext->SetFillColor(kRed);
  th1wjets_ext->SetLineColor(kRed);
  th1wjets_ext->SetLineWidth(0);
  th1ww_ext->Scale(WW_scale);
  th1ww_ext->SetFillColor(kAzure+8);
  th1ww_ext->SetLineColor(kAzure+8);
  th1ww_ext->SetLineWidth(0);
  th1wz_ext->Scale(WZ_scale);
  th1wz_ext->SetFillColor(11);
  th1wz_ext->SetLineWidth(0);
  th1zz_ext->Scale(ZZ_scale);
  th1zz_ext->SetFillColor(11);
  th1zz_ext->SetLineWidth(0);
  
  th1qcd_ext->SetFillColor(kGray+1);
  th1qcd_ext->SetLineColor(kGray+1);
  th1qcd_ext->SetLineWidth(0);
  th1zjets_ext->Scale(ZJets_scale);
  th1zjets_ext->SetFillColor(kYellow);
  th1zjets_ext->SetLineColor(kYellow);
  th1zjets_ext->SetLineWidth(0);
  std::cout << "qcd "  << th1qcd_ext->Integral()   << std::endl;
  std::cout << "tt "   << th1Top_ext->Integral()   << std::endl;
  std::cout << "ww "   << th1ww_ext->Integral()    << std::endl;
  std::cout << "wz "   << th1wz_ext->Integral()    << std::endl;
  std::cout << "zz "   << th1zz_ext->Integral()    << std::endl;
  std::cout << "z "    << th1zjets_ext->Integral() << std::endl;

  double den_qcd = 
    th1Top_ext->Integral()+
    th1stops_ext->Integral()+
    th1stopt_ext->Integral()+
    th1stoptw_ext->Integral()+
    th1stopps_ext->Integral()+
    th1stoppt_ext->Integral()+
    th1stopptw_ext->Integral()+
    th1wjets_ext->Integral()+
    th1ww_ext->Integral()+
    th1wz_ext->Integral()+
    th1zz_ext->Integral()+
    th1zjets_ext->Integral();

  double den = 
    th1Top_ext->Integral()+
    th1stops_ext->Integral()+
    th1stopt_ext->Integral()+
    th1stoptw_ext->Integral()+
    th1stopps_ext->Integral()+
    th1stoppt_ext->Integral()+
    th1stopptw_ext->Integral()+
    th1wjets_ext->Integral()+
    th1ww_ext->Integral()+
    th1wz_ext->Integral()+
    th1zz_ext->Integral()+
    th1qcd_ext->Integral()+
    th1zjets_ext->Integral();

  std::cout << "den = " <<den <<std::endl;
  std::cout <<" data " <<  th1data_ext->Integral() << std::endl;
/*
  th1qcd->Scale   (th1data->Integral()/den); std::cout <<"qcd "   << th1qcd->Integral()   << std::endl;
  th1Top->Scale   (th1data->Integral()/den); std::cout <<"tt "    << th1Top->Integral()   << std::endl;
  th1stops->Scale (th1data->Integral()/den); std::cout <<"stops " << th1stops->Integral() << std::endl;
  th1stopt->Scale (th1data->Integral()/den); std::cout <<"stopt " << th1stopt->Integral() << std::endl;
  th1stoptw->Scale(th1data->Integral()/den); std::cout <<"stoptw "<< th1stoptw->Integral()<< std::endl;
  th1stopps->Scale (th1data->Integral()/den); std::cout <<"stops " << th1stopps->Integral() << std::endl;
  th1stoppt->Scale (th1data->Integral()/den); std::cout <<"stopt " << th1stoppt->Integral() << std::endl;
  th1stopptw->Scale(th1data->Integral()/den); std::cout <<"stoptw "<< th1stopptw->Integral()<< std::endl;
  th1wjets->Scale (th1data->Integral()/den); std::cout <<"wjets " << th1wjets->Integral() << std::endl;
  th1ww->Scale    (th1data->Integral()/den); std::cout <<"ww "    << th1ww->Integral()    << std::endl;
  th1wz->Scale    (th1data->Integral()/den); std::cout << "wz "   << th1wz->Integral()    << std::endl;
  th1zz->Scale    (th1data->Integral()/den); std::cout << "zz "   << th1zz->Integral()    << std::endl;
  th1zjets->Scale (th1data->Integral()/den); std::cout << "z "    << th1zjets->Integral() << std::endl;
*/
  double den2 =
    th1Top_ext->Integral()+
    th1stops_ext->Integral()+
    th1stopt_ext->Integral()+
    th1stoptw_ext->Integral()+
    th1stopps_ext->Integral()+
    th1stoppt_ext->Integral()+
    th1stopptw_ext->Integral()+
    th1wjets_ext->Integral()+
    th1ww_ext->Integral()+
    th1wz_ext->Integral()+
    th1zz_ext->Integral()+
    th1qcd_ext->Integral()+
    th1zjets_ext->Integral();

  std::cout << "den2 " << den2 << std::endl;

  th1Top_ext->Add(th1stoptw_ext,1);
  th1Top_ext->Add(th1stopt_ext,1);
  th1Top_ext->Add(th1stops_ext,1);
  th1Top_ext->Add(th1stopptw_ext,1);
  th1Top_ext->Add(th1stoppt_ext,1);
  th1Top_ext->Add(th1stopps_ext,1);
  th1ww_ext->Add(th1wz_ext,1);
  th1ww_ext->Add(th1zz_ext,1);

  // Sum all the backgrounds

  TH1D *th1tot = (TH1D*)th1wjets_ext->Clone("th1tot");
  th1tot->Reset();
  th1tot->Add(th1ww_ext,1);
  th1tot->Add(th1qcd_ext,1);
  th1tot->Add(th1Top_ext,1);
  th1tot->Add(th1wjets_ext,1);
  th1tot->Add(th1zjets_ext,1);
  TH1D* th1totClone = ( TH1D*) th1tot->Clone("th1totClone");
  th1totClone->SetMarkerStyle(0);
  th1totClone->SetFillStyle(3003);
  th1totClone->SetFillColor(1);
  th1totClone->SetLineColor(0);
  double binErr(0.0);
  for(int i=1; i<=th1totClone->GetNbinsX(); ++i) {
    binErr = sqrt(
		  (th1ww_ext->GetBinError(i))**2 +
		  (th1qcd_ext->GetBinError(i))**2 +
		  (th1Top_ext->GetBinError(i))**2 +
		  (th1wjets_ext->GetBinError(i))**2+
		  (th1zjets_ext->GetBinError(i))**2);
    th1totClone->SetBinError(i, binErr);
  }

  // Compose the stack

  hs->Add(th1zjets_ext);
  d1->cd();
  gPad->SetBottomMargin(0.0);
  gPad->SetTopMargin(0.1);
  gPad->SetRightMargin(0.20);
  gPad->SetLeftMargin(0.12);

  hs->Add(th1qcd_ext);
  hs->Add(th1Top_ext);
  hs->Add(th1ww_ext);
  hs->Add(th1wjets_ext);

  // Set up the legend

  float  legX0=0.795, legX1=0.999, legY0=0.4, legY1=0.88;
  // float  legX0=0.35, legX1=0.85, legY0=0.4, legY1=0.88;
  // float  legX0=0.18, legX1=0.52, legY0=0.4, legY1=0.88;
  TLegend * Leg = new TLegend( legX0, legY0, legX1, legY1);
  Leg->SetFillColor(0);
  Leg->SetFillStyle(0);
  Leg->SetTextSize(0.04);
  if (domu)
    Leg->AddEntry(th1data_ext,  "Muon Data",  "PLE");
  else
    Leg->AddEntry(th1data_ext,  "Electron Data",  "PLE");
  Leg->AddEntry(th1wjets_ext,  "W+jets",  "f");
  Leg->AddEntry(th1ww_ext,  "WW/WZ/ZZ ",  "f");
  Leg->AddEntry(th1Top_ext,  "top",  "f");
  Leg->AddEntry(th1qcd_ext,  "multijet",  "f");

  Leg->AddEntry(th1zjets_ext,  "Z+Jets",  "f");
  Leg->AddEntry(th1tot,  "MC Uncertainty",  "f");
  Leg->SetFillColor(0);

/*
  th1H500->SetLineColor(kBlack);
  th1H500->SetLineWidth(3);
  th1H500->Scale(10);
*/

  TH1* th1totempty = new TH1D("th1totempty", "th1totempty", th1data_ext->GetXaxis()->GetNbins(), th1data_ext->GetXaxis()->GetXmin(), th1data_ext->GetXaxis()->GetXmax());
  th1data_ext->SetMarkerStyle(20);
  th1data_ext->SetMarkerSize(1.25);
  th1data_ext->SetLineWidth(2);

  th1tot->SetFillStyle(3001);
  th1tot->SetFillColor(1);
  th1tot->SetLineColor(1);
  th1tot->SetMarkerStyle(0);

  char tmpc[100];    sprintf(tmpc,"Events");
  th1totempty->SetYTitle(tmpc);
  //  th1totempty->GetYaxis()->SetTitleSize(0.1);
  th1totempty->GetYaxis()->SetTitleOffset(1.0);
  th1totempty->GetYaxis()->SetLabelOffset(0.01);
  //  th1totempty->GetYaxis()->CenterTitle(true);
  th1totempty->GetYaxis()->SetLabelSize(0.04);
  //th1totClone->Draw("e3");   

  th1tot->SetMinimum(minValue);
  int maxbin = th1data_ext->GetMaximumBin();
  float maxval = th1data_ext->GetBinContent(step_min+1);
  std::cout << "maxval " <<maxval <<std::endl;
  th1totempty->SetMaximum(1.6*maxval);
  th1totempty->SetMinimum(minValue);
  th1totempty->GetXaxis()->SetRangeUser(step_min,step_max+step_extra);
//  if(pv.slog==1) th1totempty->SetMaximum(1.6*maxval);
  th1data_ext->SetMinimum(minValue);

  // Draw it all

  th1totempty->Draw();
  //th1tot->Draw("e2same");
  th1data_ext->Draw("esame");
  hs->Draw("samehist");
  th1tot->Draw("e2same");

  th1data_ext->Draw("esame");
  cmspre(intLUMI/1000.0);
  Leg->Draw();  
  // th1data->Draw("Axissame");
  gPad->RedrawAxis();
  d2->cd();

  TH1F    * hhratio    = (TH1F*) th1data_ext->Clone("hhratio")  ;
  hhratio->Sumw2();
 
  gPad->SetLeftMargin(0.12);
  gPad->SetTopMargin(0);
  gPad->SetRightMargin(0.20);
  gPad->SetFrameBorderSize(0);
  gPad->SetBottomMargin(0.3);
  gPad->SetTickx();

  hhratio->SetMarkerSize(1.25);
  //  hhratio->GetYaxis()->SetRangeUser(0.48,1.52);
  hhratio->GetYaxis()->SetRangeUser(0.3,1.7);
//  hhratio->GetXaxis()->SetTitle(pv.xlabel);
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
  TH1D *th1emptyclone = new TH1D("th1emptyclone", "th1emptyclone", th1data_ext->GetXaxis()->GetNbins(), th1data_ext->GetXaxis()->GetXmin(), th1data_ext->GetXaxis()->GetXmax());
  th1emptyclone->GetYaxis()->SetRangeUser(0.4,1.6);
//  th1emptyclone->GetXaxis()->SetTitle(pv.xlabel);
  th1emptyclone->GetXaxis()->SetRangeUser(step_min,step_max+step_extra);
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
  for ( int iBin = step_min+1; iBin <= step_max+1; iBin++ ) th1emptyclone -> GetXaxis() -> SetBinLabel(iBin, th1data -> GetXaxis() -> GetBinLabel(iBin));
  for ( int iExtraStep = 0; iExtraStep < step_extra; iExtraStep++ ) th1emptyclone -> GetXaxis() -> SetBinLabel(step_max+1+iExtraStep, additional_cuts_name[iExtraStep].c_str());
  th1emptyclone->Draw();
  errbox->Draw();
  hhratio->Draw("esame");
  TLine *line; line = new TLine(step_min,1.0,step_max+step_extra,1.0);
  line->SetLineStyle(1);
  line->SetLineWidth(1);
  line->SetLineColor(1);
  line->Draw();

  TString outfile = TString("OutDir/")+(domu?TString("mu_"):TString("el_"))+TString("events");
  
  c1->Print(outfile+".png");
  c1->Print(outfile+".C");
  //gPad->WaitPrimitive();
  c1->Modified();
  c1->Update();
  c1->SaveAs(outfile+".pdf"); 

  f.cd();
  th1data -> Write();
  th1data_ext -> Write();

  // f.Write();

}

