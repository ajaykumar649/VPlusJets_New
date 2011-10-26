// -*- mode: C++ -*-

#ifndef RooWjjMjjFitter_h
#define RooWjjMjjFitter_h

#include "RooWjjFitterParams.h"
#include "RooWjjFitterUtils.h"

#include "RooWorkspace.h"

class RooFitResult;

class RooWjjMjjFitter {
public:
  RooWjjMjjFitter();
  RooWjjMjjFitter(RooWjjFitterParams & pars);
  virtual ~RooWjjMjjFitter() { }

  RooFitResult * fit();
  RooPlot * computeChi2(double& chi2, int& ndf);

  RooAbsPdf * makeFitter();
  RooAbsData * loadData();

  RooAbsPdf * makeDibosonPdf();
  RooAbsPdf * makeWpJPdf();
  RooAbsPdf * makettbarPdf();
  RooAbsPdf * makeSingleTopPdf();
  RooAbsPdf * makeQCDPdf();
  RooAbsPdf * makeZpJPdf();
  RooAbsPdf * makeNPPdf();

  void loadParameters(TString fname);

  RooWorkspace& getWorkSpace() { return ws_; }

  void resetYields();

private:
  RooWorkspace ws_;
  RooWjjFitterParams params_;
  RooWjjFitterUtils utils_;

  double initWjets_;
  double initDiboson_;
  double ttbarNorm_;
  double singleTopNorm_;
  double zjetsNorm_;
  double QCDNorm_;

  TString rangeString_;
};

#endif
