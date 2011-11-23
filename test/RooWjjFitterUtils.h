// -*- mode: C++ -*-

#ifndef RooWjjFitterUtils_h
#define RooWjjFitterUtils_h

#include <vector>

#include "TString.h"
#include "RooWorkspace.h"
#include "RooHist.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"

#include "RooWjjFitterParams.h"

class TH1;
class RooAbsReal;
class RooDataHist;
class RooDataSet;
class RooPlot;
class TLegend;

class RooWjjFitterUtils {

public:
  RooWjjFitterUtils();
  RooWjjFitterUtils(RooWjjFitterParams & pars);
  virtual ~RooWjjFitterUtils();

  void vars2ws(RooWorkspace& ws) const;

  TH1 * File2Hist(TString fname, TString histName, 
		  int jes_scl = -1, bool noCuts = false, 
		  double binMult = 1.0, TString cutOverride = "") const;
  RooAbsPdf * Hist2Pdf(TH1 * hist, TString pdfName, 
		       RooWorkspace& ws) const;
  RooDataSet * File2Dataset(TString fname, TString dsName,
			    bool trunc = false,
			    bool noCuts = false) const;
//   RooDataSet * File2DatasetNoCuts(TString fname, TString dsName,
// 				  bool trunc = false) const;
  TString fullCuts(bool trunc = false) const;

  void hist2RandomTree(TH1 * theHist, TString fname) const;

  static double computeChi2(RooHist& hist, RooAbsPdf& pdf, RooRealVar& obs, 
			    int& nbin);
  static TLegend * legend4Plot(RooPlot * plot);
  static void activateBranches(TTree & t);

protected:

  void initialize();

  void updatenjets();
  static double sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin);
  static double sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin);

  RooWjjFitterParams params_;

  RooAbsReal * massVar_;
  RooRealVar * mjj_;

  TString jetCut_;
};

#endif
