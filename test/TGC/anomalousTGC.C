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
const string inputDirPath = "dcap://cmsdca1.fnal.gov:24140/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lnujj/MCFM_WW_aTGC";
const string f0Name = "WW_NLO_norm";
string f1Name;
string f2Name;
string f3Name;
string f4Name;
string f5Name;
string f6Name;
string f7Name;
string f8Name;
string f9Name;
string f10Name;

const string treeName = "h300";

TFile* f0;
TFile* f1;
TFile* f2;
TFile* f3;
TFile* f4;
TFile* f5;
TFile* f6;
TFile* f7;
TFile* f8;
TFile* f9;
TFile* f10;

TTree* tree0;
TTree* tree1;
TTree* tree2;
TTree* tree3;
TTree* tree4;
TTree* tree5;
TTree* tree6;
TTree* tree7;
TTree* tree8;
TTree* tree9;
TTree* tree10;


const string var1 = "sqrt(px5*px5+py5*py5)";
const string histName1 = "Jet1pT";
const string title1 = "Jet 1 P_{T} [GeV]";

const string var2 = "sqrt(px4*px4+py4*py4)";
const string histName2 = "LeptonpT";
const string title2 = "Lepton P_{T} [GeV]";


const string var3 = "sqrt((px3+px4)*(px3+px4)+(py3+py4)*(py3+py4))";
const string histName3 = "LeptonicWpT";
const string title3 = "Lept. W P_{T} [GeV]";


const string var4 = "sqrt((px5+px6)*(px5+px6)+(py5+py6)*(py5+py6))";
const string histName4 = "HadronicWpT";
const string title4 = "Hadromic W P_{T} [GeV]";

const string var5 = "sqrt((E3+E4+E5+E6)*(E3+E4+E5+E6)-((px3+px4+px5+px6)*(px3+px4+px5+px6)+(py3+py4+py5+py6)*(py3+py4+py5+py6)+(pz3+pz4+pz5+pz6)*(pz3+pz4+pz5+pz6)))";
const string histName5 = "Mass4Body";
const string title5 = "M_{lnujj} [GeV]";



const string var6 = "sqrt( (E3+E4)**2 - (pz3+pz4)**2 - (py3+py4)**2 - (px3+px4)**2 )";
const string histName6 = "LeptonicWmass";
const string title6 = "Lept. W mass [GeV]";


const string var7 = "sqrt( (E5+E6)**2 - (pz5+pz6)**2 - (py5+py6)**2 - (px5+px6)**2 )";
const string histName7 = "HadronicWmass";
const string title7 = "Hadromic W mass [GeV]";

const string var8 = "px3+px4+px5+px6+px7";
const string histName8 = "SumPx";
const string title8 = "Sum P_{x} [GeV]";

const string var9 = "py3+py4+py5+py6+py7";
const string histName9 = "SumPy";
const string title9 = "Sum P_{y} [GeV]";




//define alternate coupling strings
const string first = "Delta_K_gamma";
const string second = "Delta_K_Z";
const string third = "Lambda_gamma";
const string fourth = "Lambda_Z";
const string fifth = "Delta_g1_Z"; 



/////////// define all member functions here first ////////////////

TH1D* aesthetics( TH1D* hist, Int_t col_num);
void title_binning( const char title[], int bins, 
		    float min, float max );
TLegend* legend( TH1D* hist0, TH1D* hist1, TH1D* hist2, TH1D* hist3, 
		  TH1D* hist4, TH1D* hist5, 
		 TH1D* hist6, TH1D*hist7, TH1D* hist8, TH1D* hist9, TH1D* hist10 );
//////void scaling( TH1D* hist, Int_t dm_max, bool doTruncation=false );
TH1D* draw_hist(string fname, Int_t varIndex, char* name, TTree* tree, 
		 Int_t color, bool doYaxisTitle=true );
TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator);
void  MakeComparisonPlot( Int_t varIndex, bool doLogY=false );
void GetCrossSection(string fName, float& xsec, float& err);




