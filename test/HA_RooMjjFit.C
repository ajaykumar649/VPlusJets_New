/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *   Osipenkov, Ilya, Texas A&M - ilyao@fnal.gov
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

// #ifndef RooWjjFitter_h
// #define RooWjjFitter_h

// ROOT
#include <string.h>
#include <TString.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCut.h>
// #include <RooRealVar.h>
// #include <RooAbsReal.h>
// #include <RooAbsPdf.h>
// #include <RooFormulaVar.h>
// #include <RooAddPdf.h>
// #include <RooDataSet.h>
// #include <RooFitResult.h>
// #include <RooKeysPdf.h>
// #include <RooPlot.h>
// #include <RooHist.h>
// #include <TPaveText.h>
// #include <TLatex.h>
// #include <RooGaussian.h>
#include <TLegend.h>
// #include <RooCurve.h>



//const TString MCDirectory = "pTsmeareddata";
// const TString MCDirectory =   "data/ReducedTree/NewReducedTree/";
// const TString DataDirectory = "data/ReducedTree/NewReducedTree/";

const TString MCDirectory =   "data/ReducedTree/NewKfitRDTree/";
const TString DataDirectory = "data/ReducedTree/NewKfitRDTree/";

const TString QCDDirectory = "428MC";

// const double MINRange = 50.0;
// const double MAXRange = 150.0;
const double MINRange = 50.0;
const double MAXRange = 120.0;
//const double MAXRange = 120.0;

//const int BINWIDTH = 10;
const int BINWIDTH = 5;
const bool includeNuisancePDF = true;
//const bool drawSystematics = true;
const bool drawSystematics = false;
double singleTopNorm_;
double ttbarNorm_;
double zjetsNorm_;
//double ztautauNorm_;
double dibosonNorm_;
double NMC_WpJ_;


const double WMassMin = 65.;
const double WMassMax = 95.;


const float IntLUMI = 2100.0;
const bool truncateFitRange = true;
const int NBINSFORPDF = (int)((MAXRange-MINRange)/(BINWIDTH));


RooRealVar *mjj_;
RooRealVar *JES_scale;
RooAbsReal *shiftedMass;
RooRealVar *JES_scale2;
RooAbsReal *shiftedMass2;
// RooRealVar *reso_;
// RooRealVar *zero_;
// RooGaussModel *resModel_;

using namespace RooFit;



void HA_RooMjjFit(int channel=0) {

  RooWjjFitterNarrow(channel, 2, "Mass2j_PFCor");

}







