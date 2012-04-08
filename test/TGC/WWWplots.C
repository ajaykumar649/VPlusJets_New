/* ******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *
 * Implementation details:
 *  - Uses standard ROOT classes.
 *  - Script plots the distribution of WWW->lnu+4jets events
 *
 * History:
 *  
 *
 * Copyright (C) 2012 FNAL 
 ******************************************************************* */


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <stdio.h>
#include <string.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCut.h>
#include <TStyle.h>
#include <TString.h> 
#include <TLegend.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <iostream>
#include "TLorentzVector.h"

using namespace std; 


const string inputDirPath = "/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree_PAT/RD_";




const double minMass = 40.;
const double maxMass = 200.;
const int nbins = 30;
const double intLumi = 5020.;
const double WJets_scale = 31500.* intLumi/81000000;
const double TT_scale = 163. * intLumi/3701947;
const double WV_scale = 61.2 * intLumi/4225916;
/////////// define all member functions here first ////////////////

void ChooseHadronicWCandidates(TTree* in_tree, TH1D* W1,  TH1D* W2);
void cmsPrelim();



void WWWplots()
{


  //TCut* mycuts = new TCut("(ggdevt == 4) && (W_mt > 50) && (JetPFCor_Pt[4]<30.) && (numPFCorJetBTags==0)");

  /// Get Data ///////////
  TFile *file0 = TFile::Open((inputDirPath+"WmunuJets_DataAll_GoldenJSON_4p7invfb.root").c_str(), "read");
  TTree* tree0 = (TTree*) file0->Get("WJet");
  TFile *file1 = TFile::Open((inputDirPath+"WenuJets_DataAllSingleElectronTrigger_GoldenJSON_4p7invfb.root").c_str(), "read");
  TTree* tree1 = (TTree*) file1->Get("WJet");


  /// Get W+jets MC ///////////
  TFile *file11 = TFile::Open((inputDirPath+"mu_WpJ_CMSSW428.root").c_str(), "read");
  TTree* tree11 = (TTree*) file11->Get("WJet");
  TFile *file12 = TFile::Open((inputDirPath+"el_WpJ_CMSSW428.root").c_str(), "read");
  TTree* tree12 = (TTree*) file12->Get("WJet");


  /// Get ttbar MC ///////////
  TFile *file21 = TFile::Open((inputDirPath+"mu_TTbar_CMSSW428.root").c_str(), "read");
  TTree* tree21 = (TTree*) file21->Get("WJet");
  TFile *file22 = TFile::Open((inputDirPath+"el_TTbar_CMSSW428.root").c_str(), "read");
  TTree* tree22 = (TTree*) file22->Get("WJet");



  /// Get WW MC ///////////
  TFile *file31 = TFile::Open((inputDirPath+"mu_WW_CMSSW428.root").c_str(), "read");
  TTree* tree31 = (TTree*) file31->Get("WJet");
  TFile *file32 = TFile::Open((inputDirPath+"el_WW_CMSSW428.root").c_str(), "read");
  TTree* tree32 = (TTree*) file32->Get("WJet");


  /// Get single top MC ///////////
  TFile *file41 = TFile::Open((inputDirPath+"mu_STopT_T_CMSSW428.root").c_str(), "read");
  TTree* tree41 = (TTree*) file41->Get("WJet");
  TFile *file42 = TFile::Open((inputDirPath+"el_STopT_T_CMSSW428.root").c_str(), "read");
  TTree* tree42 = (TTree*) file42->Get("WJet");





  //// Define all the histograms for W1
  TH1D* h_jjmass1 = new TH1D("h_jjmass1","", nbins, minMass, maxMass);
  TH1D* h_jjmass1_WJets = new TH1D("h_jjmass1_WJets","", nbins, minMass, maxMass);
  TH1D* h_jjmass1_TT = new TH1D("h_jjmass1_TT","", nbins, minMass, maxMass);
  TH1D* h_jjmass1_ST = (TH1D*) h_jjmass1_TT->Clone("h_jjmass1_ST");
  TH1D* h_jjmass2_ST = (TH1D*) h_jjmass1_TT->Clone("h_jjmass2_ST");
  TH1D* h_jjmass1_WV = new TH1D("h_jjmass1_WV","", nbins, minMass, maxMass);
  h_jjmass1->GetXaxis()->SetTitle("m_{jj} (GeV)");
  char tit[50];
  sprintf(tit,"Events / %d GeV", (int) (maxMass-minMass)/nbins );
  h_jjmass1->GetYaxis()->SetTitle(tit);
  h_jjmass1_WJets->SetLineColor(2);
  h_jjmass1_WJets->SetFillColor(2);
  h_jjmass1_TT->SetLineColor(8);
  h_jjmass1_TT->SetFillColor(8);
  h_jjmass1_WV->SetLineColor(5);
  h_jjmass1_WV->SetFillColor(5);



  //// Define all the histograms for W2
  TH1D* h_jjmass2 = new TH1D("h_jjmass2","", nbins, minMass, maxMass);
  TH1D* h_jjmass2_WJets = new TH1D("h_jjmass2_WJets","", nbins, minMass, maxMass);
  TH1D* h_jjmass2_TT = new TH1D("h_jjmass2_TT","", nbins, minMass, maxMass);
  TH1D* h_jjmass2_WV = new TH1D("h_jjmass2_WV","", nbins, minMass, maxMass);
  h_jjmass2->GetXaxis()->SetTitle("m_{jj} (GeV)");
  h_jjmass2->GetYaxis()->SetTitle(tit);
  h_jjmass2_WJets->SetLineColor(2);
  h_jjmass2_WJets->SetFillColor(2);
  h_jjmass2_TT->SetLineColor(8);
  h_jjmass2_TT->SetFillColor(8);
  h_jjmass2_WV->SetLineColor(5);
  h_jjmass2_WV->SetFillColor(5);



  //// Fill all the histograms 
  ChooseHadronicWCandidates(tree0, h_jjmass1, h_jjmass2);
  ChooseHadronicWCandidates(tree1, h_jjmass1, h_jjmass2);
  ChooseHadronicWCandidates(tree11, h_jjmass1_WJets, h_jjmass2_WJets);
  ChooseHadronicWCandidates(tree12, h_jjmass1_WJets, h_jjmass2_WJets);
  ChooseHadronicWCandidates(tree21, h_jjmass1_TT, h_jjmass2_TT);
  ChooseHadronicWCandidates(tree22, h_jjmass1_TT, h_jjmass2_TT);
  ChooseHadronicWCandidates(tree31, h_jjmass1_WV, h_jjmass2_WV);
  ChooseHadronicWCandidates(tree32, h_jjmass1_WV, h_jjmass2_WV);
  ChooseHadronicWCandidates(tree41, h_jjmass1_ST, h_jjmass2_ST); 
  ChooseHadronicWCandidates(tree42, h_jjmass1_ST, h_jjmass2_ST); 


  ///// Scale the MC histograms by cross section x luminosity 
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetOptStat(0);
  h_jjmass1_WJets->Scale( WJets_scale );
  h_jjmass1_TT->Scale( TT_scale );
  h_jjmass1_WV->Scale( WV_scale );

  h_jjmass2_WJets->Scale( WJets_scale );
  h_jjmass2_TT->Scale( TT_scale );
  h_jjmass2_WV->Scale( WV_scale );
  h_jjmass1_ST->Scale( 84.94/3900171);
  h_jjmass2_ST->Scale( 84.94/3900171);
  h_jjmass1_TT->Add( h_jjmass1_ST);
  h_jjmass2_TT->Add( h_jjmass2_ST);



  /////// Signal Histogram: cross section = (0.22+0.1)/2 pb
  const double xsec_eff_acc_lumi = (0.22*0.1)*0.5*0.2*0.5*0.6*intLumi;
  TH1D* h_jjmass1_signal = (TH1D*) h_jjmass1_WV->Clone("h_jjmass_WV");
  h_jjmass1_signal->SetLineColor(4);
  h_jjmass1_signal->SetFillColor(4);
  h_jjmass1_signal->Scale( xsec_eff_acc_lumi /  h_jjmass1_signal->Integral() );
  TH1D* h_jjmass2_signal = (TH1D*) h_jjmass2_WV->Clone("h_jjmass_WV");
  h_jjmass2_signal->SetLineColor(4);
  h_jjmass2_signal->SetFillColor(4);
  h_jjmass2_signal->Scale( xsec_eff_acc_lumi /  h_jjmass2_signal->Integral() );



  TCanvas* c_jjmass1 = new TCanvas("c_jjmass1","jj mass 1", 500, 500);
  h_jjmass1_WJets->Add( h_jjmass1_WV );
  h_jjmass1_TT->Add(h_jjmass1_WJets);
  h_jjmass1_signal->Add( h_jjmass1_TT );
  h_jjmass1->SetMaximum( 1.6 * h_jjmass1->GetMaximum() );
  h_jjmass1->Draw("e");
  h_jjmass1_signal->Draw("same");
  h_jjmass1_TT->Draw("same");
  h_jjmass1_WJets->Draw("same");
  h_jjmass1_WV->Draw("same");
  h_jjmass1->Draw("esame");
  TLegend* legend1 = new TLegend(0.55,0.7,0.89,0.89);
  legend1->SetFillColor(0);
  legend1->AddEntry(h_jjmass1, "Data", "PLE");  
  legend1->AddEntry(h_jjmass1_TT, "t#bar{t}+top", "F");
  legend1->AddEntry(h_jjmass1_WJets, "W+jets", "F");
  legend1->AddEntry(h_jjmass1_WV, "WV+jets", "F");
  legend1->AddEntry(h_jjmass1_signal, "WVV", "F");
  legend1->Draw();
  cmsPrelim();
  c_jjmass1->SaveAs("jjmass1.png");



  TCanvas* c_jjmass2 = new TCanvas("c_jjmass2","jj mass 2", 500, 500);
  h_jjmass2_WJets->Add( h_jjmass2_WV );
  h_jjmass2_TT->Add(h_jjmass2_WJets);
  h_jjmass2_signal->Add( h_jjmass2_TT );
  h_jjmass2->SetMaximum( 1.6 * h_jjmass2->GetMaximum() );
  h_jjmass2->Draw("e");
  h_jjmass2_signal->Draw("same");
  h_jjmass2_TT->Draw("same");
  h_jjmass2_WJets->Draw("same");
  h_jjmass2_WV->Draw("same");
  h_jjmass2->Draw("esame");
  TLegend* legend2 = new TLegend(0.55,0.7,0.89,0.89);
  legend2->SetFillColor(0);
  legend2->AddEntry(h_jjmass2, "Data", "PLE");  
  legend2->AddEntry(h_jjmass2_TT, "t#bar{t}+top", "F");
  legend2->AddEntry(h_jjmass2_WJets, "W+jets", "F");
  legend2->AddEntry(h_jjmass2_WV, "WV+jets", "F");
  legend2->AddEntry(h_jjmass2_signal, "WVV", "F");
  legend2->Draw();
  cmsPrelim();
  c_jjmass2->SaveAs("jjmass2.png");



  //// Add W1 & W2
  TH1D* h_jjmass = (TH1D*) h_jjmass1->Clone("h_jjmass");
  TH1D* h_jjmass_WJets = (TH1D*) h_jjmass1_WJets->Clone("h_jjmass_WJets");
  TH1D* h_jjmass_TT = (TH1D*) h_jjmass1_TT->Clone("h_jjmass_TT");
  TH1D* h_jjmass_WV = (TH1D*) h_jjmass1_WV->Clone("h_jjmass_WV");
  TH1D* h_jjmass_signal = (TH1D*) h_jjmass1_signal->Clone("h_jjmass_signal");
  h_jjmass->Add(h_jjmass2);
  h_jjmass_WJets->Add(h_jjmass2_WJets);
  h_jjmass_TT->Add(h_jjmass2_TT);
  h_jjmass_WV->Add(h_jjmass2_WV);
  h_jjmass_signal->Add(h_jjmass2_signal);


  TCanvas* c_jjmass = new TCanvas("c_jjmass","jj mass both", 500, 500);
  h_jjmass->SetMaximum( 1.5 * h_jjmass->GetMaximum() );
  h_jjmass->Draw("e");
  h_jjmass_signal->Draw("same");
  h_jjmass_TT->Draw("same");
  h_jjmass_WJets->Draw("same");
  h_jjmass->Draw("esame");
  h_jjmass_WV->Draw("same");
  TLegend* legend = new TLegend(0.55,0.7,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_jjmass, "Data", "PLE");  
  legend->AddEntry(h_jjmass_TT, "t#bar{t}+top", "F");
  legend->AddEntry(h_jjmass_WJets, "W+jets", "F");
  legend->AddEntry(h_jjmass_WV, "WV+jets", "F");
  legend->AddEntry(h_jjmass_signal, "WVV", "F");
  legend->Draw();
  cmsPrelim();
  c_jjmass->SaveAs("jjmass.png");

}






