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



// const TString MCDirectory =   "data/ReducedTree/NewReducedTree/";
// const TString DataDirectory = "data/ReducedTree/NewReducedTree/";

const TString MCDirectory =   "data/ReducedTree/NewKfitRDTree/";
const TString DataDirectory = "data/ReducedTree/NewKfitRDTree/";

const TString QCDDirectory = "data/ReducedTree/NewKfitRDTree/";

const TString dataFileMu = "RD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root";
const TString dataFileEl = "RD_WenuJets_DataAll_GoldenJSON_2p1invfb.root";


double MINRange = 100.0;
double MAXRange = 800.0;
int BINWIDTH = 20;



const bool includeNuisancePDF = true;
const bool drawSystematics = true;
const bool normalizeToAbsolute = false;
double singleTopNorm_;
double ttbarNorm_;
double zjetsNorm_;
double dibosonNorm_;
double NMC_WpJ_;

const double WMassMin = 65.;
const double WMassMax = 95.;


const float IntLUMI = 2100.0;
const bool truncateFitRange = false;
int NBINSFORPDF = 35.;


RooRealVar *mjj_;
RooRealVar *JES_scale;
RooAbsReal *shiftedMass;
RooRealVar *JES_scale2;
RooAbsReal *shiftedMass2;

using namespace RooFit;

void testFitter(int channel=0, int nJet=2,const char* PLOTVAR="fit_mlvjj");


TH1D* GetDibosonHistogramShape(char* histName, double& integral, int channel, 
			      const char* PLOTVAR, const char* cut);
TH1D* GetWjetsHistogramShapeFromData(char* histName, int channel, 
				     const char* PLOTVAR, float alpha, 
				     float SBWidth, const char* cut, 
				     float Mass4bodyMin, float Mass4bodyMax, int Mass4bodyBins);

TH1D* GetTTbarHistogramShape(char* histName, double& integral, int channel, 
			    const char* PLOTVAR, const char* cut);

TH1D* GetSingleTopHistogramShape(char* histName, double& integral, int channel, 
				const char* PLOTVAR, const char* cut);

TH1D* GetQCDHistogramShape(char* histName, int channel, 
			  const char* PLOTVAR, const char* cut);

TH1D* GetZjetsHistogramShape(char* histName, double& integral, int channel, 
			    const char* PLOTVAR, const char* cut);

TH1D* GetHistogramShape(char* histName, double weight, double& integral, 
		       int channel, const char* PLOTVAR, const char* cut, 
		       TString muInputFile, TString eleInputFile);


RooAbsPdf* makeBkgPdf(int channel, const char* PLOTVAR, 
		      float alpha=0.5, float SBWidth=10., const char* cut="gdevtt", 
		      float Mass4bodyMin=MINRange, float Mass4bodyMax=MAXRange, 
		      int Mass4bodyBins=NBINSFORPDF);



// RooAbsPdf* makeNewPhysicsPdf(int channel, const char * plotVar, 
// 			     const char * cut);
void cmsPrelim();
void cmsPrelim2();
void ActivateTreeBranches(TTree& t, bool isElectronTree=false);




