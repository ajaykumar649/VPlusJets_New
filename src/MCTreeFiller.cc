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
 *   To fill W/Z related MC information into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

// CMS includes
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TMath.h" 
// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/MCTreeFiller.h"


ewk::MCTreeFiller::MCTreeFiller(const char *name, TTree* tree, 
				const edm::ParameterSet iConfig)
{

  // ********** Vector boson ********** //
  tree_     = tree;
  name_     = name;
  Vtype_    = iConfig.getParameter<std::string>("VBosonType"); 
  ptype_    = iConfig.getParameter<std::string>("LeptonType");
  pdgIdDau_ = 11;
  if(  iConfig.existsAs<edm::InputTag>("srcGenParticles") )
	  mInputgenParticles  = iConfig.getParameter<edm::InputTag>("srcGenParticles");
  if(ptype_=="muon") pdgIdDau_ = 13; 
  if( !(tree==0) ) SetBranches();
}





void ewk::MCTreeFiller::SetBranches()
{

	std::cout << "srcgenparticles " << mInputgenParticles << std::endl;

// Declare jet branches
  std::string lept1;
  std::string lept2;
  if( Vtype_=="Z" ) {
    if(ptype_=="muon") {
      lept1 = "muplus";
      lept2 = "muminus";
    } else {
      lept1 = "eplus";
      lept2 = "eminus";
    } 
  } else {
    if(ptype_=="muon") {
      lept1 = "muon";
      lept2 = "neutino";
    } else {
      lept1 = "electron";
      lept2 = "neutrino";
    }
  }

  SetBranch( &H_mass,      "H_mass_gen");
  SetBranch( &H_px,        "H_px_gen");
  SetBranch( &H_py,        "H_py_gen");
  SetBranch( &H_pz,        "H_pz_gen");
  SetBranch( &H_E,         "H_e_gen");
  SetBranch( &H_Pt,        "H_pt_gen");
  SetBranch( &H_Et,        "H_et_gen");
  SetBranch( &H_Eta,       "H_eta_gen");    
  SetBranch( &H_Phi,       "H_phi_gen");
  SetBranch( &H_Vx,        "H_vx_gen");
  SetBranch( &H_Vy,        "H_vy_gen");
  SetBranch( &H_Vz,        "H_vz_gen");
  SetBranch( &H_Y,         "H_y_gen");
  SetBranch( &H_Id,         "H_Id_gen");

/*
  ///////////////////////////////////////////////
  SetBranch( &l1px,             lept1+"_px_gen" );
  SetBranch( &l1py,             lept1+"_py_gen" );
  SetBranch( &l1pz,             lept1+"_pz_gen" );
  SetBranch( &l1E,              lept1+"_e_gen" );
  SetBranch( &l1Pt,             lept1+"_pt_gen" );
  SetBranch( &l1Et,             lept1+"_et_gen" );
  SetBranch( &l1Eta,            lept1+"_eta_gen" ); 
  SetBranch( &l1Theta,          lept1+"_theta_gen" ); 
  SetBranch( &l1Phi,            lept1+"_phi_gen" );
  SetBranch( &l1Charge,         lept1+"_charge_gen" );
  SetBranch( &l1Vx,             lept1+"_vx_gen" );
  SetBranch( &l1Vy,             lept1+"_vy_gen" );
  SetBranch( &l1Vz,             lept1+"_vz_gen" );
  SetBranch( &l1Y,              lept1+"_y_gen" );
	  
  ////////////////////////////////////////////////////////
  SetBranch( &l2px,             lept2+"_px_gen" );
  SetBranch( &l2py,             lept2+"_py_gen" );
  SetBranch( &l2pz,             lept2+"_pz_gen" );
  SetBranch( &l2E,              lept2+"_e_gen" );
  SetBranch( &l2Pt,             lept2+"_pt_gen" );
  SetBranch( &l2Et,             lept2+"_et_gen" );
  SetBranch( &l2Eta,            lept2+"_eta_gen" ); 
  SetBranch( &l2Theta,          lept2+"_theta_gen" );    
  SetBranch( &l2Phi,            lept2+"_phi_gen" );
  SetBranch( &l2Charge,         lept2+"_charge_gen" );
  SetBranch( &l2Vx,             lept2+"_vx_gen" );
  SetBranch( &l2Vy,             lept2+"_vy_gen" );
  SetBranch( &l2Vz,             lept2+"_vz_gen" );
  SetBranch( &l2Y,              lept2+"_y_gen" );
*/

 ////////////////////////////////////////////////////////
  SetBranch( Parton_px,             "Parton_px[2]" );
  SetBranch( Parton_py,             "Parton_py[2]" );
  SetBranch( Parton_pz,             "Parton_pz[2]" );
  SetBranch( Parton_E,              "Parton_E[2]" );
  SetBranch( Parton_Pt,             "Parton_pt[2]" );
  SetBranch( Parton_Et,             "Parton_et[2]" );
  SetBranch( Parton_Eta,            "Parton_eta[2]" ); 
  SetBranch( Parton_Theta,          "Parton_theta[2]" );    
  SetBranch( Parton_Phi,            "Parton_phi[2]" );
  SetBranch( Parton_Charge,         "Parton_charge[2]" );
  SetBranch( Parton_Vx,             "Parton_vx[2]" );
  SetBranch( Parton_Vy,             "Parton_vy[2]" );
  SetBranch( Parton_Vz,             "Parton_vz[2]" );
  SetBranch( Parton_Y,              "Parton_y[2]" );
  SetBranch( Parton_Id,              "Parton_Id[2]" );


 ////////////////////////////////////////////////////////
  SetBranch( &Lepton_px,             "Lepton_px" );
  SetBranch( &Lepton_py,             "Lepton_py" );
  SetBranch( &Lepton_pz,             "Lepton_pz" );
  SetBranch( &Lepton_E,              "Lepton_E" );
  SetBranch( &Lepton_Pt,             "Lepton_pt" );
  SetBranch( &Lepton_Et,             "Lepton_et" );
  SetBranch( &Lepton_Eta,            "Lepton_eta" );    
  SetBranch( &Lepton_Theta,          "Lepton_theta" );    
  SetBranch( &Lepton_Phi,            "Lepton_phi" );
  SetBranch( &Lepton_Charge,         "Lepton_charge" );
  SetBranch( &Lepton_Vx,             "Lepton_vx" );
  SetBranch( &Lepton_Vy,             "Lepton_vy" );
  SetBranch( &Lepton_Vz,             "Lepton_vz" );
  SetBranch( &Lepton_Y,              "Lepton_y" );
  SetBranch( &Lepton_Id,              "Lepton_Id" );

 ////////////////////////////////////////////////////////
  SetBranch( &Met_px,             "Met_px" );
  SetBranch( &Met_py,             "Met_py" );
  SetBranch( &Met_pz,             "Met_pz" );
  SetBranch( &Met_E,              "Met_E" );
  SetBranch( &Met_Pt,             "Met_pt" );
  SetBranch( &Met_Et,             "Met_et" );
  SetBranch( &Met_Eta,            "Met_eta" );    
  SetBranch( &Met_Theta,          "Met_theta" );    
  SetBranch( &Met_Phi,            "Met_phi" );
  SetBranch( &Met_Charge,         "Met_charge" );
  SetBranch( &Met_Vx,             "Met_vx" );
  SetBranch( &Met_Vy,             "Met_vy" );
  SetBranch( &Met_Vz,             "Met_vz" );
  SetBranch( &Met_Y,              "Met_y" );
  SetBranch( &Met_Id,              "Met_Id" );


    
}
/////////////////////////////////////////////////////////////////////////






