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
// The goal of this macro is to get an effective normalization before the QCD fit is done. 
// If we assume that the QCD tree is pure and well reproduces the QCD behaviour in the signal region ( iso applied )
// then we can find the best scale to have a good data/MC agreement before the QCD fit. If, for each bin, the agreement
// is good, we can conclude that the assumption is correct and we can dispose of the cut flow comparison for any step
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

void findQCDfastNorm(bool domu=false,bool domva=false,
		    bool dovbf=false)
{
  gROOT->ProcessLine(".L tdrstyle.C");
  double intLUMI = 11900.;
  if ( domu ) intLUMI = 11500.;

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

  float QCD_scale_init =  1. ; // V

  
  //Cuts for QCD evaluation

  float step_min = 4;
  float step_max = 12;
  float step_max_presel = 7;

  TFile *fin2;
  TFile *fQCD;
  std::vector<TFile*> MC_files;

  if (domu) {
    fin2            = new TFile("InData/RD_WmunuJets_DataAll_GoldenJSON_11p5invfb.root", "read");
    fQCD            = new TFile("InData/RD_mu_WpJ_CMSSW532.root", "read");
    if (dovbf)
      MC_files.push_back( new TFile("InData/RD_mu_W4Jets_CMSSW428.root","READ") );
    else
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
    if (dovbf)
      MC_files.push_back( new TFile("InData/RD_el_W4Jets_CMSSW428.root","READ") );
    else
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

  // Prepare the histos
  TH1F* h_data = (TH1F*) fin2->Get("h_events_weighted");
  TH1F* h_qcd = (TH1F*)  fQCD->Get("h_events_weighted");
  
  std::vector<TH1F*> MC_histos;
  for ( int iFile = 0; iFile < MC_files.size(); iFile++ ) MC_histos.push_back((TH1F*)    MC_files[iFile]->Get("h_events_weighted"));
  std::vector<TH1F*> MC_histos_noWeight;
  for ( int iFile = 0; iFile < MC_files.size(); iFile++ ) MC_histos_noWeight.push_back((TH1F*)    MC_files[iFile]->Get("h_events"));
  
  
  // Now prepare the sums: alpha = sum(1-MC/D)/sum(QCD/D)
  long double num  = 0;
  long double den  = 0;
  long double alpha;

  for ( int iBin = step_min+1; iBin <= step_max+1; iBin++ ) {
    double nDATA = h_data -> GetBinContent(iBin);
    double nQCD  = h_qcd  -> GetBinContent(iBin);
    double nMC = 0;
    for ( int iFile = 0; iFile < MC_files.size(); iFile++ ) {
     float weightSF = 1.;
     if ( iBin <= step_max_presel+1 ) 
       weightSF = MC_histos[iFile] -> GetBinContent(step_max_presel+1+1)/MC_histos_noWeight[iFile] -> GetBinContent(step_max_presel+1+1);
     nMC += MC_histos[iFile] -> GetBinContent(iBin) * scale[iFile] * weightSF;
    }
    num += 1. - nMC/nDATA;
    den += nQCD/nDATA;
  }
  
  alpha = num/den;
  
  std::cout << "alpha is " << alpha << std::endl;

  
}