////////////// primary function //////////////////////////
void anomalousTGC(string coupling=first) {
  f1Name = coupling+"-0.25d0";
  f2Name = coupling+"-0.2d0";
  f3Name = coupling+"-0.15d0";
  f4Name = coupling+"-0.1d0";
  f5Name = coupling+"-0.05d0";
  f6Name = coupling+"0.05d0";
  f7Name = coupling+"0.1d0";
  f8Name = coupling+"0.15d0";
  f9Name = coupling+"0.2d0";
  f10Name = coupling+"0.25d0";

  // read the input samples for various values of aTGC parameters
  f0 = TFile::Open( (inputDirPath+"/Delta_K_Z0.0d0.root").c_str(), "read");
  tree0 = (TTree*)f0->Get(treeName.c_str());
  int normtree0 = tree0->GetEntries();
  f1 = TFile::Open( (inputDirPath+"/"+f1Name+".root").c_str(), "read");
  tree1 = (TTree*)f1->Get(treeName.c_str());
  int normtree1 = tree1->GetEntries();
  f2 = TFile::Open( (inputDirPath+"/"+f2Name+".root").c_str(), "read");
  tree2 = (TTree*)f2->Get(treeName.c_str());
  int normtree2 = tree2->GetEntries();
  f3 = TFile::Open( (inputDirPath+"/"+f3Name+".root").c_str(), "read");
  tree3 = (TTree*)f3->Get(treeName.c_str());
  int normtree3 = tree3->GetEntries();
  f4 = TFile::Open( (inputDirPath+"/"+f4Name+".root").c_str(), "read");
  tree4 = (TTree*)f4->Get(treeName.c_str());
  int normtree4 = tree4->GetEntries();
  f5 = TFile::Open( (inputDirPath+"/"+f5Name+".root").c_str(), "read");
  tree5 = (TTree*)f5->Get(treeName.c_str());
  int normtree5 = tree5->GetEntries();
  f6 = TFile::Open( (inputDirPath+"/"+f6Name+".root").c_str(), "read");
  tree6 = (TTree*)f6->Get(treeName.c_str());
  int normtree6 = tree6->GetEntries();
  f7 = TFile::Open( (inputDirPath+"/"+f7Name+".root").c_str(), "read");
  tree7 = (TTree*)f7->Get(treeName.c_str());
  int normtree7 = tree7->GetEntries();
  f8 = TFile::Open( (inputDirPath+"/"+f8Name+".root").c_str(), "read");
  tree8 = (TTree*)f8->Get(treeName.c_str());
  int normtree8 = tree8->GetEntries();
  f9 = TFile::Open( (inputDirPath+"/"+f9Name+".root").c_str(), "read");
  tree9 = (TTree*)f9->Get(treeName.c_str());
  int normtree9 = tree9->GetEntries();
  f10 = TFile::Open( (inputDirPath+"/"+f10Name+".root").c_str(), "read");
  tree10 = (TTree*)f10->Get(treeName.c_str());
  int normtree10 = tree10->GetEntries();

  ///////////// Print out the tree names and number of entries etc. /////////////

  cout << f0Name.c_str() << " : "  << treeName.c_str() << " : " << normtree0 << endl;
  cout << f1Name.c_str() << " : "  << treeName.c_str() << " : " << normtree1 << endl;
  cout << f2Name.c_str() << " : "  << treeName.c_str() << " : " << normtree2 << endl;
  cout << f3Name.c_str() << " : "  << treeName.c_str() << " : " << normtree3 << endl;
  cout << f4Name.c_str() << " : "  << treeName.c_str() << " : " << normtree4 << endl;
  cout << f5Name.c_str() << " : "  << treeName.c_str() << " : " << normtree5 << endl;
  cout << f6Name.c_str() << " : "  << treeName.c_str() << " : " << normtree6 << endl;
  cout << f7Name.c_str() << " : "  << treeName.c_str() << " : " << normtree7 << endl;
  cout << f8Name.c_str() << " : "  << treeName.c_str() << " : " << normtree8 << endl;
  cout << f9Name.c_str() << " : "  << treeName.c_str() << " : " << normtree9 << endl;
  cout << f10Name.c_str() << " : "  << treeName.c_str() << " : " << normtree10 << endl;
  //////////////////////////////////////////////////////////////////////////////////


  // Filling histograms
  MakeComparisonPlot(1, true);
  MakeComparisonPlot(2, true);
  MakeComparisonPlot(3, true);
  MakeComparisonPlot(4, true);
  MakeComparisonPlot(5, true);

  MakeComparisonPlot(6, false);
  MakeComparisonPlot(7, false);
  MakeComparisonPlot(8, true);
  MakeComparisonPlot(9, true);
}








