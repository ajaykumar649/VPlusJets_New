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
  RooAbsPdf * makeFitter();

  RooAbsPdf * makeDibosonPdf();
  RooAbsPdf * makeWpJPdf();
  RooAbsPdf * makettbarPdf();
  RooAbsPdf * makeSingleTopPdf();
  RooAbsPdf * makeQCDPdf();
  RooAbsPdf * makeZpJPdf();
  RooAbsPdf * makeNPPdf();

  RooWorkspace& getWorkSpace() { return ws_; }

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
};

#endif
