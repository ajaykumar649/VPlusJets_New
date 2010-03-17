// ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>

// RooFit headers
#include <RooAbsData.h>
#include <RooDataSet.h>
#include <RooAddPdf.h>
#include <RooBifurGauss.h>
#include <RooBreitWigner.h>
#include <RooCategory.h>
#include <RooCatType.h>
#include <RooCBShape.h>
#include <RooChi2Var.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooGaussian.h>
#include <RooGenericPdf.h>
#include <RooGlobalFunc.h>
#include <RooLandau.h>
#include <RooMinuit.h>
#include <RooNLLVar.h>
#include <RooPlot.h>
#include <RooRealVar.h>
#include <RooSimultaneous.h>
#include <RooTreeData.h>
#include <RooVoigtian.h>
#include <RooAbsReal.h>



const bool fitCBLineShape_    = false;
const bool fitGaussLineShape_ = false;

// The signal & background Pdf 
RooRealVar *rooMass_;
RooAddPdf* signalShapePdf_;
RooAddPdf* bkgShapePdf_;


// Private variables needed for CBLineShape
RooRealVar* rooCBMean_;
RooRealVar* rooCBSigma_;
RooRealVar* rooCBAlpha_;
RooRealVar* rooCBN_;
RooCBShape* rooCBPdf_;
RooRealVar* rooCBDummyFrac_;


// Private variables needed for Gaussian Line Shape
RooRealVar* rooGaussMean_;
RooRealVar* rooGaussSigma_;
RooRealVar* rooGaussDummyFrac_;
RooGaussian* rooGaussPdf_;


// Private variables needed for ZLineShape
RooRealVar*    rooZMean_;
RooRealVar*    rooZWidth_;
RooRealVar*    rooZSigma_;
RooRealVar*    rooZWidthL_;
RooRealVar*    rooZWidthR_;
RooRealVar*    rooZBifurGaussFrac_; 
RooVoigtian*   rooZVoigtPdf_;
RooBifurGauss* rooZBifurGaussPdf_;

// Private variables needed for background shape
RooRealVar *rooCMSBkgAlpha_;
RooRealVar *rooCMSBkgBeta_;
RooRealVar *rooCMSBkgPeak_;
RooRealVar *rooCMSBkgGamma_;
RooRealVar *rooCMSBkgDummyFrac_;  
RooGenericPdf *rooCMSBkgPdf_;
RooFormulaVar *u;
RooFormulaVar *erf;
TCanvas *c;



void ZeeMassFitter( TH1& hist )
{

  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{e^{+}e^{-}}", 
			    60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;

  gROOT->cd();
  RooDataHist* data = new RooDataHist("fitData","fitData",
				      RooArgList(Mass), &hist);
  data->get()->Print();
  cout << "Made datahist" << endl;


  // ********** Construct signal & bkg shape PDF ********** //
  makeSignalPdf();
  cout << "Made signal pdf" << endl;

  makeBkgPdf();
  cout << "Made bkg pdf" << endl;


  // Now define some efficiency/yield variables  
  RooRealVar numSignal("numSignal","numSignal", 4000.0, 0.0, 100000.0);
  RooRealVar numBkg("numBkg","numBkg", 1000.0, 0.0, 10000000.0);
  RooArgList components(*signalShapePdf_,*bkgShapePdf_);
  RooArgList yields(numSignal, numBkg);


  // The total pdf ...
  RooAddPdf totalPdf("totalPdf","totalPdf", components, yields);
  totalPdf.Print();


  // ********* Do the Actual Fit ********** //  
  RooFitResult *fitResult = totalPdf.fitTo(*data,"mer");
  fitResult->Print("v");


  std::cout << "Signal yield: " << numSignal.getVal() << " +- "
	    << numSignal.getError() << std::endl;
  std::cout << "Background yield: "<< numBkg.getVal() << " +- "
	    << numBkg.getError() << std::endl;




  // ********** Make and save Canvas for the plots ********** //
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.19);
  tdrStyle->SetPadRightMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.5);


  TString cname = TString("fit_canvas_") + hist.GetName();
  c = new TCanvas(cname,"",600,600);
  RooPlot* frame1 = Mass.frame();
  data->plotOn(frame1);
  // data->plotOn(frame1,RooFit::DataError(RooAbsData::SumW2));
  // totalPdf.plotOn(frame1,Components(*bkgShapePdf_),LineColor(kRed));
  totalPdf.plotOn(frame1);
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  //    if(data) delete data;
  //    if(c) delete c;
}










