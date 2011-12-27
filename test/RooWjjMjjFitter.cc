#include "RooWjjMjjFitter.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TMath.h"
#include "TLine.h"
#include "TLegend.h"

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooProdPdf.h"
#include "RooRandom.h"
#include "RooAbsBinning.h"
#include "RooTreeDataStore.h"
#include "RooGenericPdf.h"

#include "TPad.h"


// #include "ComputeChi2.C"

using namespace RooFit;

RooWjjMjjFitter::RooWjjMjjFitter() :
  ws_("ws", "ws"), initWjets_(0.), initDiboson_(0.),
  ttbarNorm_(0.), singleTopNorm_(0.), zjetsNorm_(0.),
  QCDNorm_(0.), QCDError_(0.), errorType_(RooAbsData::SumW2)
{
}

RooWjjMjjFitter::RooWjjMjjFitter(RooWjjFitterParams & pars) :
  ws_("ws", "ws"), params_(pars), utils_(pars),
  initWjets_(0.), initDiboson_(0.), ttbarNorm_(0.), singleTopNorm_(0.), 
  zjetsNorm_(0.), QCDNorm_(0.), QCDError_(0.),errorType_(RooAbsData::SumW2)
{
  utils_.vars2ws(ws_);
  RooRealVar * mass = ws_.var(params_.var);
  mass->setRange("lowSideBand", params_.minFit, params_.minTrunc);
  mass->setRange("highSideBand", params_.maxTrunc, params_.maxFit);
  mass->setRange("fitRange", params_.minFit, params_.maxFit);

  rangeString_ = "fitRange";
  if (params_.truncRange) {
    // data = loadData(true);
    rangeString_ = "lowSideBand,highSideBand";
  }
}

RooFitResult * RooWjjMjjFitter::fit() {

  RooAbsPdf * totalPdf = makeFitter(true);
  // RooRealVar * mass = ws_.var(params_.var);

  RooAbsData * data = loadData();
  cout << "Made dataset" << endl;

  std::cout << "-------- Number of expected QCD events: " << QCDNorm_
	    << std::endl;

  RooFitResult *fitResult = 0;
  
  resetYields();
  RooArgSet * params = totalPdf->getParameters(data);
  loadParameters(params_.constraintParamsFile);
  
  RooRealVar * nQCD = ws_.var("nQCD");
  RooGaussian constQCD("constQCD","constQCD", *nQCD, RooConst(nQCD->getVal()),
		       RooConst(nQCD->getError())) ;
  RooRealVar * nTTbar = ws_.var("nTTbar");
  RooGaussian constTTbar("constTTbar","constTTbar", *nTTbar, 
			 RooConst(nTTbar->getVal()),
			 RooConst(nTTbar->getError())) ;
  RooRealVar * nSingleTop = ws_.var("nSingleTop");
  RooGaussian constSingleTop("constSingleTop","constSingleTop", *nSingleTop, 
			     RooConst(nSingleTop->getVal()),
			     RooConst(nSingleTop->getError())) ;
  RooRealVar * nZjets = ws_.var("nZjets");
  RooGaussian constZpJ("constZpJ", "constZpJ", *nZjets, 
		       RooConst(nZjets->getVal()), 
		       RooConst(nZjets->getError()));
  RooRealVar * nDiboson = ws_.var("nDiboson");
  RooGaussian constDiboson("constDiboson", "constDiboson", *nDiboson,
			   RooConst(nDiboson->getVal()), 
			   RooConst(nDiboson->getError()));
  
  RooArgList ConstrainedVars;
  RooArgList Constraints;
  
  if (params_.constrainDiboson) {
    Constraints.add(constDiboson);
    ConstrainedVars.add(*nDiboson);
  }
  Constraints.add(constQCD);
  ConstrainedVars.add(*nQCD);
  Constraints.add(constSingleTop);
  ConstrainedVars.add(*nSingleTop);
  Constraints.add(constZpJ);
  ConstrainedVars.add(*nZjets);
  Constraints.add(constTTbar);
  ConstrainedVars.add(*nTTbar);
  
  
  
  loadParameters(params_.initParamsFile);
  //cout << "params_.e_fSU=" << params_.e_fSU << endl;
  
  ws_.Print();
  std::cout << "\n***constraints***\n";
  TIter con(Constraints.createIterator());
  RooGaussian * tc;
  while ((tc = (RooGaussian *)con()))
    tc->Print();
  std::cout << "*** ***\n\n";
  
  RooAbsPdf * fitPdf = totalPdf;
  if (!params_.externalConstraints) {
    Constraints.add(*totalPdf);
    fitPdf = new RooProdPdf("fitPdf", "fitPdf", Constraints);
  }
  fitResult = fitPdf->fitTo(*data, Save(true), 
			    ( (params_.externalConstraints) ?
			      ExternalConstraints(Constraints) :
			      Constrained() ),
			    RooFit::Extended(true), 
			    RooFit::Minos(false), 
			    RooFit::Hesse(true),
			    PrintEvalErrors(-1),
			    RooFit::Range(rangeString_),
			    Warnings(false) 
			    );
  
  fitResult->Print("v");
  params->writeToFile("lastWjjFitParams.txt");
  
//    if (params_.fitToyDataset) {
//      char NJ_char[5];
//      sprintf(NJ_char,"%i",params_.njets);
//      TString NJ_str=NJ_char;

//      TFile *fAll = new TFile("/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/ToyValidation_results/FitSummary_1K_"+NJ_str+"j.root", "UPDATE");
//      fAll->cd();
//      TString fRname;
//      fRname=params_.toydataFile;
//      fRname="fit_"+fRname;
//      fitResult->Write(fRname);
//      fAll->Close();
//    }

  delete params;
  return fitResult;
}

RooPlot * RooWjjMjjFitter::computeChi2(double& chi2, int& ndf) {

  RooRealVar * mass = ws_.var(params_.var);
  RooAbsBinning& plotBins =  mass->getBinning("plotBinning");
  mass->setBinning(plotBins);
  RooAbsData * data = ws_.data("data");

  TH1 * dataHist = utils_.newEmptyHist("theData");
  RooTreeDataStore * dataStore = 
    dynamic_cast<RooTreeDataStore *>(data->store());
  dataStore->tree().Draw(TString::Format("%s>>%s", mass->GetName(),
					 "theData"),
			 utils_.fitCuts(), "goff");
  //dataHist->Scale(1., "width");
  RooHist h_data(*dataHist, 0., 1, errorType_, 1.0,
		 false);
  RooAbsPdf * totalPdf = ws_.pdf("totalPdf");


  int chi2bins;
  chi2 = RooWjjFitterUtils::computeChi2(h_data, *totalPdf, *mass, chi2bins);

  chi2bins -= ndf;
  std::cout << "\n *** chi^2/dof = " << chi2 << "/" << chi2bins << " = " 
	    << chi2/chi2bins << " ***\n"
	    << " *** chi^2 probability = " << TMath::Prob(chi2, chi2bins)
	    << " ***\n\n";

  delete dataHist;
  ndf = chi2bins;
  return 0;
}

