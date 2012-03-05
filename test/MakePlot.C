#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TPaveText.h"
#include "TArrow.h"
#include "TGraph.h"
#include "TFile.h"
#include "TStyle.h"
#include "TMarker.h"

#include "./MakePlot.h"

// ====================================================================================
// test code
// ====================================================================================
void TestPieceCode()
{
  return;
}


// ====================================================================================
// TDR setup
// ====================================================================================
void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //  tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}


// ====================================================================================
// overflowbin
// ====================================================================================
TH1F * addoverflowbin(TH1F *h)
{ 
  UInt_t     nx    = h->GetNbinsX()+1;
  Double_t * xbins = new Double_t[nx+1];
  for (UInt_t i=0;i<nx;i++){
    xbins[i] = h->GetBinLowEdge(1) + i*h->GetBinWidth(1);
    if(h->GetXaxis()->GetXbins()->GetArray()) 
      xbins[i]=*(h->GetXaxis()->GetXbins()->GetArray()+i);
  }
  xbins[nx]=xbins[nx-1]+h->GetBinWidth(nx);
  // Book a temporary histogram having ab extra bin for overflows
  char tpname[100]; 
  sprintf(tpname, "%s-overflowbin", h->GetName());
  TH1F *htmp = new TH1F(tpname, h->GetTitle(), nx, xbins);
  // Reset the axis labels
  htmp->SetXTitle(h->GetXaxis()->GetTitle());
  htmp->SetYTitle(h->GetYaxis()->GetTitle());
  for (UInt_t i=1; i<=nx; i++)
    htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i)); // Fill the new hitogram including the extra bin for overflows
  htmp->Fill(h->GetBinLowEdge(1)-1, h->GetBinContent(0));   // Fill the underflows
  htmp->SetEntries(h->GetEntries());                        // Restore the number of entries
  return htmp;
}


// ====================================================================================
// project histogram 
// ====================================================================================
TH1F * getHist( const char *filename, 
		const char *tree, 
		const char *leaf, 
		const char *cut,
		const char *hist1, int nbin, float xmin, float xmax,
		int rflag  = 0        ) {
  TFile *f1 = (TFile *) gROOT->GetListOfFiles()->FindObject(filename);
  if (!f1 || !f1->IsOpen()) {
    //std::cout << "load into memory: " << filename << std::endl;
    f1 = new TFile(filename, "READ");
  }
  if (!f1) {
    std::cout << "can't open files" << std::endl;
    return 0;
  }
  f1->cd();
  TTree *tr = (TTree *)f1->Get(tree);
  TH1F  *h1 = new TH1F(hist1, hist1, nbin, xmin, xmax);
  if (!tr || !h1) return 0;
  tr->SetBranchStatus("*", 1);
  tr->Project(hist1, leaf, cut);
  //tr->Draw((string(leaf)+">>"+string(hist1)).c_str(), cut);
  if (rflag==0){
    return h1;}
  else         {
    TH1F * hov = addoverflowbin(h1); 
    return hov;
  }
}


// ====================================================================================
// overflowbin
// ====================================================================================
void thishisthaveoerflowbin(void) {
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(90);
  t->SetTextAlign(12);
  t->SetTextSize(0.02);
  t->DrawLatex(0.96, 0.18, "Overflow Bin");
  t->Draw();
}


// ====================================================================================
// project histogram 
// ====================================================================================
TH1F * getHist( const char *filename, 
		       const char *tree, 
		       const char *leaf, 
		       const char *cut,
		       const char *hist1, int nbin, double xBinArray[],
		       int rflag  = 0        ) {
  TFile *f1 = (TFile *) gROOT->GetListOfFiles()->FindObject(filename);
  if (!f1 || !f1->IsOpen()) {
    //std::cout << "load into memory: " << filename << std::endl;
    f1 = new TFile(filename, "READ");
  }
  if (!f1) {
    std::cout << "can't open files" << std::endl;
    return 0;
  }
  f1->cd();
  TTree *tr = (TTree *)f1->Get(tree);
  TH1F  *h1 = new TH1F(hist1, hist1, nbin, xBinArray);
  if (!tr || !h1) return 0;
  tr->SetBranchStatus("*", 1);
  tr->Project(hist1, leaf, cut);
  //tr->Draw((string(leaf)+">>"+string(hist1)).c_str(), cut);
  
  if (rflag==0){
    return h1;}
  else         {
    TH1F * hov = addoverflowbin(h1); 
    return hov;
  }
}


// ====================================================================================
// Self Function
// ====================================================================================
void drawfreetext(string text = "Test",      double xx=0.45, double yy=0.40, double tcolor=2 ,double tsize = 0.045) {
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(0);
  t->SetTextAlign(12);
  t->SetTextSize(tsize);
  t->SetTextColor(tcolor);
  t->DrawLatex(xx, yy, text.c_str());
  t->Draw();
}


// ====================================================================================
// Self Function
// ====================================================================================
void cmspre()
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);
  
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.93,"#sqrt{s} = 7 TeV");
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.70,0.93,Form("#int #font[12]{L} dt = %.1f fb^{-1}", 4.7));

  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.05,0.93,"CMS preliminary 2011");

}


// ====================================================================================
// Self Function
// ====================================================================================
void saveCanvas(TCanvas    * MyCa, 
		const char * outfilename = "tmp")
{
  //MyCa->Print((string(outfilename)+ ".eps").c_str());
  //MyCa->Print((string(outfilename)+ ".gif").c_str());
  MyCa->Print((string(outfilename)+ ".png").c_str());
  MyCa->Print((string(outfilename)+ ".pdf").c_str());
  MyCa->Print((string(outfilename)+".root").c_str());
}
  
// ====================================================================================
// Self Function
// ====================================================================================
void ListSample()
{ //Make Table for MC Samples
  char tmpc[1000];
  for(int i = 0; i<TotI;i++) {
    sprintf(tmpc, "Sample %2.2i & %-50s & %-15.0f & %-15.3f & %-15.3f & %-15.3f & %-15.1f \\\\", 
	    i, PlainNames[i],XSecs[i][2],XSecs[i][0],XSecs[i][1],XSecs[i][3],
            XSecs[i][2]/ (XSecs[i][0]*XSecs[i][1]*XSecs[i][3])                   );
    cout << tmpc << endl;
  }
}



