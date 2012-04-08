#include <iostream>
#include "RooGlobalFunc.h"

const char* inputDirectory = "/home/kalanand/nobackup/WjjTrees/ReducedTree/NewKfitRDTree/";




void plotEfficiency() {

  gROOT->ProcessLine(".L EffTableReader.cc+");
  gROOT->ProcessLine(".L EffTableLoader.cc+");

    plotEfficiency("RD_mu_WpJ_CMSSW428.root", false);
  //   plotEfficiency("RD_el_WpJ_CMSSW428.root", false);
//   plotEfficiency("RD_el_WpJ_CMSSW428.root", true);
}



void plotEfficiency(char* inFile, bool computeDijetMHTefficiency) {
  
  bool isElectronTree = false;
  if( TString(inFile).Contains("_el_") ) isElectronTree = true;

  const char* treeName = "WJet";
  const std::string fDir = "/uscms/home/kalanand/cms/documents/notes/AN-11-266/trunk/";
  
  std::string eleHLTTableText = "eleEffsWP80ToHLTEle27_May10ReReco.txt";
  char* outputFileName = "effTree.root";
  if(isElectronTree && computeDijetMHTefficiency) {
    eleHLTTableText = "eleEffsHLTEle2jPfMht_data_LWA_Ele.txt";
    outputFileName = "effTree_HLTEle2jPfMht.root";
  }
  else if(isElectronTree) outputFileName = "effTree_HLTEle27_May10ReReco.root";
  else outputFileName = "effTree_muon.root";


  EffTableLoader scaleFactorEleReco(fDir + "eleEffsSCToReco_ScaleFactors.txt");
  EffTableLoader scaleFactorEleId(fDir + "eleEffsRecoToWP80_ScaleFactors.txt");
  EffTableLoader scaleFactorMuId(fDir + "muonEffsRecoToIso_ScaleFactors.txt");
  EffTableLoader effEle(fDir + eleHLTTableText);
  EffTableLoader effMu(fDir + "muonEffsIsoToHLT_data_LP_LWA.txt");
  EffTableLoader effJ30(fDir + "eleEffsHLTEle2jPfMht_data_LWA_Jet30.txt");
  EffTableLoader effJ25NoJ30(fDir + "eleEffsHLTEle2jPfMht_data_LWA_Jet25Not30.txt");
  EffTableLoader effMHT(fDir + "eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt");

///////////// old tree ////////////////////////////
  TFile fin( TString(inputDirectory) + TString(inFile), "read");
   TTree* in_tree = (TTree*) fin.Get(treeName);
   Long64_t nentries = in_tree->GetEntries();
   Float_t         JetPFCor_Pt[6];
   Float_t         JetPFCor_Eta[6];
   Float_t         event_met_pfmet;
   Int_t           event_nPV;
   Int_t           evtNJ;
   Float_t         lepton_pt;
   Float_t         lepton_eta;
   Float_t         MassV2j_PFCor;
   Float_t         Mass2j_PFCor;
   Float_t         fi2_mlvjj;

   TBranch        *b_JetPFCor_Pt;   //!
   TBranch        *b_JetPFCor_Eta;   //!
   TBranch        *b_event_met_pfmet;   //!
   TBranch        *b_event_nPV;   //!
   TBranch        *b_event_nPV;
   TBranch        *b_evtNJ;
   TBranch        *b_lepton_pt;
   TBranch        *b_lepton_eta;
   TBranch        *b_MassV2j_PFCor;   //!
   TBranch        *b_Mass2j_PFCor;   //!
   TBranch        *b_fi2_mlvjj;

   in_tree->SetBranchStatus("*",    0);
   in_tree->SetBranchStatus("JetPFCor_Pt",    1);
   in_tree->SetBranchStatus("JetPFCor_Phi",    1);
   in_tree->SetBranchStatus("JetPFCor_Eta",    1);
   in_tree->SetBranchStatus("event_met_pfmet",    1);
   in_tree->SetBranchStatus("event_nPV",    1);
   in_tree->SetBranchStatus("evtNJ",    1);

   if(isElectronTree) {
      in_tree->SetBranchStatus("W_electron_pt",    1);
      in_tree->SetBranchStatus("W_electron_eta",    1);
   }
   else {
      in_tree->SetBranchStatus("W_muon_pt",    1);
      in_tree->SetBranchStatus("W_muon_eta",    1);
   }
   in_tree->SetBranchStatus("Mass2j_PFCor",    1);
   in_tree->SetBranchStatus("MassV2j_PFCor",    1);
   in_tree->SetBranchStatus("fi2_mlvjj",    1);
   in_tree->SetBranchStatus("W_mt",    1);
   in_tree->SetBranchStatus("event_runNo",    1);

   in_tree->SetBranchAddress("JetPFCor_Pt", JetPFCor_Pt, &b_JetPFCor_Pt);
   in_tree->SetBranchAddress("JetPFCor_Eta", JetPFCor_Eta, &b_JetPFCor_Eta);
   in_tree->SetBranchAddress("event_met_pfmet", &event_met_pfmet, &b_event_met_pfmet);
   in_tree->SetBranchAddress("event_nPV", &event_nPV, &b_event_nPV);
   in_tree->SetBranchAddress("evtNJ", &evtNJ, &b_evtNJ);
  if(isElectronTree) {
     in_tree->SetBranchAddress("W_electron_pt", &lepton_pt, &b_lepton_pt);
     in_tree->SetBranchAddress("W_electron_eta", &lepton_eta, &b_lepton_eta);
  }
  else {
     in_tree->SetBranchAddress("W_muon_pt", &lepton_pt, &b_lepton_pt);
     in_tree->SetBranchAddress("W_muon_eta", &lepton_eta, &b_lepton_eta);
  }
   in_tree->SetBranchAddress("Mass2j_PFCor", &Mass2j_PFCor, &b_Mass2j_PFCor);
   in_tree->SetBranchAddress("MassV2j_PFCor", &MassV2j_PFCor, &b_MassV2j_PFCor);
   in_tree->SetBranchAddress("fi2_mlvjj", &fi2_mlvjj, &b_fi2_mlvjj);

/////////// new tree /////////////////////////
   TFile fout( outputFileName, "recreate");
   TTree *newtree = in_tree->CopyTree("(evtNJ==2 || evtNJ==3) && (W_mt > 50.) && (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) && (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) && (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) && (JetPFCor_Pt[1]/Mass2j_PFCor<0.7)");
   nentries = newtree->GetEntries();
   Float_t recoSFLep,idSFLep,hltEffLep,hltEffDijet,hltEffMHT,hltEffTotal;
   TBranch *recoSFLep_branch = newtree->Branch("recoSFLep", &recoSFLep,"recoSFLep/F");
   TBranch *idSFLep_branch = newtree->Branch("idSFLep", &idSFLep,"idSFLep/F");
   TBranch *hltEffLep_branch = newtree->Branch("hltEffLep", &hltEffLep,"hltEffLep/F");
   TBranch *hltEffDijet_branch;
   TBranch *hltEffMHT_branch;
   TBranch *hltEffTotal_branch;

   if(computeDijetMHTefficiency) {
     hltEffDijet_branch = newtree->Branch("hltEffDijet", &hltEffDijet,"hltEffDijet/F");
     hltEffMHT_branch   = newtree->Branch("hltEffMHT",   &hltEffMHT,"hltEffMHT/F");
     hltEffTotal_branch = newtree->Branch("hltEffTotal", &hltEffTotal,"hltEffTotal/F");
   }
////////////////////////////////////////////////
/////// Loop over the old tree and fill new tree 
   float j1effj30, j1effj25noj30, j2effj30, j2effj25noj30, j3effj30, j3effj25noj30;

   for (Int_t i = 0; i < nentries; i++) {
    newtree->GetEntry(i);

    if(isElectronTree) {
      recoSFLep = scaleFactorEleReco.GetEfficiency( lepton_pt, lepton_eta );
      idSFLep = scaleFactorEleId.GetEfficiency( lepton_pt, lepton_eta );
      hltEffLep = effEle.GetEfficiency( lepton_pt, lepton_eta );
    }
    else {
      recoSFLep = 1.0;
      idSFLep = scaleFactorMuId.GetEfficiency( lepton_pt, lepton_eta );
      hltEffLep = effMu.GetEfficiency( lepton_pt, lepton_eta );
    }
    recoSFLep_branch->Fill();
    idSFLep_branch->Fill();
    hltEffLep_branch->Fill();


    if(computeDijetMHTefficiency) {
      j1effj30 = effJ30.GetEfficiency( JetPFCor_Pt[0], JetPFCor_Eta[0] );
      j1effj25noj30 = effJ25NoJ30.GetEfficiency( JetPFCor_Pt[0], JetPFCor_Eta[0] );
      j2effj30 = effJ30.GetEfficiency( JetPFCor_Pt[1], JetPFCor_Eta[1] );
      j2effj25noj30 = effJ25NoJ30.GetEfficiency( JetPFCor_Pt[1], JetPFCor_Eta[1] );
      j3effj30 = effJ30.GetEfficiency( JetPFCor_Pt[2], JetPFCor_Eta[2] );
      j3effj25noj30 = effJ25NoJ30.GetEfficiency( JetPFCor_Pt[2], JetPFCor_Eta[2] );
      if (evtNJ==2)
	hltEffDijet = dijetEffFor2jets( j1effj30, j1effj25noj30, j2effj30, j2effj25noj30);
      else 
	hltEffDijet = dijetEffFor3jets( j1effj30, j1effj25noj30, 
					j2effj30, j2effj25noj30, j3effj30, j3effj25noj30);

      hltEffMHT = effMHT.GetEfficiency( event_met_pfmet, 0. );
      hltEffTotal = hltEffLep * hltEffDijet * hltEffMHT;

      hltEffDijet_branch->Fill();
      hltEffMHT_branch->Fill();
      hltEffTotal_branch->Fill();
    }
  }

  fout.cd();
  newtree->Write();
  fout.Close();

  makeplots(outputFileName, isElectronTree, computeDijetMHTefficiency);
}




