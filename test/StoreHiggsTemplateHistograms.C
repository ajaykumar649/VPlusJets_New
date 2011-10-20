

void StoreHiggsTemplateHistograms() {

   //////// Figure out how many Higgs events were generated in the sample //////
   /////// These are needed to compute "efficiency*acceptance" for the Higgs sample ////////
   /////// We store Higgs histograms after correcting for "efficiency*acceptance" factor ////////


   /************** Number of events generated/ ran over in Summer11 samples ****/
   const int nEvtGenH160 = 109992;
   const int nEvtGenH170 = 109989;
   const int nEvtGenH180 = 109325;
   const int nEvtGenH190 = 109986;
   const int nEvtGenH200 = 109315;
   const int nEvtGenH250 = 109992;
   const int nEvtGenH300 = 109990;
   const int nEvtGenH350 = 109313;
   const int nEvtGenH400 = 107879;
   const int nEvtGenH450 = 107158;
   const int nEvtGenH500 = 107169;
   const int nEvtGenH550 = 107870;
   const int nEvtGenH600 = 108561;



  for (int nJet=2; nJet<4; nJet++) {
    for (int massRange=1; massRange<5; massRange++) {

      if( nJet==3 && massRange==4) continue;
      grabDataSubtractedHistograms( nJet, massRange);

    } /// close massRange loop 


    getHiggsMasspoint(160, nEvtGenH160, nJet);
   getHiggsMasspoint(170, nEvtGenH170, nJet);
   getHiggsMasspoint(180, nEvtGenH180, nJet);
   getHiggsMasspoint(190, nEvtGenH190, nJet);
   getHiggsMasspoint(200, nEvtGenH200, nJet);
   getHiggsMasspoint(250, nEvtGenH250, nJet);
   getHiggsMasspoint(300, nEvtGenH300, nJet);
   getHiggsMasspoint(350, nEvtGenH350, nJet);
   getHiggsMasspoint(400, nEvtGenH400, nJet);
   getHiggsMasspoint(450, nEvtGenH450, nJet);
   getHiggsMasspoint(500, nEvtGenH500, nJet);
   getHiggsMasspoint(550, nEvtGenH550, nJet);
   getHiggsMasspoint(600, nEvtGenH600, nJet);

  } /// close nJet loop 

}




