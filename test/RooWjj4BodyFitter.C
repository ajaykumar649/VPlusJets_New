/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *
 * Implementation details:
 *  Uses RooFit classes.
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/
#ifndef __CINT__
#include "RooGlobalFunc.h"
// gROOT->ProcessLine(".L RooWjjFitterParams.h+");
// gROOT->ProcessLine(".L RooWjjFitterUtils.cc+");
// gROOT->ProcessLine(".L RooWjjMjjFitter.cc+");
#endif


#include <vector>
// ROOT
//#include <string.h>
#include "TString.h"
#include <iostream>
#include <sstream>
#include "TROOT.h"



#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCut.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TStyle.h"

#include "RooRealVar.h"
#include "RooConstVar.h"
// #include <RooAbsReal.h>
// #include <RooAbsPdf.h>
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "RooHistPdf.h"
// #include <RooKeysPdf.h>
#include "RooPlot.h"
#include "RooHist.h"
#include "RooGaussian.h"
#include "RooCurve.h"

#include "RooWjjFitterParams.h"
#include "RooWjjMjjFitter.h"



// const TString DataDirectory = "data/ReducedTree/NewReducedTree/";
const TString DataDirectory = "/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_";
const TString MCDirectory =   DataDirectory;
const TString QCDDirectory = "/uscms_data/d1/kalanand/WjjTrees/NewReducedQCDTrees/";
const TString dataFileMu = "WmunuJets_DataAll_GoldenJSON_2p1invfb.root";
const TString dataFileEl = "WenuJets_DataAll_GoldenJSON_2p1invfb.root";

const double WMassMin = 65.;
const double WMassMax = 95.;
const double SBWidth_ = 10.;
const float IntLUMI = 2100.0;
const bool truncateFitRange = false;
const bool includeNuisancePDF = true;
const bool drawSystematics = true;
const bool normalizeToAbsolute = false;

double MINRange = 150.0;
double MAXRange = 600.0;
int BINWIDTH = 25;
int NBINSFORPDF = 35.;


RooWjjFitterParams* pars4body_;
RooWjjMjjFitter* rwf_;
TCut cutsSBHi_;
TCut cutsSBLo_;
double singleTopNorm_;
double ttbarNorm_;
double zjetsNorm_;
double dibosonNorm_;
double qcdNorm_;
double wjetsNorm_;
double NMC_WpJ_;
RooRealVar *mjj_;

using namespace RooFit;
void testFitter(int channel=0, int nJet=2);
TH1F* GetWjetsHistogramShapeFromData(char* histName, 
				     float alpha, float xMin, float xMax);
RooAbsPdf* makeBkgPdf(float alpha1, float alpha2, float alpha3, 
		      float fracAlpha1, float fracAlpha2);
void InitFitterParams(RooWjjFitterParams& pars, int nJet, 
		      TString cuts, TString var, 
		      double min, double max, int Nbins);
void GetSubtractionHistogram( TH1F& hist, int UpperOrLowerSB);
void cmsPrelim();
void ActivateTreeBranches(TTree& t);




void RooWjj4BodyFitter() {
  testFitter(0, 2);
}  



/*
/////////  My conventions //////////////////////
////////////////////////////////////////////////
* channel 
0 == combined,  1 == mu,    2 == ele 

*  nJet 
0 == 2 or 3 jets,   2 == 2 jets,  3 == 3 jets

* For 2-jet sample:

massRange  |     explanation    | W+jets fraction | diboson fraction | alpha
--------------------------------------------------------------------------------------------
0        |      full range    |     1           |      1           | 0.16
|     (100--800 GeV) |                 |                  | 
1        |      150--220 GeV  |  0.398702       |  0.321052        | 0.00 + 0.07 - 0.00
2        |      220--400 GeV  |  0.750248       |  0.743344        | 0.20 +- 0.04
3        |      400--800 GeV  |  0.103053       |  0.144575        | 0.13 + 0.16 - 0.13
---------------------------------------------------------------------------------------------


The W+jets shape interpolation doesn't work well in the 
full range, hence the motivation to split into four 
sub-ranges. 
We should use the sub-ranges for the Higgs mass limit 
in the following order for 2-jet sample.

sub-range  |   limit on Higgs mass mH
---------------------------------------
1          |    180, 200 GeV
2          |    250, 300, 350 GeV
3          |    400, 450 GeV
4          |    500, 550, 600 GeV 
-----------------------------------------

For 3-jet sample, I had to combine the last two 
sub-ranges because of the limited statistics in the 
data sidebands. So the mass ranges for 3-jet sample are:


massRange  |     explanation    | W+jets fraction | diboson fraction | alpha
---------------------------------------------------------------------------------------------
0        |      full range    |     1           |      1           | 0.375
|     (100--800 GeV) |                 |                  | 
1        |      150--220 GeV  |  0.306503       |  0.251909        | 0.06 + 0.13 - 0.06
2        |      220--400 GeV  |  0.724822       |  0.698483        | 0.28 +- 0.07
3        |      400--800 GeV  |  0.15226        |  0.264282        | 0.00 + 0.22 - 0.00
----------------------------------------------------------------------------------------------

We should use the sub-ranges for the Higgs mass limit 
in the following order for 3-jet sample.

sub-range  |   limit on Higgs mass mH
---------------------------------------------
1          |    180, 200 GeV
2          |    250, 300, 350 GeV
3          |    400, 450, 500, 550, 600 GeV   
---------------------------------------------

////////////////////////////////////////////////
////////////////////////////////////////////////
*/

