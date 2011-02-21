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

#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"



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

    static const int NUM_JET_MAX = 6;

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
    edm::InputTag mInputJets;
    edm::InputTag mInputBoson;
    edm::InputTag sourceByValue;
    // 'mutable' because we will fill it from a 'const' method
    mutable std::vector<std::string> bnames;

    bool doJetFlavorIdentification;


  private:
    // private data members
    
    int NumJets; 
    float Et[NUM_JET_MAX];
    float Pt[NUM_JET_MAX];
    float Eta[NUM_JET_MAX];
    float Phi[NUM_JET_MAX];
    float Theta[NUM_JET_MAX];
    float E[NUM_JET_MAX];
    float Y[NUM_JET_MAX];
    float Px[NUM_JET_MAX];
    float Py[NUM_JET_MAX];
    float Pz[NUM_JET_MAX];
    int   Flavor[NUM_JET_MAX];
    float MaxEInEmTowers[NUM_JET_MAX];
    float MaxEInHadTowers[NUM_JET_MAX];
    float EnergyFractionHadronic[NUM_JET_MAX];
    float EmEnergyFraction[NUM_JET_MAX];
    float HadEnergyInHB[NUM_JET_MAX];
    float HadEnergyInHO[NUM_JET_MAX];
    float HadEnergyInHE[NUM_JET_MAX];
    float HadEnergyInHF[NUM_JET_MAX];
    float EmEnergyInEB[NUM_JET_MAX];
    float EmEnergyInEE[NUM_JET_MAX];
    float EmEnergyInHF[NUM_JET_MAX];
    float TowersArea[NUM_JET_MAX];
    float VjetMass[NUM_JET_MAX];
    int   N90[NUM_JET_MAX]; 
    int   N60[NUM_JET_MAX];     
    float Dphi[NUM_JET_MAX];
    float Deta[NUM_JET_MAX];
    float DR[NUM_JET_MAX];
    float DphiMET[NUM_JET_MAX];
    float Response[NUM_JET_MAX];
    float bDiscriminator[NUM_JET_MAX];
    float secVertexMass[NUM_JET_MAX];
    int numBTags;


    float GenEmEnergy[NUM_JET_MAX];
    float GenHadEnergy[NUM_JET_MAX];
    float GenInvisibleEnergy[NUM_JET_MAX];
    float GenAuxiliaryEnergy[NUM_JET_MAX];

    float PFChargedHadronEnergy[NUM_JET_MAX];
    float PFChargedHadronEnergyFraction[NUM_JET_MAX];
    float PFNeutralHadronEnergy[NUM_JET_MAX];
    float PFNeutralHadronEnergyFraction[NUM_JET_MAX];
    float PFChargedEmEnergy[NUM_JET_MAX];
    float PFChargedEmEnergyFraction[NUM_JET_MAX];
    float PFChargedMuEnergy[NUM_JET_MAX];
    float PFChargedMuEnergyFraction[NUM_JET_MAX];
    float PFNeutralEmEnergy[NUM_JET_MAX];
    float PFNeutralEmEnergyFraction[NUM_JET_MAX];
    int   PFChargedMultiplicity[NUM_JET_MAX];
    int   PFNeutralMultiplicity[NUM_JET_MAX];
    int   PFMuonMultiplicity[NUM_JET_MAX];

  };

} //namespace

#endif


