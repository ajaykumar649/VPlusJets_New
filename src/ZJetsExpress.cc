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
      void buildTree();
      void clearTree();

      struct LEPTON {TLorentzVector p4; int chid; int id;};
      struct JET    
      {
        TLorentzVector p4; 
        int   id; 
        float area;
        float beta;
        float betaStar;
        float jec; 
        float unc;
        float chf;
        float nhf;
        float phf;
        float muf;
        float elf;
      };

      static bool leptonSortingRule(LEPTON x, LEPTON y) {return x.p4.Pt() > y.p4.Pt();}
      static bool jetSortingRule(JET x, JET y) {return x.p4.Pt() > y.p4.Pt();}

      // ---------- member data ---------------------------
      edm::Service<TFileService> fTFileService;	
      TTree *myTree_;
      TH1I  *hist_;
      TH1D  *mcPU_;
      bool mIsJECuncSet;
      const JetCorrector *mJEC;
      JetCorrectionUncertainty *mJECunc;

      // ---------- configurable parameters ---------------
      double mMinJetPt,mMaxJetEta,mMinLepPt,mMaxLepEta;
      string mJECservice,mPayloadName;
      edm::InputTag mJetsName,mSrcRho;
      
      // ---------- tree variables ------------------------
      // #--- event tag
      int eventNum_;
      int runNum_;
      int lumi_;
      int isRealData_;
      // #--- di-lepton system
      float llM_; 
      float llY_;
      float llPt_;
      float llPhi_;
      float llDPhi_; 
      vector<float> *jetllDPhi_;
      vector<float> *lepPt_,*lepEta_,*lepPhi_,*lepE_;
      vector<int>   *lepChId_,*lepId_;
      int nLeptons_;
      // #--- jets block
      vector<float> *jetPt_,*jetEta_,*jetY_,*jetPhi_,*jetE_;
      vector<float> *jetCHF_,*jetPHF_,*jetNHF_,*jetMUF_,*jetELF_;
      vector<float> *jetBeta_,*jetBetaStar_,*jetArea_,*jetJEC_,*jetUNC_;
      vector<int>   *jetId_; 
      int nJets_;
      // #--- global event variables
      float pfmet_;
      float pfSumEt_;
      float pfmetPhi_;
      float pfhadPt_; // pt of the vector sum of pflow particles (computed from pfmet by excluding Z daughters)
      float mT_; // transverse mass of the Z and the hadronic recoil
      float mTp_; // transverse mass of the Z and the hadronic recoil assuming 
      float mZj1_;
      float mj1j2_;
      float mLep_; // inv mass of all leptons
      float rho_; // rho from L1FastJetCorrection
      vector<float> *vtxZ_,*vtxNdof_;
      int   nVtx_;
      // --- for MC pile-up reweighting
      int puNumInteractions_;
      vector<float> *puZpositions_,*puSumpT_lowpT_,*puSumpT_highpT_,*puNtrks_lowpT_,*puNtrks_highpT_;
};

ZJetsExpress::ZJetsExpress(const edm::ParameterSet& iConfig)
{
  mMinJetPt    = iConfig.getParameter<double>        ("minJetPt");
  mMaxJetEta   = iConfig.getParameter<double>        ("maxJetEta");
  mMinLepPt    = iConfig.getParameter<double>        ("minLepPt");
  mMaxLepEta   = iConfig.getParameter<double>        ("maxLepEta");
  mJetsName    = iConfig.getParameter<edm::InputTag> ("jets");
  mSrcRho      = iConfig.getParameter<edm::InputTag> ("srcRho");
  mJECservice  = iConfig.getParameter<std::string>   ("jecService");
  mPayloadName = iConfig.getParameter<std::string>   ("payload");

  hist_   = fTFileService->make<TH1I>("hist", "hist",6,-1,5);
  mcPU_   = fTFileService->make<TH1D>("mcPU", "mcPU",40,0,40);
  myTree_ = fTFileService->make<TTree>("events", "events");

  buildTree();
}

