// ====================================================================================
// Self Function
// ====================================================================================
void cmspre()
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.85,0.93,"#sqrt{s} = 7 TeV");
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.65,0.93,Form("#int #font[12]{L} dt = %.1f fb^{-1}", 5.0));

  latex.SetTextAlign(11); // align left
//  latex.DrawLatex(0.15,0.93,"CMS,  #sqrt{s} = 7 TeV");//preliminary 2011");
  latex.DrawLatex(0.15,0.96,"CMS preliminary");

}
void mkControlPlots(){
  const double intLUMI = 1.;
  const double WJets_scale = 31500.* intLUMI/80754279;
  const double WW_scale = (43.* intLUMI/4225913) ;
  const double WZ_scale = (18. * intLUMI/4265239);
        
  const double QCD_scale = 296600000 *	0.0002855 * intLUMI/25080241 ;
  const double ZJets_scale = 3048 * intLUMI/34689542 ;
  const double ttbar_scale = 157.5 * intLUMI/3573410;
  const double SToppS_scale = 1.44 * intLUMI/117647   ;
  const double SToppT_scale = 22.65  *intLUMI/1899460;
  const double SToppTW_scale = 7.87 *intLUMI/322638 ;
  const double STopS_scale = 3.19 * intLUMI/259572   ;
  const double STopT_scale = 41.92  *intLUMI/3891502;
  const double STopTW_scale = 7.87 *intLUMI/812544 ;
//   const double Ztautau_scale = 1614 *intLUMI/19937479;

 
	

  char* xlabel = " ";
  char* plotvar= " ";
  char* outfile= " ";
  char* outfile2= " ";
  double MAXRange = 0.;
  double MINRange = 100. ;
  int    NBINS = 10;
  int    slog = 0;
  double AMAXRange = 0.;
  double AMINRange = 100. ;
  int    ANBINS = 10;
  int drawleg = 0;
  int hplot = 0;
  //    plotvar = "JetPFCor_Pt[1]/Mass2j_PFCor";	 MINRange = 0.3;  MAXRange = 0.7;  NBINS = 20;  slog = 2; xlabel = "Jet 2 p_{T}/m_{jj}"; outfile = "AN-11-110/mu_jet2pt_ov_mjj.pdf"; outfile2 = "AN-11-110/mu_jet2pt_ov_mjj.png"; AMINRange = 0.25;  AMAXRange = 0.75; ANBINS = 25; 
  //  plotvar = "JetPFCor_Pt[0]";	 MINRange = 40;  MAXRange = 300;  NBINS = 27;  slog = 3; xlabel = "Leading Jet  p_{T}";  outfile = "AN-11-110/mu_jetld_pt.pdf"; outfile2 = "AN-11-110/mu_jetld_pt.png"; AMINRange = 20;  AMAXRange = 300;  ANBINS = 28; drawleg=1;
  //	 plotvar = "JetPFCor_Pt[1]";	 MINRange = 30;  MAXRange = 200;  NBINS = 17; slog = 3; xlabel = "Jet 2 p_{T}";  outfile = "AN-11-110/mu_jetnt_pt.pdf";  outfile2 = "AN-11-110/mu_jetnt_pt.png"; AMINRange = 20;  AMAXRange = 200;  ANBINS = 18;
  //  plotvar = "JetPFCor_Eta[0]";	 MINRange = -2.5 ; MAXRange = 2.5;  NBINS = 25;  slog = 1; xlabel = "Leading Jet  #eta";  outfile = "AN-11-110/mu_jetld_eta.pdf"; outfile2 = "AN-11-110/mu_jetld_eta.png";  AMINRange = -2.7 ; AMAXRange = 2.7;  ANBINS = 27;
  // 	 plotvar = "JetPFCor_Eta[1]";    MINRange = -2.5 ; MAXRange = 2.5;  NBINS = 25;  slog = 1; xlabel = "Jet 2 #eta";  outfile = "AN-11-110/mu_jetnt_eta.pdf"; outfile2 = "AN-11-110/mu_jetnt_eta.png"; AMINRange = -2.7 ; AMAXRange = 2.7;  ANBINS = 27;
  //  plotvar = "Mass2j_PFCor";	 MINRange = 40;  MAXRange = 400;  NBINS = 36;  slog = 3;  xlabel = "m_{jj} (GeV)";  outfile = "AN-11-110/mu_mjj.pdf"; outfile2 = "AN-11-110/mu_mjj.png"; AMINRange = 30;  AMAXRange = 400;  ANBINS = 37; drawleg =1;
  	 plotvar = "W_mt";	 MINRange = 50;  MAXRange = 170;  NBINS = 24; slog = 3;  xlabel ="W Transverse Mass (GeV)";  outfile = "AN-11-110/mu_W_mt.pdf"; outfile2 = "AN-11-110/mu_W_mt.png";  AMINRange = 40;  AMAXRange = 170;  ANBINS = 26; drawleg = 1;

  //  	 plotvar = "W_muon_pt";	 MINRange = 25;  MAXRange = 175;  NBINS = 50;  slog = 3; xlabel = "Muon p_{T} (GeV)";  outfile = "AN-11-110/mu_W_muon_pt.pdf"; outfile2 = "AN-11-110/mu_W_muon_pt.png";  AMINRange = 22;  AMAXRange = 175;  ANBINS = 51;
  // 	 plotvar = "W_muon_eta";	 MINRange = -2.1;  MAXRange = 2.1;  NBINS = 21;  slog = 1;  xlabel = "Muon #eta";  outfile = "AN-11-110/mu_W_muon_eta.pdf"; outfile2 = "AN-11-110/mu_W_muon_eta.png";  AMINRange = -2.5;  AMAXRange = 2.5;  ANBINS = 25;

  //plotvar = "event_met_pfmet";	 MINRange = 25;  MAXRange = 205;  NBINS = 18;  slog = 3; xlabel = "pf MET (GeV)";  outfile = "AN-11-110/mu_event_met_pfmet.pdf";  outfile2 = "AN-11-110/mu_event_met_pfmet.png";   AMINRange = 15;  AMAXRange = 205;  ANBINS = 19;
  //  	 plotvar = "JetPFCor_Mass[0]";	 MINRange = 0;  MAXRange = 80;  NBINS = 16;   slog = 3; xlabel ="Leading Jet Mass (GeV)";  outfile = "AN-11-110/mu_jet1_mass.pdf"; outfile2 = "AN-11-110/mu_jet1_mass.png";  AMINRange = 0;  AMAXRange = 80;  ANBINS = 16;
//	 plotvar = "JetPFCor_Mass[1]"; MINRange = 0;  MAXRange = 50;  NBINS = 10;   slog = 3; xlabel = "Jet 2 Mass (GeV)";  outfile = "AN-11-110/mu_jet2_mass.pdf"; outfile2 = "AN-11-110/mu_jet2_mass.png"; AMINRange = 0;  AMAXRange = 50;  ANBINS = 10;
  //  plotvar = "sqrt(JetPFCor_Pt[0]*JetPFCor_Pt[0]+JetPFCor_Pt[1]*JetPFCor_Pt[1]+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))";  MINRange = 45;  MAXRange = 305;  NBINS = 26;   slog = 3; xlabel = "Dijet System p_{T} (GeV)";  outfile = "AN-11-110/mu_dijet_pt.pdf"; outfile2 = "AN-11-110/mu_dijet_pt.png"; AMINRange = 35;  AMAXRange = 305;  ANBINS = 27;  
  // 	 plotvar = "(JetPFCor_Eta[0]-JetPFCor_Eta[1])"; MINRange = -1.0;  MAXRange = 1.0;  NBINS = 20;   slog = 1; xlabel =  "#Delta #eta (j,j)";  outfile = "AN-11-110/mu_deltaeta_jj.pdf"; outfile2 = "AN-11-110/mu_deltaeta_jj.png"; AMINRange = -1.2;  AMAXRange = 1.2;  ANBINS = 24;

  // 	 plotvar = "W_electron_et";	  MINRange = 35;  MAXRange = 180;  NBINS = 58;  slog = 0; xlabel = "Electron e_{T} (GeV)";  outfile = "AN-11-110/mu_W_electron_et.pdf"; outfile2 = "AN-11-110/mu_W_electron_et.png"; AMINRange = 30;  AMAXRange = 180;  ANBINS = 60; 
  //     	 plotvar = "W_electron_eta";	 MINRange = -2.5;  MAXRange = 2.5;  NBINS = 25;  slog = 1;  xlabel = "Electron #eta";  outfile = "AN-11-110/mu_W_electron_eta.pdf"; outfile2 = "AN-11-110/mu_W_electron_eta.png"; AMINRange = -3.; AMAXRange = 3.;  ANBINS = 30;
  // const char* plotvar = "JetPFCor_dphiMET[0]";  const double MINRange = -3.2;  const double MAXRange = 3.2; const int NBINS = 32;   slog = 1; xlabel = "#Delta #phi (Leading Jet, MET)";  outfile = "AN-11-110/mu_deltaphi_jetldmet.pdf"; outfile2 = "AN-11-110/mu_deltaphi_jetldmet.png"; AMINRange = -3.5; AMAXRange = 3.5;  ANBINS = 35;


  // const char*  plotvar = "sqrt((JetPFCor_Eta[0]-JetPFCor_Eta[1])**2+(abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi()))**2)";  const double MINRange = 0.4;  const double MAXRange = 5.0; const int NBINS = 46;   slog = 1; xlabel = "#Delta R_{jj}";  outfile = "AN-11-110/mu_deltaRjj.pdf"; outfile2 = "AN-11-110/mu_deltaRjj.png"; AMINRange = 0.0; AMAXRange = 5.4;  ANBINS = 54;drawleg =1;


  

////////////////////////////////////////////higgs
//plotvar = "MassV2j_PFCor";   MINRange = 140; MAXRange = 700; NBINS = 56; slog = 3 ;    xlabel = "m_{l#nujj} (GeV)" ;  outfile ="AN-11-110/mu_mlvjj.pdf"; outfile2 = "AN-11-110/mu_mlvjj.png";   AMINRange = 110; AMAXRange = 700; ANBINS =59; drawleg =1;

//	 plotvar = "ptlvjj";   MINRange = 0; MAXRange = 150; NBINS = 15; slog = 3 ;    xlabel = "p_{T} of WW (GeV)" ;  outfile ="AN-11-110/mu_ptlvjj.pdf"; outfile2 = "AN-11-110/mu_ptlvjj.png";   AMINRange = 0; AMAXRange = 150; ANBINS =15;

//	 plotvar = "ylvjj";   MINRange = -2.5; MAXRange = 2.5; NBINS = 25; slog = 1 ;    xlabel = "WW rapidity" ;  outfile ="AN-11-110/mu_etalvjj.pdf"; outfile2 = "AN-11-110/mu_etalvjj.png";   AMINRange = -3.0; AMAXRange = 3.0; ANBINS =30;

  // plotvar = "qgld_Summer11CHS[0]";   MINRange = 0; MAXRange = 1; NBINS = 25; slog = 2 ;    xlabel = "Q/G Likelihood of Leading Jet" ;  outfile ="AN-11-110/mu_jetld_qgl.pdf"; outfile2 = "AN-11-110/mu_jetld_qgl.png";   AMINRange = 0; AMAXRange = 1; ANBINS =25;
  //   plotvar = "qgld_Summer11CHS[1]";   MINRange = 0; MAXRange = 1; NBINS = 25; slog = 2 ;    xlabel = "Q/G Likelihood of Second Jet" ;  outfile ="AN-11-110/mu_jetnt_qgl.pdf"; outfile2 = "AN-11-110/mu_jetnt_qgl.png";   AMINRange = 0; AMAXRange = 1; ANBINS =25;

  // plotvar = "ang_ha";   MINRange = 0; MAXRange = 1; NBINS = 10; slog = 1 ;    xlabel = "Cos #theta_{1}" ;  outfile ="AN-11-110/mu_ha.pdf"; outfile2 = "AN-11-110/mu_ha.png";   AMINRange = 0; AMAXRange = 1; ANBINS =10; drawleg =1;

//plotvar = "ang_hb";   MINRange = 0; MAXRange = 1; NBINS = 10; slog = 1 ;    xlabel = "Cos #theta_{2}" ;  outfile ="AN-11-110/mu_hb.pdf"; outfile2 = "AN-11-110/mu_hb.png";   AMINRange = 0; AMAXRange = 1; ANBINS =10;

  //plotvar = "ang_hs";   MINRange = -1.0; MAXRange = 1.; NBINS = 20; slog = 1 ;    xlabel = "Cos #theta*" ;  outfile ="AN-11-110/mu_hs.pdf"; outfile2 = "AN-11-110/mu_hs.png";   AMINRange = -1.0; AMAXRange = 1; ANBINS =20; drawleg =0;

  //plotvar = "ang_phi";   MINRange = -3.2; MAXRange = 3.2; NBINS = 32; slog = 6 ;    xlabel = "#Phi (rad)" ;  outfile ="AN-11-110/mu_phi.pdf"; outfile2 = "AN-11-110/mu_phi.png";   AMINRange = -3.5; AMAXRange = 3.5; ANBINS =35;
  //	  plotvar = "ang_phib";   MINRange = -3.2; MAXRange = 3.2; NBINS = 32; slog = 6 ;    xlabel = "#Phi_{1} (rad)" ;  outfile ="AN-11-110/mu_phib.pdf"; outfile2 = "AN-11-110/mu_phib.png";   AMINRange = -3.5; AMAXRange = 3.5; ANBINS =35;
  //   plotvar = "W_electron_charge";   MINRange = -1.2; MAXRange = 1.2; NBINS = 24; slog = 1 ;    xlabel = "Electron Charge" ;  outfile ="AN-11-110/mu_charge.pdf"; outfile2 = "AN-11-110/mu_charge.png";   AMINRange = -1.2; AMAXRange = 1.2; ANBINS =24;

//  plotvar = "W_muon_charge";   MINRange = -1.2; MAXRange = 1.2; NBINS = 24; slog = 1 ;    xlabel = "Muon Charge" ;  outfile ="AN-11-110/mu_charge.pdf"; outfile2 = "AN-11-110/mu_charge.png";   AMINRange = -1.2; AMAXRange = 1.2; ANBINS =24;
//	   plotvar = "mva2j350mu";  MINRange = 0; MAXRange = 1; NBINS = 25; slog = 2;    xlabel = "Likelihood Output 2j mu350 " ;   outfile ="AN-11-110/mu_mva2j350_top.pdf"; outfile2 = "AN-11-110/mu_mva2j350_top.png";   AMINRange = 0; AMAXRange = 1; ANBINS =; hplot =0 ; drawleg=1;

   plotvar = "mva2j180el";  MINRange = 0; MAXRange = 1; NBINS = 25; slog = 2;    xlabel = "Likelihood Output 2j el180 " ;   outfile ="AN-11-110/el_mva2j180_top.pdf"; outfile2 = "AN-11-110/el_mva2j180_top.png";   AMINRange = 0; AMAXRange = 1; ANBINS =; hplot =0 ; drawleg=1;

//  plotvar = "mva2j500mu";  MINRange = -0.2; MAXRange = 1.2; NBINS = 28; slog = 2;    xlabel = "Likelihood Output " ;   outfile ="AN-11-110/mu_mva2j500.pdf"; outfile2 = "AN-11-110/mu_mva2j500.png";   AMINRange = -0.2; AMAXRange = 1.2; ANBINS =28; hplot =1 ; drawleg=1;
	 
  //  const char* the_cut = "1";
//  double BINWIDTH = ((MAXRange-MINRange)/NBINS);
//TCut the_cut("effwt*((evtNJ==2 || evtNJ==3) && (W_mt > 50.) && (JetPFCor_Pt[0]>40.) && (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) && (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) && (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) && (JetPFCor_Pt[1]/Mass2j_PFCor<0.7)) && (event_met_pfmet >30)");

 
//  TCut the_cut("(ggdevt==2&&(Mass2j_PFCor>65&&Mass2j_PFCor<95)&&(fit_mlvjj>170&&fit_mlvjj<200)&&fit_status==0)");
   //TCut the_cut("TopWm>0");
TCut the_cut("effwt*((ggdevt==2||ggdevt==3) && fit_status==0 )");
TCut the_cut2("effwt*((ggdevt==2) && fit_status==0 )");
TCut the_cut3("effwt*((ggdevt==3) && fit_status==0  )");
//TCut the_cut("(fit_status==0 && TopWm!=0 )*effwt");

cout << TString(plotvar) << MINRange << MAXRange << NBINS << "THE CUT " << endl;



  const double BINWIDTH = ((MAXRange-MINRange)/NBINS);

 TFile f("plotvar_histo.root", "RECREATE");


  TFile fin2("InData/RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_4p7invfb.root", "read");
//TFile fin2("InData/RD_WmunuJets_DataAll_GoldenJSON_4p7invfb.root", "read");
  TTree* treedata = (TTree*) fin2.Get("WJet");
  double nData = treedata->GetEntries();
  std::cout << "ndata =" << nData <<std::endl;
  TH1* th1data = new TH1D("th1data", "th1data", NBINS, MINRange, MAXRange);
  TH1* th1data1 = new TH1D("th1data1", "th1data1", NBINS, MINRange, MAXRange);
  treedata->Draw(TString(plotvar)+TString(">>th1data"), the_cut, "goff");
  treedata->Draw(TString(plotvar)+TString(">>th1data1"), the_cut, "goff");
 
  TFile* H190_file =  new TFile("InData/RD_el_HWWMH190_CMSSW428.root", "READ");
  TFile* H500_file =  new TFile("InData/RD_el_HWWMH500_CMSSW428.root", "READ");
  TTree* treeh190 = (TTree*) H190_file->Get("WJet");
  TTree* treeh500 = (TTree*) H500_file->Get("WJet");
  TH1* th1H500 = new TH1D("th1H500", "th1H500", NBINS, MINRange, MAXRange);
  treeh500->Draw(TString(plotvar)+TString(">>th1H500"), the_cut, "goff");

	
  TFile* wwShape_file =  new TFile("InData/RD_el_WW_CMSSW428.root", "READ");
  TTree* treeTemp3 = (TTree*) wwShape_file->Get("WJet");
  TFile* wzShape_file =  new TFile("InData/RD_el_WZ_CMSSW428.root", "READ");
  TTree* treeTemp4 = (TTree*) wzShape_file->Get("WJet");

  TH1* th1ww = new TH1D("th1ww", "th1ww", NBINS, MINRange, MAXRange);
  TH1* th1wz = new TH1D("th1wz", "th1wz", NBINS, MINRange, MAXRange);
  th1ww->Sumw2();
  th1wz->Sumw2();


  treeTemp3->Draw(TString(plotvar)+TString(">>th1ww"), the_cut, "goff");
  treeTemp4->Draw(TString(plotvar)+TString(">>th1wz"), the_cut, "goff");

  TFile* wjetsShape_file =  new TFile("InData/RD_el_WpJ_CMSSW428.root", "READ");
  TTree* tree2 = (TTree*) wjetsShape_file->Get("WJet");

  TH1* th1wjets = new TH1D("th1wjets", "th1wjets", NBINS ,MINRange,MAXRange);

  tree2->Draw(TString(plotvar)+TString(">>th1wjets"), the_cut, "goff");
  th1wjets->Sumw2();
  std::cout << "got w+jets " <<std::endl;

  // top pair pdf
 
  ttbar_file =  new TFile("InData/RD_el_TTbar_CMSSW428.root", "READ");
  TTree* tree12 = (TTree*) ttbar_file->Get("WJet");
 
  TH1* th1Top = new TH1D("th1Top", "th1Top", NBINS, MINRange, MAXRange);
  th1Top->Sumw2();
  // cross section: 157.5 pb, events_gen = 3701947 (These are summer11 TTJets sample
  
   tree12->Draw(TString(plotvar)+TString(">>th1Top"), the_cut, "goff");
   std::cout << "got top " <<std::endl; 
 
  TFile* qcd_file1 =  new TFile("InData/RDQCD_WenuJets_DataAll_GoldenJSON_2p1invfb.root", "READ");
  TTree* tree22 = (TTree*) qcd_file1->Get("WJet");
   TH1* th1qcd = new TH1D("th1qcd", "th1qcd", NBINS, MINRange, MAXRange);
  th1qcd->Sumw2();
 
    tree22->Draw(TString(plotvar)+TString(">>th1qcd"), the_cut, "goff");
	int n2 = tree22->Draw(TString(plotvar),  the_cut2, "goff");
	int n3 = tree22->Draw(TString(plotvar),  the_cut3 , "goff");

  std::cout << "got qcd " << " n2 " << n2 <<  "n3  " << n3 <<std::endl; 

   TFile* zjets_file =  new TFile("InData/RD_el_ZpJ_CMSSW428.root", "READ");
  TTree* tree32 = (TTree*) zjets_file->Get("WJet");
  TH1* th1zjets = new TH1D("th1zjets", "th1zjets", NBINS, MINRange, MAXRange);
  th1zjets->Sumw2();
   tree32->Draw(TString(plotvar)+TString(">>th1zjets"), the_cut, "goff");

  TFile* stops_file =  new TFile("InData/RD_el_STopS_T_CMSSW428.root", "READ");
  TTree* tree54 = (TTree*) stops_file->Get("WJet");
  TFile* stopt_file =  new TFile("InData/RD_el_STopT_T_CMSSW428.root", "READ");
  TTree* tree55 = (TTree*) stopt_file->Get("WJet");
  TFile* stoptW_file =  new TFile("InData/RD_el_STopTW_T_CMSSW428.root", "READ");
  TTree* tree56 = (TTree*) stoptW_file->Get("WJet");
  TH1* th1stops = new TH1D("th1stops", "th1stops", NBINS, MINRange, MAXRange);
  TH1* th1stopt = new TH1D("th1stopt", "th1stopt", NBINS, MINRange, MAXRange);
  TH1* th1stoptw = new TH1D("th1stoptw", "th1stoptw", NBINS, MINRange, MAXRange);
  th1stops->Sumw2();
  th1stopt->Sumw2();
  th1stoptw->Sumw2();

  tree54->Draw(TString(plotvar)+TString(">>th1stops"), the_cut, "goff");
  tree55->Draw(TString(plotvar)+TString(">>th1stopt"), the_cut, "goff");
  tree56->Draw(TString(plotvar)+TString(">>th1stoptw"), the_cut, "goff");
 
  TFile* stopps_file =  new TFile("InData/RD_el_STopS_Tbar_CMSSW428.root", "READ");
  TTree* tree64 = (TTree*) stopps_file->Get("WJet");
  TFile* stoppt_file =  new TFile("InData/RD_el_STopT_Tbar_CMSSW428.root", "READ");
  TTree* tree65 = (TTree*) stoppt_file->Get("WJet");
  TFile* stopptW_file =  new TFile("InData/RD_el_STopTW_Tbar_CMSSW428.root", "READ");
  TTree* tree66 = (TTree*) stopptW_file->Get("WJet");
  TH1* th1stopps = new TH1D("th1stopps", "th1stopps", NBINS, MINRange, MAXRange);
  TH1* th1stoppt = new TH1D("th1stoppt", "th1stoppt", NBINS, MINRange, MAXRange);
  TH1* th1stopptw = new TH1D("th1stopptw", "th1stopptw", NBINS, MINRange, MAXRange);
  th1stopps->Sumw2();
  th1stoppt->Sumw2();
  th1stopptw->Sumw2();
  tree64->Draw(TString(plotvar)+TString(">>th1stopps"), the_cut, "goff");
  tree65->Draw(TString(plotvar)+TString(">>th1stoppt"), the_cut, "goff");
  tree66->Draw(TString(plotvar)+TString(">>th1stopptw"), the_cut, "goff");



 gROOT->ProcessLine(".L ~/tdrstyle.C");
 setTDRStyle();
 tdrStyle->SetErrorX(0.5);
 tdrStyle->SetPadRightMargin(0.05);

 tdrStyle->SetLegendBorderSize(0);

    th1data->Sumw2();

  TCanvas* c1 = new TCanvas("c1","",10,10, 800, 800);
  TPad *d1, *d2;

  c1->Divide(1,2,0,0);
  d1 = (TPad*)c1->GetPad(1);
  d1->SetPad(0.01,0.30,0.95,0.99);
  d2 = (TPad*)c1->GetPad(2);
  d2->SetPad(0.01,0.02,0.95,0.30);

  THStack* hs = new THStack("hs","MC contribution");
  th1Top->Scale(ttbar_scale);
  th1Top->SetFillColor(kGreen+2);
  th1Top->SetLineColor(kGreen+2);
  th1Top->SetLineWidth(0);
  th1stops->Scale(STopS_scale);
  th1stops->SetFillColor(7);
  th1stops->SetLineWidth(2);
  th1stopt->Scale(STopT_scale);
  th1stopt->SetFillColor(13);
  th1stopt->SetLineWidth(2);
  th1stoptw->Scale(STopTW_scale);
  th1stoptw->SetFillColor(9);
  th1stoptw->SetLineWidth(2);
  th1wjets->Scale(WJets_scale);
  th1wjets->SetFillColor(kRed);
  th1wjets->SetLineColor(kRed);
  th1wjets->SetLineWidth(0);
  th1ww->Scale(WW_scale);
  th1ww->SetFillColor(kAzure+8);
  th1ww->SetLineColor(kAzure+8);
  th1ww->SetLineWidth(0);
  th1wz->Scale(WZ_scale);
  th1wz->SetFillColor(11);
  th1wz->SetLineWidth(0);
  // th1qcd->Scale(QCD_scale);
  
  th1qcd->SetFillColor(kGray+1);
  th1qcd->SetLineColor(kGray+1);
  th1qcd->SetLineWidth(0);
  th1zjets->Scale(ZJets_scale);
  th1zjets->SetFillColor(kYellow);
  th1zjets->SetLineColor(kYellow);
  th1zjets->SetLineWidth(0);
  std::cout <<" qcd " << th1qcd->Integral() << std::endl;
  std::cout <<"tt " << th1Top->Integral() << std::endl;
  std::cout <<"ww " << th1ww->Integral() << std::endl;
  std::cout << "wz " <<th1wz->Integral() << std::endl;
  std::cout << "z " <<th1zjets->Integral() << std::endl;
  

 
  double den_qcd = th1Top->Integral()+th1stops->Integral()+th1stopt->Integral()+th1stoptw->Integral()+th1wjets->Integral()+th1ww->Integral()+th1wz->Integral()+th1zjets->Integral();
 double qcd_scale = (n2*0.062 + n3*0.021) / (n2+n3); //electron
//double qcd_scale = (n2*0.002 + n3*0.000) / (n2+n3) ;//muon
std::cout << " qcd_scale  " << qcd_scale <<std::endl;
  th1qcd->Scale(qcd_scale*den_qcd/th1qcd->Integral()); 

  double den = th1Top->Integral()+th1stops->Integral()+th1stopt->Integral()+th1stoptw->Integral()+th1wjets->Integral()+th1ww->Integral()+th1wz->Integral()+th1zjets->Integral()+th1qcd->Integral();
  std::cout << "den = " <<den <<std::endl;
  th1qcd->Scale(th1data->Integral()/den);
  std::cout <<" data " <<  th1data->Integral() << std::endl;
  std::cout <<" qcd " << th1qcd->Integral() << std::endl;
  th1Top->Scale(th1data->Integral()/den);
  std::cout <<"tt " << th1Top->Integral() << std::endl;
  th1stops->Scale(th1data->Integral()/den);
  std::cout <<"stops " << th1stops->Integral() << std::endl;
  th1stopt->Scale(th1data->Integral()/den);
  std::cout <<"stopt " << th1stopt->Integral() << std::endl;
  th1stoptw->Scale(th1data->Integral()/den);
  std::cout <<"stoptw " << th1stoptw->Integral() << std::endl;
  th1wjets->Scale(th1data->Integral()/den);
  std::cout <<"wjets " << th1wjets->Integral() << std::endl;
  th1ww->Scale(th1data->Integral()/den);
  std::cout <<"ww " << th1ww->Integral() << std::endl;
  th1wz->Scale(th1data->Integral()/den);
  std::cout << "wz " <<th1wz->Integral() << std::endl;
  th1zjets->Scale(th1data->Integral()/den);
  std::cout << "z " <<th1zjets->Integral() << std::endl;
  double den2 = th1Top->Integral()+th1stops->Integral()+th1stopt->Integral()+th1stoptw->Integral()+th1wjets->Integral()+th1ww->Integral()+th1wz->Integral()+th1zjets->Integral()+th1qcd->Integral();
  std::cout << "den2 " << den2 << std::endl;
  th1Top->Add(th1stoptw,1);
  th1Top->Add(th1stopt,1);
  th1Top->Add(th1stops,1);
  th1ww->Add(th1wz,1);

  TH1D *th1tot = (TH1D*)th1wjets->Clone("th1tot");
  th1tot->Reset();
  th1tot->Add(th1ww,1);
  th1tot->Add(th1qcd,1);
  th1tot->Add(th1Top,1);
  th1tot->Add(th1wjets,1);
  th1tot->Add(th1zjets,1);
  TH1D* th1totClone = ( TH1D*) th1tot->Clone("th1totClone");
  th1totClone->SetMarkerStyle(0);
  th1totClone->SetFillStyle(3003);
  th1totClone->SetFillColor(11);
  th1totClone->SetLineColor(0);
  double binErr(0.0);
  for(int i=0; i<th1totClone->GetNbinsX(); ++i) {
    binErr = sqrt((th1ww->GetBinError(i))**2+(th1qcd->GetBinError(i))*2+(th1Top->GetBinError(i))**2+(th1wjets->GetBinError(i))**2+(th1zjets->GetBinError(i))**2);
    th1totClone->SetBinError(i, binErr);
  }
  
  hs->Add(th1zjets);
  d1->cd();
  gPad->SetBottomMargin(0.0);
  gPad->SetTopMargin(0.1);
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.14);

  hs->Add(th1qcd);
  hs->Add(th1Top);
  hs->Add(th1wjets);
  hs->Add(th1ww);

  //  float  legX0=0.56; legX1=0.99; legY0=0.4; legY1=0.88;
  // float  legX0=0.35; legX1=0.85; legY0=0.4; legY1=0.88;
   float  legX0=0.18; legX1=0.52; legY0=0.4; legY1=0.88;
  TLegend * Leg = new TLegend( legX0, legY0, legX1, legY1);
  Leg->SetFillColor(0);
  Leg->SetFillStyle(0);
  Leg->SetTextSize(0.04);
  Leg->AddEntry(th1data,  "Electron Data",  "PLE");
  Leg->AddEntry(th1ww,  "WW/WZ ",  "f");
  Leg->AddEntry(th1wjets,  "W+jets",  "f");
  Leg->AddEntry(th1Top,  "top",  "f");
  Leg->AddEntry(th1qcd,  "QCD",  "f");

  Leg->AddEntry(th1zjets,  "Z+Jets",  "f");
  Leg->AddEntry(th1tot,  "MC Uncertainty",  "f");
  if(hplot==1) Leg->AddEntry(th1H500,  "H (500) x 10",  "L");
  Leg->SetFillColor(0);

  th1H500->SetLineColor(kBlack);
  th1H500->SetLineWidth(3);
  th1H500->Scale(10);
  

  TH1* th1totempty = new TH1D("th1totempty", "th1totempty", ANBINS, AMINRange, AMAXRange);
  th1data->SetMarkerStyle(20);
  th1data->SetMarkerSize(1.25);
  th1data->SetLineWidth(2);

  th1tot->SetFillStyle(3013);
  th1tot->SetFillColor(11);
  th1tot->SetLineColor(11);
  th1tot->SetMarkerStyle(0);


  char tmpc[100];sprintf(tmpc,"Events / %.1f GeV",BINWIDTH);
  if (slog==1)    sprintf(tmpc,"Events/ %.1f",BINWIDTH);
  if (slog==2)    sprintf(tmpc,"Events/ %.2f",BINWIDTH);
  if (slog==3)    sprintf(tmpc,"Events/ %.0f GeV",BINWIDTH);
  if (slog==6)    sprintf(tmpc,"Events/ %.1f rad",BINWIDTH);
  th1totempty->SetYTitle(tmpc);
