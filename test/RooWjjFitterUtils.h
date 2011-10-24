// -*- mode: C++ -*-

#ifndef RooWjjFitterUtils_h
#define RooWjjFitterUtils_h

#include <vector>

#include "TString.h"
#include "RooWorkspace.h"

class TH1;
class RooAbsReal;
class RooRealVar;
class RooHistPdf;
class RooDataHist;
class RooDataSet;

class RooWjjFitterUtils {

public:
  RooWjjFitterUtils();
  RooWjjFitterUtils(int nbins, double min, double max, int njets,
		    TString const& cut, TString const& var, 
		    TString const& treeName);
  virtual ~RooWjjFitterUtils();

  TH1 * File2Hist(TString const& fname, TString const& histName, 
		  int jes_scl = -1, bool noCuts = false, 
		  double binMult = 1.0) const;
  RooAbsPdf * Hist2Pdf(TH1 * hist, TString const& pdfName, 
		       RooWorkspace& ws) const;
  RooDataSet * File2Dataset(TString const& fname, TString const& dsName) const;
  TString fullCuts() const;

  void hist2RandomTree(TH1 * theHist, TString const& fname) const;

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

private:

  void updatenjets();

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