void testFitter(int channel, int nJet)
{
  const char* PLOTVAR = "fi2_mlvjj";
  float alpha1,  alpha2,  alpha3;
  float displayScaleFactor = 1.25;
  float legendStartX = 0.65;
  float legendStartY = 0.45;
  float chi2boxStartX = 0.66;
  float chi2boxStartY = 0.23;
  float fracAlpha1;
  float fracAlpha2;


  // ------ Normalization from the mJJ fit ---------
  if(channel==0 && nJet==2) {
    dibosonNorm_ = 967.;
    wjetsNorm_ = 20645.;
    ttbarNorm_ = 1223.;
    singleTopNorm_ = 563.;
    qcdNorm_ = 415.;
    zjetsNorm_ = 500.;
  }
  else if(channel==0 && nJet==3) {
    dibosonNorm_ = 165.;
    wjetsNorm_ = 3559.;
    ttbarNorm_ = 1605.;
    singleTopNorm_ = 281.;
    qcdNorm_ = 97.;
    zjetsNorm_ = 90.;
  }
  else if(channel==0 && nJet==0) {
    dibosonNorm_ = 1132.;
    wjetsNorm_ = 24204.;
    ttbarNorm_ = 2828.;
    singleTopNorm_ = 844.;
    qcdNorm_ = 512.;
    zjetsNorm_ = 590.;
  }
  else return;


  if(nJet==0 || nJet==2) {
    alpha1 = 0.00;    ///// syst: +0.07, -0.0
    fracAlpha1 = 0.326313;
    alpha2 = 0.20;  ///// syst: +-0.04
    fracAlpha2 = 0.589244;
    alpha3 = 0.13; //// syst: + 0.16 - 0.13
  }
  else if(nJet==3) {
    alpha1 = 0.06;  /////// syst: -0.06 + 0.13
    fracAlpha1 = 0.250198;
    alpha2 = 0.28; ////// syst: +- 0.07 
    fracAlpha2 = 0.605075;
    alpha3 = 0.22; ////// syst: +- 0.22 - 0.00 
  }
  else return;


  TString PLOTPREFIX = TString("mLnuJJ-");
  PLOTPREFIX += (int)MINRange;
  PLOTPREFIX += "-";
  PLOTPREFIX += (int)MAXRange;
  const char* XLABEL = "m_{WW}";
  NBINSFORPDF = (int) (MAXRange-MINRange)/BINWIDTH;




  std::ostringstream ostr;
  ostr << "(" << "Mass2j_PFCor>" << 
    WMassMin << " && Mass2j_PFCor<" << WMassMax << ")";
  string str = ostr.str();
  const char* mycuts = str.c_str();


  TCanvas *c;
  // TCanvas *c1;
  TCanvas *c2;
  TCanvas *c3;

  // The fit variable - lepton invariant mass
  mjj_ = new RooRealVar( PLOTVAR, XLABEL, MINRange, MAXRange, "GeV");
  RooRealVar Mass = *mjj_;

  gROOT->cd();
  char temp[50];
  TFile fin(DataDirectory + dataFileMu, "read");
  TTree* treeTemp = (TTree*) fin.Get("WJet");
  ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* treemu = treeTemp->CopyTree( mycuts );
  fin.Close();
  double nMuData = treemu->GetEntries();
  cout << "Total Mu Events = " << nMuData << endl;



  TFile fin2(DataDirectory + dataFileEl, "read");
  TTree* treeTemp2 = (TTree*) fin2.Get("WJet");
  ActivateTreeBranches(*treeTemp2);
  gROOT->cd();
  TTree* treeele = treeTemp2->CopyTree( mycuts );
  fin2.Close();
  double nEleData = treeele->GetEntries();
  cout << "Total El Events = " << nEleData << endl;


  ///////// convert Histograms into RooDataHists
  TTree* treeForDataSet = treemu;
  if(channel == 2) treeForDataSet = treeele;
  RooDataSet* data = new RooDataSet("data","data", treeForDataSet, Mass);
  RooDataSet* data_ele = new RooDataSet("data_ele","data_ele",treeele, Mass);
  if(channel == 0) data->append(*data_ele);
  cout << "Made dataset" << endl;



  RooRealVar nWjets("nWjets","nWjets",        wjetsNorm_);
  RooRealVar nDiboson("nDiboson","nDiboson",  dibosonNorm_);
  RooRealVar nTTbar("nTTbar","", ttbarNorm_);
  RooRealVar nSingleTop("nSingleTop","", singleTopNorm_);
  RooRealVar nQCD("nQCD","nQCD", qcdNorm_);
  RooRealVar nZjets("nZjets","nZjets", zjetsNorm_);
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  pars4body_= new RooWjjFitterParams();
  InitFitterParams( *pars4body_, nJet, mycuts, TString(PLOTVAR), 
		    MINRange, MAXRange, NBINSFORPDF);
  if(channel==2) pars4body_->includeMuons = false;
  if(channel==1) pars4body_->includeElectrons = false;
  rwf_ = new RooWjjMjjFitter(*pars4body_);
  rwf_->loadData();
  //////// -------------- for sidebands ---------------
  std::ostringstream ostrLowerSB;
  ostrLowerSB << "Mass2j_PFCor>" <<  
    pars4body_->minWmass - SBWidth_ << " && Mass2j_PFCor<" << pars4body_->minWmass;
  std::ostringstream ostrHigherSB;
  ostrHigherSB << "Mass2j_PFCor>" << 
    pars4body_->maxWmass << " && Mass2j_PFCor<" << pars4body_->maxWmass + SBWidth_;
  const char* masscut = ostrLowerSB.str().c_str();
  cutsSBLo_ = TCut(masscut);
  masscut = ostrHigherSB.str().c_str();
  cutsSBHi_ = TCut(masscut);
  /////------------------------------------------------


  // ********** Construct signal & bkg shape PDF ********** //
  RooAbsPdf* dibosonPdf_ = rwf_->makeDibosonPdf();
  cout << "Made signal pdf" << endl;

  RooAbsPdf* qcdPdf_;
  RooAbsPdf* ttPdf_;
  RooAbsPdf* singleTopPdf_;
  RooAbsPdf* zjetsPdf_;
  RooAbsPdf* dummyWjetsPdf_;

  if(includeNuisancePDF) { 
    qcdPdf_        = rwf_->makeQCDPdf();
    ttPdf_         = rwf_->makettbarPdf();
    singleTopPdf_  = rwf_->makeSingleTopPdf();
    zjetsPdf_      = rwf_->makeZpJPdf();
    dummyWjetsPdf_ = rwf_->makeWpJPdf();
  }

  cout << " ********************************************************" << endl;
  cout << " ********************************************************" << endl;
  cout << " norm diboson = " << rwf_->initDiboson_ << endl;
  cout << " norm wjets = " << rwf_->initWjets_ << endl;
  cout << " norm ttbar = " << rwf_->ttbarNorm_ << endl;
  cout << " norm single top = " << rwf_->singleTopNorm_ << endl;
  cout << " norm zjets = " << rwf_->zjetsNorm_ << endl;
  cout << " norm qcd = " << rwf_->QCDNorm_ << endl;
  cout << " ********************************************************" << endl;
  cout << " ********************************************************" << endl;

  rwf_->initDiboson_ = dibosonNorm_;
  rwf_->initWjets_   = wjetsNorm_;
  rwf_->ttbarNorm_   = ttbarNorm_;
  rwf_->singleTopNorm_ = singleTopNorm_;
  rwf_->zjetsNorm_   = zjetsNorm_;
  rwf_->QCDNorm_     = qcdNorm_;


  RooAbsPdf *bkgShapePdf_ = makeBkgPdf(alpha1, alpha2, 
				       alpha3, fracAlpha1, fracAlpha2);
  cout << "Made W+jets pdf from data sidebands" << endl;
  cout << "Made bkg pdf" << endl;


  // ********* Construct the total PDF ********** //  
  RooArgList* components;
  RooArgList* yields;	 
  if(includeNuisancePDF) {
    components = new RooArgList(*dibosonPdf_,
				*bkgShapePdf_, *ttPdf_, 
				*singleTopPdf_, *qcdPdf_, 
				*zjetsPdf_);
    yields = new RooArgList(nDiboson, nWjets, 
			    nTTbar, nSingleTop, nQCD, nZjets);
  }
  else {
    components = new RooArgList(*dibosonPdf_,*bkgShapePdf_);
    yields = new RooArgList(nDiboson, nWjets);
  }
  RooAddPdf totalPdf("totalPdf","extended sum pdf", *components, *yields);
  Mass.setRange("RangeForPlot", MINRange, MAXRange) ;
  //  Mass.setRange("RangeDefault", MINRange, MAXRange) ;


  // ********** Make and save Canvas for the plots ********** //
  gStyle->SetErrorX(0.5);
  gStyle->SetPadLeftMargin(0.19);
  gStyle->SetPadRightMargin(0.10);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetTitleYOffset(1.5);

  RooAbsData::ErrorType errorType = RooAbsData::SumW2;
  RooCmdArg argTotalNorm = RooCmdArg::none();
  RooCmdArg argWjjNorm = RooCmdArg::none();
  RooCmdArg argBkgNorm = RooCmdArg::none();
  RooCmdArg argTopNorm = RooCmdArg::none();
  RooCmdArg argQCDNorm = RooCmdArg::none();
  RooCmdArg argZjetsNorm = RooCmdArg::none();

  if(normalizeToAbsolute) {
    argTotalNorm = Normalization(wjetsNorm_+dibosonNorm_+ttbarNorm_+
				 singleTopNorm_+qcdNorm_+zjetsNorm_, 
				 RooAbsPdf::NumEvent);
    argWjjNorm = Normalization(wjetsNorm_, RooAbsPdf::NumEvent);
    argBkgNorm = Normalization(wjetsNorm_+ttbarNorm_+singleTopNorm_+ 
			       qcdNorm_+zjetsNorm_, RooAbsPdf::NumEvent);
    argTopNorm = Normalization(ttbarNorm_+singleTopNorm_, 
			       RooAbsPdf::NumEvent);
    argQCDNorm = Normalization(qcdNorm_, RooAbsPdf::NumEvent);
    argZjetsNorm = Normalization(zjetsNorm_, RooAbsPdf::NumEvent);
  }



  TString cname = PLOTPREFIX + TString("-combined-fit");
  if(channel==1) cname = PLOTPREFIX + TString("-mu-fit");
  if(channel==2) cname = PLOTPREFIX + TString("-ele-fit");
  if(truncateFitRange) cname = cname + TString("-truncated");
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame1 = Mass.frame( MINRange, MAXRange, 
				(int) ((MAXRange-MINRange)/BINWIDTH) );
  data->plotOn(frame1,RooFit::DataError(errorType), Name("h_data"));
  totalPdf.plotOn(frame1,ProjWData(*data),
		  Components(*dibosonPdf_),
		  DrawOption("LF"),FillStyle(1001),
		  FillColor(kOrange), LineColor(kOrange),
		  Name("h_diboson"),Range("RangeForPlot"),
		  argTotalNorm);

  totalPdf.plotOn(frame1,ProjWData(*data), 
		  Name("h_total"), Range("RangeForPlot"),argTotalNorm);
  totalPdf.plotOn(frame1,ProjWData(*data), Components(*bkgShapePdf_), 
		  LineColor(kRed), Name("h_Wjets"), Range("RangeForPlot"),argWjjNorm);
  totalPdf.plotOn(frame1,ProjWData(*data), 
		  Components("bkgShapePdf,ttPdf,singleTopPdf,qcdPdf,zjetsPdf"), 
		  Name("h_Background"), Range("RangeForPlot"),Invisible(),argBkgNorm);
  totalPdf.plotOn(frame1,ProjWData(*data), 
		  Components("signalShapePdf,bkgShapePdf,ttPdf,singleTopPdf,qcdPdf,zjetsPdf"), 
		  Name("h_SM"), Range("RangeForPlot"),Invisible(),argTotalNorm);

  if(includeNuisancePDF) {
    totalPdf.plotOn(frame1,ProjWData(*data),Components("ttPdf,singleTopPdf"),
		    LineColor(kBlack), Name("h_Top"),Range("RangeForPlot"),argTopNorm);
    totalPdf.plotOn(frame1,ProjWData(*data),Components(*qcdPdf_), 
		    LineColor(kGreen), Name("h_QCD"), Range("RangeForPlot"),argQCDNorm);
    totalPdf.plotOn(frame1,ProjWData(*data),Components(*zjetsPdf_), 
		    LineColor(kMagenta), Name("h_Zjets"), Range("RangeForPlot"),argZjetsNorm);
  }


  data->plotOn(frame1,RooFit::DataError(errorType));
  frame1->SetMinimum(0);
  frame1->SetMaximum(displayScaleFactor* frame1->GetMaximum());
  if((int)MINRange%100==0 && (int)MAXRange%100==0) 
    frame1->GetXaxis()->SetNdivisions(505);
  frame1->Draw("e0");
  TPaveText *plotlabel4 = new TPaveText(0.55,0.87,0.85,0.92,"NDC");
  plotlabel4->SetTextColor(kBlack);
  plotlabel4->SetFillColor(kWhite);
  plotlabel4->SetBorderSize(0);
  plotlabel4->SetTextAlign(12);
  plotlabel4->SetTextSize(0.03);
  plotlabel4->SetFillStyle(0);
  TPaveText *plotlabel5 = new TPaveText(0.55,0.82,0.85,0.87,"NDC");
  plotlabel5->SetTextColor(kBlack);
  plotlabel5->SetFillColor(kWhite);
  plotlabel5->SetBorderSize(0);
  plotlabel5->SetTextAlign(12);
  plotlabel5->SetTextSize(0.03);
  plotlabel5->SetFillStyle(0);
  TPaveText *plotlabel6 = new TPaveText(0.55,0.77,0.85,0.82,"NDC");
  plotlabel6->SetTextColor(kBlack);
  plotlabel6->SetFillColor(kWhite);
  plotlabel6->SetBorderSize(0);
  plotlabel6->SetTextAlign(12);
  plotlabel6->SetTextSize(0.03);
  plotlabel6->SetFillStyle(0);
  TPaveText *plotlabel7 = new TPaveText(0.55,0.72,0.85,0.77,"NDC");
  plotlabel7->SetTextColor(kBlack);
  plotlabel7->SetFillColor(kWhite);
  plotlabel7->SetBorderSize(0);
  plotlabel7->SetTextAlign(12);
  plotlabel7->SetTextSize(0.03);
  plotlabel7->SetFillStyle(0);
  TPaveText *plotlabel8 = new TPaveText(0.55,0.67,0.85,0.72,"NDC");
  plotlabel8->SetTextColor(kBlack);
  plotlabel8->SetFillColor(kWhite);
  plotlabel8->SetBorderSize(0);
  plotlabel8->SetTextAlign(12);
  plotlabel8->SetTextSize(0.03);
  plotlabel8->SetFillStyle(0);
  TPaveText *plotlabel9 = new TPaveText(0.55,0.62,0.85,0.67,"NDC");
  plotlabel9->SetTextColor(kBlack);
  plotlabel9->SetFillColor(kWhite);
  plotlabel9->SetBorderSize(0);
  plotlabel9->SetTextAlign(12);
  plotlabel9->SetTextSize(0.03);
  plotlabel9->SetFillStyle(0);
  TPaveText *plotlabel1000 = new TPaveText(chi2boxStartX,
					   chi2boxStartY, 
					   chi2boxStartX+0.14, 
					   chi2boxStartY+0.09,"NDC");
  plotlabel1000->SetTextColor(kBlack);
  plotlabel1000->SetFillColor(kWhite);
  plotlabel1000->SetBorderSize(0);
  plotlabel1000->SetTextAlign(12);
  plotlabel1000->SetTextSize(0.04);
  plotlabel1000->SetFillStyle(0);;

  sprintf(temp, "Diboson = %d", (int) nDiboson.getVal());
  plotlabel4->AddText(temp);
  sprintf(temp, "W+jets = %d", (int) nWjets.getVal());
  plotlabel5->AddText(temp);
  sprintf(temp, "t#bar{t}, Top = %d", (int) (nSingleTop.getVal()+nTTbar.getVal()));
  plotlabel6->AddText(temp);
  sprintf(temp, "QCD = %d", (int) nQCD.getVal());
  plotlabel7->AddText(temp);
  sprintf(temp, "Z+jets = %d", (int) nZjets.getVal());
  plotlabel8->AddText(temp);
  double NData_WpJ=nWjets.getVal();
  double k_WpJ=NMC_WpJ_/NData_WpJ;


  double chi2fit = frame1->chiSquare("h_total", "h_data", 0) /sqrt(1.0+1.0/k_WpJ);
  cout << "======= chi^2/dof = " << chi2fit  << endl;
  double chi2fitRaw = chi2fit * (MAXRange-MINRange)/BINWIDTH;
  cout << "======= chi^2 raw = " <<  chi2fitRaw << endl;

  //    double chi2fit = frame1->chiSquare("h_total", "h_data", 
  // 				      (int)((MAXRange-MINRange)/BINWIDTH));
  sprintf(temp, "#chi^{2}/dof = %.2f",chi2fit );
  plotlabel1000->AddText(temp);
  plotlabel4->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  plotlabel7->Draw();
  plotlabel8->Draw();
  plotlabel9->Draw();
  plotlabel1000->Draw();

  cmsPrelim();
  // TLegend* legend = new TLegend(0.35,0.35,0.55,0.55);
  TLegend* legend = new TLegend(legendStartX, legendStartY,legendStartX+0.2,legendStartY+0.25);
  // TLegend* legend = new TLegend(0.23,0.57,0.46,0.81);
  legend->SetFillStyle(0);
  RooHist* datahist = frame1->getHist("h_data");
  RooCurve* dibosonhist = frame1->getCurve("h_diboson");
  RooCurve* wjetshist = frame1->getCurve("h_Wjets");
  wjetshist->SetFillColor( wjetshist->GetLineColor() );
  RooCurve* tophist = frame1->getCurve("h_Top");
  tophist->SetFillColor( tophist->GetLineColor() );
  RooCurve* qcdhist = frame1->getCurve("h_QCD");
  qcdhist->SetFillColor( qcdhist->GetLineColor() );
  RooCurve* zjetshist = frame1->getCurve("h_Zjets");
  zjetshist->SetFillColor( zjetshist->GetLineColor() );

  legend->AddEntry( datahist, "Data", "PE");  
  legend->AddEntry( dibosonhist, "Di-boson", "F");
  legend->AddEntry( wjetshist, "W+jets", "F");
  if(includeNuisancePDF)
    legend->AddEntry( tophist, "t#bar{t}, Top", "F");
  legend->AddEntry( qcdhist, "QCD", "F");
  legend->AddEntry( zjetshist, "Z+jets", "F");
  legend->SetFillColor(0);
  legend->Draw();
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));
  c->SaveAs( cname + TString(".pdf"));

  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  //-------Make stacked plot ---------------------------------
  RooPlot* sframe = Mass.frame(MINRange, MAXRange, 
			       int((MAXRange-MINRange)/BINWIDTH));
  data->plotOn(sframe, RooFit::DataError(errorType), Name("h_data"));
  int comp(1);
  totalPdf.plotOn(sframe,ProjWData(*data), DrawOption("LF"), VLines(), FillStyle(1001),
		  FillColor(kOrange), LineColor(kOrange), Name("h_total"),
		  Range("RangeForPlot"));
  // totalPdf.plotOn(sframe,ProjWData(*data), 
  // 		   Name("h_total"), Range("RangeForPlot"));
  components->remove((*components)[0]);
  int linec = kRed;
  while (components->getSize() > 0) {
    totalPdf.plotOn(sframe, ProjWData(*data), FillColor(linec), 
		    DrawOption("LF"), VLines(), Range("RangeForPlot"),
		    Components(RooArgSet(*components)),
		    FillStyle(1001), LineColor(linec));
    // totalPdf.plotOn(sframe, ProjWData(*data), 
    // 		     Components(RooArgSet(*components)),
    // 		     LineColor(linec));
    components->remove((*components)[0]);
    switch (comp) {
    case 1: linec = kBlack; break;
    case 2: linec = kGreen; 
      components->remove((*components)[0]);
      break;
    case 3: linec = kMagenta; break;
    }
    ++comp;
  }
  data->plotOn(sframe,RooFit::DataError(errorType));
  sframe->SetMinimum(0);
  sframe->SetMaximum(displayScaleFactor* sframe->GetMaximum());
  if((int)MINRange%100==0 && (int)MAXRange%100==0) 
    sframe->GetXaxis()->SetNdivisions(505);
  TCanvas * cs = new TCanvas("cs", TString(cname) + "_Stacked", 500, 500);
  sframe->Draw("e0");
  plotlabel4->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  plotlabel7->Draw();
  plotlabel8->Draw();
  plotlabel9->Draw();
  plotlabel1000->Draw();
  cmsPrelim();
  legend->Draw();

  cs->Print(cs->GetTitle() + TString(".eps"));
  cs->Print(cs->GetTitle() + TString(".pdf"));
  cs->Print(cs->GetTitle() + TString(".root"));
  cs->Print(cs->GetTitle() + TString(".png"));


  /*
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//-------Make logY plots ---------------------------------
RooPlot* logframe = frame1->Clone("logframe");
TString cname = PLOTPREFIX + TString("-combined-fit-logY");
if(channel==1) cname = PLOTPREFIX + TString("-mu-fit-logY");
if(channel==2) cname = PLOTPREFIX + TString("-ele-fit-logY");
if(truncateFitRange) cname = cname + TString("-truncated");
c1 = new TCanvas(cname,cname,500,500);
logframe->SetMinimum(1.);
logframe->SetMaximum(10000000);
logframe->Draw("e0");
plotlabel4->Draw();
plotlabel5->Draw();
plotlabel6->Draw();
plotlabel7->Draw();
plotlabel8->Draw();
plotlabel9->Draw();
cmsPrelim();
//TLegend* legendLog = new TLegend(0.6,0.42,0.85,0.62);
TLegend* legendLog = new TLegend(0.25,0.56,0.55,0.8);
legendLog->AddEntry( datahist, "Data", "P");  
legendLog->AddEntry( dibosonhist, "Di-boson", "F");
legendLog->AddEntry( wjetshist, "W+jets", "L");
if(includeNuisancePDF) 
legendLog->AddEntry( tophist, "t#bar{t}, Top", "L");
legendLog->AddEntry( qcdhist, "QCD", "L");
legendLog->AddEntry( zjetshist, "Z+jets", "L");
legendLog->SetFillColor(0);
legendLog->Draw();
c1->SetLogy(1);
c1->SaveAs( cname + TString(".eps"));
c1->SaveAs( cname + TString(".gif"));
c1->SaveAs( cname + TString(".root"));
c1->SaveAs( cname + TString(".png"));
c1->SaveAs( cname + TString(".C"));
c1->SaveAs( cname + TString(".pdf"));
// ----------------- logY plot done -----------------------------

  */


  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  //-------- Create a new frame to draw the  data - (W+jets) residual distribution 
  //--------  and add the distribution to the frame
  RooPlot* frame2 = Mass.frame( MINRange, MAXRange, (int) ((MAXRange-MINRange)/BINWIDTH)); 
  data->plotOn(frame2,MarkerStyle(0),MarkerSize(0),MarkerColor(0),LineColor(0),LineWidth(0));
  totalPdf.plotOn(frame2,ProjWData(*data),Components(*dibosonPdf_),DrawOption("LF"),VLines(), FillStyle(1001),FillColor(kOrange),Name("h_diboson"), Range("RangeForPlot"));

  //// Construct a histogram with the residuals of the data w.r.t. the curve
  RooHist* hresid = frame1->residHist("h_data", "h_Background") ;
  hresid->SetMarkerSize(0.8);
  frame2->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
  frame2->SetMaximum(620);
  frame2->SetMinimum(-120);
  frame2->GetXaxis()->SetNdivisions(505);
  cname = PLOTPREFIX + TString("-combined-fit-subtractedWjets");
  if(channel==1) cname = PLOTPREFIX + TString("-mu-fit-subtractedWjets");
  if(channel==2) cname = PLOTPREFIX + TString("-ele-fit-subtractedWjets");
  c2 = new TCanvas(cname,cname,500,500);
  frame2->Draw() ;
  hresid->Draw("P");
  cmsPrelim();
  TLegend* legend2 = new TLegend(0.6,0.8,0.85,0.9);
  legend2->SetFillStyle(0);
  legend2->AddEntry( datahist, "Data", "P");  
  legend2->AddEntry( dibosonhist, "Di-boson", "F");
  legend2->SetFillColor(0);
  legend2->Draw();
  c2->SaveAs( cname + TString(".eps"));
  c2->SaveAs( cname + TString(".gif"));
  c2->SaveAs( cname + TString(".root"));
  c2->SaveAs( cname + TString(".png"));
  c2->SaveAs( cname + TString(".C"));
  c2->SaveAs( cname + TString(".pdf"));


  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  //-------- Create a new frame to draw the data - SM residual distribution 
  //--------  and add the distribution to the frame
  RooPlot* frame3 = Mass.frame( MINRange, MAXRange, (int) ((MAXRange-MINRange)/BINWIDTH)); 
  data->plotOn(frame3,MarkerStyle(0),MarkerSize(0),MarkerColor(0),LineColor(0),LineWidth(0));
  //// Construct a histogram with the residuals of the data w.r.t. the curve
  hresid = frame1->residHist("h_data", "h_SM") ;
  hresid->SetMarkerSize(0.8);
  frame3->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
  frame3->SetMaximum(200);
  frame3->SetMinimum(-200);
  frame3->GetXaxis()->SetNdivisions(505);
  cname = PLOTPREFIX + TString("-combined-fit-subtracted");
  if(channel==1) cname = PLOTPREFIX + TString("-mu-fit-subtracted");
  if(channel==2) cname = PLOTPREFIX + TString("-ele-fit-subtracted");
  c3 = new TCanvas(cname,cname,500,500);
  frame3->Draw() ;
  hresid->Draw("P");
  cmsPrelim();
  TLegend* legend3 = new TLegend(0.6,0.8,0.85,0.9);
  legend3->SetFillStyle(0);
  legend3->AddEntry( datahist, "Data", "P");  
  legend3->SetFillColor(0);
  legend3->Draw();
  c3->SaveAs( cname + TString(".eps"));
  c3->SaveAs( cname + TString(".gif"));
  c3->SaveAs( cname + TString(".root"));
  c3->SaveAs( cname + TString(".png"));
  c3->SaveAs( cname + TString(".C"));
  c3->SaveAs( cname + TString(".pdf"));


  //    if(data) delete data;
  //    if(c) delete c;
  delete pars4body_;
  delete rwf_;
}






