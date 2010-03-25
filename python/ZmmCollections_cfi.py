import FWCore.ParameterSet.Config as cms

selectMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),                               
    #cut = cms.string('')  
    cut = cms.string('((abs(eta) < 3) & pt>15)')
)

ZToMM = cms.EDFilter("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToMM'),
    roles = cms.vstring('muon1','muon2'),
    decay = cms.string('selectMuons@+ selectMuons@-')
)

bestZmm = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToMM")
)

ZPathMu = cms.Sequence(selectMuons+ZToMM+bestZmm)

