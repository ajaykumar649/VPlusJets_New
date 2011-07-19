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

// #ifndef RooWjjFitter_h
// #define RooWjjFitter_h

// ROOT
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
// #include <TLegend.h>
// #include <RooCurve.h>


const char* PLOTPREFIX = "mLnuJJ";
//const char* PLOTVAR = "MassV2j_PFCor";
const char* PLOTVAR = "fit_mlvjj";
const char* XLABEL = "m_{l#nujj}";
const double MINRange = 140.0;
const double MAXRange = 620.0;
const int BINWIDTH = 20;
const bool includeHiggs = true;
const bool IncludeTopSingleTop = false;


const float IntLUMI = 935.0;
const float keysrho = 3.25;
const RooKeysPdf::Mirror mirrorOption = RooKeysPdf::NoMirror;
const double WmassWindowMIN = 65.0;
const double WmassWindowMAX = 95.0;



const int mHIGGS = 600;
const bool truncateFitRange = false;
const int NBINSFORPDF = (int)((MAXRange-MINRange)/BINWIDTH);
const bool USEKEYSPDF = false;
const char* mJJCutFormLvJJFit = "Mass2j_PFCor>65. && Mass2j_PFCor<95.";
//const char* mJJCutFormLvJJFit = "";
//const char* mJJCutFormLvJJFit = "Mass2j_PFCor<65.";
//const char* mJJCutFormLvJJFit = "Mass2j_PFCor>95.";
//const char* mJJCutFormLvJJFit = "Mass2j_PFCor>95. && Mass2j_PFCor<120.";
//const char* mJJCutFormLvJJFit = "Mass2j_PFCor>180.";
//const char* mJJCutFormLvJJFit = "Mass2j_PFCor<65. && Mass2j_PFCor>30.";
//const char* mJJCutFormLvJJFit = "((Mass2j_PFCor<65.) || ((Mass2j_PFCor>95.)&&(Mass2j_PFCor<120)) || (Mass2j_PFCor>180.))";

const char* jetCut;
const char* muCut;
const char* eleCut;

jetCut = "abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5 && cosJacksonAngle2j_PFCor>-0.6 && cosJacksonAngle2j_PFCor<0.8 && JetPFCor_Pt[2] < 0.1 &&"
   "sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>40. && "
  "abs(JetPFCor_Pt[1]/Mass2j_PFCor-0.5)<0.2 && "
  "W_mt>40. && JetPFCor_Pt[0]>30."
  " && JetPFCor_Pt[1]>30. && JetPFCor_Pt[2]<0.1"
  " && JetPFCor_bDiscriminator[0]<1.19"
  " && JetPFCor_bDiscriminator[1]<1.19 && fit_status==0";
// " && fit_status==0 && gdevtt"


muCut = "W_muon_pt>25. && abs(W_muon_eta)<2.1 && event_met_pfmet>30."
  " && (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt<0.1"" && abs(W_muon_d0bsp)<0.02";

eleCut = "W_electron_et>30.&& event_met_pfmet>30."
   " && (W_electron_trackiso+W_electron_hcaliso+W_electron_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_electron_pt<0.05"
   " && W_electron_isWP80"
   " && ((abs(W_electron_eta)<1.5 &&"
   " abs(W_electron_deltaphi_in)<0.03"
   " && abs(W_electron_deltaeta_in)<0.004)"
   " || (abs(W_electron_eta)>1.5"
   " && abs(W_electron_deltaphi_in)<0.02"
  " && abs(W_electron_deltaeta_in)<0.005))"
  " && sqrt((W_electron_vx-event_BeamSpot_x)**2+(W_electron_vy-event_BeamSpot_y)**2)<0.02";




TCut* jetsSel;
TCut* mu_selection;
TCut* ele_selection;



RooRealVar *mjj_;
RooRealVar *massShift;
RooAbsReal *shiftedMass;
RooRealVar *reso_;
RooRealVar *zero_;
RooGaussModel *resModel_;

RooRealVar *mg;
RooRealVar *sg;


