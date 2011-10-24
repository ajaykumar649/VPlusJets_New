#include "RooWjjFitterUtils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"

RooWjjFitterUtils::RooWjjFitterUtils() :
  nbins_(14), histmin_(50.), histmax_(120.), njets_(2),
  jes_scales_(2), massVar_(0), mjj_(0)
{
  cut_ = "";
  var_ = "Mass2j_PFCor";
  treeName_ = "WJet";
  updatenjets();
  for (int j = 0; j < 2; ++j)
    jes_scales_[j] = 0.;
  mjj_ = new RooRealVar(var_, "m_{jj}", histmin_, histmax_, "GeV/c^{2}");
  mjj_->setPlotLabel(mjj_->GetTitle());
  mjj_->setBins(nbins_);
  massVar_ = new RooFormulaVar("mass", "@0", RooArgList( *mjj_));
}

RooWjjFitterUtils::RooWjjFitterUtils(int nbins, double min, double max, 
				     int njets, TString const& cut, 
				     TString const& var, 
				     TString const& treeName) :
  nbins_(nbins), histmin_(min), histmax_(max), njets_(njets),
  jes_scales_(2), massVar_(0), mjj_(0), cut_(cut), var_(var), 
  treeName_(treeName)
{
  updatenjets();
  for (int j = 0; j < 2; ++j)
    jes_scales_[j] = 0.;
  mjj_ = new RooRealVar(var_, "m_{jj}", histmin_, histmax_, "GeV/c^{2}");
  mjj_->setBins(nbins_);
  massVar_ = new RooFormulaVar("mass", "@0", RooArgList( *mjj_));
}

RooWjjFitterUtils::~RooWjjFitterUtils()  { 
  delete mjj_; 
  delete massVar_;
}

TH1 * RooWjjFitterUtils::File2Hist(TString const& fname, 
				   TString const& histName, 
				   int jes_scl, bool noCuts, 
				   double binMult) const {
  TFile treeFile(fname);
  TTree * theTree;
  treeFile.GetObject(treeName_, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << treeName_ << " in file " << fname 
	      << '\n';
    return 0;
  }
  double tmpScale = 0.;
  if ((jes_scl >= 0) && (jes_scl < int(jes_scales_.size())))
    tmpScale = jes_scales_[jes_scl];
  TString plotStr = TString::Format("%s*(1+%0.4f)", var_.Data(), tmpScale);
  TH1D * theHist = new TH1D(histName, histName, int(nbins_*binMult), 
			    histmin_, histmax_);
  theHist->Sumw2();

  theTree->Draw(plotStr + ">>" + histName, 
		((noCuts) ? TString("") : fullCuts()),
		"goff");

  delete theTree;

  theHist->SetDirectory(0);

  return theHist;
}

RooAbsPdf * RooWjjFitterUtils::Hist2Pdf(TH1 * hist, TString const& pdfName,
					 RooWorkspace& ws) const {
  if (ws.pdf(pdfName))
    return ws.pdf(pdfName);

  RooDataHist newHist(pdfName + "_hist", pdfName + "_hist",
		      RooArgList(*mjj_), hist);
  ws.import(newHist);

  RooHistPdf thePdf = RooHistPdf(pdfName, pdfName, RooArgSet(*massVar_),
				 RooArgSet(*mjj_), newHist);
  //thePdf.Print();
  ws.import(thePdf, RooFit::RecycleConflictNodes(), RooFit::Silence());

  return ws.pdf(pdfName);
}

RooDataSet * RooWjjFitterUtils::File2Dataset(TString const& fname, 
					     TString const& dsName) const {
  TFile treeFile(fname);
  TTree * theTree;
  treeFile.GetObject(treeName_, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << treeName_ << " in file " << fname 
	      << '\n';
    return 0;
  }

  TFile holder("holder_DELETE_ME.root", "recreate");
  TTree * reducedTree = theTree->CopyTree( fullCuts() );

  RooDataSet * ds = new RooDataSet(dsName, dsName, reducedTree, 
				   RooArgSet(*mjj_));

  delete reducedTree;
  delete theTree;

  return ds;
}

TString RooWjjFitterUtils::fullCuts() const {
  TString theCut = TString::Format("((%s > %0.3f) && (%s < %0.3f))", 
				    var_.Data(), histmin_, 
				    var_.Data(), histmax_);
  theCut += TString::Format(" && (%s)", jetCut_.Data());
  if (cut_.Length() > 0)
    theCut +=  TString::Format(" && (%s)", cut_.Data());
  return "(" + theCut + ")";
}

void RooWjjFitterUtils::hist2RandomTree(TH1 * theHist, 
					TString const& fname) const {
  TFile treeFile(fname, "recreate");
  TTree WJet(treeName_, treeName_);
  double theVar;
  WJet.Branch(var_, &theVar, var_ + "/D");
  double entries = 0;
  for (entries = 0; entries < theHist->GetEntries(); ++entries) {
    theVar = theHist->GetRandom();
    WJet.Fill();
  }
  WJet.Write();
  treeFile.Close();
}


// void RooWjjFitterUtils::setWorkspace(RooWorkspace * ws) {
//   ws_ = ws;
// }

void RooWjjFitterUtils::setJES_scale(unsigned int i, double jes) {
  if (i < jes_scales_.size())
    jes_scales_[i] = jes;
}

void RooWjjFitterUtils::updatenjets() {
  jetCut_ = TString::Format("evtNJ==%d", njets_);
  if (njets_ < 2)
    jetCut_ = "evtNJ==2 || evtNJ==3";
}