/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void InitFitterParams(RooWjjFitterParams& pars, int nJet, 
		      TString cuts, TString var, 
		      double min, double max, int Nbins){

  pars.MCDirectory           = MCDirectory;
  pars.WpJDirectory          = MCDirectory;
  pars.QCDDirectory          = QCDDirectory;
  pars.DataDirectory         = DataDirectory;
  pars.NewPhysicsDirectory   = MCDirectory;
  pars.muonData              = dataFileMu;
  pars.electronData          = dataFileEl;
  pars.initParamsFile        = "HWWNominal2Jets.txt";
  if(nJet==3)  
    pars.initParamsFile      = "HWWNominal3Jets.txt";
  pars.minMass               = min;
  pars.maxMass               = max;
  pars.nbins                 = Nbins;
  pars.njets                 = nJet;
  pars.cuts                  = cuts;
  pars.var                   = var;
  pars.intLumi               = IntLUMI;
}






/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////



// ***** Function to return the background Pdf **** //
RooAbsPdf* makeBkgPdf(float alpha1, 
		      float alpha2, float alpha3, float fracAlpha1, float fracAlpha2)
{  
  char* histname = (char*)"th1wjets1";
  TH1F* th1wjets1 = GetWjetsHistogramShapeFromData(histname, alpha1, 150., 220.); 
  histname = (char*)"th1wjets2";
  TH1F* th1wjets2 = GetWjetsHistogramShapeFromData(histname, alpha2, 220., 400.); 
  histname = (char*)"th1wjets3";
  TH1F* th1wjets3 = GetWjetsHistogramShapeFromData(histname, alpha3, 400., 800.); 


  TH1F* th1wjets = new TH1F("th1wjets", "th1wjets", pars4body_->nbins, 
			    pars4body_->minMass, pars4body_->maxMass);
  th1wjets->Sumw2();
  th1wjets->Add( th1wjets1, th1wjets2, fracAlpha1, fracAlpha2 );
  th1wjets->Add( th1wjets3, 1. - fracAlpha1 - fracAlpha2 );


  //////////// Now construct the hist pdf /////////////////////////////
  NMC_WpJ_=th1wjets->GetEntries();

  RooDataHist* rdhWjets = new RooDataHist("rdhWjets","", *mjj_, th1wjets);
  RooAbsPdf* bkgShapePdf_ = new RooHistPdf("bkgShapePdf","",
					   RooArgSet(*mjj_), 
					   RooArgSet(*mjj_),*rdhWjets);
  delete th1wjets;
  delete rdhWjets;
  return bkgShapePdf_;
}






