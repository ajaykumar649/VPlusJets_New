//root -b -q -l upperLimit.C+
#include <string>
#include <iostream>

#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TText.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TLine.h"
#include "TH1F.h"
#include "tdrstyle.C"

////CMS Preliminary label and lumi -- upper left corner
void cmsLumi(bool prelim)
{
  //const float LUMINOSITY = 4.7;
  const float LUMINOSITY = 5.0;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(11); // align left
    latex.DrawLatex(0.35,0.96,Form("#scale[0.5]{#int} #font[12]{L} dt = %.1f fb^{-1}", LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,prelim ? "CMS preliminary" : "CMS");
}

void plotMjjPvalues(std::string limitFile = "limitSummary.txt", bool prelim=false)
{
  double r[100],obs[100],exp[100],sig1hi[100],sig1lo[100],sig2hi[100],sig2lo[100];
  char line[256];

  FILE *fp=fopen(limitFile.c_str(),"r");
  if (!fp) {
    cerr << limitFile << " doesn't exist." << endl;
    exit(-1);
  }
  int npts=0;
  for (int i=0; !feof(fp) && fgets(line,256,fp); ) {
    if (line[0]=='#') continue;
    int nread = sscanf(line,"%lf %lf %lf %lf %lf %lf %lf",
		       &r[i],&obs[i],&sig2lo[i],&sig1lo[i],&exp[i],&sig1hi[i],&sig2hi[i]);

    if (nread == 0) { cout << "zero read" << endl; continue; }
    else if (nread == 7) {

      cout << "obs["<<i<<"] = " << obs[i] << endl;

      if (obs[i]==0)    obs[i]   =1e-12;
      if (exp[i]==0)    exp[i]   =1e-12;
      if (sig2lo[i]==0) sig2lo[i]=1e-12;
      if (sig1lo[i]==0) sig1lo[i]=1e-12;
      if (sig1hi[i]==0) sig1hi[i]=1e-12;
      if (sig2hi[i]==0) sig2hi[i]=1e-12;

      sig1hi[i] -= exp[i];
      sig1lo[i]  = exp[i]-sig1lo[i];
      sig2hi[i] -= exp[i];
      sig2lo[i]  = exp[i]-sig2lo[i];
      i++; npts=i;
    }
    else {
      cout << "error reading line " << line << endl;
    }
  }
  fclose(fp);

  cout << "Read " << npts << " points." << endl;

  TGraph *obslim=new TGraph(npts,r,obs); // obslim->Print();
  TGraph *explim=new TGraph(npts,r,exp); // explim->Print();
  TGraphAsymmErrors *exp1sig=new TGraphAsymmErrors(npts,r,exp,0,0,sig1lo,sig1hi);
  TGraphAsymmErrors *exp2sig=new TGraphAsymmErrors(npts,r,exp,0,0,sig2lo,sig2hi);

  setTDRStyle();
  gStyle->SetLegendBorderSize(0);

  // Style for lines
  explim->SetLineWidth(3);
  explim->SetLineStyle(2);
  obslim->SetLineWidth(3);
  obslim->SetLineColor(1);
  obslim->SetLineStyle(1);
  exp1sig->SetFillColor(kGreen); // kBlue - 7);
  exp2sig->SetFillColor(kYellow);

  TMultiGraph* mg = new TMultiGraph();
  mg->SetName("");
  //  mg->SetTitle(";#sigma_{probe} / #sigma_{pred}; Signal Hypothesis P-value");
  // mg->SetTitle(";cross section #times BR(W#rightarrowl#nu) (pb); Signal p-value");
  mg->SetTitle(";cross section #times BR(W#rightarrowl#nu) (pb); Signal CL_{s} value");

  mg->Add(exp2sig, "E3");
  mg->Add(exp1sig, "E3");
  mg->Add(explim, "L");
  mg->Add(obslim, "L");

  gStyle->SetPadRightMargin(0.05);
  TCanvas *c1 = new TCanvas("c1","c1",450,400);
  float minYval = 5e-5;
  mg->Draw("AH");
  mg->GetXaxis()->SetRangeUser(r[0], 4);
  mg->GetYaxis()->SetRangeUser(minYval,1) ;
  //mg->Draw("A");
#if 0
  TLine lwh,lzp,ltc, lcdf;
  lwh.SetLineWidth(3);
  lwh.SetLineColor(2);
  lwh.DrawLine(1.45, minYval, 1.45,1);

  lzp.SetLineWidth(3);
  lzp.SetLineColor(4);
  lzp.SetLineStyle(9);
  lzp.DrawLine(1.72, minYval,1.72,1);


  ltc.SetLineWidth(5);
  ltc.SetLineColor(9);
  ltc.SetLineStyle(3);
  ltc.DrawLine(1.58, minYval,1.58,1);


  lcdf.SetLineWidth(3);
  lcdf.SetLineColor(6);
  //lcdf.SetLineStyle(7);
  lcdf.DrawLine(3.43, minYval,3.43, 1e-4);
#endif


  //gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  mg->GetHistogram()->DrawCopy("sameaxis");
  mg->GetHistogram()->DrawCopy("sameaxig");
  //gPad->RedrawAxis("g");


  TLegend *leg = new TLegend(0.62, 0.65, 0.94, 0.92, NULL, "brNDC");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(obslim, "Observed", "L");
  leg->AddEntry(explim, "Expected", "L");
  leg->AddEntry(exp1sig, "Expected #pm1#sigma", "F");
  leg->AddEntry(exp2sig, "Expected #pm2#sigma", "F");
#if 0
  leg->AddEntry(&lcdf, "CDF-like excess", "L");
  leg->AddEntry(&lwh, "WH #times 100", "L");
  leg->AddEntry(&ltc, "Technicolor", "L");
  leg->AddEntry(&lzp, "Leptophobic Z'", "L");
#endif
  leg->Draw();

  cmsLumi(prelim);

  gPad->Update();
  TString basename("mjjpvalues");
  if (prelim) basename += "_prelim";
  c1->SaveAs(basename+".png");
  c1->SaveAs(basename+".eps");
  c1->SaveAs(basename+".C");
}
