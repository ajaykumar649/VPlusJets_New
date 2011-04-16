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
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "TMath.h" 
#include "JetMETCorrections/MCJet/plugins/JetUtilMC.h" // needed for dPhi,dR

// Monte Carlo stuff
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/JetTreeFiller.h"


ewk::JetTreeFiller::JetTreeFiller(const char *name, TTree* tree, 
					const std::string jetType,
					const edm::ParameterSet iConfig)
{

  // ********** CaloJets ********** //
  if( jetType=="Calo" && iConfig.existsAs<edm::InputTag>("srcCalo") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcCalo");    
  // ********** Corrected CaloJets ********** //
  if( jetType=="Cor" && iConfig.existsAs<edm::InputTag>("srcCaloCor") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcCaloCor"); 
  // ********** GenJets ********** //
  if( jetType=="Gen" && iConfig.existsAs<edm::InputTag>("srcGen") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcGen"); 
  // ********** PFJets ********** //
  if( jetType=="PF" && iConfig.existsAs<edm::InputTag>("srcPFJets") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcPFJets"); 
  // ********** Corrected PFJets ********** //
  if( jetType=="PFCor" && iConfig.existsAs<edm::InputTag>("srcPFCor") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcPFCor"); 
  // ********** JetPlusTrack Jets ********** //
  if( jetType=="JPT" && iConfig.existsAs<edm::InputTag>("srcJPTJets") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcJPTJets"); 
  // ********** Corrected JetPlusTrack Jets ********** //
  if( jetType=="JPTCor" && iConfig.existsAs<edm::InputTag>("srcJPTCor") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcJPTCor"); 

  
  // ********** Vector boson ********** //
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 


  //  ********** Jet Flavor identification (MC) ********** //
  doJetFlavorIdentification = false;
  if( (iConfig.existsAs<bool>("runningOverMC") && iConfig.getParameter<bool>("runningOverMC") && 
       iConfig.existsAs<edm::InputTag>("srcFlavorByValue") )) {
    sourceByValue = iConfig.getParameter<edm::InputTag> ("srcFlavorByValue");
    doJetFlavorIdentification = true;
  }

  tree_     = tree;
  jetType_ = jetType;

  if( !(tree==0) ) SetBranches();
}





void ewk::JetTreeFiller::SetBranches()
{
  // Declare jet branches
  SetBranch( &NumJets, "num" + jetType_ + "Jets");
  SetBranch( &numBTags, "num" + jetType_ + "JetBTags");

  if(doJetFlavorIdentification) 
     SetBranch( Flavor, "Jet" + jetType_ + "_Flavor");
  SetBranch( Et, "Jet" + jetType_ + "_Et");
  SetBranch( Pt, "Jet" + jetType_ + "_Pt");
  SetBranch( Eta, "Jet" + jetType_ + "_Eta");
  SetBranch( Phi, "Jet" + jetType_ + "_Phi");
  SetBranch( Theta, "Jet" + jetType_ + "_Theta");
  SetBranch( Px, "Jet" + jetType_ + "_Px");
  SetBranch( Py, "Jet" + jetType_ + "_Py");
  SetBranch( Pz, "Jet" + jetType_ + "_Pz");
  SetBranch( E, "Jet" + jetType_ + "_E");
  SetBranch( Y, "Jet" + jetType_ + "_Y");
  SetBranch( Mass, "Jet" + jetType_ + "_Mass");

  /// eta-eta second moment, ET weighted
  SetBranch( etaetaMoment, "Jet" + jetType_ + "_etaetaMoment");
  /// phi-phi second moment, ET weighted
  SetBranch( phiphiMoment, "Jet" + jetType_ + "_phiphiMoment");
  /// eta-phi second moment, ET weighted
  SetBranch( etaphiMoment, "Jet" + jetType_ + "_etaphiMoment");
  /// maximum distance from jet to constituent
  SetBranch( maxDistance, "Jet" + jetType_ + "_maxDistance");
  /// # of constituents
  SetBranch( nConstituents, "Jet" + jetType_ + "_nConstituents");

  SetBranch( VjetMass, "Vplus" + jetType_ + "Jet_Mass");
  SetBranch( Dphi, "Jet" + jetType_ + "_dphiBoson");
  SetBranch( Deta, "Jet" + jetType_ + "_detaBoson");
  SetBranch( DR, "Jet" + jetType_ + "_dRBoson");
  SetBranch( DphiMET, "Jet" + jetType_ + "_dphiMET");
  SetBranch( Response, "Jet" + jetType_ + "_Response");
  SetBranch( bDiscriminator, "Jet" + jetType_ + "_bDiscriminator");
  SetBranch( secVertexMass, "Jet" + jetType_ + "_secVertexMass");
  SetBranch( Dphi2, "Jet" + jetType_ + "_dphiBoson2");
  SetBranch( Deta2, "Jet" + jetType_ + "_detaBoson2");
  SetBranch( DR2, "Jet" + jetType_ + "_dRBoson2");
  SetBranch( VjetMass2, "Vplus" + jetType_ + "Jet_Mass2");
  SetBranch( Response2, "Jet" + jetType_ + "_Response2");


  if( jetType_ == "Calo" || jetType_ == "Cor" || 
      jetType_ == "JPT" || jetType_ == "JPTCor") {
    /** Returns the jet hadronic energy fraction*/
    SetBranch( EnergyFractionHadronic, "Jet" + jetType_ + "_EnergyFractionHadronic");
    /** Returns the jet electromagnetic energy fraction*/
    SetBranch( EmEnergyFraction, "Jet" + jetType_ + "_EmEnergyFraction");
    /** Returns area of contributing towers */
    SetBranch( TowersArea, "Jet" + jetType_ + "_TowersArea");
    /** Number of constituents carrying a 90% of the total Jet energy*/
    SetBranch( N90, "Jet" + jetType_ + "_N90");
  }
  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "Gen") {
    /** Returns energy of electromagnetic particles*/
    SetBranch( GenEmEnergy, "Jet" + jetType_ + "_EmEnergy");
    /** Returns energy of hadronic particles*/
    SetBranch( GenHadEnergy, "Jet" + jetType_ + "_HadEnergy");
    /** Returns invisible energy*/
    SetBranch( GenInvisibleEnergy, "Jet" + jetType_ + "_InvisibleEnergy");
    /** Returns other energy (undecayed Sigmas etc.)*/
    SetBranch( GenAuxiliaryEnergy, "Jet" + jetType_ + "_AuxiliaryEnergy");
  }

  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "PF" || jetType_ == "PFCor") {
    /// chargedHadronEnergy 
    SetBranch( PFChargedHadronEnergy, "Jet" + jetType_ + "_ChargedHadronEnergy");
    ///  chargedHadronEnergyFraction
    SetBranch( PFChargedHadronEnergyFraction, "Jet" + jetType_ + "_ChargedHadronEnergyFrac");
    /// neutralHadronEnergy
    SetBranch( PFNeutralHadronEnergy, "Jet" + jetType_ + "_NeutralHadronEnergy");
    /// neutralHadronEnergyFraction
    SetBranch( PFNeutralHadronEnergyFraction, "Jet" + jetType_ + "_NeutralHadronEnergyFrac");
    /// chargedEmEnergy
    SetBranch( PFChargedEmEnergy, "Jet" + jetType_ + "_ChargedEmEnergy");
    /// chargedEmEnergyFraction
    SetBranch( PFChargedEmEnergyFraction, "Jet" + jetType_ + "_ChargedEmEnergyFrac");
    /// chargedMuEnergy
    SetBranch( PFChargedMuEnergy, "Jet" + jetType_ + "_ChargedMuEnergy");
    /// chargedMuEnergyFraction
    SetBranch( PFChargedMuEnergyFraction, "Jet" + jetType_ + "_ChargedMuEnergyFrac");
    /// neutralEmEnergy
    SetBranch( PFNeutralEmEnergy, "Jet" + jetType_ + "_NeutralEmEnergy");
    /// neutralEmEnergyFraction
    SetBranch( PFNeutralEmEnergyFraction, "Jet" + jetType_ + "_NeutralEmEnergyFrac");
    /// chargedMultiplicity
    SetBranch( PFChargedMultiplicity, "Jet" + jetType_ + "_ChargedMultiplicity");
    /// neutralMultiplicity
    SetBranch( PFNeutralMultiplicity, "Jet" + jetType_ + "_NeutralMultiplicity");
    /// muonMultiplicity
    SetBranch( PFMuonMultiplicity, "Jet" + jetType_ + "_MuonMultiplicity");
    /// photonEnergy 
    SetBranch( PFPhotonEnergy, "Jet" + jetType_ + "_PhotonEnergy");
    /// photonEnergyFraction 
    SetBranch( PFPhotonEnergyFraction, "Jet" + jetType_ + "_PhotonEnergyFraction");
    /// electronEnergy 
    SetBranch( PFElectronEnergy, "Jet" + jetType_ + "_ElectronEnergy"); 
    /// electronEnergyFraction 
    SetBranch( PFElectronEnergyFraction, "Jet" + jetType_ + "_ElectronEnergyFraction");
    /// muonEnergy 
    SetBranch( PFMuonEnergy, "Jet" + jetType_ + "_MuonEnergy"); 
    /// muonEnergyFraction 
    SetBranch( PFMuonEnergyFraction, "Jet" + jetType_ + "_MuonEnergyFraction");
  /// HFHadronEnergy  
    SetBranch( PFHFHadronEnergy, "Jet" + jetType_ + "_HFHadronEnergy");
  /// HFHadronEnergyFraction 
    SetBranch( PFHFHadronEnergyFraction, "Jet" + jetType_ + "_HFHadronEnergyFraction");
  /// HFEMEnergy  
    SetBranch( PFHFEMEnergy, "Jet" + jetType_ + "_HFEMEnergy");
  /// HFEMEnergyFraction 
    SetBranch( PFHFEMEnergyFraction, "Jet" + jetType_ + "_HFEMEnergyFraction");
  /// chargedHadronMultiplicity 
    SetBranch( PFChargedHadronMultiplicity, "Jet" + jetType_ + "_ChargedHadronMultiplicity");
  /// neutralHadronMultiplicity 
    SetBranch( PFNeutralHadronMultiplicity, "Jet" + jetType_ + "_NeutralHadronMultiplicity");
  /// photonMultiplicity 
    SetBranch( PFPhotonMultiplicity, "Jet" + jetType_ + "_PhotonMultiplicity");
  /// electronMultiplicity 
    SetBranch( PFElectronMultiplicity, "Jet" + jetType_ + "_ElectronMultiplicity");
  /// HFHadronMultiplicity 
    SetBranch( PFHFHadronMultiplicity, "Jet" + jetType_ + "_HFHadronMultiplicity");
  /// HFEMMultiplicity 
    SetBranch( PFHFEMMultiplicity, "Jet" + jetType_ + "_HFEMMultiplicity");
  }
  
  SetBranch( &V2jMass, "MassV2j_" + jetType_);
  SetBranch( &V3jMass, "MassV3j_" + jetType_);
  SetBranch( &V4jMass, "MassV4j_" + jetType_);
  SetBranch( &V5jMass, "MassV5j_" + jetType_);
  SetBranch( &V6jMass, "MassV6j_" + jetType_);
  SetBranch( &c2jMass, "Mass2j_" + jetType_);
  SetBranch( &c3jMass, "Mass3j_" + jetType_);
  SetBranch( &c4jMass, "Mass4j_" + jetType_);
  SetBranch( &c5jMass, "Mass5j_" + jetType_);
  SetBranch( &c6jMass, "Mass6j_" + jetType_);
}


//////////////////////////////////////////////////////////////////
/////// Helper for above function ////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ewk::JetTreeFiller::SetBranch( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6]/F").c_str() );
  bnames.push_back( name );
}


void ewk::JetTreeFiller::SetBranch( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6]/I").c_str() );
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
   NumJets = 0; 
   numBTags = 0;

   for (int j =0; j< NUM_JET_MAX; ++j) {
      Et[j] = -1.0;
      Pt[j] = -1.0;
      Eta[j] = -10.0;
      Phi[j] = -10.0;
      Theta[j] = -10.0;
      E[j] = -1.0;
      Y[j] = -10.0;
      etaetaMoment[j]  = -10.0;  
      phiphiMoment[j]  = -10.0;      
      etaphiMoment[j]  = -10.0;      
      maxDistance[j]  = -10.0;
      nConstituents[j]  = 10;
      Px[j] = -999999.9;
      Py[j] = -999999.9;
      Pz[j] = -999999.9;
      Flavor[j] = -1;
      Mass[j] = -1.0;
      MaxEInEmTowers[j] = -1.0;
      MaxEInHadTowers[j] = -1.0;
      EnergyFractionHadronic[j] = -1.0;
      EmEnergyFraction[j] = -1.0;
      HadEnergyInHB[j] = -1.0;
      HadEnergyInHO[j] = -1.0;
      HadEnergyInHE[j] = -1.0;
      HadEnergyInHF[j] = -1.0;
      EmEnergyInEB[j] = -1.0;
      EmEnergyInEE[j] = -1.0;
      EmEnergyInHF[j] = -1.0;
      TowersArea[j] = -1.0;
      VjetMass[j] = -1.0;
      N90[j] = -1; 
      N60[j] = -1;     
      Dphi[j] = -10.0;
      Deta[j] = -10.0;
      DR[j] = -10.0;
      DphiMET[j] = -10.0;
      Response[j] = -1.0;
      bDiscriminator[j] = -1.0;
      secVertexMass[j] = -1.0;

      VjetMass2[j] = -1.0;
      DR2[j] = -10.0;
      Dphi2[j] = -10.0;
      Deta2[j] = -10.0;
      Response2[j] = -1.0;

      GenEmEnergy[j] = -1.0;
      GenHadEnergy[j] = -1.0;
      GenInvisibleEnergy[j] = -1.0;
      GenAuxiliaryEnergy[j] = -1.0;

      PFChargedHadronEnergy[j] = -1.0;
      PFChargedHadronEnergyFraction[j] = -1.0;
      PFNeutralHadronEnergy[j] = -1.0;
      PFNeutralHadronEnergyFraction[j] = -1.0;
      PFChargedEmEnergy[j] = -1.0;
      PFChargedEmEnergyFraction[j] = -1.0;
      PFChargedMuEnergy[j] = -1.0;
      PFChargedMuEnergyFraction[j] = -1.0;
      PFNeutralEmEnergy[j] = -1.0;
      PFNeutralEmEnergyFraction[j] = -1.0;
      PFChargedMultiplicity[j] = -1;
      PFNeutralMultiplicity[j] = -1;
      PFMuonMultiplicity[j] = -1;
      PFPhotonEnergy[j] = -1.0;
      PFPhotonEnergyFraction[j] = -1.0;
      PFElectronEnergy[j] = -1.0;
      PFElectronEnergyFraction[j] = -1.0;
      PFMuonEnergy[j] = -1.0;
      PFMuonEnergyFraction[j] = -1.0;
      PFHFHadronEnergy[j] = -1.0;
      PFHFHadronEnergyFraction[j] = -1.0;
      PFHFEMEnergy[j] = -1.0;
      PFHFEMEnergyFraction[j] = -1.0;	 
      PFChargedHadronMultiplicity[j] = -1;
      PFNeutralHadronMultiplicity[j] = -1;
      PFPhotonMultiplicity[j] = -1;
      PFElectronMultiplicity[j] = -1;
      PFHFHadronMultiplicity[j] = -1;
      PFHFEMMultiplicity[j] = -1;
   }
   // initialization done

   V2jMass = -1.0;
   V3jMass = -1.0;
   V4jMass = -1.0;
   V5jMass = -1.0;
   V6jMass = -1.0;
   c2jMass = -1.0;
   c3jMass = -1.0;
   c4jMass = -1.0;
   c5jMass = -1.0;
   c6jMass = -1.0;
}



void ewk::JetTreeFiller::fill(const edm::Event& iEvent)
{
   // first initialize to the default values
   init();


   edm::Handle<reco::CandidateView> boson;
   iEvent.getByLabel( mInputBoson, boson);
   const int nBoson = boson->size();
   if( nBoson<1 ) return; // Nothing to fill
  
   const reco::Candidate *Vboson = &((*boson)[0]); 

   // in case when we have two candidates for the W boson in the event
   const reco::Candidate *Vboson2(0);
   if( nBoson==2) Vboson2  = &((*boson)[1]);


   edm::Handle<edm::View<reco::Jet> > jets;
   iEvent.getByLabel( mInputJets, jets ); 


   if(jets->size() < 1) return;

   /////// PfMET information /////
   edm::Handle<reco::PFMETCollection> pfmet;
   iEvent.getByLabel("pfMet", pfmet);

   // Get b tag information
   edm::Handle<reco::JetTagCollection> bTagHandle;
   iEvent.getByLabel("simpleSecondaryVertexHighEffBJetTags", bTagHandle);
   const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
   edm::Handle<reco::SecondaryVertexTagInfoCollection> svTagInfos;
   iEvent.getByLabel("secondaryVertexTagInfos", svTagInfos);
  
  
   size_t iJet = 0;
   double dist = 100000.0;
   NumJets = 0;
   numBTags = 0;


   /****************    MC Jet Flavor    ***************/
   edm::Handle<reco::JetFlavourMatchingCollection> theTagByValue; 
   if(doJetFlavorIdentification) 
      iEvent.getByLabel (sourceByValue, theTagByValue );   
 
   edm::View<reco::Jet>::const_iterator jet, endpjets = jets->end(); 
   for (jet = jets->begin();  jet != endpjets;  ++jet, ++iJet) {

      if( !(iJet< (unsigned int) NUM_JET_MAX) ) break;
      Et[iJet] = (*jet).et();
      Pt[iJet] = (*jet).pt();
      Eta[iJet] = (*jet).eta();
      Phi[iJet] = (*jet).phi();
      Theta[iJet] = (*jet).theta();
      Px[iJet] = (*jet).px();
      Py[iJet] = (*jet).py();
      Pz[iJet] = (*jet).pz();
      E[iJet]  = (*jet).energy();
      Y[iJet]  = (*jet).rapidity();
      Mass[iJet] = (*jet).mass();
      etaetaMoment[iJet]  = (*jet).etaetaMoment();  
      phiphiMoment[iJet]  = (*jet).phiphiMoment();      
      etaphiMoment[iJet]  = (*jet).etaphiMoment();      
      maxDistance[iJet]  = (*jet).maxDistance();
      nConstituents[iJet]  = (*jet).nConstituents();

      Dphi[iJet] = dPhi( (*jet).phi(), Vboson->phi() );
      Deta[iJet] = fabs( (*jet).eta() - Vboson->eta() );
      DR[iJet] = radius( (*jet).eta(), (*jet).phi(), 
			 Vboson->eta(), Vboson->phi());
      DphiMET[iJet] = dPhi( (*jet).phi(), (*pfmet)[0].phi() );

      Response[iJet] = 10.0;
      float vpt = Vboson->pt();
      if( vpt>0.0 ) Response[iJet] = (*jet).pt() / vpt;

      ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > qstar 
         = (*jet).p4() + Vboson->p4();
      VjetMass[iJet] = qstar.M();


      if( nBoson==2) {
	DR2[iJet] = radius( (*jet).eta(), (*jet).phi(), 
			    Vboson2->eta(), Vboson2->phi());
	Dphi2[iJet] = dPhi( (*jet).phi(), Vboson2->phi() );
	Deta2[iJet] = fabs( (*jet).eta() - Vboson2->eta() );
	Response2[iJet] = 10.0;
	vpt = Vboson2->pt();
	if( vpt>0.0 ) Response2[iJet] = (*jet).pt() / vpt;	
	qstar = (*jet).p4() + Vboson2->p4();
	VjetMass2[iJet] = qstar.M();	
      }


      if(doJetFlavorIdentification) {
         int flavor = -1;
         for ( reco::JetFlavourMatchingCollection::const_iterator jfm  = 
                  theTagByValue->begin();
               jfm != theTagByValue->end(); jfm ++ ) {
            edm::RefToBase<reco::Jet> aJet  = (*jfm).first;   
            const reco::JetFlavour aFlav = (*jfm).second;
            dist = radius(aJet->eta(), aJet->phi(), 
			(*jet).eta(), (*jet).phi());
            if( dist < 0.0001 ) flavor = abs(aFlav.getFlavour()); 
         }
         Flavor[iJet] = flavor;
      }

      // Loop over jets and study b tag info.
      double closestDistance = 100000.0;
      unsigned int closestIndex = 10000;

      for (unsigned int i = 0; i != bTags.size(); ++i) {
         edm::RefToBase<reco::Jet> aJet  = bTags[i].first;   
         dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
         if( dist < closestDistance ) { 
            closestDistance = dist;
            closestIndex = i;
         }
      }
      // compute B-tag discriminator
      bDiscriminator[iJet] = -1.0;
      secVertexMass[iJet] = -1.0;

      if( closestDistance<0.3 && closestIndex<bTags.size() ) {
         bDiscriminator[iJet] = bTags[closestIndex].second;
         const reco::SecondaryVertexTagInfo svTagInfo 
            = (*svTagInfos)[closestIndex];
         if (  svTagInfo.nVertices() > 0  && 
         bDiscriminator[iJet]>-1.0) {
            if(bDiscriminator[iJet]>1.74) numBTags ++;

            ///////////////////////////
            // Calculate SecVtx Mass //
            ///////////////////////////
            ROOT::Math::LorentzVector< ROOT::Math::PxPyPzM4D<double> > sumVec;
            reco::CompositeCandidate vertexCand;
            reco::Vertex::trackRef_iterator 
               kEndTracks = svTagInfo.secondaryVertex(0).tracks_end();
            for (reco::Vertex::trackRef_iterator trackIter = 
                    svTagInfo.secondaryVertex(0).tracks_begin(); 
                 trackIter != kEndTracks; 
                 ++trackIter ) 
            {
               const double kPionMass = 0.13957018;
               ROOT::Math::LorentzVector< ROOT::Math::PxPyPzM4D<double> > vec;
               vec.SetPx( (*trackIter)->px() );
               vec.SetPy( (*trackIter)->py() );
               vec.SetPz( (*trackIter)->pz() );
               vec.SetM (kPionMass);
               sumVec += vec;
            } // for trackIter
            secVertexMass[iJet] = sumVec.M();
         } // endif svTagInfo.nVertices condition
      }// endif closestDistance condition				      


      // CaloJet specific quantities
      const std::type_info & type = typeid(*jet);
      if ( type == typeid(reco::CaloJet) ) {
         const reco::CaloJet calojet = static_cast<const reco::CaloJet &>(*jet);

         MaxEInEmTowers[iJet] = calojet.maxEInEmTowers();
         MaxEInHadTowers[iJet] = calojet.maxEInHadTowers();
         EnergyFractionHadronic[iJet] 
            = calojet.energyFractionHadronic();
         EmEnergyFraction[iJet] 
            = calojet.emEnergyFraction();
         HadEnergyInHB[iJet] = calojet.hadEnergyInHB();
         HadEnergyInHO[iJet] = calojet.hadEnergyInHO();
         HadEnergyInHE[iJet] = calojet.hadEnergyInHE();
         HadEnergyInHF[iJet] = calojet.hadEnergyInHF();
         EmEnergyInEB[iJet] = calojet.emEnergyInEB();
         EmEnergyInEE[iJet] = calojet.emEnergyInEE();
         EmEnergyInHF[iJet] = calojet.emEnergyInHF();
         TowersArea[iJet] = calojet.towersArea();
         N90[iJet] = calojet.n90();
         N60[iJet] = calojet.n60(); 
      }
      if ( type == typeid(reco::GenJet) ) {
         // GenJet specific quantities
         reco::GenJet genjet = static_cast<const reco::GenJet &>(*jet);
         GenEmEnergy[iJet] = genjet.emEnergy();
         GenHadEnergy[iJet] = genjet.hadEnergy();
         GenInvisibleEnergy[iJet] = genjet.invisibleEnergy();
         GenAuxiliaryEnergy[iJet] = genjet.auxiliaryEnergy();	  
      }
      if ( type == typeid(reco::PFJet) ) {
         // PFJet specific quantities
         reco::PFJet pfjet = static_cast<const reco::PFJet &>(*jet);
         PFChargedHadronEnergy[iJet] = pfjet.chargedHadronEnergy();
         PFChargedHadronEnergyFraction[iJet] = 
            pfjet.chargedHadronEnergyFraction ();
         PFNeutralHadronEnergy[iJet] = pfjet.neutralHadronEnergy();
         PFNeutralHadronEnergyFraction[iJet] = 
            pfjet.neutralHadronEnergyFraction ();
         PFChargedEmEnergy[iJet] = pfjet.chargedEmEnergy ();
         PFChargedEmEnergyFraction[iJet] = 
            pfjet.chargedEmEnergyFraction ();
         PFChargedMuEnergy[iJet] = pfjet.chargedMuEnergy ();
         PFChargedMuEnergyFraction[iJet] = 
            pfjet.chargedMuEnergyFraction ();
         PFNeutralEmEnergy[iJet] = pfjet.neutralEmEnergy ();
         PFNeutralEmEnergyFraction[iJet] = 
            pfjet.neutralEmEnergyFraction ();
         PFChargedMultiplicity[iJet] = pfjet.chargedMultiplicity();
         PFNeutralMultiplicity[iJet] = pfjet.neutralMultiplicity();
         PFMuonMultiplicity[iJet] = pfjet.muonMultiplicity();
	 PFPhotonEnergy[iJet] = pfjet.photonEnergy();
	 PFPhotonEnergyFraction[iJet] = pfjet.photonEnergyFraction();
	 PFElectronEnergy[iJet] = pfjet.electronEnergy();
	 PFElectronEnergyFraction[iJet] = pfjet.electronEnergyFraction();
	 PFMuonEnergy[iJet] = pfjet.muonEnergy();
	 PFMuonEnergyFraction[iJet] = pfjet.muonEnergyFraction();
	 PFHFHadronEnergy[iJet] = pfjet.HFHadronEnergy();
	 PFHFHadronEnergyFraction[iJet] = pfjet.HFHadronEnergyFraction();
	 PFHFEMEnergy[iJet] = pfjet.HFEMEnergy();
	 PFHFEMEnergyFraction[iJet] = pfjet.HFEMEnergyFraction();	 
	 PFChargedHadronMultiplicity[iJet] = pfjet.chargedHadronMultiplicity();
	 PFNeutralHadronMultiplicity[iJet] = pfjet.neutralHadronMultiplicity();
	 PFPhotonMultiplicity[iJet] = pfjet.photonMultiplicity();
	 PFElectronMultiplicity[iJet] = pfjet.electronMultiplicity();
	 PFHFHadronMultiplicity[iJet] = pfjet.HFHadronMultiplicity();
	 PFHFEMMultiplicity[iJet] = pfjet.HFEMMultiplicity();
      }// close PF jets loop
   }// close jets iteration loop

   NumJets = iJet;




   // Now compute all the invariant masses
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > Vj;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > V2j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > V3j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > V4j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > V5j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > V6j;

   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > c2j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > c3j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > c4j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > c5j;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > c6j;

   if( NumJets>1 ) { 
     V2j =  Vboson->p4() + (*jets)[0].p4() + (*jets)[1].p4();
     V2jMass =  V2j.M();
     c2j =  (*jets)[0].p4() + (*jets)[1].p4();
     c2jMass =  c2j.M();
   }

   if( NumJets>2 ) {
     V3j =  Vboson->p4() + (*jets)[0].p4() + (*jets)[1].p4() + 
     (*jets)[2].p4();
     V3jMass =  V3j.M();
     c3j =  (*jets)[0].p4() + (*jets)[1].p4() + (*jets)[2].p4();
     c3jMass =  c3j.M();
   }

   if( NumJets>3 ) { 
     V4j =  Vboson->p4() + (*jets)[0].p4() + (*jets)[1].p4() + 
       (*jets)[2].p4() + (*jets)[3].p4();
     V4jMass =  V4j.M();
     c4j =  (*jets)[0].p4() + (*jets)[1].p4() + 
       (*jets)[2].p4() + (*jets)[3].p4();
     c4jMass =  c4j.M();
   }

   if( NumJets>4 ) { 
     V5j =  Vboson->p4() + (*jets)[0].p4() + (*jets)[1].p4() + 
     (*jets)[2].p4() + (*jets)[3].p4() + (*jets)[4].p4();
     V5jMass =  V5j.M();
     c5j =  (*jets)[0].p4() + (*jets)[1].p4() + 
       (*jets)[2].p4() + (*jets)[3].p4() + (*jets)[4].p4();
     c5jMass =  c5j.M();
   }

   if( NumJets>5 ) { 
     V6j =  Vboson->p4() + (*jets)[0].p4() + (*jets)[1].p4() + 
     (*jets)[2].p4() + (*jets)[3].p4() + (*jets)[4].p4() + (*jets)[5].p4();
     V6jMass =  V6j.M();
     c6j =  (*jets)[0].p4() + (*jets)[1].p4() + 
     (*jets)[2].p4() + (*jets)[3].p4() + (*jets)[4].p4() + (*jets)[5].p4();
     c6jMass =  c6j.M();
   }

   //FillBranches();
}