///////// --------- channel 0 : combined,  1: mu,    2: ele --------------
void RooWjjFitterNarrow(int channel=0, int nJet, char PLOTVAR[])
{

// 3JcutsV2 : Default
  // const char* mycuts = "((evtNJ==2 || evtNJ==3) && fit_status==0 && fit_chi2<30.)";
//   const char* mycuts = "((evtNJ==2 || evtNJ==3) && JetPFCor_Pt[1]/Mass2j_PFCor>0.3 && JetPFCor_bDiscriminator[0]<1.74 && JetPFCor_bDiscriminator[1]<1.74 && fit_status==0 && fit_chi2<30. && (JetPFCor_QGLikelihood[0]*JetPFCor_QGLikelihood[1])>0.1 && cosJacksonAngle2j_PFCor<0.8 && cosJacksonAngle2j_PFCor>-0.6)";


//   const char* mycuts = "((evtNJ==2||evtNJ==3)&&fit_status==0)";

//  const char* mycuts = "(evtNJ==2)";


  char* njetCut = (char*) "(evtNJ==2)";
  if(nJet==3) njetCut = (char*) "(evtNJ==3)";
  if(njetCut==0) njetCut = (char*) "(evtNJ==2||evtNJ==3)";


  std::ostringstream ostr;
  ostr << "(" << njetCut <<  ")";
  string str = ostr.str();
  const char* mycuts = str.c_str();


  const char* PLOTPREFIX = "mJJ";
  const char* XLABEL = "m_{jj}";

  TCanvas *c;
  TCanvas *c1;
  TCanvas *c2;


   // The fit variable - dijet invariant mass
   mjj_ = new RooRealVar( "Mass2j_PFCor", XLABEL, MINRange, MAXRange, "GeV");
   RooRealVar Mass = *mjj_;
   double nMuData, nEleData, QCDNorm;

   gROOT->cd();
   char temp[50];
   TFile fin(DataDirectory + "RD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root", "read");
   TTree* treeTemp = (TTree*) fin.Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* treemu = treeTemp->CopyTree( mycuts );
   fin.Close();
   nMuData = treemu->GetEntries();
   cout << "Total Mu Events = " << nMuData << endl;


   TFile fin2(DataDirectory + "RD_WenuJets_DataAll_GoldenJSON_2p1invfb.root", "read");
   TTree* treeTemp2 = (TTree*) fin2.Get("WJet");
   ActivateTreeBranches(*treeTemp2, true);
   gROOT->cd();
   TTree* treeele = treeTemp2->CopyTree( mycuts );
   fin2.Close();
   nEleData = treeele->GetEntries();
   cout << "Total El Events = " << nEleData << endl;

   // ------ Normalization of QCD events: 0.8% in Mu data and 3% in Ele data
   if(channel==1) QCDNorm = 0.008*nMuData; 
   if(channel==2) QCDNorm = 0.03*nEleData; 
   if(channel==0) QCDNorm = 0.008*nMuData + 0.03*nEleData; 


   ///////// convert Histograms into RooDataHists
   TTree* treeForDataSet = treemu;
   if(channel == 2) treeForDataSet = treeele;
   RooDataSet* data = new RooDataSet("data","data", treeForDataSet, Mass);
   RooDataSet* data_ele = new RooDataSet("data_ele","data_ele",treeele, Mass);
   data->Print();
   data_ele->Print();
   if(channel == 0) data->append(*data_ele);
   cout << "Made dataset" << endl;
   data->Print();


   // ********** Construct signal & bkg shape PDF ********** //
   RooAbsPdf* signalShapePdf_ = makeSignalPdf(channel, PLOTVAR, mycuts);
   cout << "Made signal pdf" << endl;
   RooAbsPdf *bkgShapePdf_ = makeBkgPdf(channel, PLOTVAR, 0, mycuts);

  //------- for systematics: q2 up --------
  // RooAbsPdf *bkgShapePdf_ = makeBkgPdf(channel, PLOTVAR, 1);

  //------- for systematics: q2 down --------
   //RooAbsPdf *bkgShapePdf_ = makeBkgPdf(channel, PLOTVAR, 2);

   RooAbsPdf* qcdPdf_;
   RooAbsPdf* ttPdf_;
   RooAbsPdf* singleTopPdf_;
   RooAbsPdf* zjetsPdf_;

   if(includeNuisancePDF) { 
     qcdPdf_ = makeQCDPdf(channel, PLOTVAR, mycuts);
     ttPdf_ = makeTopPairPdf(channel, PLOTVAR, mycuts);
     singleTopPdf_ = makeSingleTopPdf(channel, PLOTVAR, mycuts);
     zjetsPdf_ = makeZJetsPdf(channel, PLOTVAR, mycuts);
   }
   cout << "Made bkg pdf" << endl;



   // Define background yield variables: they are not related to each other 
   RooRealVar nWjets("nWjets","nWjets",        20000.,     0.0,   1000000.);
   // RooRealVar nDiboson("nDiboson","nDiboson",  dibosonNorm_); 
   RooRealVar nDiboson("nDiboson","nDiboson",  1.286*dibosonNorm_); 
   ////// Normalize to WW->lnulnu cross section measurement
   // fix the top and single top normalization
   RooRealVar nTTbar("nTTbar","", ttbarNorm_);
   RooRealVar nSingleTop("nSingleTop","", singleTopNorm_);
   RooRealVar nQCD("nQCD","nQCD", QCDNorm);
   RooRealVar nZjets("nZjets","nZjets", zjetsNorm_);
   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////

   RooArgList* components;
   RooArgList* yields;	 
   if(includeNuisancePDF) {
     components = new RooArgList(*signalShapePdf_,*bkgShapePdf_, *ttPdf_, *singleTopPdf_, *qcdPdf_, *zjetsPdf_);
     yields = new RooArgList(nDiboson, nWjets, nTTbar, nSingleTop, nQCD, nZjets);
   }
   else {
     components = new RooArgList(*signalShapePdf_,*bkgShapePdf_);
     yields = new RooArgList(nDiboson, nWjets);
   }
   RooAddPdf totalPdf("totalPdf","extended sum pdf", *components, *yields);


   Mass.setRange("Range55To250", 40, 300) ;
   Mass.setRange("RangeLowerSB", MINRange, WMassMin) ;
   Mass.setRange("RangeUpperSB", WMassMax, MAXRange);
   Mass.setRange("RangeWmass",   WMassMin, WMassMax);
   Mass.setRange("RangeForPlot", MINRange, MAXRange) ;
   Mass.setRange("RangeDefault", MINRange, MAXRange) ;


   // ********* Do the Actual Fit ********** //  
   // const char* rangeString = "Range55To250";
   char* rangeString = "RangeDefault";
   if(truncateFitRange) 
      rangeString = "RangeLowerSB,RangeUpperSB";

   RooFitResult *fitResult;
//    if(channel==0) {
//      singleTopNorm_ *= 2.0;
//    }

   //   RooGaussian constJES("constJES","", *JES_scale, RooConst(0.0),RooConst(0.05)) ;
   RooGaussian constJES2("constJES2","", *JES_scale2, RooConst(0.0),RooConst(0.05)) ;
   RooGaussian constQCD("constQCD","", nQCD, RooConst(QCDNorm),RooConst(0.5*QCDNorm)) ;
   RooGaussian constTTbar("constTTbar","", nTTbar, RooConst(ttbarNorm_),RooConst(0.1*ttbarNorm_)) ;
   RooGaussian constSingleTop("constSingleTop","", nSingleTop, RooConst(singleTopNorm_),RooConst(0.1*singleTopNorm_)) ;
   RooGaussian constZpJ("constZpJ", "constZpJ", nZjets, 
			RooConst(nZjets.getVal()),
			RooConst(nZjets.getVal()*0.1));
   RooGaussian constDiboson("constDiboson", "constDiboson", nDiboson,
			    RooConst(nDiboson.getVal()), 
			    RooConst(nDiboson.getVal()*0.15));


   RooArgSet exConstraints(constJES2);

   nDiboson.setConstant( false );
   exConstraints.add( constDiboson );

   nTTbar.setConstant(false);
   exConstraints.add(constTTbar);

   nSingleTop.setConstant(false);
   exConstraints.add(constSingleTop);

   nZjets.setConstant(false);
   exConstraints.add(constZpJ);

   nQCD.setConstant(false);
   exConstraints.add(constQCD);

   exConstraints.add(constDiboson);
   JES_scale2.setVal(0.0);
   JES_scale2.setConstant( kTRUE );
   //nDiboson.setConstant( kTRUE );

   fitResult = totalPdf.fitTo(*data, Save(true), 
			      ExternalConstraints(exConstraints),
			      RooFit::Extended(true), 
			      //RooFit::Minos(true), 
			      RooFit::Hesse(true),
			      PrintEvalErrors(-1),
			      RooFit::Range(rangeString),
			      Warnings(false) 
			      );

   fitResult->Print("v");



   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////
   // ----------- These are needed for systematics ----------------
   // **** since our systematics are mostly uncorrelated we add them in quadrature ----
   // ==== (1) From MC statistics: The uncertainty in W+jets yield grows by 0.1196% 
   // = sqrt(0.005826**2 - 0.005702**2). This is symmetric.
   // ==== (2) JEC: By fixing the JEC to 0 the W+jets yield changes by -0.1871% 
   // = 33143 (default) - 33081 (fixed to 0). We can assume this to be symmetric.
   // ==== (3) from q2 variation: We get 32429 W+jets events in default fit. 
   //   We get 31539 events with q^2 down template (-2.7444%), 
   //   32307 from scale up template (-0.3762%). 
   //   The maximum difference being -2.7444 %.


   double NBkg_sys = sqrt(0.001196**2 + 0.001871**2 + 0.027444**2)*nWjets.getVal(); 
   double NBkg_statsys = sqrt(NBkg_sys**2+nWjets.getError()**2); 

   RooDataSet* wjetsToyData = bkgShapePdf_->generate(*mjj_, 10000000);
   TH1D* wjetstoyhist = new TH1D("wjetstoyhist","", 100*NBINSFORPDF, MINRange, MAXRange);
   wjetstoyhist->Sumw2();
   wjetsToyData->tree()->Draw(TString(PLOTVAR)+TString(">>wjetstoyhist"),"","goff");
   TH1D* wjetshist_sysP = wjetstoyhist->Clone("wjetshist_sysP");
   wjetshist_sysP->SetLineColor(2);
   //  wjetshist_sysP->SetLineStyle(2);
   wjetshist_sysP->SetLineWidth(2);
   TH1D* wjetshist_sysM = wjetstoyhist->Clone("wjetshist_sysM");
   wjetshist_sysM->SetLineColor(2);
   //wjetshist_sysM->SetLineStyle(2);
   wjetshist_sysM->SetLineWidth(2);
   double den = wjetstoyhist->Integral();
   wjetshist_sysP->Scale( 100*NBkg_sys / den);
   wjetshist_sysM->Scale( -100*NBkg_sys / den);
   TH1D* wjetshist_statsysM = wjetshist_sysM->Clone("wjetshist_statsysM"); 
   TH1D* wjetshist_statsysP = wjetshist_sysM->Clone("wjetshist_statsysP"); 
   wjetshist_statsysM->SetLineStyle(1);
   wjetshist_statsysP->SetLineStyle(1);
   wjetshist_statsysM->Scale( -100*NBkg_statsys / wjetshist_statsysM->Integral() );
   wjetshist_statsysP->Scale( 100*NBkg_statsys / wjetshist_statsysP->Integral() );

   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////


   // ********** Make and save Canvas for the plots ********** //
   gROOT->ProcessLine(".L ~kalanand/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.19);
  tdrStyle->SetPadRightMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.5);
  RooAbsData::ErrorType errorType = RooAbsData::SumW2;


   TString cname = TString(PLOTPREFIX) + TString("-combined-fit");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit");
   if(truncateFitRange) cname = cname + TString("-truncated");
   c = new TCanvas(cname,cname,500,500);
   RooPlot* frame1 = Mass.frame( MINRange, MAXRange, 
				 (int) ((MAXRange-MINRange)/BINWIDTH) );
   data->plotOn(frame1,RooFit::DataError(errorType), Name("h_data"), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data),
		   Components(*signalShapePdf_),
		   DrawOption("LF"), VLines(),FillStyle(1001),
		   FillColor(kOrange), LineColor(kOrange),
		   Name("h_diboson"),Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Name("h_total"), VLines(), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components(*bkgShapePdf_), 
		   LineColor(kRed), VLines(), Name("h_Wjets"), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components("bkgShapePdf,ttPdf,singleTopPdf,qcdPdf,zjetsPdf"), 
		   Name("h_Background"), Range("RangeForPlot"), VLines(),Invisible());
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components("signalShapePdf,bkgShapePdf,ttPdf,singleTopPdf,qcdPdf,zjetsPdf"), 
		   Name("h_SM"), Range("RangeForPlot"), VLines(),
		   Invisible());

   if(includeNuisancePDF) {
     totalPdf.plotOn(frame1,ProjWData(*data),Components("ttPdf,singleTopPdf"),
		     LineColor(kBlack), Name("h_Top"), VLines(),Range("RangeForPlot"));
     totalPdf.plotOn(frame1,ProjWData(*data),Components(*qcdPdf_), 
		     LineColor(kGreen), Name("h_QCD"), VLines(), Range("RangeForPlot"));
     totalPdf.plotOn(frame1,ProjWData(*data),Components(*zjetsPdf_), 
		     LineColor(kMagenta), Name("h_Zjets"), VLines(), Range("RangeForPlot"));
   }

   data->plotOn(frame1,RooFit::DataError(errorType));
   frame1->SetMinimum(0);
   frame1->SetMaximum(2* frame1->GetMaximum());
   frame1->Draw("e0");
   TPaveText *plotlabel4 = new TPaveText(0.55,0.87,0.85,0.92,"NDC");
   plotlabel4->SetTextColor(kBlack);
   plotlabel4->SetFillColor(kWhite);
   plotlabel4->SetFillStyle(0);
   plotlabel4->SetBorderSize(0);
   plotlabel4->SetTextAlign(12);
   plotlabel4->SetTextSize(0.03);
   TPaveText *plotlabel5 = new TPaveText(0.55,0.82,0.85,0.87,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetFillStyle(0);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   TPaveText *plotlabel6 = new TPaveText(0.55,0.77,0.85,0.82,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetFillStyle(0);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   TPaveText *plotlabel7 = new TPaveText(0.55,0.72,0.85,0.77,"NDC");
   plotlabel7->SetTextColor(kBlack);
   plotlabel7->SetFillColor(kWhite);
   plotlabel7->SetFillStyle(0);
   plotlabel7->SetBorderSize(0);
   plotlabel7->SetTextAlign(12);
   plotlabel7->SetTextSize(0.03);
   TPaveText *plotlabel8 = new TPaveText(0.55,0.67,0.85,0.72,"NDC");
   plotlabel8->SetTextColor(kBlack);
   plotlabel8->SetFillColor(kWhite);
   plotlabel8->SetFillStyle(0);
   plotlabel8->SetBorderSize(0);
   plotlabel8->SetTextAlign(12);
   plotlabel8->SetTextSize(0.03);
   TPaveText *plotlabel9 = new TPaveText(0.55,0.62,0.85,0.67,"NDC");
   plotlabel9->SetTextColor(kBlack);
   plotlabel9->SetFillColor(kWhite);
   plotlabel9->SetFillStyle(0);
   plotlabel9->SetBorderSize(0);
   plotlabel9->SetTextAlign(12);
   plotlabel9->SetTextSize(0.03);

   float chi2LegendHeight = 0.23;
   if(nJet==3) chi2LegendHeight = 0.3;
   TPaveText *plotlabel1000 = new TPaveText(0.4,chi2LegendHeight,0.7,chi2LegendHeight+0.15,"NDC");
   plotlabel1000->SetTextColor(kBlack);
   plotlabel1000->SetFillColor(kWhite);
   plotlabel1000->SetFillStyle(0);
   plotlabel1000->SetBorderSize(0);
   plotlabel1000->SetTextAlign(12);
   plotlabel1000->SetTextSize(0.04);

   //sprintf(temp, "Di-boson = %d #pm %d", nDiboson.getVal(), nDiboson.getPropagatedError(*fitResult));
   sprintf(temp, "Di-boson = %d #pm %d", nDiboson.getVal(), nDiboson.getError());
   plotlabel4->AddText(temp);
   //sprintf(temp, "W+jets = %d #pm %d", nWjets.getVal(), nWjets.getPropagatedError(*fitResult));
   sprintf(temp, "W+jets = %d #pm %d", nWjets.getVal(), nWjets.getError());
   plotlabel5->AddText(temp);
   sprintf(temp, "t#bar{t}, Top = %d #pm %d", nSingleTop.getVal()+nTTbar.getVal(), 
	   TMath::Sqrt(pow(nSingleTop.getError(),2)+pow(nTTbar.getError(),2)));
   plotlabel6->AddText(temp);
   sprintf(temp, "QCD = %d #pm %d", nQCD.getVal(), nQCD.getError());
   plotlabel7->AddText(temp);
   sprintf(temp, "Z+jets = %d #pm %d", nZjets.getVal(), nZjets.getError());
   plotlabel8->AddText(temp);
   //double chi2fit = frame1->chiSquare("h_total", "h_data", 3)/1.8;
   double NData_WpJ=nWjets.getVal();
   double k_WpJ=NMC_WpJ_/NData_WpJ;
   double chi2fit = frame1->chiSquare("h_total", "h_data", fitResult->floatParsFinal().getSize())/(1.0+1.0/k_WpJ);
   sprintf(temp, "#chi^{2}/dof = %.2f",chi2fit );
   plotlabel1000->AddText(temp);
   plotlabel4->Draw();
   plotlabel5->Draw();
   plotlabel6->Draw();
   plotlabel7->Draw();
   plotlabel8->Draw();
   plotlabel9->Draw();
   plotlabel1000->Draw();
   cout << "======= chi^2/dof = " << chi2fit << endl;
   cmsPrelim2();
   //TLegend* legend = new TLegend(0.35,0.35,0.55,0.55);
   //  TLegend* legend = new TLegend(0.65,0.45,0.85,0.62);
   TLegend* legend = new TLegend(0.25,0.6,0.45,0.8);
   legend->SetName("legend");
   RooHist* datahist = frame1->getHist("h_data");
   RooCurve* dibosonhist = frame1->getCurve("h_diboson");
   RooCurve* wjetshist = frame1->getCurve("h_Wjets");
   RooCurve* tophist = frame1->getCurve("h_Top");
   RooCurve* qcdhist = frame1->getCurve("h_QCD");
   RooCurve* zjetshist = frame1->getCurve("h_Zjets");
   //RooCurve* ztautauhist = frame1->getCurve("h_Ztautau");

   legend->AddEntry( datahist, "Data", "P");  
   legend->AddEntry( dibosonhist, "Di-boson", "F");
   legend->AddEntry( wjetshist, "W+jets", "L");
   if(includeNuisancePDF)
     legend->AddEntry( tophist, "t#bar{t}, Top", "L");
   legend->AddEntry( qcdhist, "QCD", "L");
   legend->AddEntry( zjetshist, "Z+jets", "L");
   // legend->AddEntry( ztautauhist, "Z#rightarrow#tau#tau", "L");
   legend->SetFillColor(0);
   legend->Draw();

   double lineheight = 2200.;
   if(nJet==3) lineheight = 750;
   TLine* lowerBoundary = new TLine(WMassMin, 0., WMassMin, lineheight);
   lowerBoundary->SetLineWidth(3);
   TLine* upperBoundary = new TLine(WMassMax, 0., WMassMax, lineheight);
   upperBoundary->SetLineWidth(3);
   lowerBoundary->Draw();
   upperBoundary->Draw();
   c->SaveAs( cname + TString(".eps"));
   c->SaveAs( cname + TString(".gif"));
   c->SaveAs( cname + TString(".root"));
   c->SaveAs( cname + TString(".png"));
   c->SaveAs( cname + TString(".C"));
   c->SaveAs( cname + TString(".pdf"));


   // make stacked plot
   RooPlot* sframe = Mass.frame(MINRange, MAXRange, 
				int((MAXRange-MINRange)/BINWIDTH));
   data->plotOn(sframe, RooFit::DataError(errorType), Name("h_data"), Range("RangeForPlot"));
   int comp(1);
   totalPdf.plotOn(sframe,ProjWData(*data), VLines(), DrawOption("LF"), FillStyle(1001),
		   FillColor(kOrange), LineColor(kOrange), Name("h_total"),
		   Range("RangeForPlot"));
//    totalPdf.plotOn(sframe,ProjWData(*data), 
//    		   Name("h_total"), Range("RangeForPlot"));
   components->remove((*components)[0]);
   int linec = kRed;
   while (components->getSize() > 0) {
     totalPdf.plotOn(sframe, ProjWData(*data), FillColor(linec), 
		     DrawOption("LF"), VLines(), 
		     Components(RooArgSet(*components)),
		     FillStyle(1001), LineColor(linec),
		     Range("RangeForPlot"));
//      totalPdf.plotOn(sframe, ProjWData(*data), 
//      		     Components(RooArgSet(*components)),
//      		     LineColor(linec),
// 		     Range("RangeForPlot"));
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
   data->plotOn(sframe,RooFit::DataError(errorType), Range("RangeForPlot"));
   sframe->SetMinimum(0);
   sframe->SetMaximum(2* sframe->GetMaximum());

   TCanvas * cs = new TCanvas("cs", TString(PLOTPREFIX) + "_Stacked", 500, 500);
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
   lowerBoundary->Draw();
   upperBoundary->Draw();
   cs.Print(cs.GetTitle() + TString(".eps"));
   cs.Print(cs.GetTitle() + TString(".pdf"));
   cs.Print(cs.GetTitle() + TString(".root"));
   cs.Print(cs.GetTitle() + TString(".png"));

   ///////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////
   //-------Make logY plots ---------------------------------
   RooPlot* frame1log = frame1->Clone("frame1log");
   TString cname = TString(PLOTPREFIX) + TString("-combined-fit-logY");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-logY");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-logY");
   if(truncateFitRange) cname = cname + TString("-truncated");
   c1 = new TCanvas(cname,cname,500,500);
   frame1->SetMinimum(1.);
   frame1->SetMaximum(1000000000);
   frame1->Draw("e0");
   plotlabel4->Draw();
   plotlabel5->Draw();
   plotlabel6->Draw();
   plotlabel7->Draw();
   plotlabel8->Draw();
   plotlabel9->Draw();
   cmsPrelim2();
   //TLegend* legend = new TLegend(0.6,0.42,0.85,0.62);
   TLegend* legend = new TLegend(0.25,0.56,0.55,0.8);
   legend->SetName("legendlog-Y");
   legend->AddEntry( datahist, "Data", "P");  
   legend->AddEntry( dibosonhist, "Di-boson", "F");
   legend->AddEntry( wjetshist, "W+jets", "L");
   if(includeNuisancePDF) 
     legend->AddEntry( tophist, "t#bar{t}, Top", "L");
   legend->AddEntry( qcdhist, "QCD", "L");
   legend->AddEntry( zjetshist, "Z+jets", "L");
   legend->SetFillColor(0);
   legend->Draw();
   TLine* lowerBoundary1 = new TLine(WMassMin, 0., WMassMin, 10000.);
   lowerBoundary1->SetLineWidth(3);
   TLine* upperBoundary1 = new TLine(WMassMax, 0., WMassMax, 10000.);
   upperBoundary1->SetLineWidth(3);
   lowerBoundary1->Draw();
   upperBoundary1->Draw();
   c1->SetLogy(1);
   c1->SaveAs( cname + TString(".eps"));
   c1->SaveAs( cname + TString(".gif"));
   c1->SaveAs( cname + TString(".root"));
   c1->SaveAs( cname + TString(".png"));
   c1->SaveAs( cname + TString(".C"));
   c1->SaveAs( cname + TString(".pdf"));
   // ----------------- logY plot done -----------------------------




   ///////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////
   //-------- Create a new frame to draw the residual distribution 
   //--------  and add the distribution to the frame
   RooPlot* frame2 = Mass.frame( MINRange, MAXRange, (int) ((MAXRange-MINRange)/BINWIDTH)); 
   // RooPlot* frame2 = Title("After subtracting W+jets")) ;
   // data->plotOn(frame2, RooFit::DataError(errorType), Invisible());
   data->plotOn(frame2,MarkerStyle(0),MarkerSize(0),MarkerColor(0),LineColor(0),LineWidth(0), Range("RangeForPlot"));
   totalPdf.plotOn(frame2,ProjWData(*data),Components(*signalShapePdf_),DrawOption("LF"), VLines(), FillStyle(1001),FillColor(kOrange),Name("h_diboson"), Range("RangeForPlot"));

   //    totalPdf.plotOn(frame2,ProjWData(*data), Components("stSPdf, stTPdf, stTWPdf"),LineColor(kBlack), Name("h_SingleTop"), Range("RangeForPlot"));
   
   //// Construct a histogram with the residuals of the data w.r.t. the curve
   const char* baseline = "h_Background";
   RooHist* hresid = frame1->residHist("h_data", baseline) ;
   hresid->SetMarkerSize(0.8);
   // frame2->addPlotable(hresid, "P") ;  
   frame2->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
   frame2->SetMaximum(750);
   frame2->SetMinimum(-150);

   cname = TString(PLOTPREFIX) + TString("-combined-fit-subtracted");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-subtracted");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-subtracted");
   if(truncateFitRange) cname = cname + TString("-truncated");
   c2 = new TCanvas(cname,cname,500,500);
   frame2->Draw() ;
   hresid->Draw("P");
   if(drawSystematics) {
     wjetshist_sysP->Draw("Csame");
     wjetshist_sysM->Draw("Csame");
   }
   cmsPrelim2();
   TLegend* legend2 = new TLegend(0.6,0.7,0.85,0.9);
   legend2->SetName("legendSubtracted");
   legend2->AddEntry( datahist, "Data", "P");  
   legend2->AddEntry( dibosonhist, "Di-boson", "F");
   if(drawSystematics) {
     legend2->AddEntry( wjetshist_sysM, "Systematics", "L");
     // legend2->AddEntry( wjetshist_statsysM, "Fit stat. #oplus syst.", "L");
   }
   legend2->SetFillColor(0);
   legend2->Draw();
   TLine* lowerBoundary2 = new TLine(WMassMin, 0., WMassMin, 300.);
   lowerBoundary2->SetLineWidth(3);
   TLine* upperBoundary2 = new TLine(WMassMax, 0., WMassMax, 300.);
   upperBoundary2->SetLineWidth(3);
   lowerBoundary2->Draw();
   upperBoundary2->Draw();
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
   data->plotOn(frame3,MarkerStyle(0),MarkerSize(0),MarkerColor(0),LineColor(0),LineWidth(0), Range("RangeForPlot"));
   //// Construct a histogram with the residuals of the data w.r.t. the curve
   hresid = frame1->residHist("h_data", "h_total", true) ;
   //hresid = frame1->residHist("h_data", "h_total");
   hresid->SetMarkerSize(0.8);
   //frame3->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
   frame3->GetYaxis()->SetTitle("Residual ( #sigma )");
   frame3->SetMaximum(6);
   frame3->SetMinimum(-6);
   frame3->GetXaxis()->SetNdivisions(505);
   cname = TString(PLOTPREFIX) + TString("-combined-fit-residual");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-residual");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-residual");
   if(truncateFitRange) cname = cname + TString("-truncated");
   c3 = new TCanvas(cname,cname,500,500);
   frame3->Draw() ;
   hresid->Draw("P");
   cmsPrelim2();
   TLegend* legend2 = new TLegend(0.6,0.8,0.85,0.9);
   legend2->SetName("legendResidual");
   legend2->AddEntry( datahist, "Data", "P");  
   legend2->SetFillColor(0);
   legend2->Draw();
   TLine* lowerBoundary3 = new TLine(WMassMin, -5., WMassMin, 5.);
   lowerBoundary3->SetLineWidth(3);
   TLine* upperBoundary3 = new TLine(WMassMax, -5., WMassMax, 5.);
   upperBoundary3->SetLineWidth(3);
   lowerBoundary3->Draw();
   upperBoundary3->Draw();
   c3->SaveAs( cname + TString(".eps"));
   c3->SaveAs( cname + TString(".gif"));
   c3->SaveAs( cname + TString(".root"));
   c3->SaveAs( cname + TString(".png"));
   c3->SaveAs( cname + TString(".C"));
   c3->SaveAs( cname + TString(".pdf"));


