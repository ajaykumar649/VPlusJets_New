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
 *   To fill jet related quantities into a specified TTree
 *   Can work with CaloJet, GenJet, JPT jet, PF jet.
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef ElectroWeakAnalysis_VPlusJets_JetTreeFiller_h
#define ElectroWeakAnalysis_VPlusJets_JetTreeFiller_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "RecoJets/JetProducers/interface/JetIDHelper.h"



/** Monte Carlo Jet flavor classification.
    if ( Flavour == 1 ) : Down 
    if ( Flavour == 2 ) : Up 
    if ( Flavour == 3 ) : Strange 
    if ( Flavour == 4 ) : Charm 
    if ( Flavour == 5 ) : Bottom 
    if ( Flavour == 21 ): Gluon
*/



namespace ewk {

  class JetTreeFiller {
  public:
    /// specify the name of the TTree, and the configuration for it
    JetTreeFiller(const char *name, TTree* tree, 
		  const std::string jetType,
		  const edm::ParameterSet iConfig );


    /// default constructor
    JetTreeFiller() {};


    /// Destructor, does nothing 
      ~JetTreeFiller() {};


    /// To be called once per event to fill the values for jets
    void fill(const edm::Event &iEvent);
    void fill_jet_ID_struct();

    static const int NUM_ALGO_MAX = 3;
    static const int NUM_JET_MAX = 10;

  protected:

    /// To be called once per event, to initialize variable values to default
    void init() const ;
    /// Helper function for main constructor
    void SetBranches(); 
    void SetBranch( float* x, std::string name);
    void SetBranch( int* x, std::string name);
    void FillBranches() const;
    void init();

    TTree* tree_;
    std::string jetType_;
    std::vector< edm::InputTag > mInputJets;
    edm::InputTag mInputBoson;
    std::vector< edm::InputTag >  sourceByValue;
    // 'mutable' because we will fill it from a 'const' method
    mutable std::vector<std::string> bnames;
    // JetID helper
    reco::helper::JetIDHelper jet_ID_helper_;
    reco::JetID jet_ID_struct_;

    bool doJetFlavorIdentification;


  private:
    // private data members
    
    int NumJetAlgo;
    int NumJets[NUM_ALGO_MAX]; 
    int passingLoose[NUM_ALGO_MAX][NUM_JET_MAX];
    int passingMedium[NUM_ALGO_MAX][NUM_JET_MAX];
    int passingTight[NUM_ALGO_MAX][NUM_JET_MAX];
    float Et[NUM_ALGO_MAX][NUM_JET_MAX];
    float Pt[NUM_ALGO_MAX][NUM_JET_MAX];
    float Eta[NUM_ALGO_MAX][NUM_JET_MAX];
    float Phi[NUM_ALGO_MAX][NUM_JET_MAX];
    float Theta[NUM_ALGO_MAX][NUM_JET_MAX];
    float E[NUM_ALGO_MAX][NUM_JET_MAX];
    float Y[NUM_ALGO_MAX][NUM_JET_MAX];
    float Px[NUM_ALGO_MAX][NUM_JET_MAX];
    float Py[NUM_ALGO_MAX][NUM_JET_MAX];
    float Pz[NUM_ALGO_MAX][NUM_JET_MAX];
    int   Flavor[NUM_ALGO_MAX][NUM_JET_MAX];
    float MaxEInEmTowers[NUM_ALGO_MAX][NUM_JET_MAX];
    float MaxEInHadTowers[NUM_ALGO_MAX][NUM_JET_MAX];
    float EnergyFractionHadronic[NUM_ALGO_MAX][NUM_JET_MAX];
    float EmEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float HadEnergyInHB[NUM_ALGO_MAX][NUM_JET_MAX];
    float HadEnergyInHO[NUM_ALGO_MAX][NUM_JET_MAX];
    float HadEnergyInHE[NUM_ALGO_MAX][NUM_JET_MAX];
    float HadEnergyInHF[NUM_ALGO_MAX][NUM_JET_MAX];
    float EmEnergyInEB[NUM_ALGO_MAX][NUM_JET_MAX];
    float EmEnergyInEE[NUM_ALGO_MAX][NUM_JET_MAX];
    float EmEnergyInHF[NUM_ALGO_MAX][NUM_JET_MAX];
    float TowersArea[NUM_ALGO_MAX][NUM_JET_MAX];
    float VjetMass[NUM_ALGO_MAX][NUM_JET_MAX];
    int   N90[NUM_ALGO_MAX][NUM_JET_MAX]; 
    int   N60[NUM_ALGO_MAX][NUM_JET_MAX];     
    float Dphi[NUM_ALGO_MAX][NUM_JET_MAX];
    float Deta[NUM_ALGO_MAX][NUM_JET_MAX];
    float DR[NUM_ALGO_MAX][NUM_JET_MAX];
    float Response[NUM_ALGO_MAX][NUM_JET_MAX];
    float bDiscriminator[NUM_ALGO_MAX][NUM_JET_MAX];
    float secVertexMass[NUM_ALGO_MAX][NUM_JET_MAX];
    int numBTags[NUM_ALGO_MAX];


    float GenEmEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float GenHadEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float GenInvisibleEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float GenAuxiliaryEnergy[NUM_ALGO_MAX][NUM_JET_MAX];

    float PFChargedHadronEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFChargedHadronEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFNeutralHadronEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFNeutralHadronEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFChargedEmEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFChargedEmEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFChargedMuEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFChargedMuEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFNeutralEmEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float PFNeutralEmEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    int   PFChargedMultiplicity[NUM_ALGO_MAX][NUM_JET_MAX];
    int   PFNeutralMultiplicity[NUM_ALGO_MAX][NUM_JET_MAX];
    int   PFMuonMultiplicity[NUM_ALGO_MAX][NUM_JET_MAX];

  };

} //namespace

#endif


