//root -b -q -l upperLimit.C+
#include <string>
#include <iostream>

#include "TH1F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
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

#include "tdrstyle.C"

////CMS lumi -- upper left corner
void cmsLumi(bool prelim)
{
  //const float LUMINOSITY = 4.73;
  const float LUMINOSITY = 5.02;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(11); // align left
    latex.DrawLatex(0.65,0.87,Form("#int #font[12]{L} dt = %.1f fb^{-1}", LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,prelim ? "CMS preliminary" : "CMS");
}

void plotMjjLimitBars(std::string limitFile = "limitSummary.txt", bool prelim=false)
{
  double obs[20],exp[20],sig1hi[20],sig1lo[20],sig2hi[20],sig2lo[20];
  double xi[20],exl[20],exh[20];
  char model[20][20];
  char line[256];

  FILE *fp=fopen(limitFile.c_str(),"r");
  if (!fp) {
    cerr << limitFile << " doesn't exist." << endl;
    exit(-1);
  }
  int npts=0;
  for (int i=0; !feof(fp) && fgets(line,256,fp); ) {
    if (line[0]=='#') continue;
    int nread = sscanf(line,"%s %lf %lf %lf %lf %lf %lf",
		       model[i],&obs[i],&sig2lo[i],&sig1lo[i],&exp[i],&sig1hi[i],&sig2hi[i]);

    switch(model[i][0]) {
    case 't': strcpy(model[i],"Technicolor"); break;
    case 'w': strcpy(model[i],"WH #times 100"); break;
    case 'z': strcpy(model[i],"Leptophobic Z'"); break;
    case 'g': strcpy(model[i],"Generic Gaussian"); break;
    case 'c': strcpy(model[i],"CDF-like excess"); break;
    }

    if (nread == 7) {
      xi [i]     = i+1;
      exl[i]     = 0.25;
      exh[i]     = 0.25;
      sig1hi[i] -= exp[i];
      sig1lo[i]  = exp[i]-sig1lo[i];
      sig2hi[i] -= exp[i];
      sig2lo[i]  = exp[i]-sig2lo[i];
      i++; npts=i;
    }
  }
  fclose(fp);

  cout << "Read " << npts << " points." << endl;

  TGraphErrors      *obslim =new TGraphErrors(npts,xi,obs,exl); obslim->Print();
  TGraphErrors      *explim =new TGraphErrors(npts,xi,exp,exl); explim->Print();
  TGraphAsymmErrors *exp1sig=new TGraphAsymmErrors(npts,xi,exp,exl,exh,sig1lo,sig1hi);
  TGraphAsymmErrors *exp2sig=new TGraphAsymmErrors(npts,xi,exp,exl,exh,sig2lo,sig2hi);

  setTDRStyle();

  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetLegendBorderSize(0);
  //gStyle->SetLegendFillColor(0);

  // Style for lines
  explim->SetLineWidth(3);
  explim->SetLineStyle(2);
  obslim->SetLineWidth(3);
  obslim->SetLineColor(1);
  obslim->SetLineStyle(1);
  exp1sig->SetFillColor(kGreen); // kBlue - 7);
  exp2sig->SetFillColor(kYellow);

  TMultiGraph* mg = new TMultiGraph();
  mg->UseCurrentStyle();
  mg->SetName("");
  mg->SetTitle("; ;#sigma_{excluded} / #sigma_{predicted}"); // (gg #rightarrow H #rightarrow WW #rightarrow #ell#nujj)");

  obslim->SetMarkerStyle(20);

  mg->Add(exp2sig, "E2");
  mg->Add(exp1sig, "E2");
  mg->Add(explim, "P");
  mg->Add(obslim, "P");

  //gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.07);
  gStyle->SetPadLeftMargin(0.13);
  TCanvas *c1 = new TCanvas("cbars","cbars",450,400);

  mg->Draw("AH");

  mg->GetHistogram()->GetXaxis()->Set(npts,0.5,npts+0.5);
  mg->GetHistogram()->GetYaxis()->SetRangeUser(0., 4.0);
  //mg->GetHistogram()->GetYaxis()->SetTitleSize(0.07);
  mg->GetHistogram()->GetYaxis()->SetTitleOffset(0.8);
  mg->GetHistogram()->GetYaxis()->SetLabelSize(0.05);
  mg->GetHistogram()->GetXaxis()->SetLabelSize(0.05);
  mg->GetHistogram()->GetYaxis()->SetNdivisions(505);

  for (int i=1; i<=npts; i++)
    mg->GetHistogram()->GetXaxis()->SetBinLabel(i,model[i-1]);

  //mg->GetXaxis()->SetRangeUser(0., double(npts));
  mg->GetYaxis()->SetRangeUser(0,3);
  //mg->Draw("A");

  TLine l;
  l.SetLineWidth(2);
  l.SetLineColor(2);
  l.DrawLine(mg->GetXaxis()->GetXmin(),1.0,mg->GetXaxis()->GetXmax(),1.0);


  TLegend *leg = new TLegend(0.16, 0.75, 0.55, 0.92, NULL, "brNDC");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->AddEntry(obslim, "95% C.L. Observed Limit", "L");
  leg->AddEntry(explim, "95% C.L. Expected Limit", "L");
  leg->AddEntry(exp1sig, "#pm1#sigma Expected Limit", "F");
  leg->AddEntry(exp2sig, "#pm2#sigma Expected Limit", "F");
  leg->Draw();

  cmsLumi(prelim);
  //drawStandardText("CMS PRELIMINARY 2011",0.78,0.68);
  //drawLumiText(0.78,0.63,2.1);

  TString basename("mjjlimitbars");
  if (prelim) basename += "_prelim";
  gPad->SetGridy();
  gPad->Update();
  gPad->SaveAs(basename+".png");
  gPad->SaveAs(basename+".eps");
  gPad->SaveAs(basename+".C");
}
