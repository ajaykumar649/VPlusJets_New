#include "RooWjjFitterUtils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TLegend.h"

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
#include "RooPlot.h"

RooWjjFitterUtils::RooWjjFitterUtils() :
  jes_scales_(2)
{
  initialize();
  for (int j = 0; j < 2; ++j)
    jes_scales_[j] = 0.;
}

RooWjjFitterUtils::RooWjjFitterUtils(RooWjjFitterParams & pars) :
  params_(pars), jes_scales_(2)
{
  initialize();
  jes_scales_[0] = params_.JES_scale1;
  jes_scales_[1] = params_.JES_scale2;
}

RooWjjFitterUtils::~RooWjjFitterUtils()  { 
  delete mjj_; 
  delete massVar_;
}

void RooWjjFitterUtils::initialize() {
  updatenjets();
  mjj_ = new RooRealVar(params_.var, "m_{jj}", params_.minMass, 
			params_.maxMass, "GeV/c^{2}");
  mjj_->setPlotLabel(mjj_->GetTitle());
  mjj_->setBins(params_.nbins);
  massVar_ = new RooFormulaVar("mass", "@0", RooArgList( *mjj_));  
}

TH1 * RooWjjFitterUtils::File2Hist(TString fname, 
				   TString histName, 
				   int jes_scl, bool noCuts, 
				   double binMult) const {
  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject(params_.treeName, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << params_.treeName << " in file " << fname 
	      << '\n';
    return 0;
  }
  double tmpScale = 0.;
  if ((jes_scl >= 0) && (jes_scl < int(jes_scales_.size())))
    tmpScale = jes_scales_[jes_scl];
  TString plotStr = TString::Format("%s*(1+%0.4f)", params_.var.Data(), 
				    tmpScale);
  TH1D * theHist = new TH1D(histName, histName, int(params_.nbins*binMult), 
			    params_.minMass, params_.maxMass);
  theHist->Sumw2();

  theTree->Draw(plotStr + ">>" + histName, 
		((noCuts) ? TString("") : fullCuts()),
		"goff");

  delete theTree;

  theHist->SetDirectory(0);

  delete treeFile;

  //theHist->Print();
  return theHist;
}

