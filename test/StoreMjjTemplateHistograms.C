

const double MINRange = 40.0;
const double MAXRange = 300.0;
const int BINWIDTH = 10;
const int NBINSFORPDF = (int)((MAXRange-MINRange)/BINWIDTH);
const char* selection = "gdevtt";


void StoreMjjTemplateHistograms() {
  grabDataSubtractedHistograms();

   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_WWtoAnything.root", "WW_muon");
   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_WZtoAnything.root", "WZ_muon");
   getTemplate("data/ReducedTree/RD_WenuJets_CMSSW415-Spring11MC_WWtoAnything.root", "WW_electron");
   getTemplate("data/ReducedTree/RD_WenuJets_CMSSW415-Spring11MC_WZtoAnything.root", "WZ_electron");


   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW423-Summer11MC_WJets.root", "Wjets_muon");
   getTemplate("data/ReducedTree/RD_WenuJets_CMSSW423-Summer11MC_WJets.root", "Wjets_electron");


   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW423-Summer11MC_TTToLNu2Q2B.root", "ttbar_muon");
   getTemplate("data/ReducedTree/RD_WenuJets_CMSSW423-Summer11MC_TTToLNu2Q2B.root", "ttbar_electron");

   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-s-channel.root", "singletop_s_channel_muon");
   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-t-channel.root", "singletop_t_channel_muon");
   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW415-Spring11MC_SingleTopLNu2Q2B-tw-channel.root","singletop_tw_channel_muon");

   getTemplate("data/W_mt_MET20Iso03_WmunuJets_QCD_Pt-20_MuEnrichedPt-15.root", "QCD_MuEnrichedPt_15", "OutTree");
   getTemplate("data/W_mt_MET20Iso03_WenuJets_QCD_Pt-30to80_EMEnriched.root", "QCD_EMEnriched_Pt_30to80", "OutTree");
   getTemplate("data/W_mt_MET20Iso03_WenuJets_QCD_Pt-80to170_EMEnriched.root", "QCD_EMEnriched_Pt_80to170", "OutTree");
   getTemplate("data/W_mt_MET20Iso03_WenuJets_QCD_Pt-30to80_BCtoE.root", "QCD_BCtoE_Pt_30to80", "OutTree");
   getTemplate("data/W_mt_MET20Iso03_WenuJets_QCD_Pt-80to170_BCtoE.root", "QCD_BCtoE_Pt_80to170", "OutTree");


   getTemplate("data/ReducedTree/RD_WmunuJets_CMSSW423-Summer11MC_DYJetsToLL_MadGraph.root", "ZJets_muon");
   getTemplate("data/ReducedTree/RD_WenuJets_CMSSW423-Summer11MC_DYJetsToLL_MadGraph.root", "ZJets_electron");
}




void grabDataSubtractedHistograms() {
  TFile f("Histograms_Mjj_data_and_template.root", "update");


  TFile* fitFile = new TFile( "plots_Round3/mJJ-combined-fit.root", "read");
  TCanvas* fitCan = (TCanvas*) fitFile->Get( "mJJ-combined-fit" );
  RooHist* data = (RooHist*) fitCan->FindObject( "h_data" );
  RooCurve* fit = (RooCurve*) fitCan->FindObject( "h_total" );

  TFile* subtrFile = new TFile( "plots_Round3/mJJ-combined-fit-subtracted.root", "read");
  TCanvas* subtrCan = (TCanvas*) subtrFile->Get( "mJJ-combined-fit-subtracted" );
  RooHist* subtrHist = (RooHist*) subtrCan->FindObject( "resid_h_data_h_Background" );
  RooCurve* Diboson = (RooCurve*) subtrCan->FindObject( "h_diboson" );



  TFile* fitFile1 = new TFile( "plots_Round3/mJJ-mu-fit.root", "read");
  TCanvas* fitCan1 = (TCanvas*) fitFile1->Get( "mJJ-mu-fit" );
  RooHist* data1 = (RooHist*) fitCan1->FindObject( "h_data" );
  RooCurve* fit1 = (RooCurve*) fitCan1->FindObject( "h_total" );

  TFile* subtrFile1 = new TFile( "plots_Round3/mJJ-mu-fit-subtracted.root", "read");
  TCanvas* subtrCan1 = (TCanvas*) subtrFile1->Get( "mJJ-mu-fit-subtracted" );
  RooHist* subtrHist1 = (RooHist*) subtrCan1->FindObject( "resid_h_data_h_Background" );
  RooCurve* Diboson1 = (RooCurve*) subtrCan1->FindObject( "h_diboson" );



  TFile* fitFile2 = new TFile( "plots_Round3/mJJ-ele-fit.root", "read");
  TCanvas* fitCan2 = (TCanvas*) fitFile2->Get( "mJJ-ele-fit" );
  RooHist* data2 = (RooHist*) fitCan2->FindObject( "h_data" );
  RooCurve* fit2 = (RooCurve*) fitCan2->FindObject( "h_total" );

  TFile* subtrFile2 = new TFile( "plots_Round3/mJJ-ele-fit-subtracted.root", "read");
  TCanvas* subtrCan2 = (TCanvas*) subtrFile2->Get( "mJJ-ele-fit-subtracted" );
  RooHist* subtrHist2 = (RooHist*) subtrCan2->FindObject( "resid_h_data_h_Background" );
  RooCurve* Diboson2 = (RooCurve*) subtrCan2->FindObject( "h_diboson" );


  // --------- Now save everything in the output file -------
 f.cd();
 data->Write("hist_data");
 fit->Write("curve_fit");
 subtrHist->Write("hist_data_subtracted");
 Diboson->Write("curve_diboson");

 data1->Write("hist_data_muon");
 fit1->Write("curve_fit_muon");
 subtrHist1->Write("hist_data_subtracted_muon");
 Diboson1->Write("curve_diboson_muon");

 data2->Write("hist_data_electron");
 fit2->Write("curve_fit_electron");
 subtrHist2->Write("hist_data_subtracted_electron");
 Diboson2->Write("curve_diboson_electron");

 f.Close();
}



void getTemplate(char* inputDataFileName, char* outHistogramName, char* inputTreeName="WJet") {

   TFile f("Histograms_Mjj_data_and_template.root", "update");
   TFile* mhfile = new TFile( inputDataFileName, "read");
   TTree* treeTemp = (TTree*) mhfile->Get(inputTreeName);

   TH1* th1H = new TH1D(outHistogramName, outHistogramName, NBINSFORPDF, MINRange, MAXRange);
   char* treecuts = selection; 
   if(!(inputTreeName=="WJet"))treecuts = "";

   treeTemp->Draw( TString("Mass2j_PFCor>>")+TString(outHistogramName), treecuts,"goff");
   th1H->Scale( 1. / th1H->Integral() );

   f.cd();
   th1H->Write();
   f.Close();
}