void grabDataSubtractedHistograms(int nJet, int massRange) {
  TFile f("Histograms_data_and_template.root", "update");


  // Figure out the plot directory name first
  TString dir2jet = "./plots_10172011_2jetsample";
  TString dir3jet = "./plots_10172011_3jetsample";
  TString dirName = "";
  if(nJet==2) dirName = dir2jet;
  if(nJet==3) dirName = dir3jet;

  TString massStr = "";
  if(nJet==2 && massRange==1) massStr = "150-230";
  else if(nJet==2 && massRange==2) massStr = "200-400";
  else if(nJet==2 && massRange==3) massStr = "360-500";
  else if(nJet==2 && massRange==4) massStr = "450-800";
  else if(nJet==3 && massRange==1) massStr = "150-230";
  else if(nJet==3 && massRange==2) massStr = "200-400";
  else if(nJet==3 && massRange==3) massStr = "360-800";

  TString ConnectorStr = "";
  if( !(massStr=="") ) ConnectorStr = "-";
  TString fitFileName = TString("mLnuJJ-") + massStr + ConnectorStr + TString("combined-fit");


  TFile* fitFile = new TFile( dirName + TString("/") + fitFileName+".root", "read");
  TCanvas* fitCan = (TCanvas*) fitFile->Get( fitFileName );
  RooHist* data = (RooHist*) fitCan->FindObject( "h_data" );
  RooCurve* fit = (RooCurve*) fitCan->FindObject( "h_total" );
  RooCurve* fit_wjj = (RooCurve*) fitCan->FindObject( "h_Wjets" );
  RooCurve* fit_diboson = (RooCurve*) fitCan->FindObject( "h_diboson" );
  RooCurve* fit_Top = (RooCurve*) fitCan->FindObject( "h_Top" );
  RooCurve* fit_QCD = (RooCurve*) fitCan->FindObject( "h_QCD" );
  RooCurve* fit_Zjets = (RooCurve*) fitCan->FindObject( "h_Zjets" );


  TFile* systFileUp = new TFile( dirName + TString("SystUp/") + fitFileName+".root", "read");
  TCanvas* systFileUpCan = (TCanvas*) systFileUp->Get( fitFileName );
  RooCurve* fit_wjj_systUp = (RooCurve*) systFileUpCan->FindObject( "h_Wjets" );
  RooCurve* fit_systUp = (RooCurve*) systFileUpCan->FindObject( "h_total" );

  TFile* systFileDown = new TFile( dirName + TString("SystDown/") + fitFileName+".root", "read");
  TCanvas* systFileDownCan = (TCanvas*) systFileDown->Get( fitFileName );
  RooCurve* fit_wjj_systDown = (RooCurve*) systFileDownCan->FindObject( "h_Wjets" );
  RooCurve* fit_systDown = (RooCurve*) systFileDownCan->FindObject("h_total");


  ///// Now save everything ///////////////
  TString outPrefix = TString("2jet_MassRange_") + massStr + TString("_");
  if(nJet==3) outPrefix = TString("3jet_MassRange_") + massStr + TString("_");


  data->SetName( outPrefix+"hist_data" );
  fit->SetName( outPrefix+"curve_fitTotal" );
  fit_wjj->SetName( outPrefix+"curve_WJets" );
  fit_diboson->SetName( outPrefix+"curve_diboson" );
  fit_Top->SetName(  outPrefix+"curve_Top" );
  fit_QCD->SetName(  outPrefix+"curve_QCD" );
  fit_Zjets->SetName(  outPrefix+"curve_Zjets" );
  fit_wjj_systUp->SetName( outPrefix+"curve_WJets_SystUp" );
  fit_wjj_systDown->SetName( outPrefix+"curve_WJets_SystDown" );
  fit_systUp->SetName( outPrefix+"curve_fitTotal_SystUp" );
  fit_systDown->SetName( outPrefix+"curve_fitTotal_SystDown" );

  
  f.cd();
  data->Write();
  fit->Write();
  fit_wjj->Write();
  fit_diboson->Write();
  fit_Top->Write();
  fit_QCD->Write();
  fit_Zjets->Write();
  fit_wjj_systUp->Write();
  fit_wjj_systDown->Write();
  fit_systUp->Write();
  fit_systDown->Write();

  f.Close();
  delete fitFile;
  delete systFileUp;
  delete systFileDown;
}





void getHiggsMasspoint(int mHiggs, int nEventsGen, int nJet) {

  const double MINRange = 150.0;
  const double MAXRange = 800.0;
  const int BINWIDTH = 10;
  const int NBINSFORPDF = (int)((MAXRange-MINRange)/BINWIDTH);
  char* selection = (char*) "evtNJ==2";
  if(nJet==3) selection = (char*) "evtNJ==3";



   TFile f("Histograms_data_and_template.root", "update");
   char temp[100];

   sprintf(temp, "data/ReducedTree/NewKfitRDTree/RD_mu_HWWMH%d_CMSSW428.root", mHiggs);
   TFile* mhfile = new TFile( temp, "read");
   TTree* treeTemp = (TTree*) mhfile->Get("WJet");
   sprintf(temp, "%djet_HiggsTemplate_Mass_%d", nJet, mHiggs);

   TH1* th1H = new TH1D(temp, temp, NBINSFORPDF, MINRange, MAXRange);
   treeTemp->Draw( TString("fit_mlvjj>>")+TString(temp), selection,"goff");
   //th1H->Scale( 1. / th1H->Integral() );

   th1H->Scale( 1. / nEventsGen );
   f.cd();
   th1H->Write();
   f.Close();
   delete th1H;
   delete mhfile;
}
