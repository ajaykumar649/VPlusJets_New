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
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "TMath.h" 
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
// Monte Carlo stuff
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"


// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/VtoMuonTreeFiller.h"


ewk::VtoMuonTreeFiller::VtoMuonTreeFiller(const char *name, TTree* tree, 
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
//  fout=new std::ofstream("record.txt");
}





void ewk::VtoMuonTreeFiller::SetBranches()
{
  // Declare jet branches
  std::string lept1 = "muplus";
  std::string lept2 = "muminus";
  if( !(Vtype_=="Z") ) lept1 = "muon";



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



  SetBranch( &m1Classification, lept1+"_classification" );
  SetBranch( &ism1Loose,        lept1+"_isloose" );
  SetBranch( &ism1Tight,        lept1+"_istight" );
  SetBranch( &m1_trigger,       lept1+"_trigger" );


  SetBranch( &m1px,             lept1+"_px" );
  SetBranch( &m1py,             lept1+"_py" );
  SetBranch( &m1pz,             lept1+"_pz" );
  SetBranch( &m1E,              lept1+"_e" );
  SetBranch( &m1Pt,             lept1+"_pt" );
  SetBranch( &m1Et,             lept1+"_et" );
  SetBranch( &m1Eta,            lept1+"_eta" ); 
  SetBranch( &m1Theta,          lept1+"_theta" ); 
  SetBranch( &m1Phi,            lept1+"_phi" );
  SetBranch( &m1Charge,         lept1+"_charge" );
  SetBranch( &m1Vx,             lept1+"_vx" );
  SetBranch( &m1Vy,             lept1+"_vy" );
  SetBranch( &m1Vz,             lept1+"_vz" );
  SetBranch( &m1Y,              lept1+"_y" );
 
  SetBranch( &m1Nhits,          lept1+"_nHits" );
  SetBranch( &m1Chi2,          lept1+"_Chi2" );
  SetBranch( &m1Do,             lept1+"_Do" );
  SetBranch( &m1DoCorr,         lept1+"_DoCorr" );
  SetBranch( &m1_trackiso,      lept1+"_trackiso" );
  SetBranch( &m1_hcaliso,       lept1+"_hcaliso" );
  SetBranch( &m1_ecaliso,       lept1+"_ecaliso" );
  SetBranch( &m1HcalIsodep,       lept1+"_hcalisodep" );
  SetBranch( &m1EcalIsodep,       lept1+"_ecalisodep" );




  ////////////////////////////////////////////////////////
  if(Vtype_=="Z") {	  

    SetBranch( &m2Classification, lept2+"_classification" );
    SetBranch( &ism2Loose,        lept2+"_isloose" );
    SetBranch( &ism2Tight,        lept2+"_istight" );
    SetBranch( &m2_trigger,       lept2+"_trigger" );


    SetBranch( &m2px,             lept2+"_px" );
    SetBranch( &m2py,             lept2+"_py" );
    SetBranch( &m2pz,             lept2+"_pz" );
    SetBranch( &m2E,              lept2+"_e" );
    SetBranch( &m2Pt,             lept2+"_pt" );
    SetBranch( &m2Et,             lept2+"_et" );
    SetBranch( &m2Eta,            lept2+"_eta" ); 
    SetBranch( &m2Theta,          lept2+"_theta" );    
    SetBranch( &m2Phi,            lept2+"_phi" );
    SetBranch( &m2Charge,         lept2+"_charge" );
    SetBranch( &m2Vx,             lept2+"_vx" );
    SetBranch( &m2Vy,             lept2+"_vy" );
    SetBranch( &m2Vz,             lept2+"_vz" );
    SetBranch( &m2Y,              lept2+"_y" );
    SetBranch( &m2_trackiso,      lept2+"_trackiso" );
    SetBranch( &m2_hcaliso,       lept2+"_hcaliso" );
    SetBranch( &m2_ecaliso,       lept2+"_ecaliso");
 
    SetBranch( &m2Nhits,          lept2+"_nHits" );
    SetBranch( &m2Chi2,          lept2+"_Chi2" );
    SetBranch( &m2Do,             lept2+"_Do" );
    SetBranch( &m2DoCorr,         lept2+"_DoCorr" );
    SetBranch( &m2_trackiso,      lept2+"_trackiso" );
    SetBranch( &m2_hcaliso,       lept2+"_hcaliso" );
    SetBranch( &m2_ecaliso,       lept2+"_ecaliso" );
    
    SetBranch( &m2HcalIsodep,       lept2+"_hcalisodep" );
    SetBranch( &m2EcalIsodep,       lept2+"_ecalisodep" );

 
  }	  
}
/////////////////////////////////////////////////////////////////////////






