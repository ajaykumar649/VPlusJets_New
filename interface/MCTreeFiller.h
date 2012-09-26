/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 * Class:   MCTreeFiller
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill W/Z related MC information into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef ElectroWeakAnalysis_VPlusJets_MCTreeFiller_h
#define ElectroWeakAnalysis_VPlusJets_MCTreeFiller_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace ewk {

  class MCTreeFiller {
  public:
    /// specify the name of the TTree, and the configuration for it
    MCTreeFiller(const char *name, TTree* tree, 
			  const edm::ParameterSet iConfig );

    /// default constructor
    MCTreeFiller() {};


    /// Destructor, does nothing 
      ~MCTreeFiller() {};


    /// To be called once per event to fill the values for jets
    void fill(const edm::Event &iEvent);


  protected:

    /// To be called once per event, to initialize variable values to default
    void init();
    /// Helper function for main constructor
    void SetBranches(); 
    void SetBranch( float* x, std::string name );
    void SetBranch( int* x, std::string name );
    void SetBranch( bool* x, std::string name );
    void SetBranch( float x, std::string name );

    TTree* tree_;
    const char *  name_;
    std::string Vtype_;
    std::string ptype_;
    int pdgIdDau_;
    edm::InputTag mInputBoson;
    edm::InputTag mInputgenParticles;

  private:
    // private data members
    
    float H_mass;
    float H_px;
    float H_py;
    float H_pz;
    float H_E;
    float H_Pt;
    float H_Et;
    float H_Eta;
    float H_Phi;
    float H_Vx;
    float H_Vy;
    float H_Vz;
    float H_Y;
    int H_Id;

/*
    int l1Charge;
    int l2Charge;

    float l1px;
    float l1py;
    float l1pz;
    float l1E;
    float l1Et;
    float l1Pt;
    float l1Eta;
    float l1Theta;
    float l1Phi;
    float l1Vx;
    float l1Vy;
    float l1Vz;
    float l1Y;
	  
    ///////////////////
    float l2px;
    float l2py;
    float l2pz;
    float l2E;
    float l2Pt;
    float l2Et;
    float l2Eta;
    float l2Theta;
    float l2Phi;
    float l2Vx;
    float l2Vy;
    float l2Vz;
    float l2Y;	  
*/
    ///////////////////
   
    int Parton_Charge[2];
    float Parton_px[2];
    float Parton_py[2];
    float Parton_pz[2];
    float Parton_E[2];
    float Parton_Pt[2];
    float Parton_Et[2];
    float Parton_Eta[2];
    float Parton_Theta[2];
    float Parton_Phi[2];
    float Parton_Vx[2];
    float Parton_Vy[2];
    float Parton_Vz[2];
    float Parton_Y[2];
    int Parton_Id[2];

    ///////////////////
    float Lepton_px;
    float Lepton_py;
    float Lepton_pz;
    float Lepton_E;
    float Lepton_Pt;
    float Lepton_Et;
    float Lepton_Eta;
    int   Lepton_Charge;
    float Lepton_Theta;
    float Lepton_Phi;
    float Lepton_Vx;
    float Lepton_Vy;
    float Lepton_Vz;
    float Lepton_Y;
    int Lepton_Id;

    ///////////////////
    float Met_px;
    float Met_py;
    float Met_pz;
    float Met_E;
    float Met_Pt;
    float Met_Et;
    float Met_Eta;
    int   Met_Charge;
    float Met_Theta;
    float Met_Phi;
    float Met_Vx;
    float Met_Vy;
    float Met_Vz;
    float Met_Y;
    int Met_Id;



  };

} //namespace

#endif