RooAbsPdf* signalShapePdf_;
RooAbsPdf* signalShape_;
RooAbsPdf* signalShapePdfele_;
RooAbsPdf *bkgShapePdfele_;
RooAbsPdf *bkgShape_;
RooAbsPdf *bkgShapePdf_;
RooAbsPdf *gauss;
RooAbsPdf *ttPdf_;
RooAbsPdf *stSPdf_;
RooAbsPdf *stTPdf_;
RooAbsPdf *stTWPdf_;
RooAbsPdf *singleTopPdf_;
RooAbsPdf *mHShapePdf_;

TFile* wjetsShape_mu_file;
TFile* wjetsShape_ele_file;
TFile* wwShape_mu_file;
TFile* wwShape_ele_file;
TFile* ttbar_mu_file;
TFile* st1_mu_file;
TFile* st2_mu_file;
TFile* st3_mu_file;
TFile* mhfile;

TCanvas *c;
TCanvas *c1;
TCanvas *c2;


using namespace RooFit;

///////// --------- channel 0 : combined,  1: mu,    2: ele --------------
void RooWjj4BodyFitter(int channel=0)
{
 jetsSel = new TCut (jetCut);
 mu_selection = new TCut ( (*jetsSel) && TCut(muCut) );
 ele_selection = new TCut ( (*jetsSel) && TCut(eleCut) );


   // The fit variable - lepton invariant mass
   mjj_ = new RooRealVar( PLOTVAR, XLABEL, MINRange, MAXRange, "GeV");
   // Set #bins to be used for FFT sampling to 10000
   // mjj_->setBins( 10000,"cache"); 


   RooRealVar Mass = *mjj_;
   //RooRealVar Mass = *shiftedMass;


   TCut massCut = "";
   massCut = mJJCutFormLvJJFit;

   *mu_selection = TCut( *mu_selection && massCut);
   *ele_selection = TCut( *ele_selection && massCut);

 
   gROOT->cd();
   char temp[50];
   TFile fin("data/ReducedTree/RD_WmunuJets_DataAll_GoldenJSON_953invpb.root", "read");
   TTree* treeTemp = (TTree*) fin.Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* treemu = treeTemp->CopyTree( *mu_selection);


   TFile fin2("data/ReducedTree/RD_WenuJets_DataAll_GoldenJSON_917invpb.root", "read");
   TTree* treeTemp2 = (TTree*) fin2.Get("WJet");
   ActivateTreeBranches(*treeTemp2, true);
   gROOT->cd();
   TTree* treeele = treeTemp2->CopyTree( *ele_selection);


   ///////// convert Histograms into RooDataHists
   TTree* treeForDataSet = treemu;
   if(channel == 2) treeForDataSet = treeele;
   RooDataSet* data = new RooDataSet("data","data", treeForDataSet, Mass);
   RooDataSet* data_ele = new RooDataSet("data_ele","data_ele",treeele, Mass);
  
   if(channel == 0) data->append(*data_ele);
   cout << "Made dataset" << endl;




   // ********** Construct signal & bkg shape PDF ********** //
   makeSignalPdf();
   cout << "Made signal pdf" << endl;
   makeBkgPdf(channel);
   if(IncludeTopSingleTop)
     makeTTbarPdf(channel);
   cout << "Made bkg pdf" << endl;
   if(includeHiggs) {
     makeHiggsPdf(channel);
     cout << "Made bkg pdf" << endl;
   }

   // Define background yield variables: they are not related to each other  
   RooRealVar nBkg("nBkg","nBkg",  25000.,     0.0,   500000.);
   RooRealVar nSig("nSig","nSig",  1000.,      0.0,   5000.);
   RooRealVar nHiggs("nHiggs","nHiggs", 300.,  100.0,   500.);
   // fix the top and single top normalization
//    double NUMTOP = IntLUMI*0.2;
//    double NUMSINGLETOP = IntLUMI*0.4;

   double NUMTOP = IntLUMI*0.015;
   double NUMSINGLETOP = NUMTOP;
   if( strcmp(PLOTPREFIX, "mLnuJJ") ==0 ) {
     NUMTOP *= 0.5;
     NUMSINGLETOP *= 0.5;
   }
   if(channel > 0) {
     NUMTOP *= 0.5;
     NUMSINGLETOP *= 0.5;
   }
   RooRealVar nTTbar("nTTbar","", NUMTOP);
   RooRealVar normSingleTop("normSingleTop","", NUMSINGLETOP);
   RooFormulaVar nSTschannel("nSTschannel", "0.046* normSingleTop", RooArgList(normSingleTop));
   RooFormulaVar nSTtchannel("nSTtchannel", "0.628 * 0.32442* normSingleTop", RooArgList(normSingleTop));
   RooFormulaVar nSTtwchannel("nSTtwchannel", "0.326 * normSingleTop", RooArgList(normSingleTop));
   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////
   RooArgList* components;
   RooArgList* yields;	 
   if(IncludeTopSingleTop) {
     if(includeHiggs) {
       components = new RooArgList(*signalShapePdf_,*bkgShapePdf_, *ttPdf_, *stSPdf_,*stTPdf_,*stTWPdf_,*mHShapePdf_);
       yields = new RooArgList(nSig, nBkg, nTTbar, nSTschannel, nSTtchannel, nSTtwchannel, nHiggs);	 
     }
     else {
       components = new RooArgList(*signalShapePdf_,*bkgShapePdf_, *ttPdf_, *stSPdf_, *stTPdf_, *stTWPdf_);
       yields = new RooArgList(nSig, nBkg, nTTbar, nSTschannel, nSTtchannel, nSTtwchannel);
     }
   }
   else {
     if(includeHiggs) {
       components =new RooArgList(*signalShapePdf_,*bkgShapePdf_,*mHShapePdf_);
       yields = new RooArgList(nSig, nBkg, nHiggs);	 
     }
     else {
       components = new RooArgList(*signalShapePdf_,*bkgShapePdf_);
       yields = new RooArgList(nSig, nBkg);
     }
   }
   RooAddPdf totalPdf("totalPdf","extended sum pdf", *components, *yields);



   Mass.setRange("Range200To250", 200, 250) ;
   Mass.setRange("RangeBelow200", 170, 200) ;
   Mass.setRange("RangeAbove200", 200, 550) ;
   Mass.setRange("RangeAbove150", MINRange, MAXRange) ;
   Mass.setRange("RangeForPlot", MINRange, MAXRange) ;
   Mass.setRange("RangeDefault", MINRange, MAXRange) ;


   // ********* Do the Actual Fit ********** //  
   //const char* rangeString = "Range55To250";
   char* rangeString = "RangeDefault";
   if(truncateFitRange && strcmp(PLOTPREFIX, "mLnuJJ") ==0 ) 
      rangeString = "RangeBelow200,RangeAbove250";
   else rangeString = "RangeAbove200";

   RooFitResult *fitResult;
   double nSigConst = 744.; 
   double nSigConstError = 95.; 
   // double nBkgConst = 8.84*nSigConst; 
   double nBkgConst = 5706.; 
   double nBkgConstError = 108.; 

   if(channel == 1) {
     nSigConst *= 0.5; 
     nSigConstError = 40.; 
     nBkgConst *= 0.5; 
     nBkgConstError = 70.;
   }
   if(channel == 2) {
     nSigConst *= 0.5; 
     nSigConstError = 40.; 
     nBkgConst *= 0.5; 
     nBkgConstError = 70.;
   }


   RooGaussian constNsig("constNsig","",nSig,RooConst(nSigConst),
			 RooConst(nSigConstError)) ;
   RooGaussian constNbkg("constNbkg","",nBkg,RooConst(nBkgConst),
			 RooConst(nBkgConstError)) ;


   RooGaussian constJES("constJES","", *massShift, RooConst(0.0),
   RooConst(0.1)) ;


     nBkg.setVal(nBkgConst);
     nBkg.setConstant( kTRUE );
     nSig.setVal(nSigConst);
     nSig.setConstant( kTRUE );   

     RooCmdArg* argConstrain1;
     if(!includeHiggs) argConstrain1 = new RooCmdArg(RooFit::ExternalConstraints(constJES));
     else { 
       massShift->setVal( 0.0 );
       massShift->setConstant( kTRUE );
       argConstrain1 = RooCmdArg::none();
     }

     fitResult = totalPdf.fitTo(*data, Save(true), 
				*argConstrain1,
				RooFit::Extended(true), 
				//RooFit::Minos(true), 
				PrintEvalErrors(-1),
				RooFit::Range(rangeString),
				Warnings(false) 
				);


// if( strcmp(PLOTPREFIX, "mLnuJJ") ==0 ) 
//   fitResult = totalPdf.fitTo(*data, Save(true), 
// 			     RooFit::Extended(true), 
// 			     ExternalConstraints(constNbkg),
// 			     RooFit::Minos(true), 
// 			     PrintEvalErrors(-1),
// 			     RooFit::Range(rangeString),
// 			     Warnings(false) 
// 			     );



   fitResult->Print("v");

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
   sprintf(temp, "-mH%d", mHIGGS);
   if(includeHiggs) cname = cname + TString(temp);
   c = new TCanvas(cname,cname,500,500);
   RooPlot* frame1 = Mass.frame( MINRange, MAXRange, 
				 (int) ((MAXRange-MINRange)/BINWIDTH) );
   data->plotOn(frame1,RooFit::DataError(errorType));
   totalPdf.plotOn(frame1,ProjWData(*data),
		   Components(*signalShapePdf_),
		   DrawOption("LF"),FillStyle(1001),
		   FillColor(kOrange), LineColor(kOrange),
		   Name("h_diboson"),Range("RangeForPlot"));
   if(includeHiggs) 
     totalPdf.plotOn(frame1,ProjWData(*data),
		     Components(*mHShapePdf_),
		     DrawOption("LF"),FillStyle(1001),
		     LineColor(kGray+1),FillColor(kGray+1),
		     Name("h_higgs"), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Name("h_total"), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components(*bkgShapePdf_), 
		   LineColor(kRed), Name("h_Wjets"), Range("RangeForPlot"));
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components("bkgShapePdf,ttPdf,stSPdf,stTPdf,stTWPdf"), 
		   Name("h_Background"), Range("RangeForPlot"),Invisible());
   totalPdf.plotOn(frame1,ProjWData(*data), 
		   Components("signalShapePdf,bkgShapePdf,ttPdf,stSPdf,stTPdf,stTWPdf"), 
		   Name("h_SM"), Range("RangeForPlot"),
		   Invisible());
   if(IncludeTopSingleTop)
     totalPdf.plotOn(frame1,ProjWData(*data), 
		     Components("ttPdf, stSPdf, stTPdf, stTWPdf"),
		     LineColor(kBlack), Name("h_Top"), 
		     Range("RangeForPlot"));
   

   data->plotOn(frame1,RooFit::DataError(errorType));
   frame1->SetMinimum(0);
   frame1->GetXaxis()->SetNdivisions(505);
   frame1->SetMaximum(1.25* frame1->GetMaximum());
   frame1->Draw("e0");
   TPaveText *plotlabel4 = new TPaveText(0.55,0.87,0.85,0.92,"NDC");
   plotlabel4->SetTextColor(kBlack);
   plotlabel4->SetFillColor(kWhite);
   plotlabel4->SetBorderSize(0);
   plotlabel4->SetTextAlign(12);
   plotlabel4->SetTextSize(0.03);
   TPaveText *plotlabel5 = new TPaveText(0.55,0.82,0.85,0.87,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   TPaveText *plotlabel6 = new TPaveText(0.55,0.77,0.85,0.82,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   // double singleTopN = 4.6*0.32442 + 63.0 * 0.32442 + 10.6;
   if(!includeHiggs) {
     sprintf(temp, "Di-boson = %d (fixed)", nSig.getVal());
     plotlabel4->AddText(temp);
     sprintf(temp, "W + jets = %d (fixed)", nBkg.getVal());
     plotlabel5->AddText(temp);
     plotlabel4->Draw();
     plotlabel5->Draw();
   }
   else {
      sprintf(temp, "Higgs %d = %.1f #pm %.1f", mHIGGS, nHiggs.getVal(), nHiggs.getPropagatedError(*fitResult));
      plotlabel4->AddText(temp);
      plotlabel4->Draw();
      sprintf(temp, "Di-boson = %d #pm %d", nSig.getVal(), nSig.getPropagatedError(*fitResult));
      plotlabel5->AddText(temp);
      plotlabel5->Draw();
      sprintf(temp, "W + jets = %d #pm %d", nBkg.getVal(), nBkg.getPropagatedError(*fitResult));
      plotlabel6->AddText(temp);
      plotlabel6->Draw();
   }
//   cout << "======= chi^2/dof = " << frame1->chiSquare() << endl;

   cmsPrelim2();
   // TLegend* legend = new TLegend(0.35,0.35,0.55,0.55);
   TLegend* legend = new TLegend(0.65,0.5,0.85,0.7);
   RooHist* datahist = frame1->getHist("h_data");
   RooCurve* dibosonhist = frame1->getCurve("h_diboson");
   RooCurve* wjetshist = frame1->getCurve("h_Wjets");
   RooCurve* tophist = frame1->getCurve("h_Top");
   RooCurve* tophist;
   if(includeHiggs) RooCurve* higgshist = frame1->getCurve("h_higgs");

   legend->AddEntry( datahist, "Data", "P");  
   legend->AddEntry( dibosonhist, "Di-boson", "F");
   legend->AddEntry( wjetshist, "W+jets", "L");
   if(IncludeTopSingleTop)
     legend->AddEntry( tophist, "t#bar{t}, Top", "L");
   if(includeHiggs) {
     sprintf(temp, "Higgs %d", mHIGGS);
     legend->AddEntry( higgshist, temp, "F");
   }

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
   //-------Make logY plots ---------------------------------
   TString cname = TString(PLOTPREFIX) + TString("-combined-fit-logY");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-logY");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-logY");
   if(truncateFitRange) cname = cname + TString("-truncated");
   sprintf(temp, "-mH%d", mHIGGS);
   if(includeHiggs) cname = cname + TString(temp);
   c1 = new TCanvas(cname,cname,500,500);
   frame1->SetMinimum(0.5);
   frame1->SetMaximum(20000);
   frame1->Draw("e0");
   plotlabel4->Draw();
   plotlabel5->Draw();
   if(includeHiggs) plotlabel6->Draw();
   cmsPrelim2();
   TLegend* legend = new TLegend(0.6,0.62,0.85,0.82);
   legend->AddEntry( datahist, "Data", "P");  
   legend->AddEntry( dibosonhist, "Di-boson", "F");
   legend->AddEntry( wjetshist, "W+jets", "L");
   if(IncludeTopSingleTop) 
     legend->AddEntry( tophist, "t#bar{t}, Top", "L");
   if(includeHiggs) {
     sprintf(temp, "Higgs %d", mHIGGS);
     legend->AddEntry( higgshist, temp, "F");
   }
   legend->SetFillColor(0);
   legend->Draw();
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
   //-------- Create a new frame to draw the  data - (W+jets) residual distribution 
   //--------  and add the distribution to the frame
   RooPlot* frame2 = Mass.frame( MINRange, MAXRange, (int) ((MAXRange-MINRange)/BINWIDTH)); 
   data->plotOn(frame2,MarkerStyle(0),MarkerSize(0),MarkerColor(0),LineColor(0),LineWidth(0));
   totalPdf.plotOn(frame2,ProjWData(*data),Components(*signalShapePdf_),DrawOption("LF"),FillStyle(1001),FillColor(kOrange),Name("h_diboson"), Range("RangeForPlot"));
   if(includeHiggs)   
     totalPdf.plotOn(frame2,ProjWData(*data),Components(*mHShapePdf_),DrawOption("LF"),FillStyle(1001),FillColor(kGray+1),Name("h_higgs"), Range("RangeForPlot"));

   //// Construct a histogram with the residuals of the data w.r.t. the curve
   RooHist* hresid = frame1->residHist("h_data", "h_Background") ;
   hresid->SetMarkerSize(0.8);
   frame2->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
   frame2->SetMaximum(200);
   frame2->SetMinimum(-100);
   frame2->GetXaxis()->SetNdivisions(505);
   cname = TString(PLOTPREFIX) + TString("-combined-fit-subtractedWjets");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-subtractedWjets");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-subtractedWjets");
   if(truncateFitRange) cname = cname + TString("-truncated");
   sprintf(temp, "-mH%d", mHIGGS);
   if(includeHiggs) cname = cname + TString(temp);
   c2 = new TCanvas(cname,cname,500,500);
   frame2->Draw() ;
   hresid->Draw("P");
   cmsPrelim2();
   TLegend* legend2 = new TLegend(0.6,0.8,0.85,0.9);
   legend2->AddEntry( datahist, "Data", "P");  
   if(includeHiggs) {
     sprintf(temp, "Higgs %d", mHIGGS);
     legend2->AddEntry( higgshist, temp, "F");
   }
   else legend2->AddEntry( dibosonhist, "Di-boson", "F");
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
   if(includeHiggs)   
     totalPdf.plotOn(frame3,ProjWData(*data),Components(*mHShapePdf_),DrawOption("LF"),FillStyle(1001),FillColor(kGray+1),Name("h_higgs"), Range("RangeForPlot"));   
   //// Construct a histogram with the residuals of the data w.r.t. the curve
   hresid = frame1->residHist("h_data", "h_SM") ;
   hresid->SetMarkerSize(0.8);
   frame3->GetYaxis()->SetTitle(frame1->GetYaxis()->GetTitle());
   frame3->SetMaximum(200);
   frame3->SetMinimum(-100);
   frame3->GetXaxis()->SetNdivisions(505);
   cname = TString(PLOTPREFIX) + TString("-combined-fit-subtracted");
   if(channel==1) cname = TString(PLOTPREFIX) + TString("-mu-fit-subtracted");
   if(channel==2) cname = TString(PLOTPREFIX) + TString("-ele-fit-subtracted");
   if(truncateFitRange) cname = cname + TString("-truncated");
   sprintf(temp, "-mH%d", mHIGGS);
   if(includeHiggs) cname = cname + TString(temp);
   c3 = new TCanvas(cname,cname,500,500);
   frame3->Draw() ;
   hresid->Draw("P");
   cmsPrelim2();
   TLegend* legend2 = new TLegend(0.6,0.8,0.85,0.9);
   legend2->AddEntry( datahist, "Data", "P");  
   if(includeHiggs) {
     sprintf(temp, "Higgs %d", mHIGGS);
     legend2->AddEntry( higgshist, temp, "F");
   }
   legend2->SetFillColor(0);
   legend2->Draw();
   c3->SaveAs( cname + TString(".eps"));
   c3->SaveAs( cname + TString(".gif"));
   c3->SaveAs( cname + TString(".root"));
   c3->SaveAs( cname + TString(".png"));
   c3->SaveAs( cname + TString(".C"));
   c3->SaveAs( cname + TString(".pdf"));


   //    if(data) delete data;
   //    if(c) delete c;
}