TH1D* aesthetics( TH1D* hist, Int_t col_num){
  hist->SetLineColor(col_num);
  hist->SetMarkerColor(col_num);
  hist->GetYaxis()->SetTitleOffset(1.15);
  return hist;
} // aesthetics




void title_binning( const char title[], int bins, float min, float max ) {
    sprintf((char*) title, (char*) "Number of Events / %.1f GeV", ((max - min)/bins) );
  }





TLegend* legend( TH1D* hist0, TH1D* hist1, TH1D* hist2, TH1D* hist3, TH1D* hist4, TH1D* hist5, TH1D* hist6,
		 TH1D*hist7, TH1D* hist8, TH1D* hist9, TH1D* hist10) {
  Double_t x1=0.7, y1=0.59, x2=x1+0.2, y2=y1+0.3;
  TLegend *l = new TLegend( x1, y1, x2, y2 );
  l->AddEntry(hist0, "SM", "lp");
  l->AddEntry(hist1, "#Delta#kappa#gamma -0.25", "lp");
  l->AddEntry(hist2, "#Delta#kappa#gamma -0.20", "lp");
  l->AddEntry(hist3, "#Delta#kappa#gamma -0.15", "lp");
  l->AddEntry(hist4, "#Delta#kappa#gamma -0.10", "lp");
  l->AddEntry(hist5, "#Delta#kappa#gamma -0.05", "lp");
  l->AddEntry(hist6, "#Delta#kappa#gamma +0.05", "lp");
  l->AddEntry(hist7, "#Delta#kappa#gamma +0.10", "lp");
  l->AddEntry(hist8, "#Delta#kappa#gamma +0.15", "lp");
  l->AddEntry(hist9, "#Delta#kappa#gamma +0.20", "lp");
  l->AddEntry(hist10, "#Delta#kappa#gamma +0.25", "lp");
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






TH1D* draw_hist( string fname, Int_t varIndex, char* name, TTree* tree, 
		  Int_t color, bool doYaxisTitle ){

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
  else return 0;

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
    dm_min = 100.; 
    dm_max = 1000.;
  }

  //////////// now make histogram ////////////////////////
  TCut mycut("wt*(abs( 0.5 * log((E4 + pz4)/((E4 - pz4)))) < 2.5)");
  TString histname = TString(plotname.c_str()) + 
    TString(varIndex) + TString(name);
  TH1D* hist = new TH1D(histname, "", bins, dm_min, dm_max);
  aesthetics( hist, color );
  hist->Sumw2();
  tree->Draw( TString(variable.c_str())+TString(">>")+histname, mycut, "goff");

  float xsection = 0.0, error = 0.0;
  GetCrossSection(fname, xsection, error);
  hist->Scale(0.001*xsection/tree->GetEntries());


  hist->GetXaxis()->SetTitle(axis_label.c_str());
  if (doYaxisTitle){
    hist->SetNdivisions(505);
    const char title[100]="";
    title_binning(title, bins, dm_min, dm_max);
    hist->GetYaxis()->SetTitle(title);
  } // if
    cout << "varIndex " << varIndex << endl;
  cout << "bins " << bins << endl;
  cout << "dm min " << dm_min << endl;
  cout << "dm max " << dm_max << endl;
return hist;
} // draw_hist






TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator)
{
  TString histname = TString(hist_numerator->GetName()) + TString("_ratio");
  TH1D* hist = (TH1D*) hist_numerator->Clone(histname);
  hist->Divide( hist_denominator);
  hist->GetYaxis()->SetMoreLogLabels(true);
  hist->GetYaxis()->SetNoExponent(true);
  hist->GetYaxis()->SetTitle("Ratio wrt SM");
  hist->GetYaxis()->SetRangeUser(0., 2.);
  hist->GetXaxis()->SetNdivisions(505);
  cout << "hist name " << histname << endl;
  return hist;
} // get_ratio



void  MakeComparisonPlot( Int_t varIndex, bool doLogY ){

  string plotname;
  if(varIndex==1) plotname = histName1;
  else if(varIndex==2) plotname = histName2;
  else if(varIndex==3) plotname = histName3;
  else if(varIndex==4) plotname = histName4;
  else if(varIndex==5) plotname = histName5;
  else return;


  // get and draw the basic graphs for comparisons with anomalous couplings
  TH1D* hist0 = draw_hist( "SM", varIndex, (char*)"-0.00", tree0, 1, false);
  TH1D* hist1 = draw_hist( f1Name, varIndex, (char*)"-0.25", tree1, 7, false );
  TH1D* hist2 = draw_hist( f2Name, varIndex, (char*)"-0.20", tree2, 2, false );
  TH1D* hist3 = draw_hist( f3Name, varIndex, (char*)"-0.15", tree3, 2, false );
  TH1D* hist4 = draw_hist( f4Name, varIndex, (char*)"-0.10", tree4, 6, false );
  TH1D* hist5 = draw_hist( f5Name, varIndex, (char*)"-0.05", tree5, 3, false );
  TH1D* hist6 = draw_hist( f6Name, varIndex, (char*)"0.05",  tree6, 4, false );
  TH1D* hist7 = draw_hist( f7Name, varIndex, (char*)"0.10",  tree7, 9, false );
  TH1D* hist8 = draw_hist( f8Name, varIndex, (char*)"0.15",  tree8, 5, false );
  TH1D* hist9 = draw_hist( f9Name, varIndex, (char*)"0.20",  tree9, 2, false );
  TH1D* hist10 = draw_hist( f10Name, varIndex, (char*)"0.25", tree10, 6, false );



  TCanvas* c1 = new TCanvas( ("can1_"+plotname).c_str(), plotname.c_str(), 500, 500);
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.28);

  //  if(doLogY) hist1->SetMinimum(0.00001);
  hist0->Draw("c");
  hist1->Draw("csame");
  hist2->Draw("csame");
  hist3->Draw("csame");
  hist4->Draw("csame");
  hist5->Draw("csame");
  hist6->Draw("csame");
  hist7->Draw("csame");
  hist8->Draw("csame");
  hist9->Draw("csame");
  hist10->Draw("csame"); 
  if(doLogY) c1->SetLogy();
  // Draw the legend
  TLegend* l =  legend( hist0, hist1, hist2, hist3, hist4, hist5, hist6, hist7, hist8, hist9, hist10 );
  l->Draw();
  c1->Print( (plotname+".png").c_str() ); // MORE DRAW AND PRINT OUT

  // get the ratio between between the normal and each of the individual anomalous couplings
  TH1D* ratio_hist1 = get_ratio( hist0, hist1 );
  TH1D* ratio_hist2 = get_ratio( hist0, hist2 );
  TH1D* ratio_hist3 = get_ratio( hist0, hist3 );
  TH1D* ratio_hist4 = get_ratio( hist0, hist4 );
  TH1D* ratio_hist5 = get_ratio( hist0, hist5 );
  TH1D* ratio_hist6 = get_ratio( hist0, hist6 );
  TH1D* ratio_hist7 = get_ratio( hist0, hist7 );
  TH1D* ratio_hist8 = get_ratio( hist0, hist8 );
  TH1D* ratio_hist9 = get_ratio( hist0, hist9 );
  TH1D* ratio_hist10 = get_ratio( hist0, hist10 );
 

  TCanvas* c2 = new TCanvas( ("can2_"+plotname).c_str(), plotname.c_str(), 500, 500);
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.28);

  ratio_hist1->SetLineColor(2);
  ratio_hist2->SetLineColor(4);
  ratio_hist3->SetLineColor(6);
  ratio_hist4->SetLineColor(7);
  ratio_hist5->SetLineColor(8);
  ratio_hist6->SetLineColor(9);
  ratio_hist7->SetLineColor(9);
  ratio_hist8->SetLineColor(0);
  ratio_hist9->SetLineColor(5);
  ratio_hist10->SetLineColor(4);
  

  ratio_hist1->Draw("c");
  ratio_hist2->Draw("csame");
  ratio_hist3->Draw("csame");
  ratio_hist4->Draw("csame");
  ratio_hist5->Draw("csame");
  ratio_hist6->Draw("csame");
  ratio_hist7->Draw("csame");
  ratio_hist8->Draw("csame");
  ratio_hist9->Draw("csame");
  ratio_hist10->Draw("csame"); 
  // if(doLogY) c2->SetLogy();
  // Draw the legend
  TLegend* l2 = legend( hist0, hist1, hist2, hist3, hist4, hist5, hist6, hist7, hist8, hist9, hist10 );
  l2->Draw();
  c2->Print( (plotname+"_ratio.png").c_str() ); //ONCE AGAIN, DRAW AND PRINT OUT


  delete c1;
  delete c2;
} // MakeComparisonPlot