/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// ***** Function to ttbar histogram from MC **** //

TH1F* GetWjetsHistogramShapeFromData(char* histName, 
				     float alpha, float xMin, float xMax) {
  // W+jets pdf from data sidebands 
  ////////////////////////////////////////////////
  /// define the histograms 
  TFile* wjetsShape_mu_file =  new TFile(DataDirectory + dataFileMu, "READ");
  TTree* treeTemp = (TTree*) wjetsShape_mu_file->Get("WJet");
  ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* tree1 = treeTemp->CopyTree(pars4body_->cuts);

  TFile* wjetsShape_ele_file =  new TFile(DataDirectory + dataFileEl, "READ");
  treeTemp = (TTree*) wjetsShape_ele_file->Get("WJet");
  ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* tree2 = treeTemp->CopyTree(pars4body_->cuts);


  ////////////////////////////////////////////////
  /// now fill the histograms for the lower and upper sideband
  char truncSt[100];
  sprintf(truncSt, "%s > %f && %s < %f", pars4body_->var.Data(),
	  xMin, pars4body_->var.Data(), xMax);
  TCut truncation = TCut(truncSt);


  TH1F* thwjetsHi = new TH1F(histName, histName, pars4body_->nbins, 
			     pars4body_->minMass, pars4body_->maxMass);
  TH1F* thwjetsLo = (TH1F*) thwjetsHi->Clone("thwjetsLo");
  if(pars4body_->includeMuons){
    tree1->Draw(pars4body_->var +TString(">>+thwjetsHi"), cutsSBHi_ && truncation, "goff");
    tree1->Draw(pars4body_->var +TString(">>+thwjetsLo"), cutsSBLo_ && truncation, "goff");
  }
  if(pars4body_->includeElectrons) {
    tree2->Draw(pars4body_->var +TString(">>+thwjetsHi"), cutsSBHi_ && truncation, "goff");
    tree2->Draw(pars4body_->var +TString(">>+thwjetsLo"), cutsSBLo_ && truncation, "goff");
  }

  /////// Now subtract all contributions other than W+jets /////////////
  GetSubtractionHistogram( *thwjetsHi, 2);
  GetSubtractionHistogram( *thwjetsLo, 1);
  //////////// Need to scale each sideband to 1 before adding them //////////
  thwjetsHi->Scale( 1./ thwjetsHi->Integral() );
  thwjetsLo->Scale( 1./ thwjetsLo->Integral() );
  thwjetsHi->Add(thwjetsLo, thwjetsHi, alpha, 1.0-alpha);

  delete tree1;
  delete tree2;
  delete thwjetsLo;

  return thwjetsHi;
}