// // ***** Function to return the signal Pdf *** //
void makeSignalPdf() {

  TTree* treeTemp;
  wwShape_mu_file =  new TFile("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_WWtoAnything.root", "READ");
   treeTemp = (TTree*) wwShape_mu_file->Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* tree1 = treeTemp->CopyTree(*mu_selection);
   cout << "Events passing all selection in WW->mu tree = " << tree1->GetEntries() << endl;

 
   wwShape_ele_file =  new TFile("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_WZtoAnything.root", "READ");
   treeTemp = (TTree*) wwShape_ele_file->Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* tree2 = treeTemp->CopyTree(*mu_selection);
   cout << "Events passing all selection in WZ->mu tree = " << tree2->GetEntries() << endl;


   if(USEKEYSPDF) {
     RooDataSet* rds = new RooDataSet("rds","", tree1, *mjj_);
     RooDataSet* rds2 = new RooDataSet("rds2","", tree2, *mjj_);
     rds->append(*rds2);
     signalShapePdf_ = new RooKeysPdf("signalShapePdf", "", *mjj_, *rds, 
				      mirrorOption, keysrho);
   }
   else {
     TH1* th1ww = new TH1D("th1ww", "th1ww", NBINSFORPDF, MINRange, MAXRange);
     tree1->Draw( TString(PLOTVAR)+TString(">>th1ww"), *mu_selection, "goff");
     tree2->Draw(TString(PLOTVAR)+TString(">>+th1ww"), "", "goff");

     massShift = new RooRealVar("massShift","",-1., -10., 10.);
     shiftedMass = new RooFormulaVar("shiftedMass", "@0+@1", 
     RooArgSet( *mjj_, *massShift) );

     RooDataHist* rdh = new RooDataHist("rdh","", *mjj_, th1ww);
     signalShapePdf_ = new RooHistPdf("signalShapePdf","",*shiftedMass, *mjj_,*rdh);
   }
   delete tree1;
   delete tree2;
   delete treeTemp;
}





