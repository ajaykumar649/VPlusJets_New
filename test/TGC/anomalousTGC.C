/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *   Kellen McGee, Johns Hopkins
 *   Kristina Krylova, Buffalo
 *
 * Description:
 *
 * Implementation details:
 *  - Uses standard ROOT classes.
 *  - Script takes the root files for the normal and 
 *    anomalous couplings as inputs, and compares their distributions.
 *
 * History:
 *  - Kristina+Kellen wrote the initial macro to plot 
 *    kinematic distributions for various aTGC values.
 *  - Kalanand made several changes and made it compilable
 *    to improve the execution time.
 *
 * Copyright (C) 2011 FNAL 
 ********************************************************************/


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <stdio.h>
#include <vector>
#include <string.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCut.h>
#include <TStyle.h>
#include <TString.h> 
#include <TLegend.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <iostream>

using namespace std; 


//const string inputDirPath = "/pnfs/cms/WAX/11/store/user/lnujj/MCFM_WW_aTGC";
const string inputDirPath = "dcap://cmsdca1.fnal.gov:24140/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lnujj/MCFM_WW_aTGC/Grid2D_LambdaZ_KappaGamma";
const string f0Name = "WW_NLO_norm";
string fName[7];
const string treeName = "h300";
TFile* f[7];
TTree* tree[7];
TFile* fSM;
TTree* treeSM;


const string var1 = "sqrt(px5*px5+py5*py5)";
const string histName1 = "Jet1pT";
const string title1 = "Jet 1 P_{T} [GeV]";

const string var2 = "sqrt(px4*px4+py4*py4)";
const string histName2 = "LeptonpT";
const string title2 = "Lepton P_{T} [GeV]";


const string var3 = "sqrt((px3+px4)*(px3+px4)+(py3+py4)*(py3+py4))";
const string histName3 = "LeptonicWpT";
const string title3 = "Leptonic W P_{T} [GeV]";


const string var4 = "sqrt((px5+px6)*(px5+px6)+(py5+py6)*(py5+py6))";
const string histName4 = "HadronicWpT";
const string title4 = "Hadronic W P_{T} [GeV]";

const string var5 = "sqrt((E3+E4+E5+E6)*(E3+E4+E5+E6)-((px3+px4+px5+px6)*(px3+px4+px5+px6)+(py3+py4+py5+py6)*(py3+py4+py5+py6)+(pz3+pz4+pz5+pz6)*(pz3+pz4+pz5+pz6)))";
const string histName5 = "Mass4Body";
const string title5 = "M_{lnujj} [GeV]";


const string var6 = "sqrt( (E3+E4)**2 - (pz3+pz4)**2 - (py3+py4)**2 - (px3+px4)**2 )";
const string histName6 = "LeptonicWmass";
const string title6 = "Lept. W mass [GeV]";


const string var7 = "sqrt( (E5+E6)**2 - (pz5+pz6)**2 - (py5+py6)**2 - (px5+px6)**2 )";
const string histName7 = "HadronicWmass";
const string title7 = "Hadronic W mass [GeV]";

const string var8 = "px3+px4+px5+px6+px7";
const string histName8 = "SumPx";
const string title8 = "Sum P_{x} [GeV]";

const string var9 = "py3+py4+py5+py6+py7";
const string histName9 = "SumPy";
const string title9 = "Sum P_{y} [GeV]";



//define alternate coupling strings
string steps[7] = {"-0.6", "-0.4", "-0.2", "0.0", "0.2", "0.4", "0.6"};



/////////// define all member functions here first ////////////////
void runAnomalousTGC(int atgcIndex);
TH1D* aesthetics( TH1D* hist, Color_t col_num);
void title_binning( const char title[], int bins, 
		    float min, float max );
TLegend* legend( std::string label, std::vector<TH1D*> &hist, bool upperleft=false);
//////void scaling( TH1D* hist, Int_t dm_max, bool doTruncation=false );
void draw_hist( Int_t varIndex, bool doYaxisTitle, std::vector<TH1D*> &hist);
TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator);
void  MakeComparisonPlot( std::string label, Int_t varIndex, bool doLogY=false );
void GetCrossSection(string paramName, float& xsec, float& err);











