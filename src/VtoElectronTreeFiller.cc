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
 *   To fill W--> enu or Z-->ee  related quantities into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

// CMS includes
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"  
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TMath.h" 

// Monte Carlo stuff
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"


// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/VtoElectronTreeFiller.h"


ewk::VtoElectronTreeFiller::VtoElectronTreeFiller(const char *name, TTree* tree, 
							const edm::ParameterSet iConfig):
  trigMatcher( new TriggerMatcher (iConfig) )
{

  // ********** Vector boson ********** //
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 
  else std::cout << "***Error:" << name << " Collection not specified !" << std::endl;

  tree_     = tree;
  name_     = name;
  Vtype_    = iConfig.getParameter<std::string>("VBosonType"); 
  if( !(tree==0) ) SetBranches();

}





void ewk::VtoElectronTreeFiller::SetBranches()
{
  // Declare jet branches
  std::string lept1 = "eplus";
  std::string lept2 = "eminus";
  if( !(Vtype_=="Z") ) lept1 = "electron";

  SetBranch( &V_mass,        "mass");
  SetBranch( &V_px,        "px");
  SetBranch( &V_py,        "py");
  SetBranch( &V_pz,        "pz");
  SetBranch( &V_E,         "e");
  SetBranch( &V_Pt,        "pt");
  SetBranch( &V_Et,        "et");
  SetBranch( &V_Eta,       "eta");    
  SetBranch( &V_Phi,       "phi");
  SetBranch( &V_Vx,        "vx");
  SetBranch( &V_Vy,        "vy");
  SetBranch( &V_Vz,        "vz");
  SetBranch( &V_Y,         "y");
  ///////////////////////////////////////////////
  SetBranch( &e1px,             lept1+"_px" );
  SetBranch( &e1py,             lept1+"_py" );
  SetBranch( &e1pz,             lept1+"_pz" );
  SetBranch( &e1E,              lept1+"_e" );
  SetBranch( &e1Pt,             lept1+"_pt" );
  SetBranch( &e1Et,             lept1+"_et" );
  SetBranch( &e1Eta,            lept1+"_eta" ); 
  SetBranch( &e1Theta,          lept1+"_theta" ); 
  SetBranch( &e1Phi,            lept1+"_phi" );
  SetBranch( &e1Charge,         lept1+"_charge" );
  SetBranch( &e1Vx,             lept1+"_vx" );
  SetBranch( &e1Vy,             lept1+"_vy" );
  SetBranch( &e1Vz,             lept1+"_vz" );
  SetBranch( &e1Y,              lept1+"_y" );
  SetBranch( &e1_trackiso,      lept1+"_trackiso" );
  SetBranch( &e1_hcaliso,       lept1+"_hcaliso" );
  SetBranch( &e1_ecaliso,       lept1+"_ecaliso" );
  SetBranch( &e1Classification, lept1+"_classification" );
  SetBranch( &ise1Loose,        lept1+"_isloose" );
  SetBranch( &ise1Tight,        lept1+"_istight" );
  SetBranch( &e1_trigger,       lept1+"_trigger" );
  SetBranch( &e1_sc_x,          lept1+"_sc_x" );
  SetBranch( &e1_sc_y,          lept1+"_sc_y" );
  SetBranch( &e1_sc_z,          lept1+"_sc_z" );
  SetBranch( &e1_EoverPout,     lept1+"_eoverp_out" );
  SetBranch( &e1_EoverPin,      lept1+"_eoverp_in" );
  SetBranch( &e1_numberOfBrems, lept1+"_numbrem" );
  SetBranch( &e1_BremFraction,  lept1+"_fbrem" );
  SetBranch( &e1_DeltaEtaIn,    lept1+"_deltaeta_in" );
  SetBranch( &e1_DeltaPhiIn,    lept1+"_deltaphi_in" );
  SetBranch( &e1_DeltaPhiOut,   lept1+"_deltaphi_out" );
  SetBranch( &e1_DeltaEtaOut,   lept1+"_deltaeta_out" );
  SetBranch( &e1_Trackmom_calo, lept1+"_trackmom_calo" );
  SetBranch( &e1_Trackmom_vtx,  lept1+"_trackmom_vtx" );	  
  SetBranch( &e1_HoverE,        lept1+"_hovere" );	    
  SetBranch( &e1_E9overE25,     lept1+"_e9e25" );
  SetBranch( &e1_SigmaEtaEta,   lept1+"_sigmaetaeta" );
  SetBranch( &e1_SigmaIetaIeta, lept1+"_sigmaietaieta" );	  
  ////////////////////////////////////////////////////////
  if(Vtype_=="Z") {	  
    SetBranch( &e2px,             lept2+"_px" );
    SetBranch( &e2py,             lept2+"_py" );
    SetBranch( &e2pz,             lept2+"_pz" );
    SetBranch( &e2E,              lept2+"_e" );
    SetBranch( &e2Pt,             lept2+"_pt" );
    SetBranch( &e2Et,             lept2+"_et" );
    SetBranch( &e2Eta,            lept2+"_eta" ); 
    SetBranch( &e2Theta,          lept2+"_theta" );    
    SetBranch( &e2Phi,            lept2+"_phi" );
    SetBranch( &e2Charge,         lept2+"_charge" );
    SetBranch( &e2Vx,             lept2+"_vx" );
    SetBranch( &e2Vy,             lept2+"_vy" );
    SetBranch( &e2Vz,             lept2+"_vz" );
    SetBranch( &e2Y,              lept2+"_y" );
    SetBranch( &e2_trackiso,      lept2+"_trackiso" );
    SetBranch( &e2_hcaliso,       lept2+"_hcaliso" );
    SetBranch( &e2_ecaliso,       lept2+"_ecaliso");
    SetBranch( &e2Classification, lept2+"_classification" );
    SetBranch( &ise2Loose,        lept2+"_isloose" );
    SetBranch( &ise2Tight,        lept2+"_istight" );
    SetBranch( &e2_trigger,       lept2+"_trigger" );
    SetBranch( &e2_sc_x,          lept2+"_sc_x" );
    SetBranch( &e2_sc_y,          lept2+"_sc_y" );
    SetBranch( &e2_sc_z,          lept2+"_sc_z" );
    SetBranch( &e2_EoverPout,     lept2+"_eoverp_out" );
    SetBranch( &e2_EoverPin,      lept2+"_eoverp_in" );
    SetBranch( &e2_numberOfBrems, lept2+"_numbrem" );
    SetBranch( &e2_BremFraction,  lept2+"_fbrem" );
    SetBranch( &e2_DeltaEtaIn,    lept2+"_deltaeta_in" );
    SetBranch( &e2_DeltaPhiIn,    lept2+"_deltaphi_in" );
    SetBranch( &e2_DeltaPhiOut,   lept2+"_deltaphi_out" );
    SetBranch( &e2_DeltaEtaOut,   lept2+"_deltaeta_out" );
    SetBranch( &e2_Trackmom_calo, lept2+"_trackmom_calo" );
    SetBranch( &e2_Trackmom_vtx,  lept2+"_trackmom_vtx" );	  
    SetBranch( &e2_HoverE,        lept2+"_hovere" );	   	  
    SetBranch( &e2_E9overE25,     lept2+"_e9e25" );
    SetBranch( &e2_SigmaEtaEta,   lept2+"_sigmaetaeta" );
    SetBranch( &e2_SigmaIetaIeta, lept2+"_sigmaietaieta" );
  }	  
}
/////////////////////////////////////////////////////////////////////////






