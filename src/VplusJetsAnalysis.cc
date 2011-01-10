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


// user include files
#include "ElectroWeakAnalysis/VPlusJets/interface/VplusJetsAnalysis.h" 
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"


ewk::VplusJetsAnalysis::VplusJetsAnalysis(const edm::ParameterSet& iConfig) :
  fOutputFileName ( iConfig.getParameter<std::string>("HistOutFile") ),
  hOutputFile ( new TFile( fOutputFileName.c_str(), "RECREATE" ) ), 
  myTree ( new TTree(iConfig.getParameter<std::string>("TreeName").c_str(),"V+jets Tree") ),
  CaloJetFiller (  new JetTreeFiller("CaloJetFiller", myTree, "Calo", iConfig) ),
  CorrectedCaloJetFiller (  new JetTreeFiller("CorrectedCaloJetFiller", 
					      myTree, "Cor", iConfig) ),
  CorrectedPFJetFiller (  new JetTreeFiller("CorrectedPFJetFiller", 
					      myTree, "PFCor", iConfig) ),
  CorrectedJPTJetFiller (  new JetTreeFiller("CorrectedJPTJetFiller", 
					      myTree, "JPTCor", iConfig) ),
  GenJetFiller ( new JetTreeFiller("GenJetFiller", myTree, "Gen", iConfig) ),
  PFJetFiller (  new JetTreeFiller("PFJetFiller", myTree, "PF", iConfig) ),
  JPTJetFiller ( new JetTreeFiller("JPTJetFiller", myTree, "JPT", iConfig) ),
  recoBosonFillerE( new VtoElectronTreeFiller( iConfig.getParameter<std::string>("VBosonType").c_str(), 
					       myTree, iConfig) ),
  recoBosonFillerMu( new VtoMuonTreeFiller( iConfig.getParameter<std::string>("VBosonType").c_str(), 
					    myTree, iConfig) ),
  genBosonFiller( new MCTreeFiller(iConfig.getParameter<std::string>("VBosonType").c_str(), 
				   myTree, iConfig) )
{
  // Are we running over Monte Carlo ?
  runningOverMC_=iConfig.getUntrackedParameter< bool >("runningOverMC",true);
  if(  iConfig.existsAs<edm::InputTag>("srcVectorBoson") )
    mInputBoson = iConfig.getParameter<edm::InputTag>("srcVectorBoson"); 
  LeptonType_ = iConfig.getParameter<std::string>("LeptonType");
  VBosonType_ = iConfig.getParameter<std::string>("VBosonType");
  hltPath_ = iConfig.getParameter<edm::InputTag>("hltTag");
  triggerSummaryLabel_ = 
    iConfig.getParameter<edm::InputTag>("triggerSummaryLabel"); 
}

 

ewk::VplusJetsAnalysis::~VplusJetsAnalysis() {}


void ewk::VplusJetsAnalysis::beginJob() {

  // Declare all the branches of the tree
  declareTreeBranches();
}