////////////// primary function //////////////////////////
void anomalousTGC() {
  fSM = TFile::Open( (inputDirPath+"/LambdaZ_0.0_dKgamma_0.0.root").c_str(), "read");
  treeSM = (TTree*)fSM->Get(treeName.c_str());

  for (int j=0; j<7; ++j) { runAnomalousTGC(j); }

}








////////////// utility  function //////////////////////////
void runAnomalousTGC(int atgcIndex) {

  char temp[100];
  sprintf(temp, "#lambda_Z = %.1f, #kappa#gamma = ", 0.2*(atgcIndex-3));
  std::string label = string(temp);


  // read the input samples for various values of aTGC parameters
  int normtree[7];
  for(int i=0; i<7; ++i) {
    sprintf(temp, "LambdaZ_%.1f_dKgamma_%.1f.root", 0.2*(atgcIndex-3), 0.2*(i-3));
    fName[i] = string(temp);
    f[i] = TFile::Open( (inputDirPath+"/"+fName[i]).c_str(), "read");
    tree[i] = (TTree*)f[i]->Get(treeName.c_str());
    normtree[i] = tree[i]->GetEntries();
  }


  ///////////// Print out the tree names and number of entries etc. /////////////
  for(int i=0; i<7; ++i) {
    cout << fName[i].c_str() << " : "  << treeName.c_str() << " : " << normtree[i] << endl;
  }



  //////////////////////////////////////////////////////////////////////////////////


  // Filling histograms for the five kinematic variables, e.g., W pT
  MakeComparisonPlot(label, 1, true);
  MakeComparisonPlot(label, 2, true);
  MakeComparisonPlot(label, 3, true);
  MakeComparisonPlot(label, 4, true);
  MakeComparisonPlot(label, 5, true);

  MakeComparisonPlot(label, 6, false);
  MakeComparisonPlot(label, 7, false);
  MakeComparisonPlot(label, 8, true);
  MakeComparisonPlot(label, 9, true);
}








TH1D* aesthetics( TH1D* hist, Color_t col_num){
  hist->SetLineColor(col_num);
  hist->SetMarkerColor(col_num);
  hist->GetYaxis()->SetTitleOffset(1.15);
  return hist;
} // aesthetics




void title_binning( const char title[], int bins, float min, float max ) {
  sprintf((char*) title, (char*) "A.U. / %d GeV", (int) ((max - min)/bins) );
  }





TLegend* legend( std::string label, std::vector<TH1D*> &hist, bool upperleft) {
  Double_t x1=0.7, y1=0.49;
  if(upperleft) x1=0.22;
  Double_t  x2=x1+0.2, y2=y1+0.42;
  TLegend *l = new TLegend( x1, y1, x2, y2 );

  for(int i=0; i<7; ++i) {
    l->AddEntry(hist[i], (label+steps[i]).c_str(), "lp");
  }
  l->SetFillColor(0);
  return l;
} // legend







// // // void scaling( TH1D* hist, Int_t dm_max, bool doTruncation){

// // //   if(doTruncation) hist->GetXaxis()->SetRangeUser( 0.1, dm_max ); 
// // //   Double_t scale = 1.0;
// // //   Double_t integral = hist->Integral();
// // //   cout << "integral " << integral << endl;
// // //   if(integral !=0.0) scale=1.0/integral;
// // //   cout << "maximum dm " << dm_max  << endl;
// // //   cout << "scale " << scale << endl;
// // //   cout << "integral " << integral << endl;
// // //   hist->Scale(scale);
// // // } // scaling