//------------ Some jugglery to compute the PDF normalizations in W mass window -------
   RooAbsReal* igx_sig;
   RooAbsReal* igx_bkg;
   RooAbsReal* igx_TTbar;
   RooAbsReal* igx_SingleTop;
   RooAbsReal* igx_QCD;
   RooAbsReal* igx_Zjets;
   RooAbsReal* igx_tot;

   igx_sig = signalShapePdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_bkg = bkgShapePdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_TTbar = ttPdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_SingleTop = singleTopPdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_QCD = qcdPdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_Zjets = zjetsPdf_->createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));
   igx_tot = totalPdf.createIntegral(Mass,NormSet(Mass),Range("RangeWmass"));

   double numDiboson = nDiboson.getVal() * igx_sig->getVal();
   double numWjets   = nWjets.getVal() * igx_bkg->getVal();
   double numTTbar   = nTTbar.getVal() * igx_TTbar->getVal();
   double numSingleTop = nSingleTop.getVal() * igx_SingleTop->getVal();
   double numQCD     = nQCD.getVal() * igx_QCD->getVal();
   double numZjets   = nZjets.getVal() * igx_Zjets->getVal();

   double numTotal   = numDiboson+numWjets+numTTbar+numSingleTop+numQCD+numZjets;
   double numBkg     = numWjets+numTTbar+numSingleTop+numQCD+numZjets;


   //------------ Compute number of data events in W mass window -------

   gROOT->cd();
   //char masscut[500];
   std::ostringstream ostr;
   ostr << "Mass2j_PFCor>" << WMassMin << " && Mass2j_PFCor<" << WMassMax;
   char* masscut = ostr.str().c_str();
   TTree* treemuWmass = treemu->CopyTree( masscut);
   delete treemu;
   TTree* treeeleWmass = treeele->CopyTree( masscut);
   delete treeele;
   double numData = treemuWmass->GetEntries() + treeeleWmass->GetEntries();


   cout << "-------- Printing yields in W mass range: -------" << endl;
   cout << "numData = " << numData << endl;
   cout << "numDiboson = " << numDiboson << endl;
   cout << "numWjets = " <<  numWjets << endl;
   cout << "numTTbar = " <<  numTTbar << endl;
   cout << "numSingleTop = " << numSingleTop  << endl;
   cout << "numQCD = " <<  numQCD << endl;
   cout << "numZjets = " << numZjets  << endl;

   cout << "numTotal = " <<  numTotal << endl;
   cout << "numBkg = " <<  numBkg << endl;
   cout << "---------------------------------------------------------" << endl;

   //    if(data) delete data;
   //    if(c) delete c;

}









