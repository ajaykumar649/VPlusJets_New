#include "RooWjjMjjFitter.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TMath.h"

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

// #include "ComputeChi2.C"

using namespace RooFit;

RooWjjMjjFitter::RooWjjMjjFitter() :
  ws_("ws", "ws"), initWjets_(0.), initDiboson_(0.),
  ttbarNorm_(0.), singleTopNorm_(0.), zjetsNorm_(0.),
  QCDNorm_(0.)
{
  utils_.setJES_scale(0, params_.JES_scale1);
  utils_.setJES_scale(1, params_.JES_scale2);
}

RooWjjMjjFitter::RooWjjMjjFitter(RooWjjFitterParams & pars) :
  ws_("ws", "ws"), params_(pars), utils_(pars.nbins, pars.minMass, 
					 pars.maxMass, pars.njets,
					 pars.cuts, pars.var, pars.treeName),
  initWjets_(0.), initDiboson_(0.), ttbarNorm_(0.), singleTopNorm_(0.), 
  zjetsNorm_(0.), QCDNorm_(0.)
{
  utils_.setJES_scale(0, params_.JES_scale1);
  utils_.setJES_scale(1, params_.JES_scale2);
}

RooFitResult * RooWjjMjjFitter::fit() {

  RooAbsPdf * totalPdf = makeFitter();
  RooRealVar * mass = ws_.var(params_.var);

  RooAbsData * data = loadData();
  cout << "Made dataset" << endl;

  std::cout << "-------- Number of expected QCD events: " << QCDNorm_
	    << std::endl;

   mass->setRange("lowSideBand", params_.minMass, params_.minTrunc);
   mass->setRange("highSideBand", params_.maxTrunc, params_.maxMass);

   // const char* rangeString = "Range55To250";
   rangeString_ = "";
   if (params_.truncRange) 
      rangeString_ = "lowSideBand,highSideBand";

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


   RooArgSet exConstraints;

   exConstraints.add(constTTbar);
   exConstraints.add(constSingleTop);
   exConstraints.add(constZpJ);
   exConstraints.add(constQCD);
   if (params_.constrainDiboson) 
     exConstraints.add(constDiboson);

   loadParameters(params_.initParamsFile);

   std::cout << "\n***External constraints***\n";
   TIter con(exConstraints.createIterator());
   RooGaussian * tc;
   while ((tc = (RooGaussian *)con()))
     tc->Print();
   std::cout << "*** ***\n\n";
   
   fitResult = totalPdf->fitTo(*data, Save(true), 
			       ExternalConstraints(exConstraints),
			       RooFit::Extended(true), 
			       RooFit::Minos(false), 
			       RooFit::Hesse(true),
			       PrintEvalErrors(-1),
			       RooFit::Range(rangeString_),
			       Warnings(false) 
			       );

   fitResult->Print("v");
   params->writeToFile("lastWjjFitParams.txt");

   delete params;
   return fitResult;
}