RooAbsPdf * RooWjjMjjFitter::makeFitter(bool allOne) {

  if (ws_.pdf("totalPdf"))
    return ws_.pdf("totalPdf");

  RooRealVar * mass = ws_.var(params_.var);
  RooAbsPdf * dibosonPdf = makeDibosonPdf();
  makeWpJPdf();
  RooAbsPdf * ttPdf = makettbarPdf();
  RooAbsPdf * stPdf = makeSingleTopPdf();
  RooAbsPdf * qcdPdf = makeQCDPdf();
  RooAbsPdf * ZpJPdf =  makeZpJPdf();

  RooRealVar nWjets("nWjets","nWjets", initWjets_, 0.0, 100000.);
  nWjets.setError(TMath::Sqrt(initWjets_));
  RooRealVar nDiboson("nDiboson","nDiboson",  initDiboson_, 0.0, 10000.);
  nDiboson.setError(initDiboson_*0.15);
  RooRealVar nTTbar("nTTbar","", ttbarNorm_);
  nTTbar.setError(ttbarNorm_*0.063);
  RooRealVar nSingleTop("nSingleTop","", singleTopNorm_);
  nSingleTop.setError(singleTopNorm_*0.05);
  RooRealVar nQCD("nQCD","nQCD", QCDNorm_);
  nQCD.setError(QCDError_);
  RooRealVar nZjets("nZjets","nZjets", zjetsNorm_);
  nZjets.setError(zjetsNorm_*0.043);
  RooRealVar nNP("nNP", "N_{new physics}", 0., 0., 10000.);
  nNP.setError(100.);

  RooAbsPdf * WpJPdfMU = ws_.pdf("WpJPdfMU");
  RooRealVar * fMU = ws_.var("fMU");
  RooFormulaVar NMU("NMU", "@0*@1*(@1 >= 0.)", RooArgList(nWjets,*fMU));
  RooAbsPdf * WpJPdfMD = ws_.pdf("WpJPdfMD");
  RooFormulaVar NMD("NMD", "@0*@1*(-1)*(@1 < 0.)", RooArgList(nWjets,*fMU));
  RooAbsPdf * WpJPdfSU = ws_.pdf("WpJPdfSU");
  RooRealVar * fSU = ws_.var("fSU");
  RooFormulaVar NSU("NSU", "@0*@1*(@1 >= 0.)", RooArgList(nWjets,*fSU));
  RooAbsPdf * WpJPdfSD = ws_.pdf("WpJPdfSD");
  RooFormulaVar NSD("NSD", "@0*@1*(-1)*(@1 < 0.)", RooArgList(nWjets,*fSU));

  RooAbsPdf * WpJPdfNom = ws_.pdf("WpJPdfNom");
  RooFormulaVar NNom("NNom", "@0*(1.-abs(@1)-abs(@2))",
		     RooArgList(nWjets,*fMU,*fSU));


  RooRealVar turnOn("turnOn","turnOn", 50.);
  turnOn.setConstant(false);
  RooRealVar width("width","width", 5., 0., 100.);
  RooRealVar seff("seff", "seff", 7000., 0., 10000.);
  seff.setConstant(true);
  RooRealVar power("power", "power", 4, 0., 100.);
  RooRealVar power2("power2", "power2", 2, 0., 100.);
  RooRealVar decay(power, "decay");
  RooRealVar fturnon("fturnOn", "fturnOn", 0.5, 0., 1.);
  RooRealVar width2(width);
  width2.SetName("width2");
  RooRealVar turnOn2(turnOn, "turnOn2");
  RooGenericPdf bkgErfExp("WpJPdfAll","WpJPdfAll",
// 			  "TMath::Power(1-@0/@5,decay)/TMath::Power(@0,@3+@4*log(@0/@5))"
 			  "1./TMath::Power(@0,@3+@4*log(@0/@5))"
// 			  "1./TMath::Power(@0,@3)"
// 			  "exp(-@0/@3)"
//  			  "*(fturnOn*(TMath::Erf((@0-@1)/@2)+1) + "
// 			  "(1-fturnOn)*(TMath::Erf((@0-@6)/@2)+1))",
 			  "*(TMath::Erf((@0-@1)/@2)+1)",
// 			  "*1./(1+exp(-(@0-@1)/@2))",
			  RooArgList(*mass, turnOn, width, power, power2,
				     seff));

  RooArgList components(*dibosonPdf);
  RooArgList yields(nDiboson);
  if (allOne) {
    components.add(bkgErfExp);
    yields.add(nWjets);
  } else {
    components.add(RooArgList(*WpJPdfMU,*WpJPdfMD,*WpJPdfSU,
			      *WpJPdfSD,*WpJPdfNom));
    yields.add(RooArgList(NMU, NMD, NSU, NSD, NNom));
//     components.add(*WpJPdf);
//     yields.add(nWjets);
  }
  components.add(RooArgList(*ttPdf, *stPdf, *qcdPdf, *ZpJPdf));
  yields.add(RooArgList(nTTbar, nSingleTop, nQCD, nZjets));
  
  if (params_.doNewPhysics) {
    RooAbsPdf * NPPdf = makeNPPdf();
    components.add(*NPPdf);
    yields.add(nNP);
  }
  RooAddPdf totalPdf("totalPdf","extended sum pdf", components, yields);

  ws_.import(totalPdf);

  return ws_.pdf("totalPdf"); 
}

RooAbsPdf * RooWjjMjjFitter::make4BodyPdf(RooWjjMjjFitter & fitter2body) {
  if (ws_.pdf("totalPdf4"))
    return ws_.pdf("totalPdf4");

  makeFitter();
  loadParameters(params_.initParamsFile);

  RooAbsPdf * dibosonPdf = makeDibosonPdf();
  RooAbsPdf * WpJPdf = makeWpJ4BodyPdf(fitter2body);
  RooAbsPdf * ttPdf = makettbarPdf();
  RooAbsPdf * stPdf = makeSingleTopPdf();
  RooAbsPdf * qcdPdf = makeQCDPdf();
  RooAbsPdf * ZpJPdf =  makeZpJPdf();

  RooRealVar * nWjets = ws_.var("nWjets");
  RooRealVar * nDiboson = ws_.var("nDiboson");
  RooRealVar * nTTbar = ws_.var("nTTbar");
  RooRealVar * nSingleTop = ws_.var("nSingleTop");
  RooRealVar * nQCD = ws_.var("nQCD");
  RooRealVar * nZjets = ws_.var("nZjets");

  RooArgList components(*dibosonPdf);
  RooArgList yields(*nDiboson);
  components.add(*WpJPdf);
  yields.add(*nWjets);
  components.add(RooArgList(*ttPdf, *stPdf, *qcdPdf, *ZpJPdf));
  yields.add(RooArgList(*nTTbar, *nSingleTop, *nQCD, *nZjets));

  if (params_.doNewPhysics) {
    RooAbsPdf * NPPdf = makeNPPdf();
    RooRealVar * nNP = ws_.var("nNP");
    components.add(*NPPdf);
    yields.add(*nNP);
  }

  RooAddPdf totalPdf4("totalPdf4", "totalPdf4", components, yields);

  ws_.import(totalPdf4);

  return ws_.pdf("totalPdf4");
}