// // ***** Function to return the signal Pdf *** //
RooAbsPdf*  makeSignalPdf(int channel, char PLOTVAR[], char* cut="gdevtt") {

  TFile* wwShape_mu_file =  new TFile(MCDirectory + "RD_mu_WW_CMSSW428.root", "READ");
  TTree* treeTemp1 = (TTree*) wwShape_mu_file->Get("WJet");
  ActivateTreeBranches(*treeTemp1);
  TFile* wzShape_mu_file =  new TFile(MCDirectory + "RD_mu_WZ_CMSSW428.root", "READ");
  TTree* treeTemp2 = (TTree*) wzShape_mu_file->Get("WJet");
  ActivateTreeBranches(*treeTemp2);


  TFile* wwShape_ele_file =  new TFile(MCDirectory + "RD_el_WW_CMSSW428.root", "READ");
  treeTemp3 = (TTree*) wwShape_ele_file->Get("WJet");
  ActivateTreeBranches(*treeTemp3, true);
  TFile* wzShape_ele_file =  new TFile(MCDirectory + "RD_el_WZ_CMSSW428.root", "READ");
  treeTemp4 = (TTree*) wzShape_ele_file->Get("WJet");
  ActivateTreeBranches(*treeTemp3, true);

  //Scale the trees by the Crossection/Ngenerated (43/4225916=1.01753087377979123e-05 for WW and 18/4265243=4.22015814808206740e-06 for WZ).

  TString WW_selection = TString("0.0000101753087377979123*") + TString(cut);
  TString WZ_selection = TString("0.00000422015814808206740*") + TString(cut);

  TH1* th1ww = new TH1D("th1ww", "th1ww", 5*NBINSFORPDF, MINRange, MAXRange);

  if(channel==0 || channel==1) {
    treeTemp1->Draw( TString(PLOTVAR)+TString(">>th1ww"), WW_selection, "goff");
    treeTemp2->Draw( TString(PLOTVAR)+TString(">>+th1ww"), WZ_selection, "goff");
  }

  if(channel==0) {
    treeTemp3->Draw(TString(PLOTVAR)+TString(">>+th1ww"), WW_selection, "goff");
    treeTemp4->Draw( TString(PLOTVAR)+TString(">>+th1ww"), WZ_selection, "goff");
  }
  if(channel==2) {
    treeTemp3->Draw(TString(PLOTVAR)+TString(">>th1ww"), WW_selection, "goff");
    treeTemp4->Draw( TString(PLOTVAR)+TString(">>+th1ww"), WZ_selection, "goff");
  }

  dibosonNorm_ = th1ww->Integral() * IntLUMI;

  cout << "-------- Number of expected WW+WZ events = " << th1ww->Integral() << " x " << IntLUMI << " = " << dibosonNorm_ << endl;


//   JES_scale = new RooRealVar("JES_scale","", 0.0,   -0.1, 0.1);
//   shiftedMass = new RooFormulaVar("shiftedMass", "@0*(1.+@1)", RooArgSet( *mjj_, *JES_scale) );
  JES_scale2 = new RooRealVar("JES_scale2","", 0.0, -0.1, 0.1);
  shiftedMass = new RooFormulaVar("shiftedMass", "@0*(1.+@1)", RooArgSet( *mjj_, *JES_scale2) );
  shiftedMass2 = new RooFormulaVar("shiftedMass2", "@0*(1.+@1)", RooArgSet( *mjj_, *JES_scale2) );


  RooDataHist* rdh = new RooDataHist("rdh","", *mjj_, th1ww);
  RooAbsPdf* signalShapePdf_ = new RooHistPdf("signalShapePdf","",*shiftedMass2, *mjj_,*rdh);

  delete treeTemp1;
  delete treeTemp2;
  delete treeTemp3;
  delete treeTemp4;

  return signalShapePdf_;
}