void ewk::MCTreeFiller::init()   
{
  // initialize private data members
  H_mass                  = -1.;
  H_px                  = -99999.;
  H_py                  = -99999.;
  H_pz                  = -99999.;
  H_E                   = -1.;
  H_Pt                  = -1.;
  H_Et                  = -1.;
  H_Eta                 = -10.;
  H_Phi                 = -10.;
  H_Vx                  = -10.;
  H_Vy                  = -10.;
  H_Vz                  = -10.;
  H_Y                   = -10.;
  H_Id                   = 0;
/*
  l1Charge           = -10;
  l2Charge          = -10;

  l1px               = -99999.;
  l1py               = -99999.;
  l1pz               = -99999.;
  l1E                = -1.;
  l1Et               = -1.;
  l1Pt               = -1.;
  l1Eta              = -10.;
  l1Theta            = -99999.;
  l1Phi              = -10.;
  l1Vx               = -10.;
  l1Vy               = -10.;
  l1Vz               = -10.;
  l1Y                = -10.;	  
	  
  l2px              = -99999.;
  l2py              = -99999.;
  l2pz              = -99999.;
  l2E               = -1.;
  l2Pt              = -1.;
  l2Et              = -1.;
  l2Eta             = -10.;
  l2Theta           = -99999.;
  l2Phi             = -10.;
  l2Vx              = -10.;
  l2Vy              = -10.;
  l2Vz              = -10.;
  l2Y               = -10.;
*/
for ( int i =0; i<2; i++){
  Parton_px[i]              = -99999.;
  Parton_py[i]              = -99999.;
  Parton_pz[i]              = -99999.;
  Parton_E[i]               = -1.;
  Parton_Pt[i]              = -1.;
  Parton_Et[i]              = -1.;
  Parton_Eta[i]             = -10.;
  Parton_Theta[i]           = -99999.;
  Parton_Phi[i]             = -10.;
  Parton_Vx[i]              = -10.;
  Parton_Vy[i]              = -10.;
  Parton_Vz[i]              = -10.;
  Parton_Y[i]              =  -10.;
  Parton_Id[i]              = 0;
}

  Lepton_px              = -99999.;
  Lepton_py              = -99999.;
  Lepton_pz              = -99999.;
  Lepton_E               = -1.;
  Lepton_Pt              = -1.;
  Lepton_Et              = -1.;
  Lepton_Eta             = -10.;
  Lepton_Charge          = -10;
  Lepton_Theta           = -99999.;
  Lepton_Phi             = -10.;
  Lepton_Vx              = -10.;
  Lepton_Vy              = -10.;
  Lepton_Vz              = -10.;
  Lepton_Y               = -10.;
  Lepton_Id              = 0;

  Met_px              = -99999.;
  Met_py              = -99999.;
  Met_pz              = -99999.;
  Met_E               = -1.;
  Met_Pt              = -1.;
  Met_Et              = -1.;
  Met_Eta             = -10.;
  Met_Charge          = -10;
  Met_Theta           = -99999.;
  Met_Phi             = -10.;
  Met_Vx              = -10.;
  Met_Vy              = -10.;
  Met_Vz              = -10.;
  Met_Y               = -10.;
  Met_Id              = 0;


  // initialization done
}