RooPlot * RooWjjMjjFitter::computeChi2(double& chi2, int& ndf) {

  RooRealVar * mass = ws_.var(params_.var);
  RooPlot * chi2frame = mass->frame(params_.minMass, params_.maxMass,
				    params_.nbins);
  mass->setRange("RangeForPlot", params_.minMass, params_.maxMass);
  RooAbsData * data = ws_.data("data");
  RooAbsPdf * totalPdf = ws_.pdf("totalPdf");

  data->plotOn(chi2frame, //RooFit::DataError(errorType), 
	       RooFit::Invisible(),
	       RooFit::Name("h_data"), RooFit::MarkerColor(kRed));
  totalPdf->plotOn(chi2frame, RooFit::ProjWData(*data),
		   RooFit::Name("h_total"),
		   RooFit::Invisible(),
		   ( (rangeString_.Length() > 0)? 
		     RooFit::NormRange("RangeForPlot") :
		     RooCmdArg::none() ),
		   ( (rangeString_.Length() > 0)? 
		     RooFit::Range("RangeForPlot", false) :
		     RooCmdArg::none() )
		   );
  totalPdf->plotOn(chi2frame, RooFit::ProjWData(*data),
		   RooFit::Name("h_fit"),
		   ( (rangeString_.Length() > 0)? 
		     RooFit::NormRange(rangeString_) :
		     RooCmdArg::none() ),
		   ( (rangeString_.Length() > 0)? 
		     RooFit::Range(rangeString_) :
		     RooCmdArg::none() )
		   );
  data->plotOn( chi2frame, //RooFit::DataError(errorType),
		RooFit::Name("theData"),
		(rangeString_.Length() > 0)? RooFit::CutRange(rangeString_) :
		RooCmdArg::none() 
		);
  int chi2bins;
  chi2 = RooWjjFitterUtils::computeChi2(*(chi2frame->getHist("theData")),
					*totalPdf, *mass, chi2bins);

  chi2bins -= ndf;
  std::cout << "\n *** chi^2/dof = " << chi2 << "/" << chi2bins << " = " 
	    << chi2/chi2bins << " ***\n"
	    << " *** chi^2 probability = " << TMath::Prob(chi2, chi2bins)
	    << " ***\n\n";

  ndf = chi2bins;
  return chi2frame;
}

RooAbsPdf * RooWjjMjjFitter::makeFitter() {

  if (ws_.pdf("totalPdf"))
    return ws_.pdf("totalPdf");

  RooAbsPdf * dibosonPdf = makeDibosonPdf();
  RooAbsPdf * WpJPdf = makeWpJPdf();
  RooAbsPdf * ttPdf = makettbarPdf();
  RooAbsPdf * stPdf = makeSingleTopPdf();
  RooAbsPdf * qcdPdf = makeQCDPdf();
  RooAbsPdf * ZpJPdf =  makeZpJPdf();

  RooRealVar nWjets("nWjets","nWjets", initWjets_, 0.0, 100000.);
  nWjets.setError(TMath::Sqrt(initWjets_));
  RooRealVar nDiboson("nDiboson","nDiboson",  initDiboson_, 0.0, 10000.);
  nDiboson.setError(initDiboson_*0.15);
  RooRealVar nTTbar("nTTbar","", ttbarNorm_);
  nTTbar.setError(ttbarNorm_*0.1);
  RooRealVar nSingleTop("nSingleTop","", singleTopNorm_);
  nSingleTop.setError(singleTopNorm_*0.1);
  RooRealVar nQCD("nQCD","nQCD", QCDNorm_);
  nQCD.setError(QCDNorm_*0.5);
  RooRealVar nZjets("nZjets","nZjets", zjetsNorm_);
  nZjets.setError(zjetsNorm_*0.15);
  RooRealVar nNP("nNP", "N_{new physics}", 0., 0., 10000.);
  nNP.setError(100.);

  RooArgList components(*dibosonPdf);
  RooArgList yields(nDiboson);
  components.add(*WpJPdf);
  yields.add(nWjets);
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

RooAbsData * RooWjjMjjFitter::loadData() {
  if (ws_.data("data"))
    return ws_.data("data");

  RooRealVar * mass = ws_.var(params_.var);

  RooDataSet data("data", "data", RooArgSet(*mass));
  QCDNorm_ = 0.;
  if (params_.includeMuons) {
    RooDataSet * mds = utils_.File2Dataset(params_.DataDirectory + 
					   params_.muonData, "data_muon");
    mds->Print();
    QCDNorm_ += 0.008*mds->sumEntries();
    data.append(*mds);
    delete mds;
  }
  if (params_.includeElectrons) {
    RooDataSet * eds = utils_.File2Dataset(params_.DataDirectory + 
					   params_.muonData, "data_electron");
    eds->Print();
    QCDNorm_ += 0.03*eds->sumEntries();
    data.append(*eds);
    delete eds;
  }
  ws_.import(data);

  return ws_.data("data");
}

RooAbsPdf * RooWjjMjjFitter::makeDibosonPdf() {

  //Scale the trees by the Crossection/Ngenerated (43/4225916=1.01753087377979123e-05 for WW and 18/4265243=4.22015814808206740e-06 for WZ).
  if (ws_.pdf("dibosonPdf"))
    return ws_.pdf("dibosonPdf");

  double WWweight = 53./4225916.;
  double WZweight = 17./4265243.;
  double dibosonScale = 2.;
  TH1D th1diboson("th1diboson", "th1diboson", 
		  int(utils_.getNbins()*dibosonScale),
		  utils_.getMin(), utils_.getMax());
  th1diboson.Sumw2();

  TH1 * tmpHist;

  if (params_.includeMuons) {
    tmpHist  = utils_.File2Hist(params_.MCDirectory+"mu_WW_CMSSW428.root",
				"hist_ww_mu", 0, false, dibosonScale);
    th1diboson.Add(tmpHist, WWweight);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory+"mu_WZ_CMSSW428.root",
			       "hist_wz_mu", 0, false, dibosonScale);
    th1diboson.Add(tmpHist, WZweight);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist  = utils_.File2Hist(params_.MCDirectory+"el_WW_CMSSW428.root",
				"hist_ww_el", 0, false, dibosonScale);
    th1diboson.Add(tmpHist, WWweight);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory+"el_WZ_CMSSW428.root",
			       "hist_wz_el", 0, false, dibosonScale);
    th1diboson.Add(tmpHist, WZweight);
    delete tmpHist;
  }

  initDiboson_ = th1diboson.Integral() * params_.intLumi;
  cout << "-------- Number of expected WW+WZ events = " 
       << th1diboson.Integral() << " x " << params_.intLumi 
       << " = " <<  initDiboson_ << endl;

  RooAbsPdf * dibosonPdf = utils_.Hist2Pdf(&th1diboson, "dibosonPdf",
					    ws_);
  return dibosonPdf;
}