// ***** Function to return the signal Pdf *** //
void makeSignalPdf()
{

  if( fitCBLineShape_ )
    {
      // Signal PDF variables
      rooCBMean_  = new RooRealVar("cbMean","cbMean", 91.1876, 85.0, 95.0);
      rooCBSigma_ = new RooRealVar("cbSigma","cbSigma", 1.5, 0.0, 4.0);
      rooCBAlpha_ = new RooRealVar("cbAlpha","cbAlpha", 3.0, 1.0, 10.0);
      rooCBN_     = new RooRealVar("cbN","cbN", 3.0, 0.0, 20.0);
      rooCBPdf_   = new RooCBShape("cbPdf","cbPdf",*rooMass_,
				   *rooCBMean_, *rooCBSigma_,
				   *rooCBAlpha_,*rooCBN_);
      rooCBDummyFrac_ = new RooRealVar("dummyFrac","dummyFrac",1.0);
      signalShapePdf_ = new RooAddPdf("signalShapePdf", 
				      "signalShapePdf",
				      *rooCBPdf_,*rooCBPdf_,
				      *rooCBDummyFrac_);
    }
  else if( fitGaussLineShape_ )
    {
      // Signal PDF variables
      rooGaussMean_  = new RooRealVar("gaussMean","gaussMean", 91.1876, 85.0, 95.0);
      rooGaussSigma_ = new RooRealVar("gaussSigma","gaussSigma", 1.5, 0.0, 4.0);
      rooGaussPdf_ = new RooGaussian("gaussPdf","gaussPdf",*rooMass_,
				     *rooGaussMean_,
				     *rooGaussSigma_);
      rooGaussDummyFrac_ = new RooRealVar("dummyFrac","dummyFrac",1.0);
      signalShapePdf_ = new RooAddPdf("signalShapePdf", "signalShapePdf",
				      *rooGaussPdf_,*rooGaussPdf_,
				      *rooGaussDummyFrac_);
    }
  else
    {
      // Signal PDF variables
      rooZMean_   = new RooRealVar("zMean","zMean", 91.1876, 87.5, 94.5);
      rooZWidth_  = new RooRealVar("zWidth","zWidth", 2.8, 0.0, 10.0);
      rooZSigma_  = new RooRealVar("zSigma","zSigma", 2.8, 0.0, 10.0);
      rooZWidthL_ = new RooRealVar("zWidthL","zWidthL", 1.0, 0.0, 10.0);
      rooZWidthR_ = new RooRealVar("zWidthR","zWidthR", 1.0, 0.0, 10.0);

      // Voigtian
      rooZVoigtPdf_ = new RooVoigtian("zVoigtPdf", "zVoigtPdf", 
				      *rooMass_, *rooZMean_, 
				      *rooZWidth_, *rooZSigma_);
      // Bifurcated Gaussian
      rooZBifurGaussPdf_ = new RooBifurGauss("zBifurGaussPdf", 
					     "zBifurGaussPdf", 
					     *rooMass_, *rooZMean_, 
					     *rooZWidthL_, *rooZWidthR_);      
      // Bifurcated Gaussian fraction
      rooZBifurGaussFrac_ = new RooRealVar("zBifurGaussFrac",
					   "zBifurGaussFrac",
					   0.02, 0.0, 1);      
      // The total signal PDF
      signalShapePdf_ = new RooAddPdf("signalShapePdf", "signalShapePdf",
				      *rooZVoigtPdf_,*rooZBifurGaussPdf_,
				      *rooZBifurGaussFrac_);
    }

  return;
}







// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  
  // Background PDF variables
  rooCMSBkgPeak_  = new RooRealVar("cmsBkgPeak","cmsBkgPeak", 91.1876);
  rooCMSBkgBeta_  = new RooRealVar("cmsBkgBeta","cmsBkgBeta", 0.001, 0.0, 0.1);
  rooCMSBkgAlpha_ = new RooRealVar("cmsBkgAlpha","cmsBkgAlpha",
				   62.0, 50.0, 70.0);
  rooCMSBkgGamma_ = new RooRealVar("cmsBkgGamma","cmsBkgGamma",
				   0.05, 0.0, 0.1);
  
  u = new RooFormulaVar("u","(@0-@1)*@2",RooArgList(*rooMass_, 
						 *rooCMSBkgPeak_, *rooCMSBkgGamma_));
  erf = new RooFormulaVar("erf", "RooMath::erfc((@0-@1)*@2)", 
		      RooArgList(*rooCMSBkgAlpha_, *rooMass_, *rooCMSBkgBeta_));

  rooCMSBkgPdf_ = 
    new RooGenericPdf("cmsBkgPdf",
		      "exp(-@0)",RooArgList(*u));


//   rooCMSBkgPdf_ = 
//     new RooGenericPdf("cmsBkgPdf",
// 		      "(@0>-70.0)*(@0<70.0)*exp(-@0)*@1+(1.0**20)*(@0<-70.0)", 
// 		      RooArgList(*u, *erf));

  rooCMSBkgDummyFrac_ = new RooRealVar("dummyFrac","dummyFrac",1.0);
  
  bkgShapePdf_ = new RooAddPdf("bkgShapePdf", "bkgShapePdf",
			       *rooCMSBkgPdf_,*rooCMSBkgPdf_,
			       *rooCMSBkgDummyFrac_);
  
  return;
}


