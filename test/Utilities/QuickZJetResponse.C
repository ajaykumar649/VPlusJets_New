{

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  gStyle->SetOptFit(1111);
  //tdrStyle->SetOptStat("mer"); 

  const int NPTBINS=15;
  const double PTMIN = 0.0;
  const double PTMAX = 200.0;
  const float PTBINS[NPTBINS+1] = {0,10,20, 25,30,36,43,51,61,73,87,104,124,148,177,212};


  TCut BackToBackJet("abs(JetPF_Eta[0])<1.3 && ((JetPF_Pt[1]/Z_pt<0.2) || (JetPF_Pt[1]<5.)) && abs(JetPF_dphiBoson[0])>2.7 && abs(JetPF_dphiBoson[0])<5.");
  TCut GoodZ("Z_mass>80. && Z_mass<100.");
  TCut ZptRange("Z_pt>10. && Z_pt<200");




  // --------------------------------------------------------------

  TFile* fout = new TFile("Response.root","recreate");

  TFile* f = new TFile("ZeeJets_Dec22ReReco.root");
  TTree* tree = (TTree*) f->Get("ZJet");
  TFile* fMC = new TFile("ZeeJets_MC_Fall2010.root");
  TTree* treeMC = (TTree*) fMC->Get("ZJet");



  TProfile* resp_all = new TProfile("resp_all","", NPTBINS, PTBINS);
  TProfile* resp_all_MC = new TProfile("resp_all_MC","", NPTBINS, PTBINS);
  resp_all_MC->SetLineColor(2);
  resp_all_MC->SetMarkerColor(2);

  tree->Draw("JetPF_Response[0]:Z_pt>>resp_all",
	     BackToBackJet && GoodZ && ZptRange,"goff");
  treeMC->Draw("JetPF_Response[0]:Z_pt>>resp_all_MC",
	     BackToBackJet && GoodZ && ZptRange,"goff");



  TH1D* resp_ratio = new TH1D("resp_ratio","", NPTBINS, PTBINS);
  resp_ratio->SetLineColor(5);
  resp_ratio->SetMarkerColor(5);
  resp_ratio->SetFillColor(5);
  double content =0.0;
  double error =0.0;
  double graphX[NPTBINS-1];
  double graphXerr[NPTBINS-1];
  double graphY[NPTBINS-1];
  double graphYerr[NPTBINS-1];


  for(int j=2;j<=NPTBINS;j++){
    content = resp_all->GetBinContent(j)/resp_all_MC->GetBinContent(j);
    resp_ratio->SetBinContent(j, content); 
    error = resp_ratio->GetBinContent(j) * 
      sqrt( (resp_all->GetBinError(j)/resp_all->GetBinContent(j))**2 +
	    (resp_all_MC->GetBinError(j)/resp_all_MC->GetBinContent(j))**2 );
    resp_ratio->SetBinError(j, error);

    graphX[j-2] = resp_all->GetBinCenter(j);
    graphXerr[j-2] = 0.0;
    graphY[j-2] = content;
    graphYerr[j-2] = 0.0;
    //cout << "content = " << content << ",  error = " << error << endl;
  }

  TGraphErrors* ge = new TGraphErrors(NPTBINS-1, graphX, graphY, graphXerr, graphYerr);
  TAxis* xaxis = ge->GetXaxis();
  TAxis* yaxis = ge->GetYaxis();
  xaxis->SetTitle("Z p_{T} [GeV]");
  yaxis->SetTitle("Jet response");
  yaxis->SetRangeUser(0.5, 2.0);
  ge->SetLineColor(4);
  ge->SetLineWidth(2);
  ge->SetLineStyle(2);



  TCanvas* c = new TCanvas("jetResponse", "Jet Response", 500,500);
  ge->Draw("CA");
  resp_ratio->GetXaxis()->SetRangeUser(10., 200.);
  resp_ratio->Draw("E3same");
  resp_all->Draw("same");
  resp_all_MC->Draw("same");
  ge->Draw("L");


  fout->cd();
  resp_all->Write();
  resp_all_MC->Write();
  resp_ratio->Write();
  ge->Write();
  c->Write();
  fout->Close();

  delete fout;
}