RooAbsPdf * RooWjjMjjFitter::makeWpJPdf() {  

  if (ws_.pdf("WpJPdf"))
    return ws_.pdf("WpJPdf");

  TH1D th1WpJ("th1WpJ", "th1WpJ", utils_.getNbins(),
	      utils_.getMin(), utils_.getMax());
  th1WpJ.Sumw2();
  TH1D th1WpJMU(th1WpJ);
  th1WpJMU.SetName("th1WpJMU");
  TH1D th1WpJMD(th1WpJ);
  th1WpJMD.SetName("th1WpJMD");
  TH1D th1WpJSU(th1WpJ);
  th1WpJSU.SetName("th1WpJSU");
  TH1D th1WpJSD(th1WpJ);
  th1WpJSD.SetName("th1WpJSD");

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.WpJDirectory + "mu_WpJ_CMSSW428.root",
			       "hist_wpj_mu", 1, params_.toyWpJ);
    th1WpJ.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJmatchingup_CMSSW428.root",
			       "hist_wpj_mu_mu", 1, params_.toyWpJ);
    th1WpJMU.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJmatchingdown_CMSSW428.root",
			       "hist_wpj_mu_md", 1, params_.toyWpJ);
    th1WpJMD.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJscaleup_CMSSW428.root",
			       "hist_wpj_mu_su", 1, params_.toyWpJ);
    th1WpJSU.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "mu_WpJscaledown_CMSSW428.root",
			       "hist_wpj_mu_sd", 1, params_.toyWpJ);
    th1WpJSD.Add(tmpHist);
    delete tmpHist;
  }

  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.WpJDirectory + "el_WpJ_CMSSW428.root",
			       "hist_wpj_el", 1, params_.toyWpJ);
    th1WpJ.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJmatchingup_CMSSW428.root",
			       "hist_wpj_el_mu", 1, params_.toyWpJ);
    th1WpJMU.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJmatchingdown_CMSSW428.root",
			       "hist_wpj_el_md", 1, params_.toyWpJ);
    th1WpJMD.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJscaleup_CMSSW428.root",
			       "hist_wpj_el_su", 1, params_.toyWpJ);
    th1WpJSU.Add(tmpHist);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.WpJDirectory + 
			       "el_WpJscaledown_CMSSW428.root",
			       "hist_wpj_el_sd", 1, params_.toyWpJ);
    th1WpJSD.Add(tmpHist);
    delete tmpHist;
  }

  initWjets_ = (31314./81352581.) * (th1WpJ.Integral()) * params_.intLumi;
  cout << "-------- Number of expected Wjj events = " <<  initWjets_ << endl;

  RooArgList pdfs;
  RooArgList coefs;

  RooAbsPdf * WpJPdfSU = utils_.Hist2Pdf(&th1WpJSU, "WpJPdfSU", ws_);
  RooRealVar fSU("fSU", "f_{scaleUp}", 0., 0., 1.);
  fSU.setConstant();
  pdfs.add(*WpJPdfSU);
  coefs.add(fSU);

  RooAbsPdf * WpJPdfSD = utils_.Hist2Pdf(&th1WpJSD, "WpJPdfSD", ws_);
  RooRealVar fSD("fSD", "f_{scaleDown}", 0., 0., 1.);
  fSD.setConstant();
  pdfs.add(*WpJPdfSD);
  coefs.add(fSD);

  RooAbsPdf * WpJPdfMU = utils_.Hist2Pdf(&th1WpJMU, "WpJPdfMU", ws_);
  RooRealVar fMU("fMU", "f_{matchingUp}", 0., 0., 1.);
  fMU.setConstant();
  pdfs.add(*WpJPdfMU);
  coefs.add(fMU);

  RooAbsPdf * WpJPdfMD = utils_.Hist2Pdf(&th1WpJMD, "WpJPdfMD", ws_);
  RooRealVar fMD("fMD", "f_{matchingDown}", 0., 0., 1.);
  fMD.setConstant();
  pdfs.add(*WpJPdfMD);
  coefs.add(fMD);

  RooAbsPdf * WpJPdfNom = utils_.Hist2Pdf(&th1WpJ, "WpJPdfNom", ws_);
  pdfs.add(*WpJPdfNom);

  RooAddPdf WpJPdf("WpJPdf","WpJPdf", pdfs, coefs, false);
  ws_.import(WpJPdf);

  return ws_.pdf("WpJPdf");
  //return WjetsShape;
}