RooAbsData * RooWjjMjjFitter::loadData(bool trunc) {
  if ((trunc) && (ws_.data("truncData")))
    return ws_.data("truncData");
  if ((!trunc) && (ws_.data("data")))
    return ws_.data("data");

  RooRealVar * mass = ws_.var(params_.var);

  TString dataName("data");
  if (trunc)
    dataName = "truncData";

  RooDataSet data(dataName, dataName, RooArgSet(*mass));
  QCDNorm_ = 0.;
  QCDError_ = 0.;

  double rel2jet = 0.0663, rel3jet = 0.0229, rmu2jet = 0.001625, rmu3jet = 0.;
  double erel2jet = rel2jet*0.5, erel3jet = rel3jet*0.5;
  double ermu2jet = 0.004214, ermu3jet = 0.0040797;

  if (params_.fitToyDataset) {
    RooDataSet * tds = utils_.File2Dataset(params_.ToyDatasetDirectory + 
					   params_.toydataFile, 
					   "data_muon",false,
					   trunc, true);
    tds->Print();

    double fracel=0.5;
    double fracmu=0.5;
    if ( params_.njets==3 ) {
      fracel=0.442;
      fracmu=0.558;
      QCDNorm_ += (rmu3jet*fracmu+rel3jet*fracel)*tds->sumEntries();
      QCDError_ += TMath::Power(ermu3jet*fracmu*tds->sumEntries(),2);
      QCDError_ += TMath::Power(erel3jet*fracel*tds->sumEntries(),2);
    } else {
      fracel=0.434;
      fracmu=0.566;
      QCDNorm_ += (rmu2jet*fracmu+rel2jet*fracel)*tds->sumEntries();
      QCDError_ += TMath::Power(ermu2jet*fracmu*tds->sumEntries(),2);
      QCDError_ += TMath::Power(erel2jet*fracel*tds->sumEntries(),2);
    }
    data.append(*tds);
    delete tds;

  } else {
    if (params_.includeMuons) {
      RooDataSet * mds = utils_.File2Dataset(params_.DataDirectory + 
					     params_.muonData, "data_muon", 
					     false, trunc, false);
      mds->Print();

      /// Note: for the 2+3 jets we use the 2jet bin coefficients
      if ( params_.njets==3 ) {
	QCDNorm_ += rmu3jet*mds->sumEntries();
	QCDError_ += TMath::Power(ermu3jet*mds->sumEntries(),2);
      } else {
	QCDNorm_ += rmu2jet*mds->sumEntries();
	QCDError_ += TMath::Power(ermu2jet*mds->sumEntries(),2);
      }
      data.append(*mds);
      delete mds;
    }
    if (params_.includeElectrons) {
      RooDataSet * eds = utils_.File2Dataset(params_.DataDirectory + 
					     params_.electronData, 
					     "data_electron", true, trunc, 
					     false);
      eds->Print();
      if ( params_.njets==3 ) {
	QCDNorm_ += rel3jet*eds->sumEntries();
	QCDError_ += TMath::Power(erel3jet*eds->sumEntries(),2);
      } else {
	QCDNorm_ += rel2jet*eds->sumEntries();
	QCDError_ += TMath::Power(erel2jet*eds->sumEntries(),2);
      }

      data.append(*eds);
      delete eds;
    }
  }
  QCDError_ = TMath::Sqrt(QCDError_);
  ws_.import(data);

  return ws_.data(dataName);
}

RooAbsPdf * RooWjjMjjFitter::makeDibosonPdf() {

  //Scale the trees by the Crossection/Ngenerated (43/4225916=1.01753087377979123e-05 for WW and 18/4265243=4.22015814808206740e-06 for WZ).
  if (ws_.pdf("dibosonPdf"))
    return ws_.pdf("dibosonPdf");

  double WWweight = 53./4225916.;
  double WZweight = 17./4265243.;
  double dibosonScale = 2.;
  TH1 * th1diboson = utils_.newEmptyHist("th1diboson", dibosonScale);

  TH1 * tmpHist;

  if (params_.includeMuons) {
    tmpHist  = utils_.File2Hist(params_.MCDirectory+"mu_WW_CMSSW428.root",
				"hist_ww_mu", false, 0, false, dibosonScale);
    th1diboson->Add(tmpHist, WWweight);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory+"mu_WZ_CMSSW428.root",
			       "hist_wz_mu", false, 0, false, dibosonScale);
    th1diboson->Add(tmpHist, WZweight);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist  = utils_.File2Hist(params_.MCDirectory+"el_WW_CMSSW428.root",
				"hist_ww_el", true, 0, false, dibosonScale);
    th1diboson->Add(tmpHist, WWweight);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory+"el_WZ_CMSSW428.root",
			       "hist_wz_el", true, 0, false, dibosonScale);
    th1diboson->Add(tmpHist, WZweight);
    delete tmpHist;
  }

//   std::cout << "min bin: " << th1diboson->FindBin(params_.minFit)
// 	    << " max bin: " << th1diboson->FindBin(params_.maxFit) 
// 	    << " n bins: " << th1diboson->GetNbinsX()
// 	    << '\n';
  initDiboson_ = th1diboson->Integral(th1diboson->FindBin(params_.minFit),
				      th1diboson->FindBin(params_.maxFit)-1) * 
    params_.intLumi;
  cout << "-------- Number of expected WW+WZ events = " 
       << th1diboson->Integral() << " x " << params_.intLumi 
       << " = " <<  initDiboson_ << endl;

  th1diboson->Scale(1., "width");
  RooAbsPdf * dibosonPdf = utils_.Hist2Pdf(th1diboson, "dibosonPdf",
					   ws_);
  delete th1diboson;
  return dibosonPdf;
}

