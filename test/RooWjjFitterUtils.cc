#include "RooWjjFitterUtils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TLegend.h"
#include "TEventList.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TTreeFormula.h"

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
#include "RooBinning.h"

#include "RooTH1DPdf.h"

#include "MMozer/powhegweight/interface/pwhg_wrapper.h"

static const unsigned int maxJets = 6;

RooWjjFitterUtils::RooWjjFitterUtils()
{
  initialize();
}

RooWjjFitterUtils::RooWjjFitterUtils(RooWjjFitterParams & pars) :
  params_(pars), binArray(0)
{
  initialize();
}

RooWjjFitterUtils::~RooWjjFitterUtils()  { 
  delete mjj_; 
  // delete massVar_;
  delete[] binArray;

  unsigned int i;
  for (i=0; i<effMuId.size(); ++i) {
    delete effMuId[i];
    delete effMu[i];
  }
  for (i=0; i<effEleId.size(); ++i) {
    delete effEleId[i];
    delete effEle[i];
    delete effEleReco[i];
    delete effJ30[i];
    delete effJ25NoJ30[i];
    delete effMHT[i];
  }
}

void RooWjjFitterUtils::vars2ws(RooWorkspace& ws) const {
  ws.import(*massVar_, RooFit::RecycleConflictNodes(), RooFit::Silence());
}

void RooWjjFitterUtils::initialize() {
  updatenjets();
  mjj_ = new RooRealVar(params_.var, "m_{jj}", params_.minMass, 
			params_.maxMass, "GeV");
  mjj_->setPlotLabel(mjj_->GetTitle());
  mjj_->setBins(params_.nbins);
  // massVar_ = new RooFormulaVar("mass", "@0", RooArgList( *mjj_));  
  massVar_ = mjj_;

  unsigned int i;
  if (params_.binEdges.size() > 1) {
    binArray = new double[params_.binEdges.size()];
    for (i = 0; i<params_.binEdges.size(); ++i)
      binArray[i] = params_.binEdges[i];
    RooBinning varBins(params_.binEdges.size()-1, binArray, "plotBinning");
    varBins.Print("v");
    mjj_->setBinning(varBins, "plotBinning");
    mjj_->setBinning(varBins);
  } else {
    RooBinning evenBins(params_.nbins, params_.minMass, params_.maxMass); 
    mjj_->setBinning(evenBins, "plotBinning");
    mjj_->setBinning(evenBins);  
  }

  for (i = 0; i < params_.muIdEffFiles.size(); ++i) {
    effMuId.push_back(new EffTableLoader(params_.muIdEffFiles[i].Data()));
    effMu.push_back(new EffTableLoader(params_.muHLTEffFiles[i].Data()));
  }
  for (i = 0; i < params_.eleIdEffFiles.size(); ++i) {
    effEleId.push_back(new EffTableLoader(params_.eleIdEffFiles[i].Data()));
    effEleReco.push_back(new EffTableLoader(params_.eleRecoEffFiles[i].Data()));
    effEle.push_back(new EffTableLoader(params_.eleHLTEffFiles[i].Data()));
    effJ30.push_back(new EffTableLoader(params_.eleJ30EffFiles[i].Data()));
    effJ25NoJ30.push_back(new EffTableLoader(params_.eleJ25NoJ30EffFiles[i].Data()));
    effMHT.push_back(new EffTableLoader(params_.eleMHTEffFiles[i].Data()));
    effEleWMt.push_back(new EffTableLoader(params_.eleWMtEffFiles[i].Data()));
  }

  std::cout << "full cuts: " << fullCuts() << '\n';

}

