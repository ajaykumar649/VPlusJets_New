/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 * Class:   VtoElectronTreeFiller
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill W--> enu or Z-->ee related quantities into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 * 24 Mar' 2010
 * Anil Singh, Panjab U., anil79@fnal.gov  
 * Adapted original electron code to include muon branches.
 * ************************************************************************/

#ifndef ElectroWeakAnalysis_VPlusJets_VtoMuonTreeFiller_h
#define ElectroWeakAnalysis_VPlusJets_VtoMuonTreeFiller_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

// Trigger matcher
#include "ElectroWeakAnalysis/VPlusJets/interface/TriggerMatcher.h"


namespace ewk {

  class VtoMuonTreeFiller {
  public:
    /// specify the name of the TTree, and the configuration for it
    VtoMuonTreeFiller(const char *name, TTree* tree, 
			  const edm::ParameterSet iConfig );

    /// default constructor
    VtoMuonTreeFiller() {};


    /// Destructor, does nothing 
      ~VtoMuonTreeFiller() {};


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

    TTree* tree_;
    const char *  name_;
    std::string Vtype_;
    edm::InputTag mInputBoson;


    /// The object for trigger matching
    std::auto_ptr<TriggerMatcher> trigMatcher;
    

  private:
    // private data members
    
    float V_mass;
    float V_px;
    float V_py;
    float V_pz;
    float V_E;
    float V_Pt;
    float V_Et;
    float V_Eta;
    float V_Phi;
    float V_Vx;
    float V_Vy;
    float V_Vz;
    float V_Y;

    int m1Classification; 
    int m1Charge;
    int m2Classification;
    int m2Charge;

    bool ism1Loose;
    bool ism1Tight;
    bool m1_trigger;
    bool ism2Loose;
    bool ism2Tight;
    bool m2_trigger;

    float m1px;
    float m1py;
    float m1pz;
    float m1E;
    float m1Et;
    float m1Pt;
    float m1Eta;
    float m1Theta;
    float m1Phi;
    float m1Vx;
    float m1Vy;
    float m1Vz;
    float m1Y;

    float m1Nhits;
    float m1Chi2;
    float m1Do;
    float m1DoCorr;
    float m1_trackiso;
    float m1_hcaliso;
    float m1_ecaliso;
    float m1EcalIsodep;
    float m1HcalIsodep;
    

    ///////////////////

    float m2px;
    float m2py;
    float m2pz;
    float m2E;
    float m2Pt;
    float m2Et;
    float m2Eta;
    float m2Theta;
    float m2Phi;
    float m2Vx;
    float m2Vy;
    float m2Vz;
    float m2Y;
 
    float m2Nhits;
    float m2Chi2;
    float m2Do;
    float m2DoCorr;
    float m2_trackiso;
    float m2_hcaliso;
    float m2_ecaliso;
    float m2EcalIsodep;
    float m2HcalIsodep;
 
  };

} //namespace

#endif