RooAbsPdf * RooWjjMjjFitter::makettbarPdf() {

  if (ws_.pdf("ttPdf"))
    return ws_.pdf("ttPdf");

  double ttScale = 2.;
  TH1D th1tt("th1tt", "th1tt", int(utils_.getNbins()*ttScale),
	      utils_.getMin(), utils_.getMax());
  th1tt.Sumw2();

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_TTbar_MG_CMSSW428.root",
			       "hist_tt_mu", 1, false, ttScale);
    th1tt.Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_TTbar_MG_CMSSW428.root",
			       "hist_tt_el", 1, false, ttScale);
    th1tt.Add(tmpHist);
    delete tmpHist;
  }

  ttbarNorm_ = (157.5/3701947) * th1tt.Integral() * params_.intLumi;
  cout << "-------- Number of expected ttbar events = " 
       << th1tt.Integral() << " x " << params_.intLumi << " = " 
       << ttbarNorm_ << endl;


  RooAbsPdf * ttPdf = utils_.Hist2Pdf(&th1tt, "ttPdf", ws_);

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

  double stScale = 2.;

  TH1D th1st("th1st", "th1st", int(utils_.getNbins()*stScale),
	      utils_.getMin(), utils_.getMax());
  th1st.Sumw2();

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopS_Tbar_CMSSW428.root",
			       "hist_stbar_mu", 1, false, stScale);
    th1st.Add(tmpHist, STopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopS_T_CMSSW428.root",
			       "hist_st_mu", 1, false, stScale);
    th1st.Add(tmpHist, STopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopT_Tbar_CMSSW428.root",
			       "hist_stbar_mu", 1, false, stScale);
    th1st.Add(tmpHist, TTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopT_T_CMSSW428.root",
			       "hist_st_mu", 1, false, stScale);
    th1st.Add(tmpHist, TTopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopTW_Tbar_CMSSW428.root",
			       "hist_stbar_mu", 1, false, stScale);
    th1st.Add(tmpHist, TWTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "mu_STopTW_T_CMSSW428.root",
			       "hist_st_mu", 1, false, stScale);
    th1st.Add(tmpHist, TWTopWeight);