TH1 * RooWjjFitterUtils::newEmptyHist(TString histName, int binMult) const {
  TH1D * theHist;
  if (binArray) {
    double * allBins = binArray;
    int multBins = (params_.binEdges.size()-1)*binMult + 1;
    if (binMult > 1) {
      allBins = new double[multBins];
      int binI = 0;
      double binW = 0.;
      for (unsigned int i = 0; i<params_.binEdges.size(); ++i) {
	if (i+1 < params_.binEdges.size())
	  binW = params_.binEdges[i+1] - params_.binEdges[i];
	else
	  binW = 0.;
	for (int j = 0; j < binMult; ++j)
	  if (binI < multBins)
	    allBins[binI++] = params_.binEdges[i]+j*binW/binMult;
      }
    }

//     std::cout << "binning ";
//     for (int i = 0; i < multBins; ++i) {
//       std::cout << allBins[i] << ",";
//     }
//     std::cout << '\n';
    theHist = new TH1D(histName, histName, multBins-1, allBins);
  } else
    theHist = new TH1D(histName, histName, params_.nbins*binMult, 
		       params_.minMass, params_.maxMass);
  theHist->Sumw2();
  return theHist;
}

TH1 * RooWjjFitterUtils::File2Hist(TString fname, 
				   TString histName, bool isElectron,
				   int jes_scl, bool noCuts, 
				   int binMult, TString cutOverride,
				   bool CPweights, int interfereWgt) const {
  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject(params_.treeName, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << params_.treeName << " in file " << fname 
	      << '\n';
    return 0;
  }

  double tmpScale = 0.;
  if ((jes_scl >= 0) && (jes_scl < int(params_.JES_scales.size())))
    tmpScale = params_.JES_scales[jes_scl];
//   TString plotStr = TString::Format("%s*(1+%0.4f)", params_.var.Data(), 
// 				    tmpScale);
  TH1 * theHist = newEmptyHist(histName, binMult);

  TString theCuts(cutOverride);
  if (theCuts.Length() < 1)
    theCuts = fullCuts();
  else
    std::cout << histName << " cuts: " << theCuts << '\n';
  theTree->Draw(">>" + histName + "_evtList", 
		((noCuts) ? TString("") : theCuts),
		"goff");
  TEventList * list = (TEventList *)gDirectory->Get(histName + "_evtList");

  bool localDoWeights = params_.doEffCorrections && (!noCuts) && 
    (cutOverride.Length() < 1);

  if (!localDoWeights)
    std::cout << "no weighting of histogram " << histName << '\n';
  //static unsigned int const maxJets = 6;
  activateBranches(*theTree, isElectron);
  Float_t         JetPFCor_Pt[maxJets];
  Float_t         JetPFCor_Eta[maxJets];
  Float_t         Mass2j_PFCor;
  Float_t         MassV2j_PFCor;
  Float_t         event_met_pfmet;
  Int_t           event_nPV;
  //Int_t           evtNJ;
  Float_t         lepton_pt;
  Float_t         lepton_eta;
  Float_t         W_mt;
  Float_t         W_H_mass_gen;
  Float_t         interferencewt(1.0);
  Float_t effwt;
  Float_t puwt;

  TTreeFormula poi("poi", params_.var, theTree);

  theTree->SetBranchAddress("Mass2j_PFCor", &Mass2j_PFCor);
  theTree->SetBranchAddress("MassV2j_PFCor", &MassV2j_PFCor);

//   int PoiMatch = theTree->SetBranchAddress(params_.var, &poi);
//   bool goodPoi = true;
//   if (PoiMatch != 0) goodPoi = false;

  if (localDoWeights) {
    theTree->SetBranchAddress("JetPFCor_Pt",JetPFCor_Pt);
    theTree->SetBranchAddress("JetPFCor_Eta",JetPFCor_Eta);
    theTree->SetBranchAddress("event_nPV", &event_nPV);
    theTree->SetBranchAddress("event_met_pfmet", &event_met_pfmet);
    //theTree->SetBranchAddress("ggdevt", &evtNJ);
    if(isElectron) {
      theTree->SetBranchAddress("W_electron_pt", &lepton_pt);
      theTree->SetBranchAddress("W_electron_eta", &lepton_eta);
    } else {
      theTree->SetBranchAddress("W_muon_pt", &lepton_pt);
      theTree->SetBranchAddress("W_muon_eta", &lepton_eta);
    }
    theTree->SetBranchAddress("W_mt", &W_mt);
    theTree->SetBranchAddress("effwt", &effwt);
    theTree->SetBranchAddress("puwt", &puwt);
  }

  if (CPweights)
    theTree->SetBranchAddress("W_H_mass_gen", &W_H_mass_gen);

  switch (interfereWgt) {
  case 1:
    theTree->SetBranchAddress(TString::Format("interferencewtggH%i", 
					      int(params_.mHiggs)),
			      &interferencewt);
    break;
  case 2:
    theTree->SetBranchAddress(TString::Format("interferencewt_upggH%i", 
					      int(params_.mHiggs)),
			      &interferencewt);
    break;
  case 3:
    theTree->SetBranchAddress(TString::Format("interferencewt_downggH%i", 
					      int(params_.mHiggs)),
			      &interferencewt);
    break;
  }

//   static TRandom3 rnd(987654321);
//   double epochSelector = rnd.Rndm(), sumLumi = 0.;
//   int lumiSize = (isElectron) ? params_.lumiPerEpochElectron.size() :
//     params_.lumiPerEpochMuon.size();
//   int epoch = -1;
  double evtWgt = 1.0/*, hltEffJets = 1.0, hltEffMHT = 1.0*/;
//   std::vector<double> eff30(maxJets), eff25n30(maxJets);
  for (int event = 0; event < list->GetN(); ++event) {
    theTree->GetEntry(list->GetEntry(event));
    evtWgt = 1.0;
    if (localDoWeights) {
      // evtWgt = effWeight(lepton_pt, lepton_eta, W_mt, JetPFCor_Pt,
      // 			 JetPFCor_Eta, params_.njets, event_met_pfmet,
      // 			 isElectron);
      evtWgt = effwt*puwt;
    }
    if ((CPweights) && (params_.mHiggs > 0)) {
      evtWgt *= getCPweight(params_.mHiggs, params_.wHiggs, W_H_mass_gen);
    }
    if (interfereWgt) {
      evtWgt *= interferencewt;
    }
    theHist->Fill(poi.EvalInstance()*(1.+tmpScale), evtWgt);
  }

  delete theTree;

  theHist->SetDirectory(0);

  delete treeFile;

  //theHist->Print();
  return theHist;
}

