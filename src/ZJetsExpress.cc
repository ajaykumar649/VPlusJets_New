// -*- C++ -*-
//
// Package:    ZJetsExpress
// Class:      ZJetsExpress
// 
/**\class ZJetsExpress ZJetsExpress.cc ElectroWeakAnalysis/VPlusJets/src/ZJetsExpress.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  A. Marini, K. Kousouris,  K. Theofilatos
//         Created:  Mon Oct 31 07:52:10 CDT 2011
// $Id$
//
//

// system include files
#include <memory>

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>


#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TH1I.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h" 
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h" 
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h" 
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

//
// class declaration
//
using namespace edm;
using namespace std;
using namespace reco;

class ZJetsExpress : public edm::EDAnalyzer {
   public:
      explicit ZJetsExpress(const edm::ParameterSet&);
      ~ZJetsExpress();

   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();
      // ---- method that builds the tree -------------------------------
      void buildTree();
      // ---- method that re-initializes the tree branches --------------
      void clearTree();
      // ---- structures for temporary object storage -------------------
      struct LEPTON {
        // ---- momentum 4-vector ---------------------------------------
        TLorentzVector p4; 
        // ---- charge id (+/-1: electrons, +/-2: muons) ---------------- 
        int chid; 
        // ---- tight id ------------------------------------------------
        int id;
      };
      struct JET {
        // ---- momentum 4-vector ---------------------------------------
        TLorentzVector p4; 
        // ---- tight id ------------------------------------------------
        int   id; 
        // ---- jet area (needed for pu substraction) -------------------
        float area;
        // ---- track pt fraction associated with the PV ----------------
        float beta;
        // ---- track pt fraction associated with secondary vertices ----
        float betaStar;
        // ---- jet energy correction factor ----------------------------
        float jec; 
        // ---- jet energy uncertainty ----------------------------------
        float unc;
        // ---- charged hadron energy fraction --------------------------
        float chf;
        // ---- neutral hadron energy fraction --------------------------
        float nhf;
        // ---- photon energy fraction ---------------------------------- 
        float phf;
        // ---- muon energy fraction ------------------------------------ 
        float muf;
        // ---- electron energy fraction --------------------------------
        float elf;
      };
      // ---- sorting rules ---------------------------------------------
      static bool leptonSortingRule(LEPTON x, LEPTON y) {return x.p4.Pt() > y.p4.Pt();}
      static bool jetSortingRule(JET x, JET y) {return x.p4.Pt() > y.p4.Pt();}
      // ---------- member data -----------------------------------------
      edm::Service<TFileService> fTFileService;	
      TTree *myTree_;
      // ---- histogram to record the number of events ------------------
      TH1I  *hist_;
      // ---- simulated in-time pile-up ---------------------------------
      TH1D  *mcPU_;
      // ---- flag to set the JEC uncertainty object --------------------
      bool mIsJECuncSet;
      // ---- jet energy corrector object -------------------------------
      const JetCorrector *mJEC;
      // ---- jet energy uncertainty object -----------------------------
      JetCorrectionUncertainty *mJECunc;
      // ---- configurable parameters -----------------------------------
      double mMinJetPt,mMaxJetEta,mMinLepPt,mMaxLepEta;
      string mJECservice,mPayloadName;
      edm::InputTag mJetsName,mSrcRho;
      // ---- tree variables --------------------------------------------
      // ---- event number ----------------------------------------------
      int eventNum_;
      // ---- run number ------------------------------------------------  
      int runNum_; 
      // ---- lumi section ----------------------------------------------
      int lumi_;
      // ---- flag to identify real data --------------------------------
      int isRealData_;
      // ---- dilepton mass ---------------------------------------------
      float llM_;
      // ---- dilepton rapidity -----------------------------------------  
      float llY_;
      // ---- dilepton pt -----------------------------------------------
      float llPt_;
      // ---- dilepton phi ----------------------------------------------
      float llPhi_;
      // ---- dphi between the two leptons ------------------------------
      float llDPhi_; 
      // ---- dphi between jets and dilepton ----------------------------
      vector<float> *jetllDPhi_;
      // ---- lepton kinematics -----------------------------------------
      vector<float> *lepPt_,*lepEta_,*lepPhi_,*lepE_;
      // ---- lepton charge and id -------------------------------------- 
      vector<int>   *lepChId_,*lepId_;
      // ---- number of leptons -----------------------------------------
      int nLeptons_;
      // ---- jet kinematics --------------------------------------------
      vector<float> *jetPt_,*jetEta_,*jetY_,*jetPhi_,*jetE_;
      // ---- jet composition fractions ---------------------------------
      vector<float> *jetCHF_,*jetPHF_,*jetNHF_,*jetMUF_,*jetELF_;
      // ---- other jet properties --------------------------------------
      vector<float> *jetBeta_,*jetBetaStar_,*jetArea_,*jetJEC_,*jetUNC_;
      // ---- tight jet id ----------------------------------------------
      vector<int>   *jetId_; 
      // ---- number of jets --------------------------------------------
      int nJets_;
      // ---- pf met ----------------------------------------------------
      float pfmet_;
      // ---- pf sumEt --------------------------------------------------
      float pfSumEt_;
      // ---- pf met phi ------------------------------------------------
      float pfmetPhi_;
      // ---- pt of the hadronic recoil ---------------------------------
      float pfhadPt_; 
      // ---- transverse mass of the Z and the hadronic recoil ----------
      float mT_; 
      // ---- transverse mass of the Z and the hadronic recoil assuming -
      float mTp_;
      // ---- invariant mass of the Z and the leading jet --------------- 
      float mZj1_;
      // ---- invariant mass of the two leading jets --------------------
      float mj1j2_;
      // ---- invariant mas of all leptons ------------------------------
      float mLep_;
      // ---- pf pt density ---------------------------------------------
      float rho_;
      // ---- reconstructed vertices' prperties -------------------------
      vector<float> *vtxZ_,*vtxNdof_;
      // ---- number of good reconstructed vertices ---------------------
      int   nVtx_;
      // ---- number of simulated in-time pu interactions ---------------
      int puNumInteractions_;
      // ---- properties of simulated in-time pu interactions -----------
      vector<float> *puZpositions_,*puSumpT_lowpT_,*puSumpT_highpT_,*puNtrks_lowpT_,*puNtrks_highpT_;
};
//
// class omplemetation
//
// ---- constructor -----------------------------------------------------
ZJetsExpress::ZJetsExpress(const ParameterSet& iConfig)
{
  mMinJetPt    = iConfig.getParameter<double>        ("minJetPt");
  mMaxJetEta   = iConfig.getParameter<double>        ("maxJetEta");
  mMinLepPt    = iConfig.getParameter<double>        ("minLepPt");
  mMaxLepEta   = iConfig.getParameter<double>        ("maxLepEta");
  mJetsName    = iConfig.getParameter<edm::InputTag> ("jets");
  mSrcRho      = iConfig.getParameter<edm::InputTag> ("srcRho");
  mJECservice  = iConfig.getParameter<std::string>   ("jecService");
  mPayloadName = iConfig.getParameter<std::string>   ("payload");
}
// ---- destructor ------------------------------------------------------
ZJetsExpress::~ZJetsExpress()
{
}
// ---- method called once each job just before starting event loop -----
void ZJetsExpress::beginJob()
{
  // ---- create the objects --------------------------------------------
  hist_   = fTFileService->make<TH1I>("hist", "hist",6,-1,5);
  mcPU_   = fTFileService->make<TH1D>("mcPU", "mcPU",40,0,40);
  myTree_ = fTFileService->make<TTree>("events", "events");
  // ---- build the tree ------------------------------------------------
  buildTree();
  // ---- set the jec uncertainty flag ----------------------------------
  mIsJECuncSet = false;
}
// ---- event loop ------------------------------------------------------
void ZJetsExpress::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  // ---- event counter -------------------------------------------------
  hist_->Fill(-1);  
  // ---- initialize the tree branches ----------------------------------
  clearTree();
  isRealData_ = iEvent.isRealData() ? 1:0;
  // ----  MC truth pileup block ----------------------------------------
  if (!isRealData_) {
    Handle<vector<PileupSummaryInfo> > pileupInfo;
    iEvent.getByLabel("addPileupInfo", pileupInfo);
    vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = pileupInfo->begin(); PVI != pileupInfo->end(); ++PVI) {
      // ---- only look at in-time pileup -------------------------------
      if (PVI->getBunchCrossing() != 0) continue; 
      puNumInteractions_ = PVI->getPU_NumInteractions();
      mcPU_->Fill(puNumInteractions_);
      for(int i=0; i<puNumInteractions_; i++) {
        puZpositions_   ->push_back(PVI->getPU_zpositions()[i]);
	puSumpT_lowpT_  ->push_back(PVI->getPU_sumpT_lowpT()[i]);
	puSumpT_highpT_ ->push_back(PVI->getPU_sumpT_highpT()[i]);
	puNtrks_lowpT_  ->push_back(PVI->getPU_ntrks_lowpT()[i]);
	puNtrks_highpT_ ->push_back(PVI->getPU_ntrks_highpT()[i]);
      }// NumInteractions loop
    }// PVI loop
  }
  //---- Rho ------------------------------------------------------------
  Handle<double> rho;
  iEvent.getByLabel(mSrcRho,rho);
  //---- reco vertices block --------------------------------------------
  edm::Handle<VertexCollection> vertices_;
  iEvent.getByLabel("offlinePrimaryVertices", vertices_);
  const reco::Vertex *primVtx = &(*(vertices_.product()))[0];
  for(VertexCollection::const_iterator i_vtx = vertices_->begin(); i_vtx != vertices_->end(); ++i_vtx) {  
    if (!i_vtx->isFake() && (fabs(i_vtx->z()) < 24) && (i_vtx->ndof() >= 4)) {
      vtxZ_   ->push_back(i_vtx->z());
      vtxNdof_->push_back(i_vtx->ndof());
    }
  }
  //---- leptons block --------------------------------------------------
  Handle<View<GsfElectron> > electrons_;
  iEvent.getByLabel("gsfElectrons", electrons_);
  Handle<View<Muon> > muons_;
  iEvent.getByLabel("muons",muons_);
  vector<LEPTON> myLeptons;
  // ---- loop over muons -----------------------------------------------
  for(View<Muon>::const_iterator i_mu = muons_->begin(); i_mu != muons_->end(); ++i_mu) {
    if (!(i_mu->isGlobalMuon())) continue;
    if (!(i_mu->isTrackerMuon())) continue;
    if (!muon::isGoodMuon(*i_mu,muon::GlobalMuonPromptTight)) continue;
    if ((i_mu->pt() < mMinLepPt) || (fabs(i_mu->eta()) > mMaxLepEta)) continue;
    if (fabs(i_mu->globalTrack()->normalizedChi2()) > 10) continue;
    if (fabs(i_mu->innerTrack()->dz(primVtx->position())) > 1.0) continue;
    if (fabs(i_mu->innerTrack()->dxy(primVtx->position())) > 0.04) continue;
    float muonIso = (i_mu->isolationR03().sumPt + i_mu->isolationR03().emEt + i_mu->isolationR03().hadEt)/i_mu->pt();
    if (muonIso > 0.15) continue;
    float nInnerTrkHits = i_mu->innerTrack()->hitPattern().numberOfValidHits();
    if (nInnerTrkHits < 11) continue;
    float numberOfValidPixelHits = i_mu->innerTrack()->hitPattern().numberOfValidPixelHits();
    if (numberOfValidPixelHits < 1) continue;
    float numberOfMatches = i_mu->numberOfMatches();
    if (numberOfMatches < 2) continue;
    float dptOpt = i_mu->innerTrack()->ptError()/i_mu->pt();
    if (dptOpt > 0.1) continue;
      
    LEPTON aLepton;
    TLorentzVector lepP4(i_mu->p4().Px(),i_mu->p4().Py(),i_mu->p4().Pz(),i_mu->p4().E());
    aLepton.p4   = lepP4;
    aLepton.chid = 2*i_mu->charge();
    aLepton.id   = 1;
    myLeptons.push_back(aLepton);
  }// muon loop
  // ---- loop over electrons -------------------------------------------
  for(View<GsfElectron>::const_iterator i_el = electrons_->begin();i_el != electrons_->end(); ++i_el) {
    float elPt                           = i_el->p4().Pt();
    float elEta                          = i_el->p4().Eta();
    float trackIsoRel                    = i_el->dr03TkSumPt()/i_el->p4().Pt();
    float ecalIsoRel                     = i_el->dr03EcalRecHitSumEt()/i_el->p4().Pt();
    float hcalIsoRel                     = i_el->dr03HcalTowerSumEt()/i_el->p4().Pt();
    float sigmaIetaIeta                  = i_el->sigmaIetaIeta();
    float hadronicOverEm                 = i_el->hadronicOverEm();
    float deltaPhiSuperClusterTrackAtVtx = i_el->deltaPhiSuperClusterTrackAtVtx();
    float deltaEtaSuperClusterTrackAtVtx = i_el->deltaEtaSuperClusterTrackAtVtx();
    bool  isTight(false);
    if ((elPt < mMinLepPt) || (fabs(elEta) > mMaxLepEta)) continue;
    if (i_el->isEB()) {
      float combinedIso03 = (i_el->dr03TkSumPt()+max(0.,i_el->dr03EcalRecHitSumEt()-1.)+i_el->dr03HcalTowerSumEt())/i_el->p4().Pt();
      if (combinedIso03 > 0.15) continue;          
      if (trackIsoRel > 0.15) continue;
      if (ecalIsoRel > 2.00) continue; 
      if (hcalIsoRel > 0.12) continue;
      if (sigmaIetaIeta > 0.01) continue;
      if (deltaPhiSuperClusterTrackAtVtx > 0.8) continue;
      if (deltaEtaSuperClusterTrackAtVtx > 0.007) continue;
      if (hadronicOverEm > 0.15) continue;
      if (sigmaIetaIeta < 0.01) {
	if (deltaPhiSuperClusterTrackAtVtx < 0.06) 
	  if (deltaEtaSuperClusterTrackAtVtx < 0.004) 
	    if (hadronicOverEm < 0.04) 
              isTight = true;
      }
    }// if EB
    if (i_el->isEE()) {
      float combinedIso03 = (i_el->dr03TkSumPt()+i_el->dr03EcalRecHitSumEt()+i_el->dr03HcalTowerSumEt())/i_el->p4().Pt(); 
      if (combinedIso03 > 0.1) continue;
      if (trackIsoRel > 0.08) continue;
      if (ecalIsoRel > 0.06) continue;
      if (hcalIsoRel > 0.05) continue;
      if (sigmaIetaIeta > 0.03) continue;
      if (deltaPhiSuperClusterTrackAtVtx > 0.7) continue;
      if (deltaEtaSuperClusterTrackAtVtx > 0.01) continue;
      if (hadronicOverEm > 0.15) continue;
      if (sigmaIetaIeta<0.03) {
	if (deltaPhiSuperClusterTrackAtVtx < 0.03)
	  if (deltaEtaSuperClusterTrackAtVtx < 0.007)
	    if (hadronicOverEm < 0.15) 
              isTight=true;
      } 
    }// if EE
    LEPTON aLepton;
    TLorentzVector lepP4(i_el->p4().Px(),i_el->p4().Py(),i_el->p4().Pz(),i_el->p4().E());
    aLepton.p4   = lepP4;
    aLepton.chid = i_el->charge();
    aLepton.id   = 0;
    if (isTight) {
      aLepton.id = 1;
    }
    myLeptons.push_back(aLepton);
  } // electrons loop
  hist_->Fill(int(myLeptons.size()));
  // ---- sort the leptons according to their pt ------------------------
  sort(myLeptons.begin(),myLeptons.end(),leptonSortingRule); 
  //---- jets block -----------------------------------------------------
  Handle<PFJetCollection> jets_;
  iEvent.getByLabel(mJetsName,jets_);
  vector<JET> myJets;
  //---- get the jet energy corrector -----------------------------------
  mJEC = JetCorrector::getJetCorrector(mJECservice,iSetup);
  //---- the JEC uncertainty only needs to be set-up once ---------------
  if (!mIsJECuncSet) {
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get<JetCorrectionsRecord>().get(mPayloadName,JetCorParColl); 
    JetCorrectorParameters const& JetCorPar = (*JetCorParColl)["Uncertainty"];
    mJECunc = new JetCorrectionUncertainty(JetCorPar);
    mIsJECuncSet = true;
  }
  // ---- jets loop -----------------------------------------------------
  for(PFJetCollection::const_iterator i_jet = jets_->begin(); i_jet != jets_->end(); ++i_jet) {
    TLorentzVector jetP4(i_jet->px(),i_jet->py(),i_jet->pz(),i_jet->energy());
    bool jetIsDuplicate(false);
    //----- remove the leptons ------------------------------------------
    for(unsigned int i_lep = 0; i_lep < myLeptons.size(); i_lep++) {
      float DR = myLeptons[i_lep].p4.DeltaR(jetP4);
      if (DR < 0.4) {
        jetIsDuplicate = true; 
      }
    }// lepton loop 
    if (jetIsDuplicate) continue; 
    // ---- get the jec and the uncertainty -----------------------------    
    int index = i_jet - jets_->begin();
    edm::RefToBase<reco::Jet> jetRef(edm::Ref<PFJetCollection>(jets_,index));
    double jec = mJEC->correction(*i_jet,jetRef,iEvent,iSetup);
    mJECunc->setJetEta(i_jet->eta());
    // ---- the unc is a function of the corrected pt -------------------
    mJECunc->setJetPt(jec * i_jet->pt());
    double unc = mJECunc->getUncertainty(true);
    // ---- only keep jets within the kinematic acceptance --------------
    if ((jec * jetP4.Pt() < mMinJetPt) || (fabs(jetP4.Eta()) > mMaxJetEta)) continue;
    // ---- jet vertex association --------------------------------------
    // ---- get the vector of tracks ------------------------------------
    reco::TrackRefVector vTrks(i_jet->getTrackRefs());
    float sumTrkPt(0.0),sumTrkPtBeta(0.0),sumTrkPtBetaStar(0.0),beta(0.0),betaStar(0.0);
    // ---- loop over the tracks of the jet -----------------------------
    for(reco::TrackRefVector::const_iterator i_trk = vTrks.begin(); i_trk != vTrks.end(); i_trk++) {
      sumTrkPt += (*i_trk)->pt();
      // ---- loop over all vertices ------------------------------------
      for(unsigned i_vtx = 0;i_vtx < vertices_->size();i_vtx++) {
        // ---- loop over the tracks associated with the vertex ---------
        if ((*vertices_)[i_vtx].isFake()) continue; 
        for(reco::Vertex::trackRef_iterator i_vtxTrk = (*vertices_)[i_vtx].tracks_begin(); i_vtxTrk != (*vertices_)[i_vtx].tracks_end(); ++i_vtxTrk) {
          // ---- match the jet track to the track from the vertex ------
          reco::TrackRef trkRef(i_vtxTrk->castTo<reco::TrackRef>());
          // ---- check if the tracks match -----------------------------
          if (trkRef == (*i_trk)) {
            if (i_vtx == 0) {
              sumTrkPtBeta += (*i_trk)->pt();
            }
            else {
              sumTrkPtBetaStar += (*i_trk)->pt();
            }   
            continue;
          }
        }
      }// vertices loop 
    }// jet tracks loop
    if (sumTrkPt > 0) {
      beta     = sumTrkPtBeta/sumTrkPt;
      betaStar = sumTrkPtBetaStar/sumTrkPt;
    }
    JET aJet; 
    aJet.p4       = jec * jetP4;
    aJet.jec      = jec;
    aJet.unc      = unc;
    aJet.area     = i_jet->jetArea();
    aJet.chf      = i_jet->chargedHadronEnergyFraction();
    aJet.nhf      = (i_jet->neutralHadronEnergy() + i_jet->HFHadronEnergy())/i_jet->energy();
    aJet.phf      = i_jet->photonEnergyFraction();
    aJet.elf      = i_jet->electronEnergyFraction();
    aJet.muf      = i_jet->muonEnergyFraction();
    int chm       = i_jet->chargedHadronMultiplicity();
    int npr       = i_jet->chargedMultiplicity() + i_jet->neutralMultiplicity();
    bool id       = (npr>1 && aJet.phf<0.99 && aJet.nhf<0.99 && ((fabs(i_jet->eta())<=2.4 && aJet.nhf<0.9 && aJet.phf<0.9 && aJet.elf<0.99 && aJet.chf>0 && chm>0) || fabs(i_jet->eta())>2.4));
    aJet.id       = 0;
    if (id) {
      aJet.id     = 1;
    }
    aJet.beta     = beta;
    aJet.betaStar = betaStar;
    myJets.push_back(aJet);  
  }// jet loop
  // ---- sort jets according to their corrected pt ---------------------
  sort(myJets.begin(),myJets.end(),jetSortingRule);    
  // ---- MET block -----------------------------------------------------
  Handle<View<PFMET> > pfmetCol_;
  iEvent.getByLabel("pfMet", pfmetCol_);
  float pfmetPx = (pfmetCol_->front()).px();
  float pfmetPy = (pfmetCol_->front()).py();
  // ---- counters ------------------------------------------------------
  nVtx_       = int(vtxZ_->size());
  nLeptons_   = int(myLeptons.size()); 
  nJets_      = int(myJets.size());
  // ---- keep only selected events -------------------------------------
  if ((nVtx_ > 0) && (nLeptons_ > 1) && (nJets_ > 0)) {
    TLorentzVector llP4 = myLeptons[0].p4 + myLeptons[1].p4;
    TLorentzVector pfmetP4(pfmetPx,pfmetPy,0,sqrt(pfmetPx * pfmetPx + pfmetPy * pfmetPy));
    // ---- hadronic recoil vector --------------------------------------
    TLorentzVector pfhadP4 = -pfmetP4 - llP4; 
    // ---- transverse mass calculation ---------------------------------
    float ptZ          = llP4.Pt();
    float mZ           = llP4.M();
    float pxZ          = llP4.Px();
    float pyZ          = llP4.Py();
    float ZEt          = sqrt(ptZ*ptZ + mZ*mZ);
    float metPt        = pfmetP4.Pt();
    // ---- assume Z is on mass shell -----------------------------------
    float ZnunuEt      = sqrt(metPt*metPt + 91.2*91.2); 
    float tMassSquare  = (ptZ+metPt)*(ptZ+metPt) - (pxZ+pfmetPx)*(pxZ+pfmetPx) - (pyZ+pfmetPy)*(pyZ+pfmetPy);
    float tMassSquareP = (ZEt+ZnunuEt)*(ZEt+ZnunuEt) - (pxZ+pfmetPx)*(pxZ+pfmetPx) - (pyZ+pfmetPy)*(pyZ+pfmetPy);
    // ---- Fill the myTree ---------------------------------------------
    eventNum_   = iEvent.id().event();
    runNum_     = iEvent.id().run();
    lumi_       = iEvent.luminosityBlock();
    isRealData_ = iEvent.isRealData() ? 1:0;
    rho_        = *rho; 
    llM_        = mZ; 
    llPt_       = ptZ;
    llPhi_      = llP4.Phi();
    llY_        = llP4.Rapidity();
    llDPhi_     = fabs(myLeptons[0].p4.DeltaPhi(myLeptons[1].p4));
    TLorentzVector lepP4(0,0,0,0); 
    for(unsigned l = 0; l < myLeptons.size(); l++) {
      lepP4 += myLeptons[l].p4;
      lepPt_   ->push_back(myLeptons[l].p4.Pt());
      lepEta_  ->push_back(myLeptons[l].p4.Eta());
      lepPhi_  ->push_back(myLeptons[l].p4.Phi());
      lepE_    ->push_back(myLeptons[l].p4.Energy());
      lepId_   ->push_back(myLeptons[l].id);
      lepChId_ ->push_back(myLeptons[l].chid);
    }   
    pfmet_      = (pfmetCol_->front()).pt();
    pfmetPhi_   = (pfmetCol_->front()).phi();
    pfhadPt_    = pfhadP4.Pt();
    pfSumEt_    = (pfmetCol_->front()).sumEt();
    mT_         = sqrt(tMassSquare);
    mTp_        = sqrt(tMassSquareP);
    mZj1_       = (llP4+myJets[0].p4).M();
    if (nJets_ > 1) { 
      mj1j2_    = (myJets[0].p4+myJets[1].p4).M();
    } 
    mLep_       = lepP4.M(); 
    for(unsigned j = 0; j < myJets.size(); j++) {
      jetllDPhi_   ->push_back(fabs(llP4.DeltaPhi(myJets[j].p4)));
      jetPt_       ->push_back(myJets[j].p4.Pt()); 
      jetEta_      ->push_back(myJets[j].p4.Eta()); 
      jetPhi_      ->push_back(myJets[j].p4.Phi()); 
      jetE_        ->push_back(myJets[j].p4.Energy()); 
      jetY_        ->push_back(myJets[j].p4.Rapidity()); 
      jetArea_     ->push_back(myJets[j].area);
      jetBeta_     ->push_back(myJets[j].beta);
      jetBetaStar_ ->push_back(myJets[j].betaStar);
      jetJEC_      ->push_back(myJets[j].jec);
      jetUNC_      ->push_back(myJets[j].unc);
      jetCHF_      ->push_back(myJets[j].chf);
      jetPHF_      ->push_back(myJets[j].phf);
      jetNHF_      ->push_back(myJets[j].nhf);
      jetMUF_      ->push_back(myJets[j].muf);
      jetELF_      ->push_back(myJets[j].elf);
      jetId_       ->push_back(myJets[j].id);  
    }  
    myTree_->Fill();
  }// if (nVtx > 0 && nLeptons > 1 && nJets > 0)
}
// ---- method called once each job just after ending the event loop  ---
void ZJetsExpress::endJob() 
{
}
// ---- method for tree building ----------------------------------------
void ZJetsExpress::buildTree()
{
  lepPt_             = new std::vector<float>();
  lepEta_            = new std::vector<float>();
  lepPhi_            = new std::vector<float>();
  lepE_              = new std::vector<float>();
  lepChId_           = new std::vector<int>();
  lepId_             = new std::vector<int>();
  jetPt_             = new std::vector<float>(); 
  jetEta_            = new std::vector<float>();
  jetPhi_            = new std::vector<float>();
  jetE_              = new std::vector<float>();
  jetY_              = new std::vector<float>();
  jetArea_           = new std::vector<float>();
  jetBeta_           = new std::vector<float>();
  jetBetaStar_       = new std::vector<float>();
  jetJEC_            = new std::vector<float>();
  jetUNC_            = new std::vector<float>();
  jetllDPhi_         = new std::vector<float>();
  jetCHF_            = new std::vector<float>();
  jetPHF_            = new std::vector<float>();
  jetNHF_            = new std::vector<float>();
  jetMUF_            = new std::vector<float>();
  jetELF_            = new std::vector<float>();
  jetId_             = new std::vector<int>();
  vtxZ_              = new std::vector<float>();
  vtxNdof_           = new std::vector<float>(); 
  puZpositions_      = new std::vector<float>();
  puSumpT_lowpT_     = new std::vector<float>();
  puSumpT_highpT_    = new std::vector<float>(); 
  puNtrks_lowpT_     = new std::vector<float>();
  puNtrks_highpT_    = new std::vector<float>();
  // ---- global event variables ----------------------------------------
  myTree_->Branch("isRealData"       ,&isRealData_        ,"isRealData/I");
  myTree_->Branch("eventNum"         ,&eventNum_          ,"eventNum/I");
  myTree_->Branch("runNum"           ,&runNum_            ,"runNum/I");
  myTree_->Branch("lumi"             ,&lumi_              ,"lumi/I");
  myTree_->Branch("nVtx"             ,&nVtx_              ,"nVtx/I");
  myTree_->Branch("nLeptons"         ,&nLeptons_          ,"nLeptons/I");
  myTree_->Branch("nJets"            ,&nJets_             ,"nJets/I"); 
  myTree_->Branch("rho"              ,&rho_               ,"rho/F");
  // ---- met variables -------------------------------------------------
  myTree_->Branch("pfmet"            ,&pfmet_             ,"pfmet/F");
  myTree_->Branch("pfmetPhi"         ,&pfmetPhi_          ,"pfmetPhi/F");
  myTree_->Branch("pfhadPt"          ,&pfhadPt_           ,"pfhadPt_/F");
  myTree_->Branch("pfSumEt"          ,&pfSumEt_           ,"pfSumEt/F");
  // ---- mass variables ------------------------------------------------
  myTree_->Branch("mT"               ,&mT_                ,"mT/F");
  myTree_->Branch("mTp"              ,&mTp_               ,"mTp/F");
  myTree_->Branch("mZj1"             ,&mZj1_              ,"mZj1/F");
  myTree_->Branch("mj1j2"            ,&mj1j2_             ,"mj1j2/F");
  myTree_->Branch("mLep"             ,&mLep_              ,"mLep/F");
  // ---- dilepton variables --------------------------------------------
  myTree_->Branch("llM"              ,&llM_               ,"llM/F");
  myTree_->Branch("llPt"             ,&llPt_              ,"llPt/F");
  myTree_->Branch("llPhi"            ,&llPhi_             ,"llPhi/F");
  myTree_->Branch("llDPhi"           ,&llDPhi_            ,"llDPhi/F");
  myTree_->Branch("llY"              ,&llY_               ,"llY/F");
  // ---- lepton variables ----------------------------------------------
  myTree_->Branch("lepPt"            ,"vector<float>"     ,&lepPt_);
  myTree_->Branch("lepEta"           ,"vector<float>"     ,&lepEta_);
  myTree_->Branch("lepPhi"           ,"vector<float>"     ,&lepPhi_);
  myTree_->Branch("lepE"             ,"vector<float>"     ,&lepE_);
  myTree_->Branch("lepChId"          ,"vector<int>"       ,&lepChId_);
  myTree_->Branch("lepId"            ,"vector<int>"       ,&lepId_);
  // ---- jet variables -------------------------------------------------
  myTree_->Branch("jetPt"            ,"vector<float>"     ,&jetPt_);
  myTree_->Branch("jetEta"           ,"vector<float>"     ,&jetEta_);
  myTree_->Branch("jetPhi"           ,"vector<float>"     ,&jetPhi_);
  myTree_->Branch("jetE"             ,"vector<float>"     ,&jetE_);
  myTree_->Branch("jetY"             ,"vector<float>"     ,&jetY_);
  myTree_->Branch("jetArea"          ,"vector<float>"     ,&jetArea_);
  myTree_->Branch("jetBeta"          ,"vector<float>"     ,&jetBeta_);
  myTree_->Branch("jetBetaStar"      ,"vector<float>"     ,&jetBetaStar_);
  myTree_->Branch("jetJEC"           ,"vector<float>"     ,&jetJEC_);
  myTree_->Branch("jetUNC"           ,"vector<float>"     ,&jetUNC_);
  myTree_->Branch("jetllDPhi"        ,"vector<float>"     ,&jetllDPhi_);
  myTree_->Branch("jetCHF"           ,"vector<float>"     ,&jetCHF_);
  myTree_->Branch("jetPHF"           ,"vector<float>"     ,&jetPHF_);
  myTree_->Branch("jetNHF"           ,"vector<float>"     ,&jetNHF_);
  myTree_->Branch("jetMUF"           ,"vector<float>"     ,&jetMUF_);
  myTree_->Branch("jetELF"           ,"vector<float>"     ,&jetELF_);
  myTree_->Branch("jetId"            ,"vector<int>"       ,&jetId_);
  // ---- vertex variables ----------------------------------------------
  myTree_->Branch("vtxZ"             ,"vector<float>"     ,&vtxZ_);
  myTree_->Branch("vtxNdof"          ,"vector<float>"     ,&vtxNdof_);
  // ---- simulated pu variables ----------------------------------------
  myTree_->Branch("PUnumInteractions",&puNumInteractions_ ,"puNumInteractions_/I");
  myTree_->Branch("PUzPositions"     ,"vector<float>"     ,&puZpositions_);
  myTree_->Branch("PUsumPtLowPt"     ,"vector<float>"     ,&puSumpT_lowpT_);
  myTree_->Branch("PUsumPtHighPt"    ,"vector<float>"     ,&puSumpT_highpT_);
  myTree_->Branch("PUnTrksLowPt"     ,"vector<float>"     ,&puNtrks_lowpT_);
  myTree_->Branch("PUnTrksHighPt"    ,"vector<float>"     ,&puNtrks_highpT_);  
}
// ---- method for tree initialization ----------------------------------
void ZJetsExpress::clearTree()
{
  isRealData_        = -999;
  eventNum_          = -999;
  runNum_            = -999;
  lumi_              = -999;
  nVtx_              = -999;
  nLeptons_          = -999;
  nJets_             = -999;
  rho_               = -999;
  pfmet_             = -999;
  pfmetPhi_          = -999;
  pfhadPt_           = -999;
  pfSumEt_           = -999;
  mT_                = -999;
  mTp_               = -999;
  mZj1_              = -999; 
  mj1j2_             = -999;
  mLep_              = -999;  
  llM_               = -999;
  llPt_              = -999; 
  llPhi_             = -999;
  llDPhi_            = -999;
  llY_               = -999;
  puNumInteractions_ = -999;
  lepPt_             ->clear();
  lepEta_            ->clear();
  lepPhi_            ->clear();
  lepE_              ->clear();
  lepChId_           ->clear();
  lepId_             ->clear();
  jetPt_             ->clear();
  jetEta_            ->clear();
  jetPhi_            ->clear();
  jetE_              ->clear();
  jetY_              ->clear();
  jetArea_           ->clear();
  jetBeta_           ->clear();
  jetBetaStar_       ->clear();
  jetJEC_            ->clear();
  jetUNC_            ->clear();
  jetllDPhi_         ->clear();
  jetCHF_            ->clear();
  jetPHF_            ->clear();
  jetNHF_            ->clear();
  jetMUF_            ->clear();
  jetELF_            ->clear();
  jetId_             ->clear();
  vtxZ_              ->clear();
  vtxNdof_           ->clear();
  puZpositions_      ->clear();
  puSumpT_lowpT_     ->clear();
  puSumpT_highpT_    ->clear(); 
  puNtrks_lowpT_     ->clear();
  puNtrks_highpT_    ->clear();
}
// ---- define this as a plug-in ----------------------------------------
DEFINE_FWK_MODULE(ZJetsExpress);
