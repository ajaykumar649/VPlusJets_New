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
#include "EffTableLoader.h"

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

  TH1 * newEmptyHist(TString histName, int binMult = 1) const;
  TH1 * File2Hist(TString fname, TString histName, bool isElectron, 
		  int jes_scl = -1, bool noCuts = false, 
		  int binMult = 1, TString cutOverride = "",
		  bool CPweights = false, int interfereWgt = 0) const;
  RooAbsPdf * Hist2Pdf(TH1 * hist, TString pdfName, 
		       RooWorkspace& ws, int order = 0,
		       bool fast = true) const;
  RooDataSet * File2Dataset(TString fname, TString dsName,
			    bool isElectron,
			    bool trunc = false,
			    bool noCuts = false, bool weighted = false) const;
//   RooDataSet * File2DatasetNoCuts(TString fname, TString dsName,
// 				  bool trunc = false) const;
  TString fitCuts() const;
  TString fullCuts(bool trunc = false) const;

  void hist2RandomTree(TH1 * theHist, TString fname) const;

  double * getBinArray() const { return binArray; }

  double effWeight(float lepton_pt, float lepton_eta, float W_mt, 
		   float * JetPFCor_Pt, float * JetPFCor_Eta, int Njets,
		   float event_met, bool isElectron) const;

  static double computeChi2(RooHist& hist, RooAbsPdf& pdf, RooRealVar& obs, 
			    int& nbin, bool correct = true);
  static TLegend * legend4Plot(RooPlot * plot, bool left = false);
  static void activateBranches(TTree & t, bool isElectron = false);

  static double dijetEff(unsigned int Njets, std::vector<double> const& eff30,
			 std::vector<double> const& eff25n30);
  static double getCPweight(double mH, double wH, double m, int BWflag = 1);
protected:

  void initialize();

  void updatenjets();
  static double sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin);
  static double sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin);

  RooWjjFitterParams params_;

  RooAbsReal * massVar_;
  RooRealVar * mjj_;

  double * binArray;

  TString jetCut_;

  std::vector<EffTableLoader*> effEleReco, effEleId, effEle;
  std::vector<EffTableLoader*> effMuId, effMu;
  std::vector<EffTableLoader*> effJ30, effJ25NoJ30;
  std::vector<EffTableLoader*> effMHT;
  std::vector<EffTableLoader*> effEleWMt;
  
};

#endif
