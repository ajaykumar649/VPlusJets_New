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
#include <TLorentzVector.h>
#include "JetMETCorrections/MCJet/plugins/JetUtilMC.h" // needed for dPhi,dR
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// Monte Carlo stuff
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Header file
#include "ElectroWeakAnalysis/VPlusJets/interface/JetTreeFiller.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/METzCalculator.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/AngularVars.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/ColorCorrel.h"
#include "ElectroWeakAnalysis/VPlusJets/interface/QGLikelihoodCalculator.h"

const float BTAG_DISCRIM_DEFAULT=-999.;

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
  // ********** Corrected PFJets for VBF Tag ********** //
  if( jetType=="PFCorVBFTag" && iConfig.existsAs<edm::InputTag>("srcPFCorVBFTag") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcPFCorVBFTag"); 
  // ********** JetPlusTrack Jets ********** //
  if( jetType=="JPT" && iConfig.existsAs<edm::InputTag>("srcJPTJets") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcJPTJets"); 
  // ********** Corrected JetPlusTrack Jets ********** //
  if( jetType=="JPTCor" && iConfig.existsAs<edm::InputTag>("srcJPTCor") )
    mInputJets = iConfig.getParameter<edm::InputTag>("srcJPTCor"); 

  
  if(  iConfig.existsAs<edm::InputTag>("srcMet") )
    mInputMet = iConfig.getParameter<edm::InputTag>("srcMet");


  // ********** Vector boson ********** //
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 

  //*********************  Run Over AOD or PAT  ***********//
  if( iConfig.existsAs<bool>("runningOverAOD"))
    runoverAOD = iConfig.getParameter<bool>("runningOverAOD");
		
  //*********************  bTagger  ***********//
  if( iConfig.existsAs<std::string>("bTagger"))
    bTagger = iConfig.getParameter<std::string>("bTagger");
		
  //  ********** Jet Flavor identification (MC) ********** //
  doJetFlavorIdentification = false;
  if( (iConfig.existsAs<bool>("runningOverMC") && iConfig.getParameter<bool>("runningOverMC") && 
       iConfig.existsAs<edm::InputTag>("srcFlavorByValue") )) {
    sourceByValue = iConfig.getParameter<edm::InputTag> ("srcFlavorByValue");
    doJetFlavorIdentification = true;
  }

  tree_     = tree;
  jetType_ = jetType;

  Vtype_    = iConfig.getParameter<std::string>("VBosonType"); 
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");


  if( !(tree==0) ) SetBranches();
}





void ewk::JetTreeFiller::SetBranches()
{
  // Declare jet branches
  SetBranchSingle( &NumJets, "num" + jetType_ + "Jets");
  SetBranchSingle( &numBTags, "num" + jetType_ + "JetBTags");

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
  /// Area of the jet
  SetBranch( Area, "Jet" + jetType_ + "_Area");

  SetBranch( VjetMass, "Vplus" + jetType_ + "Jet_Mass");
  SetBranch( Dphi, "Jet" + jetType_ + "_dphiBoson");
  SetBranch( Deta, "Jet" + jetType_ + "_detaBoson");
  SetBranch( DR, "Jet" + jetType_ + "_dRBoson");
  SetBranch( DphiMET, "Jet" + jetType_ + "_dphiMET");
  SetBranch( Response, "Jet" + jetType_ + "_Response");
  SetBranch( bDiscriminator, "Jet" + jetType_ + "_bDiscriminator");
  SetBranch( bDiscriminatorSSVHE, "Jet" + jetType_ + "_bDiscriminatorSSVHE");
  SetBranch( bDiscriminatorTCHE, "Jet" + jetType_ + "_bDiscriminatorTCHE");
  SetBranch( bDiscriminatorCSV, "Jet" + jetType_ + "_bDiscriminatorCSV");
  SetBranch( bDiscriminatorJP, "Jet" + jetType_ + "_bDiscriminatorJP");
  SetBranch( bDiscriminatorSSVHP, "Jet" + jetType_ + "_bDiscriminatorSSVHP");
  SetBranch( bDiscriminatorTCHP, "Jet" + jetType_ + "_bDiscriminatorTCHP");
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

  if( jetType_ == "PF" || jetType_ == "PFCor" || jetType_ == "PFCorVBFTag") {
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
    /// Sum pt of all pf constituents
    SetBranch( PFsumPtCands, "Jet" + jetType_ + "_SumPtCands");
    /// Sum pt^2 of all pf constituents
    SetBranch( PFsumPt2Cands, "Jet" + jetType_ + "_SumPt2Cands");
    /// [Sum pt^2*deltaR(cand, jet)^2] / Sum pt^2 of all pf constituents
    SetBranch( PFrmsCands, "Jet" + jetType_ + "_rmsCands");
    /// pt_D variable for QG likelihood:  pt_D = sqrt(Sum_i{pt^2})/ Sum_i{pt), over all PF cands
    // ------- See for details: CMS AN-2011/215
    // -- https://indico.cern.ch/getFile.py/access?contribId=3&resId=0&materialId=slides&confId=129897
    // -- https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=135378
    // -- https://indico.cern.ch/getFile.py/access?contribId=0&resId=0&materialId=slides&confId=144396
    // ------- JetMETCorrections/GammaJet/src/GammaJetAnalyzer.cc
    // 
    SetBranch( PFptD, "Jet" + jetType_ + "_PtD");
    // ---- Quark Gluon Likelihood
    // ---- first check out: cvs co   UserCode/pandolf/QGLikelihood
    // ---- then instantiate:  QGLikelihoodCalculator *qglikeli = new QGLikelihoodCalculator();
    // ---- finally, compute:   float qgLH = qglikeli->computeQGLikelihoodPU(jet_pt, 
    // -----                         fastjet_rho, chargedMultiplicity, neutralMultiplicity, ptD);
    SetBranch( PFqgLikelihood, "Jet" + jetType_ + "_QGLikelihood");
  }
  
  SetBranchSingle( &V2jMass,  "MassV2j_" + jetType_);
  SetBranchSingle( &V3jMass, "MassV3j_" + jetType_);
  SetBranchSingle( &V4jMass, "MassV4j_" + jetType_);
  SetBranchSingle( &V5jMass, "MassV5j_" + jetType_);
  SetBranchSingle( &V6jMass, "MassV6j_" + jetType_);
  SetBranchSingle( &c2jMass, "Mass2j_" + jetType_);
  SetBranchSingle( &c3jMass, "Mass3j_" + jetType_);
  SetBranchSingle( &c4jMass, "Mass4j_" + jetType_);
  SetBranchSingle( &c5jMass, "Mass5j_" + jetType_);
  SetBranchSingle( &c6jMass, "Mass6j_" + jetType_);


  SetBranchSingle( &V2jCosJacksonAngle, "cosJacksonAngleV2j_" + jetType_);
  SetBranchSingle( &c2jCosJacksonAngle, "cosJacksonAngle2j_" + jetType_);
  SetBranchSingle( &V3jCosJacksonAngle, "cosJacksonAngleV3j_" + jetType_);
  SetBranchSingle( &c3jCosJacksonAngle12, "cosJacksonAngle3j12_" + jetType_);
  SetBranchSingle( &c3jCosJacksonAngle23, "cosJacksonAngle3j23_" + jetType_);
  SetBranchSingle( &c3jCosJacksonAngle31, "cosJacksonAngle3j31_" + jetType_);
  SetBranchSingle( &cosphiDecayPlane, "cosphiDecayPlane_" + jetType_); 
  SetBranchSingle( &cosThetaLnu, "cosThetaLnu_" + jetType_); 
  SetBranchSingle( &cosThetaJJ, "cosThetaJJ_" + jetType_);


  if( jetType_ == "PF" || jetType_ == "PFCor" || jetType_ == "PFCorVBFTag") {
    /// Color Correlation between W jets ( jets pull )
    SetBranchSingle( &leadingDeltaTheta, "WJetsPull" + jetType_);

    /// Helicity angles in the Higgs rest frame
    SetBranchSingle( &j1Hel_HiggsCM, "cosThetaJ1HiggsCM_" + jetType_);
    SetBranchSingle( &j2Hel_HiggsCM, "cosThetaJ2HiggsCM_" + jetType_);
    SetBranchSingle( &l1Hel_HiggsCM, "cosThetaL1HiggsCM_" + jetType_);
    SetBranchSingle( &l2Hel_HiggsCM, "cosThetaL2HiggsCM_" + jetType_);
    SetBranchSingle( &b1Hel_HiggsCM, "cosThetaV1HiggsCM_" + jetType_);
    SetBranchSingle( &b2Hel_HiggsCM, "cosThetaV2HiggsCM_" + jetType_);

  }

}


//////////////////////////////////////////////////////////////////
/////// Helper for above function ////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ewk::JetTreeFiller::SetBranchSingle( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/F").c_str() );
  bnames.push_back( name );
}