//     tmpHist->Print();
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopS_Tbar_CMSSW428.root",
			       "hist_stbar_el", 1, false, stScale);
    th1st.Add(tmpHist, STopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopS_T_CMSSW428.root",
			       "hist_st_el", 1, false, stScale);
    th1st.Add(tmpHist, STopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopT_Tbar_CMSSW428.root",
			       "hist_stbar_el", 1, false, stScale);
    th1st.Add(tmpHist, TTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopT_T_CMSSW428.root",
			       "hist_st_el", 1, false, stScale);
    th1st.Add(tmpHist, TTopWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopTW_Tbar_CMSSW428.root",
			       "hist_stbar_el", 1, false, stScale);
    th1st.Add(tmpHist, TWTopBarWeight);
//     tmpHist->Print();
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.MCDirectory + 
			       "el_STopTW_T_CMSSW428.root",
			       "hist_st_el", 1, false, stScale);
    th1st.Add(tmpHist, TWTopWeight);
//     tmpHist->Print();
    delete tmpHist;
  }

  singleTopNorm_ = th1st.Integral() * params_.intLumi;

//   if (params_.includeElectrons && params_.includeMuons)
//     singleTopNorm_ *= 2;
  cout << "-------- Number of expected single top events = " << 
    th1st.Integral() << " x " << params_.intLumi << " = " << singleTopNorm_ << endl;

  RooAbsPdf * stPdf = utils_.Hist2Pdf(&th1st, "stPdf", ws_);

  return stPdf;

}


// ***** Function to return the QCD Pdf **** //
RooAbsPdf* RooWjjMjjFitter::makeQCDPdf() {  

  if (ws_.pdf("qcdPdf"))
    return ws_.pdf("qcdPdf");

  //// Scaling Coefficients = 84679.3/25080241, 3866200/70392060, 139500/2194800, 136804/2030033, 9360/1082691 = 3.37633517955429532e-03, 5.49238081681371476e-02, 6.35593220338983023e-02, 6.73900375018534198e-02, 8.64512589464584008e-03 
  double weight1 = 84679.3/25080241;
  double weight2 = 3866200./70392060.;
  double weight3 = 139500./2194800.;
  double weight4 = 136804./2030033.;
  double weight5 = 9360./1082691.;

  TH1D th1qcd("th1qcd", "th1qcd", utils_.getNbins(),
	      utils_.getMin(), utils_.getMax());
  th1qcd.Sumw2();
  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "mu_QCDMu_CMSSW428_MET20Iso03.root",
			       "hist_qcd_mu", 1, false);
    th1qcd.Add(tmpHist, weight1);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "el_QCDEl_Pt30to80_CMSSW428_MET20Iso03.root",
			       "hist_qcd_el_1", 1, false);
    th1qcd.Add(tmpHist, weight2);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "el_QCDEl_Pt80to170_CMSSW428_MET20Iso03.root",
			       "hist_qcd_el_2", 1, false);
    th1qcd.Add(tmpHist, weight3);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "el_QCDEl_BCtoE30to80_CMSSW428_MET20Iso03.root",
			       "hist_qcd_el_3", 1, false);
    th1qcd.Add(tmpHist, weight4);
    delete tmpHist;
    tmpHist = utils_.File2Hist(params_.QCDDirectory + 
			       "el_QCDEl_BCtoE80to170_CMSSW428_MET20Iso03.root",
			       "hist_qcd_el_4", 1, false);
    th1qcd.Add(tmpHist, weight5);
    delete tmpHist;
  }

  RooAbsPdf* qcdPdf = utils_.Hist2Pdf(&th1qcd,"qcdPdf", ws_);
    
  return qcdPdf;
}