RooAbsPdf * RooWjjFitterUtils::Hist2Pdf(TH1 * hist, TString pdfName,
					RooWorkspace& ws, int order,
					bool fast) const {
  if (ws.pdf(pdfName))
    return ws.pdf(pdfName);

  // hist->Print();
  if (fast) {
    RooTH1DPdf thePdf(pdfName, pdfName, *mjj_, *(dynamic_cast<TH1D*>(hist)),
		      order);
    ws.import(thePdf, RooFit::RecycleConflictNodes(), RooFit::Silence());
  } else {
    RooDataHist newHist(pdfName + "_hist", pdfName + "_hist",
			RooArgList(*mjj_), hist);
    // ws.import(newHist);

    RooHistPdf thePdf(pdfName, pdfName, RooArgSet(*massVar_),
		      RooArgSet(*mjj_), newHist, order);
    ws.import(thePdf, RooFit::RecycleConflictNodes(), RooFit::Silence());
  }

  return ws.pdf(pdfName);
}

RooDataSet * RooWjjFitterUtils::File2Dataset(TString fname, 
					     TString dsName, bool isElectron,
					     bool trunc, bool noCuts, 
					     bool weighted) const {
  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject(params_.treeName, theTree);
  if (!theTree) {
    std::cout << "failed to find tree " << params_.treeName << " in file " << fname 
	      << '\n';
    return 0;
  }

  theTree->Draw(">>" + dsName + "_evtList", 
		((noCuts) ? TString("") : fullCuts(trunc)),
		"goff");
  TEventList * list = (TEventList *)gDirectory->Get(dsName + "_evtList");

  activateBranches(*theTree, isElectron);

  Float_t         JetPFCor_Pt[maxJets];
  Float_t         JetPFCor_Eta[maxJets];
  Float_t         event_met_pfmet;
  Int_t           event_nPV;
  //Int_t           evtNJ;
  Float_t         lepton_pt;
  Float_t         lepton_eta;
  Float_t         W_mt;
  Float_t effwt;
  Float_t puwt;

  TTreeFormula poi("poi", params_.var, theTree);

  theTree->SetBranchAddress("JetPFCor_Pt",JetPFCor_Pt);
  theTree->SetBranchAddress("JetPFCor_Eta",JetPFCor_Eta);
  theTree->SetBranchAddress("event_nPV", &event_nPV);
  theTree->SetBranchAddress("event_met_pfmet", &event_met_pfmet);
  //theTree->SetBranchAddress("ggdevt", &evtNJ);
  if(isElectron) {
    theTree->SetBranchAddress("W_electron_pt", &lepton_pt);
    theTree->SetBranchAddress("W_electron_eta", &lepton_eta);
  } else {
    theTree->SetBranchAddress("W_muon_pt", &lepton_pt);
    theTree->SetBranchAddress("W_muon_eta", &lepton_eta);
  }
  theTree->SetBranchAddress("W_mt", &W_mt);
  theTree->SetBranchAddress("effwt", &effwt);
  theTree->SetBranchAddress("puwt", &puwt);

  RooArgSet cols(*mjj_);
  RooRealVar evtWgt("evtWgt", "evtWgt", 1.0);
  RooDataSet * ds;
  if (weighted) {
    cols.add(evtWgt);
    ds = new RooDataSet(dsName, dsName, cols, "evtWgt");
  } else
    ds = new RooDataSet(dsName, dsName, cols);

  //ds->Print();
  for (int event = 0; event < list->GetN(); ++event) {
    theTree->GetEntry(list->GetEntry(event));
    mjj_->setVal(poi.EvalInstance());
    ds->add(*mjj_, effwt*puwt);
  }
//   TTree * reducedTree = theTree;

//   TFile holder("holder_DELETE_ME.root", "recreate");
//   if (!noCuts) {
//     activateBranches(*theTree, isElectron);
//     std::cout << "full cuts: " << fullCuts(trunc) << '\n';
//     reducedTree = theTree->CopyTree( fullCuts(trunc) );
//     delete theTree;
//   }

  //reducedTree->Print();
//   RooDataSet * ds = new RooDataSet(dsName, dsName, reducedTree, 
// 				   RooArgSet(*mjj_));

  //delete reducedTree;
  delete treeFile;

  return ds;
}