// ***** Function to return the background Pdf **** //
// scale 1:  syst==0, scale up: syst==1, scale down: syst==2

RooAbsPdf* makeBkgPdf(int channel, char PLOTVAR[], int syst=0, char* cut="gdevtt")
{  
  // W+jets pdf
  TFile* wjetsShape_mu_file =  new TFile(MCDirectory + "RD_mu_WpJ_CMSSW428.root", "READ");
  treeTemp = (TTree*) wjetsShape_mu_file->Get("WJet");
  ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* tree1 = treeTemp->CopyTree(cut);

  TFile* wjetsShape_ele_file =  new TFile(MCDirectory + "RD_el_WpJ_CMSSW428.root", "READ");
  treeTemp = (TTree*) wjetsShape_ele_file->Get("WJet");
  ActivateTreeBranches(*treeTemp, true);
  gROOT->cd();
  TTree* tree2 = treeTemp->CopyTree(cut);


  char* weight = "1.0*";
  char* weight1 = "";
  char* weight2 = "";
  //------- for systematics: q2 up --------
  if(syst==1)  
     weight = "(1.262 - 0.007112*Mass2j_PFCor + 0.00005637*Mass2j_PFCor*Mass2j_PFCor - 0.0000001433*Mass2j_PFCor*Mass2j_PFCor*Mass2j_PFCor)*";


   //------- for systematics: q2 down --------
  if(syst==2)  
     weight = "(1.353 - 0.01078*Mass2j_PFCor + 0.00008975*Mass2j_PFCor*Mass2j_PFCor - 0.0000002139*Mass2j_PFCor*Mass2j_PFCor*Mass2j_PFCor)*";

  TString myselection = TString(weight) + TString(cut);


  TH1* th1wjets = new TH1D("th1wjets", "th1wjets",
			   NBINSFORPDF,MINRange,MAXRange);

  if(channel==0 || channel==1) 
     tree1->Draw(TString(PLOTVAR)+TString(">>th1wjets"), myselection, "goff");
  if(channel==0)
     tree2->Draw(TString(PLOTVAR)+TString(">>+th1wjets"), myselection, "goff");
  if(channel==2) 
     tree2->Draw(TString(PLOTVAR)+TString(">>th1wjets"), myselection, "goff");

  NMC_WpJ_=th1wjets->GetEntries();
  cout << "-------- Number of expected Wjj events = " << (31314./81352581.) * (th1wjets->Integral()) * IntLUMI << endl;

  RooDataHist* rdhWjets = new RooDataHist("rdhWjets","", *mjj_, th1wjets);
  RooAbsPdf* bkgShapePdf_ = new RooHistPdf("bkgShapePdf","",*shiftedMass, *mjj_,*rdhWjets);

  delete tree1;
  delete tree2;
  return bkgShapePdf_;
}