void draw_hist( Int_t varIndex, bool doYaxisTitle, std::vector<TH1D*> &hist){

  string variable;
  string plotname;
  string axis_label;
  Int_t bins; 
  Float_t dm_min; 
  Float_t dm_max;

  //////// define the histogram binning etc.////////////
  if(varIndex==1) {
    variable = var1;
    plotname = histName1;
    axis_label = title1;
    cout << "varIndex is one" << endl;
  }
  else if(varIndex==2) {
    variable = var2;
    plotname = histName2;
    axis_label = title2;
    cout << "varIndex is two" << endl;
  }
  else if(varIndex==3) {
    variable = var3;
    plotname = histName3;
    axis_label = title3;
    cout << "varIndex is three" << endl;
  }
  else if(varIndex==4) {
    variable = var4;
    plotname = histName4;
    axis_label = title4;
    cout << "varIndex is four" << endl;
  }
  else if(varIndex==5) {
    variable = var5;
    plotname = histName5;
    axis_label = title5;
    cout << "varIndex is five" << endl;
  }
  else if(varIndex==6) {
    variable = var6;
    plotname = histName6;
    axis_label = title6;
    cout << "varIndex is six" << endl;
  }
  else if(varIndex==7) {
    variable = var7;
    plotname = histName7;
    axis_label = title7;
    cout << "varIndex is seven" << endl;
  }
  else if(varIndex==8) {
    variable = var8;
    plotname = histName8;
    axis_label = title8;
    cout << "varIndex is eight" << endl;
  }
  else if(varIndex==9) {
    variable = var9;
    plotname = histName9;
    axis_label = title9;
    cout << "varIndex is nine" << endl;
  }
  //  else return 0;

  if(varIndex==5) {
    bins = 30; 
    dm_min = 150.; 
    dm_max = 500.;
  }
  else if(varIndex==6 || varIndex==7) {
    bins = 32; 
    dm_min = 40.; 
    dm_max = 200.;
  }
  else if(varIndex==8 || varIndex==9) {
    bins = 40; 
    dm_min = -100.; 
    dm_max = 100.;
  }
  else {
    bins = 18; 
    dm_min = 50.; 
    dm_max = 500.;
  }

  //////////// now make histogram ////////////////////////
  TString histname = "";
  float xsection = 0.0, error = 0.0;
  const char title[100]="";
  title_binning(title, bins, dm_min, dm_max);

  for(int i=0; i<7; ++i) {
    histname = TString(plotname) + TString(varIndex) + TString(steps[i]);
    hist[i] = new TH1D(histname, "", bins, dm_min, dm_max);

    Color_t color = kBlack;
    if(i==0) color = kBlue;
    if(i==1) color = kBlue-4;
    if(i==2) color = kBlue-7;
    if(i==3) color = kBlack;
    if(i==4) color = kRed-7;
    if(i==5) color = kRed-4;
    if(i==6) color = kRed;

    hist[i]->SetLineColor(color);
    hist[i]->SetMarkerColor(color);

    // aesthetics( hist[i], i+1 );
    hist[i]->Sumw2();
    tree[i]->Draw( TString(variable)+TString(">>")+histname, "", "goff");

    GetCrossSection(fName[i], xsection, error);

    hist[i]->Scale(0.001*xsection/tree[i]->GetEntries());
    hist[i]->GetXaxis()->SetTitle(axis_label.c_str());
    //   if (doYaxisTitle){
    hist[i]->SetNdivisions(505);      
    hist[i]->GetYaxis()->SetTitle(title);
    // } // if

    hist[i]->Print();
  }


  ///// Fill the SM histogram 
  histname = TString(plotname) + TString(varIndex) + "SM";
  hist[7] = new TH1D(histname, "", bins, dm_min, dm_max);
  treeSM->Draw( TString(variable.c_str())+TString(">>")+histname, "", "goff");
  GetCrossSection("SM", xsection, error);
  hist[7]->Scale(0.001*xsection/treeSM->GetEntries());
  hist[7]->Print();

} // draw_hist






TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator)
{
  TString histname = TString(hist_numerator->GetName()) + TString("_ratio");
  TH1D* hist = (TH1D*) hist_numerator->Clone(histname);
  hist->Divide( hist_denominator);
  hist->GetYaxis()->SetMoreLogLabels(true);
  hist->GetYaxis()->SetNoExponent(true);
  hist->GetYaxis()->SetTitle("Ratio w.r.t. SM");
  hist->GetYaxis()->SetRangeUser(0., 2.);
  hist->GetXaxis()->SetNdivisions(505);
  cout << "hist name " << histname << endl;
  return hist;
} // get_ratio







