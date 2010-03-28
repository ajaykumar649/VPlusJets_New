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

// CMS includes
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"  
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "TMath.h" 
#include "JetMETCorrections/MCJet/plugins/JetUtilMC.h" // needed for dPhi,dR

// Monte Carlo stuff
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"


// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/JetTreeFiller.h"


ewk::JetTreeFiller::JetTreeFiller(const char *name, TTree* tree, 
					const std::string jetType,
					const edm::ParameterSet iConfig)
{

  // ********** CaloJets ********** //
  if( jetType=="Calo" && iConfig.existsAs<std::vector<edm::InputTag> >("srcCalo") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcCalo");    
  // ********** Corrected CaloJets ********** //
  else if( jetType=="Cor" && iConfig.existsAs<std::vector<edm::InputTag> >("srcCaloCor") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcCaloCor"); 
  // ********** GenJets ********** //
  else if( jetType=="Gen" && iConfig.existsAs<std::vector<edm::InputTag> >("srcGen") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcGen"); 
  // ********** PFJets ********** //
  else if( jetType=="PF" && iConfig.existsAs<std::vector<edm::InputTag> >("srcPFJets") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcPFJets"); 
  // ********** JetPlusTrack Jets ********** //
  else if( jetType=="JPT" && iConfig.existsAs<std::vector<edm::InputTag> >("srcJPTJets") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcJPTJets"); 
  else std::cout << "***Error:" << jetType << " Jet Type or Collection not specified !" 
		 << std::endl;


  // ********** Vector boson ********** //
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 


  //  ********** Jet Flavor identification (MC) ********** //
  doJetFlavorIdentification = false;
  if(  iConfig.existsAs<std::vector<edm::InputTag> >("srcFlavorByValue") ) {
    sourceByValue = iConfig.getParameter<std::vector<edm::InputTag> > ("srcFlavorByValue");
    doJetFlavorIdentification = true;
  }


  tree_     = tree;
  jetType_ = jetType;
  NumJetAlgo = mInputJets.size();
  
  if( !(tree==0 || NumJetAlgo==0) ) SetBranches();

}





void ewk::JetTreeFiller::SetBranches()
{
  // Declare jet branches
  std::string abrname = "Num" + jetType_ + "JetAlgo";
  tree_->Branch( abrname.c_str(), &NumJetAlgo, (abrname +"/I").c_str() );
  abrname = "Num" + jetType_ + "Jets";
  tree_->Branch( abrname.c_str(), &(NumJets[0]), (abrname +"[6]/I").c_str() );
  if(doJetFlavorIdentification) 
    SetBranch( &(Flavor[0][0]), "Jet" + jetType_ + "Flavor");
  SetBranch( &(Et[0][0]), "Jet" + jetType_ + "Et");
  SetBranch( &(Pt[0][0]), "Jet" + jetType_ + "Pt");
  SetBranch( &(Eta[0][0]), "Jet" + jetType_ + "Eta");
  SetBranch( &(Phi[0][0]), "Jet" + jetType_ + "Phi");
  SetBranch( &(Theta[0][0]), "Jet" + jetType_ + "Theta");
  SetBranch( &(Px[0][0]), "Jet" + jetType_ + "Px");
  SetBranch( &(Py[0][0]), "Jet" + jetType_ + "Py");
  SetBranch( &(Pz[0][0]), "Jet" + jetType_ + "Pz");
  SetBranch( &(E[0][0]), "Jet" + jetType_ + "E");
  SetBranch( &(Y[0][0]), "Jet" + jetType_ + "Y");
  SetBranch( &(VjetMass[0][0]), "VplusLead" + jetType_ + "JetMass");
  SetBranch( &(Dphi[0][0]), "Jet" + jetType_ + "Dphi");
  SetBranch( &(Deta[0][0]), "VplusLead" + jetType_ + "Deta");
  SetBranch( &(DR[0][0]), "VplusLead" + jetType_ + "DR");
  SetBranch( &(Response[0][0]), "Jet" + jetType_ + "Response");

  if( jetType_ == "Calo" || jetType_ == "Cor" || jetType_ == "JPT") {

    /** Returns the maximum energy deposited in ECAL towers*/
    SetBranch( &(MaxEInEmTowers[0][0]), "Jet" + jetType_ + "MaxEInEmTowers");
    /** Returns the maximum energy deposited in HCAL towers*/
    SetBranch( &(MaxEInHadTowers[0][0]), "Jet" + jetType_ + "MaxEInHadTowers");
    /** Returns the jet hadronic energy fraction*/
    SetBranch( &(EnergyFractionHadronic[0][0]), "Jet" + jetType_ + "EnergyFractionHadronic");
    /** Returns the jet electromagnetic energy fraction*/
    SetBranch( &(EmEnergyFraction[0][0]), "Jet" + jetType_ + "EmEnergyFraction");
    /** Returns the jet hadronic energy in HB*/ 
    SetBranch( &(HadEnergyInHB[0][0]), "Jet" + jetType_ + "HadEnergyInHB");
    /** Returns the jet hadronic energy in HO*/
    SetBranch( &(HadEnergyInHO[0][0]), "Jet" + jetType_ + "HadEnergyInHO");
    /** Returns the jet hadronic energy in HE*/
    SetBranch( &(HadEnergyInHE[0][0]), "Jet" + jetType_ + "HadEnergyInHE");
    /** Returns the jet hadronic energy in HF*/
    SetBranch( &(HadEnergyInHF[0][0]), "Jet" + jetType_ + "HadEnergyInHF");
    /** Returns the jet electromagnetic energy in EB*/
    SetBranch( &(EmEnergyInEB[0][0]), "Jet" + jetType_ + "EmEnergyInEB");
    /** Returns the jet electromagnetic energy in EE*/
    SetBranch( &(EmEnergyInEE[0][0]), "Jet" + jetType_ + "EmEnergyInEE");
    /** Returns the jet electromagnetic energy extracted from HF*/
    SetBranch( &(EmEnergyInHF[0][0]), "Jet" + jetType_ + "EmEnergyInHF");
    /** Returns area of contributing towers */
    SetBranch( &(TowersArea[0][0]), "Jet" + jetType_ + "TowersArea");
    /** Number of constituents carrying a 90% of the total Jet energy*/
    SetBranch( &(N90[0][0]), "Jet" + jetType_ + "N90");
    /** Number of constituents carrying a 60% of the total Jet energy*/
    SetBranch( &(N60[0][0]), "Jet" + jetType_ + "N60");
  }
  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "Gen") {
    /** Returns energy of electromagnetic particles*/
    SetBranch( &(GenEmEnergy[0][0]), "JetGenEmEnergy");
    /** Returns energy of hadronic particles*/
    SetBranch( &(GenHadEnergy[0][0]), "JetGenHadEnergy");
    /** Returns invisible energy*/
    SetBranch( &(GenInvisibleEnergy[0][0]), "JetGenInvisibleEnergy");
    /** Returns other energy (undecayed Sigmas etc.)*/
    SetBranch( &(GenAuxiliaryEnergy[0][0]), "JetGenAuxiliaryEnergy");
  }

  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "PF") {
    /// chargedHadronEnergy 
    SetBranch( &(PFChargedHadronEnergy[0][0]), "JetPFChargedHadronEnergy");
    ///  chargedHadronEnergyFraction
    SetBranch( &(PFChargedHadronEnergyFraction[0][0]), "JetPFChargedHadronEnergyFraction");
    /// neutralHadronEnergy
    SetBranch( &(PFNeutralHadronEnergy[0][0]), "JetPFNeutralHadronEnergy");
    /// neutralHadronEnergyFraction
    SetBranch( &(PFNeutralHadronEnergyFraction[0][0]), "JetPFNeutralHadronEnergyFraction");
    /// chargedEmEnergy
    SetBranch( &(PFChargedEmEnergy[0][0]), "JetPFChargedEmEnergy");
    /// chargedEmEnergyFraction
    SetBranch( &(PFChargedEmEnergyFraction[0][0]), "JetPFChargedEmEnergyFraction");
    /// chargedMuEnergy
    SetBranch( &(PFChargedMuEnergy[0][0]), "JetPFChargedMuEnergy");
    /// chargedMuEnergyFraction
    SetBranch( &(PFChargedMuEnergyFraction[0][0]), "JetPFChargedMuEnergyFraction");
    /// neutralEmEnergy
    SetBranch( &(PFNeutralEmEnergy[0][0]), "JetPFNeutralEmEnergy");
    /// neutralEmEnergyFraction
    SetBranch( &(PFNeutralEmEnergyFraction[0][0]), "JetPFNeutralEmEnergyFraction");
    /// chargedMultiplicity
    SetBranch( &(PFChargedMultiplicity[0][0]), "JetPFChargedMultiplicity");
    /// neutralMultiplicity
    SetBranch( &(PFNeutralMultiplicity[0][0]), "JetPFNeutralMultiplicity");
    /// muonMultiplicity
    SetBranch( &(PFMuonMultiplicity[0][0]), "JetPFMuonMultiplicity");
  }
}


//////////////////////////////////////////////////////////////////
/////// Helper for above function ////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ewk::JetTreeFiller::SetBranch( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6][6]/F").c_str() );
  bnames.push_back( name );
}


void ewk::JetTreeFiller::SetBranch( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6][6]/I").c_str() );
  bnames.push_back( name );
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////







void ewk::JetTreeFiller::FillBranches() const 
{

  for(std::vector<std::string>::iterator it = bnames.begin();
      it != bnames.end(); ++it) {
    if(TBranch *br = tree_->GetBranch( (*it).c_str() ) ) br->Fill();
  }

}




void ewk::JetTreeFiller::init()   
{
  // initialize private data members

  for (int i =0; i< NUM_ALGO_MAX; ++i) {
    NumJets[i] = 0; 
    for (int j =0; j< NUM_JET_MAX; ++j) {

      Et[i][j] = -1.0;
      Pt[i][j] = -1.0;
      Eta[i][j] = -10.0;
      Phi[i][j] = -10.0;
      Theta[i][j] = -10.0;
      E[i][j] = -1.0;
      Y[i][j] = -10.0;
      Px[i][j] = -999999.9;
      Py[i][j] = -999999.9;
      Pz[i][j] = -999999.9;
      Flavor[i][j] = -1;
      MaxEInEmTowers[i][j] = -1.0;
      MaxEInHadTowers[i][j] = -1.0;
      EnergyFractionHadronic[i][j] = -1.0;
      EmEnergyFraction[i][j] = -1.0;
      HadEnergyInHB[i][j] = -1.0;
      HadEnergyInHO[i][j] = -1.0;
      HadEnergyInHE[i][j] = -1.0;
      HadEnergyInHF[i][j] = -1.0;
      EmEnergyInEB[i][j] = -1.0;
      EmEnergyInEE[i][j] = -1.0;
      EmEnergyInHF[i][j] = -1.0;
      TowersArea[i][j] = -1.0;
      VjetMass[i][j] = -1.0;
      N90[i][j] = -1; 
      N60[i][j] = -1;     
      Dphi[i][j] = -10.0;
      Deta[i][j] = -10.0;
      DR[i][j] = -10.0;
      Response[i][j] = -1.0;

      GenEmEnergy[i][j] = -1.0;
      GenHadEnergy[i][j] = -1.0;
      GenInvisibleEnergy[i][j] = -1.0;
      GenAuxiliaryEnergy[i][j] = -1.0;

      PFChargedHadronEnergy[i][j] = -1.0;
      PFChargedHadronEnergyFraction[i][j] = -1.0;
      PFNeutralHadronEnergy[i][j] = -1.0;
      PFNeutralHadronEnergyFraction[i][j] = -1.0;
      PFChargedEmEnergy[i][j] = -1.0;
      PFChargedEmEnergyFraction[i][j] = -1.0;
      PFChargedMuEnergy[i][j] = -1.0;
      PFChargedMuEnergyFraction[i][j] = -1.0;
      PFNeutralEmEnergy[i][j] = -1.0;
      PFNeutralEmEnergyFraction[i][j] = -1.0;
      PFChargedMultiplicity[i][j] = -1;
      PFNeutralMultiplicity[i][j] = -1;
      PFMuonMultiplicity[i][j] = -1;
    }
  }
  // initialization done
}



void ewk::JetTreeFiller::fill(const edm::Event& iEvent)
{
  // first initialize to the default values
  init();

  if( NumJetAlgo < 1 ) return; // Nothing to fill

  edm::Handle<reco::CandidateView> boson;
  iEvent.getByLabel( mInputBoson, boson);
  if( boson->size()<1 ) return; // Nothing to fill
  
  const reco::Candidate *Vboson = &((*boson)[0]); 
  
  for (size_t ic = 0;  ic <  (unsigned int) NumJetAlgo; ic++) {
    edm::Handle<edm::View<reco::Jet> > jets;
    iEvent.getByLabel( mInputJets[ic], jets);
 
    /****************    Jet Flavor    ***************/
    edm::Handle<reco::JetFlavourMatchingCollection> theTagByValue; 
    if(doJetFlavorIdentification) {
      iEvent.getByLabel (sourceByValue[ic], theTagByValue );   
    }   
    /****************    Jet Flavor    ***************/

    if (jets.isValid()) {
      if(jets->size() < 1) continue; 
      int ij = 0;
      for (edm::View<reco::Jet>::const_iterator jet = jets->begin (); 
	   jet != jets->end (); ++jet, ++ij) {
	if( !(ij < NUM_JET_MAX) ) continue; 

	Et[ic][ij] = (*jet).et();
	Pt[ic][ij] = (*jet).pt();
	Eta[ic][ij] = (*jet).eta();
	Phi[ic][ij] = (*jet).phi();
	Theta[ic][ij] = (*jet).theta();
	Px[ic][ij] = (*jet).px();
	Py[ic][ij] = (*jet).py();
	Pz[ic][ij] = (*jet).pz();
	E[ic][ij]  = (*jet).energy();
	Y[ic][ij]  = (*jet).rapidity();
	Dphi[ic][ij] = dPhi( (*jet).phi(), Vboson->phi() );
	Deta[ic][ij] = fabs( (*jet).eta() - Vboson->eta() );
	DR[ic][ij] = radius( (*jet).eta(), (*jet).phi(), 
			     Vboson->eta(), Vboson->phi());
	Response[ic][ij] = 10.0;
	float vpt = Vboson->pt();
	if( vpt>0.0 ) Response[ic][ij] = (*jet).pt() / vpt;

	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > qstar 
	  = (*jet).p4() + Vboson->p4();
	VjetMass[ic][ij] = qstar.M();

	if(doJetFlavorIdentification) {
	  int flavor = -1;
	  for ( reco::JetFlavourMatchingCollection::const_iterator jfm  = 
		  theTagByValue->begin();
		jfm != theTagByValue->end(); jfm ++ ) {
	    edm::RefToBase<reco::Jet> aJet  = (*jfm).first;   
	    const reco::JetFlavour aFlav = (*jfm).second;
	    double dist = radius(aJet->eta(), aJet->phi(), 
				 (*jet).eta(), (*jet).phi());
	    if( dist < 0.0001 ) flavor = abs(aFlav.getFlavour()); 
	  }
	  Flavor[ic][ij] = flavor;
	}
	// CaloJet specific quantities
	const std::type_info & type = typeid(*jet);
	if ( type == typeid(reco::CaloJet) ) {
	  const reco::CaloJet calojet = static_cast<const reco::CaloJet &>(*jet);

	  MaxEInEmTowers[ic][ij] = calojet.maxEInEmTowers();
	  MaxEInHadTowers[ic][ij] = calojet.maxEInHadTowers();
	  EnergyFractionHadronic[ic][ij] 
	    = calojet.energyFractionHadronic();
	  EmEnergyFraction[ic][ij] 
	    = calojet.emEnergyFraction();
	  HadEnergyInHB[ic][ij] = calojet.hadEnergyInHB();
	  HadEnergyInHO[ic][ij] = calojet.hadEnergyInHO();
	  HadEnergyInHE[ic][ij] = calojet.hadEnergyInHE();
	  HadEnergyInHF[ic][ij] = calojet.hadEnergyInHF();
	  EmEnergyInEB[ic][ij] = calojet.emEnergyInEB();
	  EmEnergyInEE[ic][ij] = calojet.emEnergyInEE();
	  EmEnergyInHF[ic][ij] = calojet.emEnergyInHF();
	  TowersArea[ic][ij] = calojet.towersArea();
	  N90[ic][ij] = calojet.n90();
	  N60[ic][ij] = calojet.n60(); 
	}
	if ( type == typeid(reco::GenJet) ) {
	  // GenJet specific quantities
	  reco::GenJet genjet = static_cast<const reco::GenJet &>(*jet);
	  GenEmEnergy[ic][ij] = genjet.emEnergy();
	  GenHadEnergy[ic][ij] = genjet.hadEnergy();
	  GenInvisibleEnergy[ic][ij] = genjet.invisibleEnergy();
	  GenAuxiliaryEnergy[ic][ij] = genjet.auxiliaryEnergy();	  
	}
	if ( type == typeid(reco::PFJet) ) {
	  // PFJet specific quantities
	  reco::PFJet pfjet = static_cast<const reco::PFJet &>(*jet);
	  PFChargedHadronEnergy[ic][ij] = pfjet.chargedHadronEnergy();
	  PFChargedHadronEnergyFraction[ic][ij] = 
	    pfjet.chargedHadronEnergyFraction ();
	  PFNeutralHadronEnergy[ic][ij] = pfjet.neutralHadronEnergy();
	  PFNeutralHadronEnergyFraction[ic][ij] = 
	    pfjet.neutralHadronEnergyFraction ();
	  PFChargedEmEnergy[ic][ij] = pfjet.chargedEmEnergy ();
	  PFChargedEmEnergyFraction[ic][ij] = 
	    pfjet.chargedEmEnergyFraction ();
	  PFChargedMuEnergy[ic][ij] = pfjet.chargedMuEnergy ();
	  PFChargedMuEnergyFraction[ic][ij] = 
	    pfjet.chargedMuEnergyFraction ();
	  PFNeutralEmEnergy[ic][ij] = pfjet.neutralEmEnergy ();
	  PFNeutralEmEnergyFraction[ic][ij] = 
	    pfjet.neutralEmEnergyFraction ();
	  PFChargedMultiplicity[ic][ij] = pfjet.chargedMultiplicity();
	  PFNeutralMultiplicity[ic][ij] = pfjet.neutralMultiplicity();
	  PFMuonMultiplicity[ic][ij] = pfjet.muonMultiplicity();  
	}// close PF jets loop

	NumJets[ic]++;
      }// close jets iteration loop
    } // close isValid loop

  } // Close JetAlgo loop

  //FillBranches();
}