void ewk::VtoElectronTreeFiller::init()   
{
  // initialize private data members
  V_mass                  = -1.;
  V_px                  = -99999.;
  V_py                  = -99999.;
  V_pz                  = -99999.;
  V_E                   = -1.;
  V_Pt                  = -1.;
  V_Et                  = -1.;
  V_Eta                 = -10.;
  V_Phi                 = -10.;
  V_Vx                  = -10.;
  V_Vy                  = -10.;
  V_Vz                  = -10.;
  V_Y                   = -10.;

  e1_trigger         = false;
  ise1Loose          = false;
  ise1Tight          = false;
  e2_trigger        = false;
  ise2Loose          = false;
  ise2Tight          = false;

  e1Classification   = -1; 
  e1Charge           = -10;
  e2Classification  = -1;
  e2Charge          = -10;


  e1px               = -99999.;
  e1py               = -99999.;
  e1pz               = -99999.;
  e1E                = -1.;
  e1Et               = -1.;
  e1Pt               = -1.;
  e1Eta              = -10.;
  e1Theta            = -99999.;
  e1Phi              = -10.;
  e1Vx               = -10.;
  e1Vy               = -10.;
  e1Vz               = -10.;
  e1Y                = -10.;
  e1_sc_x            = -10.;
  e1_sc_y            = -10.;
  e1_sc_z            = -10.;
  e1_EoverPout       = -1.;
  e1_EoverPin        = -1.;
  e1_numberOfBrems   = -10.;
  e1_BremFraction    = -1.;
  e1_DeltaEtaIn      = -10.;
  e1_DeltaPhiIn      = -10.;
  e1_DeltaPhiOut     = -10.;
  e1_DeltaEtaOut     = -10.;
  e1_Trackmom_calo   = -1.;
  e1_Trackmom_vtx    = -1.;	  
  e1_HoverE          = -1.;	   	  
  e1_E9overE25       = -10.;
  e1_SigmaEtaEta     = -1.;
  e1_SigmaIetaIeta   = -1.;	  
	  
  e2px              = -99999.;
  e2py              = -99999.;
  e2pz              = -99999.;
  e2E               = -1.;
  e2Pt              = -1.;
  e2Et              = -1.;
  e2Eta             = -10.;
  e2Theta           = -99999.;
  e2Phi             = -10.;
  e2Vx              = -10.;
  e2Vy              = -10.;
  e2Vz              = -10.;
  e2Y               = -10.;
  e2_sc_x            = -10.;
  e2_sc_y            = -10.;
  e2_sc_z            = -10.;
  e2_EoverPout       = -1.;
  e2_EoverPin        = -1.;
  e2_numberOfBrems   = -10.;
  e2_BremFraction    = -1.;
  e2_DeltaEtaIn      = -10.;
  e2_DeltaPhiIn      = -10.;
  e2_DeltaPhiOut     = -10.;
  e2_DeltaEtaOut     = -10.;
  e2_Trackmom_calo   = -1.;
  e2_Trackmom_vtx    = -1.;	  
  e2_HoverE          = -1.;	    
  e2_E9overE25       = -10.;
  e2_SigmaEtaEta     = -1.;
  e2_SigmaIetaIeta     = -1.;	  
	  
  //////////////
  e1_trackiso     = 500.0;
  e1_hcaliso      = 500.0;
  e1_ecaliso      = 500.0;
  e2_trackiso    = 500.0;
  e2_hcaliso     = 500.0;
  e2_ecaliso     = 500.0;

  // initialization done
}