RooAbsPdf * RooWjjFitterUtils::Hist2Pdf(TH1 * hist, TString pdfName,
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

RooDataSet * RooWjjFitterUtils::File2Dataset(TString fname, 
					     TString dsName, 
					     bool trunc) const {
  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject(params_.treeName, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << params_.treeName << " in file " << fname 
	      << '\n';
    return 0;
  }

  activateBranches(*theTree);
  TFile holder("holder_DELETE_ME.root", "recreate");
  TTree * reducedTree = theTree->CopyTree( fullCuts(trunc) );

  RooDataSet * ds = new RooDataSet(dsName, dsName, reducedTree, 
				   RooArgSet(*mjj_));

  delete reducedTree;
  delete theTree;
  delete treeFile;

  return ds;
}

TString RooWjjFitterUtils::fullCuts(bool trunc) const {
  TString theCut = TString::Format("((%s > %0.3f) && (%s < %0.3f))", 
				    params_.var.Data(), params_.minMass, 
				    params_.var.Data(), params_.maxMass);
  if (trunc) {
    theCut = TString::Format("(((%s>%0.3f) && (%s<%0.3f)) || "
			     "((%s>%0.3f) && (%s<%0.3f)))",
			     params_.var.Data(), params_.minMass,
			     params_.var.Data(), params_.minTrunc,
			     params_.var.Data(), params_.maxTrunc,
			     params_.var.Data(), params_.maxMass);
  }
  theCut += TString::Format(" && (%s)", jetCut_.Data());
  if (params_.cuts.Length() > 0)
    theCut +=  TString::Format(" && (%s)", params_.cuts.Data());
  return "(" + theCut + ")";
}

void RooWjjFitterUtils::hist2RandomTree(TH1 * theHist, 
					TString fname) const {
  TFile treeFile(fname, "recreate");
  TTree WJet(params_.treeName, params_.treeName);
  double theVar;
  WJet.Branch(params_.var, &theVar, params_.var + "/D");
  double entries = 0;
  for (entries = 0; entries < theHist->GetEntries(); ++entries) {
    theVar = theHist->GetRandom();
    WJet.Fill();
  }
  WJet.Write();
  treeFile.Close();
}

void RooWjjFitterUtils::updatenjets() {
  jetCut_ = TString::Format("evtNJ==%d", params_.njets);
  if (params_.njets < 2)
    jetCut_ = "evtNJ==2 || evtNJ==3";
}

double RooWjjFitterUtils::computeChi2(RooHist& hist, RooAbsPdf& pdf, 
				      RooRealVar& obs, int& nbin) {
  int np = hist.GetN();
  nbin = 0;
  double chi2(0);

  double x,y,eyl,eyh,exl,exh;
  double avg, pdfSig2;
  double Npdf = pdf.expectedEvents(RooArgSet(obs));
//   std::cout << " *** N_pdf: " << Npdf << " ***\n";
  TString className;
  RooAbsReal * binInt;
  for (int i=0; i<np; ++i) {
    hist.GetPoint(i,x,y);
    eyl = hist.GetEYlow()[i];
    eyh = hist.GetEYhigh()[i];
    exl = hist.GetEXlow()[i];
    exh = hist.GetEXhigh()[i];

    obs.setRange("binRange", x-exl, x+exh);
    binInt = pdf.createIntegral(obs, RooFit::NormSet(obs),
				RooFit::Range("binRange"));
    avg = Npdf*binInt->getVal();
    delete binInt;

    obs.setVal(x);
    pdfSig2 = 0.;
    className = pdf.ClassName();
//     std::cout << TString::Format("bin [%0.2f, %0.2f]", x-exl, x+exh) << '\n';
    if (className == "RooHistPdf") {
      RooHistPdf& tmpPdf = dynamic_cast<RooHistPdf&>(pdf);
      pdfSig2 = sig2(tmpPdf, obs, avg);
    } else if (className == "RooAddPdf") {
      RooAddPdf& tmpPdf = dynamic_cast<RooAddPdf&>(pdf);
      pdfSig2 = sig2(tmpPdf, obs, avg);
    }
    
    if (y != 0) {
      ++nbin;
      double pull2 = (y-avg)*(y-avg);
      pull2 = (y>avg) ? pull2/(eyl*eyl + pdfSig2) : pull2/(eyh*eyh + pdfSig2) ;
      chi2 += pull2;
    }
  }

  return chi2;
}

TLegend * RooWjjFitterUtils::legend4Plot(RooPlot * plot) {
  TObject * theObj;
  TString objName, objTitle;
  TLegend * theLeg = new TLegend(0.70, 0.65, 0.92, 0.92, "", "NDC");
  theLeg->SetName("theLegend");

  theLeg->SetBorderSize(0);
  theLeg->SetLineColor(0);
  theLeg->SetFillColor(0);
  theLeg->SetFillStyle(0);
  theLeg->SetLineWidth(0);
  theLeg->SetLineStyle(0);
  theLeg->SetTextFont(42);
  int entryCnt = 0;
  for(int obj=0; obj < plot->numItems(); ++obj) {
    objName = plot->nameOf(obj);
    if (!(plot->getInvisible(objName))) {
      theObj = plot->getObject(obj);
      objTitle = theObj->GetTitle();
      if (objTitle.Length() < 1)
	objTitle = objName;
      theLeg->AddEntry(theObj, objTitle, plot->getDrawOptions(objName));
      ++entryCnt;
    }
  }
  theLeg->SetY1NDC(0.92 - 0.04*entryCnt - 0.02);
  theLeg->SetY1(theLeg->GetY1NDC());
  return theLeg;
}

void RooWjjFitterUtils::activateBranches(TTree& t) {
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
  t.SetBranchStatus("JetPFCor_QGLikelihood",    1);

  t.SetBranchStatus("event_met_pfmet",    1);
  t.SetBranchStatus("event_met_pfmetPhi",    1);
  t.SetBranchStatus("event_met_pfmetsignificance",    1);
  t.SetBranchStatus("event_BeamSpot_x",    1);
  t.SetBranchStatus("event_BeamSpot_y",    1);
  t.SetBranchStatus("event_RhoForLeptonIsolation",    1);
  t.SetBranchStatus("event_nPV",    1);

  t.SetBranchStatus("W_mt",    1);
  t.SetBranchStatus("W_pt",    1);
  t.SetBranchStatus("W_pzNu1",    1);
  t.SetBranchStatus("W_pzNu2",    1);
  t.SetBranchStatus("fit_status",    1);
  t.SetBranchStatus("gdevtt",    1);
  t.SetBranchStatus("fit_chi2",    1);
  t.SetBranchStatus("fit_NDF",    1);
  t.SetBranchStatus("fi2_chi2",    1);
  t.SetBranchStatus("fi2_NDF",    1);
  t.SetBranchStatus("fit_mlvjj", 1);
  t.SetBranchStatus("evtNJ",    1);

  t.SetBranchStatus("Mass2j_PFCor",    1);
  t.SetBranchStatus("MassV2j_PFCor",    1);
  t.SetBranchStatus("cosJacksonAngle2j_PFCor",    1);
  t.SetBranchStatus("cosJacksonAngleV2j_PFCor",    1);
}

double RooWjjFitterUtils::sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin) {

  double retVal = 0.;
  TString className;
  double iN = 0, sumf = 0.;
  bool allCoefs(false);
  bool fCoefs(true);
  if (pdf.coefList().getSize() == pdf.pdfList().getSize())
    allCoefs = true;
  if (pdf.coefList().getSize()+1 == pdf.pdfList().getSize())
    fCoefs = true;
  RooAbsReal * coef = 0;
  RooAbsPdf * ipdf = 0;
//   std::cout << "pdf: " << pdf.GetName() 
// 	    << " N for pdf: " << Nbin
// 	    << '\n';
  for (int i = 0; i < pdf.pdfList().getSize(); ++i) {
    ipdf = dynamic_cast<RooAbsPdf *>(pdf.pdfList().at(i));
    coef = 0;
    iN = 0;
    if (pdf.coefList().getSize() > i)
      coef = dynamic_cast< RooAbsReal * >(pdf.coefList().at(i));
    if ((allCoefs) && (coef)) {
      iN = coef->getVal();
      RooAbsReal * binInt = ipdf->createIntegral(obs, RooFit::NormSet(obs),
						 RooFit::Range("binRange"));
      iN *= binInt->getVal();
      delete binInt;

    } else if ((fCoefs) && (coef)) {
      iN = coef->getVal()*Nbin;
      sumf += coef->getVal();
    } else if (fCoefs) {
      iN = (1.-sumf)*Nbin;
    } else {
      iN = ipdf->expectedEvents(RooArgSet(obs));
      RooAbsReal * binInt = ipdf->createIntegral(obs, RooFit::NormSet(obs),
						 RooFit::Range("binRange"));
      iN *= binInt->getVal();
      delete binInt;
    }

    className = ipdf->ClassName();
    if (className == "RooHistPdf") {
      RooHistPdf * tmpPdf = dynamic_cast<RooHistPdf *>(ipdf);
      retVal += sig2(*tmpPdf, obs, iN);
    } else if (className == "RooAddPdf") {
      RooAddPdf * tmpPdf = dynamic_cast<RooAddPdf *>(ipdf);
      retVal += sig2(*tmpPdf, obs, iN);
    }
  }
  return retVal;

}