//  th1totempty->GetYaxis()->SetTitleSize(0.1);
  th1totempty->GetYaxis()->SetTitleOffset(1.2);
  th1totempty->GetYaxis()->SetLabelOffset(0.01);
//  th1totempty->GetYaxis()->CenterTitle(true);
   th1totempty->GetYaxis()->SetLabelSize(0.04);
  // th1totClone->Draw("e3");   

  th1tot->SetMinimum(0.0);
  int maxbin = th1data->GetMaximumBin();
  float maxval = th1data->GetBinContent(maxbin);
  cout << "maxval " <<maxval <<endl;
  th1totempty->SetMaximum(1.6*maxval);
  th1totempty->SetMinimum(0.0);
  if(slog==1) th1totempty->SetMaximum(1.6*maxval);
  th1data->SetMinimum(0.0);

  th1totempty->Draw();
  //th1tot->Draw("e2same");
  th1data->Draw("esame");
  hs->Draw("samehist");
  if (hplot ==1) th1H500->Draw("same");
  th1tot->Draw("e3same");

  th1data->Draw("esame");
  cmspre();    
  if (drawleg ==1)  Leg->Draw();  
   // th1data->Draw("Axissame");
  gPad->RedrawAxis();
  d2->cd();

 TH1F    * hhratio    = (TH1F*) th1data->Clone("hhratio")  ;
  hhratio->Sumw2();

  gPad->SetLeftMargin(0.14);
  gPad->SetTopMargin(0);
  gPad->SetRightMargin(0.05);
  gPad->SetFrameBorderSize(0);
  gPad->SetBottomMargin(0.3);
  gPad->SetTickx();

  hhratio->SetMarkerSize(1.25);