// ***** Function to return the Z+jets Pdf **** //
RooAbsPdf * RooWjjMjjFitter::makeZpJPdf() {  

  if (ws_.pdf("ZpJPdf"))
    return ws_.pdf("ZpJPdf");
  
  double ZpJScale = 2.;

  TH1D th1ZpJ("th1ZpJ", "th1ZpJ", int(utils_.getNbins()*ZpJScale),
	      utils_.getMin(), utils_.getMax());
  th1ZpJ.Sumw2();

  TH1 * tmpHist;

  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + "mu_ZpJ_CMSSW428.root",
			       "hist_zpj_mu", 1, false, ZpJScale);
    th1ZpJ.Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.MCDirectory + "el_ZpJ_CMSSW428.root",
			       "hist_zpj_el", 1, false, ZpJScale);
    th1ZpJ.Add(tmpHist);
    delete tmpHist;
  }

  zjetsNorm_ = (3048./36277961.) * th1ZpJ.Integral() * params_.intLumi;
  cout << "-------- Number of expected zjets events = " 
       << th1ZpJ.Integral() << " x " << params_.intLumi << " = " 
       << zjetsNorm_ << endl;


  RooAbsPdf * ZpJPdf = utils_.Hist2Pdf(&th1ZpJ, "ZpJPdf", ws_);

  return ZpJPdf;
}

RooAbsPdf * RooWjjMjjFitter::makeNPPdf() {

  if (ws_.pdf("NPPdf"))
    return ws_.pdf("NPPdf");

  TH1D th1NP("th1NP", "th1NP", utils_.getNbins(), utils_.getMin(),
	     utils_.getMax());
  th1NP.Sumw2();

  TH1 * tmpHist;
  if (params_.includeMuons) {
    tmpHist = utils_.File2Hist(params_.NewPhysicsDirectory + 
			       "mu_ZprimeMadGraph_CMSSW428.root",
			       "hist_np_mu", 1, false);
    th1NP.Add(tmpHist);
    delete tmpHist;
  }
  if (params_.includeElectrons) {
    tmpHist = utils_.File2Hist(params_.NewPhysicsDirectory + 
			       "el_ZprimeMadGraph_CMSSW428.root",
			       "hist_np_el", 1, false);
    th1NP.Add(tmpHist);
    delete tmpHist;
  }

  RooAbsPdf * NPPdf = utils_.Hist2Pdf(&th1NP, "NPPdf", ws_);

  return NPPdf;
}

void RooWjjMjjFitter::loadParameters(TString const& fname) {
  
  if (fname.Length() > 0) {
    RooArgSet * params = ws_.pdf("totalPdf")->getParameters(ws_.data("data"));
    params->readFromFile(fname);
    delete params;
  }
}

void RooWjjMjjFitter::resetYields() {
  ws_.var("nWjets")->setVal(initWjets_);
  ws_.var("nWjets")->setError(TMath::Sqrt(initWjets_));
  ws_.var("nDiboson")->setVal(initDiboson_);
  ws_.var("nDiboson")->setError(initDiboson_*0.15);
  ws_.var("nTTbar")->setVal(ttbarNorm_);
  ws_.var("nTTbar")->setError(ttbarNorm_*0.1);
  ws_.var("nSingleTop")->setVal(singleTopNorm_);
  ws_.var("nSingleTop")->setError(singleTopNorm_*0.1);
  ws_.var("nQCD")->setVal(QCDNorm_);
  ws_.var("nQCD")->setError(QCDNorm_*0.5);
  ws_.var("nZjets")->setVal(zjetsNorm_);
  ws_.var("nZjets")->setError(zjetsNorm_*0.15);
  if (params_.doNewPhysics) {
    ws_.var("nNP")->setVal( 0. );
    ws_.var("nNP")->setError(100.);
  }
}