// ***** Function to return the Top pair Pdf **** //
RooAbsPdf* makeTopPairPdf(int channel, char PLOTVAR[], char* cut="gdevtt")
{  
  // top pair pdf
  TFile* ttbar_mu_file =  new TFile(MCDirectory + "RD_mu_TTbar_MG_CMSSW428.root", "READ");
  TTree* tree1 = (TTree*) ttbar_mu_file->Get("WJet");
  ActivateTreeBranches(*tree1);

  TFile* ttbar_ele_file =  new TFile(MCDirectory + "RD_el_TTbar_MG_CMSSW428.root", "READ");
  TTree* tree2 = (TTree*) ttbar_ele_file->Get("WJet");
  ActivateTreeBranches(*tree2, true);


  TH1* th1Top = new TH1D("th1Top", "th1Top", NBINSFORPDF, MINRange, MAXRange);
  // --------- cross section: 157.5 pb, events_gen = 3701947 (These are summer11 TTJets sample
  // --------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTop2011DataMCTrig
  TString myselection = TString("0.00004254518*") + TString(cut);
  if(channel==0 || channel==1) 
    tree1->Draw(TString(PLOTVAR)+TString(">>th1Top"), myselection, "goff");
  if(channel==0)
    tree2->Draw(TString(PLOTVAR)+TString(">>+th1Top"), myselection, "goff");
  if(channel==2) 
    tree2->Draw(TString(PLOTVAR)+TString(">>th1Top"), myselection, "goff");


  ttbarNorm_ = th1Top->Integral() * IntLUMI;
  cout << "-------- Number of expected ttbar events = " << 
    th1Top->Integral() << " x " << IntLUMI << " = " << ttbarNorm_ << endl;


  RooDataHist* rdhTop = new RooDataHist("rdhTop","", *mjj_, th1Top);
  RooAbsPdf* ttPdf_ = new RooHistPdf("ttPdf","",*shiftedMass, *mjj_,*rdhTop);

//   RooDataHist* rdhTop = new RooDataHist("rdhTop","", *mjj_, th1Top);
//   ttPdf_ = new RooHistPdf("ttPdf", "", *mjj_, *rdhTop);
  delete tree1;
  delete tree2;
  return ttPdf_;
}





