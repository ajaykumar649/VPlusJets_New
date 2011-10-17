#include "RooHist.h"
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "TString.h"
#include "TH1.h"

#include "TPad.h"

double sig2(RooAddPdf& pdf, RooRealVar& obs, double Npdf);
double sig2(RooHistPdf& pdf, RooRealVar& obs, double Npdf);

double ComputeChi2(RooHist& hist, RooAbsPdf& pdf, RooRealVar& obs, int& nbin) {

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

double sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin) {
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

double sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin) {
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