ZJetsExpress::~ZJetsExpress()
{

}

void ZJetsExpress::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  hist_->Fill(-1); // event counter
  clearTree();

  //----  MC truth pileup block -----------------------------
  isRealData_ = iEvent.isRealData() ? 1:0;
  if (!isRealData_) {
    Handle<vector<PileupSummaryInfo> > pileupInfo;
    iEvent.getByLabel("addPileupInfo", pileupInfo);
    vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = pileupInfo->begin(); PVI != pileupInfo->end(); ++PVI) {
      if (PVI->getBunchCrossing() != 0) continue; // only look at in-time pileup
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

  //-------------- Rho ------------------------------------------------
  Handle<double> rho;
  iEvent.getByLabel(mSrcRho,rho);

  //----- vertices block ------------------------------------
  edm::Handle<VertexCollection> vertices_;
  iEvent.getByLabel("offlinePrimaryVertices", vertices_);
  const reco::Vertex *primVtx = &(*(vertices_.product()))[0];
  
  for(VertexCollection::const_iterator vertexit = vertices_->begin(); vertexit != vertices_->end(); ++vertexit) {  
    if (!vertexit->isFake()) {
      vtxZ_   ->push_back(vertexit->z());
      vtxNdof_->push_back(vertexit->ndof());
    }
  }

  //------------- leptons block -------------------------------------------
  Handle<View<GsfElectron> > electrons_;
  iEvent.getByLabel("gsfElectrons", electrons_);
  Handle<View<Muon> > muons_;
  iEvent.getByLabel("muons",muons_);
  vector<LEPTON> myLeptons;

  for(View<Muon>::const_iterator Mit = muons_->begin(); Mit != muons_->end(); ++Mit) {
    if (!(Mit->isGlobalMuon())) continue;
    if (!(Mit->isTrackerMuon())) continue;
    if (!muon::isGoodMuon(*Mit,muon::GlobalMuonPromptTight)) continue;
    if ((Mit->pt() < mMinLepPt) || (fabs(Mit->eta()) > mMaxLepEta)) continue;
    if (fabs(Mit->globalTrack()->normalizedChi2()) > 10) continue;
    if (fabs(Mit->innerTrack()->dz(primVtx->position())) > 1.0) continue;
    if (fabs(Mit->innerTrack()->dxy(primVtx->position())) > 0.04) continue;
    float muonIso = (Mit->isolationR03().sumPt + Mit->isolationR03().emEt + Mit->isolationR03().hadEt)/Mit->pt();
    if (muonIso > 0.15) continue;
    float nInnerTrkHits = Mit->innerTrack()->hitPattern().numberOfValidHits();
    if (nInnerTrkHits < 11) continue;
    float numberOfValidPixelHits = Mit->innerTrack()->hitPattern().numberOfValidPixelHits();
    if (numberOfValidPixelHits < 1) continue;
    float numberOfMatches = Mit->numberOfMatches();
    if (numberOfMatches < 2) continue;
    float dptOpt = Mit->innerTrack()->ptError()/Mit->pt();
    if (dptOpt > 0.1) continue;
      
    LEPTON aLepton;
    TLorentzVector lepP4(Mit->p4().Px(),Mit->p4().Py(),Mit->p4().Pz(),Mit->p4().E());
    aLepton.p4   = lepP4;
    aLepton.chid = 2*Mit->charge();
    aLepton.id   = 1;
    myLeptons.push_back(aLepton);
  }// muon loop
  
  for(View<GsfElectron>::const_iterator El = electrons_->begin();El != electrons_->end(); ++El) {
    float elPt                           = El->p4().Pt();
    float elEta                          = El->p4().Eta();
    float trackIsoRel                    = El->dr03TkSumPt()/El->p4().Pt();
    float ecalIsoRel                     = El->dr03EcalRecHitSumEt()/El->p4().Pt();
    float hcalIsoRel                     = El->dr03HcalTowerSumEt()/El->p4().Pt();
    float sigmaIetaIeta                  = El->sigmaIetaIeta();
    float hadronicOverEm                 = El->hadronicOverEm();
    float deltaPhiSuperClusterTrackAtVtx = El->deltaPhiSuperClusterTrackAtVtx();
    float deltaEtaSuperClusterTrackAtVtx = El->deltaEtaSuperClusterTrackAtVtx();
    bool  isTight(false);
    if ((elPt < mMinLepPt) || (fabs(elEta) > mMaxLepEta)) continue;
    if (El->isEB()) {
      float combinedIso03 = (El->dr03TkSumPt()+max(0.,El->dr03EcalRecHitSumEt()-1.)+El->dr03HcalTowerSumEt())/El->p4().Pt();
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
    if (El->isEE()) {
      float combinedIso03 = (El->dr03TkSumPt()+El->dr03EcalRecHitSumEt()+El->dr03HcalTowerSumEt())/El->p4().Pt(); 
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
    TLorentzVector lepP4(El->p4().Px(),El->p4().Py(),El->p4().Pz(),El->p4().E());
    aLepton.p4   = lepP4;
    aLepton.chid = El->charge();
    aLepton.id   = 0;
    if (isTight) {
      aLepton.id = 1;
    }
    myLeptons.push_back(aLepton);
  } // end of electrons loop

  hist_->Fill(int(myLeptons.size()));

  sort(myLeptons.begin(),myLeptons.end(),leptonSortingRule);
 
  //--------------- jets block -----------------------------------
  Handle<PFJetCollection> jets_;
  iEvent.getByLabel(mJetsName,jets_);
  vector<JET> myJets;
  //---------------- set the JEC ---------------------------
  mJEC = JetCorrector::getJetCorrector(mJECservice,iSetup);
  //---- the JEC uncertainty only needs to be set-up once --
  if (!mIsJECuncSet) {
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get<JetCorrectionsRecord>().get(mPayloadName,JetCorParColl); 
    JetCorrectorParameters const& JetCorPar = (*JetCorParColl)["Uncertainty"];
    mJECunc = new JetCorrectionUncertainty(JetCorPar);
    mIsJECuncSet = true;
  }
  for(PFJetCollection::const_iterator Jit = jets_->begin(); Jit != jets_->end(); ++Jit) {
    TLorentzVector jetP4(Jit->px(),Jit->py(),Jit->pz(),Jit->energy());
    bool jetIsDuplicate(false);

    //----- remove the leptons -----------
    for(unsigned int l = 0; l < myLeptons.size(); l++) {
      float DR = myLeptons[l].p4.DeltaR(jetP4);
      if (DR < 0.4) {
        jetIsDuplicate = true; 
      }
    }// lepton loop 
    
    if (jetIsDuplicate) continue; 
        
    int index = Jit - jets_->begin();
    edm::RefToBase<reco::Jet> jetRef(edm::Ref<PFJetCollection>(jets_,index));
    double jec = mJEC->correction(*Jit,jetRef,iEvent,iSetup);
    mJECunc->setJetEta(Jit->eta());
    mJECunc->setJetPt(jec * Jit->pt());
    double unc = mJECunc->getUncertainty(true);

    if ((jec * jetP4.Pt() < mMinJetPt) || (fabs(jetP4.Eta()) > mMaxJetEta)) continue;

    //---- jet vertex association -----------
    //---- get the vector of tracks -----
    reco::TrackRefVector vTrks(Jit->getTrackRefs());
    float sumTrkPt(0.0),sumTrkPtBeta(0.0),sumTrkPtBetaStar(0.0),beta(0.0),betaStar(0.0);
    //---- loop over the tracks of the jet ----
    for(reco::TrackRefVector::const_iterator i_trk = vTrks.begin(); i_trk != vTrks.end(); i_trk++) {
      sumTrkPt += (*i_trk)->pt();
      //---- loop over all vertices ----------------------------
      for(unsigned ivtx = 0;ivtx < vertices_->size();ivtx++) {
        //---- loop over the tracks associated with the vertex --- 
        if ((*vertices_)[ivtx].isFake()) continue; 
        for(reco::Vertex::trackRef_iterator i_vtxTrk = (*vertices_)[ivtx].tracks_begin(); i_vtxTrk != (*vertices_)[ivtx].tracks_end(); ++i_vtxTrk) {
          //---- match the jet track to the track from the vertex ----
          reco::TrackRef trkRef(i_vtxTrk->castTo<reco::TrackRef>());
          //---- check if the tracks match -------------------------
          if (trkRef == (*i_trk)) {
            if (ivtx == 0) {
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
    aJet.area     = Jit->jetArea();
    aJet.chf      = Jit->chargedHadronEnergyFraction();
    aJet.nhf      = (Jit->neutralHadronEnergy() + Jit->HFHadronEnergy())/Jit->energy();
    aJet.phf      = Jit->photonEnergyFraction();
    aJet.elf      = Jit->electronEnergyFraction();
    aJet.muf      = Jit->muonEnergyFraction();
    int chm       = Jit->chargedHadronMultiplicity();
    int npr       = Jit->chargedMultiplicity() + Jit->neutralMultiplicity();
    bool id       = (npr>1 && aJet.phf<0.99 && aJet.nhf<0.99 && ((fabs(Jit->eta())<=2.4 && aJet.nhf<0.9 && aJet.phf<0.9 && aJet.elf<0.99 && aJet.chf>0 && chm>0) || fabs(Jit->eta())>2.4));
    aJet.id       = (int)id;
    aJet.beta     = beta;
    aJet.betaStar = betaStar;
    myJets.push_back(aJet);  
  }// jet loop

  sort(myJets.begin(),myJets.end(),jetSortingRule);    

  //------------ MET block ----------------------------
  Handle<View<PFMET> > pfmetCol_;
  iEvent.getByLabel("pfMet", pfmetCol_);
  float pfmetPx = (pfmetCol_->front()).px();
  float pfmetPy = (pfmetCol_->front()).py();

  nVtx_       = int(vtxZ_->size());
  nLeptons_   = int(myLeptons.size()); 
  nJets_      = int(myJets.size());

  if ((nVtx_ > 0) && (nLeptons_ > 1) && (nJets_ > 0)) {
    TLorentzVector llP4 = myLeptons[0].p4 + myLeptons[1].p4;
    TLorentzVector pfmetP4(pfmetPx,pfmetPy,0,sqrt(pfmetPx * pfmetPx + pfmetPy * pfmetPy));
    TLorentzVector pfhadP4 = -pfmetP4 - llP4; // hadronic recoil vector

    // --- transverse mass calculation
    float ptZ          = llP4.Pt();
    float mZ           = llP4.M();
    float pxZ          = llP4.Px();
    float pyZ          = llP4.Py();
    float ZEt          = sqrt(ptZ*ptZ + mZ*mZ);
    float metPt        = pfmetP4.Pt();
    float ZnunuEt      = sqrt(metPt*metPt + 91.2*91.2); // assume Z is on mass shell
    float tMassSquare  = (ptZ+metPt)*(ptZ+metPt) - (pxZ+pfmetPx)*(pxZ+pfmetPx) - (pyZ+pfmetPy)*(pyZ+pfmetPy);
    float tMassSquareP = (ZEt+ZnunuEt)*(ZEt+ZnunuEt) - (pxZ+pfmetPx)*(pxZ+pfmetPx) - (pyZ+pfmetPy)*(pyZ+pfmetPy);
  
    // --- Fill the myTree
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
      mj1j2_      = (myJets[0].p4+myJets[1].p4).M();
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


// ------------ method called once each job just before starting event loop  ------------
void ZJetsExpress::beginJob()
{
  mIsJECuncSet = false;
}

// ------------ method called once each job just after ending the event loop  ------------
void ZJetsExpress::endJob() 
{
}

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
  
  myTree_->Branch("isRealData"       ,&isRealData_        ,"isRealData/I");
  myTree_->Branch("eventNum"         ,&eventNum_          ,"eventNum/I");
  myTree_->Branch("runNum"           ,&runNum_            ,"runNum/I");
  myTree_->Branch("lumi"             ,&lumi_              ,"lumi/I");
  myTree_->Branch("nVtx"             ,&nVtx_              ,"nVtx/I");
  myTree_->Branch("nLeptons"         ,&nLeptons_          ,"nLeptons/I");
  myTree_->Branch("nJets"            ,&nJets_             ,"nJets/I"); 
  myTree_->Branch("rho"              ,&rho_               ,"rho/F");

  myTree_->Branch("pfmet"            ,&pfmet_             ,"pfmet/F");
  myTree_->Branch("pfmetPhi"         ,&pfmetPhi_          ,"pfmetPhi/F");
  myTree_->Branch("pfhadPt"          ,&pfhadPt_           ,"pfhadPt_/F");
  myTree_->Branch("pfSumEt"          ,&pfSumEt_           ,"pfSumEt/F");
  
  myTree_->Branch("mT"               ,&mT_                ,"mT/F");
  myTree_->Branch("mTp"              ,&mTp_               ,"mTp/F");
  myTree_->Branch("mZj1"             ,&mZj1_              ,"mZj1/F");
  myTree_->Branch("mj1j2"            ,&mj1j2_             ,"mj1j2/F");
  myTree_->Branch("mLep"             ,&mLep_              ,"mLep/F");
 
  myTree_->Branch("llM"              ,&llM_               ,"llM/F");
  myTree_->Branch("llPt"             ,&llPt_              ,"llPt/F");
  myTree_->Branch("llPhi"            ,&llPhi_             ,"llPhi/F");
  myTree_->Branch("llDPhi"           ,&llDPhi_            ,"llDPhi/F");
  myTree_->Branch("llY"              ,&llY_               ,"llY/F");

  myTree_->Branch("lepPt"            ,"vector<float>"     ,&lepPt_);
  myTree_->Branch("lepEta"           ,"vector<float>"     ,&lepEta_);
  myTree_->Branch("lepPhi"           ,"vector<float>"     ,&lepPhi_);
  myTree_->Branch("lepE"             ,"vector<float>"     ,&lepE_);
  myTree_->Branch("lepChId"          ,"vector<int>"       ,&lepChId_);
  myTree_->Branch("lepId"            ,"vector<int>"       ,&lepId_);
  
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

  myTree_->Branch("vtxZ"             ,"vector<float>"     ,&vtxZ_);
  myTree_->Branch("vtxNdof"          ,"vector<float>"     ,&vtxNdof_);

  myTree_->Branch("PUnumInteractions","vector<int>"       ,&puNumInteractions_);
  myTree_->Branch("PUzPositions"     ,"vector<float>"     ,&puZpositions_);
  myTree_->Branch("PUsumPtLowPt"     ,"vector<float>"     ,&puSumpT_lowpT_);
  myTree_->Branch("PUsumPtHighPt"    ,"vector<float>"     ,&puSumpT_highpT_);
  myTree_->Branch("PUnTrksLowPt"     ,"vector<float>"     ,&puNtrks_lowpT_);
  myTree_->Branch("PUnTrksHighPt"    ,"vector<float>"     ,&puNtrks_highpT_);  
}
void ZJetsExpress::clearTree()
{
  isRealData_ = -999;
  eventNum_   = -999;
  runNum_     = -999;
  lumi_       = -999;
  nVtx_       = -999;
  nLeptons_   = -999;
  nJets_      = -999;
  rho_        = -999;
  pfmet_      = -999;
  pfmetPhi_   = -999;
  pfhadPt_    = -999;
  pfSumEt_    = -999;
  mT_         = -999;
  mTp_        = -999;
  mZj1_       = -999; 
  mj1j2_      = -999;
  mLep_       = -999;  
  llM_        = -999;
  llPt_       = -999; 
  llPhi_      = -999;
  llDPhi_     = -999;
  llY_        = -999;
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

//define this as a plug-in
DEFINE_FWK_MODULE(ZJetsExpress);