RooAbsPdf * RooWjjMjjFitter::makeWpJPdf() {  

  if (ws_.pdf("WpJPdf"))
    return ws_.pdf("WpJPdf");

  TH1 * th1WpJ = utils_.newEmptyHist("th1WpJ");
  TH1 * th1WpJMU = utils_.newEmptyHist("th1WpJMU");
  TH1 * th1WpJMD = utils_.newEmptyHist("th1WpJMD");
  TH1 * th1WpJSU = utils_.newEmptyHist("th1WpJSU");
  TH1 * th1WpJSD = utils_.newEmptyHist("th1WpJSD");

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.WpJDirectory + "mu_WpJ_CMSSW428.root",
			       "hist_wpj_mu", false, 1, params_.toyWpJ);
    th1WpJ->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJmatchingup_CMSSW428.root",
			       "hist_wpj_mu_mu", false, 1, params_.toyWpJ);
    th1WpJMU->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJmatchingdown_CMSSW428.root",
			       "hist_wpj_mu_md", false, 1, params_.toyWpJ);
    th1WpJMD->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJscaleup_CMSSW428.root",
			       "hist_wpj_mu_su", false, 1, params_.toyWpJ);
    th1WpJSU->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJscaledown_CMSSW428.root",
			       "hist_wpj_mu_sd", false, 1, params_.toyWpJ);
    th1WpJSD->Add(tmpHist);
    delete tmpHist;
  }

  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.WpJDirectory + "el_WpJ_CMSSW428.root",
			       "hist_wpj_el", true, 1, params_.toyWpJ);
    th1WpJ->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJmatchingup_CMSSW428.root",
			       "hist_wpj_el_mu", true, 1, params_.toyWpJ);
    th1WpJMU->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJmatchingdown_CMSSW428.root",
			       "hist_wpj_el_md", true, 1, params_.toyWpJ);
    th1WpJMD->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJscaleup_CMSSW428.root",
			       "hist_wpj_el_su", true, 1, params_.toyWpJ);
    th1WpJSU->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJscaledown_CMSSW428.root",
			       "hist_wpj_el_sd", true, 1, params_.toyWpJ);
    th1WpJSD->Add(tmpHist);
    delete tmpHist;
  }

  initWjets_ = (31314./81352581.) * 
    (th1WpJ->Integral(th1WpJ->FindBin(params_.minFit),
		      th1WpJ->FindBin(params_.maxFit)-1)) * 
    params_.intLumi;
  cout << "-------- Number of expected Wjj events = " <<  initWjets_ << endl;

//   th1WpJ->Draw();
//   gPad->Update();
//   gPad->WaitPrimitive();

  th1WpJ->Scale(1, "width");
  th1WpJMU->Scale(1, "width");
  th1WpJMD->Scale(1, "width");
  th1WpJSU->Scale(1, "width");
  th1WpJSD->Scale(1, "width");

//   th1WpJ->Draw();
//   gPad->Update();
//   gPad->WaitPrimitive();

  RooArgList pdfs;
  RooArgList coefs;

  RooAbsPdf * WpJPdfSU = utils_.Hist2Pdf(th1WpJSU, "WpJPdfSU", ws_);
  RooRealVar fSU("fSU", "f_{scaleUp}", 0., 0., 1.);
  fSU.setConstant();
  pdfs.add(*WpJPdfSU);
  coefs.add(fSU);

  RooAbsPdf * WpJPdfSD = utils_.Hist2Pdf(th1WpJSD, "WpJPdfSD", ws_);
  RooRealVar fSD("fSD", "f_{scaleDown}", 0., 0., 1.);
  fSD.setConstant();
  pdfs.add(*WpJPdfSD);
  coefs.add(fSD);

  RooAbsPdf * WpJPdfMU = utils_.Hist2Pdf(th1WpJMU, "WpJPdfMU", ws_);
  RooRealVar fMU("fMU", "f_{matchingUp}", 0., 0., 1.);
  fMU.setConstant();
  pdfs.add(*WpJPdfMU);
  coefs.add(fMU);

  RooAbsPdf * WpJPdfMD = utils_.Hist2Pdf(th1WpJMD, "WpJPdfMD", ws_);
  RooRealVar fMD("fMD", "f_{matchingDown}", 0., 0., 1.);
  fMD.setConstant();
  pdfs.add(*WpJPdfMD);
  coefs.add(fMD);

  RooAbsPdf * WpJPdfNom = utils_.Hist2Pdf(th1WpJ, "WpJPdfNom", ws_);
  pdfs.add(*WpJPdfNom);

  RooAddPdf WpJPdf("WpJPdf","WpJPdf", pdfs, coefs, false);
  ws_.import(WpJPdf);


  delete th1WpJSU;
  delete th1WpJSD;
  delete th1WpJMU;
  delete th1WpJMD;
  delete th1WpJ;
  return ws_.pdf("WpJPdf");
  //return WjetsShape;
}

RooAbsPdf * RooWjjMjjFitter::makettbarPdf() {

  if (ws_.pdf("ttPdf"))
    return ws_.pdf("ttPdf");

  double ttScale = 1.;
  TH1 * th1tt = utils_.newEmptyHist("th1tt", ttScale);

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_TTbar_CMSSW428.root",
			       "hist_tt_mu", false, 1, false, ttScale);
    th1tt->Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_TTbar_CMSSW428.root",
			       "hist_tt_el", true, 1, false, ttScale);
    th1tt->Add(tmpHist);
    delete tmpHist;
  }

  ttbarNorm_ = (157.5/3701947) * 
    th1tt->Integral(th1tt->FindBin(params_.minFit),
		    th1tt->FindBin(params_.maxFit)-1) * 
    params_.intLumi;
  cout << "-------- Number of expected ttbar events = " 
       << th1tt->Integral() << " x " << params_.intLumi << " = " 
       << ttbarNorm_ << endl;

  th1tt->Scale(1., "width");
  RooAbsPdf * ttPdf = utils_.Hist2Pdf(th1tt, "ttPdf", ws_);
  delete th1tt;
  return ttPdf;

}

RooAbsPdf * RooWjjMjjFitter::makeSingleTopPdf() {

  if (ws_.pdf("stPdf"))
    return ws_.pdf("stPdf");

  // --------- s channel cross section: Tbar - 1.44 pb, events_gen = 137980; T - 3.19 pb, events_gen = 259971
  // --------- t channel cross section: Tbar - 22.65 pb, events_gen = 1944826; T - 41.92 pb, events_gen = 3900171
  // --------- tW channel cross section: Tbar - 7.87 pb, events_gen = 787629; T - 7.87 pb, events_gen = 795379
  // --------- https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma 

  double STopWeight = 3.19/259971;
  double STopBarWeight = 1.44/137980;
  double TTopWeight = 41.92/3900171;
  double TTopBarWeight = 22.65/1944826;
  double TWTopWeight = 7.87/795379;
  double TWTopBarWeight = 7.87/787629;

  std::cout << "s channel tbar: " << STopBarWeight
	    << " t: " << STopWeight << "\n"
	    << "t channel tbar: " << TTopBarWeight
	    << " t: " << TTopWeight << "\n"
	    << "tW channel tbar: " << TWTopBarWeight
	    << " t: " << TWTopWeight
	    << "\n";

  double stScale = 1.;

  TH1 * th1st = utils_.newEmptyHist("th1st", stScale);

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopS_Tbar_CMSSW428.root",
			       "hist_stbar_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, STopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopS_T_CMSSW428.root",
			       "hist_st_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, STopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopT_Tbar_CMSSW428.root",
			       "hist_stbar_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, TTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopT_T_CMSSW428.root",
			       "hist_st_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, TTopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopTW_Tbar_CMSSW428.root",
			       "hist_stbar_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, TWTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopTW_T_CMSSW428.root",
			       "hist_st_mu", false, 1, false, stScale);
    th1st->Add(tmpHist, TWTopWeight);
