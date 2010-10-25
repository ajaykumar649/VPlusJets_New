import FWCore.ParameterSet.Config as cms

selectMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string('isGlobalMuon && pt > 20 && abs(eta)<2.5'),
     filter = cms.bool(True)                      
)



ZToMM = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToMM'),
    roles = cms.vstring('muon1', 'muon2'),
    decay = cms.string('selectMuons@+ selectMuons@-'),
   checkCharge = cms.bool(True)                    
)

bestZmumu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToMM")
)

ZPath = cms.Sequence(selectMuons*ZToMM*bestZmumu)