void  MakeComparisonPlot( std::string label, Int_t varIndex, bool doLogY ){

  string plotname;
  if(varIndex==1) plotname = histName1;
  else if(varIndex==2) plotname = histName2;
  else if(varIndex==3) plotname = histName3;
  else if(varIndex==4) plotname = histName4;
  else if(varIndex==5) plotname = histName5;
  else return;

  TString atgcLabel(label);
  atgcLabel.ReplaceAll("#lambda_Z = ", "_");
  atgcLabel.ReplaceAll(", #kappa#gamma = ", "");


  // get and draw the basic graphs for comparisons with anomalous couplings
  std::vector<TH1D*> hist(8);
  draw_hist( varIndex, false, hist);
  TH1D* ratio_hist[7];
  for(int i=0; i<7; ++i) {
    ratio_hist[i] = get_ratio( hist[7], hist[i] );
  }


  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  //gStyle->SetPadRightMargin(0.28);

  TCanvas* c1 = new TCanvas( ("can1_"+plotname).c_str(), plotname.c_str(), 500, 500);
  hist[0]->Draw("pe");
  hist[0]->Draw("same chist");
  for(int i=0; i<7; ++i) { hist[i]->Draw("pe same"); hist[i]->Draw("same chist"); }
  if(doLogY) c1->SetLogy();
  TLegend* l =  legend( label, hist );
  c1->Print(TString(plotname)+atgcLabel+TString(".png")); 





  // draw ratio between the normal and each anomalous coupling
  TCanvas* c2 = new TCanvas( ("can2_"+plotname).c_str(), plotname.c_str(), 500, 500);
  ratio_hist[1]->GetYaxis()->SetRangeUser(0.8, 1.8); 
  ratio_hist[1]->Draw("pe"); 
  ratio_hist[1]->Draw("same chist");
  for(int i=0; i<7; ++i) { ratio_hist[i]->Draw("pe same"); ratio_hist[i]->Draw("same chist"); }
  TLegend* l2 =  legend( label, hist, true );
  l2->Draw();
  c2->Print(TString(plotname)+atgcLabel+TString("_ratio.png")); 


  delete c1;
  delete c2;
} // MakeComparisonPlot