void ewk::MCTreeFiller::fill(const edm::Event& iEvent)
{
  // first initialize to the default values
  init();


  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(mInputgenParticles, genParticles);

  size_t nGen = genParticles->size();
  if( nGen < 1 ) return; // Nothing to fill


  // now iterate over the daughters  
//  const reco::Candidate *V=NULL;
//  const reco::Candidate* lepton1=NULL;
//  const reco::Candidate* lepton2=NULL;
  const reco::Candidate *H=NULL;
  const reco::Candidate *Lepton=NULL;
  const reco::Candidate *Parton1=NULL;
  const reco::Candidate *Parton2=NULL;
  const reco::Candidate *Met=NULL;

  for(size_t i = 0; i < nGen; ++ i) {
/*
     V = &((*genParticles)[i]);
    // The vector boson must have stutus==3  
    if( !(abs(V->status())==3) ) continue;
    size_t ndau = 0;
    if(!(V==NULL)) ndau = V->numberOfDaughters();
    // The vector boson must decay to leptons
    if(ndau<1) continue;
    if( (Vtype_=="Z") && !( V->pdgId()==22 || V->pdgId()==23) ) {
    if( (Vtype_=="W") && !(abs(V->pdgId())==24) ) {
	//std::cout<<V->pdgId()<<std::endl;
    // Loop over daugthers
    for(size_t j = 0; j < ndau; ++ j) {
      const reco::Candidate *d = V->daughter( j );
      // first look for Z --> l+l-
      if( !(d==NULL) && (V->pdgId()==23 || V->pdgId()==22) ) {
        if (d->pdgId()==-pdgIdDau_)  lepton1  = d;
        if (d->pdgId()==pdgIdDau_) lepton2 = d;
      } // if not, then look for W-->lnu
      else if( !(d==NULL) && abs(V->pdgId())==24) {
        if ( abs(d->pdgId())==pdgIdDau_ )  lepton1  = d;
        if ( abs(d->pdgId())==(pdgIdDau_+1) )  lepton2  = d;
      } 
    } // end ndaughter loop
}}
*/
//;;;;;;;;;;;;;;;;;;;;;;;;;;;my stuff;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    H = &((*genParticles)[i]);
    if( !((abs(H->status())==3) && (abs(H->pdgId())==25) ))continue;
  //Higgs must decay to W
	std::cout<<"hhhhhhhhhhhh    "<<H->pdgId()<<std::endl;
    size_t Hndau =0;
    size_t Wndau =0;
    if(!(H==NULL)) Hndau = H->numberOfDaughters();
//        std::cout<<"WWWW    "<<Hndau<<std::endl;
    if(Hndau<1) continue;
    for(size_t k =0; k< Hndau-1; k++){
//loop over higgs daughter
      const reco::Candidate *e = H->daughter( k );
      if( !(e==NULL) && (abs(e->pdgId())==24) ) {
//if higgs dau is W
        std::cout<<"WWWW    "<<e->pdgId()<<std::endl;
           Wndau = e->numberOfDaughters();
           if(Wndau<1) continue;
           for (size_t l =0; l<Wndau-1; l++){
//loop over W daughter
           const reco::Candidate *f = e->daughter( l );
        //std::cout<<" dau    "<<e->numberOfDaughters()<<std::endl;
        std::cout<<" dau    "<<f->pdgId()<<std::endl;
           if( !(f==NULL)&& (abs(f->pdgId())<=4)){ 
           Parton1 =e->daughter(0);
           Parton2 =e->daughter(1);}
           else if (!(f==NULL) && ((abs(f->pdgId())==12) ||(abs(f->pdgId())==14))) Met =f;
           else if (!(f==NULL) && ((abs(f->pdgId())==11) ||(abs(f->pdgId())==13))) Lepton =f;
	}
	}	
	} 

  ////////// Higgs boson quantities //////////////
if(H==NULL) return;
  H_mass = H->mass();
  H_Eta = H->eta();   
  H_Phi = H->phi();
  H_Vx = H->vx();
  H_Vy = H->vy();
  H_Vz = H->vz();
  H_Y  = H->rapidity();
  H_px = H->px();
  H_py = H->py();
  H_pz = H->pz();
  H_E  = H->energy();
  H_Pt = H->pt();
  H_Et = H->et();
  H_Id = H->pdgId();

} //nGen loop end
/*
  ////////// lepton #1 quantities //////////////
  if( !(lepton1 == NULL) ) {
    l1Charge           = lepton1-> charge();
    l1Vx               = lepton1->vx();
    l1Vy               = lepton1->vy();
    l1Vz               = lepton1->vz();
    l1Y                = lepton1->rapidity();
    l1Theta            = lepton1->theta();
    l1Eta              = lepton1->eta();    
    l1Phi              = lepton1->phi();
    l1E                = lepton1->energy();
    l1px               = lepton1->px();
    l1py               = lepton1->py();
    l1pz               = lepton1->pz();
    l1Pt               = lepton1->pt();
    l1Et               = lepton1->et();	  
  }

  ////////// lepton #2 quantities: in case of Z ///////
  if( !(lepton2 == NULL) ) {
    l2Charge          = lepton2->charge();
    l2Vx              = lepton2->vx();
    l2Vy              = lepton2->vy();
    l2Vz              = lepton2->vz();
    l2Y               = lepton2->rapidity();
    l2Theta           = lepton2->theta();
    l2Eta             = lepton2->eta();    
    l2Phi             = lepton2->phi();
    l2E               = lepton2->energy();
    l2px              = lepton2->px();
    l2py              = lepton2->py();
    l2pz              = lepton2->pz();
    l2Pt              = lepton2->pt();
    l2Et              = lepton2->et();	 
  } 
*/
//Parton filling
  if( !(Parton1 == NULL) ) {
  
    Parton_Charge[0]          = Parton1->charge();
    Parton_Vx[0]              = Parton1->vx();
    Parton_Vy[0]              = Parton1->vy();
    Parton_Vz[0]              = Parton1->vz();
    Parton_Y[0]               = Parton1->rapidity();
    Parton_Theta[0]           = Parton1->theta();
    Parton_Eta[0]             = Parton1->eta();
    Parton_Phi[0]             = Parton1->phi();
    Parton_E[0]               = Parton1->energy();
    Parton_px[0]              = Parton1->px();
    Parton_py[0]              = Parton1->py();
    Parton_pz[0]              = Parton1->pz();
    Parton_Pt[0]              = Parton1->pt();
    Parton_Et[0]              = Parton1->et(); 
    Parton_Id[0]              = Parton1->pdgId();
}

//Parton filling
  if( !(Parton2 == NULL) ) {

    Parton_Charge[1]          = Parton2->charge();
    Parton_Vx[1]              = Parton2->vx();
    Parton_Vy[1]              = Parton2->vy();
    Parton_Vz[1]              = Parton2->vz();
    Parton_Y[1]               = Parton2->rapidity();
    Parton_Theta[1]           = Parton2->theta();
    Parton_Eta[1]             = Parton2->eta();
    Parton_Phi[1]             = Parton2->phi();
    Parton_E[1]               = Parton2->energy();
    Parton_px[1]              = Parton2->px();
    Parton_py[1]              = Parton2->py();
    Parton_pz[1]              = Parton2->pz();
    Parton_Pt[1]              = Parton2->pt();
    Parton_Et[1]              = Parton2->et();
    Parton_Id[1]              = Parton2->pdgId();
}

//Lepton filling

  if( !(Lepton == NULL) ) {
    Lepton_Charge          = Lepton->charge();
    Lepton_Vx              = Lepton->vx();
    Lepton_Vy              = Lepton->vy();
    Lepton_Vz              = Lepton->vz();
    Lepton_Y               = Lepton->rapidity();
    Lepton_Theta           = Lepton->theta();
    Lepton_Eta             = Lepton->eta();
    Lepton_Phi             = Lepton->phi();
    Lepton_E               = Lepton->energy();
    Lepton_px              = Lepton->px();
    Lepton_py              = Lepton->py();
    Lepton_pz              = Lepton->pz();
    Lepton_Pt              = Lepton->pt();
    Lepton_Et              = Lepton->et();
    Lepton_Id              = Lepton->pdgId();
  }


//Met filling

  if( !(Met == NULL) ) {
    Met_Charge          = Met->charge();
    Met_Vx              = Met->vx();
    Met_Vy              = Met->vy();
    Met_Vz              = Met->vz();
    Met_Y               = Met->rapidity();
    Met_Theta           = Met->theta();
    Met_Eta             = Met->eta();
    Met_Phi             = Met->phi();
    Met_E               = Met->energy();
    Met_px              = Met->px();
    Met_py              = Met->py();
    Met_pz              = Met->pz();
    Met_Pt              = Met->pt();
    Met_Et              = Met->et();
    Met_Id              = Met->pdgId();
  }





}



////////////////// utilities, helpers ///////////////////
 
void ewk::MCTreeFiller::SetBranch( float* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/F").c_str() );
}


void ewk::MCTreeFiller::SetBranch( int* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/I").c_str() );
}


void ewk::MCTreeFiller::SetBranch( bool* x, std::string name)
{
  std::string brName = std::string(name_) + "_" + name;
  tree_->Branch( brName.c_str(), x, ( brName+"/O").c_str() );
}

