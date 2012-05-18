//root -b -q -l upperLimit.C+
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
#include "TString.h"

#include "tdrstyle.C"

#include "hwwinputs.h"

//#define LOGSCALE

////CMS Preliminary label and lumi -- upper left corner
void cmsLumi(bool prelim)
{
  const float LUMINOSITY = intlumipbinv * global_scale/1000.;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);
  latex.SetTextFont(42);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 8 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(11); // align left
    latex.DrawLatex(0.5,0.96,Form("#scale[0.5]{#int} #font[12]{L} dt = %.1f fb^{-1}", LUMINOSITY));
    //latex.DrawLatex(0.45,0.96,Form("#scale[0.5]{#int} #font[12]{L} dt = %.1f fb^{-1} (scaled)", LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,prelim ? "CMS preliminary" : "CMS");
}

void plotLimit(TString limitFile = "limit-cfginfo.tab",
	       bool plotObs = false,
	       bool plotPrelim=true)
{
  double m[50],obs[50],exp[50],sig1hi[50],sig1lo[50],sig2hi[50],sig2lo[50];
  char line[256];

  FILE *fp=fopen(limitFile.Data(),"r");
  if (!fp) {
    cerr << limitFile << " doesn't exist." << endl;
    exit(-1);
  }
  int npts=0;
  for (int i=0; !feof(fp) && fgets(line,256,fp); ) {
    if (line[0]=='#') continue;
    int nread = sscanf(line,"%lf %lf %lf %lf %lf %lf %lf",
		       &m[i],&obs[i],&sig2lo[i],&sig1lo[i],&exp[i],&sig1hi[i],&sig2hi[i]);

    if (nread == 0) continue;
    else if (nread == 7) {
      sig1hi[i] -= exp[i];
      sig1lo[i] = exp[i]-sig1lo[i];
      sig2hi[i] -= exp[i];
      sig2lo[i] = exp[i]-sig2lo[i];
      i++; npts=i;
    }
  }
  fclose(fp);

  cout << "Read " << npts << " points." << endl;

  TGraph *obslim=new TGraph(npts,m,obs); // obslim->Print();
  TGraph *explim=new TGraph(npts,m,exp); // explim->Print();
  TGraphAsymmErrors *exp1sig=new TGraphAsymmErrors(npts,m,exp,0,0,sig1lo,sig1hi);
  TGraphAsymmErrors *exp2sig=new TGraphAsymmErrors(npts,m,exp,0,0,sig2lo,sig2hi);

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
  mg->SetTitle(";M_{H} (GeV);#sigma_{excluded} / #sigma_{SM}"); // (gg #rightarrow H #rightarrow WW #rightarrow #ell#nujj)");

  mg->Add(exp2sig, "E3");
  mg->Add(exp1sig, "E3");
  mg->Add(explim, "L");
  if (plotObs)
    mg->Add(obslim, "L");

  TCanvas *c1 = new TCanvas("c1","c1",800,600);

  mg->Draw("AH");
  //mg->GetXaxis()->SetRangeUser(0., double(npts));

#ifdef LOGSCALE
  mg->GetYaxis()->SetRangeUser(5e-2,40) ;
#else
  mg->GetYaxis()->SetRangeUser(0,30) ;
#endif

  //mg->Draw("A");

  TLine l;
  l.SetLineWidth(2);
  l.SetLineColor(2);
  l.DrawLine(mg->GetXaxis()->GetXmin(),1.0,mg->GetXaxis()->GetXmax(),1.0);


  gPad->SetGridy();

#ifdef LOGSCALE
  gPad->SetLogy();
#endif

  mg->GetHistogram()->DrawCopy("sameaxis");
  mg->GetHistogram()->DrawCopy("sameaxig");

  TLegend *leg = new TLegend(0.58, 0.77, 0.95, 0.94, NULL, "brNDC");
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetFillColor(0);
  if (plotObs)
    leg->AddEntry(obslim, "95% C.L. Observed Limit", "L");
  leg->AddEntry(explim, "95% C.L. Expected Limit", "L");
  leg->AddEntry(exp1sig, "#pm1#sigma Expected Limit", "F");
  leg->AddEntry(exp2sig, "#pm2#sigma Expected Limit", "F");
  leg->Draw();

  cmsLumi(plotPrelim);

  gPad->Update();

  TString output1=limitFile, output2=limitFile;

  output1.Replace(limitFile.Length()-4,4, plotObs ? ".png" : "_exponly.png");
  output2.Replace(limitFile.Length()-4,4, plotObs ? ".eps" : "_exponly.eps");

  gPad->SaveAs(output1.Data());
  gPad->SaveAs(output2.Data());
}
