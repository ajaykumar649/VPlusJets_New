/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill W/Z + jets related quantities into a specified TTree
 *   Can work with CaloJet, GenJet, JPT jet, PF jet.
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef VplusJetsAnalysis_h
#define VplusJetsAnalysis_h

// system include files
#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h" 
#include "TFile.h"
#include "TTree.h"

#include "ElectroWeakAnalysis/VPlusJets/interface/JetTreeFiller.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/VtoElectronTreeFiller.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/MCTreeFiller.h"

//
// class decleration
//
namespace ewk
{
  class VplusJetsAnalysis : public edm::EDAnalyzer {
  public:
    explicit VplusJetsAnalysis(const edm::ParameterSet&);
    ~VplusJetsAnalysis();

    virtual void analyze(const edm::Event&, 
			 const edm::EventSetup& iSetup);
    virtual void beginJob() ;
    virtual void endJob() ;
    virtual void declareTreeBranches();


  private:
    // ----------member data ---------------------------
    // names of modules, producing object collections
     
    /// output ROOT file for the tree and histograms
    std::string fOutputFileName ;
    TFile*  hOutputFile ;
    TTree*  myTree;
    bool runningOverMC_;

    /// The objects that actually computes variables and fill the tree 
    std::auto_ptr<ewk::JetTreeFiller> CaloJetFiller;
    std::auto_ptr<ewk::JetTreeFiller> CorrectedCaloJetFiller;
    std::auto_ptr<ewk::JetTreeFiller> GenJetFiller;
    std::auto_ptr<ewk::JetTreeFiller> PFJetFiller; 
    std::auto_ptr<ewk::JetTreeFiller> JPTJetFiller;
    std::auto_ptr<ewk::VtoElectronTreeFiller> recoBosonFiller;
    std::auto_ptr<ewk::MCTreeFiller> genBosonFiller;

    // private data members
    int run;
    int event; 
    int lumi; 
    int bunch; 
    int nPV; 
    float mPVx[20];
    float mPVy[20];
    float mPVz[20];
    float mMET;
    float mSumET;
    float mMETnoHF;
    float mSumETnoHF;
  };
}
#endif