// ***** Function to return the Higgs Pdf **** //
void makeHiggsPdf(int channel)
{  
   char temp[100];
   sprintf(temp, "data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-%d.root", mHIGGS);
   mhfile = new TFile( temp, "read");
   TTree* treeTemp = (TTree*) mhfile->Get("WJet");
   ActivateTreeBranches(*treeTemp);
   TTree* tree = treeTemp->CopyTree(*mu_selection);
   if(USEKEYSPDF) {
     RooDataSet* rds = new RooDataSet("rds","", tree, *mjj_);
     mHShapePdf_ = new RooKeysPdf("mHShapePdf", "", *mjj_, *rds, 
				  mirrorOption, keysrho);
   }
   else {
     TH1* th1H = new TH1D("th1H", "th1H", NBINSFORPDF, MINRange, MAXRange);
     tree->Draw( TString(PLOTVAR)+TString(">>th1H"), *mu_selection, "goff");
     RooDataHist* rdhH = new RooDataHist("rdhH","", *mjj_, th1H);
     mHShapePdf_ = new RooHistPdf("mHShapePdf", "", *mjj_, *rdhH);
   }
   delete tree;
   delete treeTemp;
}







// ***** Function to return the background Pdf **** //
void makeBkgPdf(int channel)
{  
  // W+jets pdf
  TCut sidebands("((Mass2j_PFCor>95. && Mass2j_PFCor<125.) || (Mass2j_PFCor>35. && Mass2j_PFCor<65.))");
  //   wjetsShape_mu_file =  new TFile("data/ReducedTree/RD_WmunuJets_CMSSW423-Summer11MC_WJets.root", "READ");
  wjetsShape_mu_file =  new TFile("data/ReducedTree/RD_WmunuJets_DataAll_GoldenJSON_953invpb.root", "READ");

   treeTemp = (TTree*) wjetsShape_mu_file->Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* tree1 = treeTemp->CopyTree( (*jetsSel) && TCut(muCut) && sidebands);

   //    wjetsShape_ele_file =  new TFile("data/ReducedTree/RD_WenuJets_CMSSW423-Summer11MC_WJets.root", "READ");
   wjetsShape_ele_file =  new TFile("data/ReducedTree/RD_WenuJets_DataAll_GoldenJSON_917invpb.root", "READ");
   treeTemp = (TTree*) wjetsShape_ele_file->Get("WJet");
   ActivateTreeBranches(*treeTemp, true);
   gROOT->cd();
   TTree* tree2 = treeTemp->CopyTree( (*jetsSel) && TCut(eleCut) && sidebands);

   if(USEKEYSPDF) {
     RooDataSet* rdsb = new RooDataSet("rdsb","", tree1, *mjj_);
     RooDataSet* rdsb2 = new RooDataSet("rdsb2","", tree2, *mjj_);
     rdsb->append(*rdsb2);
     bkgShapePdf_ = new RooKeysPdf("bkgShapePdf", "", *mjj_, *rdsb, 
   				   mirrorOption, keysrho);
   }
   else {
     TH1* th1wjets = new TH1D("th1wjets", "th1wjets",
   			      NBINSFORPDF,MINRange,MAXRange);
     if(channel==0 || channel==1) 
       tree1->Draw(TString(PLOTVAR)+TString(">>th1wjets"), "", "goff");
     if(channel==0)
       tree2->Draw(TString(PLOTVAR)+TString(">>+th1wjets"), "", "goff");
     if(channel==2) 
       tree2->Draw(TString(PLOTVAR)+TString(">>th1wjets"), "", "goff");

     RooDataHist* rdh = new RooDataHist("rdh","", *mjj_, th1wjets);
     bkgShapePdf_ = new RooHistPdf("bkgShapePdf", "", *mjj_, *rdh);
   }



   // single top pdf
   if(IncludeTopSingleTop) {
     st1_mu_file =  new TFile("data/WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-s-channel.root", "READ");
     treeTemp = (TTree*) st1_mu_file->Get("WJet");
     ActivateTreeBranches(*treeTemp);
     gROOT->cd();
     TTree* tree3 = treeTemp->CopyTree(*mu_selection);
     if(USEKEYSPDF) {
       RooDataSet* rds3 = new RooDataSet("rds3","", tree3, *mjj_);
       stSPdf_ = new RooKeysPdf("stSPdf", "", *mjj_, *rds3, 
				mirrorOption, keysrho);
     }
     else {
       TH1* th1st1 = new TH1D("th1st1", "th1st1",NBINSFORPDF,MINRange,MAXRange);
       tree3->Draw( TString(PLOTVAR)+TString(">>th1st1"), *mu_selection, "goff");
       RooDataHist* rdhst1 = new RooDataHist("rdhst1","", *mjj_, th1st1);
       stSPdf_ = new RooHistPdf("stSPdf", "", *mjj_, *rdhst1);
     }
     delete tree3;
     delete treeTemp;


     st2_mu_file =  new TFile("data/WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-t-channel.root", "READ");
     treeTemp = (TTree*) st2_mu_file->Get("WJet");
     ActivateTreeBranches(*treeTemp);
     gROOT->cd();
     TTree* tree4 = treeTemp->CopyTree(*mu_selection);
     if(USEKEYSPDF) {
       RooDataSet* rds4 = new RooDataSet("rds4","", tree4, *mjj_);
       stTPdf_ = new RooKeysPdf("stTPdf", "", *mjj_, *rds4, 
				mirrorOption, keysrho);
     }
     else {
       TH1* th1st2 = new TH1D("th1st2", "th1st2",NBINSFORPDF,MINRange,MAXRange);
       tree4->Draw( TString(PLOTVAR)+TString(">>th1st2"), *mu_selection, "goff");
       RooDataHist* rdhst2 = new RooDataHist("rdhst2","", *mjj_, th1st2);
       stTPdf_ = new RooHistPdf("stTPdf", "", *mjj_, *rdhst2);
     }
     delete tree4;
     delete treeTemp;



     st3_mu_file =  new TFile("data/WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-t-channel.root", "READ");
     treeTemp = (TTree*) st3_mu_file->Get("WJet");
     ActivateTreeBranches(*treeTemp);
     gROOT->cd();
     TTree* tree5 = treeTemp->CopyTree(*mu_selection);
     if(USEKEYSPDF) {
       RooDataSet* rds5 = new RooDataSet("rds5","", tree5, *mjj_);
       stTWPdf_ = new RooKeysPdf("stTWPdf", "", *mjj_, *rds5, 
				 mirrorOption, keysrho);
     }
     else {
       TH1* th1st3 = new TH1D("th1st3", "th1st3",NBINSFORPDF,MINRange,MAXRange);
       tree5->Draw( TString(PLOTVAR)+TString(">>th1st3"), *mu_selection, "goff");
       RooDataHist* rdhst3 = new RooDataHist("rdhst3","", *mjj_, th1st3);
       stTWPdf_ = new RooHistPdf("stTWPdf", "", *mjj_, *rdhst3);
     }
     delete tree5;
     delete treeTemp;
   }


}