TString RooWjjFitterUtils::fitCuts() const {
  if (params_.truncRange)
    return TString::Format("(((%s>%0.3f) && (%s<%0.3f)) || "
			   "((%s>%0.3f) && (%s<%0.3f)))",
			   params_.var.Data(), params_.minFit,
			   params_.var.Data(), params_.minTrunc,
			   params_.var.Data(), params_.maxTrunc,
			   params_.var.Data(), params_.maxFit);
  else
    return TString::Format("(%s>%0.3f) && (%s<%0.3f)",
			   params_.var.Data(), params_.minFit,
			   params_.var.Data(), params_.maxFit);
}

TString RooWjjFitterUtils::fullCuts(bool trunc) const {
  TString theCut = TString::Format("(%s > %0.3f) && (%s < %0.3f)", 
				    params_.var.Data(), params_.minMass, 
				    params_.var.Data(), params_.maxMass);
  if (trunc) {
    theCut = fitCuts();
  }
  //theCut += TString::Format(" && (%s)", jetCut_.Data());
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

double RooWjjFitterUtils::effWeight(float lepton_pt, float lepton_eta, 
				    float W_mt, float * JetPFCor_Pt, 
				    float * JetPFCor_Eta, int Njets,
				    float event_met,
				    bool isElectron) const {

  static TRandom3 rnd(987654321);
  static std::vector<double> eff30(maxJets);
  static std::vector<double> eff25n30(maxJets);
  double epochSelector = rnd.Rndm(), sumLumi = 0.;
  int lumiSize = (isElectron) ? params_.lumiPerEpochElectron.size() :
    params_.lumiPerEpochMuon.size();
  int epoch = -1;
  double evtWgt = 1.0;

  while ( (epochSelector > sumLumi/params_.intLumi) && 
	  (++epoch < lumiSize) ) {
    if (isElectron)
      sumLumi += params_.lumiPerEpochElectron[epoch];
    else
      sumLumi += params_.lumiPerEpochMuon[epoch];
  }
  if (isElectron) {
    std::cout << "start jets ... ";
    unsigned int nj(Njets);
    for (unsigned int i = 0; i < nj; ++i) {
      std::cout << "\njet " << i << " pt " << JetPFCor_Pt[i]
		<< " eta " << JetPFCor_Eta[i];
      eff30[i] = effJ30[epoch]->GetEfficiency(JetPFCor_Pt[i], 
					      JetPFCor_Eta[i]);
      std::cout << "\nnoj30 ";
      eff25n30[i] = effJ25NoJ30[epoch]->GetEfficiency(JetPFCor_Pt[i], 
						      JetPFCor_Eta[i]);
    }
    // std::cout << "done\nStart dijet ... ";
    evtWgt *= dijetEff(Njets, eff30, eff25n30);
    // std::cout << "done\nStart MHT ... ";
    evtWgt *= effMHT[epoch]->GetEfficiency(event_met, 0.);
    // std::cout << "done\nStart el reco ... ";
    evtWgt *= effEleReco[epoch]->GetEfficiency(lepton_pt, lepton_eta);
    // std::cout << "done\nStart el id ... ";
    evtWgt *= effEleId[epoch]->GetEfficiency(lepton_pt, lepton_eta);
    // std::cout << "done\nStart el ... ";
    evtWgt *= effEle[epoch]->GetEfficiency(lepton_pt, lepton_eta);
    // std::cout << "done\nStart el Mt ... ";
    evtWgt *= effEleWMt[epoch]->GetEfficiency(W_mt, lepton_eta);
    std::cout << "done" << std::endl;
  } else {
    evtWgt *= effMuId[epoch]->GetEfficiency(lepton_pt, lepton_eta);
    evtWgt *= effMu[epoch]->GetEfficiency(lepton_pt, lepton_eta);
  }

  return evtWgt;
}

void RooWjjFitterUtils::updatenjets() {
  jetCut_ = TString::Format("evtNJ==%d", params_.njets);
  if (params_.njets < 2)
    jetCut_ = "evtNJ==2 || evtNJ==3";
}

double RooWjjFitterUtils::computeChi2(RooHist& hist, RooAbsPdf& pdf, 
				      RooRealVar& obs, int& nbin, 
				      bool correct) {
  int np = hist.GetN();
  nbin = 0;
  double chi2(0);

  double x,y,eyl,eyh,exl,exh;
  double avg, pdfSig2;
  double Npdf = pdf.expectedEvents(RooArgSet(obs));
  TString className;
  RooAbsReal * binInt;
  RooAbsReal * fullInt = pdf.createIntegral(obs, obs);
  double sumN = 0., compN = 0., dataN = 0.;
  for (int i=0; i<np; ++i) {
    hist.GetPoint(i,x,y);
    eyl = hist.GetEYlow()[i];
    eyh = hist.GetEYhigh()[i];
    exl = hist.GetEXlow()[i];
    exh = hist.GetEXhigh()[i];

    obs.setVal(x);
    obs.setRange("binRange", x-exl, x+exh);
    binInt = pdf.createIntegral(obs, obs, "binRange");
    avg = Npdf*binInt->getVal()/fullInt->getVal();
    sumN += avg;
    delete binInt;

    pdfSig2 = 0.;
    className = pdf.ClassName();
//     std::cout << TString::Format("bin [%0.2f, %0.2f]", x-exl, x+exh);// << '\n';
    if (correct) {
      if (className == "RooHistPdf") {
	RooHistPdf& tmpPdf = dynamic_cast<RooHistPdf&>(pdf);
	pdfSig2 = sig2(tmpPdf, obs, avg);
      } else if (className == "RooAddPdf") {
	RooAddPdf& tmpPdf = dynamic_cast<RooAddPdf&>(pdf);
	pdfSig2 = sig2(tmpPdf, obs, avg);
      }
    }
    
//     std::cout << " y: " << y << " avg: " << avg 
// 	      << " eyl: " << eyl << " eyh: " << eyh 
// 	      << '\n';
    if (y != 0) {
      ++nbin;
      compN += avg;
      dataN += y;
      double pull2 = (y-avg)*(y-avg);
      pull2 = (y>avg) ? pull2/(eyl*eyl + pdfSig2) : pull2/(eyh*eyh + pdfSig2) ;
      chi2 += pull2;
    }
  }
//   std::cout << "Npdf: " << Npdf << " sumN: " << sumN 
// 	    << " compN: " << compN << " dataN: " << dataN
// 	    << '\n';
  delete fullInt;
  return chi2;
}

TLegend * RooWjjFitterUtils::legend4Plot(RooPlot * plot, bool left) {
  TObject * theObj;
  TString objName, objTitle;
  //  TLegend * theLeg = new TLegend(0.70, 0.65, 0.92, 0.92, "", "NDC");
  TLegend * theLeg;
  if (left) 
    theLeg = new TLegend(0.2, 0.62, 0.55, 0.92, "", "NDC");
  else
    theLeg = new TLegend(0.65, 0.62, 0.92, 0.92, "", "NDC");
  theLeg->SetName("theLegend");

  theLeg->SetBorderSize(0);
  theLeg->SetLineColor(0);
  theLeg->SetFillColor(0);
  theLeg->SetFillStyle(0);
  theLeg->SetLineWidth(0);
  theLeg->SetLineStyle(0);
  theLeg->SetTextFont(42);
  theLeg->SetTextSize(.045);

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
  theLeg->SetY1NDC(0.9 - 0.05*entryCnt - 0.005);
  theLeg->SetY1(theLeg->GetY1NDC());
  return theLeg;
}

void RooWjjFitterUtils::activateBranches(TTree& t, bool isElectron) {
  t.SetBranchStatus("*",    0);
  t.SetBranchStatus("JetPFCor_Pt",    1);
  t.SetBranchStatus("JetPFCor_Px",    1);
  t.SetBranchStatus("JetPFCor_Py",    1);
  t.SetBranchStatus("JetPFCor_Pz",    1);
  t.SetBranchStatus("JetPFCor_Eta",    1);
  t.SetBranchStatus("JetPFCor_Phi",    1);
  t.SetBranchStatus("JetPFCor_dphiMET",    1);
  t.SetBranchStatus("JetPFCor_bDiscriminator",    1);
  t.SetBranchStatus("JetPFCor_QGLikelihood",    1);

  t.SetBranchStatus("event_met_pfmet",    1);
  t.SetBranchStatus("event_met_pfmetPhi",    1);
  t.SetBranchStatus("event_met_pfmetsignificance",    1);
  // t.SetBranchStatus("event_BeamSpot_x",    1);
  // t.SetBranchStatus("event_BeamSpot_y",    1);
  // t.SetBranchStatus("event_RhoForLeptonIsolation",    1);
  t.SetBranchStatus("event_nPV",    1);
  t.SetBranchStatus("effwt", 1);
  t.SetBranchStatus("puwt", 1);


  if (isElectron) {
    t.SetBranchStatus("W_electron_*", 1);
  } else {
    t.SetBranchStatus("W_muon_*", 1);
  }

  if ((t.FindBranch("mva2j400mu")) || (t.FindBranch("mva2j400el")))
    t.SetBranchStatus("mva*", 1);
  if (t.FindBranch("qgld_Summer11CHS"))
    t.SetBranchStatus("qgld*", 1);
  //t.SetBranchStatus("ang*", 1);
  if (t.FindBranch("fit_mlvjj"))
    t.SetBranchStatus("*lvjj", 1);
  t.SetBranchStatus("W_mt",    1);
  t.SetBranchStatus("W_pt",    1);
  t.SetBranchStatus("W_pzNu1",    1);
  t.SetBranchStatus("W_pzNu2",    1);
  if (t.FindBranch("fit_status"))
    t.SetBranchStatus("fit_status",    1);
  if (t.FindBranch("ggdevt"))
    t.SetBranchStatus("ggdevt",    1);
  if (t.FindBranch("fit_chi2"))
    t.SetBranchStatus("fit_chi2",    1);
  if (t.FindBranch("fit_NDF"))
    t.SetBranchStatus("fit_NDF",    1);
//   t.SetBranchStatus("fit_mlvjj", 1);
  if (t.FindBranch("evtNJ"))
    t.SetBranchStatus("evtNJ",    1);
  if (t.FindBranch("vbf_event"))
    t.SetBranchStatus("vbf_*", 1);

  t.SetBranchStatus("Mass2j_PFCor",    1);
  t.SetBranchStatus("MassV2j_PFCor",    1);

  if (t.FindBranch("W_H_mass_gen"))
    t.SetBranchStatus("W_H_mass_gen", 1);
  if (t.FindBranch("interferencewtggH600"))
    t.SetBranchStatus("interferencewt*", 1);
}

double RooWjjFitterUtils::dijetEff(unsigned int Njets, 
				   std::vector<double> const& eff30,
				   std::vector<double> const& eff25n30) {

//   if (Njets == 2) {
//     return (eff30[0]*eff30[1]+eff30[0]*eff25n30[1]+eff30[1]*eff25n30[0]);
//   }
  int N = int(TMath::Power(3, int(Njets))), n, t, oneCnt, twoCnt;
  unsigned int i;
  std::vector<int> digits(Njets, 0);
  double theEff(0.), tmpEff(1.0);
  for(n=0; n<N; ++n) {
    t = n;
    oneCnt = 0;
    twoCnt = 0;
    i = digits.size()-1;

    //base 3 conversion
    while (t>0) {
      digits[i] = t%3;
      t /= 3; 
      switch(digits[i--]){
      case 1:
	oneCnt++;
	break;
      case 2:
	twoCnt++;
	break;
      }
    }

    if ((twoCnt > 0) && ((twoCnt > 1) || (oneCnt > 0))) {
      tmpEff = 1.0;
      for (i = 0; i<digits.size(); ++i) {
	switch (digits[i]) {
	case 0:
	  tmpEff *= 1. - eff30[i] - eff25n30[i];
	  break;
	case 1:
	  tmpEff *= eff25n30[i];
	  break;
	case 2:
	  tmpEff *= eff30[i];
	  break;
	}
      }
      theEff += tmpEff;
    }
  }
  if ((theEff > 1.0) || (theEff <= 0.)) {
    std::cout << "Njets: " << Njets << '\n';
    for(i=0; i<Njets; ++i)
      std::cout << "jet " << i << " eff30: " << eff30[i] 
		<< " eff25n30: " << eff25n30[i]
		<< '\n';
    assert(true);
  }
  return theEff;
}

double RooWjjFitterUtils::getCPweight(double mH, double wH, double m, 
				      int BWflag) {
  static pwhegwrapper phw;
  return phw.getweight(mH, wH, 172.5, m, BWflag);
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
    RooAbsReal * fullInt = ipdf->createIntegral(obs, RooFit::NormSet(obs));
    if ((allCoefs) && (coef)) {
      iN = coef->getVal();
      RooAbsReal * binInt = ipdf->createIntegral(obs, RooFit::NormSet(obs),
						 RooFit::Range("binRange"));
      iN *= binInt->getVal()/fullInt->getVal();
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
      iN *= binInt->getVal()/fullInt->getVal();
      delete binInt;
    }
    delete fullInt;
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
