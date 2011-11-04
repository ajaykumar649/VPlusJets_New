import FWCore.ParameterSet.Config as cms
process = cms.Process("ANA")
# ---- access the global tag (needed for the JEC) -----------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_42_V19::All'
# ---- load the reco jet configurations --------------------------------
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
# ---- load the JEC services --------------------------------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
# ---- format the message service ---------------------------------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
# ---- maximum number of events to run over -----------------------------
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
# ---- define the source ------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/873/80730A1C-85DD-E011-A7B2-BCAEC53296F9.root')
)
# ---- define the output file -------------------------------------------
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ZJetsExpress.root"),
    closeFileFast = cms.untracked.bool(True)
)
# ---- ZJetsExpress analyzer --------------------------------------------
process.accepted = cms.EDAnalyzer('ZJetsExpress',
    jets            = cms.InputTag('ak5PFJets'),
    srcRho          = cms.InputTag('kt6PFJets','rho'),
    minNjets        = cms.int32(-1),
    jetLepIsoRadius = cms.double(0.4),
    minJetPt        = cms.double(30),
    maxJetEta       = cms.double(2.5),
    minLepPt        = cms.double(20),
    maxLepEta       = cms.double(2.4),
    jecService      = cms.string('ak5PFL1FastL2L3Residual'),
    payload         = cms.string('AK5PF'),
    isMC            = cms.bool(False)
)
# ---- duplicate the analyzer with different name -----------------------
process.rejected = process.accepted.clone()
# ---- filter the required HLT bits -------------------------------------
process.hltFilter = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('HLT_DoubleMu6_v*','HLT_DoubleMu7_v*','HLT_DoubleMu8_v*','HLT_Mu13_Mu8_v*'),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)
############# turn-on the fastjet area calculation needed for the L1Fastjet ##############
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
############# slimming the PFJet collection by raising the pt cut #################
process.ak5PFJets.jetPtMin = cms.double(15.0)

# ---- first sequence: events that pass the trigger ---------------------
process.s1 = cms.Sequence(process.hltFilter + process.kt6PFJets + process.ak5PFJets + process.accepted)
process.p1 = cms.Path(process.s1)
# ---- second sequence: events that fail the trigger --------------------
process.s2 = cms.Sequence(~process.hltFilter + process.kt6PFJets + process.ak5PFJets + process.rejected)
process.p2 = cms.Path(process.s2)
# ---- schedule ---------------------------------------------------------
process.schedule = cms.Schedule(process.p1,process.p2)