float dijetEffFor2jets(float eff30J1, float eff25n30J1, 
                       float eff30J2, float eff25n30J2) 
{
   return (eff30J1*eff30J2+eff30J1*eff25n30J2+eff30J2*eff25n30J1); 
}



float dijetEffFor3jets(float eff30J1, float eff25n30J1, 
                       float eff30J2, float eff25n30J2, 
                       float eff30J3, float eff25n30J3) 
{
   float T012 = (1. - eff30J1 - eff25n30J1) * eff25n30J2 * eff30J3;
   float T021 = (1. - eff30J1 - eff25n30J1) * eff30J2 * eff25n30J3;
   float T022 = (1. - eff30J1 - eff25n30J1) * eff30J2 * eff30J3;
   float T102 = eff25n30J1 * (1. - eff30J2 - eff25n30J2) * eff30J3;
   float T112 = eff25n30J1 * eff25n30J2 * eff30J3;
   float T120 = eff25n30J1 * eff30J2 * (1. - eff30J3 - eff25n30J3);
   float T121 = eff25n30J1 * eff30J2 * eff25n30J3;
   float T122 = eff25n30J1 * eff30J2 * eff30J3;
   float T201 = eff30J1 * (1. - eff30J2 - eff25n30J2) * eff25n30J3;
   float T202 = eff30J1 * (1. - eff30J2 - eff25n30J2) * eff30J3;
   float T210 = eff30J1 * eff25n30J2 * (1. - eff30J3 - eff25n30J3);
   float T211 = eff30J1 * eff25n30J2 * eff25n30J3;
   float T212 = eff30J1 * eff25n30J2 * eff30J3;
   float T220 = eff30J1 * eff30J2 * (1. - eff30J3 - eff25n30J3);
   float T221 = eff30J1 * eff30J2 * eff25n30J3;
   float T222 = eff30J1 * eff30J2 * eff30J3;

   return (T012 + T021 + T022 + T102 + T112 + T120 + T121 + 
   T122 + T201 + T202 + T210 + T211 + T212 + T220 + T221 + T222);
}




