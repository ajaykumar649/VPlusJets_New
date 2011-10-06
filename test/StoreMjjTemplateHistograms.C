

const double MINRange = 40.0;
const double MAXRange = 300.0;
const int BINWIDTH = 10;
const int NBINSFORPDF = (int)((MAXRange-MINRange)/BINWIDTH);
const char* selection = "( (evtNJ==2 || evtNJ==3)"
  " && (JetPFCor_Pt[0]>40.)"
  " && (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2)"
  " && (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.)"
  // " && (JetPFCor_bDiscriminator[0]<1.74)"
  // " && (JetPFCor_bDiscriminator[1]<1.74)"
  // " && (JetPFCor_QGLikelihood[0]*JetPFCor_QGLikelihood[1]>0.1) )"
  " && (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) )";

const TString plots_dir = "/uscms/home/ilyao/nobackup/KinematicFitterS11/2fbFit/FitResults_3JcutsV0";
const TString data_dir = "/uscms_data/d2/kalanand/WjjTrees";
const TString QCDTemplate_dir = "/uscms/home/ilyao/nobackup/AnalysisLevelNtuples/VPlusJets/428MC";


void StoreMjjTemplateHistograms() {
  grabDataSubtractedHistograms();

   getTemplate(data_dir + "/ReducedTree/RD_mu_WW_CMSSW428.root", "WW_muon");
   getTemplate(data_dir + "/ReducedTree/RD_mu_WZ_CMSSW428.root", "WZ_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_WW_CMSSW428.root", "WW_electron");
   getTemplate(data_dir + "/ReducedTree/RD_el_WZ_CMSSW428.root", "WZ_electron");


   getTemplate(data_dir + "/ReducedTree/RD_mu_WpJ_CMSSW428.root", "Wjets_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_WpJ_CMSSW428.root", "Wjets_electron");


   getTemplate(data_dir + "/ReducedTree/RD_mu_TTbar_CMSSW428.root", "ttbar_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_TTbar_CMSSW428.root", "ttbar_electron");

   getTemplate(data_dir + "/ReducedTree/RD_mu_STopS_CMSSW428.root", "singletop_s_channel_muon");
   getTemplate(data_dir + "/ReducedTree/RD_mu_STopT_CMSSW428.root", "singletop_t_channel_muon");
   getTemplate(data_dir + "/ReducedTree/RD_mu_STopTW_CMSSW428.root","singletop_tw_channel_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_STopS_CMSSW428.root", "singletop_s_channel_electron");
   getTemplate(data_dir + "/ReducedTree/RD_el_STopT_CMSSW428.root", "singletop_t_channel_electron");
   getTemplate(data_dir + "/ReducedTree/RD_el_STopTW_CMSSW428.root","singletop_tw_channel_electron");

   getTemplate(QCDTemplate_dir + "/RD_mu_QCDMu_CMSSW428_MET20Iso03.root", "QCD_MuEnrichedPt_15");
   getTemplate(QCDTemplate_dir + "/RD_el_QCDEl_Pt30to80_CMSSW428_MET20Iso03.root", "QCD_EMEnriched_Pt_30to80");
   getTemplate(QCDTemplate_dir + "/RD_el_QCDEl_Pt80to170_CMSSW428_MET20Iso03.root", "QCD_EMEnriched_Pt_80to170");
   getTemplate(QCDTemplate_dir + "/RD_el_QCDEl_BCtoE30to80_CMSSW428_MET20Iso03.root", "QCD_BCtoE_Pt_30to80");
   getTemplate(QCDTemplate_dir + "/RD_el_QCDEl_BCtoE80to170_CMSSW428_MET20Iso03.root", "QCD_BCtoE_Pt_80to170");

   getTemplate(data_dir + "/ReducedTree/RD_mu_ZpJ_CMSSW428.root", "ZJets_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_ZpJ_CMSSW428.root", "ZJets_electron");

   getTemplate(data_dir + "/ReducedTree/RD_mu_Ztautau_CMSSW428.root", "ZJets_muon");
   getTemplate(data_dir + "/ReducedTree/RD_el_Ztautau_CMSSW428.root", "ZJets_electron");

   // New Physics models normalized to 1/fb
   getNPTemplate('Z', data_dir + "/ReducedTree/RD_Zprime_Wenujj_50k.root", "Zprime_electron");
   getNPTemplate('Z', data_dir + "/ReducedTree/RD_Zprime_Wmunujj_50k.root", "Zprime_muon");
   getNPTemplate('T', data_dir + "/ReducedTree/RD_Technicolor_CorrectConfig_Wenujj_50k.root", "Technicolor_electron");
   getNPTemplate('T', data_dir + "/ReducedTree/RD_Technicolor_CorrectConfig_Wmunujj_50k.root", "Technicolor_muon");
   getNPTemplate('H', data_dir + "/ReducedTree/RD_WH150qq_WenuJJ_50k.root", "WH_electron");
   getNPTemplate('H', data_dir + "/ReducedTree/RD_WH150qq_WmunuJJ_50k.root", "WH_muon");

}