//     tmpHist->Print();
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopS_Tbar_CMSSW428.root",
			       "hist_stbar_el", true, 1, false, stScale);
    th1st->Add(tmpHist, STopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopS_T_CMSSW428.root",
			       "hist_st_el", true, 1, false, stScale);
    th1st->Add(tmpHist, STopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopT_Tbar_CMSSW428.root",
			       "hist_stbar_el", true, 1, false, stScale);
    th1st->Add(tmpHist, TTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopT_T_CMSSW428.root",
			       "hist_st_el", true, 1, false, stScale);
    th1st->Add(tmpHist, TTopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopTW_Tbar_CMSSW428.root",
			       "hist_stbar_el", true, 1, false, stScale);
    th1st->Add(tmpHist, TWTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopTW_T_CMSSW428.root",
			       "hist_st_el", true, 1, false, stScale);
    th1st->Add(tmpHist, TWTopWeight);
//     tmpHist->Print();
    delete tmpHist;
  }

  singleTopNorm_ = th1st->Integral(th1st->FindBin(params_.minFit),
				   th1st->FindBin(params_.maxFit)-1) * 
    params_.intLumi;

//   if (params_.includeElectrons && params_.includeMuons)
//     singleTopNorm_ *= 2;
  cout << "-------- Number of expected single top events = " << 
    th1st->Integral() << " x " << params_.intLumi << " = " << singleTopNorm_ << endl;

  th1st->Scale(1., "width");
  RooAbsPdf * stPdf = utils_.Hist2Pdf(th1st, "stPdf", ws_);
  delete th1st;
  return stPdf;

}


// ***** Function to return the QCD Pdf **** //
RooAbsPdf* RooWjjMjjFitter::makeQCDPdf() {  

  if (ws_.pdf("qcdPdf"))
    return ws_.pdf("qcdPdf");

  //// Scaling Coefficients = 84679.3/25080241, 3866200/70392060, 139500/2194800, 136804/2030033, 9360/1082691 = 3.37633517955429532e-03, 5.49238081681371476e-02, 6.35593220338983023e-02, 6.73900375018534198e-02, 8.64512589464584008e-03 
//   double weight1 = 84679.3/25080241;
//   double weight2 = 3866200./70392060.;
//   double weight3 = 139500./2194800.;
//   double weight4 = 136804./2030033.;
//   double weight5 = 9360./1082691.;

  TH1 * th1qcd = utils_.newEmptyHist("th1qcd");

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "RDQCD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root",
			       "hist_qcd_mu", false, 1, false);
    th1qcd->Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "RDQCD_WenuJets_DataAll_GoldenJSON_2p1invfb.root",
			       "hist_qcd_el", true, 1, false);
    th1qcd->Add(tmpHist);
    delete tmpHist;
  }

  th1qcd->Scale(1., "width");
  RooAbsPdf* qcdPdf = utils_.Hist2Pdf(th1qcd,"qcdPdf", ws_);
  delete th1qcd;    
  return qcdPdf;
}


// ***** Function to return the Z+jets Pdf **** //
RooAbsPdf * RooWjjMjjFitter::makeZpJPdf() {  

  if (ws_.pdf("ZpJPdf"))
    return ws_.pdf("ZpJPdf");
  
  double ZpJScale = 1.;

  TH1 * th1ZpJ = utils_.newEmptyHist("th1ZpJ", ZpJScale);

  TH1 * tmpHist;

  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + "mu_ZpJ_CMSSW428.root",
			       "hist_zpj_mu", false, 1, false, ZpJScale);
    th1ZpJ->Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + "el_ZpJ_CMSSW428.root",
			       "hist_zpj_el", true, 1, false, ZpJScale);
    th1ZpJ->Add(tmpHist);
    delete tmpHist;
  }

  zjetsNorm_ = (3048./36277961.) * 
    th1ZpJ->Integral(th1ZpJ->FindBin(params_.minFit),
		     th1ZpJ->FindBin(params_.maxFit)-1) * 
    params_.intLumi;
  cout << "-------- Number of expected zjets events = " 
       << th1ZpJ->Integral() << " x " << params_.intLumi << " = " 
       << zjetsNorm_ << endl;

  th1ZpJ->Scale(1., "width");

  RooAbsPdf * ZpJPdf = utils_.Hist2Pdf(th1ZpJ, "ZpJPdf", ws_);
  delete th1ZpJ;
  return ZpJPdf;
}

RooAbsPdf * RooWjjMjjFitter::makeNPPdf() {

  if (ws_.pdf("NPPdf"))
    return ws_.pdf("NPPdf");

  TH1 * th1NP = utils_.newEmptyHist("th1NP");

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.NewPhysicsDirectory + 
			       "mu_ZprimeMadGraph_CMSSW428.root",
			       "hist_np_mu", false, 1, false);
    th1NP->Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.NewPhysicsDirectory + 
			       "el_ZprimeMadGraph_CMSSW428.root",
			       "hist_np_el", true, 1, false);
    th1NP->Add(tmpHist);
    delete tmpHist;
  }

  th1NP->Scale(1., "width");

  RooAbsPdf * NPPdf = utils_.Hist2Pdf(th1NP, "NPPdf", ws_);
  delete th1NP;
  return NPPdf;
}

RooAbsPdf * RooWjjMjjFitter::makeWpJ4BodyPdf(RooWjjMjjFitter & fitter2body) {
  if (ws_.pdf("WpJ4BodyPdf"))
    return ws_.pdf("WpJ4BodyPdf");

  fitter2body.ws_.var(fitter2body.params_.var)->setRange("RangeWmass", 
							 params_.minWmass,
							 params_.maxWmass);
  ws_.var(params_.var)->SetTitle("m_{l#nujj}");

  RooWjjFitterParams parsSBHi(params_);
  parsSBHi.cuts = params_.SBHicut;
  RooWjjMjjFitter shapesSBHi(parsSBHi);
  shapesSBHi.makeQCDPdf();
  shapesSBHi.makettbarPdf();
  shapesSBHi.makeSingleTopPdf();
  shapesSBHi.makeZpJPdf();
  shapesSBHi.makeDibosonPdf();

  RooWjjFitterParams parsSBLo(params_);
  parsSBLo.cuts = params_.SBLocut;
  RooWjjMjjFitter shapesSBLo(parsSBLo);
  shapesSBLo.makeQCDPdf();
  shapesSBLo.makettbarPdf();
  shapesSBLo.makeSingleTopPdf();
  shapesSBLo.makeZpJPdf();
  shapesSBLo.makeDibosonPdf();

  TH1 * th1wjets = utils_.newEmptyHist("th1wjets");

  double sumalpha = 0.;
  TH1 * wjets = 0;
  double tmpWeight;
  for (unsigned int range = 0; range < params_.alphas.size(); ++range) {
    wjets = getWpJHistFromData(TString::Format("wjets%i", range),
			       params_.alphas[range], 
			       params_.minMasses[range],
			       params_.maxMasses[range],
			       fitter2body, shapesSBHi,
			       shapesSBLo);
    if (range < params_.falphas.size()) {
      tmpWeight = params_.falphas[range];
      sumalpha += params_.falphas[range];
    } else
      tmpWeight = 1.-sumalpha;

//     wjets->Print();
//     wjets->Draw();
//     gPad->WaitPrimitive();

    th1wjets->Add(wjets, tmpWeight);

    delete wjets;
  }

//   th1wjets.Print();
//   th1wjets.Draw();
//   gPad->WaitPrimitive();
  th1wjets->Scale(1, "width");
  RooAbsPdf * WpJ4BodyPdf = utils_.Hist2Pdf(th1wjets, "WpJ4BodyPdf", ws_);
  delete th1wjets;
  return WpJ4BodyPdf;
}