void makeplots(char* filename, 
	       bool isElectronTree, 
	       bool computeDijetMHTefficiency) {

  gROOT->ProcessLine(".L ~kalanand/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.19);
  tdrStyle->SetPadRightMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.5);



  TFile *file0 = TFile::Open(filename);
  TTree* tree = (TTree*) file0->Get("WJet");
  TCanvas* c1;
  TCanvas* c2;
  TCanvas* c3;
  TCanvas* c4;
  TCanvas* c5;
  TCanvas* c6;
  TCanvas* c7;
  TCanvas* c54body;
  char* cname;
  float integ;


  if(isElectronTree && computeDijetMHTefficiency) {
    cname = "fig_eff_HLTEle2jPfMht_total";
    c1 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffTotal:Mass2j_PFCor>>hprofejjt(40,0.,400.)","","prof");
    hprofejjt->GetYaxis()->SetRangeUser(0.3, 1.1);
    hprofejjt->GetYaxis()->SetTitle("Efficiency");
    hprofejjt->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c1->SaveAs( TString(cname) + TString(".pdf"));


    cname = "fig_eff_HLTEle2jPfMht_ele";
    c2 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffLep:Mass2j_PFCor>>hprofejje(40,0.,400.)","","prof");
    hprofejje->GetYaxis()->SetRangeUser(0.85, 1.);
    hprofejje->GetYaxis()->SetTitle("Efficiency");
    hprofejje->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c2->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTEle2jPfMht_mht";
    c3 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffMHT:Mass2j_PFCor>>hprofejjm(40,0.,400.)","","prof");
    hprofejjm->GetYaxis()->SetRangeUser(0.6, 1.1);
    hprofejjm->GetYaxis()->SetTitle("Efficiency");
    hprofejjm->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c3->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTEle2jPfMht_dijet";
    c4 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffDijet:Mass2j_PFCor>>hprofejj(40,0.,400.)","","prof");
    hprofejj->GetYaxis()->SetRangeUser(0.3, 1.1);
    hprofejj->GetYaxis()->SetTitle("Efficiency");
    hprofejj->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c4->SaveAs( TString(cname) + TString(".pdf"));


    cname = "fig_eff_HLTEle2jPfMht_template";
    c5 = new TCanvas(cname,cname,500,500);
    tree->Draw("Mass2j_PFCor>>htemp1(40,0.,400.)","","e");
    htemp1->GetXaxis()->SetTitle("m_{jj} (GeV)");
    htemp1->GetYaxis()->SetTitle("Fraction of events / 10 GeV");
    tree->Draw("Mass2j_PFCor>>htemp2","hltEffTotal", "esame");
    htemp1->SetLineColor(4);
    htemp2->SetLineColor(2);
    htemp1->SetLineWidth(3);
    htemp2->SetLineWidth(3);
    integ = htemp1->Integral();
    htemp1->Scale( 1./ integ );
    integ = htemp2->Integral();
    htemp2->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend = new TLegend(0.5,0.75,0.88,0.88);
    legend->SetName("legend");
    legend->SetHeader("Efficiency correction");
    legend->AddEntry( htemp1, "Before", "le");  
    legend->AddEntry( htemp2, "After", "le");
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->Draw();
    c5->SaveAs( TString(cname) + TString(".pdf"));


    cname = "fig_eff_HLTEle2jPfMht_template4body";
    c54body = new TCanvas(cname,cname,500,500);
    tree->Draw("MassV2j_PFCor>>htemp14body(50,0.,1000.)","","e");
    htemp14body->GetXaxis()->SetTitle("m_{l#nujj} (GeV)");
    htemp14body->GetXaxis()->SetNdivisions(505);
    htemp14body->GetYaxis()->SetTitle("Fraction of events / 20 GeV");
    tree->Draw("MassV2j_PFCor>>htemp24body","hltEffTotal", "esame");
    htemp14body->SetLineColor(4);
    htemp24body->SetLineColor(2);
    htemp14body->SetLineWidth(3);
    htemp24body->SetLineWidth(3);
    integ = htemp14body->Integral();
    htemp14body->Scale( 1./ integ );
    integ = htemp24body->Integral();
    htemp24body->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend4body = new TLegend(0.5,0.75,0.88,0.88);
    legend4body->SetName("legend");
    legend4body->SetHeader("Efficiency correction");
    legend4body->AddEntry( htemp14body, "Before", "le");  
    legend4body->AddEntry( htemp24body, "After", "le");
    legend4body->SetFillColor(0);
    legend4body->SetFillStyle(0);
    legend4body->Draw();
    c54body->SaveAs( TString(cname) + TString(".pdf"));

  }
  else if(isElectronTree) {
    cname = "fig_eff_HLTEle27_May10ReReco";
    c1 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffLep:Mass2j_PFCor>>hprofelhlt(40,0.,400.)","","prof");
    hprofelhlt->GetYaxis()->SetRangeUser(0.8, 1.);
    hprofelhlt->GetYaxis()->SetTitle("Efficiency");
    hprofelhlt->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c1->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTEle27_May10ReReco_template";
    c5 = new TCanvas(cname,cname,500,500);
    tree->Draw("Mass2j_PFCor>>htemp3(40,0.,400.)","","e");
    htemp3->GetXaxis()->SetTitle("m_{jj} (GeV)");
    htemp3->GetYaxis()->SetTitle("Fraction of events / 10 GeV");
    tree->Draw("Mass2j_PFCor>>htemp4","hltEffLep", "esame");
    htemp3->SetLineColor(4);
    htemp4->SetLineColor(2);
    htemp3->SetLineWidth(3);
    htemp4->SetLineWidth(3);
    integ = htemp3->Integral();
    htemp3->Scale( 1./  integ);
    integ = htemp4->Integral();
    htemp4->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend = new TLegend(0.5,0.75,0.88,0.88);
    legend->SetName("legend");
    legend->SetHeader("Efficiency correction");
    legend->AddEntry( htemp3, "Before", "le");  
    legend->AddEntry( htemp4, "After", "le");
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->Draw();
    c5->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTEle27_May10ReReco_template4body";
    c54body = new TCanvas(cname,cname,500,500);
    tree->Draw("MassV2j_PFCor>>htemp34body(50,0.,1000.)","","e");
    htemp34body->GetXaxis()->SetTitle("m_{l#nujj} (GeV)");
    htemp34body->GetXaxis()->SetNdivisions(505);
    htemp34body->GetYaxis()->SetTitle("Fraction of events / 20 GeV");
    tree->Draw("MassV2j_PFCor>>htemp44body","hltEffLep", "esame");
    htemp34body->SetLineColor(4);
    htemp44body->SetLineColor(2);
    htemp34body->SetLineWidth(3);
    htemp44body->SetLineWidth(3);
    integ = htemp34body->Integral();
    htemp34body->Scale( 1./ integ );
    integ = htemp44body->Integral();
    htemp44body->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend4body = new TLegend(0.5,0.75,0.88,0.88);
    legend4body->SetName("legend");
    legend4body->SetHeader("Efficiency correction");
    legend4body->AddEntry( htemp34body, "Before", "le");  
    legend4body->AddEntry( htemp44body, "After", "le");
    legend4body->SetFillColor(0);
    legend4body->SetFillStyle(0);
    legend4body->Draw();
    c54body->SaveAs( TString(cname) + TString(".pdf"));

  }
  else {
    cname = "fig_eff_HLTMu";
    c1 = new TCanvas(cname,cname,500,500);
    tree->Draw("hltEffLep:Mass2j_PFCor>>hprofmu(40,0.,400.)","","prof");
    hprofmu->GetYaxis()->SetRangeUser(0.95, 1.0);
    hprofmu->GetYaxis()->SetTitle("Efficiency");
    hprofmu->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c1->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTMu_template";
    c5 = new TCanvas(cname,cname,500,500);
    tree->Draw("Mass2j_PFCor>>htemp5(40,0.,400.)","","e");
    htemp5->GetXaxis()->SetTitle("m_{jj} (GeV)");
    htemp5->GetYaxis()->SetTitle("Fraction of events / 10 GeV");
    tree->Draw("Mass2j_PFCor>>htemp6","hltEffLep", "esame");
    htemp5->SetLineColor(4);
    htemp5->SetLineWidth(3);
    htemp6->SetLineColor(2);
    htemp6->SetLineWidth(3);
    integ = htemp5->Integral();
    htemp5->Scale( 1./ integ );
    integ = htemp6->Integral();
    htemp6->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend = new TLegend(0.5,0.75,0.88,0.88);
    legend->SetName("legend");
    legend->SetHeader("Efficiency correction");
    legend->AddEntry( htemp5, "Before", "le");  
    legend->AddEntry( htemp6, "After", "le");
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->Draw();
    c5->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_HLTMu_template4body";
    c54body = new TCanvas(cname,cname,500,500);
    tree->Draw("MassV2j_PFCor>>htemp54body(50,0.,1000.)","","e");
    htemp54body->GetXaxis()->SetTitle("m_{l#nujj} (GeV)");
    htemp54body->GetXaxis()->SetNdivisions(505);
    htemp54body->GetYaxis()->SetTitle("Fraction of events / 20 GeV");
    tree->Draw("MassV2j_PFCor>>htemp64body","hltEffLep", "esame");
    htemp54body->SetLineColor(4);
    htemp64body->SetLineColor(2);
    htemp54body->SetLineWidth(3);
    htemp64body->SetLineWidth(3);
    integ = htemp54body->Integral();
    htemp54body->Scale( 1./ integ );
    integ = htemp64body->Integral();
    htemp64body->Scale( 1./ integ );
    gPad->Modified();
    TLegend* legend4body = new TLegend(0.5,0.75,0.88,0.88);
    legend4body->SetName("legend");
    legend4body->SetHeader("Efficiency correction");
    legend4body->AddEntry( htemp54body, "Before", "le");  
    legend4body->AddEntry( htemp64body, "After", "le");
    legend4body->SetFillColor(0);
    legend4body->SetFillStyle(0);
    legend4body->Draw();
    c54body->SaveAs( TString(cname) + TString(".pdf"));
  }

  /////// ------------------ plot reco efficiency scale factors ---------
  if(isElectronTree) {
    cname = "fig_eff_ele_SCToReco_ScaleFactors";
    c6 = new TCanvas(cname,cname,500,500);
    tree->Draw("recoSFLep:Mass2j_PFCor>>hprofel(40,0.,400.)","","prof");
    hprofel->GetYaxis()->SetRangeUser(0.95, 1.05);
    hprofel->GetYaxis()->SetTitle("Efficiency scale factor");
    hprofel->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c6->SaveAs( TString(cname) + TString(".pdf"));

    cname = "fig_eff_ele_RecoToID_ScaleFactors";
    c7 = new TCanvas(cname,cname,500,500);
    tree->Draw("idSFLep:Mass2j_PFCor>>htemp(40,0.,400.)","","prof");
    htemp->GetYaxis()->SetRangeUser(0.95, 1.03);
    htemp->GetYaxis()->SetTitle("Efficiency scale factor");
    htemp->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c7->SaveAs( TString(cname) + TString(".pdf"));
  }
  else {
    cname = "fig_eff_mu_RecoToIso_ScaleFactors";
    c6 = new TCanvas(cname,cname,500,500);
    tree->Draw("idSFLep:Mass2j_PFCor>>htempMuReco(40,0.,400.)","","prof");
    htempMuReco->GetYaxis()->SetRangeUser(0.99, 1.01);
    htempMuReco->GetYaxis()->SetTitle("Efficiency scale factor");
    htempMuReco->GetXaxis()->SetTitle("m_{jj} (GeV)");
    c6->SaveAs( TString(cname) + TString(".pdf"));
  }
}