// ------------ method called to produce the data  ------------
void ewk::VplusJetsAnalysis::analyze(const edm::Event& iEvent, 
				     const edm::EventSetup& iSetup) {

  // write event information
  run   = 0;
  event = 0;
  lumi  = 0;
  bunch = 0;
  nPV   = 0; 
 
  // run, event, bunch crossing, ....
  run   = iEvent.id().run();
  event = iEvent.id().event();
  lumi  = iEvent.luminosityBlock();
  bunch = iEvent.bunchCrossing();


  // primary/secondary vertices
  edm::Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel("offlinePrimaryVertices",recVtxs);
  for(unsigned int ind=0;ind<recVtxs->size();ind++) 
    {
      if(nPV>20) continue;
	mPVx[ind] =   -10000.0;
	mPVy[ind] =   -10000.0;
	mPVz[ind] =   -10000.0;

      if (!((*recVtxs)[ind].isFake())) {
	nPV += 1;
	mPVx[ind] =  (*recVtxs)[ind].x() ;
	mPVy[ind] =  (*recVtxs)[ind].y() ;
	mPVz[ind] =  (*recVtxs)[ind].z() ;
      }
    }


  //////////// Beam spot //////////////
  edm::Handle<reco::BeamSpot> beamSpot;
  iEvent.getByLabel("offlineBeamSpot", beamSpot);
  mBSx = beamSpot->position().X();
  mBSy = beamSpot->position().Y();
  mBSz = beamSpot->position().Z();


  ////////////// CaloMET information //////
  edm::Handle<reco::CaloMETCollection> met;
  iEvent.getByLabel("met",met);
  if (met->size() == 0) {
    mMET   = -1.0;
    mSumET = -1.0;
    mMETSign  = -1.0;
  }
  else {
    mMET   = (*met)[0].et();
    mSumET = (*met)[0].sumEt();
    mMETSign   = (*met)[0].significance();
  }


  /////// TcMET information /////
  edm::Handle<reco::METCollection> tcmet;
  iEvent.getByLabel("tcMet", tcmet);
  if (tcmet->size() == 0) {
    mtcMET   = -1;
    mtcSumET = -1;
    mtcMETSign = -1;
  }
  else {
    mtcMET   = (*tcmet)[0].et();
    mtcSumET = (*tcmet)[0].sumEt();
    mtcMETSign = (*tcmet)[0].significance();
  }

  /////// PfMET information /////
  edm::Handle<reco::PFMETCollection> pfmet;
  iEvent.getByLabel("pfMet", pfmet);
  if (pfmet->size() == 0) {
    mpfMET   = -1;
    mpfSumET = -1;
    mpfMETSign = -1;
  }
  else {
    mpfMET   = (*pfmet)[0].et();
    mpfSumET = (*pfmet)[0].sumEt();
    mpfMETSign = (*pfmet)[0].significance();
  }






  // fill jet branches
  edm::Handle<reco::CandidateView> boson;
  iEvent.getByLabel( mInputBoson, boson);
  if( boson->size()<1 ) return; // Nothing to fill


  CaloJetFiller->fill(iEvent);
  CorrectedCaloJetFiller->fill(iEvent);
  GenJetFiller->fill(iEvent);
  PFJetFiller->fill(iEvent);
  JPTJetFiller->fill(iEvent);
  CorrectedPFJetFiller->fill(iEvent);
  CorrectedJPTJetFiller->fill(iEvent);


  /**  Store reconstructed vector boson information */
  // ------------ trigger objects 
  edm::Handle<trigger::TriggerEvent> triggerObj;
  iEvent.getByLabel(triggerSummaryLabel_,triggerObj); 
  if(!triggerObj.isValid()) { 
    edm::LogInfo("TriggerEvent") << " objects not found"; 
  }
  


 // HLT config does not change within runs!
  changed = false;
  // Initialize HLT config provider !!!!!
  if( !(hltConfig_.init(iEvent.getRun(),iSetup,hltPath_.process(),changed)) )
    throw cms::Exception("HLTConfigProvider::init() returned non 0");



  // find the filter index
  edm::InputTag filterTag;
  if(!changed) {
    std::vector<std::string> filters = hltConfig_.moduleLabels( hltPath_.label() );

    // loop over all trigger filters associated with this path
    for(std::vector<std::string>::iterator filter =
	  filters.begin(); filter!= filters.end(); ++filter ) {
       
      edm::InputTag testTag(*filter,"", hltPath_.process() );       
      int testindex = triggerObj->filterIndex(testTag);
      if ( !(testindex >= triggerObj->sizeFilters()) ) 
	filterTag=testTag;
    } 
  }


  // now store  boson information
  recoBosonFillerE->fill(iEvent, filterTag, changed);
  recoBosonFillerMu->fill(iEvent, filterTag, changed);
  

  /**  Store generated vector boson information */
  if( runningOverMC_ ) genBosonFiller->fill(iEvent);
  
  myTree->Fill();

} // analyze method






void ewk::VplusJetsAnalysis::endJob()
{
  hOutputFile->SetCompressionLevel(2);
  hOutputFile->cd();
  myTree->Write();

  delete myTree;
  hOutputFile->Close();
  delete hOutputFile;
}




//  **** Utility: declare TTree branches for ntuple variables ***
void ewk::VplusJetsAnalysis::declareTreeBranches() {

  myTree->Branch("event_runNo",  &run,   "event_runNo/I");
  myTree->Branch("event_evtNo",  &event, "event_evtNo/I");
  myTree->Branch("event_lumi",   &lumi,  "event_lumi/I"); 
  myTree->Branch("event_bunch",  &bunch, "event_bunch/I"); 
  myTree->Branch("event_nPV",    &nPV,   "event_nPV/I"); 
  myTree->Branch("event_PVx",    mPVx,   "event_PVx[20]/F"); 
  myTree->Branch("event_PVy",    mPVy,   "event_PVy[20]/F"); 
  myTree->Branch("event_PVz",    mPVz,   "event_PVz[20]/F"); 
  myTree->Branch("event_met_calomet",    &mMET,  "event_met_calomet/F"); 
  myTree->Branch("event_met_calosumet",  &mSumET,"event_met_calosumet/F"); 
  myTree->Branch("event_met_calometsignificance", &mMETSign,  "event_met_calometsignificance/F"); 
  myTree->Branch("event_met_tcmet",    &mtcMET,  "event_met_tcmet/F"); 
  myTree->Branch("event_met_tcsumet",  &mtcSumET,"event_met_tcsumet/F"); 
  myTree->Branch("event_met_tcmetsignificance", &mtcMETSign,  "event_met_tcmetsignificance/F"); 
  myTree->Branch("event_met_pfmet",    &mpfMET,  "event_met_pfmet/F"); 
  myTree->Branch("event_met_pfsumet",  &mpfSumET,"event_met_pfsumet/F"); 
  myTree->Branch("event_met_pfmetsignificance", &mpfMETSign,  "event_met_pfmetsignificance/F"); 
  myTree->Branch("event_BeamSpot_x"       ,&mBSx              ,"mBSx/F");
  myTree->Branch("event_BeamSpot_y"       ,&mBSy              ,"mBSy/F");
  myTree->Branch("event_BeamSpot_z"       ,&mBSz              ,"mBSz/F");
}





// declare this class as a plugin
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
using ewk::VplusJetsAnalysis;
DEFINE_FWK_MODULE(VplusJetsAnalysis);