void grabDataSubtractedHistograms() {
  TFile f("Histograms_Mjj_data_and_template.root", "update");


  TFile* fitFile = new TFile( plots_dir + "/mJJ-combined-fit.root", "read");
  TCanvas* fitCan = (TCanvas*) fitFile->Get( "mJJ-combined-fit" );
  RooHist* data = (RooHist*) fitCan->FindObject( "h_data" );
  RooCurve* fit_total = (RooCurve*) fitCan->FindObject( "h_total" );
  RooCurve* fit_diboson = (RooCurve*) fitCan->FindObject( "h_diboson" );
  RooCurve* fit_Wjets = (RooCurve*) fitCan->FindObject( "h_Wjets" );

  RooCurve* fit_Top = (RooCurve*) fitCan->FindObject( "h_Top" );
  RooCurve* fit_QCD = (RooCurve*) fitCan->FindObject( "h_QCD" );
  RooCurve* fit_Zjets = (RooCurve*) fitCan->FindObject( "h_Zjets" );
  RooCurve* fit_Ztautau = (RooCurve*) fitCan->FindObject( "h_Ztautau" );

  TFile* subtrFile = new TFile( plots_dir + "/mJJ-combined-fit-subtracted.root", "read");
  TCanvas* subtrCan = (TCanvas*) subtrFile->Get( "mJJ-combined-fit-subtracted" );
  RooHist* subtrHist = (RooHist*) subtrCan->FindObject( "resid_h_data_h_Background" );
  RooCurve* Diboson = (RooCurve*) subtrCan->FindObject( "h_diboson" );



//   TFile* fitFile1 = new TFile( plots_dir + "/mJJ-mu-fit.root", "read");
//   TCanvas* fitCan1 = (TCanvas*) fitFile1->Get( "mJJ-mu-fit" );
//   RooHist* data1 = (RooHist*) fitCan1->FindObject( "h_data" );
//   RooCurve* fit1 = (RooCurve*) fitCan1->FindObject( "h_total" );

//   TFile* subtrFile1 = new TFile( plots_dir + "/mJJ-mu-fit-subtracted.root", "read");
//   TCanvas* subtrCan1 = (TCanvas*) subtrFile1->Get( "mJJ-mu-fit-subtracted" );
//   RooHist* subtrHist1 = (RooHist*) subtrCan1->FindObject( "resid_h_data_h_Background" );
//   RooCurve* Diboson1 = (RooCurve*) subtrCan1->FindObject( "h_diboson" );



//   TFile* fitFile2 = new TFile( plots_dir + "/mJJ-ele-fit.root", "read");
//   TCanvas* fitCan2 = (TCanvas*) fitFile2->Get( "mJJ-ele-fit" );
//   RooHist* data2 = (RooHist*) fitCan2->FindObject( "h_data" );
//   RooCurve* fit2 = (RooCurve*) fitCan2->FindObject( "h_total" );

//   TFile* subtrFile2 = new TFile( plots_dir + "/mJJ-ele-fit-subtracted.root", "read");
//   TCanvas* subtrCan2 = (TCanvas*) subtrFile2->Get( "mJJ-ele-fit-subtracted" );
//   RooHist* subtrHist2 = (RooHist*) subtrCan2->FindObject( "resid_h_data_h_Background" );
//   RooCurve* Diboson2 = (RooCurve*) subtrCan2->FindObject( "h_diboson" );


  // --------- Now save everything in the output file -------
 f.cd();
 data->Write("hist_data");
 fit_total->Write("fit_total");
 fit_diboson->Write("fit_diboson");
 fit_Wjets->Write("fit_Wjets");
 fit_Top->Write("fit_Top");
 fit_QCD->Write("fit_QCD");
 fit_Zjets->Write("fit_Zjets");
 fit_Ztautau->Write("fit_Ztautau");

 subtrHist->Write("hist_data_subtracted");
 Diboson->Write("curve_diboson");

//  data1->Write("hist_data_muon");
//  fit1->Write("curve_fit_muon");
//  subtrHist1->Write("hist_data_subtracted_muon");
//  Diboson1->Write("curve_diboson_muon");

//  data2->Write("hist_data_electron");
//  fit2->Write("curve_fit_electron");
//  subtrHist2->Write("hist_data_subtracted_electron");
//  Diboson2->Write("curve_diboson_electron");

 f.Close();
}