void RooWjj4BodyFitter() {
  // RooWjj4BodyFitter(0, 2, "fit_mlvjj", 0.475);
  //  RooWjj4BodyFitter(0, 2, "fit_mlvjj", 0.39);
  // "MassV2j_PFCor"

  //   testFitter(0, 2, (char*) "fi2_mlvjj", 4);
  testFitter(0, 3, (char*) "fi2_mlvjj");

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

void testFitter(int channel, int nJet, const char* PLOTVAR)
{
   double wjetsNorm_;
   float alpha1,  alpha2,  alpha3;
   float displayScaleFactor = 1.25;
   float legendStartX = 0.65;
   float legendStartY = 0.45;
   float SBWidth = 10.;
   float chi2boxStartX = 0.66;
   float chi2boxStartY = 0.23;
   float fracAlpha1;
   float fracAlpha2;

   if(nJet==0 || nJet==2) {
     alpha1 = 0.00;    ///// syst: +0.07, -0.0
     fracAlpha1 = 0.326313;
     // displayScaleFactor = 1.8;
     // legendStartX = 0.23;
       alpha2 = 0.20;  ///// syst: +-0.04
       fracAlpha2 = 0.589244;
//        legendStartX = 0.65;
//        legendStartY = 0.4;
//        chi2boxStartX = 0.3;
//        chi2boxStartY = 0.3;
       alpha3 = 0.13; //// syst: + 0.16 - 0.13
//        displayScaleFactor = 1.8;
//        legendStartX = 0.67;
//        legendStartY = 0.41;
//        SBWidth = 10.;
//        chi2boxStartX = 0.33;
   }
   else if(nJet==3) {
       alpha1 = 0.06;  /////// syst: -0.06 + 0.13
       fracAlpha1 = 0.250198;
//        displayScaleFactor = 1.8;
//        legendStartX = 0.23;
//        chi2boxStartY = 0.32;
       alpha2 = 0.28; ////// syst: +- 0.07 
       fracAlpha2 = 0.605075;
//        displayScaleFactor = 2.0;
//        legendStartX = 0.65;
//        legendStartY = 0.4;
//        chi2boxStartX = 0.3;
//        chi2boxStartY = 0.32;
       alpha3 = 0.22; ////// syst: +- 0.22 - 0.00 
//        displayScaleFactor = 1.8;
//        legendStartX = 0.67;
//        legendStartY = 0.41;
//        SBWidth = 20.;
//        chi2boxStartX = 0.33;
//        chi2boxStartY = 0.5;
   }
   else return;


   TString PLOTPREFIX = TString("mLnuJJ-");
   PLOTPREFIX += (int)MINRange;
   PLOTPREFIX += "-";
   PLOTPREFIX += (int)MAXRange;
  const char* XLABEL = "m_{WW}";
  NBINSFORPDF = (int) (MAXRange-MINRange)/BINWIDTH;



//   char* njetCut = (char*) "(evtNJ==2)";
//   if(nJet==3) njetCut = (char*) "(evtNJ==3)";
//   if(njetCut==0) njetCut = (char*) "(evtNJ==2||evtNJ==3)";


//   std::ostringstream ostr;
//   ostr << "(" << njetCut << " && fit_status==0 && Mass2j_PFCor>" << 
//     WMassMin << " && Mass2j_PFCor<" << WMassMax << ")";
//   string str = ostr.str();
//   const char* mycuts = str.c_str();


//   std::ostringstream ostr2;
//   ostr2 << "(" << njetCut << " && fit_status==0" <<  ")";
//   string str2 = ostr2.str();
//   const char* mycutsNoMassCut = str2.c_str();



  char* njetCut = (char*) "(evtNJ==2)";
  if(nJet==3) njetCut = (char*) "(evtNJ==3)";
  if(njetCut==0) njetCut = (char*) "(evtNJ==2||evtNJ==3)";


  std::ostringstream ostr;
  ostr << "(" << njetCut << " && Mass2j_PFCor>" << 
    WMassMin << " && Mass2j_PFCor<" << WMassMax << ")";
  string str = ostr.str();
  const char* mycuts = str.c_str();


  std::ostringstream ostr2;
  ostr2 << "(" << njetCut <<  ")";
  string str2 = ostr2.str();
  const char* mycutsNoMassCut = str2.c_str();



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
   ActivateTreeBranches(*treeTemp2, true);
   gROOT->cd();
   TTree* treeele = treeTemp2->CopyTree( mycuts );
   fin2.Close();
   double nEleData = treeele->GetEntries();
   cout << "Total El Events = " << nEleData << endl;



   // ------ Normalization of QCD events: 0.8% in Mu data and 3% in Ele data
   double QCDNorm=0.0;
   if(channel==1) QCDNorm = 0.008*nMuData; 
   if(channel==2) QCDNorm = 0.03*nEleData; 
   if(channel==0) QCDNorm = 0.008*nMuData + 0.03*nEleData; 


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
   RooRealVar nQCD("nQCD","nQCD", QCDNorm);
   RooRealVar nZjets("nZjets","nZjets", zjetsNorm_);
   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////
   RooWjjFitterParams fitterPars;
   fitterPars.MCDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_';
   fitterPars.WpJDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_';
   fitterPars.QCDDirectory = '/uscms_data/d1/kalanand/WjjTrees/NewReducedQCDTrees/';
   fitterPars.initParamsFile = "HWWConstraints2Jets.txt";
   fitterPars.DataDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_';
   fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_';
   fitterPars.minMass = 150;
   fitterPars.maxMass = 800.;
   fitterPars.nbins = 65;
   fitterPars.truncRange = false;
   fitterPars.njets = nJet;
   fitterPars.cuts = mycuts;
   fitterPars.var = TString(PLOTVAR);

   RooWjjMjjFitter* rwf = new RooWjjMjjFitter(fitterPars);


   // ********** Construct signal & bkg shape PDF ********** //
   RooAbsPdf* signalShapePdf_ = rwf->makeDibosonPdf();
   cout << "Made signal pdf" << endl;

   RooAbsPdf *bkgShapePdf1_ = makeBkgPdf(channel, PLOTVAR, alpha1, SBWidth, mycutsNoMassCut, 
					150., 220., 7);
   RooAbsPdf *bkgShapePdf2_ = makeBkgPdf(channel, PLOTVAR, alpha2, SBWidth, mycutsNoMassCut, 
					220., 400., 9);
   RooAbsPdf *bkgShapePdf3_ = makeBkgPdf(channel, PLOTVAR, alpha3, SBWidth, mycutsNoMassCut, 
					400., 800., 10);
   RooRealVar fAlpha1("fAlpha1","fAlpha1",  fracAlpha1);
   RooRealVar fAlpha2("fAlpha2","fAlpha2",  fracAlpha2);

   RooAbsPdf *bkgShapePdf_ = new RooAddPdf("bkgShapePdf","", 
					   RooArgList(*bkgShapePdf1_,*bkgShapePdf2_,*bkgShapePdf3_), 
					   RooArgList(fAlpha1,fAlpha2));

   cout << "Made W+jets pdf from data sidebands" << endl;

   RooAbsPdf* qcdPdf_;
   RooAbsPdf* ttPdf_;
   RooAbsPdf* singleTopPdf_;
   RooAbsPdf* zjetsPdf_;

   if(includeNuisancePDF) { 
     qcdPdf_       = rwf->makeQCDPdf();
     ttPdf_        = rwf->makettbarPdf();
     singleTopPdf_ = rwf->makeSingleTopPdf();
     zjetsPdf_      = rwf->makeZpJPdf();
   }
   cout << "Made bkg pdf" << endl;


   // ********* Do the Actual Fit ********** //  

   if(channel==0 && nJet==2) {
     dibosonNorm_ = 967.;
     wjetsNorm_ = 20645.;
     ttbarNorm_ = 1223.;
     singleTopNorm_ = 563.;
     QCDNorm = 415.;
     zjetsNorm_ = 500.;
   }
   else if(channel==0 && nJet==3) {
     dibosonNorm_ = 165.;
     wjetsNorm_ = 3559.;
     ttbarNorm_ = 1605.;
     singleTopNorm_ = 281.;
     QCDNorm = 97.;
     zjetsNorm_ = 90.;
   }
   else if(channel==0 && nJet==0) {
     dibosonNorm_ = 1132.;
     wjetsNorm_ = 24204.;
     ttbarNorm_ = 2828.;
     singleTopNorm_ = 844.;
     QCDNorm = 512.;
     zjetsNorm_ = 590.;
   }
   else return;



   RooArgList* components;
   RooArgList* yields;	 
   if(includeNuisancePDF) {
     components = new RooArgList(*signalShapePdf_,
				 *bkgShapePdf_, *ttPdf_, 
				 *singleTopPdf_, *qcdPdf_, 
				 *zjetsPdf_);
     yields = new RooArgList(nDiboson, nWjets, 
			     nTTbar, nSingleTop, nQCD, nZjets);
   }
   else {
     components = new RooArgList(*signalShapePdf_,*bkgShapePdf_);
     yields = new RooArgList(nDiboson, nWjets);
   }
   RooAddPdf totalPdf("totalPdf","extended sum pdf", *components, *yields);
   Mass.setRange("RangeForPlot", MINRange, MAXRange) ;
   //  Mass.setRange("RangeDefault", MINRange, MAXRange) ;
   //JES_scale->setVal(0.0);
   JES_scale2->setVal(0.0);


   // ********** Make and save Canvas for the plots ********** //
//    gROOT->ProcessLine(".L ~kalanand/tdrstyle.C");
//   setTDRStyle();
//   tdrStyle->SetErrorX(0.5);
//   tdrStyle->SetPadLeftMargin(0.19);
//   tdrStyle->SetPadRightMargin(0.10);
//   tdrStyle->SetPadBottomMargin(0.15);
//   tdrStyle->SetLegendBorderSize(0);
//   tdrStyle->SetTitleYOffset(1.5);

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
				 singleTopNorm_+QCDNorm+zjetsNorm_, 
				 RooAbsPdf::NumEvent);
    argWjjNorm = Normalization(wjetsNorm_, RooAbsPdf::NumEvent);
    argBkgNorm = Normalization(wjetsNorm_+ttbarNorm_+singleTopNorm_+ 
			       QCDNorm+zjetsNorm_, RooAbsPdf::NumEvent);
    argTopNorm = Normalization(ttbarNorm_+singleTopNorm_, 
			       RooAbsPdf::NumEvent);
    argQCDNorm = Normalization(QCDNorm, RooAbsPdf::NumEvent);
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
		   Components(*signalShapePdf_),
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

   cmsPrelim2();
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
   cmsPrelim2();
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
   cmsPrelim2();
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
   totalPdf.plotOn(frame2,ProjWData(*data),Components(*signalShapePdf_),DrawOption("LF"),VLines(), FillStyle(1001),FillColor(kOrange),Name("h_diboson"), Range("RangeForPlot"));

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
   cmsPrelim2();
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
   cmsPrelim2();
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
}






