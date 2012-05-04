

void comparePythiaMCFMWW() {

 TFile* pythia_file = new TFile("~/nobackup/WjjTrees/Full2011DataFall11MC/ReducedTree_PAT/RD_mu_WW_CMSSW428.root", "READ");

 TTree* pythia_tree = (TTree*) pythia_file->Get("WJet");


const string var3 = "sqrt((px3+px4)*(px3+px4)+(py3+py4)*(py3+py4))";
const string histName3 = "LeptonicWpT";
const string title3 = "Leptonic W P_{T} [GeV]";

const string var4 = "sqrt((px5+px6)*(px5+px6)+(py5+py6)*(py5+py6))";
const string histName4 = "HadronicWpT";
const string title4 = "Hadronic W P_{T} [GeV]";


 TFile* mcfm_file = TFile::Open( "dcap://cmsdca1.fnal.gov:24140/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lnujj/MCFM_WW_aTGC/Grid2D_LambdaZ_KappaGamma/LambdaZ_0.0_dKgamma_0.0.root", "read");

 TTree* mcfm_tree = (TTree*)mcfm_file ->Get("h300");

 Int_t bins = 20; 
 Float_t dm_min = 100.; 
 Float_t dm_max = 500.;
 TH1D* pythia_hist_LeptWpt = new TH1D("pythia_hist_LeptWpt", "", bins, dm_min, dm_max);
 pythia_hist_LeptWpt->Sumw2();
 pythia_hist_LeptWpt->SetLineWidth(2);
 pythia_tree->Draw( "W_muon_pt>>pythia_hist_LeptWpt", "ggdevt==2||ggdevt==3", "goff");
 pythia_hist_LeptWpt->Scale(1./pythia_hist_LeptWpt->Integral() );

 TH1D* pythia_hist_dummy = pythia_hist_LeptWpt->Clone("pythia_hist_dummy");
 pythia_hist_dummy->Smooth(5);
pythia_hist_dummy ->SetLineColor(2);
pythia_hist_dummy ->SetMarkerColor(2);
pythia_hist_dummy->SetLineWidth(2);

 TH1D* mcfm_hist_LeptWpt = new TH1D("mcfm_hist_LeptWpt", "", bins, dm_min, dm_max);
 mcfm_hist_LeptWpt->Sumw2();
 mcfm_hist_LeptWpt->SetLineColor(2);
 mcfm_hist_LeptWpt->SetMarkerColor(2);
 mcfm_hist_LeptWpt->SetLineWidth(2);
 mcfm_tree->Draw( TString(var3)+TString(">>mcfm_hist_LeptWpt"), "", "goff");
 mcfm_hist_LeptWpt->Scale(1./mcfm_hist_LeptWpt->Integral() );
 //pythia_hist_dummy->Divide(mcfm_hist_LeptWpt);

 gStyle->SetOptStat(0);
 gStyle->SetPadTopMargin(0.05);
 gStyle->SetPadBottomMargin(0.13);
 gStyle->SetPadLeftMargin(0.16);
 //gStyle->SetPadRightMargin(0.28);

 TCanvas* c1 = new TCanvas("can1_LeptWpt", "LeptWpt", 500, 500);
 pythia_hist_LeptWpt->Draw("pe");
 //mcfm_hist_LeptWpt->Draw("same chist");
 pythia_hist_dummy->Draw("same chist");
 Double_t x1=0.7, y1=0.49;
  Double_t  x2=x1+0.2, y2=y1+0.42;
 TLegend* l = new TLegend( x1, y1, x2, y2 );
 l->AddEntry(pythia_hist_LeptWpt, "Pythia", "lp");
 l->AddEntry(mcfm_hist_LeptWpt, "MCFM", "lp");
 l->SetFillColor(0);
 l->Draw();
 c1->Print("Pythia_mcfm_LeptWpt.png"); 
  
}
