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
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"


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
  if( jetType=="Cor" && iConfig.existsAs<std::vector<edm::InputTag> >("srcCaloCor") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcCaloCor"); 
  // ********** GenJets ********** //
  if( jetType=="Gen" && iConfig.existsAs<std::vector<edm::InputTag> >("srcGen") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcGen"); 
  // ********** PFJets ********** //
  if( jetType=="PF" && iConfig.existsAs<std::vector<edm::InputTag> >("srcPFJets") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcPFJets"); 
  // ********** Corrected PFJets ********** //
  if( jetType=="PFCor" && iConfig.existsAs<std::vector<edm::InputTag> >("srcPFCor") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcPFCor"); 
  // ********** JetPlusTrack Jets ********** //
  if( jetType=="JPT" && iConfig.existsAs<std::vector<edm::InputTag> >("srcJPTJets") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcJPTJets"); 
  // ********** Corrected JetPlusTrack Jets ********** //
  if( jetType=="JPTCor" && iConfig.existsAs<std::vector<edm::InputTag> >("srcJPTCor") )
    mInputJets 
      = iConfig.getParameter<std::vector<edm::InputTag> >("srcJPTCor"); 

  
  // ********** Vector boson ********** //
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 


  //  ********** Jet Flavor identification (MC) ********** //
  doJetFlavorIdentification = false;
  if(  iConfig.existsAs<std::vector<edm::InputTag> >("srcFlavorByValue") ) {
    sourceByValue = iConfig.getParameter<std::vector<edm::InputTag> > ("srcFlavorByValue");
    doJetFlavorIdentification = true;
  }

  //  ********** Jet Id parameters ********** //
  edm::ParameterSet JetIDParams;
  JetIDParams.addParameter("useRecHits", true);
  JetIDParams.addParameter("hbheRecHitsColl", edm::InputTag("hbhereco"));
  JetIDParams.addParameter("hoRecHitsColl", edm::InputTag("horeco"));
  JetIDParams.addParameter("hfRecHitsColl", edm::InputTag("hfreco"));
  JetIDParams.addParameter("ebRecHitsColl", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
  JetIDParams.addParameter("eeRecHitsColl", edm::InputTag("ecalRecHit", "EcalRecHitsEE"));

  jet_ID_helper_ = reco::helper::JetIDHelper( JetIDParams );

  tree_     = tree;
  jetType_ = jetType;
  NumJetAlgo = mInputJets.size();
  
  if( !(tree==0 || NumJetAlgo==0) ) SetBranches();

}





void ewk::JetTreeFiller::SetBranches()
{
  // Declare jet branches
  std::string abrname = "nnum" + jetType_ + "JetAlgo";
  tree_->Branch( abrname.c_str(), &NumJetAlgo, (abrname +"/I").c_str() );
  abrname = "num" + jetType_ + "Jets";
  tree_->Branch( abrname.c_str(), &(NumJets[0]), (abrname +"[6]/I").c_str() );
  abrname = "num" + jetType_ + "JetBTags";
  tree_->Branch( abrname.c_str(), &(numBTags[0]),(abrname +"[6]/I").c_str() );

  if(doJetFlavorIdentification) 
    SetBranch( &(Flavor[0][0]), "Jet" + jetType_ + "_Flavor");
  SetBranch( &(Et[0][0]), "Jet" + jetType_ + "_Et");
  SetBranch( &(Pt[0][0]), "Jet" + jetType_ + "_Pt");
  SetBranch( &(Eta[0][0]), "Jet" + jetType_ + "_Eta");
  SetBranch( &(Phi[0][0]), "Jet" + jetType_ + "_Phi");
  SetBranch( &(Theta[0][0]), "Jet" + jetType_ + "_Theta");
  SetBranch( &(Px[0][0]), "Jet" + jetType_ + "_Px");
  SetBranch( &(Py[0][0]), "Jet" + jetType_ + "_Py");
  SetBranch( &(Pz[0][0]), "Jet" + jetType_ + "_Pz");
  SetBranch( &(E[0][0]), "Jet" + jetType_ + "_E");
  SetBranch( &(Y[0][0]), "Jet" + jetType_ + "_Y");
  SetBranch( &(VjetMass[0][0]), "VplusLead" + jetType_ + "Jet_Mass");
  SetBranch( &(Dphi[0][0]), "Jet" + jetType_ + "_Dphi");
  SetBranch( &(Deta[0][0]), "VplusLead" + jetType_ + "_Deta");
  SetBranch( &(DR[0][0]), "VplusLead" + jetType_ + "_DR");
  SetBranch( &(Response[0][0]), "Jet" + jetType_ + "_Response");
  SetBranch( &(bDiscriminator[0][0]), "Jet" + jetType_ + "_bDiscriminator");
  SetBranch( &(secVertexMass[0][0]), "Jet" + jetType_ + "_secVertexMass");


  if( jetType_ == "Calo" || jetType_ == "PF" || jetType_ == "JPT") {
    SetBranch( &(passingLoose[0][0]), "Jet" + jetType_ + "_passingLooseId");
    SetBranch( &(passingMedium[0][0]), "Jet" + jetType_ + "_passingMediumId");
    SetBranch( &(passingTight[0][0]), "Jet" + jetType_ + "_passingTightId");
  }

  if( jetType_ == "Calo" || jetType_ == "Cor" || 
      jetType_ == "JPT" || jetType_ == "JPTCor") {

    /** Returns the maximum energy deposited in ECAL towers*/
    SetBranch( &(MaxEInEmTowers[0][0]), "Jet" + jetType_ + "_MaxEInEmTowers");
    /** Returns the maximum energy deposited in HCAL towers*/
    SetBranch( &(MaxEInHadTowers[0][0]), "Jet" + jetType_ + "_MaxEInHadTowers");
    /** Returns the jet hadronic energy fraction*/
    SetBranch( &(EnergyFractionHadronic[0][0]), "Jet" + jetType_ + "_EnergyFractionHadronic");
    /** Returns the jet electromagnetic energy fraction*/
    SetBranch( &(EmEnergyFraction[0][0]), "Jet" + jetType_ + "_EmEnergyFraction");
    /** Returns the jet hadronic energy in HB*/ 
    SetBranch( &(HadEnergyInHB[0][0]), "Jet" + jetType_ + "_HadEnergyInHB");
    /** Returns the jet hadronic energy in HO*/
    SetBranch( &(HadEnergyInHO[0][0]), "Jet" + jetType_ + "_HadEnergyInHO");
    /** Returns the jet hadronic energy in HE*/
    SetBranch( &(HadEnergyInHE[0][0]), "Jet" + jetType_ + "_HadEnergyInHE");
    /** Returns the jet hadronic energy in HF*/
    SetBranch( &(HadEnergyInHF[0][0]), "Jet" + jetType_ + "_HadEnergyInHF");
    /** Returns the jet electromagnetic energy in EB*/
    SetBranch( &(EmEnergyInEB[0][0]), "Jet" + jetType_ + "_EmEnergyInEB");
    /** Returns the jet electromagnetic energy in EE*/
    SetBranch( &(EmEnergyInEE[0][0]), "Jet" + jetType_ + "_EmEnergyInEE");
    /** Returns the jet electromagnetic energy extracted from HF*/
    SetBranch( &(EmEnergyInHF[0][0]), "Jet" + jetType_ + "_EmEnergyInHF");
    /** Returns area of contributing towers */
    SetBranch( &(TowersArea[0][0]), "Jet" + jetType_ + "_TowersArea");
    /** Number of constituents carrying a 90% of the total Jet energy*/
    SetBranch( &(N90[0][0]), "Jet" + jetType_ + "_N90");
    /** Number of constituents carrying a 60% of the total Jet energy*/
    SetBranch( &(N60[0][0]), "Jet" + jetType_ + "_N60");
  }
  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "Gen") {
    /** Returns energy of electromagnetic particles*/
    SetBranch( &(GenEmEnergy[0][0]), "Jet" + jetType_ + "_EmEnergy");
    /** Returns energy of hadronic particles*/
    SetBranch( &(GenHadEnergy[0][0]), "Jet" + jetType_ + "_HadEnergy");
    /** Returns invisible energy*/
    SetBranch( &(GenInvisibleEnergy[0][0]), "Jet" + jetType_ + "_InvisibleEnergy");
    /** Returns other energy (undecayed Sigmas etc.)*/
    SetBranch( &(GenAuxiliaryEnergy[0][0]), "Jet" + jetType_ + "_AuxiliaryEnergy");
  }

  /////////////////////////////////////////////////////////////////////////

  if( jetType_ == "PF" || jetType_ == "PFCor") {
    /// chargedHadronEnergy 
    SetBranch( &(PFChargedHadronEnergy[0][0]), "Jet" + jetType_ + "_ChargedHadronEnergy");
    ///  chargedHadronEnergyFraction
    SetBranch( &(PFChargedHadronEnergyFraction[0][0]), "Jet" + jetType_ + "_ChargedHadronEnergyFraction");
    /// neutralHadronEnergy
    SetBranch( &(PFNeutralHadronEnergy[0][0]), "Jet" + jetType_ + "_NeutralHadronEnergy");
    /// neutralHadronEnergyFraction
    SetBranch( &(PFNeutralHadronEnergyFraction[0][0]), "Jet" + jetType_ + "_NeutralHadronEnergyFraction");
    /// chargedEmEnergy
    SetBranch( &(PFChargedEmEnergy[0][0]), "Jet" + jetType_ + "_PFChargedEmEnergy");
    /// chargedEmEnergyFraction
    SetBranch( &(PFChargedEmEnergyFraction[0][0]), "Jet" + jetType_ + "_ChargedEmEnergyFraction");
    /// chargedMuEnergy
    SetBranch( &(PFChargedMuEnergy[0][0]), "Jet" + jetType_ + "_ChargedMuEnergy");
    /// chargedMuEnergyFraction
    SetBranch( &(PFChargedMuEnergyFraction[0][0]), "Jet" + jetType_ + "_ChargedMuEnergyFraction");
    /// neutralEmEnergy
    SetBranch( &(PFNeutralEmEnergy[0][0]), "Jet" + jetType_ + "_NeutralEmEnergy");
    /// neutralEmEnergyFraction
    SetBranch( &(PFNeutralEmEnergyFraction[0][0]), "Jet" + jetType_ + "_NeutralEmEnergyFraction");
    /// chargedMultiplicity
    SetBranch( &(PFChargedMultiplicity[0][0]), "Jet" + jetType_ + "_ChargedMultiplicity");
    /// neutralMultiplicity
    SetBranch( &(PFNeutralMultiplicity[0][0]), "Jet" + jetType_ + "_NeutralMultiplicity");
    /// muonMultiplicity
    SetBranch( &(PFMuonMultiplicity[0][0]), "Jet" + jetType_ + "_MuonMultiplicity");
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
    numBTags[i] = 0;

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
      passingLoose[i][j]=0;
      passingMedium[i][j]=0;
      passingTight[i][j]=0;
      bDiscriminator[i][j] = -1.0;
      secVertexMass[i][j] = -1.0;

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
  edm::Handle<edm::View<reco::Jet> > jets[NUM_ALGO_MAX];


  // Calo/JPT jet ID
  JetIDSelectionFunctor jet_ID_loose( JetIDSelectionFunctor::PURE09, 
				      JetIDSelectionFunctor::LOOSE );
  JetIDSelectionFunctor jet_ID_tight( JetIDSelectionFunctor::PURE09, 
				      JetIDSelectionFunctor::TIGHT );



  // Get b tag information
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("simpleSecondaryVertexBJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  edm::Handle<reco::SecondaryVertexTagInfoCollection> svTagInfos;
  iEvent.getByLabel("secondaryVertexTagInfos", svTagInfos);
  
  
  
  
  size_t iJet = 0;
  double dist = 100000.0;
  
  // start main loop
  for (size_t iAlgo = 0;  iAlgo <  (unsigned int) NumJetAlgo; iAlgo++) {


    iEvent.getByLabel( mInputJets[iAlgo], jets[iAlgo] ); 
    if(!jets[iAlgo].isValid()) break; 
    if(jets[iAlgo]->size() < 1) break;
    NumJets[iAlgo] = 0;
    numBTags[iAlgo] = 0;


    /****************    MC Jet Flavor    ***************/
    edm::Handle<reco::JetFlavourMatchingCollection> theTagByValue; 
    if(doJetFlavorIdentification) 
      iEvent.getByLabel (sourceByValue[iAlgo], theTagByValue );   
 


    iJet = 0;
    edm::View<reco::Jet>::const_iterator jet, endpjets = jets[iAlgo]->end(); 
    for (jet = jets[iAlgo]->begin();  jet != endpjets;  ++jet, ++iJet) {

      if( !(iJet< (unsigned int) NUM_JET_MAX) ) break;
      Et[iAlgo][iJet] = (*jet).et();
      Pt[iAlgo][iJet] = (*jet).pt();
      Eta[iAlgo][iJet] = (*jet).eta();
      Phi[iAlgo][iJet] = (*jet).phi();
      Theta[iAlgo][iJet] = (*jet).theta();
      Px[iAlgo][iJet] = (*jet).px();
      Py[iAlgo][iJet] = (*jet).py();
      Pz[iAlgo][iJet] = (*jet).pz();
      E[iAlgo][iJet]  = (*jet).energy();
      Y[iAlgo][iJet]  = (*jet).rapidity();
      Dphi[iAlgo][iJet] = dPhi( (*jet).phi(), Vboson->phi() );
      Deta[iAlgo][iJet] = fabs( (*jet).eta() - Vboson->eta() );
      DR[iAlgo][iJet] = radius( (*jet).eta(), (*jet).phi(), 
			   Vboson->eta(), Vboson->phi());
      Response[iAlgo][iJet] = 10.0;
      float vpt = Vboson->pt();
      if( vpt>0.0 ) Response[iAlgo][iJet] = (*jet).pt() / vpt;

      ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > qstar 
	= (*jet).p4() + Vboson->p4();
      VjetMass[iAlgo][iJet] = qstar.M();

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
	Flavor[iAlgo][iJet] = flavor;
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
      bDiscriminator[iAlgo][iJet] = -1.0;
      secVertexMass[iAlgo][iJet] = -1.0;

      if( closestDistance<0.3 && closestIndex<bTags.size() ) {
	bDiscriminator[iAlgo][iJet] = bTags[closestIndex].second;
	const reco::SecondaryVertexTagInfo svTagInfo 
	  = (*svTagInfos)[closestIndex];
	if (  svTagInfo.nVertices() > 0  && 
	      bDiscriminator[iAlgo][iJet]>-1.0) {
	  if(bDiscriminator[iAlgo][iJet]>2.02) numBTags[iAlgo] ++;

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
	  secVertexMass[iAlgo][iJet] = sumVec.M();
	} // endif svTagInfo.nVertices condition
      }// endif closestDistance condition				      


      // CaloJet specific quantities
      const std::type_info & type = typeid(*jet);
      if ( type == typeid(reco::CaloJet) ) {
	const reco::CaloJet calojet = static_cast<const reco::CaloJet &>(*jet);

	MaxEInEmTowers[iAlgo][iJet] = calojet.maxEInEmTowers();
	MaxEInHadTowers[iAlgo][iJet] = calojet.maxEInHadTowers();
	EnergyFractionHadronic[iAlgo][iJet] 
	  = calojet.energyFractionHadronic();
	EmEnergyFraction[iAlgo][iJet] 
	  = calojet.emEnergyFraction();
	HadEnergyInHB[iAlgo][iJet] = calojet.hadEnergyInHB();
	HadEnergyInHO[iAlgo][iJet] = calojet.hadEnergyInHO();
	HadEnergyInHE[iAlgo][iJet] = calojet.hadEnergyInHE();
	HadEnergyInHF[iAlgo][iJet] = calojet.hadEnergyInHF();
	EmEnergyInEB[iAlgo][iJet] = calojet.emEnergyInEB();
	EmEnergyInEE[iAlgo][iJet] = calojet.emEnergyInEE();
	EmEnergyInHF[iAlgo][iJet] = calojet.emEnergyInHF();
	TowersArea[iAlgo][iJet] = calojet.towersArea();
	N90[iAlgo][iJet] = calojet.n90();
	N60[iAlgo][iJet] = calojet.n60(); 

	//calculate the Calo jetID
	passingLoose[iAlgo][iJet]=0;
	passingMedium[iAlgo][iJet]=0;
	passingTight[iAlgo][iJet]=0;
	if( (jetType_ == "Calo") || (jetType_ == "JPT") ) {
	  jet_ID_helper_.calculate( iEvent, calojet );
	  fill_jet_ID_struct();
	  passingLoose[iAlgo][iJet]  = jet_ID_loose( calojet, jet_ID_struct_ ); 
	  passingMedium[iAlgo][iJet] = jet_ID_loose( calojet, jet_ID_struct_ ); 
	  passingTight[iAlgo][iJet]  = jet_ID_tight( calojet, jet_ID_struct_ ); 
	}
      }
      if ( type == typeid(reco::GenJet) ) {
	// GenJet specific quantities
	reco::GenJet genjet = static_cast<const reco::GenJet &>(*jet);
	GenEmEnergy[iAlgo][iJet] = genjet.emEnergy();
	GenHadEnergy[iAlgo][iJet] = genjet.hadEnergy();
	GenInvisibleEnergy[iAlgo][iJet] = genjet.invisibleEnergy();
	GenAuxiliaryEnergy[iAlgo][iJet] = genjet.auxiliaryEnergy();	  
      }
      if ( type == typeid(reco::PFJet) ) {
	// PFJet specific quantities
	reco::PFJet pfjet = static_cast<const reco::PFJet &>(*jet);
	PFChargedHadronEnergy[iAlgo][iJet] = pfjet.chargedHadronEnergy();
	PFChargedHadronEnergyFraction[iAlgo][iJet] = 
	  pfjet.chargedHadronEnergyFraction ();
	PFNeutralHadronEnergy[iAlgo][iJet] = pfjet.neutralHadronEnergy();
	PFNeutralHadronEnergyFraction[iAlgo][iJet] = 
	  pfjet.neutralHadronEnergyFraction ();
	PFChargedEmEnergy[iAlgo][iJet] = pfjet.chargedEmEnergy ();
	PFChargedEmEnergyFraction[iAlgo][iJet] = 
	  pfjet.chargedEmEnergyFraction ();
	PFChargedMuEnergy[iAlgo][iJet] = pfjet.chargedMuEnergy ();
	PFChargedMuEnergyFraction[iAlgo][iJet] = 
	  pfjet.chargedMuEnergyFraction ();
	PFNeutralEmEnergy[iAlgo][iJet] = pfjet.neutralEmEnergy ();
	PFNeutralEmEnergyFraction[iAlgo][iJet] = 
	  pfjet.neutralEmEnergyFraction ();
	PFChargedMultiplicity[iAlgo][iJet] = pfjet.chargedMultiplicity();
	PFNeutralMultiplicity[iAlgo][iJet] = pfjet.neutralMultiplicity();
	PFMuonMultiplicity[iAlgo][iJet] = pfjet.muonMultiplicity();

	//calculate the PF jetID
	bool ThisIsClean=true;
	passingLoose[iAlgo][iJet]=0;
	passingMedium[iAlgo][iJet]=0;
	passingTight[iAlgo][iJet]=0;

	//apply following only if |eta|<2.4: CHF>0, CEMF<0.99, chargedMultiplicity>0   
	if(( pfjet.chargedHadronEnergy()/ pfjet.energy())<= 0.0  
	   && fabs(pfjet.eta())<2.4) ThisIsClean=false; 
	if( (pfjet.chargedEmEnergy()/pfjet.energy())>= 0.99 
	   && fabs(pfjet.eta())<2.4 ) ThisIsClean=false;
	if( pfjet.chargedMultiplicity()<=0 && fabs(pfjet.eta())<2.4 ) ThisIsClean=false;

	// always require #Constituents > 1
	if( pfjet.nConstituents() <=1 ) ThisIsClean=false;

	if(ThisIsClean && (pfjet.neutralHadronEnergy()/pfjet.energy())< 0.99 
	   && (pfjet.neutralEmEnergy()/pfjet.energy())<0.99) passingLoose[iAlgo][iJet]=1;

	if(ThisIsClean && (pfjet.neutralHadronEnergy()/pfjet.energy())< 0.95 
	   && (pfjet.neutralEmEnergy()/pfjet.energy())<0.95) passingMedium[iAlgo][iJet]=1;

	if(ThisIsClean && (pfjet.neutralHadronEnergy()/pfjet.energy())< 0.90 
	   && (pfjet.neutralEmEnergy()/pfjet.energy())<0.90) passingTight[iAlgo][iJet]=1;
	
      }// close PF jets loop

      NumJets[iAlgo] = NumJets[iAlgo] + 1;

    }// close jets iteration loop

  } // Close JetAlgo loop

  //FillBranches();
}



//______________________________________________________________________________
void ewk::JetTreeFiller::fill_jet_ID_struct()
{
  reco::JetID& ss = jet_ID_struct_;
  const reco::helper::JetIDHelper & hh = jet_ID_helper_;
  ss.fHPD = hh.fHPD();
  ss.fRBX = hh.fRBX();
  ss.n90Hits            =  hh.n90Hits();
  ss.fSubDetector1      =  hh.fSubDetector1();
  ss.fSubDetector2      =  hh.fSubDetector2();
  ss.fSubDetector3      =  hh.fSubDetector3();
  ss.fSubDetector4      =  hh.fSubDetector4();
  ss.restrictedEMF      =  hh.restrictedEMF();
  ss.nHCALTowers        =  hh.nHCALTowers();
  ss.nECALTowers        =  hh.nECALTowers();
  ss.approximatefHPD    =  hh.approximatefHPD();
  ss.approximatefRBX    =  hh.approximatefRBX();
  ss.hitsInN90          =  hh.hitsInN90();    

  ss.fEB     = hh.fEB   ();
  ss.fEE     = hh.fEE   ();
  ss.fHB     = hh.fHB   (); 
  ss.fHE     = hh.fHE   (); 
  ss.fHO     = hh.fHO   (); 
  ss.fLong   = hh.fLong ();
  ss.fShort  = hh.fShort();
  ss.fLS     = hh.fLSbad();
  ss.fHFOOT  = hh.fHFOOT();

  ss.numberOfHits2RPC =  ss.numberOfHits3RPC = ss.numberOfHitsRPC = 0;
}

