import FWCore.ParameterSet.Config as cms
process = cms.Process("SKIM")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/873/80730A1C-85DD-E011-A7B2-BCAEC53296F9.root',
    '/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/872/6E90F149-37DE-E011-886B-003048D2C0F2.root',
    '/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/866/FCC1BADC-D7DB-E011-B711-001D09F244BB.root',
    '/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/866/D6DDD0A9-C2DB-E011-A408-001D09F2512C.root',
    '/store/data/Run2011B/DoubleMu/AOD/PromptReco-v1/000/175/866/501F8B60-C8DB-E011-A823-003048D2C0F4.root' 
    )
)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService=cms.Service("TFileService",fileName=cms.string('DebugHistos.root'))

#############   MuMu Filter ##########################
process.ZmumuFilter  = cms.EDFilter('ZMuMuFilter',
        muons   = cms.InputTag('muons'), 
        minPt   = cms.double(15),
        maxEta  = cms.double(2.4),
        minMass = cms.double(20),
        debug   = cms.untracked.int32(1)
)

#############   output module ########################
process.aod = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *',
        #------- Tracks -------------------
        'keep *_generalTracks_*_*',
        #------- PF Candidates ------------
        'keep *_particleFlow__*',
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        #------- Leptons ------------------
        'keep *_gsfElectrons_*_*',
        'keep *_muons__*',
        'keep *_globalMuons_*_*',
        #------- PFJet collections --------
        'keep *_kt6PFJets_rho_SKIM',
        'keep *_kt6PFJets_sigma_SKIM',
        'keep *_ak5PFJets__SKIM',  
        #------- Trigger collections ------
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        #------- Various collections ------
        'keep *_EventAuxilary_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_offlinePrimaryVerticesWithBS_*_*',
        #------- MET collections ----------
        'keep *_met_*_*',
        'keep *_pfMet_*_*'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('skimPath')),
    fileName = cms.untracked.string('ZmumuAOD.root')
)
############# turn-on the fastjet area calculation needed for the L1Fastjet ##############
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
############# slimming the PFJet collection by raising the pt cut #################
process.ak5PFJets.jetPtMin = cms.double(15.0)

process.skimPath = cms.Path(

process.ZmumuFilter *
############# run the kt6PFJets reconstruction to calculate the fastjet density rho #####
process.kt6PFJets * 
############# reconstruct the PFJets with jet area calculation ############
process.ak5PFJets
)

process.end = cms.EndPath(process.aod)