/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////



// ***** Function to return the background Pdf **** //
RooAbsPdf* makeBkgPdf(int channel, const char* PLOTVAR, 
		      float alpha, float SBWidth, const char* cut, 
		      float Mass4bodyMin, float Mass4bodyMax, int Mass4bodyBins)
{  

 TH1D* th1wjets = GetWjetsHistogramShapeFromData((char*)"th1wjets", channel, 
						 PLOTVAR, alpha, SBWidth, cut, 
						 Mass4bodyMin, Mass4bodyMax, Mass4bodyBins); 

  //////////// if using one alpha /////////////////////////////
  NMC_WpJ_=th1wjets->GetEntries();

  RooDataHist* rdhWjets = new RooDataHist("rdhWjets","", *mjj_, th1wjets);
  RooAbsPdf* bkgShapePdf_ = new RooHistPdf("bkgShapePdf","",
					   RooArgSet(*shiftedMass), 
					   RooArgSet(*mjj_),*rdhWjets);

  return bkgShapePdf_;
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// ***** Function to diboson histogram from MC **** //
TH1D* GetDibosonHistogramShape(char* histName, double& integral, int channel, 
		       const char* PLOTVAR, const char* cut) {


  // First get histogram shape for WW
  double weight = 0.0000101753087377979123;
  TString muInputFile = "RD_mu_WW_CMSSW428.root";
  TString eleInputFile = "RD_el_WW_CMSSW428.root";
  double wwNorm = 0.0;
  TH1D* th1ww = GetHistogramShape( histName, weight, wwNorm, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);

  // Then get histogram shape for WZ
  weight = 0.00000422015814808206740;
  muInputFile = "RD_mu_WZ_CMSSW428.root";
  eleInputFile = "RD_el_WZ_CMSSW428.root";
  double wzNorm = 0.0;
  TH1D* th1wz = GetHistogramShape( (char*)"th1wz", weight, wzNorm, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);


  // Add them together, weighted by the relative cross section
  th1ww->Add( th1wz );
  integral = wwNorm + wzNorm;

  return th1ww;
}




// ***** Function to ttbar histogram from MC **** //
TH1D* GetWjetsHistogramShapeFromData(char* histName, int channel, 
				     const char* PLOTVAR, float alpha, 
				     float SBWidth, const char* cut, 
				     float Mass4bodyMin, float Mass4bodyMax, int Mass4bodyBins) {

  // W+jets pdf from data sidebands 
   std::ostringstream ostrLowerSB;
   ostrLowerSB << "Mass2j_PFCor>" << WMassMin - SBWidth << " && Mass2j_PFCor<" << WMassMin;
   std::ostringstream ostrHigherSB;
   ostrHigherSB << "Mass2j_PFCor>" << WMassMax << " && Mass2j_PFCor<" << WMassMax + SBWidth;
   const char* masscut = ostrLowerSB.str().c_str();
   TCut sidebandLo(masscut);
   masscut = ostrHigherSB.str().c_str();
   TCut sidebandHi(masscut);


  TFile* wjetsShape_mu_file =  new TFile(DataDirectory + dataFileMu, "READ");
  TTree* treeTemp = (TTree*) wjetsShape_mu_file->Get("WJet");
  ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* tree1 = treeTemp->CopyTree(cut);

  TFile* wjetsShape_ele_file =  new TFile(DataDirectory + dataFileEl, "READ");
  treeTemp = (TTree*) wjetsShape_ele_file->Get("WJet");
  ActivateTreeBranches(*treeTemp, true);
  gROOT->cd();
  TTree* tree2 = treeTemp->CopyTree(cut);


  TH1D* thwjetsHi = new TH1D(histName, histName,
			   Mass4bodyBins, Mass4bodyMin, Mass4bodyMax);
  TH1D* thwjetsLo = (TH1D*) thwjetsHi->Clone("thwjetsLo");


  double integral=0.0;
  TCut cutsSBHi = TCut(cut) && sidebandHi;
  TCut cutsSBLo = TCut(cut) && sidebandLo;


  TH1D* th1dibosonHi = GetDibosonHistogramShape((char*)"th1dibosonHi", integral, 
						channel, PLOTVAR, cutsSBHi.GetTitle());
  TH1D* th1dttbarHi = GetTTbarHistogramShape((char*)"th1dttbarHi", integral, 
					     channel, PLOTVAR, cutsSBHi.GetTitle());
  TH1D* th1dsingletopHi = GetSingleTopHistogramShape((char*)"th1dsingletopHi", integral, 
						     channel, PLOTVAR, cutsSBHi.GetTitle());
  TH1D* th1dqcdHi = GetQCDHistogramShape((char*)"th1dqcdHi", channel, 
					 PLOTVAR, cutsSBHi.GetTitle());
  TH1D* th1zjetsHi = GetZjetsHistogramShape((char*)"th1zjetsHi", integral, channel, 
					    PLOTVAR, cutsSBHi.GetTitle());


  TH1D* th1dibosonLo = GetDibosonHistogramShape((char*)"th1dibosonLo", integral, 
						channel, PLOTVAR, cutsSBLo.GetTitle());
  TH1D* th1dttbarLo = GetTTbarHistogramShape((char*)"th1dttbarLo", integral, 
					     channel, PLOTVAR, cutsSBLo.GetTitle());
  TH1D* th1dsingletopLo = GetSingleTopHistogramShape((char*)"th1dsingletopLo", integral, 
						     channel, PLOTVAR, cutsSBLo.GetTitle());
  TH1D* th1dqcdLo = GetQCDHistogramShape((char*)"th1dqcdLo", channel, 
					 PLOTVAR, cutsSBLo.GetTitle());
  TH1D* th1zjetsLo = GetZjetsHistogramShape((char*)"th1zjetsLo", integral, channel, 
					    PLOTVAR, cutsSBLo.GetTitle());


  if(channel==0 || channel==1) {
    tree1->Draw(TString(PLOTVAR)+TString(">>thwjetsHi"), sidebandHi, "goff");
    tree1->Draw(TString(PLOTVAR)+TString(">>thwjetsLo"), sidebandLo, "goff");
  }
  if(channel==0) {
    tree2->Draw(TString(PLOTVAR)+TString(">>+thwjetsHi"), sidebandHi, "goff");
    tree2->Draw(TString(PLOTVAR)+TString(">>+thwjetsLo"), sidebandLo, "goff");
  }
  if(channel==2) {
    tree2->Draw(TString(PLOTVAR)+TString(">>thwjetsHi"), sidebandHi, "goff");
    tree2->Draw(TString(PLOTVAR)+TString(">>+thwjetsLo"), sidebandLo, "goff");
  }


  /////// normalize the QCD properly ///////////  
  if(channel==0) {
    th1dqcdHi->Scale( thwjetsHi->Integral() *  0.0168);
    th1dqcdLo->Scale( thwjetsLo->Integral() *  0.0168);
  }
  else if(channel==1) {
    th1dqcdHi->Scale( thwjetsHi->Integral() *  0.008);
    th1dqcdLo->Scale( thwjetsLo->Integral() *  0.008);
  }
  else {
    th1dqcdHi->Scale( thwjetsHi->Integral() *  0.03);
    th1dqcdLo->Scale( thwjetsLo->Integral() *  0.03);
  }



  /////// Now subtract all contributions other than W+jets /////////////
  thwjetsHi->Add( th1dibosonHi, -1.);
  thwjetsHi->Add( th1dttbarHi, -1.);
  thwjetsHi->Add( th1dsingletopHi, -1.);
  thwjetsHi->Add( th1dqcdHi, -1.);
  thwjetsHi->Add( th1zjetsHi, -1.);


  thwjetsLo->Add( th1dibosonLo, -1.);
  thwjetsLo->Add( th1dttbarLo, -1.);
  thwjetsLo->Add( th1dsingletopLo, -1.);
  thwjetsLo->Add( th1dqcdLo, -1.);
  thwjetsLo->Add( th1zjetsLo, -1.);


  //////////// Need to scale each sideband to 1 before addiing them //////////
  thwjetsHi->Scale( 1./ thwjetsHi->Integral() );
  thwjetsLo->Scale( 1./ thwjetsLo->Integral() );
  thwjetsHi->Add(thwjetsLo, thwjetsHi, alpha, 1.0-alpha);

  delete tree1;
  delete tree2;
  delete thwjetsLo;

  delete th1dibosonHi;
  delete th1dttbarHi;
  delete th1dsingletopHi;
  delete th1dqcdHi;
  delete th1dibosonLo;
  delete th1dttbarLo;
  delete th1dsingletopLo;
  delete th1dqcdLo;
  delete th1zjetsLo;

  return thwjetsHi;
}





// ***** Function to ttbar histogram from MC **** //
TH1D* GetTTbarHistogramShape(char* histName, double& integral, int channel, 
		       const char* PLOTVAR, const char* cut) {

  // --------- cross section: 157.5 pb, events_gen = 3701947 (These are summer11 TTJets sample
  // --------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTop2011DataMCTrig

  double weight = 0.00004254518;
  TString muInputFile = "RD_mu_TTbar_MG_CMSSW428.root";
  TString eleInputFile = "RD_el_TTbar_MG_CMSSW428.root";

  TH1D* th1Top = GetHistogramShape( histName, weight,  integral,
				   channel, PLOTVAR, cut, muInputFile, eleInputFile);
  return th1Top;
}




// ***** Function to return single top histogram from MC **** //
TH1D* GetSingleTopHistogramShape(char* histName, double& integral, int channel, 
		       const char* PLOTVAR, const char* cut) {


  // single top pdf (all samples are powheg, regardless of the unusual labeling scheme)
  // Get histogram shape for s-channel Tbar
  // --------- cross section: Tbar - 1.44 pb, events_gen = 137980; T - 3.19 pb, events_gen = 259971
  // --------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma 
  double weight = 0.000010436295;
  TString muInputFile = "RD_mu_STopS_Tbar_CMSSW428.root";
  TString eleInputFile = "RD_el_STopS_Tbar_CMSSW428.root";
  double sNormTbar = 0.0;
  TH1* th1sTbar = GetHistogramShape( (char*)"th1sTbar", weight, sNormTbar, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);
  // Get histogram shape for s-channel T
  weight = 0.0000122706;
  muInputFile = "RD_mu_STopS_T_CMSSW428.root";
  eleInputFile = "RD_el_STopS_T_CMSSW428.root";
  double sNormT = 0.0;
  TH1* th1sT = GetHistogramShape( (char*)"th1sT", weight, sNormT, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);

  // Get histogram shape for t-channel Tbar
  weight = 0.000011646286;
  muInputFile = "RD_mu_STopT_Tbar_CMSSW428.root";
  eleInputFile = "RD_el_STopT_Tbar_CMSSW428.root";
  double tNormTbar = 0.0;
  TH1* th1tTbar = GetHistogramShape( (char*)"th1tTbar", weight, tNormTbar, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);
  // Get histogram shape for t-channel T
  weight = 0.000010748247;
  muInputFile = "RD_mu_STopT_T_CMSSW428.root";
  eleInputFile = "RD_el_STopT_T_CMSSW428.root";
  double tNormT = 0.0;
  TH1* th1tT = GetHistogramShape( (char*)"th1tT", weight, tNormT, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);

  // Get histogram shape for tW-channel Tbar
  weight = 0.000009992014;
  muInputFile = "RD_mu_STopTW_Tbar_CMSSW428.root";
  eleInputFile = "RD_el_STopTW_Tbar_CMSSW428.root";
  double twNormTbar = 0.0;
  TH1* th1twTbar = GetHistogramShape( (char*)"th1twTbar", weight, twNormTbar, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);
  // Get histogram shape for t-channel T
  weight = 0.000009894654;
  muInputFile = "RD_mu_STopTW_T_CMSSW428.root";
  eleInputFile = "RD_el_STopTW_T_CMSSW428.root";
  double twNormT = 0.0;
  TH1* th1twT = GetHistogramShape( (char*)"th1twT", weight, twNormT, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);


  /////////////// Add all these histograms now ////////////////////
  TH1D* th1SingleTop = (TH1D*) th1sTbar->Clone(histName);
  th1SingleTop->Add(th1sT);
  th1SingleTop->Add(th1tTbar);
  th1SingleTop->Add(th1tT);
  th1SingleTop->Add(th1twTbar);
  th1SingleTop->Add(th1twT);

  integral = sNormTbar + sNormT + tNormTbar + tNormT + twNormTbar + twNormT;

  return th1SingleTop;
}


// ***** Function to ttbar histogram from MC **** //
TH1D* GetQCDHistogramShape(char* histName, int channel, 
		       const char* PLOTVAR, const char* cut) {
  TFile* fqcd0 =  new TFile(QCDDirectory + "/RD_mu_QCDMu_CMSSW428.root", "READ");
  TTree* tree10 = (TTree*) fqcd0->Get("WJet");
  TFile* fqcd1 =  new TFile(QCDDirectory + "/RD_el_QCDEl_Pt30to80_CMSSW428.root", "READ");
  TTree* tree11 = (TTree*) fqcd1->Get("WJet");
  TFile* fqcd2 =  new TFile(QCDDirectory + "/RD_el_QCDEl_Pt80to170_CMSSW428.root", "READ");
  TTree* tree12 = (TTree*) fqcd2->Get("WJet");
  TFile* fqcd3 =  new TFile(QCDDirectory + "/RD_el_QCDEl_BCtoE30to80_CMSSW428.root", "READ");
  TTree* tree13 = (TTree*) fqcd3->Get("WJet");
  TFile* fqcd4 =  new TFile(QCDDirectory + "/RD_el_QCDEl_BCtoE80to170_CMSSW428.root", "READ");
  TTree* tree14 = (TTree*) fqcd4->Get("WJet");

  //// Scaling Coefficients = 84679.3/25080241, 3866200/70392060, 139500/2194800, 136804/2030033, 9360/1082691 = 3.37633517955429532e-03, 5.49238081681371476e-02, 6.35593220338983023e-02, 6.73900375018534198e-02, 8.64512589464584008e-03 

  TH1* th1qcdMu = new TH1D("th1qcdMu", "th1qcdMu", NBINSFORPDF, MINRange, MAXRange);
  TString myselection = TString("0.00337633517955429532*") + TString(cut);
  tree10->Draw( TString(PLOTVAR)+TString(">>th1qcdMu"), myselection, "goff");

  TH1* th1qcdEle = (TH1*) th1qcdMu->Clone("th1qcdEle");
  //sprintf(scale, "%f", 5.49238081681371476e-02);
  myselection = TString("0.0549238081681371476*") + TString(cut);
  tree11->Draw( TString(PLOTVAR)+TString(">>th1qcdEle"), myselection, "goff");
  //sprintf(scale, "%f", 6.35593220338983023e-02);
  myselection = TString("0.0635593220338983023*") + TString(cut);
  tree12->Draw( TString(PLOTVAR)+TString(">>+th1qcdEle"), myselection, "goff");
  //sprintf(scale, "%f", 6.73900375018534198e-02);
  myselection = TString("0.0673900375018534198*") + TString(cut);
  tree13->Draw( TString(PLOTVAR)+TString(">>+th1qcdEle"), myselection, "goff");
  //sprintf(scale, "%f", 8.64512589464584008e-03);
  myselection = TString("0.00864512589464584008*") + TString(cut);
  tree14->Draw( TString(PLOTVAR)+TString(">>+th1qcdEle"), myselection, "goff");



  TH1D* th1qcd; 
  if(channel==0 || channel==1) 
    th1qcd = (TH1D* ) th1qcdMu->Clone(histName);
  else th1qcd = (TH1D* ) th1qcdEle->Clone(histName);
  if(channel==0) th1qcd->Add(th1qcdEle);

  th1qcd->Scale ( 1. / th1qcd->Integral() );

  //     cout << " ----- num. all QCD events = " << th1qcd->Integral() << endl;
  //     cout << " ----- num. mu QCD events = " << th1qcdMu->Integral() << endl;
  //     cout << " ----- num. ele QCD events = " << th1qcdEle->Integral() << endl;

  delete tree10;
  delete tree11;
  delete tree12;
  delete tree13;
  delete tree14;
  delete fqcd0;
  delete fqcd1;
  delete fqcd2;
  delete fqcd3;

  return th1qcd;
}



// ***** Function to get Z+jets histogram from MC **** //
TH1D* GetZjetsHistogramShape(char* histName, double& integral, int channel, 
		       const char* PLOTVAR, const char* cut) {

  // ----------LO cross section: 5635.6 * 0.4381 pb, events_gen = 36277961 => weight = 0.000068056646
  // ---------- https://twiki.cern.ch/twiki/bin/view/CMS/MadGraphSummer11Production
  // ----------NNLO cross section: 3048 pb , events_gen = 36277961 => weight = 0.000084017952387
  // ---------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTop2011DataMCTrig 
  // ----------- https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSections

  double weight = 0.000084017952387;
  TString muInputFile = "RD_mu_ZpJ_CMSSW428.root";
  TString eleInputFile = "RD_el_ZpJ_CMSSW428.root";


  TH1D* th1ZJets = GetHistogramShape( histName, weight, integral, 
				     channel, PLOTVAR, cut, muInputFile, eleInputFile);
  return th1ZJets;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////




// ***** Function to return MC histogram normalized to 1 **** //
TH1D* GetHistogramShape(char* histName, double weight, double& integral, 
		       int channel, const char* PLOTVAR, const char* cut, 
		       TString muInputFile, TString eleInputFile) {

  TFile* mu_file =  new TFile(MCDirectory + muInputFile, "READ");
  TTree* tree1 = (TTree*) mu_file->Get("WJet");
  ActivateTreeBranches(*tree1);

  TFile* ele_file =  new TFile(MCDirectory + eleInputFile, "READ");
  TTree* tree2 = (TTree*) ele_file->Get("WJet");
  ActivateTreeBranches(*tree2, true);


  TH1D* th1 = new TH1D( histName, histName, NBINSFORPDF, MINRange, MAXRange);
  std::ostringstream sstream;
  sstream << weight;
  std::string weightAsString = sstream.str();

  TString myselection = TString(weightAsString) + TString("*") + TString(cut);

  if(channel==0 || channel==1) 
    tree1->Draw(TString(PLOTVAR)+TString(">>")+TString(histName), myselection, "goff");
  if(channel==0)
    tree2->Draw(TString(PLOTVAR)+TString(">>+")+TString(histName), myselection, "goff");
  if(channel==2) 
    tree2->Draw(TString(PLOTVAR)+TString(">>")+TString(histName), myselection, "goff");


  integral = th1->Integral() * IntLUMI;
  // th1->Scale( 1./ th1->Integral() );


  delete tree1;
  delete tree2;

  return th1;
}



////CMS Preliminary label and lumu
void cmsPrelim()
{
  const float LUMINOSITY = IntLUMI;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(31); // align right
    latex.DrawLatex(0.82,0.7,Form("#int #font[12]{L} dt = %d pb^{-1}", (int) LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2011");
}




////CMS Preliminary label and lumu -- upper left corner
void cmsPrelim2()
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




void ActivateTreeBranches(TTree& t, bool isElectronTree) {
  t.SetBranchStatus("*",    0);
  t.SetBranchStatus("JetPFCor_Pt",    1);
  t.SetBranchStatus("JetPFCor_Px",    1);
  t.SetBranchStatus("JetPFCor_Py",    1);
  t.SetBranchStatus("JetPFCor_Pz",    1);
  t.SetBranchStatus("JetPFCor_Eta",    1);
  t.SetBranchStatus("JetPFCor_Phi",    1);
  t.SetBranchStatus("JetPFCor_etaetaMoment",    1);
  t.SetBranchStatus("JetPFCor_phiphiMoment",    1);
  t.SetBranchStatus("JetPFCor_ChargedHadronMultiplicity",    1);
  t.SetBranchStatus("JetPFCor_ChargedHadronEnergyFrac",    1);
  t.SetBranchStatus("JetPFCor_NeutralHadronMultiplicity",    1);
  t.SetBranchStatus("JetPFCor_bDiscriminator",    1);
  t.SetBranchStatus("JetPFCor_QGLikelihood",    1);

  t.SetBranchStatus("event_met_pfmet",    1);
  t.SetBranchStatus("event_met_pfmetPhi",    1);
  t.SetBranchStatus("event_met_pfmetsignificance",    1);
  t.SetBranchStatus("event_BeamSpot_x",    1);
  t.SetBranchStatus("event_BeamSpot_y",    1);
  //  t.SetBranchStatus("event_BeamSpot_z",    1);
  t.SetBranchStatus("event_RhoForLeptonIsolation",    1);
  //  t.SetBranchStatus("W_*",    1);

  t.SetBranchStatus("W_mt",    1);
  t.SetBranchStatus("W_pzNu1",    1);
  t.SetBranchStatus("W_pzNu2",    1);
  // t.SetBranchStatus("WJetsPullPFCor",    1);
  //t.SetBranchStatus("fit_*",    1);

  //   t.SetBranchStatus("fit_aj_px",    1);
  //   t.SetBranchStatus("fit_aj_py",    1);
  //   t.SetBranchStatus("fit_aj_pz",    1);
  //   t.SetBranchStatus("fit_aj_e",    1);

  //   t.SetBranchStatus("fit_bj_px",    1);
  //   t.SetBranchStatus("fit_bj_py",    1);
  //   t.SetBranchStatus("fit_bj_pz",    1);
  //   t.SetBranchStatus("fit_bj_e",    1);

  t.SetBranchStatus("fit_mlvjj",    1);
  t.SetBranchStatus("fi2_mlvjj",    1);
  t.SetBranchStatus("fit_chi2",    1);
  t.SetBranchStatus("fit_NDF",    1);
  t.SetBranchStatus("fit_status",    1);
  t.SetBranchStatus("gdevtt",    1);
  t.SetBranchStatus("evtNJ",    1);


  if(isElectronTree) {
    t.SetBranchStatus("W_electron_et",    1);
    t.SetBranchStatus("W_electron_trackiso",    1);
    t.SetBranchStatus("W_electron_hcaliso",    1);
    t.SetBranchStatus("W_electron_ecaliso",    1);
    t.SetBranchStatus("W_electron_pt",    1);
    t.SetBranchStatus("W_electron_isWP80",    1);
    t.SetBranchStatus("W_electron_eta",    1);
    t.SetBranchStatus("W_electron_deltaphi_in",    1);
    t.SetBranchStatus("W_electron_deltaeta_in",    1);
    t.SetBranchStatus("W_electron_vx",    1);
    t.SetBranchStatus("W_electron_vy",    1);
    // t.SetBranchStatus("W_electron_vz",    1);
    t.SetBranchStatus("W_electron_e",    1);
    t.SetBranchStatus("W_electron_px",    1);
    t.SetBranchStatus("W_electron_py",    1);
    t.SetBranchStatus("W_electron_pz",    1);

  }
  else {
    t.SetBranchStatus("W_muon_pt",    1);
    t.SetBranchStatus("W_muon_eta",    1);
    t.SetBranchStatus("W_muon_trackiso",    1);
    t.SetBranchStatus("W_muon_hcaliso",    1);
    t.SetBranchStatus("W_muon_ecaliso",    1);
    t.SetBranchStatus("W_muon_d0bsp",    1);
    t.SetBranchStatus("W_muon_e",    1);
    t.SetBranchStatus("W_muon_px",    1);
    t.SetBranchStatus("W_muon_py",    1);
    t.SetBranchStatus("W_muon_pz",    1);
  }
  t.SetBranchStatus("Mass2j_PFCor",    1);
  t.SetBranchStatus("MassV2j_PFCor",    1);
  t.SetBranchStatus("cosJacksonAngle2j_PFCor",    1);
  t.SetBranchStatus("cosJacksonAngleV2j_PFCor",    1);
}

//#endif