RooPlot * RooWjjMjjFitter::stackedPlot(bool logy, fitMode fm) {
  RooRealVar * mass = ws_.var(params_.var);
  mass->setRange("RangeForPlot", params_.minMass, 
				 params_.maxMass);
  RooAbsBinning& plotBins =  mass->getBinning("plotBinning");
  mass->setBinning(plotBins);
//   ws_.var(params_.var)->Print("v");
  RooPlot * sframe = mass->frame();
  sframe->SetName("mass_stacked");
  RooAbsData * data = ws_.data("data");

  TString plotHistName(TString::Format("dataHistPlot_%i", logy));
  TH1 * dataHist = utils_.newEmptyHist(plotHistName);
  RooTreeDataStore * dataStore = 
    dynamic_cast<RooTreeDataStore *>(data->store());
  dataStore->tree().Draw(TString::Format("%s>>%s", mass->GetName(),
					 plotHistName.Data()),
			 "", "goff");
  dataHist->SetTitle("data");

  
//   dataHist->Draw();
//   gPad->Update();
//   gPad->WaitPrimitive();
  dataHist->Scale(1., "width");
  RooHist * h_data = new RooHist(*dataHist, 0., 1, errorType_, 1.0,
				 false);
  RooHist * theData = new RooHist(*h_data);
//   dataHist->Draw();
//   gPad->Update();
//   gPad->WaitPrimitive();
  delete dataHist;

  RooAddPdf * totalPdf = dynamic_cast<RooAddPdf *>(ws_.pdf("totalPdf"));
  if (fm == mlnujj) 
    totalPdf = dynamic_cast<RooAddPdf *>(ws_.pdf("totalPdf4"));
  RooArgList components(totalPdf->pdfList());

//   components.Print("v");
//   h_data->SetName("h_data");
//   sframe->addPlotable(h_data, "pe", true, true);
//   data->plotOn(sframe, RooFit::DataError(errorType_), 
// 	       RooFit::Binning(plotBins),
// 	       RooFit::Name("h_data"),
// 	       RooFit::Invisible());

  int comp(1);
  double nexp(totalPdf->expectedEvents(RooArgSet(*(ws_.var(params_.var)))));
//   std::cout << "totalPdf expected: " << nexp << '\n'
// 	    << "frame NEvt: " << sframe->getFitRangeNEvt() << '\n'
// 	    << "frame BinW: " << sframe->getFitRangeBinW() << '\n'
// 	    << '\n';
//   TH1 * pdfHist = totalPdf->createHistogram("pdfHist", *mass,
// 					    Binning(plotBins),
// 					    Scaling(false));

//   pdfHist->Scale(nexp);
//   pdfHist->SetLineColor(kRed);
//   pdfHist->SetMarkerColor(kRed);
//   pdfHist->Draw();
//   dataHist->Draw("same");
//   gPad->Update();
//   gPad->WaitPrimitive();

  totalPdf->plotOn(sframe,ProjWData(*data), DrawOption("LF"), FillStyle(1001),
		   FillColor(kOrange), LineColor(kOrange), Name("h_total"),
 		   NormRange("RangeForPlot"),
		   Normalization(nexp, RooAbsReal::Raw),
		   VLines(), Range("RangeForPlot"));
  RooCurve * tmpCurve = sframe->getCurve("h_total");
  tmpCurve->SetTitle("WW/WZ");
  components.remove(*(components.find("dibosonPdf")));
  if (params_.doNewPhysics) {
    totalPdf->plotOn(sframe, ProjWData(*data), DrawOption("LF"), 
		     FillStyle(1001), Name("h_NP"), VLines(),
		     FillColor(kCyan+2), LineColor(kCyan+2), 
		     Normalization(nexp, RooAbsReal::Raw),
		     Components(RooArgSet(components)),
		     Range("RangeForPlot"));
    components.remove(*(components.find("NPPdf")));
    tmpCurve = sframe->getCurve("h_NP");
    tmpCurve->SetTitle("new physics");
  }
  int linec(kRed);
  TString pdfName("h_background");
  RooAbsCollection * removals;
  while (components.getSize() > 0) {
    totalPdf->plotOn(sframe, ProjWData(*data), FillColor(linec), 
// 		     Name(pdfName),
		     DrawOption("LF"), Range("RangeForPlot"),
		     NormRange("RangeForPlot"),
		     Normalization(nexp, RooAbsReal::Raw),
		     Components(RooArgSet(components)), VLines(),
		     FillStyle(1001), LineColor(linec));
    tmpCurve = sframe->getCurve();
    tmpCurve->SetName(pdfName);
    removals = 0;
    switch (comp) {
    case 1: 
      removals = components.selectByName("WpJPdf*");
      linec = kBlack; 
      tmpCurve->SetTitle("W+jets");
      break;
    case 2: 
      linec = kGreen;
      removals = components.selectByName("ttPdf*,stPdf*");
      tmpCurve->SetTitle("top");
      break;
    case 3: 
      linec = kMagenta; 
      removals = components.selectByName("qcdPdf*");
      tmpCurve->SetTitle("QCD");
      break;
    default:
      linec = kCyan;
      removals = components.selectByName("ZpJPdf*");
      tmpCurve->SetTitle("Z+jets");
    }
    if (removals) {
      components.remove(*removals);
      delete removals;
    }
    if (components.getSize() > 0) {
      pdfName = components[0].GetName();
      pdfName = "h_" + pdfName;
    }
    ++comp;
  }
  theData->SetName("theData");
  sframe->addPlotable(theData, "pe");
//   data->plotOn(sframe, RooFit::DataError(errorType_), Name("theData"),
// 	       RooFit::Binning(plotBins));
  RooHist * tmpHist = sframe->getHist("theData");
  tmpHist->SetTitle("data");
//   sframe->addObject(dataHist, "samepe");
  TLegend * legend = RooWjjFitterUtils::legend4Plot(sframe);
  sframe->addObject(legend);
  if (params_.truncRange) {
    TLine * lowerLine = new TLine(params_.minTrunc, 0., params_.minTrunc, 
				  sframe->GetMaximum());
    lowerLine->SetLineWidth(3);
    lowerLine->SetLineColor(kBlue+2);
    lowerLine->SetLineStyle(kDashed);
    TLine * upperLine = new TLine(params_.maxTrunc, 0., params_.maxTrunc, 
				  sframe->GetMaximum());
    upperLine->SetLineWidth(3);
    upperLine->SetLineColor(kBlue+2);
    upperLine->SetLineStyle(kDashed);
    sframe->addObject(lowerLine);
    sframe->addObject(upperLine);
  }
  if (logy) {
    sframe->SetMinimum(0.01);
    sframe->SetMaximum(1.0e6);
  } else {
    sframe->SetMinimum(1e-6);
    sframe->SetMaximum(1.4*sframe->GetMaximum());
  }
  if ((params_.truncRange) && (params_.blind)) {
    TBox * blind = new TBox(params_.minTrunc,sframe->GetMinimum(),
			    params_.maxTrunc,sframe->GetMaximum());
    blind->SetFillColor(kBlack);
    blind->SetFillStyle(1001);
    sframe->addObject(blind);
  }
  sframe->GetYaxis()->SetTitle("Events / GeV");
  return sframe;
}