void ewk::VtoMuonTreeFiller::init()   
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

  m1_trigger         = false;
  ism1Loose          = false;
  ism1Tight          = false;
  m2_trigger        = false;
  ism2Loose          = false;
  ism2Tight          = false;

  m1Classification   = -1; 
  m1Charge           = -10;
  m2Classification  = -1;
  m2Charge          = -10;


  m1px               = -99999.;
  m1py               = -99999.;
  m1pz               = -99999.;
  m1E                = -1.;
  m1Et               = -1.;
  m1Pt               = -1.;
  m1Eta              = -10.;
  m1Theta            = -99999.;
  m1Phi              = -10.;
  m1Vx               = -10.;
  m1Vy               = -10.;
  m1Vz               = -10.;
  m1Y                = -10.;

  m1Nhits           = -1111.;
  m1Chi2            = +9999.;
  m1Do              = +9999.;
  m1DoCorr         = +9999.;
  
  m1EcalIsodep = -1111;
  m1HcalIsodep = -1111;
	  
  m1_trackiso     = 500.0;
  m1_hcaliso      = 500.0;
  m1_ecaliso      = 500.0;
  
  
  m2px               = -99999.;
  m2py               = -99999.;
  m2pz               = -99999.;
  m2E                = -1.;
  m2Et               = -1.;
  m2Pt               = -1.;
  m2Eta              = -10.;
  m2Theta            = -99999.;
  m2Phi              = -10.;
  m2Vx               = -10.;
  m2Vy               = -10.;
  m2Vz               = -10.;
  m2Y                = -10.;

  m2Nhits            = -1111.;
  m2Chi2             = +9999.;
  m2Do               = +9999.;
  m2DoCorr           = +9999.;
  
  m2_trackiso    = 500.0;
  m2_hcaliso     = 500.0;
  m2_ecaliso     = 500.0;

  // initialization done
}







