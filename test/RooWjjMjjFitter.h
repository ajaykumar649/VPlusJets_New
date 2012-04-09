// -*- mode: C++ -*-

#ifndef RooWjjMjjFitter_h
#define RooWjjMjjFitter_h

#include "RooWjjFitterParams.h"
#include "RooWjjFitterUtils.h"
#include "RooAbsData.h"

#include "RooWorkspace.h"

class RooFitResult;

class RooWjjMjjFitter {
public:

  enum SideBand { LowSB, HighSB };
  enum fitMode { mjj, mlnujj };
  RooWjjMjjFitter();
  RooWjjMjjFitter(RooWjjFitterParams & pars);
  virtual ~RooWjjMjjFitter() { }

  RooFitResult * fit(bool repeat = false);
  double computeChi2(int& ndf, bool correct = true);

  RooAbsPdf * makeFitter(bool allOne = false);
  RooAbsPdf * make4BodyPdf(RooWjjMjjFitter & fitter2body);
  RooAbsData * loadData(bool trunc = false);

  RooAbsPdf * makeDibosonPdf(int parameterize = 0);
  RooAbsPdf * makeWpJPdf(bool allOne = false);
  RooAbsPdf * makettbarPdf();
  RooAbsPdf * makeSingleTopPdf();
  RooAbsPdf * makeQCDPdf();
  RooAbsPdf * makeZpJPdf();
  RooAbsPdf * makeNPPdf();

  RooAbsPdf * makeWpJ4BodyPdf(RooWjjMjjFitter & fitter2body);

  RooPlot * stackedPlot(bool logy = false, fitMode fm = mjj, 
			bool leftLeg = false);
  RooPlot * residualPlot(RooPlot * thePlot, TString curveName,
			 TString pdfName = "", bool normalize = false,
			 fitMode fm = mjj);

  void loadParameters(TString fname);

  RooWorkspace& getWorkSpace() { return ws_; }

  RooRealVar * getVar() { return ws_.var(params_.var); }

  void resetYields();

  ////   Use For MC Dataset Toy Generation
  void generateToyMCSet(RooAbsPdf *inputPdf, const char* outFileName, int NEvts, int seedInitializer);
  void resetfSUfMU(double fSU, double fMU);

protected:
  TH1 * getWpJHistFromData(TString histName, double alpha,
			   double alphaDown, double alphaUp,
			   double xMin, double xMax, 
			   RooWjjMjjFitter & fitter2body,
			   RooWjjMjjFitter & shapesSBHi,
			   RooWjjMjjFitter & shapesSBLo,
			   TH1 * const alphaDownHist = 0,
			   TH1 * const alphaUpHist = 0);
  void subtractHistogram(TH1& hist, SideBand sideBand,
			 double xMin, double xMax,
			 RooWjjMjjFitter & fitter2body,
			 RooWjjMjjFitter & shapesSB);
  static void addHistograms(TH1& hist1, TH1& hist2, double weight = 1.0);
private:
  RooWorkspace ws_;
  RooWjjFitterParams params_;
  RooWjjFitterUtils utils_;

  double initWjets_;
  double initDiboson_;
  double ttbarNorm_;
  double singleTopNorm_;
  double zjetsNorm_;
  double QCDNorm_, QCDError_;
  RooAbsData::ErrorType errorType_;
  static Color_t const DibosonColor = kAzure + 8;

  TString rangeString_;
  int histOrder;

};

#endif