// ***** Function to return TTbar Pdf **** //
void makeTTbarPdf(int channel)
{  
   // top pair pdf
   ttbar_mu_file =  new TFile("data/ReducedTree/RD_WmunuJets_CMSSW423-Summer11MC_TTToLNu2Q2B.root", "READ");
   TTree* treeTemp = (TTree*) ttbar_mu_file->Get("WJet");
   ActivateTreeBranches(*treeTemp);
   gROOT->cd();
   TTree* tree = treeTemp->CopyTree(*mu_selection);

   if(USEKEYSPDF) {
     RooDataSet* rds = new RooDataSet("rds","", tree, *mjj_);
     ttPdf_ = new RooKeysPdf("ttPdf", "", *mjj_, *rds, mirrorOption,keysrho);
   }
   else {
     TH1* th1Top = new TH1D("th1Top", "th1Top", NBINSFORPDF, MINRange, MAXRange);
     tree->Draw( TString(PLOTVAR)+TString(">>th1Top"), *mu_selection, "goff");
     RooDataHist* rdhTop = new RooDataHist("rdhTop","", *mjj_, th1Top);
     ttPdf_ = new RooHistPdf("ttPdf", "", *mjj_, *rdhTop);
   }
   delete tree;
   delete treeTemp;
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
    latex.DrawLatex(0.21,0.85,Form("#int #font[12]{L} dt = %d pb^{-1}", (int) LUMINOSITY));
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
  t.SetBranchStatus("fit_chi2",    1);
  t.SetBranchStatus("fit_NDF",    1);
  t.SetBranchStatus("fit_status",    1);
  t.SetBranchStatus("gdevtt",    1);


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