void ChooseHadronicWCandidates(TTree* in_tree, TH1D* W1,  TH1D* W2)
{


  Float_t         JetPFCor_Pt[6];
  Float_t         JetPFCor_Eta[6];
  Float_t         JetPFCor_Phi[6];
  Float_t         JetPFCor_E[6];
  Float_t         qgld_Summer11CHS[6];
  Float_t         W_mt;
  Int_t           ggdevt;
  Int_t           numPFCorJetBTags;



  TBranch        *b_JetPFCor_Pt;   //!
  TBranch        *b_JetPFCor_Eta;   //!
  TBranch        *b_JetPFCor_Phi;   //!
  TBranch        *b_JetPFCor_E;   //!
  TBranch        *b_qgld_Summer11CHS;   //!
  TBranch        *b_W_mt;   //!
  TBranch        *b_ggdevt;   //!
  TBranch        *b_numPFCorJetBTags;   //!


  in_tree->SetBranchAddress("JetPFCor_Pt", JetPFCor_Pt, &b_JetPFCor_Pt);
  in_tree->SetBranchAddress("JetPFCor_Eta", JetPFCor_Eta, &b_JetPFCor_Eta);
  in_tree->SetBranchAddress("JetPFCor_Phi", JetPFCor_Phi, &b_JetPFCor_Phi);
  in_tree->SetBranchAddress("JetPFCor_E", JetPFCor_E, &b_JetPFCor_E);
  in_tree->SetBranchAddress("qgld_Summer11CHS", qgld_Summer11CHS, &b_qgld_Summer11CHS);

  in_tree->SetBranchAddress("W_mt", &W_mt, &b_W_mt);
  in_tree->SetBranchAddress("ggdevt", &ggdevt, &b_ggdevt);
  in_tree->SetBranchAddress("numPFCorJetBTags", &numPFCorJetBTags, &b_numPFCorJetBTags);


  Long64_t nentries = in_tree->GetEntries();

  TLorentzVector jet[4]; 

  for (Int_t i = 0; i < nentries; i++) {
    in_tree->GetEntry(i);
    if( !(ggdevt==4 && W_mt>50 && JetPFCor_Pt[4]<30. && numPFCorJetBTags==0) ) continue;
    // if( !(ggdevt==4 && W_mt>50 && JetPFCor_Pt[4]<30.) ) continue;

    if(!(qgld_Summer11CHS[0]>0.1 && qgld_Summer11CHS[1]>0.1 
	 && qgld_Summer11CHS[2]>0.1 && qgld_Summer11CHS[3]>0.1)) continue;


    //// first fill up all the LorentzVectors
    for (Int_t ijet = 0; ijet < 4; ijet++) {
      jet[ijet].SetPtEtaPhiE(JetPFCor_Pt[ijet], JetPFCor_Eta[ijet], JetPFCor_Phi[ijet], JetPFCor_E[ijet]  );
    }



    //// Now define mass 
    double mass[6];
    double dijetPt[6];
    const double maxDeta = 1.5;
    for (Int_t inum = 0; inum < 6; inum++) { mass[inum]=0.; dijetPt[inum]=0.; }
    if( fabs(JetPFCor_Eta[0]-JetPFCor_Eta[1])< maxDeta) { 
      mass[0] = (jet[0]+jet[1]).M();
      dijetPt[0] = (jet[0]+jet[1]).Pt();
    }
    if( fabs(JetPFCor_Eta[0]-JetPFCor_Eta[2])< maxDeta) { 
      mass[1] = (jet[0]+jet[2]).M();
      dijetPt[1] = (jet[0]+jet[2]).Pt();
    }
    if( fabs(JetPFCor_Eta[0]-JetPFCor_Eta[3])< maxDeta) {  
      mass[2] = (jet[0]+jet[3]).M();
      dijetPt[2] = (jet[0]+jet[3]).Pt();
    }
    if( fabs(JetPFCor_Eta[1]-JetPFCor_Eta[2])< maxDeta) { 
       mass[3] = (jet[1]+jet[2]).M();
       dijetPt[3] = (jet[1]+jet[2]).Pt();
    }
    if( fabs(JetPFCor_Eta[1]-JetPFCor_Eta[3])< maxDeta) { 
       mass[4] = (jet[1]+jet[3]).M();
       dijetPt[4] = (jet[1]+jet[3]).Pt();
    }
    if( fabs(JetPFCor_Eta[2]-JetPFCor_Eta[3])< maxDeta) { 
       mass[5] = (jet[2]+jet[3]).M();
       dijetPt[5] = (jet[2]+jet[3]).Pt();
    }





    /// Find the pair of indices with the smallest mass difference
    double dmass = 9999999999.;
    double tempdmass = 9999999999.;
    int firstInd=-1, secondInd = -1;

    for (Int_t inum = 0; inum < 6; inum++) {
      if(dijetPt[inum]<40.) continue;
      for (Int_t jnum = inum; jnum < 6; jnum++) {
	if(inum==jnum ) continue;
	//if(dijetPt[jnum]<40.) continue;
	tempdmass = fabs( mass[inum] - mass[jnum] );
	if(tempdmass<dmass) { dmass=tempdmass;  firstInd=inum;  secondInd=jnum;}
      }
    }


    if(firstInd>-1 && secondInd>-1 && dmass<2.1) {
      if(dijetPt[firstInd]>dijetPt[secondInd]) { 
	W1->Fill( mass[firstInd] );
	W2->Fill( mass[secondInd] ); 
      }
      else {
	W2->Fill( mass[firstInd] );
	W1->Fill( mass[secondInd] ); 
      }
    }// check index and mass difference tolerance
    else {
      W1->Fill( 0.);
      W2->Fill( 0.);
    }
  }
}



////CMS Preliminary label and lumi
void cmsPrelim()
{
  const float LUMINOSITY = intLumi;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(31); // align right
    latex.DrawLatex(0.85,0.55,Form("#int #font[12]{L} dt = %.1f fb^{-1}",
    LUMINOSITY/1000.));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary");
}