//////////////////////// Get the right cross section for each sample ////////////
void GetCrossSection(string fName, float& xsec, float& err) {

  ///// all numbers are in the units of fb ///////////
  if(fName.compare("SM")==0) { xsec =       693.0 ; err =      0.7 ;}
  if(fName.compare("Delta_g1_Z-0.25d0")==0) { xsec =       697.238 ; err =      1.855 ;}
  if(fName.compare("Delta_g1_Z-0.2d0")==0) { xsec =       695.873 ; err =      2.016 ;}
  if(fName.compare("Delta_g1_Z-0.15d0")==0) { xsec =       692.746 ; err =      1.630 ;}
  if(fName.compare("Delta_g1_Z-0.1d0")==0) { xsec =       692.702 ; err =      1.988 ;}
  if(fName.compare("Delta_g1_Z-0.05d0")==0) { xsec =       693.278 ; err =      2.005 ;}
  if(fName.compare("Delta_g1_Z0.05d0")==0) { xsec =       693.544 ; err =      1.676 ;}
  if(fName.compare("Delta_g1_Z0.1d0")==0) { xsec =       693.238 ; err =      1.897 ;}
  if(fName.compare("Delta_g1_Z0.15d0")==0) { xsec =       696.764 ; err =      2.096 ;}
  if(fName.compare("Delta_g1_Z0.2d0")==0) { xsec =       697.837 ; err =      1.691 ;}
  if(fName.compare("Delta_g1_Z0.25d0")==0) { xsec =      700.0;  err = 2.0 ;}
  if(fName.compare("Delta_K_Z-0.25d0")==0) { xsec =       793.863 ; err =      2.141 ;}
  if(fName.compare("Delta_K_Z-0.2d0")==0) { xsec =       756.142 ; err =      1.832 ;}
  if(fName.compare("Delta_K_Z-0.15d0")==0) { xsec =       730.164 ; err =      1.399 ;}
  if(fName.compare("Delta_K_Z-0.1d0")==0) { xsec =       712.810 ; err =      1.700 ;}
  if(fName.compare("Delta_K_Z-0.05d0")==0) { xsec =       700.142 ; err =      2.044 ;}
  if(fName.compare("Delta_K_Z0.05d0")==0) { xsec =       694.202 ; err =      2.052 ;}
  if(fName.compare("Delta_K_Z0.1d0")==0) { xsec =       696.469 ; err =      1.700 ;}
  if(fName.compare("Delta_K_Z0.15d0")==0) { xsec =       709.119 ; err =      1.922 ;}
  if(fName.compare("Delta_K_Z0.2d0")==0) { xsec =       727.232 ; err =      1.833 ;}
  if(fName.compare("Delta_K_Z0.25d0")==0) { xsec =       755.345 ; err =      1.981 ;}
  if(fName.compare("Delta_K_gamma-0.25d0")==0) { xsec =       715.286 ; err =      1.958 ;}
  if(fName.compare("Delta_K_gamma-0.2d0")==0) { xsec =       712.681 ; err =      1.969 ;}
  if(fName.compare("Delta_K_gamma-0.15d0")==0) { xsec =       704.529 ; err =      2.133 ;}
  if(fName.compare("Delta_K_gamma-0.1d0")==0) { xsec =       696.965 ; err =      1.837 ;}
  if(fName.compare("Delta_K_gamma-0.05d0")==0) { xsec =       692.775 ; err =      1.667 ;}
  if(fName.compare("Delta_K_gamma0.05d0")==0) { xsec =       689.879 ; err =      1.748 ;}
  if(fName.compare("Delta_K_gamma0.1d0")==0) { xsec =       690.957 ; err =      1.896 ;}
  if(fName.compare("Delta_K_gamma0.15d0")==0) { xsec =       692.173 ; err =      2.121 ;}
  if(fName.compare("Delta_K_gamma0.2d0")==0) { xsec =       691.701 ; err =      1.754 ;}
  if(fName.compare("Delta_K_gamma0.25d0")==0) { xsec =       695.953 ; err =      1.882 ;}
  if(fName.compare("Lambda_Z-0.25d0")==0) { xsec =       788.894 ; err =      2.013 ;}
  if(fName.compare("Lambda_Z-0.2d0")==0) { xsec =       746.479 ; err =      1.851 ;}
  if(fName.compare("Lambda_Z-0.15d0")==0) { xsec =       718.517 ; err =      1.831 ;}
  if(fName.compare("Lambda_Z-0.05d0")==0) { xsec =       689.782 ; err =      1.713 ;}
  if(fName.compare("Lambda_Z0.05d0")==0) { xsec =       700.448 ; err =      1.751 ;}
  if(fName.compare("Lambda_Z0.1d0")==0) { xsec =       720.156 ; err =      2.095 ;}
  if(fName.compare("Lambda_Z0.15d0")==0) { xsec =       751.032 ; err =      1.977 ;}
  if(fName.compare("Lambda_Z0.2d0")==0) { xsec =       793.941 ; err =      1.803 ;}
  if(fName.compare("Lambda_Z0.25d0")==0) { xsec =       842.683 ; err =      1.637 ;}
  if(fName.compare("Lambda_gamma-0.25d0")==0) { xsec =       706.943 ; err =      1.693 ;}
  if(fName.compare("Lambda_gamma-0.2d0")==0) { xsec =       700.210 ; err =      1.923 ;}
  if(fName.compare("Lambda_gamma-0.15d0")==0) { xsec =       696.097 ; err =      1.912 ;}
  if(fName.compare("Lambda_gamma-0.1d0")==0) { xsec =       691.994 ; err =      2.053 ;}
  if(fName.compare("Lambda_gamma-0.05d0")==0) { xsec =       691.558 ; err =      1.667 ;}
  if(fName.compare("Lambda_gamma0.05d0")==0) { xsec =       692.655 ; err =      1.568 ;}
  if(fName.compare("Lambda_gamma0.1d0")==0) { xsec =       692.777 ; err =      2.107 ;}
  if(fName.compare("Lambda_gamma0.15d0")==0) { xsec =       703.369 ; err =      1.878 ;}
  if(fName.compare("Lambda_gamma0.2d0")==0) { xsec =       711.104 ; err =      1.743 ;}
  if(fName.compare("Lambda_gamma0.25d0")==0) { xsec =      720.0; err = 2.0 ;} 
}