double RooWjjFitterUtils::sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin) {

  obs.setVal(obs.getMin("binRange"));
  double binVol, weight, weightErr;
  double sumw = 0., sumw2 = 0.;
//   std::cout << "pdf: " << pdf.GetName()
// 	    << " Nbin: " << Nbin;
  while (obs.getVal() < obs.getMax("binRange")) {
    pdf.dataHist().get(obs);
    binVol = pdf.dataHist().binVolume();
    weight = pdf.dataHist().weight();
    weightErr = pdf.dataHist().weightError(RooAbsData::SumW2);
//     std::cout << " binVol: " << binVol
// 	      << " weight: " << weight
// 	      << " weightErr: " << weightErr
// 	      << ' ';
    sumw += weight;
    sumw2 += weightErr*weightErr;
    obs.setVal(obs.getVal() + binVol);
  }
//   std::cout << " sumw: " << sumw
// 	    << " sumw2: " << sumw2;
  if (sumw == 0) return 0.;
  double retVal = Nbin*sqrt(sumw2)/sumw;
//   std::cout << " sig: " << retVal << '\n';
  return retVal*retVal;

}


////////////////////////////////////////////////////////////////////
////   Use For Fitting/Optimizing On Toy MC Sets
////////////////////////////////////////////////////////////////////

RooDataSet * RooWjjFitterUtils::File2DatasetNoCuts(TString fname, 
						   TString dsName, 
						   bool /*trunc*/) const {
  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject(params_.treeName, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << params_.treeName << " in file " << fname 
	      << '\n';
    return 0;
  }

  activateBranches(*theTree);
  TFile holder("holder_DELETE_ME.root", "recreate");
  TTree * reducedTree = theTree->CopyTree("");

  RooDataSet * ds = new RooDataSet(dsName, dsName, reducedTree, 
				   RooArgSet(*mjj_));

  delete reducedTree;
  delete theTree;
  delete treeFile;

  return ds;
}

////////////////////////////////////////////////////////////////////