RooPlot * RooWjjMjjFitter::residualPlot(RooPlot * thePlot, TString curveName,
					TString pdfName, bool normalize, 
					fitMode fm) {
  RooPlot * rframe = thePlot->emptyClone("mass_residuals");
  RooAbsData * data = ws_.data("data");
  RooAddPdf * totalPdf = dynamic_cast<RooAddPdf *>(ws_.pdf("totalPdf"));
  if (fm == mlnujj) 
    totalPdf = dynamic_cast<RooAddPdf *>(ws_.pdf("totalPdf4"));
  RooCurve * tmpCurve;
//   RooHist * theData = new RooHist(*(thePlot->getHist("theData")));
  if (pdfName.Length() > 0) {
    double nexp(totalPdf->expectedEvents(RooArgSet(*(ws_.var(params_.var)))));
    totalPdf->plotOn(rframe, ProjWData(*data), Components(pdfName),
		     Normalization(nexp, RooAbsReal::Raw),
		     DrawOption("LF"), VLines(), FillStyle(1001),
		     FillColor(kOrange), //Name("h_"+ pdfName), 
		     LineColor(kOrange), 
		     NormRange("RangeForPlot"),
		     Range("RangeForPlot"));
    tmpCurve = rframe->getCurve();
    tmpCurve->SetName("h_" + pdfName);
    tmpCurve->SetTitle("WW/WZ");
    if (params_.doNewPhysics) {
      totalPdf->plotOn(rframe, ProjWData(*data), Components("NPPdf"),
		       Normalization(nexp, RooAbsReal::Raw),
		       DrawOption("LF"), VLines(), FillStyle(1001),
		       FillColor(kCyan+2), //Name("h_NP"),
		       LineColor(kCyan+2), 
		       NormRange("RangeForPlot"),
		       Range("RangeForPlot"));
      tmpCurve = rframe->getCurve();
      tmpCurve->SetName("h_NP");
      tmpCurve->SetTitle("new physics");
    }
  }
  RooHist * hresid = thePlot->residHist("theData", curveName, normalize);
  hresid->SetTitle("data");
//   RooHist * bands = new RooHist(*hresid);
//   bands->SetName("band");
//   bands->SetTitle("error bands");
//   bands->SetFillStyle(1001);
//   rframe->addPlotable(bands, "e3");
  rframe->addPlotable(hresid, "p");
  
  TLegend * legend = RooWjjFitterUtils::legend4Plot(rframe);
  rframe->addObject(legend);

  if (!normalize) {
    rframe->SetMaximum(initDiboson_*(0.1));
    rframe->SetMinimum(initDiboson_*(-0.05));
    rframe->GetYaxis()->SetTitle("Events / GeV");
  } else {
    rframe->SetMaximum(5.);
    rframe->SetMinimum(-5.);
    rframe->GetYaxis()->SetTitle("pull ( #sigma )");
  }

  if (params_.truncRange) {
    TLine * lowerLine = new TLine(params_.minTrunc,rframe->GetMinimum()*0.6,
				  params_.minTrunc,rframe->GetMaximum()*0.6);
    lowerLine->SetLineWidth(3);
    lowerLine->SetLineColor(kBlue+2);
    lowerLine->SetLineStyle(kDashed);
    TLine * upperLine = new TLine(params_.maxTrunc,rframe->GetMinimum()*0.6, 
				  params_.maxTrunc,rframe->GetMaximum()*0.6);
    upperLine->SetLineWidth(3);
    upperLine->SetLineColor(kBlue+2);
    upperLine->SetLineStyle(kDashed);
    rframe->addObject(lowerLine);
    rframe->addObject(upperLine);
  }  
  if ((params_.truncRange) && (params_.blind)) {
    TBox * blind = new TBox(params_.minTrunc,rframe->GetMinimum(),
			    params_.maxTrunc,rframe->GetMaximum());
    blind->SetFillColor(kBlack);
    blind->SetFillStyle(1001);
    rframe->addObject(blind);
  }

  return rframe;

}

void RooWjjMjjFitter::loadParameters(TString fname) {
  
  if (fname.Length() > 0) {
    RooArgSet obs(*(ws_.var(params_.var)));
    RooArgSet * params = ws_.pdf("totalPdf")->getParameters(obs);
    params->readFromFile(fname);
    if ( params_.useExternalMorphingPars ) {
      resetfSUfMU(params_.e_fSU, params_.e_fMU);
    }
      
    delete params;
  }
}

void RooWjjMjjFitter::resetYields() {
  ws_.var("nWjets")->setVal(initWjets_);
  ws_.var("nWjets")->setError(TMath::Sqrt(initWjets_));
  ws_.var("nDiboson")->setVal(initDiboson_);
  ws_.var("nDiboson")->setError(initDiboson_*0.15);
  ws_.var("nTTbar")->setVal(ttbarNorm_);
  ws_.var("nTTbar")->setError(ttbarNorm_*0.063);
  ws_.var("nSingleTop")->setVal(singleTopNorm_);
  ws_.var("nSingleTop")->setError(singleTopNorm_*0.05);
  ws_.var("nQCD")->setVal(QCDNorm_);
  ws_.var("nQCD")->setError(QCDError_);
  ws_.var("nZjets")->setVal(zjetsNorm_);
  ws_.var("nZjets")->setError(zjetsNorm_*0.043);
  if (params_.doNewPhysics) {
    ws_.var("nNP")->setVal( 0. );
    ws_.var("nNP")->setError(100.);
  }
}

////////////////////////////////////////////////////////////////////
////   Use For MC Dataset Toy Generation
////////////////////////////////////////////////////////////////////

