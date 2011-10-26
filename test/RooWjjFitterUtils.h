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

class TH1;
class RooAbsReal;
class RooDataHist;
class RooDataSet;
class RooPlot;
class TLegend;

class RooWjjFitterUtils {

public:
  RooWjjFitterUtils();
  RooWjjFitterUtils(int nbins, double min, double max, int njets,
		    TString cut, TString var, TString treeName);
  virtual ~RooWjjFitterUtils();

  TH1 * File2Hist(TString fname, TString histName, 
		  int jes_scl = -1, bool noCuts = false, 
		  double binMult = 1.0) const;
  RooAbsPdf * Hist2Pdf(TH1 * hist, TString pdfName, 
		       RooWorkspace& ws) const;
  RooDataSet * File2Dataset(TString fname, TString dsName) const;
  TString fullCuts() const;

  void hist2RandomTree(TH1 * theHist, TString fname) const;

  int getNbins() const { return nbins_; }
  double getMin() const { return histmin_; }
  double getMax() const { return histmax_; }
  int getNjets() const { return njets_; }
  double getJES_scale(unsigned int i) const { 
    return ((i<jes_scales_.size()) ? jes_scales_[i] : 0.) ;
  }
  TString getCut() const { return cut_; }
  TString getVar() const { return var_; }
  TString getTreeName() const { return treeName_; }

  void setJES_scale(unsigned int i, double jes);

  static double computeChi2(RooHist& hist, RooAbsPdf& pdf, RooRealVar& obs, 
			    int& nbin);
  static TLegend * legend4Plot(RooPlot * plot);

protected:

  void updatenjets();
  static double sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin);
  static double sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin);

  int nbins_;
  double histmin_, histmax_;
  int njets_;
  std::vector<double> jes_scales_;

  RooAbsReal * massVar_;
  RooRealVar * mjj_;

  TString cut_;
  TString var_;
  TString treeName_;

  TString jetCut_;
};

#endif