void ewk::JetTreeFiller::SetBranchSingle( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/I").c_str() );
  bnames.push_back( name );
}



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
    nConstituents[j]  = -1;
    Px[j] = -999999.9;
    Py[j] = -999999.9;
    Pz[j] = -999999.9;
    Flavor[j] = -1;
    Mass[j] = -1.0;
    Area[j] = -10.;

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
    bDiscriminator[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorSSVHE[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorTCHE[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorCSV[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorJP[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorSSVHP[j] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorTCHP[j] = BTAG_DISCRIM_DEFAULT;
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
    PFsumPtCands[j]=0.;
    PFsumPt2Cands[j]=0.;
    PFrmsCands[j]=0.;
    PFptD[j] = -1.0;
    PFqgLikelihood[j] = -1.0;
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

  V2jCosJacksonAngle = -10.0;
  c2jCosJacksonAngle = -10.0;
  V3jCosJacksonAngle = -10.0;
  c3jCosJacksonAngle12 = -10.0;
  c3jCosJacksonAngle23 = -10.0;
  c3jCosJacksonAngle31 = -10.0;

  cosphiDecayPlane = 10.0; 
  cosThetaLnu = 10.0; 
  cosThetaJJ = 10.0;

  leadingDeltaTheta = -10.0;
  j1Hel_HiggsCM = -10.0;;
  j2Hel_HiggsCM = -10.0;;
  l1Hel_HiggsCM = -10.0;;
  l2Hel_HiggsCM = -10.0;;
  b1Hel_HiggsCM = -10.0;;
  b2Hel_HiggsCM = -10.0;;
   
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
  edm::Handle<edm::View<reco::MET> > pfmet;
  iEvent.getByLabel(mInputMet, pfmet);


  edm::Handle<reco::JetTagCollection> bTagHandle1;
  edm::Handle<reco::JetTagCollection> bTagHandle2;
  edm::Handle<reco::JetTagCollection> bTagHandle3;
  edm::Handle<reco::JetTagCollection> bTagHandle4;
  edm::Handle<reco::JetTagCollection> bTagHandle5;
  edm::Handle<reco::JetTagCollection> bTagHandle6;
 
  if(runoverAOD){
    // Get b tag information
    // ------------- SSV-HE ------------------------
    iEvent.getByLabel("simpleSecondaryVertexHighEffBJetTags", bTagHandle1);

    // ------------- TC-HE ------------------------
    iEvent.getByLabel("trackCountingHighEffBJetTags", bTagHandle2);
  
    // ------------- CSV ------------------------
  
    iEvent.getByLabel("combinedSecondaryVertexBJetTags", bTagHandle3);
  
    // ------------- JP ------------------------
 
    iEvent.getByLabel("jetProbabilityBJetTags", bTagHandle4);
  
    // ------------- SSV-HP ------------------------
    iEvent.getByLabel("simpleSecondaryVertexHighPurBJetTags", bTagHandle5);
  

    // ------------- TC-HP ------------------------
 
    iEvent.getByLabel("trackCountingHighPurBJetTags", bTagHandle6);

  }
  const reco::JetTagCollection  &  bTagsSSVHE = *(bTagHandle1.product()) ;
  const reco::JetTagCollection  &  bTagsTCHE = *(bTagHandle2.product()) ;
  const reco::JetTagCollection  &  bTagsCSV = *(bTagHandle3.product()) ;
  const reco::JetTagCollection  &  bTagsJP = *(bTagHandle4.product());
  const reco::JetTagCollection  &  bTagsSSVHP = *(bTagHandle5.product());
  const reco::JetTagCollection  &  bTagsTCHP = *(bTagHandle6.product()) ;
 
  
  edm::Handle<reco::SecondaryVertexTagInfoCollection> svTagInfos;
  if(runoverAOD){
    iEvent.getByLabel("secondaryVertexTagInfos", svTagInfos);
  }
  // ---- Quark Gluon Likelihood
  QGLikelihoodCalculator *qglikeli = new QGLikelihoodCalculator();  

  size_t iJet = 0;
  double dist = 100000.0;
  NumJets = 0;
  numBTags = 0;


  /////// Pileup density "rho" in the event from fastJet pileup calculation /////
  float fastjet_rho = -999999.9;
  edm::Handle<double> rho;
  const edm::InputTag eventrho("kt6PFJetsPFlow", "rho");
  iEvent.getByLabel(eventrho,rho);
  if( *rho == *rho) fastjet_rho = *rho;

//   // get PFCandidates
//   edm::Handle<reco::PFCandidateCollection>  PFCandidates;
//   if(runoverAOD) iEvent.getByLabel("particleFlow", PFCandidates);


  /****************    MC Jet Flavor    ***************/
  edm::Handle<reco::JetFlavourMatchingCollection> theTagByValue; 
  if(doJetFlavorIdentification) 
    if(runoverAOD){
      iEvent.getByLabel (sourceByValue, theTagByValue ); }

  const reco::Jet *ij1=0;
  const reco::Jet *ij2=0;

  // Loop over reco jets 
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
    Area[iJet] = (*jet).jetArea();
    if(runoverAOD){
      etaetaMoment[iJet]  = (*jet).etaetaMoment();
      phiphiMoment[iJet]  = (*jet).phiphiMoment();      
      etaphiMoment[iJet]  = (*jet).etaphiMoment();
      maxDistance[iJet]  = (*jet).maxDistance();
      nConstituents[iJet]  = (*jet).nConstituents();
    }
 	
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



    // ------- Compute deltaR (jet, vector boson) -----
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
      if(runoverAOD){
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
      else
	{
	  edm::Ptr<reco::Jet> ptrJet = jets->ptrAt( jet - jets->begin() );			 
	  if ( ptrJet.isNonnull() && ptrJet.isAvailable() ) {
	    const pat::Jet* pjet = dynamic_cast<const pat::Jet *>(ptrJet.get()) ;
	    if(pjet !=0) Flavor[iJet] = pjet->partonFlavour();
	  }

			  
	}
    }

    // study b tag info.
    // ------------- SSV-HE ------------------------
    double closestDistance = 100000.0;
    unsigned int closestIndex = 10000;
      
    // compute B-tag discriminator
    bDiscriminator[iJet] = BTAG_DISCRIM_DEFAULT;
    bDiscriminatorSSVHE[iJet] = BTAG_DISCRIM_DEFAULT;
    secVertexMass[iJet] = -1.0;
    if(runoverAOD){
      for (unsigned int i = 0; i != bTagsSSVHE.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsSSVHE[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
		  
      // std::cout << " ++++++++++++++++++++  jbTagsSSVHE.size() " <<  bTagsSSVHE.size() <<std::endl;
      if( closestDistance<0.3 && closestIndex<bTagsSSVHE.size() ) {
	bDiscriminator[iJet] = bTagsSSVHE[closestIndex].second;
	bDiscriminatorSSVHE[iJet] = bTagsSSVHE[closestIndex].second;
	const reco::SecondaryVertexTagInfo svTagInfo = (*svTagInfos)[closestIndex];
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
	  

      // ------------- TC-HE ------------------------
      bDiscriminatorTCHE[iJet] = BTAG_DISCRIM_DEFAULT;
      closestDistance = 100000.0;
      closestIndex = 10000;
      
      for (unsigned int i = 0; i != bTagsTCHE.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsTCHE[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
      if( closestDistance<0.3 && closestIndex<bTagsTCHE.size() )
	bDiscriminatorTCHE[iJet] = bTagsTCHE[closestIndex].second;

      // ------------- CSV ------------------------
      bDiscriminatorCSV[iJet] = BTAG_DISCRIM_DEFAULT;
      closestDistance = 100000.0;
      closestIndex = 10000;
      
      for (unsigned int i = 0; i != bTagsCSV.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsCSV[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
      if( closestDistance<0.3 && closestIndex<bTagsCSV.size() )
	bDiscriminatorCSV[iJet] = bTagsCSV[closestIndex].second;

      // ------------- JP ------------------------
      bDiscriminatorJP[iJet] = BTAG_DISCRIM_DEFAULT;
      closestDistance = 100000.0;
      closestIndex = 10000;
      
      for (unsigned int i = 0; i != bTagsJP.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsJP[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
      if( closestDistance<0.3 && closestIndex<bTagsJP.size() )
	bDiscriminatorJP[iJet] = bTagsJP[closestIndex].second;

      // ------------- SSV-HP ------------------------
      bDiscriminatorSSVHP[iJet] = BTAG_DISCRIM_DEFAULT;
      closestDistance = 100000.0;
      closestIndex = 10000;
      
      for (unsigned int i = 0; i != bTagsSSVHP.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsSSVHP[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
      if( closestDistance<0.3 && closestIndex<bTagsSSVHP.size() )
	bDiscriminatorSSVHP[iJet] = bTagsSSVHP[closestIndex].second;

      // ------------- TC-HP ------------------------
      bDiscriminatorTCHP[iJet] = BTAG_DISCRIM_DEFAULT;
      closestDistance = 100000.0;
      closestIndex = 10000;
      
      for (unsigned int i = 0; i != bTagsTCHP.size(); ++i) {
	edm::RefToBase<reco::Jet> aJet  = bTagsTCHP[i].first;   
	dist = radius(aJet->eta(), aJet->phi(),(*jet).eta(), (*jet).phi());
	if( dist < closestDistance ) { 
	  closestDistance = dist;
	  closestIndex = i;
	}
      }
      if( closestDistance<0.3 && closestIndex<bTagsTCHP.size() )
	bDiscriminatorTCHP[iJet] = bTagsTCHP[closestIndex].second;

    }
    else
      {  
	edm::Ptr<reco::Jet> ptrJet = jets->ptrAt( jet - jets->begin() );
		  
	if ( ptrJet.isNonnull() && ptrJet.isAvailable() ) {
	  const pat::Jet* pjet = dynamic_cast<const pat::Jet *>(ptrJet.get()) ;
	  if(pjet !=0)
	    {
	      //bDiscriminator[iJet] = (*pjet).bDiscriminator(bTagger);
	      bDiscriminatorSSVHE[iJet] = (*pjet).bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	      bDiscriminatorTCHE[iJet] = (*pjet).bDiscriminator("trackCountingHighEffBJetTags");
	      bDiscriminatorCSV[iJet] = (*pjet).bDiscriminator("combinedSecondaryVertexBJetTags");
	      bDiscriminatorJP[iJet] = (*pjet).bDiscriminator("jetProbabilityBJetTags");
	      bDiscriminatorSSVHP[iJet] = (*pjet).bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
	      bDiscriminatorTCHP[iJet] = (*pjet).bDiscriminator("trackCountingHighPurBJetTags");
	      bDiscriminator[iJet] = bDiscriminatorSSVHE[iJet];
	      if(bDiscriminator[iJet]>1.74) numBTags ++;
	    }

	}
      }

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
    if ( type == typeid(reco::PFJet) || type == typeid(pat::Jet)) {
      // PFJet specific quantities
      std::vector<reco::PFCandidatePtr> pfCandidates;
      double pfjetpt(-99999.9), pfjeteta(-99999.9), pfjetphi(-99999.9), pfjetenergy(-99999.9);

      if(type == typeid(reco::PFJet)) {
	reco::PFJet pfjet  = static_cast<const reco::PFJet &>(*jet);
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
	pfCandidates = pfjet.getPFConstituents();
	pfjetpt = pfjet.pt(); 
	pfjeteta = pfjet.eta(); 
	pfjetphi = pfjet.phi();
	pfjetenergy = pfjet.energy();
      }
      if(type == typeid(pat::Jet)) {
	pat::Jet pfjet  = static_cast<const pat::Jet &>(*jet);
	if(pfjet.isPFJet()) {
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
	  // PFElectronEnergyFraction[iJet] = pfjet.electronEnergyFraction();
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
	  pfCandidates = pfjet.getPFConstituents();
	  pfjetpt = pfjet.pt(); 
	  pfjeteta = pfjet.eta(); 
	  pfjetphi = pfjet.phi();
	  pfjetenergy = pfjet.energy();
	}
      }
      // ------- Compute pt_D and Quark Gluon Likelihood

		  
      float sumPt_cands=0.;
      float sumPt2_cands=0.;
      float rms_cands=0.;
			
      typedef std::vector<reco::PFCandidatePtr>::const_iterator IC;

      for (IC jt = pfCandidates.begin(); // If pfCandidates has no constituents then the loop simply won't execute
	   jt != pfCandidates.end(); jt++) { // and so no segmentation fault should occur
	const reco::PFCandidatePtr pfCandPtr = *jt;
	if ( !(pfCandPtr.isNonnull() && pfCandPtr.isAvailable()) ) continue;
			  
	// reco::PFCandidate::ParticleType id = (*jt)->particleId();
	// Convert particle momentum to normal TLorentzVector, wrong type :(
	math::XYZTLorentzVectorD const& p4t = pfCandPtr->p4();

	TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
	TLorentzVector jetp4;
	jetp4.SetPtEtaPhiE(pfjetpt, pfjeteta, pfjetphi, pfjetenergy);
	if(p4.Pt()!=0){
	  sumPt_cands += p4.Pt();
	  sumPt2_cands += (p4.Pt()*p4.Pt());
	  float deltaR = jetp4.DeltaR(p4);
	  rms_cands += (p4.Pt()*p4.Pt()*deltaR*deltaR);
	}
      }			  
      PFsumPtCands[iJet]  = sumPt_cands;
      PFsumPt2Cands[iJet] = sumPt2_cands;
      if(sumPt_cands != 0)  PFptD[iJet] = sqrt( sumPt2_cands )/sumPt_cands;
      if(rms_cands  != 0)   PFrmsCands[iJet] = rms_cands/sumPt2_cands;
      PFqgLikelihood[iJet]= qglikeli->computeQGLikelihoodPU( Pt[iJet], fastjet_rho, 
							     PFChargedMultiplicity[iJet], 
							     PFNeutralMultiplicity[iJet], 
							     PFptD[iJet]);	 
      if ( ij1==0 ) ij1=&(*jet); 
      if ( ij1>0 && ij2==0 ) ij2=&(*jet);
		  
    }// close PF jets loop
  }// close jets iteration loop

  NumJets = (int) iJet;


  // get the two daughters of vector boson
  const reco::Candidate* m0 = Vboson->daughter(0);
  const reco::Candidate* m1 = Vboson->daughter(1);

  TLorentzVector p4lepton1;
  TLorentzVector p4lepton2;
  TLorentzVector p4MET;
  TLorentzVector p4lepton;
  METzCalculator* metz = new METzCalculator();
  if (LeptonType_=="electron") metz->SetLeptonType("electron");
  double nupz;


  // Compute pz if one of the lepton is neutrino
  if( m0->isElectron() || m0->isMuon() ) 
    p4lepton1.SetPxPyPzE(m0->px(), m0->py(), m0->pz(), m0->energy());
  else {
    p4MET.SetPxPyPzE((*pfmet)[0].px(), (*pfmet)[0].py(), (*pfmet)[0].pz(), (*pfmet)[0].energy());
    p4lepton.SetPxPyPzE(m1->px(), m1->py(), m1->pz(), m1->energy());
    metz->SetMET(p4MET);
    metz->SetLepton(p4lepton);
    nupz = metz->Calculate();
    p4lepton1.SetPxPyPzE( m0->px(), m0->py(), nupz, sqrt(m0->px()*m0->px()+m0->py()*m0->py()+nupz*nupz) );
  }

  if( m1->isElectron() || m1->isMuon() ) 
    p4lepton2.SetPxPyPzE(m1->px(), m1->py(), m1->pz(), m1->energy());
  else {
    p4MET.SetPxPyPzE((*pfmet)[0].px(), (*pfmet)[0].py(), (*pfmet)[0].pz(), (*pfmet)[0].energy());
    p4lepton.SetPxPyPzE(m0->px(), m0->py(), m0->pz(), m0->energy());
    metz->SetMET(p4MET);
    metz->SetLepton(p4lepton);
    nupz = metz->Calculate();
    p4lepton2.SetPxPyPzE( m1->px(), m1->py(), nupz, sqrt(m1->px()*m1->px()+m1->py()*m1->py()+nupz*nupz) );
  }

  delete metz;
  delete qglikeli;


  // Now compute all the invariant masses
  TLorentzVector Vj;
  TLorentzVector V2j;
  TLorentzVector V3j;
  TLorentzVector V4j;
  TLorentzVector V5j;
  TLorentzVector V6j;

  TLorentzVector c2j;
  TLorentzVector c3j;
  TLorentzVector c4j;
  TLorentzVector c5j;
  TLorentzVector c6j;


  // 4-vectors for the first four jets
  TLorentzVector p4j1;
  TLorentzVector p4j2;
  TLorentzVector p4j3;
  TLorentzVector p4j4;
  TLorentzVector p4j5;
  TLorentzVector p4j6;
  TLorentzVector p4V = p4lepton1 + p4lepton2;


  if( NumJets>1 ) { 
    p4j1.SetPxPyPzE( Px[0], Py[0], Pz[0], E[0] );
    p4j2.SetPxPyPzE( Px[1], Py[1], Pz[1], E[1] );
    c2j =  p4j1 + p4j2;
    c2jMass =  c2j.M();
    V2j =  p4V + c2j;
    V2jMass =  V2j.M();
    V2jCosJacksonAngle = JacksonAngle( p4V, V2j);
    c2jCosJacksonAngle = JacksonAngle( p4j1, p4j2);
  }

  if( NumJets>2 ) {
    p4j3.SetPxPyPzE( Px[2], Py[2], Pz[2], E[2] );
    c3j =  p4j1 + p4j2 + p4j3;
    c3jMass =  c3j.M();
    V3j =  p4V + c3j;
    V3jMass =  V3j.M();
    V3jCosJacksonAngle = JacksonAngle( p4V, V3j);
    c3jCosJacksonAngle12 = JacksonAngle( p4j1,  p4j2 );
    c3jCosJacksonAngle23 = JacksonAngle( p4j2,  p4j3 );
    c3jCosJacksonAngle31 = JacksonAngle( p4j3,  p4j1 );
  }

  if( NumJets>3 ) { 
    p4j4.SetPxPyPzE( Px[3], Py[3], Pz[3], E[3] );
    c4j =  p4j1 + p4j2 + p4j3 + p4j4;
    c4jMass =  c4j.M();
    V4j =  p4V + c4j;
    V4jMass =  V4j.M();

  }

  if( NumJets>4 ) { 
    p4j5.SetPxPyPzE( Px[4], Py[4], Pz[4], E[4] );
    c5j =  p4j1 + p4j2 + p4j3 + p4j4 + p4j5;
    c5jMass =  c5j.M();
    V5j =  p4V + c5j;
    V5jMass =  V5j.M();

  }

  if( NumJets>5 ) { 
    p4j6.SetPxPyPzE( Px[5], Py[5], Pz[5], E[5] );

    c6j =  p4j1 + p4j2 + p4j3 + p4j4 + p4j5 + p4j6;
    c6jMass =  c6j.M();
    V6j =  p4V + c6j;
    V6jMass =  V6j.M();

  }



  // Angle between the decay planes of two W
  cosphiDecayPlane = 10.0; 
  cosThetaLnu = 10.0; 
  cosThetaJJ = 10.0;

  if( NumJets>1 ) dg_kin_Wuv_Wjj( p4lepton1, p4lepton2, 
				  p4j1, p4j2, cosphiDecayPlane, 
				  cosThetaLnu, cosThetaJJ);


  // Color correlation between two W jets ( jets pull ) 
  if ( !(0==ij1 || 0==ij2) ) {
    if(runoverAOD) {
      const reco::PFJet& pfj1 = static_cast<const reco::PFJet &> (*ij1) ;
      const reco::PFJet& pfj2 = static_cast<const reco::PFJet &> (*ij2) ;
      leadingDeltaTheta = TMath::Abs( getDeltaTheta( &pfj1 , &pfj2) );
    }
    else {
      const pat::Jet& pfj1 = static_cast<const pat::Jet &> (*ij1) ;
      const pat::Jet& pfj2 = static_cast<const pat::Jet &> (*ij2) ;
      leadingDeltaTheta = TMath::Abs( getDeltaTheta( &pfj1 , &pfj2) );
    }
  }  



 
  // Cos(theta*) or Helicity Angles in Higgs rest frame
  if( NumJets>1 ){
    TLorentzVector p4boson1;
    p4boson1.SetPxPyPzE(Vboson->px(), Vboson->py(), Vboson->pz(), Vboson->energy());
    TLorentzVector p4boson2 = p4j1 + p4j2;
    TLorentzVector p4higgs  = p4boson1 + p4boson2;
    TVector3 higgsBoost = p4higgs.BoostVector();

    j1Hel_HiggsCM = getHelicity( p4j1 , higgsBoost );
    j2Hel_HiggsCM = getHelicity( p4j2 , higgsBoost );
    l1Hel_HiggsCM = getHelicity( p4lepton1 , higgsBoost );
    l2Hel_HiggsCM = getHelicity( p4lepton2 , higgsBoost );
    b1Hel_HiggsCM = getHelicity( p4boson1 , higgsBoost );
    b2Hel_HiggsCM = getHelicity( p4boson2 , higgsBoost );
  }

  //FillBranches();
}