void RooWjjMjjFitter::generateToyMCSet(RooAbsPdf *inputPdf, const char* outFileName, int NEvts, int seedInitializer)
/// Generates a ToyMC dataset (for the mjj distribution) from the inputPdf
{
  int seed;
  seed=3487+seedInitializer*3;
  RooRandom::randomGenerator()->SetSeed(seed);
  TFile *fMC = new TFile(outFileName, "RECREATE");

  RooRealVar * mass = ws_.var(params_.var);
  RooDataSet* toymc =  inputPdf->generate(*mass,NEvts);
  const TTree* tMC  = toymc->tree();
  fMC->cd();
  tMC->Write();
  fMC->Close();

}


void RooWjjMjjFitter::resetfSUfMU(double fSU, double fMU) {
  RooArgSet * params = ws_.pdf("totalPdf")->getParameters(ws_.data("data"));
  if ( fSU>0 ) {
    params->setRealValue("fSU",fSU);
    params->setRealValue("fSD",0.0);
  } else {
    params->setRealValue("fSU",0.0);
    params->setRealValue("fSD",-fSU);
  }
  if ( fMU>0 ) {
    params->setRealValue("fMU",fMU);
    params->setRealValue("fMD",0.0);
  } else {
    params->setRealValue("fMU",0.0);
    params->setRealValue("fMD",-fMU);
  }

  delete params;
}

////////////////////////////////////////////////////////////////////

TH1 * RooWjjMjjFitter::getWpJHistFromData(TString histName, double alpha, 
					  double xMin, double xMax, 
					  RooWjjMjjFitter & fitter2body,
					  RooWjjMjjFitter & shapesSBHi,
					  RooWjjMjjFitter & shapesSBLo) {
  TString massCut(TString::Format("((%s > %f) && (%s < %f))", 
				  params_.var.Data(), xMin, params_.var.Data(),
				  xMax));
  TString cutsSBHi(TString::Format("(%s) && (%s)",
				   massCut.Data(), params_.SBHicut.Data()));
  TString cutsSBLo(TString::Format("(%s) && (%s)",
				   massCut.Data(), params_.SBLocut.Data()));

//   int tmpBins((xMax-xMin)/(params_.maxMass-params_.minMass)*params_.nbins 
// 	      + 0.5);

  TH1 * th1wpjHi = utils_.newEmptyHist(histName + "_Hi");
  TH1 * th1wpjLo = utils_.newEmptyHist(histName + "_Lo");

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.DataDirectory + params_.muonData, 
			       histName + "_Hi_mu", false, 1, false, 1., 
			       cutsSBHi);
    th1wpjHi->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.DataDirectory + params_.muonData, 
			       histName + "_Lo_mu", false, 1, false, 1., 
			       cutsSBLo);
    th1wpjLo->Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.DataDirectory + params_.electronData, 
			       histName + "_Hi_el", true, 1, false, 1., 
			       cutsSBHi);
    th1wpjHi->Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.DataDirectory + params_.electronData, 
			       histName + "_Lo_el", true, 1, false, 1., 
			       cutsSBLo);
    th1wpjLo->Add(tmpHist);
    delete tmpHist;
  }

//   th1wpjHi.Print();
//   th1wpjHi.Draw();
//   gPad->WaitPrimitive();

  subtractHistogram(*th1wpjHi, HighSB, xMin, xMax, fitter2body, shapesSBHi);

//   th1wpjHi.Print();
//   th1wpjHi.Draw();
//   gPad->WaitPrimitive();

//   th1wpjLo.Print();
//   th1wpjLo.Draw();
//   gPad->WaitPrimitive();

  subtractHistogram(*th1wpjLo, LowSB, xMin, xMax, fitter2body, shapesSBLo);

//   th1wpjLo.Print();
//   th1wpjLo.Draw();
//   gPad->WaitPrimitive();

  th1wpjHi->Scale(1./th1wpjHi->Integral());
  th1wpjLo->Scale(1./th1wpjLo->Integral());

  th1wpjHi->Add(th1wpjLo, th1wpjHi, alpha, 1. - alpha);

  delete th1wpjLo;
  th1wpjHi->SetName(histName);

  return th1wpjHi;
}

void RooWjjMjjFitter::subtractHistogram(TH1& hist, SideBand sideBand,
					double m4min, double m4max, 
					RooWjjMjjFitter & fitter2body,
					RooWjjMjjFitter & shapesSB) {
  RooRealVar * mass = fitter2body.ws_.var(fitter2body.params_.var);
  RooRealVar * m4b = shapesSB.ws_.var(params_.var);
  double maxLimit = params_.maxSBLo, minLimit = params_.minSBLo;
  if (sideBand == HighSB) {
    maxLimit = params_.maxSBHi;
    minLimit = params_.minSBHi;
  }

  mass->setRange("sideband", minLimit, maxLimit);
  TString pdfName("qcdPdf");
  TString normName("nQCD");
  RooAbsReal * fullInt;
  RooAbsReal * SBInt;
  TH1 * tempHist;

  int nbins = hist.GetNbinsX(), ibin;
//   double xMin = hist.GetBinLowEdge(1);
//   double xMax = hist.GetBinLowEdge(nbins+1);
  double x;
  for (int comp = 0; comp < 5; ++comp) {
    switch (comp) {
    case 0:
      normName = "nDiboson";
      pdfName = "dibosonPdf";
      break;
    case 1:
      normName = "nTTbar";
      pdfName = "ttPdf";
      break;
    case 2:
      normName = "nSingleTop";
      pdfName = "stPdf";
      break;
    case 3:
      normName = "nQCD";
      pdfName = "qcdPdf";
      break;
    case 4:
      normName = "nZjets";
      pdfName = "ZpJPdf";
      break;
    }

    fullInt = 
      fitter2body.ws_.pdf(pdfName)->createIntegral(*mass,
						   RooFit::NormSet(*mass));
    SBInt = 
      fitter2body.ws_.pdf(pdfName)->createIntegral(*mass,
						   RooFit::NormSet(*mass),
						   RooFit::Range("sideband"));
    tempHist = 
      shapesSB.ws_.pdf(pdfName)->createHistogram(pdfName + "_tempHist", *m4b);
    tempHist->Scale( fitter2body.ws_.var(normName)->getVal()/tempHist->Integral() * SBInt->getVal()/fullInt->getVal() );
    for (ibin = 1; ibin <= nbins; ++ibin) {
      x = tempHist->GetBinCenter(ibin);
      if ((x < m4min) || (x >= m4max)) {
	tempHist->SetBinContent(ibin, 0);
	tempHist->SetBinError(ibin, 0);
      }
    }
//     tempHist->Print();
//     tempHist->Draw();
//     gPad->WaitPrimitive();
    hist.Add(tempHist, -1.);
    delete tempHist;
    delete fullInt;
    delete SBInt;
  }
}

void RooWjjMjjFitter::addHistograms(TH1& hist1, TH1& hist2, double weight) {
  int bin;
  double x;
  for( bin = 1; bin <= hist2.GetNbinsX(); ++bin) {
    x = hist2.GetBinCenter(bin);
    hist1.Fill(x, hist2.GetBinContent(bin)*weight);
  }
}