// ====================================================================================
// Self Function
// ====================================================================================
void OptimizeMVAOutPut(int myflag = 0, int myflagb=-1, float opmva =0.50,  int tuneAlpha = 0,
		       int a_nbin = 70, float a_led= 100, float a_hed=800,
		       float sblL = 55, float sblH = 65,  float sbhL =95,  float sbhH = 115) {  setTDRStyle();
  // Save output to file
  char tpname[100];

  const float  syswsh      = 0.01;
  const int    nbin        = 100;  const int   rflag  = 0; 
  const int    nmp         = 48; // Total number of mva training points
  const int    nbgc        = 12;
  const int    sqnc        = 1;
  const int    nvbs        = 8;  // Total number of variables using to do CC
  const char * nnff[nvbs]  = {"mva","ang_hs","Jet1QGLhood",             "Jet2QGLhood",             "ang_hb","ang_ha","ptlvjj","ylvjj"};
  const char * ccva[nvbs]  = {"mva","ang_hs","JetPFCor_QGLikelihood[0]","JetPFCor_QGLikelihood[1]","ang_hb","ang_ha","ptlvjj","ylvjj"};
//const int    apcc[nvbs]  = {0,    1,    1,    1,    1,    1,    1,    1   };
  const int    apcc[nvbs]  = {1,    0,    0,    0,    0,    0,    0,    0   };
  const int    sccc[nvbs]  = {1,    3,    1,    1,    2,    3,    0,    3   };
  float        ndef[nvbs]  = {0.24, 0.55, 0.80, 0.80, 0.85, 0.90, 0.90, 0.95};
  const char * cate[nmp]   = {
    "2j170mu","2j180mu","2j190mu","2j200mu","2j250mu","2j300mu","2j350mu","2j400mu","2j450mu","2j500mu","2j550mu","2j600mu",
    "3j170mu","3j180mu","3j190mu","3j200mu","3j250mu","3j300mu","3j350mu","3j400mu","3j450mu","3j500mu","3j550mu","3j600mu",
    "2j170el","2j180el","2j190el","2j200el","2j250el","2j300el","2j350el","2j400el","2j450el","2j500el","2j550el","2j600el",
    "3j170el","3j180el","3j190el","3j200el","3j250el","3j300el","3j350el","3j400el","3j450el","3j500el","3j550el","3j600el", };
  const char * plcut[nmp]    = {
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>165&&fit_mlvjj<180)&&fit_status==0)", // 2j170mu =====
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>170&&fit_mlvjj<200)&&fit_status==0)", // 2j180mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>185&&fit_mlvjj<210)&&fit_status==0)", // 2j190mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>190&&fit_mlvjj<220)&&fit_status==0)", // 2j200mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>226&&fit_mlvjj<287)&&fit_status==0)", // 2j250mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>265&&fit_mlvjj<347)&&fit_status==0)", // 2j300mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>308&&fit_mlvjj<401)&&fit_status==0)", // 2j350mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>346&&fit_mlvjj<457)&&fit_status==0)", // 2j400mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>381&&fit_mlvjj<512)&&fit_status==0)", // 2j450mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>415&&fit_mlvjj<568)&&fit_status==0)", // 2j500mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>440&&fit_mlvjj<617)&&fit_status==0)", // 2j550mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>462&&fit_mlvjj<663)&&fit_status==0)", // 2j600mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>165&&fit_mlvjj<190)&&fit_status==0)", // 3j170mu =====
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>170&&fit_mlvjj<210)&&fit_status==0)", // 3j180mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>185&&fit_mlvjj<220)&&fit_status==0)", // 3j190mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>190&&fit_mlvjj<230)&&fit_status==0)", // 3j200mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>216&&fit_mlvjj<300)&&fit_status==0)", // 3j250mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>241&&fit_mlvjj<355)&&fit_status==0)", // 3j300mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>269&&fit_mlvjj<407)&&fit_status==0)", // 3j350mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>300&&fit_mlvjj<465)&&fit_status==0)", // 3j400mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>332&&fit_mlvjj<518)&&fit_status==0)", // 3j450mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>362&&fit_mlvjj<569)&&fit_status==0)", // 3j500mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>398&&fit_mlvjj<616)&&fit_status==0)", // 3j550mu
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>419&&fit_mlvjj<660)&&fit_status==0)", // 3j600mu
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>165&&fit_mlvjj<180)&&fit_status==0)", // 2j170el =====
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>170&&fit_mlvjj<200)&&fit_status==0)", // 2j180el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>185&&fit_mlvjj<210)&&fit_status==0)", // 2j190el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>190&&fit_mlvjj<220)&&fit_status==0)", // 2j200el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>226&&fit_mlvjj<287)&&fit_status==0)", // 2j250el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>265&&fit_mlvjj<347)&&fit_status==0)", // 2j300el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>308&&fit_mlvjj<401)&&fit_status==0)", // 2j350el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>346&&fit_mlvjj<457)&&fit_status==0)", // 2j400el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>381&&fit_mlvjj<512)&&fit_status==0)", // 2j450el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>415&&fit_mlvjj<568)&&fit_status==0)", // 2j500el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>440&&fit_mlvjj<617)&&fit_status==0)", // 2j550el
    "(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>462&&fit_mlvjj<663)&&fit_status==0)", // 2j600el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>165&&fit_mlvjj<190)&&fit_status==0)", // 3j170el =====
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>170&&fit_mlvjj<210)&&fit_status==0)", // 3j180el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>185&&fit_mlvjj<220)&&fit_status==0)", // 3j190el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>190&&fit_mlvjj<230)&&fit_status==0)", // 3j200el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>216&&fit_mlvjj<300)&&fit_status==0)", // 3j250el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>241&&fit_mlvjj<355)&&fit_status==0)", // 3j300el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>269&&fit_mlvjj<407)&&fit_status==0)", // 3j350el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>300&&fit_mlvjj<465)&&fit_status==0)", // 3j400el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>332&&fit_mlvjj<518)&&fit_status==0)", // 3j450el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>362&&fit_mlvjj<569)&&fit_status==0)", // 3j500el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>398&&fit_mlvjj<616)&&fit_status==0)", // 3j550el
    "(ggdevt==3&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>419&&fit_mlvjj<660)&&fit_status==0)", // 3j600el
  };
  TH1F    * hh[nvbs][nmp][nbgc];
  TH1F    * hs[nvbs][nmp]; 
  TH1F    * hb[nvbs][nmp]; 
  TH1F    * hw[nvbs][nmp]; 
  TH1F    * hd[nvbs][nmp]; 
  THStack * Ts[nvbs][nmp]; 
  TLegend * Lg[nvbs][nmp];
  int      opl[nvbs][nmp];
  int      oph[nvbs][nmp];
  TH2F *opHist[nvbs][nmp];
  TH1D * opXop[nvbs][nmp];
  TH1D * opYop[nvbs][nmp];
  TH1D * opDop[nvbs][nmp];

  // Alpha Scan
  const char * f_plmassvar       = "fit_mlvjj";
  int aba[nmp]={0}; int abl[nmp]={0}; int abh[nmp]={0};
  
  for (int p=0; p<nmp; p++){ if (p!=myflag && myflag>=0) continue;
    FILE *textfile = fopen(Form("HwwOptimizeResult-Point-%s.txt",cate[p]),"w");
    int ctt = (int)(p/12); // Category
    int winl = atoi(&plcut[p][59]), winh= atoi(&plcut[p][74]); //cout << "Win : " << winl << " -- " << winh << endl;    return;
    float presS = 0, presB=0; float mvaCut=0;
    std::vector<std::string> allopcuts;
    char ppvar[200] = ""; char ppcut[5000] = ""; char opcutC[5000] = "(1==1)";
    for (int c=0; c<nvbs; c++){ if (c!=myflagb && myflagb>=0) continue;
      int sctype = sccc[c]; float xmin  = 0.0; float xmax = 1.0;
      sprintf(ppvar,"%s",ccva[c]);sprintf(ppcut,"effwt*(%s)",plcut[p]);
      // Setup Cut and Variable
      if (c==0){xmin=  0.0; xmax= 1.0; sprintf(ppvar,"%s%s",ccva[c], cate[p]);}//sprintf(ppcut,"%s",plcut[p]);}
      if (c==1){xmin= -1.0; xmax= 1.0; }
      if (c==2){xmin=  0.01;xmax= 1.01;}
      if (c==3){xmin=  0.01;xmax= 1.01;}
      if (c==4){xmin=  0.0; xmax= 1.0; }
      if (c==5){xmin= -1.0; xmax= 1.0; }
      if (c==6){xmin=  0.0; xmax= 200; }
      if (c==7){xmin= -2.5; xmax= 2.5; }
      if (sqnc==1) sprintf(ppcut,"effwt*(%s&&%s)",plcut[p],opcutC);
      if (sqnc==0) ndef[c] = ndef[0];
      cout << ppvar << "  @  " << ppcut << endl;
      break;
      // Fill All background
      sprintf(tpname, "Ts[%i][%i]",c,p); Ts[c][p] = new THStack(tpname, ""); Lg[c][p] = new TLegend(0.75,0.55,0.90,0.85);
      sprintf(tpname, "hb[%i][%i]",c,p); hb[c][p] = getHist(Mu_InFileNames[9], "WJet", "mva2j170mu", "0", tpname, nbin, xmin, xmax, rflag); hb[c][p]->Sumw2();// Make an empty hist
      for (int i = nbgc-1; i>=0; i--){ if (i==9) continue; //skip QCD
      	sprintf(tpname, "hh[%i][%i][%i]",c, p, i); 
      	if (ctt==0||ctt==1)hh[c][p][i] = getHist(Mu_InFileNames[i], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);
      	if (ctt==2||ctt==3)hh[c][p][i] = getHist(El_InFileNames[i], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);
      	double m_norfac =  DatLumi_mu *1.0/(XSecs[i][2]/ (XSecs[i][0]*XSecs[i][1]*XSecs[i][3]));
      	hh[c][p][i]->Sumw2(); hh[c][p][i]->Scale(m_norfac); hh[c][p][i]->SetFillColor(i); 
      	Ts[c][p]->Add(hh[c][p][i]); Lg[c][p]->AddEntry(hh[c][p][i],PlainNames[i],"f"); hb[c][p]->Add(hh[c][p][i],+1);
      }
      // Fill Data
      sprintf(tpname, "hd[%i][%i]",c,p);
      if (ctt==0||ctt==1)hd[c][p] = getHist(Mu_InFileNames[51], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);
      if (ctt==2||ctt==3)hd[c][p] = getHist(El_InFileNames[51], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);      hd[c][p]->Sumw2();
      
      // Fill Higgs Signal
      int lb = 0; if (ctt==0) lb = p+22;if (ctt==1) lb = p+10;if (ctt==2) lb = p-2;if (ctt==3) lb = p-14;
      double g_norfac = DatLumi_mu *1.0/(XSecs[lb][2]/ (XSecs[lb][0]*XSecs[lb][1]*XSecs[lb][3])); 
      sprintf(tpname, "hs[%i][%i]",c,p);
      if (ctt==0||ctt==1)hs[c][p] = getHist(Mu_InFileNames[lb], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);
      if (ctt==2||ctt==3)hs[c][p] = getHist(El_InFileNames[lb], "WJet", ppvar, ppcut, tpname, nbin, xmin, xmax, rflag);      hs[c][p]->Sumw2(); hs[c][p]->Scale(g_norfac);
      
      // Fill Wjj Special
      sprintf(tpname, "hw[%i][%i]",c,p);       hw[c][p]=(TH1F*)hh[c][p][0]->Clone(tpname);
      
      // Do optimization
      float bwef[nbin][nbin+1],bbef[nbin][nbin+1],ssef[nbin][nbin+1],ctll[nbin][nbin+1],cthh[nbin][nbin+1],sovb[nbin][nbin+1];
      float exss[nbin][nbin+1],exbb[nbin][nbin+1],exbw[nbin][nbin+1],obda[nbin][nbin+1];
      float maxsovb = -999;
      for(int i=0; i<nbin; i++){ // initial
	for(int j=0; j<nbin+1; j++){
	  ctll[i][j]=0;cthh[i][j]=0;bwef[i][j]=0;bbef[i][j]=0;ssef[i][j]=0;exss[i][j]=0;exbb[i][j]=0;exbw[i][j]=0;obda[i][j]=0;sovb[i][j]=0;
	}
      }
      for(int i=0; i<nbin; i++){
	for(int j=i+1; j<nbin+1; j++){
	  ctll[i][j] = xmin+ 1.0*i*(xmax-xmin)/nbin;  cthh[i][j] = xmin+ 1.0*j*(xmax-xmin)/nbin;
	  bwef[i][j] = hw[c][p]->Integral(i+1, j) /hw[c][p]->Integral();
	  bbef[i][j] = hb[c][p]->Integral(i+1, j) /hb[c][p]->Integral();
	  ssef[i][j] = hs[c][p]->Integral(i+1, j) /hs[c][p]->Integral();
	  float opSS = hs[c][p]->Integral(i+1, j);
	  float opBB = hb[c][p]->Integral(i+1, j);
	  float opBW = hw[c][p]->Integral(i+1, j);
	  float opOB = hd[c][p]->Integral(i+1, j);
	  exss[i][j] = opSS; exbb[i][j]=opBB; exbw[i][j]=opBW; obda[i][j]=opOB;
	  if (sctype== 1) {if(j!=nbin)    {continue;}} // Only allowed some of points to go for optimization
	  if (sctype== 2) {if(i!=0)       {continue;}}
	  if (sctype== 3) {if(i!=(nbin-j)){continue;}}
	  sovb[i][j] = 0; if (opBB!=0) {sovb[i][j] = opSS/sqrt( opBB + syswsh*syswsh*opBB*opBB );}
	  if (ssef[i][j]>ndef[c] ) {  if (sovb[i][j]>maxsovb){ maxsovb= sovb[i][j]; opl[c][p] = i; oph[c][p] = j;}  }
	}
      }
      opHist[c][p]    = new TH2F(Form("opHist[%i][%i]",c,p),"",nbin,xmin,xmax,nbin,xmin,xmax);
      for(int i=0; i<nbin; i++){for(int j=1; j<nbin+1; j++){opHist[c][p]->SetBinContent(i+1,j,sovb[i][j]);}}
      opXop[c][p] = (TH1D*) opHist[c][p]->ProjectionX(Form("opXop[%i][%i]",c,p), nbin,nbin);
      opYop[c][p] = (TH1D*) opHist[c][p]->ProjectionY(Form("opYop[%i][%i]",c,p), 1,1      );
      opDop[c][p] = (TH1D*) opHist[c][p]->ProjectionY(Form("opDop[%i][%i]",c,p)           );
      float lcc = ctll[opl[c][p]][oph[c][p]], hcc = cthh[opl[c][p]][oph[c][p]]; if (c==0) mvaCut = lcc;
      float bee = ssef[opl[c][p]][oph[c][p]], bdd = sovb[opl[c][p]][oph[c][p]];
      float exs = exss[opl[c][p]][oph[c][p]], exb = exbb[opl[c][p]][oph[c][p]];
      if (c==1||myflagb>=0) {presS=hs[c][p]->Integral(); presB=hb[c][p]->Integral();}
      // ======================================================================================================
      // === End Fill === Start Plot
      // ======================================================================================================
      gStyle->SetOptFit(0);
      gStyle->SetOptStat(0);

      // Plot 0
      TCanvas * MyCa0     = new TCanvas("MyCa0","MyCa0",10,10,500,500); MyCa0->Draw(); //MyCa0->SetGridx(1); MyCa0->SetGridy(1); //MyCa0->SetLeftMargin(0.25);
      if (sctype== 0) {
	gStyle->SetPalette(1);
	opHist[c][p]->GetXaxis()->SetTitle(Form("Low Cut"));
	opHist[c][p]->GetYaxis()->SetTitle(Form("High Cut"));
	opHist[c][p]->GetZaxis()->SetTitle(Form("Significance"));
	opHist[c][p]->Draw("SURF3");
      }
      else {
	TH1D * opPlHist = (TH1D*)opXop[c][p]->Clone("opPlHist");
	if (sctype== 1) {opPlHist = (TH1D*)opXop[c][p]->Clone("opPlHist");}
	if (sctype== 2) {opPlHist = (TH1D*)opYop[c][p]->Clone("opPlHist");}
	if (sctype== 3) {opPlHist = (TH1D*)opDop[c][p]->Clone("opPlHist");}
	opPlHist->GetXaxis()->SetTitle(Form("%s - %s",ppvar,cate[p])); if (sctype==2||sctype==3) opPlHist->GetXaxis()->SetTitleOffset(0.9);
	opPlHist->GetYaxis()->SetTitle("#frac{S}{#sqrt{B+(#delta_{sys}#times B)^{2}}}   ");
	opPlHist->GetYaxis()->SetTitleOffset(-1.2);
	opPlHist->GetYaxis()->SetTitleSize(0.040);
	//opPlHist->GetXaxis()->CenterTitle();
	opPlHist->Draw("histc");
	TArrow * arrow0a = new TArrow(lcc, sovb[opl[c][p]][oph[c][p]], lcc, 0, 0.03,"->"); arrow0a->Draw("");
	TArrow * arrow0b = new TArrow(hcc, sovb[opl[c][p]][oph[c][p]], hcc, 0, 0.03,"->"); arrow0b->Draw("");
      }
      TPaveText *pta0 = new TPaveText(0.38, 0.25, 0.85, 0.50,"BRNDC");pta0->SetBorderSize(0);pta0->SetLineWidth(1);pta0->SetFillStyle(0);;pta0->SetTextAlign(12);
      sprintf(tpname, "Optimization #delta_{sys} = %.1f%%",  syswsh*100.0     );TText *text3f = pta0->AddText(tpname); text3f->SetTextColor(kBlue);
      sprintf(tpname, "%.3f<%s<%.3f",     lcc,ppvar,hcc                       );TText *text3a = pta0->AddText(tpname); text3a->SetTextColor(65);
      sprintf(tpname, "SoB Val : %.3f",   bdd                                 );TText *text3b = pta0->AddText(tpname); text3b->SetTextColor(kRed);
      sprintf(tpname, "Sig Eff : %.3f%%", 100*(                           bee));TText *text3c = pta0->AddText(tpname); text3c->SetTextColor(kRed);
      sprintf(tpname, "WJJ Rej : %.3f%%", 100*(1.0-bwef[opl[c][p]][oph[c][p]]));TText *text3d = pta0->AddText(tpname); text3d->SetTextColor(kBlue);
      sprintf(tpname, "TBK Rej : %.3f%%", 100*(1.0-bbef[opl[c][p]][oph[c][p]]));TText *text3e = pta0->AddText(tpname); text3e->SetTextColor(43);
      sprintf(tpname, "SCType-%i - %s",   sctype,cate[p]                      );TText *text3g = pta0->AddText(tpname); text3g->SetTextColor(65);
      sprintf(tpname, "Sig=%.1f / Bkg=%.1f",   exs, exb                       );TText *text3h = pta0->AddText(tpname); text3h->SetTextColor(kRed);
      pta0->Draw(); 

      // Plot 1
      TH1F * sSp = (TH1F*) hs[c][p]->Clone("sSp");    sSp->Scale(1.0/sSp->Integral()); sSp->SetFillColor(kBlue);sSp->SetLineColor(kBlue);sSp->SetFillStyle(0);
      TH1F * wSp = (TH1F*) hw[c][p]->Clone("wSp");    wSp->Scale(1.0/wSp->Integral()); wSp->SetFillColor(kRed); wSp->SetLineColor(kRed); wSp->SetFillStyle(0);
      TH1F * bSp = (TH1F*) hb[c][p]->Clone("wSp");    bSp->Scale(1.0/bSp->Integral()); bSp->SetFillColor(kRed); bSp->SetLineColor(kRed); bSp->SetFillStyle(0);
      TCanvas * MyCa1     = new TCanvas("MyCa1","MyCa1",10,10,500,500); MyCa1->Draw();
      wSp->Draw("hist"); 
      wSp->SetXTitle(Form("%s - %s",ppvar,cate[p]));
      wSp->SetYTitle("dN/dx");
      sSp->Draw("samehist");
      TLegend * Lega1 = new TLegend(0.20,0.75,0.50,0.85);
      //Lega1->SetHeader(Form("%s - %s",ppvar,cate[p]));
      Lega1->AddEntry(sSp, "Hww Signal" ,     "f");
      Lega1->AddEntry(wSp, "Wjj Background" , "f");
      Lega1->SetFillStyle(0);
      Lega1->SetBorderSize(0);
      Lega1->Draw();
      pta0->Draw(); 
      TArrow * arrow1a = new TArrow(lcc, wSp->GetMaximum()*0.3, lcc, 0, 0.03,"->"); arrow1a->Draw("");
      TArrow * arrow1b = new TArrow(hcc, wSp->GetMaximum()*0.3, hcc, 0, 0.03,"->"); arrow1b->Draw("");
      
      // Plot 2
      TCanvas * MyCa2     = new TCanvas("MyCa2","MyCa2",10,10,500,500); MyCa2->Draw();
      bSp->Draw("hist"); 
      bSp->SetXTitle(Form("%s - %s",ppvar,cate[p]));
      bSp->SetYTitle("dN/dx");
      sSp->Draw("samehist");
      TLegend * Lega1b = new TLegend(0.20,0.75,0.50,0.85);
      //Lega1b->SetHeader(Form("%s - %s",ppvar,cate[p]));
      Lega1b->AddEntry(sSp, "Hww Signal" ,     "f");
      Lega1b->AddEntry(wSp, "Total Background" , "f");
      Lega1b->SetFillStyle(0);
      Lega1b->SetBorderSize(0);
      Lega1b->Draw();      
      pta0->Draw(); 
      TArrow * arrow2a = new TArrow(lcc, bSp->GetMaximum()*0.3, lcc, 0, 0.03,"->"); arrow2a->Draw("");
      TArrow * arrow2b = new TArrow(hcc, bSp->GetMaximum()*0.3, hcc, 0, 0.03,"->"); arrow2b->Draw("");
      
      // Plot 3
      TCanvas * MyCa3     = new TCanvas("MyCa3","MyCa3",10,10,500,500); MyCa3->Draw();
      hd[c][p]->SetMaximum(hd[c][p]->GetMaximum()*1.3);
      hd[c][p]->SetMarkerStyle(20);
      hd[c][p]->SetXTitle(Form("%s - %s",nnff[c],cate[p]));
      hd[c][p]->SetYTitle(Form("Events / %.2f",(xmax-xmin)/nbin));
      hd[c][p]->SetMarkerSize(0.5);
      hd[c][p]->Draw();
      Ts[c][p]->Draw("samehist");
      hd[c][p]->Draw("samee");
      hs[c][p]->SetLineColor(95); 
      hs[c][p]->SetLineStyle(2); 
      hs[c][p]->SetLineWidth(2);
      hs[c][p]->Draw("samehist");
      Lg[c][p]->AddEntry(hs[c][p], "Hww Signal", "l");
      Lg[c][p]->SetFillStyle(0);
      Lg[c][p]->SetBorderSize(0);
      Lg[c][p]->Draw();
      TArrow * arrow3a = new TArrow(lcc, hd[c][p]->GetMaximum()*0.3, lcc, 0, 0.03,"->"); arrow3a->Draw("");
      TArrow * arrow3b = new TArrow(hcc, hd[c][p]->GetMaximum()*0.3, hcc, 0, 0.03,"->"); arrow3b->Draw("");
      pta0->Draw(); 
      cmspre();
      MyCa3->RedrawAxis();
      if (apcc[c]==1) {
	sprintf(opcutC,"%s&&(%.3f<=%s&&%s<=%.3f)", opcutC, lcc, ppvar, ppvar, hcc);
	allopcuts.push_back(Form("S=%.2f | #epsilon:%.2f%% | (S/B=%.1f/%.1f) | Cut:%s[%.2f,%.2f]",bdd, bee*100.0, exs,exb, nnff[c],lcc,hcc));}
      
      //saveCanvas(MyCa0, Form("./figures/CCOptimize/cc-%s-optmz-%s-%i-%s",cate[p],nnff[c],sqnc,"sgnf")    );
      //saveCanvas(MyCa1, Form("./figures/CCOptimize/cc-%s-optmz-%s-%i-%s",cate[p],nnff[c],sqnc,"ssbw")    );
      //saveCanvas(MyCa2, Form("./figures/CCOptimize/cc-%s-optmz-%s-%i-%s",cate[p],nnff[c],sqnc,"ssbb")    );
      //saveCanvas(MyCa3, Form("./figures/CCOptimize/cc-%s-optmz-%s-%i-%s",cate[p],nnff[c],sqnc,"comp")    );
    }
    // ======================================================================================================
    // Now start to study the alpha method
    // ======================================================================================================
    // Setup for the alpha method, if need multi-alpha, this script only can do one by one.
    const int a_type =  0; const int a_npt  = 101; // sample and alpha scan step [0,1]
    const float sigL = 65, sigH =  95; // sig  range
    // tune all
    //sblL = 45, sblH = 65, sbhL =  95, sbhH = 100; //default of sideband
    char  isrva[100] = "";

    // Specify the range and nbin for alpha, now it's point dependent
    if (tuneAlpha==0) {
      if ( (p>=0  && p<=3 ) || 
	   (p>=12 && p<=15) ||
	   (p>=24 && p<=27) ||
	   (p>=36 && p<=39)   ) { a_nbin = 8; a_led= 170; a_hed=250;}
      if ( (p>=4  && p<=4 ) || 
	   (p>=16 && p<=16) ||
	   (p>=28 && p<=28) ||
	   (p>=40 && p<=40)   ) { a_nbin =15; a_led= 200; a_hed=500;}
      if ( (p>=5  && p<=5)  || 
	   (p>=17 && p<=17) ||
	   (p>=29 && p<=29) ||
	   (p>=41 && p<=41)   ) { a_nbin =13; a_led= 240; a_hed=500;}
      if ( (p>=6  && p<=11) || 
	   (p>=18 && p<=23) ||
	   (p>=30 && p<=35) ||
	   (p>=42 && p<=47)   ) { a_nbin =12; a_led= 300; a_hed=780;}
      
      if (p==2    || 
	  p==2+12 ||
	  p==2+24 ||
	  p==2+36 ||  
	  p==3    || 
	  p==3+12 ||
	  p==3+24 ||
	  p==3+36   )  { a_nbin = 7; a_led= 180; a_hed=250;}
      
      // Muon 2j 
      if (p==0     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==1     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==2     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==3     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==4     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==5     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==6     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==7     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==8     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==9     ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==10    ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==11    ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      
      // Muon 3j 
      if (p==0 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==1 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==2 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==3 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==4 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==5 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==6 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==7 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==8 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==9 +12 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==10+12 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==11+12 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      
      // Electron 2j
      if (p==0 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==1 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==2 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==3 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==4 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==5 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==6 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==7 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==8 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==9 +24 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==10+24 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==11+24 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      
      // Electron 3j
      if (p==0 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==1 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==2 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==3 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 115; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==4 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==5 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==6 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==7 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==8 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==9 +36 ) { sblL = 55; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==10+36 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
      if (p==11+36 ) { sblL = 40; sblH = 65; sbhL = 95; sbhH = 200; sprintf(isrva,"_ReAlpha_%.0f-%.0f_%.0f-%.0f",sblL,sblH,sbhL,sbhH); }
    }
    TH1F * a_hll[nbgc]; TH1F * a_hlow = new TH1F("a_hlow", "", a_nbin, a_led, a_hed); a_hlow->Sumw2();
    TH1F * a_hgg[nbgc]; TH1F * a_hsgg = new TH1F("a_hsgg", "", a_nbin, a_led, a_hed); a_hsgg->Sumw2();
    TH1F * a_hhh[nbgc]; TH1F * a_hhig = new TH1F("a_hhig", "", a_nbin, a_led, a_hed); a_hhig->Sumw2();

    sprintf(opcutC, "mva%s>%.3f", cate[p],opmva); mvaCut=opmva;
    if(p>=24) sprintf(opcutC, "mva%s>%.3f&&W_electron_pt>35", cate[p],opmva);
    char a_cutl[5000]; sprintf(a_cutl, "effwt*((ggdevt==%i)&&(Mass2j_PFCor>%.0f&&Mass2j_PFCor<%.0f)&&(fit_status==0)&&%s)",(int)(ctt%2+2),sblL,sblH,opcutC);
    char a_cutg[5000]; sprintf(a_cutg, "effwt*((ggdevt==%i)&&(Mass2j_PFCor>%.0f&&Mass2j_PFCor<%.0f)&&(fit_status==0)&&%s)",(int)(ctt%2+2),sigL,sigH,opcutC);
    char a_cuth[5000]; sprintf(a_cuth, "effwt*((ggdevt==%i)&&(Mass2j_PFCor>%.0f&&Mass2j_PFCor<%.0f)&&(fit_status==0)&&%s)",(int)(ctt%2+2),sbhL,sbhH,opcutC);

    cout << a_cutl << endl;
    cout << a_cutg << endl;
    cout << a_cuth << endl;

    for (int i = nbgc-1; i>=0; i--){
      if (i==9) continue; //skip QCD 
      double a_norfac =  DatLumi_mu *1.0/(XSecs[i][2]/ (XSecs[i][0]*XSecs[i][1]*XSecs[i][3]));
      if (ctt==0||ctt==1){  // muon channel
	sprintf(tpname,"a_hll[%i]",i);a_hll[i]=getHist(Mu_InFileNames[i],"WJet",f_plmassvar,a_cutl,tpname,a_nbin,a_led,a_hed,0);a_hll[i]->Sumw2();a_hll[i]->Scale(a_norfac);
	sprintf(tpname,"a_hgg[%i]",i);a_hgg[i]=getHist(Mu_InFileNames[i],"WJet",f_plmassvar,a_cutg,tpname,a_nbin,a_led,a_hed,0);a_hgg[i]->Sumw2();a_hgg[i]->Scale(a_norfac);
	sprintf(tpname,"a_hhh[%i]",i);a_hhh[i]=getHist(Mu_InFileNames[i],"WJet",f_plmassvar,a_cuth,tpname,a_nbin,a_led,a_hed,0);a_hhh[i]->Sumw2();a_hhh[i]->Scale(a_norfac);
      }
      if (ctt==2||ctt==3){  // electron channel
	sprintf(tpname,"a_hll[%i]",i);a_hll[i]=getHist(El_InFileNames[i],"WJet",f_plmassvar,a_cutl,tpname,a_nbin,a_led,a_hed,0);a_hll[i]->Sumw2();a_hll[i]->Scale(a_norfac);
	sprintf(tpname,"a_hgg[%i]",i);a_hgg[i]=getHist(El_InFileNames[i],"WJet",f_plmassvar,a_cutg,tpname,a_nbin,a_led,a_hed,0);a_hgg[i]->Sumw2();a_hgg[i]->Scale(a_norfac);
	sprintf(tpname,"a_hhh[%i]",i);a_hhh[i]=getHist(El_InFileNames[i],"WJet",f_plmassvar,a_cuth,tpname,a_nbin,a_led,a_hed,0);a_hhh[i]->Sumw2();a_hhh[i]->Scale(a_norfac);
      }
      if (a_type>=0){ if (a_type==i) { a_hlow->Add(a_hll[i],+1);a_hsgg->Add(a_hgg[i],+1);a_hhig->Add(a_hhh[i],+1);}
      } else  {                        a_hlow->Add(a_hll[i],+1);a_hsgg->Add(a_hgg[i],+1);a_hhig->Add(a_hhh[i],+1);}
    }
    a_hlow->Scale(1.0/a_hlow->Integral());    a_hsgg->Scale(1.0/a_hsgg->Integral());    a_hhig->Scale(1.0/a_hhig->Integral());    // Now normalize the shapes to unit area
    TH1F * a_shape[a_npt]; float a_alpha[a_npt], a_chi2[a_npt]; float minalpha = 99999;
    for (int i = 0; i<a_npt; i++){ // find the best alpha point
      double alp = i*1.0/(a_npt-1); double lf  = alp; double hf  = 1.0-alp;
      TH1F * hhl = (TH1F*) a_hlow->Clone("hhl");
      TH1F * hhg = (TH1F*) a_hsgg->Clone("hhg");
      TH1F * hhh = (TH1F*) a_hhig->Clone("hhh");
      hhl->Scale(lf);    hhh->Scale(hf);    hhl->Add(hhh,+1); 
      TH1F*htmps = (TH1F*) hhl->Clone("htmps"); //for (int j=1;j<htmps->GetNbinsX()+1;j++){htmps->SetBinError(j,0.0001);} // Use the signal region statistics only
      sprintf(tpname,"a_shape[%i]",i); 
      a_shape[i] = (TH1F*) hhl->Clone(tpname);
      a_alpha[i] = alp; 
      a_chi2[i]  = hhg->Chi2Test(htmps,"CHI2/NDF");
      if (a_chi2[i]<minalpha){minalpha=a_chi2[i];aba[p]=i;}
    }
    float dis_minlow = 99999, dis_minhig = 99999; float syschi2 = a_chi2[aba[p]] + 1.0/a_nbin;
    for (int i = 0; i<a_npt; i++){ // find the alpha points for systematics +- 1 unit chi2
      if (i<=aba[p] && fabs(a_chi2[i]-syschi2)<dis_minlow){dis_minlow = fabs(a_chi2[i]-syschi2); abl[p]=i; }
      if (i>=aba[p] && fabs(a_chi2[i]-syschi2)<dis_minhig){dis_minhig = fabs(a_chi2[i]-syschi2); abh[p]=i; }
    }

    // ------------- Best alpha plot
    TCanvas *MyCa_aph1  = new TCanvas("MyCa_aph1","MyCa_aph1",10,10,500, 500);  MyCa_aph1->Draw();
    a_hsgg->SetMaximum(1.8*a_hsgg->GetMaximum());
    //a_hsgg->SetMinimum(0);
    a_hsgg->SetXTitle("Invariant Mass of Mlvjj [GeV]");
    a_hsgg->SetYTitle("Shape [Unit Area]");
    a_hsgg->Draw("e");
    for (int i = 0; i<a_npt; i++){a_shape[i]->SetLineStyle(3); a_shape[i]->SetLineWidth(1); a_shape[i]->SetLineColor(43);
      if (i==0                  ){a_shape[i]->SetLineStyle(1); a_shape[i]->SetLineWidth(2); a_shape[i]->SetLineColor(kGreen);}
      if (i==a_npt-1            ){a_shape[i]->SetLineStyle(1); a_shape[i]->SetLineWidth(2); a_shape[i]->SetLineColor(kOrange);}
      a_shape[i]->Draw("samehist");
    }
    a_shape[abl[p]]->SetLineStyle(1); a_shape[abl[p]]->SetLineWidth(2); a_shape[abl[p]]->SetLineColor(65); a_shape[abl[p]]->Draw("samehist");    
    a_shape[abh[p]]->SetLineStyle(1); a_shape[abh[p]]->SetLineWidth(2); a_shape[abh[p]]->SetLineColor(65); a_shape[abh[p]]->Draw("samehist");    
    a_shape[aba[p]]->SetLineStyle(1); a_shape[aba[p]]->SetLineWidth(2); a_shape[aba[p]]->SetLineColor(kRed); a_shape[aba[p]]->Draw("samehist");    
    a_hsgg->Draw("samee");
    TLegend * Leg_aph1 = new TLegend(0.35,0.70,0.90,0.85);
    Leg_aph1->AddEntry(a_hsgg,           Form("Signal Region : Mjj [%.0f, %.0f]", sigL,sigH),                             "pl");
    Leg_aph1->AddEntry(a_shape[a_npt-1], Form("Low  Sideband : Mjj [%.0f, %.0f]", sblL,sblH),                              "l");
    Leg_aph1->AddEntry(a_shape[0],       Form("High Sideband : Mjj [%.0f, %.0f]", sbhL,sbhH),                              "l");
    Leg_aph1->AddEntry(a_shape[aba[p]],  Form("Best #alpha = %.2f @ #chi^{2}/NDF = %.3f", a_alpha[aba[p]],a_chi2[aba[p]]), "l");
    Leg_aph1->AddEntry(a_shape[abh[p]],  Form("MC Syst. range #alpha [%.2f, %.2f]",a_alpha[abl[p]],a_alpha[abh[p]]),       "l");
    Leg_aph1->SetFillStyle(0);
    Leg_aph1->SetBorderSize(0);
    Leg_aph1->Draw();
    TPaveText *pta_aph1=new TPaveText(0.468,0.40,0.90,0.68,"BRNDC");pta_aph1->SetBorderSize(0);pta_aph1->SetTextColor(60);pta_aph1->SetFillStyle(0);;pta_aph1->SetTextAlign(12);
    sprintf(tpname, "MC %s Alpha - %s",a_type>=0?PlainNames[a_type]:"All MC", cate[p]);TText *text_aph1f = pta_aph1->AddText(tpname); text_aph1f->SetTextColor(kRed);
    sprintf(tpname, "C&C Mass Window : %i -  %i", winl, winh                         );TText *text_aph1e = pta_aph1->AddText(tpname); text_aph1e->SetTextColor(43);
    sprintf(tpname, "Applied Optimized Cus : "                                       );TText *text_aph1d = pta_aph1->AddText(tpname); text_aph1d->SetTextColor(95);
    for (unsigned int i=0; i<allopcuts.size();i++) pta_aph1->AddText((allopcuts[i]).c_str());
    pta_aph1->Draw();
    TArrow * arrow811 = new TArrow(winl,0,winl, 0.15*a_hsgg->GetMaximum(), 0.02,"-"); arrow811->SetLineColor(35); arrow811->Draw("same");
    TArrow * arrow812 = new TArrow(winh,0,winh, 0.15*a_hsgg->GetMaximum(), 0.02,"-"); arrow812->SetLineColor(35); arrow812->Draw("same");
    a_hsgg->Draw("samee0");

    // ------------- For MC pseudo systematic
    TH1F * a_hsysh = new TH1F("a_hsysh", "", a_nbin, a_led, a_hed); a_hsysh->Sumw2();
    TH1F * a_hsysl = new TH1F("a_hsysl", "", a_nbin, a_led, a_hed); a_hsysl->Sumw2();
    TH1F * a_hsyse = new TH1F("a_hsyse", "", a_nbin, a_led, a_hed); a_hsyse->Sumw2();
    for (int i=1; i<a_shape[aba[p]]->GetNbinsX()+1; i++) {
      if (a_shape[aba[p]]->GetBinContent(i)!=0) {
	float errv = 
	  fabs(a_shape[abl[p]]->GetBinContent(i)-a_shape[aba[p]]->GetBinContent(i))>fabs(a_shape[abh[p]]->GetBinContent(i)-a_shape[aba[p]]->GetBinContent(i))?
	  fabs(a_shape[abl[p]]->GetBinContent(i)-a_shape[aba[p]]->GetBinContent(i)):fabs(a_shape[abh[p]]->GetBinContent(i)-a_shape[aba[p]]->GetBinContent(i));
	a_hsysh->SetBinContent(i, 1.0+(errv/a_shape[aba[p]]->GetBinContent(i)) );
	a_hsysl->SetBinContent(i, 1.0-(errv/a_shape[aba[p]]->GetBinContent(i)) );
	a_hsyse->SetBinContent(i, 1.0                                );
      } else {
	a_hsysh->SetBinContent(i, 1.0);
	a_hsysl->SetBinContent(i, 1.0);
	a_hsyse->SetBinContent(i, 1.0);    
      }
    }
    TCanvas *MyCa_aph2  = new TCanvas("MyCa_aph2","MyCa_aph2",10,10,500, 500);  MyCa_aph2->Draw();
    a_hsysh->SetMaximum(1.5);
    a_hsysh->SetMinimum(0.5);
    a_hsysh->SetLineColor(kRed);
    a_hsysh->Draw("hist");
    a_hsysh->GetXaxis()->SetTitle("Invariant Mass of Mlvjj [GeV]");
    a_hsysh->GetYaxis()->SetTitle("MC Pseudo Systematic Error");
    a_hsysl->SetLineColor(kBlue);
    a_hsysl->Draw("samehist");
    TPaveText *pta_aph2=new TPaveText(0.468,0.75,0.90,0.85,"BRNDC");pta_aph2->SetBorderSize(0);pta_aph2->SetLineWidth(1);pta_aph2->SetFillStyle(0);;pta_aph2->SetTextAlign(12);
    sprintf(tpname, "MC %s Alpha - %s",a_type>=0?PlainNames[a_type]:"All MC", cate[p]);TText *text_aph2f = pta_aph2->AddText(tpname); text_aph2f->SetTextColor(kRed);
    sprintf(tpname, "C&C Mass Window : %i -  %i", winl, winh                         );TText *text_aph2e = pta_aph2->AddText(tpname); text_aph2e->SetTextColor(43);
    pta_aph2->Draw();
    TArrow * ar911 = new TArrow(winl,a_hsysh->GetMinimum(),winl,a_hsysh->GetMaximum(),0.02,"-"); ar911->SetLineColor(35); ar911->Draw("same");
    TArrow * ar912 = new TArrow(winh,a_hsysh->GetMinimum(),winh,a_hsysh->GetMaximum(),0.02,"-"); ar912->SetLineColor(35); ar912->Draw("same");

    // ------------- Alpha Scan plot
    TCanvas *MyCa_aph3  = new TCanvas("MyCa_aph3","MyCa_aph3",10,10,500, 500);  MyCa_aph3->Draw();
    TGraph  *gr_aph3 = new TGraph(a_npt,a_alpha, a_chi2);
    gr_aph3->GetXaxis()->SetLimits(0,1);
    gr_aph3->SetMinimum(a_chi2[aba[p]]-1.3/a_nbin);
    gr_aph3->SetMaximum(a_chi2[aba[p]]+8.0/a_nbin);
    gr_aph3->SetFillColor(53);
    gr_aph3->SetMarkerSize(1.0);
    gr_aph3->SetMarkerColor(kBlue);
    gr_aph3->SetMarkerStyle(20);
    gr_aph3->SetLineColor(kBlue);
    gr_aph3->SetLineStyle(2);
    gr_aph3->SetLineWidth(3);
    gr_aph3->SetTitle("");
    gr_aph3->GetXaxis()->SetTitle("Alpha Scan");
    gr_aph3->GetYaxis()->SetTitle("Shape #chi^{2} / NDF");
    gr_aph3->GetYaxis()->CenterTitle();
    gr_aph3->GetXaxis()->CenterTitle();
    gr_aph3->Draw("ACP");
    TPaveText *pta101 = new TPaveText(0.15+0.8*0.01*abl[p], 0.13, 0.15+0.8*0.01*abh[p], 0.90,"BRNDC"); 
    pta101->SetBorderSize(0); pta101->SetLineWidth(1); pta101->SetFillStyle(1001); pta101->SetFillColor(kRed); pta101->SetTextAlign(22);
    //TText *text101 = pta101->AddText("             Shift Region for Systematic");text101->SetTextAngle(90);text101->SetTextAlign(22);text101->SetTextSize(0.04); 
    pta101->Draw();
    TArrow * ar101 = new TArrow(0, syschi2, 1.0, syschi2, 0.03,"-"); ar101->SetLineWidth(2);ar101->SetLineStyle(2);ar101->SetLineColor(kGreen);ar101->Draw("");
    TPaveText *pta_aph3=new TPaveText(0.01,0.90,0.99,0.99,"BRNDC");pta_aph3->SetBorderSize(0);pta_aph3->SetLineWidth(1);pta_aph3->SetFillStyle(0);;pta_aph3->SetTextAlign(22);
    sprintf(tpname, "Best #alpha=%.2f @ #chi^{2}/NDF=%.3f; #chi^{2} increase by 1 @ #alpha=%.2f, %.2f", a_alpha[aba[p]], a_chi2[aba[p]], a_alpha[abl[p]], a_alpha[abh[p]]);
    TText *text_aph3a = pta_aph3->AddText(tpname); text_aph3a->SetTextColor(kBlue);
    pta_aph3->Draw(); gr_aph3->Draw("samecp");MyCa_aph3->RedrawAxis();

    // ------------- MC shape compare with extrapolation shape
    TCanvas *MyCa_aph4  = new TCanvas("MyCa_aph4","MyCa_aph4",10,10,500, 500);  MyCa_aph4->Draw();
    TH1F * a_hratio = (TH1F *) a_hsgg->Clone("a_hratio");
    a_hratio->Divide(a_shape[aba[p]]);
    a_hratio->SetMarkerColor(kBlue);
    a_hratio->SetLineColor(kBlue);
    a_hratio->SetMaximum(2);
    a_hratio->SetMinimum(0);
    a_hratio->SetYTitle("Signal Shape / Best Shape");
    a_hratio->Draw("e0");
    TArrow * ar111 = new TArrow(a_led, 1, a_hed, 1, 0.03,"-");ar111->SetLineWidth(2);ar111->SetLineStyle(1);ar111->SetFillColor(kRed);ar111->SetLineColor(kRed);ar111->Draw();
    TArrow * ar112 = new TArrow(winl,a_hratio->GetMinimum(),winl,a_hratio->GetMaximum(),0.02,"-"); ar112->SetLineColor(35); ar112->Draw("same");
    TArrow * ar113 = new TArrow(winh,a_hratio->GetMinimum(),winh,a_hratio->GetMaximum(),0.02,"-"); ar113->SetLineColor(35); ar113->Draw("same");
    pta_aph3->Draw();a_hratio->Draw("samee0");

    // ------------- Test Data. For all sideband or after subtraction, Wjets only
    TH1F * a_hdall = new TH1F("a_hdall", "", a_nbin, a_led, a_hed);    TH1F * a_hctpll[nbgc];
    TH1F * a_hdagg = new TH1F("a_hdagg", "", a_nbin, a_led, a_hed);    TH1F * a_hctpgg[nbgc];
    TH1F * a_hdahh = new TH1F("a_hdahh", "", a_nbin, a_led, a_hed);    TH1F * a_hctphh[nbgc];
    if (ctt==0||ctt==1) {
      a_hdall=getHist(Mu_InFileNames[51],"WJet",f_plmassvar,a_cutl,"a_hdall",a_nbin,a_led,a_hed,0);a_hdall->Sumw2(); 
      a_hdagg=getHist(Mu_InFileNames[51],"WJet",f_plmassvar,a_cutg,"a_hdagg",a_nbin,a_led,a_hed,0);a_hdagg->Sumw2();
      a_hdahh=getHist(Mu_InFileNames[51],"WJet",f_plmassvar,a_cuth,"a_hdahh",a_nbin,a_led,a_hed,0);a_hdahh->Sumw2();
    }
    if (ctt==2||ctt==3) {
      a_hdall=getHist(El_InFileNames[51],"WJet",f_plmassvar,a_cutl,"a_hdall",a_nbin,a_led,a_hed,0);a_hdall->Sumw2(); 
      a_hdagg=getHist(El_InFileNames[51],"WJet",f_plmassvar,a_cutg,"a_hdagg",a_nbin,a_led,a_hed,0);a_hdagg->Sumw2();
      a_hdahh=getHist(El_InFileNames[51],"WJet",f_plmassvar,a_cuth,"a_hdahh",a_nbin,a_led,a_hed,0);a_hdahh->Sumw2();
    }
    if ( a_type>=0 ) { // alpha is getting for only one kind of MC, for data testing, subtract all other MC contribution.
      for (int i = nbgc-1; i>=0; i--){ // f_darefc is the overall factor making sum of all MC has same number of events as data in mjj [65, 95]
	if (i==9) continue; //skip QCD 
	float f_darefc = 1.0;
	sprintf(tpname,"a_hctpll[%i]",i); a_hctpll[i] = (TH1F*) a_hll[i]->Clone(tpname); a_hctpll[i]->Scale(f_darefc);
	sprintf(tpname,"a_hctpgg[%i]",i); a_hctpgg[i] = (TH1F*) a_hgg[i]->Clone(tpname); a_hctpgg[i]->Scale(f_darefc);
	sprintf(tpname,"a_hctphh[%i]",i); a_hctphh[i] = (TH1F*) a_hhh[i]->Clone(tpname); a_hctphh[i]->Scale(f_darefc);
	if (i!=a_type) {
	  a_hdall->Add(a_hctpll[i],-1);
	  a_hdagg->Add(a_hctpgg[i],-1);
	  a_hdahh->Add(a_hctphh[i],-1);
	}
      }
    }
    // Now after subtraction, normalize the data shape to unit area and build shape using MC alpha
    a_hdall->Scale(1.0/a_hdall->Integral());
    a_hdagg->Scale(1.0/a_hdagg->Integral());
    a_hdahh->Scale(1.0/a_hdahh->Integral());  
    // best shape in data and systematic region
    TH1F * be_da=(TH1F*)a_hdall->Clone("be_da");be_da->Scale(a_alpha[aba[p]]);TH1F*be_tp=(TH1F*)a_hdahh->Clone("be_tp");be_tp->Scale((1.0-a_alpha[aba[p]]));be_da->Add(be_tp,+1);
    TH1F * sl_da=(TH1F*)a_hdall->Clone("sl_da");sl_da->Scale(a_alpha[abl[p]]);TH1F*sl_tp=(TH1F*)a_hdahh->Clone("sl_tp");sl_tp->Scale((1.0-a_alpha[abl[p]]));sl_da->Add(sl_tp,+1);
    TH1F * sh_da=(TH1F*)a_hdall->Clone("sh_da");sh_da->Scale(a_alpha[abh[p]]);TH1F*sh_tp=(TH1F*)a_hdahh->Clone("sh_tp");sh_tp->Scale((1.0-a_alpha[abh[p]]));sh_da->Add(sh_tp,+1);
    
    TCanvas *MyCa_aph5  = new TCanvas("MyCa_aph5","MyCa_aph5",10,10,500, 500);  MyCa_aph5->Draw();
    be_da->SetMaximum(1.8*be_da->GetMaximum());
    //be_da->SetMinimum(0);
    be_da->SetLineWidth(2);
    be_da->SetLineStyle(1);
    be_da->SetLineColor(kRed);
    be_da->SetXTitle("Invariant Mass of Mlvjj [GeV]");
    be_da->SetYTitle("Shape [Unit Area]");
    be_da->Draw("hist");
    sl_da->SetLineColor(65); sl_da->SetLineWidth(2); sl_da->SetLineStyle(1);sl_da->Draw("samehist");
    sh_da->SetLineColor(65); sh_da->SetLineWidth(2); sh_da->SetLineStyle(1);sh_da->Draw("samehist");
    a_hdall->SetLineColor(kOrange); a_hdall-> SetLineWidth(2);            a_hdall->Draw("samehist");
    a_hdahh->SetLineColor(kGreen);  a_hdahh-> SetLineWidth(2);            a_hdahh->Draw("samehist");
    be_da->Draw("samehist");
    a_hdagg->SetMarkerColor(kBlack);   a_hdagg->Draw("samee");
    TLegend * Leg_aph5 = new TLegend(0.40,0.70,0.90,0.85);
    Leg_aph5->AddEntry(a_hdagg,       Form("Signal Region : Mjj [%.0f, %.0f]", sigL,sigH),          "pl");
    Leg_aph5->AddEntry(a_hdall,       Form("Low  Sideband : Mjj [%.0f, %.0f]", sblL,sblH),           "l");
    Leg_aph5->AddEntry(a_hdahh,       Form("High Sideband : Mjj [%.0f, %.0f]", sbhL,sbhH),           "l");
    Leg_aph5->AddEntry(be_da,         Form("Best #alpha = %.2f Shape in Data", a_alpha[aba[p]]),     "l");
    Leg_aph5->AddEntry(sl_da,         Form("MC Syst. #alpha [%.2f, %.2f]",a_alpha[abl[p]],a_alpha[abh[p]]),       "l");
    Leg_aph5->SetFillStyle(0);
    Leg_aph5->SetBorderSize(0);
    Leg_aph5->Draw();
    cmspre();
    TPaveText *pta_aph5=new TPaveText(0.39,0.40,0.90,0.70,"BRNDC");pta_aph5->SetBorderSize(0);pta_aph5->SetTextColor(55);pta_aph5->SetFillStyle(0);;pta_aph5->SetTextAlign(12);
    sprintf(tpname, "Test alpha in Data : %s", cate[p]);                                         TText *text_aph5a = pta_aph5->AddText(tpname); text_aph5a->SetTextColor(65);
    sprintf(tpname, "Best MC #alpha=%.2f @ #chi^{2}/NDF=%.3f", a_alpha[aba[p]], a_chi2[aba[p]]); TText *text_aph5c = pta_aph5->AddText(tpname); text_aph5c->SetTextColor(kBlue);
    sprintf(tpname, "Alpha is from : %s",a_type>=0?PlainNames[a_type]:"All MC");                 TText *text_aph5d = pta_aph5->AddText(tpname); text_aph5d->SetTextColor(kRed);
    sprintf(tpname, "C&C Mass Window : %i -  %i", winl,winh);                                    TText *text_aph5e = pta_aph5->AddText(tpname); text_aph5e->SetTextColor(43);
    sprintf(tpname, "Applied Optimized Cus : " );                                                TText *text_aph5f = pta_aph5->AddText(tpname); text_aph5f->SetTextColor(95);
    for (unsigned int i=0; i<allopcuts.size();i++) pta_aph5->AddText((allopcuts[i]).c_str());
    pta_aph5->Draw();
    TArrow * ar121 = new TArrow(winl,0,winl, 0.15*be_da->GetMaximum(), 0.02,"-"); ar121->SetLineColor(35); ar121->Draw("same");
    TArrow * ar122 = new TArrow(winh,0,winh, 0.15*be_da->GetMaximum(), 0.02,"-"); ar122->SetLineColor(35); ar122->Draw("same");

    // Plot summary
    TCanvas *MyCa_aph6  = new TCanvas("MyCa_aph6","MyCa_aph6",10,10,500, 500);  MyCa_aph6->Draw();
    TPaveText *pta_aph6=new TPaveText(0.01,0.01,0.99,0.99,"BRNDC");pta_aph6->SetBorderSize(0);pta_aph6->SetTextColor(55);pta_aph6->SetFillStyle(0);;pta_aph6->SetTextAlign(12);
    sprintf(tpname, "C&C Summary %s", cate[p]         );                                         TText *text_aph6a = pta_aph6->AddText(tpname); text_aph6a->SetTextColor(65);
    sprintf(tpname, "Best MC #alpha=%.2f @ #chi^{2}/NDF=%.3f : Range [%.2f, %.2f]", a_alpha[aba[p]], a_chi2[aba[p]],a_alpha[abl[p]],a_alpha[abh[p]] ); TText *text_aph6c = pta_aph6->AddText(tpname); text_aph6c->SetTextColor(kBlue);
    sprintf(tpname, "MC Syst. range #alpha [%.2f, %.2f]",      a_alpha[abl[p]],a_alpha[abh[p]]); TText *text_aph6g = pta_aph5->AddText(tpname); text_aph6g->SetTextColor(kBlue);
    sprintf(tpname, "Alpha is from : %s",a_type>=0?PlainNames[a_type]:"All MC");                 TText *text_aph6d = pta_aph6->AddText(tpname); text_aph6d->SetTextColor(kRed);
    sprintf(tpname, "C&C Mass Window : %i -  %i", winl,winh);                                    TText *text_aph6e = pta_aph6->AddText(tpname); text_aph6e->SetTextColor(43);
    sprintf(tpname, "After pre-selection expected: Sig=%.1f  Bkg=%.1f", presS, presB );          TText *text_aph6f = pta_aph6->AddText(tpname); text_aph6f->SetTextColor(kRed);
    sprintf(tpname, "Applied Optimized Cus : " );                                                TText *text_aph6h = pta_aph6->AddText(tpname); text_aph6h->SetTextColor(95);
    for (unsigned int i=0; i<allopcuts.size();i++) pta_aph6->AddText((allopcuts[i]).c_str());
    pta_aph6->Draw();
			   
    //saveCanvas(MyCa_aph1, Form("./figures/CCOptimize/cc-%s-alpha-%i-%s",cate[p],sqnc,"mcget")    );
    //saveCanvas(MyCa_aph2, Form("./figures/CCOptimize/cc-%s-alpha-%i-%s",cate[p],sqnc,"mcsys")    );
    //saveCanvas(MyCa_aph3, Form("./figures/CCOptimize/cc-%s-alpha-%i-%s",cate[p],sqnc,"mcsca")    );
    //saveCanvas(MyCa_aph4, Form("./figures/CCOptimize/cc-%s-alpha-%i-%s",cate[p],sqnc,"mctet")    );
    //saveCanvas(MyCa_aph5, Form("./figures/CCOptimize/cc-%s-alpha-%i-%s",cate[p],sqnc,"datet")    );
    //saveCanvas(MyCa_aph6, Form("./figures/CCOptimize/cc-%s-optmz-%i-%s",cate[p],sqnc,"summa")    );
    // Output File
    fprintf(textfile,"( %i, \"%s\", %.3f, %.1f, %.1f, %i, %.3f, %.3f, %.3f, (%.1f,%.1f), (%.1f,%.1f) ),\n", atoi(&cate[p][2]),ppvar, mvaCut, a_led, a_hed, a_nbin, a_alpha[aba[p]], a_alpha[abl[p]],a_alpha[abh[p]], sblL, sblH, sbhL, sbhH );
    fclose(textfile);
  }
  /////////
}


// ====================================================================================
// Self Function
// ====================================================================================
void MakePlot(int myflag = -1, int myflagb = 0, float myopmva = 0.5)
{
  cout << "Hello Fan : myflag=" << myflag << myflagb << myopmva << endl;
  ////
  
  //OptimizeMVAOutPut(0    ,0, 0.30);
  //OptimizeMVAOutPut(1    ,0, 0.60);
  //OptimizeMVAOutPut(2    ,0, 0.70);
  //OptimizeMVAOutPut(3    ,0, 0.60);
  //OptimizeMVAOutPut(4    ,0, 0.70);
  //OptimizeMVAOutPut(5    ,0, 0.60);
  //OptimizeMVAOutPut(6    ,0, 0.60);
  //OptimizeMVAOutPut(7    ,0, 0.60);
  //OptimizeMVAOutPut(8    ,0, 0.60);
  //OptimizeMVAOutPut(9    ,0, 0.60);
  //OptimizeMVAOutPut(10   ,0, 0.60);
  //OptimizeMVAOutPut(11   ,0, 0.80);

  //OptimizeMVAOutPut(0 +12,0, 0.30);
  //OptimizeMVAOutPut(1 +12,0, 0.30);
  //OptimizeMVAOutPut(2 +12,0, 0.30);
  //OptimizeMVAOutPut(3 +12,0, 0.30);
  //OptimizeMVAOutPut(4 +12,0, 0.40);
  //OptimizeMVAOutPut(5 +12,0, 0.50);
  //OptimizeMVAOutPut(6 +12,0, 0.50);
  //OptimizeMVAOutPut(7 +12,0, 0.50);
  //OptimizeMVAOutPut(8 +12,0, 0.50);
  //OptimizeMVAOutPut(9 +12,0, 0.50);
  //OptimizeMVAOutPut(10+12,0, 0.60);
  //OptimizeMVAOutPut(11+12,0, 0.80);

  ////
  //OptimizeMVAOutPut(0 +24,0, 0.30);
  //OptimizeMVAOutPut(1 +24,0, 0.60);
  //OptimizeMVAOutPut(2 +24,0, 0.60);
  //OptimizeMVAOutPut(3 +24,0, 0.60);
  //OptimizeMVAOutPut(4 +24,0, 0.60);
  //OptimizeMVAOutPut(5 +24,0, 0.60);
  //OptimizeMVAOutPut(6 +24,0, 0.60);
  //OptimizeMVAOutPut(7 +24,0, 0.50);
  //OptimizeMVAOutPut(8 +24,0, 0.50);
  //OptimizeMVAOutPut(9 +24,0, 0.50);
  //OptimizeMVAOutPut(10+24,0, 0.70);
  //OptimizeMVAOutPut(11+24,0, 0.80);


  //OptimizeMVAOutPut(0 +36,0, 0.30);
  //OptimizeMVAOutPut(1 +36,0, 0.30);
  //OptimizeMVAOutPut(2 +36,0, 0.30);
  //OptimizeMVAOutPut(3 +36,0, 0.30);
  //OptimizeMVAOutPut(4 +36,0, 0.40);
  //OptimizeMVAOutPut(5 +36,0, 0.40);
  //OptimizeMVAOutPut(6 +36,0, 0.40);
  //OptimizeMVAOutPut(7 +36,0, 0.40);
  //OptimizeMVAOutPut(8 +36,0, 0.40);
  //OptimizeMVAOutPut(9 +36,0, 0.60);
  //OptimizeMVAOutPut(10+36,0, 0.70);
  //OptimizeMVAOutPut(11+36,0, 0.70);

}