//////////////////////// Get the right cross section for each sample ////////////
void GetCrossSection(string paramName, float& xsec, float& err) {
  if(paramName.compare("SM")==0) { xsec =       856.199; err =     1.045;}

  if(paramName.compare("LambdaZ_-0.6_dKgamma_-0.6.root")==0) { xsec =      2049.823; err =     1.385;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_-0.4.root")==0) { xsec =      2001.091; err =     1.351;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_-0.2.root")==0) { xsec =      1971.554; err =     1.372;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_0.0.root")==0) { xsec =      1959.936; err =     1.355;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_0.2.root")==0) { xsec =      1964.816; err =     1.343;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_0.4.root")==0) { xsec =      1987.817; err =     1.350;}
  if(paramName.compare("LambdaZ_-0.6_dKgamma_0.6.root")==0) { xsec =      2028.575; err =     1.372;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_-0.6.root")==0) { xsec =      1417.959; err =     1.024;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_-0.4.root")==0) { xsec =      1369.149; err =     0.946;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_-0.2.root")==0) { xsec =      1340.213; err =     0.991;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_0.0.root")==0) { xsec =      1326.794; err =     0.958;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_0.2.root")==0) { xsec =      1331.933; err =     1.004;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_0.4.root")==0) { xsec =      1354.890; err =     0.968;}
  if(paramName.compare("LambdaZ_-0.4_dKgamma_0.6.root")==0) { xsec =      1394.851; err =     0.972;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_-0.6.root")==0) { xsec =      1053.221; err =     0.733;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_-0.4.root")==0) { xsec =      1004.695; err =     0.737;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_-0.2.root")==0) { xsec =       974.390; err =     0.729;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_0.0.root")==0) { xsec =       960.664; err =     0.725;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_0.2.root")==0) { xsec =       965.478; err =     0.733;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_0.4.root")==0) { xsec =       987.523; err =     0.774;}
  if(paramName.compare("LambdaZ_-0.2_dKgamma_0.6.root")==0) { xsec =      1026.728; err =     0.724;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_-0.6.root")==0) { xsec =       952.089; err =     0.800;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_-0.4.root")==0) { xsec =       901.500; err =     0.856;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_-0.2.root")==0) { xsec =       870.836; err =     1.296;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_0.0.root")==0) { xsec =       856.199; err =     1.045;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_0.2.root")==0) { xsec =       861.957; err =     1.225;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_0.4.root")==0) { xsec =       883.845; err =     0.856;}
  if(paramName.compare("LambdaZ_0.0_dKgamma_0.6.root")==0) { xsec =       923.050; err =     0.820;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_-0.6.root")==0) { xsec =      1111.790; err =     0.791;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_-0.4.root")==0) { xsec =      1063.428; err =     0.817;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_-0.2.root")==0) { xsec =      1030.739; err =     0.773;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_0.0.root")==0) { xsec =      1016.994; err =     0.757;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_0.2.root")==0) { xsec =      1020.137; err =     0.798;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_0.4.root")==0) { xsec =      1041.589; err =     0.742;}
  if(paramName.compare("LambdaZ_0.2_dKgamma_0.6.root")==0) { xsec =      1080.398; err =     0.796;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_-0.6.root")==0) { xsec =      1535.021; err =     1.040;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_-0.4.root")==0) { xsec =      1484.369; err =     1.037;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_-0.2.root")==0) { xsec =      1453.069; err =     1.009;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_0.0.root")==0) { xsec =      1439.129; err =     1.028;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_0.2.root")==0) { xsec =      1442.755; err =     1.035;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_0.4.root")==0) { xsec =      1462.876; err =     1.006;}
  if(paramName.compare("LambdaZ_0.4_dKgamma_0.6.root")==0) { xsec =      1502.613; err =     1.017;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_-0.6.root")==0) { xsec =      2224.066; err =     1.492;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_-0.4.root")==0) { xsec =      2174.382; err =     1.432;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_-0.2.root")==0) { xsec =      2141.274; err =     1.451;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_0.0.root")==0) { xsec =      2127.033; err =     1.443;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_0.2.root")==0) { xsec =      2129.372; err =     1.448;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_0.4.root")==0) { xsec =      2150.308; err =     1.436;}
  if(paramName.compare("LambdaZ_0.6_dKgamma_0.6.root")==0) { xsec =      2188.084; err =     1.431;}
 

  /*
  ///// all numbers are in the units of fb ///////////
  if(paramName.compare("SM")==0) { xsec =       693.0 ; err =      0.7 ;}
  if(paramName.compare("Delta_g1_Z-0.25d0")==0) { xsec =       697.238 ; err =      1.855 ;}
  if(paramName.compare("Delta_g1_Z-0.2d0")==0) { xsec =       695.873 ; err =      2.016 ;}
  if(paramName.compare("Delta_g1_Z-0.15d0")==0) { xsec =       692.746 ; err =      1.630 ;}
  if(paramName.compare("Delta_g1_Z-0.1d0")==0) { xsec =       692.702 ; err =      1.988 ;}
  if(paramName.compare("Delta_g1_Z-0.05d0")==0) { xsec =       693.278 ; err =      2.005 ;}
  if(paramName.compare("Delta_g1_Z0.05d0")==0) { xsec =       693.544 ; err =      1.676 ;}
  if(paramName.compare("Delta_g1_Z0.1d0")==0) { xsec =       693.238 ; err =      1.897 ;}
  if(paramName.compare("Delta_g1_Z0.15d0")==0) { xsec =       696.764 ; err =      2.096 ;}
  if(paramName.compare("Delta_g1_Z0.2d0")==0) { xsec =       697.837 ; err =      1.691 ;}
  if(paramName.compare("Delta_g1_Z0.25d0")==0) { xsec =      700.0;  err = 2.0 ;}
  if(paramName.compare("Delta_K_Z-0.25d0")==0) { xsec =       793.863 ; err =      2.141 ;}
  if(paramName.compare("Delta_K_Z-0.2d0")==0) { xsec =       756.142 ; err =      1.832 ;}
  if(paramName.compare("Delta_K_Z-0.15d0")==0) { xsec =       730.164 ; err =      1.399 ;}
  if(paramName.compare("Delta_K_Z-0.1d0")==0) { xsec =       712.810 ; err =      1.700 ;}
  if(paramName.compare("Delta_K_Z-0.05d0")==0) { xsec =       700.142 ; err =      2.044 ;}
  if(paramName.compare("Delta_K_Z0.05d0")==0) { xsec =       694.202 ; err =      2.052 ;}
  if(paramName.compare("Delta_K_Z0.1d0")==0) { xsec =       696.469 ; err =      1.700 ;}
  if(paramName.compare("Delta_K_Z0.15d0")==0) { xsec =       709.119 ; err =      1.922 ;}
  if(paramName.compare("Delta_K_Z0.2d0")==0) { xsec =       727.232 ; err =      1.833 ;}
  if(paramName.compare("Delta_K_Z0.25d0")==0) { xsec =       755.345 ; err =      1.981 ;}
  if(paramName.compare("Delta_K_gamma-0.25d0")==0) { xsec =       715.286 ; err =      1.958 ;}
  if(paramName.compare("Delta_K_gamma-0.2d0")==0) { xsec =       712.681 ; err =      1.969 ;}
  if(paramName.compare("Delta_K_gamma-0.15d0")==0) { xsec =       704.529 ; err =      2.133 ;}
  if(paramName.compare("Delta_K_gamma-0.1d0")==0) { xsec =       696.965 ; err =      1.837 ;}
  if(paramName.compare("Delta_K_gamma-0.05d0")==0) { xsec =       692.775 ; err =      1.667 ;}
  if(paramName.compare("Delta_K_gamma0.05d0")==0) { xsec =       689.879 ; err =      1.748 ;}
  if(paramName.compare("Delta_K_gamma0.1d0")==0) { xsec =       690.957 ; err =      1.896 ;}
  if(paramName.compare("Delta_K_gamma0.15d0")==0) { xsec =       692.173 ; err =      2.121 ;}
  if(paramName.compare("Delta_K_gamma0.2d0")==0) { xsec =       691.701 ; err =      1.754 ;}
  if(paramName.compare("Delta_K_gamma0.25d0")==0) { xsec =       695.953 ; err =      1.882 ;}
  if(paramName.compare("Lambda_Z-0.25d0")==0) { xsec =       788.894 ; err =      2.013 ;}
  if(paramName.compare("Lambda_Z-0.2d0")==0) { xsec =       746.479 ; err =      1.851 ;}
  if(paramName.compare("Lambda_Z-0.15d0")==0) { xsec =       718.517 ; err =      1.831 ;}
  if(paramName.compare("Lambda_Z-0.05d0")==0) { xsec =       689.782 ; err =      1.713 ;}
  if(paramName.compare("Lambda_Z0.05d0")==0) { xsec =       700.448 ; err =      1.751 ;}
  if(paramName.compare("Lambda_Z0.1d0")==0) { xsec =       720.156 ; err =      2.095 ;}
  if(paramName.compare("Lambda_Z0.15d0")==0) { xsec =       751.032 ; err =      1.977 ;}
  if(paramName.compare("Lambda_Z0.2d0")==0) { xsec =       793.941 ; err =      1.803 ;}
  if(paramName.compare("Lambda_Z0.25d0")==0) { xsec =       842.683 ; err =      1.637 ;}
  if(paramName.compare("Lambda_gamma-0.25d0")==0) { xsec =       706.943 ; err =      1.693 ;}
  if(paramName.compare("Lambda_gamma-0.2d0")==0) { xsec =       700.210 ; err =      1.923 ;}
  if(paramName.compare("Lambda_gamma-0.15d0")==0) { xsec =       696.097 ; err =      1.912 ;}
  if(paramName.compare("Lambda_gamma-0.1d0")==0) { xsec =       691.994 ; err =      2.053 ;}
  if(paramName.compare("Lambda_gamma-0.05d0")==0) { xsec =       691.558 ; err =      1.667 ;}
  if(paramName.compare("Lambda_gamma0.05d0")==0) { xsec =       692.655 ; err =      1.568 ;}
  if(paramName.compare("Lambda_gamma0.1d0")==0) { xsec =       692.777 ; err =      2.107 ;}
  if(paramName.compare("Lambda_gamma0.15d0")==0) { xsec =       703.369 ; err =      1.878 ;}
  if(paramName.compare("Lambda_gamma0.2d0")==0) { xsec =       711.104 ; err =      1.743 ;}
  if(paramName.compare("Lambda_gamma0.25d0")==0) { xsec =      720.0; err = 2.0 ;} 
  */
}
