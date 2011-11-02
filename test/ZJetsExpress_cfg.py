import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_42_V19::All'
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/k/kkousour/scratch0/data/zjetAOD.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("zjetAOD_out.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.accepted = cms.EDAnalyzer('ZJetsExpress',
       jets       = cms.InputTag('ak5PFJets'),
       srcRho     = cms.InputTag('kt6PFJets','rho'),
       minJetPt   = cms.double(30),
       maxJetEta  = cms.double(2.5),
       minLepPt   = cms.double(20),
       maxLepEta  = cms.double(2.4),
       jecService = cms.string('ak5PFL1FastL2L3Residual'),
       payload    = cms.string('AK5PF')
)

process.rejected = process.accepted.clone()

############# hlt filter #########################
process.hltFilter = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('HLT_DoubleMu6_v*','HLT_DoubleMu7_v*','HLT_DoubleMu8_v*','HLT_Mu13_Mu8_v*'),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)

process.s1 = cms.Sequence(process.hltFilter + process.accepted)
process.s2 = cms.Sequence(~process.hltFilter + process.rejected)

process.p1 = cms.Path(process.s1)
process.p2 = cms.Path(process.s2)

process.schedule = cms.Schedule(process.p1,process.p2)