// ***** Function to return the SingleTop Pdf **** //
RooAbsPdf* makeSingleTopPdf(int channel, char PLOTVAR[], char* cut="gdevtt")
{  
  // single top pdf (all samples are powheg, regardless of the unusual labeling scheme)
  TFile* st1_Tbar_mu_file =  new TFile(MCDirectory + "RD_mu_STopS_Tbar_CMSSW428.root", "READ");
  TTree* tree1Temp = (TTree*) st1_Tbar_mu_file->Get("WJet");
  ActivateTreeBranches(*tree1Temp);
  TFile* st1_Tbar_el_file =  new TFile(MCDirectory + "RD_el_STopS_Tbar_CMSSW428.root", "READ");
  TTree* tree2Temp = (TTree*) st1_Tbar_el_file->Get("WJet");
  ActivateTreeBranches(*tree2Temp, true);
  TFile* st1_T_mu_file =  new TFile(MCDirectory + "RD_mu_STopS_T_CMSSW428.root", "READ");
  TTree* tree3Temp = (TTree*) st1_T_mu_file->Get("WJet");
  ActivateTreeBranches(*tree3Temp);
  TFile* st1_T_el_file =  new TFile(MCDirectory + "RD_el_STopS_T_CMSSW428.root", "READ");
  TTree* tree4Temp = (TTree*) st1_T_el_file->Get("WJet");
  ActivateTreeBranches(*tree4Temp, true);

  TH1* th1st = new TH1D("th1st", "th1st",NBINSFORPDF,MINRange,MAXRange);
  // --------- cross section: Tbar - 1.44 pb, events_gen = 137980; T - 3.19 pb, events_gen = 259971
  // --------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma 
  TString Tbar_selection = TString("0.000010436295*") + TString(cut);
  TString T_selection = TString("0.0000122706*") + TString(cut);

  if(channel==0 || channel==1) {
    tree1Temp->Draw(TString(PLOTVAR)+TString(">>th1st"), Tbar_selection, "goff");
    tree3Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==0) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==2) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  delete tree1Temp;
  delete tree2Temp;
  delete tree3Temp;
  delete tree4Temp;


  TFile* st2_Tbar_mu_file =  new TFile(MCDirectory + "RD_mu_STopT_Tbar_CMSSW428.root", "READ");
  TTree* tree1Temp = (TTree*) st2_Tbar_mu_file->Get("WJet");
  ActivateTreeBranches(*tree1Temp);
  TFile* st2_Tbar_el_file =  new TFile(MCDirectory + "RD_el_STopT_Tbar_CMSSW428.root", "READ");
  TTree* tree2Temp = (TTree*) st2_Tbar_el_file->Get("WJet");
  ActivateTreeBranches(*tree2Temp, true);
  TFile* st2_T_mu_file =  new TFile(MCDirectory + "RD_mu_STopT_T_CMSSW428.root", "READ");
  TTree* tree3Temp = (TTree*) st2_T_mu_file->Get("WJet");
  ActivateTreeBranches(*tree3Temp);
  TFile* st2_T_el_file =  new TFile(MCDirectory + "RD_el_STopT_T_CMSSW428.root", "READ");
  TTree* tree4Temp = (TTree*) st2_T_el_file->Get("WJet");
  ActivateTreeBranches(*tree4Temp, true);
  // --------- cross section: Tbar - 22.65 pb, events_gen = 1944826; T - 41.92 pb, events_gen = 3900171
  Tbar_selection = TString("0.000011646286*") + TString(cut);
  T_selection = TString("0.000010748247*") + TString(cut);
  if(channel==0 || channel==1) {
    tree1Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree3Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==0) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==2) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  delete tree1Temp;
  delete tree2Temp;
  delete tree3Temp;
  delete tree4Temp;


  TFile* st3_Tbar_mu_file =  new TFile(MCDirectory + "RD_mu_STopTW_Tbar_CMSSW428.root", "READ");
  TTree* tree1Temp = (TTree*) st3_Tbar_mu_file->Get("WJet");
  ActivateTreeBranches(*tree1Temp);
  TFile* st3_Tbar_el_file =  new TFile(MCDirectory + "RD_el_STopTW_Tbar_CMSSW428.root", "READ");
  TTree* tree2Temp = (TTree*) st3_Tbar_el_file->Get("WJet");
  ActivateTreeBranches(*tree2Temp, true);
  TFile* st3_T_mu_file =  new TFile(MCDirectory + "RD_mu_STopTW_T_CMSSW428.root", "READ");
  TTree* tree3Temp = (TTree*) st3_T_mu_file->Get("WJet");
  ActivateTreeBranches(*tree3Temp);
  TFile* st3_T_el_file =  new TFile(MCDirectory + "RD_el_STopTW_T_CMSSW428.root", "READ");
  TTree* tree4Temp = (TTree*) st3_T_el_file->Get("WJet");
  ActivateTreeBranches(*tree4Temp, true);
  // --------- cross section: Tbar - 7.87 pb, events_gen = 787629; T - 7.87 pb, events_gen = 795379
  Tbar_selection = TString("0.000009992014*") + TString(cut);
  T_selection = TString("0.000009894654*") + TString(cut);
  if(channel==0 || channel==1) {
    tree1Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree3Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==0) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }
  if(channel==2) {
    tree2Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), Tbar_selection, "goff");
    tree4Temp->Draw(TString(PLOTVAR)+TString(">>+th1st"), T_selection, "goff");
  }




  singleTopNorm_ = th1st->Integral() * IntLUMI;
  cout << "-------- Number of expected single top events = " << 
    th1st->Integral() << " x " << IntLUMI << " = " << singleTopNorm_ << endl;


  RooDataHist* rdhst = new RooDataHist("rdhst","", *mjj_, th1st);
  RooAbsPdf* singleTopPdf_ = new RooHistPdf("singleTopPdf","",*shiftedMass, *mjj_,*rdhst);


//   RooDataHist* rdhst = new RooDataHist("rdhst","", *mjj_, th1st);
//   singleTopPdf_ = new RooHistPdf("singleTopPdf", "", *mjj_, *rdhst);
//   delete treeTemp;
  delete tree1Temp;
  delete tree2Temp;
  return singleTopPdf_;
}