//  hhratio->GetYaxis()->SetRangeUser(0.48,1.52);
  hhratio->GetYaxis()->SetRangeUser(0.3,1.7);
  hhratio->GetXaxis()->SetTitle(xlabel);
  hhratio->GetXaxis()->SetTitleOffset(0.9);
  hhratio->GetXaxis()->SetTitleSize(0.15);
  hhratio->GetXaxis()->SetLabelSize(0.15);
  char tmpc2[100];sprintf(tmpc2,"Ratio Data/MC");
  hhratio->SetYTitle(tmpc2);
  hhratio->GetYaxis()->SetTitleSize(0.1);
  hhratio->GetYaxis()->SetTitleOffset(0.5);
  hhratio->GetYaxis()->CenterTitle(true);
  hhratio->GetYaxis()->SetLabelSize(0.1);
  cout << hhratio->GetNbinsX() << endl;
  cout << th1tot->GetNbinsX() << endl;
  hhratio->Divide(th1tot);
  double binError(0.0), mcbinentry(0.0), mcerror(0.0);
  for(int i=0; i<hhratio->GetNbinsX(); ++i) {
    binError = hhratio->GetBinError(i);
    mcerror = th1tot->GetBinError(i);
    mcbinentry = th1tot->GetBinContent(i);
    if(mcbinentry>0.) mcerror /= mcbinentry;
    else mcerror = 0.0;
    binError = sqrt(binError**2 + mcerror**2);
    hhratio->SetBinError(i, binError);
  }
  TH1D *th1emptyclone = new TH1D("th1emptyclone", "th1emptyclone", ANBINS, AMINRange, AMAXRange);
  th1emptyclone->GetYaxis()->SetRangeUser(0.4,1.6);
  th1emptyclone->GetXaxis()->SetTitle(xlabel);
  th1emptyclone->GetXaxis()->SetTitleOffset(0.9);
  th1emptyclone->GetXaxis()->SetTitleSize(0.15);
  th1emptyclone->GetXaxis()->SetLabelSize(0.15);
  char tmpc2[100];sprintf(tmpc2,"Ratio Data/MC");
  th1emptyclone->SetYTitle(tmpc2);
  th1emptyclone->GetYaxis()->SetTitleSize(0.1);
  th1emptyclone->GetYaxis()->SetNdivisions(505);
th1emptyclone->GetYaxis()->SetTitleOffset(0.5);
  th1emptyclone->GetYaxis()->CenterTitle(true);
  th1emptyclone->GetYaxis()->SetLabelSize(0.1);
  th1emptyclone->Draw();
  hhratio->Draw("esame");
  TLine *line; line = new TLine(AMINRange,1.0,AMAXRange,1.0);
  line->SetLineStyle(1);
  line->SetLineWidth(1);
  line->SetLineColor(1);
  line->Draw();

   c1->Print(outfile);
  //gPad->WaitPrimitive();
  c1->Modified();
  c1->Update();
  c1->SaveAs(outfile2); 

  // f.Write();



}