void ewk::VtoMuonTreeFiller::fill(const edm::Event& iEvent)
{
//  (*fout)<<"Event: \n"<<iEvent.id()<<std::endl;
//   // first initialize to the default values
  init();

  reco::BeamSpot beamSpot;
  edm::Handle<reco::BeamSpot> beamspotHandle;
  if (!iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), beamspotHandle)) {
    LogTrace("") << ">>> No beam spot found !!!";
    return;
  }
  
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
  const reco::Muon* elm1=NULL;
  const reco::Muon* elm2=NULL;

  // if Z--> e+e- then elm1 = e+, elm2 = e-
  if(Vtype_=="Z") {
    
    const reco::Candidate* lep0 = Vboson->daughter( 0 );
    const reco::Candidate* lep1 = Vboson->daughter( 1 );
    const reco::Muon& muonDau1  = dynamic_cast<const reco::Muon& >(*lep0->masterClone());
    const reco::Muon& muonDau2  = dynamic_cast<const reco::Muon& >(*lep1->masterClone());
    if(!(muonDau1.isGlobalMuon()))return;
    if(!(muonDau2.isGlobalMuon()))return;
    const reco::Muon* m1 = new reco::Muon(muonDau1);
    const reco::Muon* m2 = new reco::Muon(muonDau2);
    
    if(!(m1==NULL && m2==NULL) ) {
      if(m1->charge() > 0) { elm1 = m1; elm2 = m2; }
      //std::cout << "chargm1 = " << m1->charge() << ", chargm2 = " << m2->charge() << std::endl;
    }
  }
  else if(Vtype_=="W") 
    elm1  = dynamic_cast<const reco::Muon*>( Vboson->daughter(0) );
  else {
    std::cout << "***Error:" << "Something wrong in Obj collection !" << std::endl;
    return;  // if no electron found, then return
  }


  //   ////////// electron #1 quantities //////////////
  if( !(elm1 == NULL) ) {
    m1Charge           = elm1-> charge();
    m1Vx               = elm1->vx();
    m1Vy               = elm1->vy();
    m1Vz               = elm1->vz();
    m1Y                = elm1->rapidity();
    // isolation 
    //  m1Classification  = elm1->classification();
    //return ;    
    m1_trackiso       = elm1->isolationR03().emEt;
    m1_ecaliso        = elm1->isolationR03().hadEt;
    m1_hcaliso        = elm1->isolationR03().sumPt;
    m1EcalIsodep      = elm1->isolationR03().emVetoEt;
    m1HcalIsodep      = elm1->isolationR03().hadVetoEt;
    
    m1px = elm1->px();
    m1py = elm1->py();
    m1pz = elm1->pz();
    m1E  = elm1->energy();;
    m1Et = elm1->et();
    m1Pt = elm1->pt();
    m1Eta = elm1->eta();
    m1Theta = elm1->theta();
    m1Phi = elm1->phi();
    m1Y = elm1->rapidity();
    m1Charge = elm1->threeCharge();
    //m1Vx = elm1->Pt();
    //m1Vy = elm1->Pt();
    //m1Vz = elm1->Pt();
    reco::TrackRef gtk = elm1->globalTrack();
    m1Nhits = gtk->hitPattern().numberOfValidTrackerHits();
    m1Chi2 = gtk->normalizedChi2();
    m1Do = gtk->dxy();
    m1DoCorr = gtk->dxy(beamspotHandle->position());
    m1_trigger = trigMatcher->CheckTriggerMatch( iEvent, m1Eta, m1Phi);
//    (*fout)<<"Mu1Pt: "<<m1Pt<<std::endl;

  }
  

//   ////////// electron #2 quantities //////////////
   if( !(elm2 == NULL) ) {

     m2Charge           = elm2-> charge();
     m2Vx               = elm2->vx();
     m2Vy               = elm2->vy();
     m2Vz               = elm2->vz();
     m2Y                = elm2->rapidity();
     /// isolation 
     //  m2Classification  = elm2->classification();
     m2_trackiso       = elm2->isolationR03().emEt;
     m2_ecaliso        = elm2->isolationR03().hadEt;
     m2_hcaliso        = elm2->isolationR03().sumPt;
     m2EcalIsodep      = elm2->isolationR03().emVetoEt;
     m2HcalIsodep      = elm2->isolationR03().hadVetoEt;
     m2px = elm2->px();
     m2py = elm2->py();
     m2pz = elm2->pz();
     m2E  = elm2->energy();;
     m2Et = elm2->et();
     m2Pt = elm2->pt();
     m2Eta = elm2->eta();
     m2Theta = elm2->theta();
     m2Phi = elm2->phi();
     m2Y = elm2->rapidity();
     m2Charge = elm2->threeCharge();
     //m2Vx = elm2->Pt();
     //m2Vy = elm2->Pt();
     //m2Vz = elm2->Pt();
     reco::TrackRef gtk = elm2->globalTrack();
     m2Nhits = gtk->hitPattern().numberOfValidTrackerHits();
     m2Chi2 = gtk->normalizedChi2();
     m2Do = gtk->dxy();
     m2DoCorr = gtk->dxy(beamspotHandle->position());
     m2_trigger = trigMatcher->CheckTriggerMatch( iEvent, m1Eta, m1Phi);
//     (*fout)<<"Mu2Pt: "<<m2Pt<<std::endl;
   } 
}





////////////////// utilities, helpers ///////////////////
 
void ewk::VtoMuonTreeFiller::SetBranch( float* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/F").c_str() );
}


void ewk::VtoMuonTreeFiller::SetBranch( int* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/I").c_str() );
}


void ewk::VtoMuonTreeFiller::SetBranch( bool* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/O").c_str() );
}