void getTemplate(char* inputDataFileName, char* outHistogramName, char* inputTreeName="WJet") {

   TFile f("Histograms_Mjj_data_and_template.root", "update");
   TFile* mhfile = new TFile( inputDataFileName, "read");
   cout << "File Name : " << inputDataFileName << endl;
   TTree* treeTemp = (TTree*) mhfile->Get(inputTreeName);
   cout << "Tree Entries = " << treeTemp->GetEntries() << endl;

   TH1* th1H = new TH1D(outHistogramName, outHistogramName, NBINSFORPDF, MINRange, MAXRange);
   char* treecuts = selection; 
   if(!(inputTreeName=="WJet"))treecuts = "";

   treeTemp->Draw( TString("Mass2j_PFCor>>")+TString(outHistogramName), treecuts,"goff");
   th1H->Scale( 1. / th1H->Integral() );

   f.cd();
   th1H->Write();
   f.Close();
}

/*
  The model parameters is as follows:
  Z : Z prime
  T : Technicolor
  H : WH/ZH higgs
 */
void getNPTemplate(char model, char* inputDataFileName, 
		   char* outHistogramName, char* inputTreeName="WJet") {

  float sampleLumi = 1.0; // fb^-1
  switch (model) {
  case 'Z':
  case 'z':
    sampleLumi = 29.069;
  case 'T':
  case 't':
    sampleLumi = 31.519;
  case 'H':
  case 'h':
    sampleLumi = 3448.276;
  }
  TFile f("Histograms_Mjj_data_and_template.root", "update");
  TFile* mhfile = new TFile( inputDataFileName, "read");
  cout << "File Name : " << inputDataFileName << endl;
  TTree* treeTemp = (TTree*) mhfile->Get(inputTreeName);
  if (!treeTemp) {
    cout << "Tree not found in file " << inputDataFileName << ".  Skipping."
	 << endl;
    return;
  }
  cout << "Tree Entries = " << treeTemp->GetEntries() << endl;
  
  TH1* th1H = new TH1D(outHistogramName, outHistogramName, 
		       NBINSFORPDF, MINRange, MAXRange);
  char* treecuts = selection; 
  if(!(inputTreeName=="WJet"))treecuts = "";

  treeTemp->Draw( TString("Mass2j_PFCor>>")+TString(outHistogramName), treecuts,"goff");
  th1H->Scale( 1. / sampleLumi );

  f.cd();
  th1H->Write();
  f.Close();
}