void ewk::VtoElectronTreeFiller::fill(const edm::Event& iEvent)
{
  // first initialize to the default values
  init();

  edm::Handle<reco::CandidateView> boson;
  iEvent.getByLabel( mInputBoson, boson);
  if( boson->size()<1 ) return; // Nothing to fill
  
  const reco::Candidate *Vboson = &((*boson)[0]); 
  if( Vboson == 0) return;

  ////////// Vector boson quantities //////////////
  V_mass = Vboson->mass();
  V_Eta = Vboson->eta();   
  V_Phi = Vboson->phi();
  V_Vx = Vboson->vx();
  V_Vy = Vboson->vy();
  V_Vz = Vboson->vz();
  V_Y  = Vboson->rapidity();
  V_px = Vboson->px();
  V_py = Vboson->py();
  V_pz = Vboson->pz();
  V_E  = Vboson->energy();
  V_Pt = Vboson->pt();
  V_Et = Vboson->et();

  // now iterate over the daughters  
  const reco::GsfElectron* ele1=NULL;
  const reco::GsfElectron* ele2=NULL;

  // if Z--> e+e- then ele1 = e+, ele2 = e-
  if(Vtype_=="Z") {
    const reco::GsfElectron* e1  
      = dynamic_cast<const reco::GsfElectron*>( Vboson->daughter(0) );
    const reco::GsfElectron* e2
      = dynamic_cast<const reco::GsfElectron*>( Vboson->daughter(1) );
    if(!(e1==NULL && e2==NULL) ) {
      if(e1->charge() > 0) { ele1 = e1; ele2 = e2; }
      std::cout << "charge1 = " << e1->charge() << ", charge2 = " << e2->charge() << std::endl;
    }
  }
  else if(Vtype_=="W") 
    ele1  = dynamic_cast<const reco::GsfElectron*>( Vboson->daughter(0) );
  else {
    std::cout << "***Error:" << "Something wrong in Obj collection !" << std::endl;
    return;  // if no electron found, then return
  }

  ////////// electron #1 quantities //////////////
  if( !(ele1 == NULL) ) {
    e1Charge           = ele1-> charge();
    e1Vx               = ele1->vx();
    e1Vy               = ele1->vy();
    e1Vz               = ele1->vz();
    e1Y                = ele1->rapidity();
    /// isolation 
    e1_trackiso       = ele1->dr04TkSumPt();
    e1_ecaliso        = ele1->dr04EcalRecHitSumEt();
    e1_hcaliso        = ele1->dr04HcalTowerSumEt();
    e1Classification  = ele1->classification();
    e1_numberOfBrems  = ele1->numberOfBrems();      
    e1_BremFraction   = ele1->fbrem();
    e1_DeltaEtaIn     = ele1->deltaEtaSuperClusterTrackAtVtx();
    e1_DeltaPhiIn     = ele1->deltaPhiSuperClusterTrackAtVtx();
    e1_DeltaPhiOut    = ele1->deltaPhiSeedClusterTrackAtCalo();
    e1_DeltaEtaOut    = ele1->deltaEtaSeedClusterTrackAtCalo();
    //Track Momentum information
    e1_Trackmom_calo  = sqrt(ele1->trackMomentumAtCalo().perp2());
    e1_Trackmom_vtx   = sqrt(ele1->trackMomentumAtVtx().perp2());
    //get Hcal energy over Ecal Energy
    e1_HoverE = ele1->hadronicOverEm();	  
    e1_EoverPout      = ele1->eSeedClusterOverPout();
    e1_EoverPin       = ele1->eSuperClusterOverP();
    //get SuperCluster (sc) infos
    reco::SuperClusterRef SCp = ele1->superCluster();
    e1_sc_x          = SCp->x();
    e1_sc_y          = SCp->y();
    e1_sc_z          = SCp->z();
    e1Theta          = SCp->position().Theta();
    e1Eta            = SCp->eta();    
    e1Phi            = SCp->phi();
    e1E              = SCp->energy();
    e1px             = e1E * sin(e1Theta) * cos(e1Phi);
    e1py             = e1E * sin(e1Theta) * sin(e1Phi);
    e1pz             = e1E * cos(e1Theta);
    e1Pt             = e1E / cosh(e1Eta);
    e1Et             = e1Pt;	  
    e1_SigmaEtaEta   = ele1->sigmaEtaEta();
    e1_SigmaIetaIeta = ele1->sigmaIetaIeta();
    e1_trigger = trigMatcher->CheckTriggerMatch( iEvent, e1Eta, e1Phi);
  }

  ////////// electron #2 quantities //////////////
  if( !(ele2 == NULL) ) {
    e2Charge          = ele2->charge();
    e2Vx              = ele2->vx();
    e2Vy              = ele2->vy();
    e2Vz              = ele2->vz();
    e2Y               = ele2->rapidity();
    /// isolation 
    e1_trackiso       = ele2->dr04TkSumPt();
    e1_ecaliso        = ele2->dr04EcalRecHitSumEt();
    e1_hcaliso        = ele2->dr04HcalTowerSumEt();
    e2Classification  = ele2->classification();
    e2_EoverPout = ele2->eSeedClusterOverPout();
    e2_EoverPin  = ele2->eSuperClusterOverP();
    e2_numberOfBrems  = ele2->numberOfBrems();
    e2_BremFraction   = ele2->fbrem();
    e2_DeltaEtaIn     = ele2->deltaEtaSuperClusterTrackAtVtx();
    e2_DeltaPhiIn     = ele2->deltaPhiSuperClusterTrackAtVtx();
    e2_DeltaPhiOut    = ele2->deltaPhiSeedClusterTrackAtCalo();
    e2_DeltaEtaOut    = ele2->deltaEtaSeedClusterTrackAtCalo();
    //Track Momentum information
    e2_Trackmom_calo  = sqrt(ele2->trackMomentumAtCalo().perp2());
    e2_Trackmom_vtx   = sqrt(ele2->trackMomentumAtVtx().perp2());   
    //get Hcal energy over Ecal Energy
    e2_HoverE = ele2->hadronicOverEm();	  
    //get SuperCluster (sc) infos
    reco::SuperClusterRef SCm = ele2->superCluster();
    e2_sc_x          = SCm->x();
    e2_sc_y          = SCm->y();
    e2_sc_z          = SCm->z();
    e2Theta          = SCm->position().Theta();
    e2Eta            = SCm->eta();    
    e2Phi            = SCm->phi();
    e2E              = SCm->energy();
    e2px             = e2E * sin(e2Theta) * cos(e2Phi);
    e2py             = e2E * sin(e2Theta) * sin(e2Phi);
    e2pz             = e2E * cos(e2Theta);
    e2Pt             = e2E / cosh(e2Eta);
    e2Et             = e2Pt;	 
    e2_SigmaEtaEta   = ele2->sigmaEtaEta();
    e2_SigmaIetaIeta = ele2->sigmaIetaIeta();
    e2_trigger = trigMatcher->CheckTriggerMatch( iEvent, e2Eta, e2Phi );   
  } 

}





////////////////// utilities, helpers ///////////////////
 
void ewk::VtoElectronTreeFiller::SetBranch( float* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/F").c_str() );
}


void ewk::VtoElectronTreeFiller::SetBranch( int* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/I").c_str() );
}


void ewk::VtoElectronTreeFiller::SetBranch( bool* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/O").c_str() );
}