//////// Upper SB: UpperOrLowerSB==2, Lower SB: UpperOrLowerSB==1
void GetSubtractionHistogram( TH1F& hist, int UpperOrLowerSB) {

  /////////////////////////////////////////////////////////////////////////
  // ********** First, need 2-body shapes for normalization ********** //
  RooWjjFitterParams pars2body_(*pars4body_);
  pars2body_.var = "Mass2j_PFCor";
  pars2body_.cuts = "";
  pars2body_.minMass = 50.;
  pars2body_.minMass = 150.;
  pars2body_.nbins = 20;
  pars2body_.minWmass = WMassMin;
  pars2body_.maxWmass = WMassMax;
  RooWjjMjjFitter rwf2body_(pars2body_);
  rwf2body_.loadData();
  RooAbsPdf*  qcdPdf2body_       = rwf2body_.makeQCDPdf();
  RooAbsPdf*  ttPdf2body_        = rwf2body_.makettbarPdf();
  RooAbsPdf*  singleTopPdf2body_ = rwf2body_.makeSingleTopPdf();
  RooAbsPdf*  zjetsPdf2body_     = rwf2body_.makeZpJPdf();
  RooAbsPdf*  dibosonPdf2body_   = rwf2body_.makeDibosonPdf();
  RooAbsPdf*  dummyWjetsPdf2body_= rwf2body_.makeWpJPdf();

  ///// -------- unit normalized PDF is fine for computing ratio of integrals
  cout << " norm diboson = " << rwf2body_.initDiboson_ << endl;
  cout << " norm wjets = " << rwf2body_.initWjets_ << endl;
  cout << " norm ttbar = " << rwf2body_.ttbarNorm_ << endl;
  cout << " norm single top = " << rwf2body_.singleTopNorm_ << endl;
  cout << " norm zjets = " << rwf2body_.zjetsNorm_ << endl;
  cout << " norm qcd = " << rwf2body_.QCDNorm_ << endl;


  //// Create the numeric integrals in various Mjj ranges
  RooWjjFitterUtils util(pars2body_);
  RooRealVar*  variable = util.mjj_;
  RooRealVar Mass = *variable;
  Mass.setRange("RangeWmass", pars2body_.minWmass, pars2body_.maxWmass);
  Mass.setRange("RangeSBLo", pars2body_.minWmass-SBWidth_, pars2body_.minWmass);
  Mass.setRange("RangeSBHi", pars2body_.maxWmass, pars2body_.maxWmass+SBWidth_);
  Mass.setRange("RangeDefault", pars2body_.minMass, pars2body_.maxMass) ;

  RooAbsReal* igx_qcd_0 = qcdPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
  RooAbsReal* igx_tt_0  = ttPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
  RooAbsReal* igx_singtop_0 = singleTopPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
  RooAbsReal* igx_zjets_0 = zjetsPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
  RooAbsReal* igx_diboson_0 = dibosonPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
  ////////
  RooAbsReal* igx_qcd_L = qcdPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBLo"));
  RooAbsReal* igx_tt_L  = ttPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBLo"));
  RooAbsReal* igx_singtop_L = singleTopPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBLo"));
  RooAbsReal* igx_zjets_L = zjetsPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBLo"));
  RooAbsReal* igx_diboson_L = dibosonPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBLo"));
  ////////
  RooAbsReal* igx_qcd_H = qcdPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBHi"));
  RooAbsReal* igx_tt_H  = ttPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBHi"));
  RooAbsReal* igx_singtop_H = singleTopPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBHi"));
  RooAbsReal* igx_zjets_H = zjetsPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBHi"));
  RooAbsReal* igx_diboson_H = dibosonPdf2body_->createIntegral(Mass,NormSet(Mass),Range("RangeSBHi"));
  ////////

  double qcdNorm = 0.;
  double ttbarNorm = 0.;
  double singleTopNorm = 0.;
  double zjetsNorm = 0.;
  double dibosonNorm = 0.;

  if(UpperOrLowerSB==1) {
    qcdNorm       = qcdNorm_ * igx_qcd_L->getVal() / igx_qcd_0->getVal();
    ttbarNorm     = ttbarNorm_ * igx_tt_L->getVal() / igx_tt_0->getVal();
    singleTopNorm = singleTopNorm_ * igx_singtop_L->getVal() / igx_singtop_0->getVal();
    zjetsNorm     = zjetsNorm_ * igx_zjets_L->getVal() / igx_zjets_0->getVal();
    dibosonNorm   = dibosonNorm_ * igx_diboson_L->getVal()/igx_diboson_0->getVal();
  }
  if (UpperOrLowerSB==2) {
    qcdNorm       = qcdNorm_ * igx_qcd_H->getVal() / igx_qcd_0->getVal();
    ttbarNorm     = ttbarNorm_ * igx_tt_H->getVal() / igx_tt_0->getVal();
    singleTopNorm = singleTopNorm_ * igx_singtop_H->getVal() / igx_singtop_0->getVal();
    zjetsNorm     = zjetsNorm_ * igx_zjets_H->getVal() / igx_zjets_0->getVal();
    dibosonNorm   = dibosonNorm_ * igx_diboson_H->getVal()/igx_diboson_0->getVal();
  }

  /////////////////////////////////////////////////////////////////////////
  // ********** Construct 4-body shape histograms ********** //
  RooWjjFitterParams parsSB(*pars4body_);
  if(UpperOrLowerSB==1)
    parsSB.cuts = cutsSBLo_;
  if(UpperOrLowerSB==2)
    parsSB.cuts = cutsSBHi_;

  RooWjjMjjFitter rwfSB(parsSB);
  RooAbsPdf*  qcdPdfSB       = rwfSB.makeQCDPdf();
  RooAbsPdf*  ttPdfSB        = rwfSB.makettbarPdf();
  RooAbsPdf*  singleTopPdfSB = rwfSB.makeSingleTopPdf();
  RooAbsPdf*  zjetsPdfSB     = rwfSB.makeZpJPdf();
  RooAbsPdf*  dibosonPdfSB   = rwfSB.makeDibosonPdf();

  const RooCmdArg binning = RooFit::Binning(pars4body_->nbins, 
					    pars4body_->minMass, pars4body_->maxMass);
  TH1F* tempHist;
  //// ------- starting with QCD
  tempHist = (TH1F*) qcdPdfSB->createHistogram("tempHist", *variable, binning);
  tempHist->Scale (qcdNorm / tempHist->Integral() );
  hist.Add(tempHist, -1.);
  delete tempHist;
  //// ------- then ttbar
  tempHist = (TH1F*) ttPdfSB->createHistogram("tempHist", *variable, binning);
  tempHist->Scale (ttbarNorm / tempHist->Integral() );
  hist.Add(tempHist, -1.);
  delete tempHist;
  //// ------- then single top
  tempHist = (TH1F*) singleTopPdfSB->createHistogram("tempHist", *variable, binning);
  tempHist->Scale ( singleTopNorm / tempHist->Integral() );
  hist.Add(tempHist, -1.);
  delete tempHist;
  //// ------- then Z+jets
  tempHist = (TH1F*) zjetsPdfSB->createHistogram("tempHist", *variable, binning);
  tempHist->Scale ( zjetsNorm / tempHist->Integral() );
  hist.Add(tempHist, -1.);
  delete tempHist;
  //// ------- finally, diboson
  tempHist = (TH1F*) dibosonPdfSB->createHistogram("tempHist", *variable, binning);
  tempHist->Scale ( dibosonNorm / tempHist->Integral() );
  hist.Add(tempHist, -1.);
  delete tempHist;
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

////CMS Preliminary label and lumu -- upper left corner
void cmsPrelim()
{
  const float LUMINOSITY = IntLUMI;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(11); // align left
    latex.DrawLatex(0.21,0.85,Form("#int #font[12]{L} dt = %.1f fb^{-1}", LUMINOSITY/1000));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2011");
}




void ActivateTreeBranches(TTree& t) {
  t.SetBranchStatus("*",    0);
  t.SetBranchStatus("fit_mlvjj",    1);
  t.SetBranchStatus("fi2_mlvjj",    1);
  t.SetBranchStatus("fit_chi2",    1);
  t.SetBranchStatus("fit_NDF",    1);
  t.SetBranchStatus("fit_status",    1);
  t.SetBranchStatus("gdevtt",    1);
  t.SetBranchStatus("evtNJ",    1);
  t.SetBranchStatus("Mass2j_PFCor",    1);
  t.SetBranchStatus("MassV2j_PFCor",    1);
}

//#endif