// ***** Function to return the QCD Pdf **** //
RooAbsPdf* makeQCDPdf(int channel, char PLOTVAR[], char* cut="gdevtt")
{  
  // QCD pdf
  TFile* fqcd0 =  new TFile(QCDDirectory + "/RD_mu_QCDMu_CMSSW428_MET20Iso03.root", "READ");
  TTree* tree10 = (TTree*) fqcd0->Get("WJet");
  TFile* fqcd1 =  new TFile(QCDDirectory + "/RD_el_QCDEl_Pt30to80_CMSSW428_MET20Iso03.root", "READ");
  TTree* tree11 = (TTree*) fqcd1->Get("WJet");
  TFile* fqcd2 =  new TFile(QCDDirectory + "/RD_el_QCDEl_Pt80to170_CMSSW428_MET20Iso03.root", "READ");
  TTree* tree12 = (TTree*) fqcd2->Get("WJet");
  TFile* fqcd3 =  new TFile(QCDDirectory + "/RD_el_QCDEl_BCtoE30to80_CMSSW428_MET20Iso03.root", "READ");
  TTree* tree13 = (TTree*) fqcd3->Get("WJet");
  TFile* fqcd4 =  new TFile(QCDDirectory + "/RD_el_QCDEl_BCtoE80to170_CMSSW428_MET20Iso03.root", "READ");
  TTree* tree14 = (TTree*) fqcd4->Get("WJet");

  //// Scaling Coefficients = 84679.3/25080241, 3866200/70392060, 139500/2194800, 136804/2030033, 9360/1082691 = 3.37633517955429532e-03, 5.49238081681371476e-02, 6.35593220338983023e-02, 6.73900375018534198e-02, 8.64512589464584008e-03 
  char scale[50];
  TH1* th1qcdMu = new TH1D("th1qcdMu", "th1qcdMu", 0.5*NBINSFORPDF, MINRange, MAXRange);
  //sprintf(scale, "%f", 3.37633517955429532e-03);
  myselection = TString("0.00337633517955429532*") + TString(cut);
  tree10->Draw( TString(PLOTVAR)+TString(">>th1qcdMu"), myselection, "goff");

  TH1* th1qcdEle = new TH1D("th1qcdEle", "th1qcdEle", NBINSFORPDF, MINRange, MAXRange);
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
    th1qcd = (TH1D* ) th1qcdMu->Clone("th1qcd");
  else th1qcd = (TH1D* ) th1qcdEle->Clone("th1qcd");
  if(channel==0) th1qcd->Add(th1qcdEle);

  //     cout << " ----- num. all QCD events = " << th1qcd->Integral() << endl;
  //     cout << " ----- num. mu QCD events = " << th1qcdMu->Integral() << endl;
  //     cout << " ----- num. ele QCD events = " << th1qcdEle->Integral() << endl;

  th1qcd->Scale( 1./ th1qcd->Integral() );

  RooDataHist* rdhqcd = new RooDataHist("rdhqcd","", *mjj_, th1qcd);
  RooAbsPdf* qcdPdf_ = new RooHistPdf("qcdPdf","",*shiftedMass, *mjj_,*rdhqcd);

//   RooDataHist* rdhqcd = new RooDataHist("rdhqcd","", *mjj_, th1qcd);
//   qcdPdf_ = new RooHistPdf("qcdPdf", "", *mjj_, *rdhqcd);

  delete tree10;
  delete tree11;
  delete tree12;
  delete tree13;
  delete tree14;
  delete fqcd0;
  delete fqcd1;
  delete fqcd2;
  delete fqcd3;

  return qcdPdf_;
}




// ***** Function to return the Z+jets Pdf **** //
RooAbsPdf* makeZJetsPdf(int channel, char PLOTVAR[], char* cut="gdevtt")
{  
  TFile* zjets_mu_file =  new TFile(MCDirectory + "RD_mu_ZpJ_CMSSW428.root", "READ");
  TTree* tree1 = (TTree*) zjets_mu_file->Get("WJet");
  ActivateTreeBranches(*tree1);

  TFile* zjets_ele_file =  new TFile(MCDirectory + "RD_el_ZpJ_CMSSW428.root", "READ");
  TTree* tree2 = (TTree*) zjets_ele_file->Get("WJet");
  ActivateTreeBranches(*tree2, true);


  TH1* th1ZJets = new TH1D("th1ZJets", "th1ZJets", NBINSFORPDF, MINRange, MAXRange);
  // ----------LO cross section: 5635.6 * 0.4381 pb, events_gen = 36277961 => weight = 0.000068056646
  // ---------- https://twiki.cern.ch/twiki/bin/view/CMS/MadGraphSummer11Production
  // ----------NNLO cross section: 3048 pb , events_gen = 36277961 => weight = 0.000084017952387
  // ---------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTop2011DataMCTrig 
  // ----------- https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSections

  TString myselection = TString("0.000084017952387*") + TString(cut);

  if(channel==0 || channel==1) 
    tree1->Draw(TString(PLOTVAR)+TString(">>th1ZJets"), myselection, "goff");
  if(channel==0)
    tree2->Draw(TString(PLOTVAR)+TString(">>+th1ZJets"), myselection, "goff");
  if(channel==2) 
    tree2->Draw(TString(PLOTVAR)+TString(">>th1ZJets"), myselection, "goff");


  zjetsNorm_ = th1ZJets->Integral() * IntLUMI;
  cout << "-------- Number of expected zjets events = " << 
    th1ZJets->Integral() << " x " << IntLUMI << " = " << zjetsNorm_ << endl;


  RooDataHist* rdhZJets = new RooDataHist("rdhZJets","", *mjj_, th1ZJets);
  RooAbsPdf* zjetsPdf_ = new RooHistPdf("zjetsPdf","",*shiftedMass, *mjj_,*rdhZJets);

//   RooDataHist* rdhZJets = new RooDataHist("rdhZJets","", *mjj_, th1ZJets);
//   zjetsPdf_ = new RooHistPdf("zjetsPdf", "", *mjj_, *rdhZJets);
  delete tree1;
  delete tree2;

  return zjetsPdf_;
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
      latex.DrawLatex(0.82,0.7,Form("#int #font[12]{L} dt = %.1f fb^{-1}", LUMINOSITY/1000));
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




void ActivateTreeBranches(TTree& t, bool isElectronTree=false) {
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
  t.SetBranchStatus("event_RhoForLeptonIsolation",    1);
  t.SetBranchStatus("event_nPV",    1);

  t.SetBranchStatus("W_mt",    1);
  t.SetBranchStatus("W_pt",    1);
  t.SetBranchStatus("W_pzNu1",    1);
  t.SetBranchStatus("W_pzNu2",    1);
  t.SetBranchStatus("fit_mlvjj",    1);
  t.SetBranchStatus("fi2_mlvjj",    1);
  t.SetBranchStatus("fit_status",    1);
  t.SetBranchStatus("gdevtt",    1);
  t.SetBranchStatus("fit_chi2",    1);
  t.SetBranchStatus("fit_NDF",    1);
  t.SetBranchStatus("fi2_chi2",    1);
  t.SetBranchStatus("fi2_NDF",    1);
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


