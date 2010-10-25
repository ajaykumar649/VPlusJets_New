import FWCore.ParameterSet.Config as cms

selectMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string('pt > 20 && abs(eta)<2.5')
)



WToMunu = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string("et>0"),
    checkCharge = cms.bool(False),
    name = cms.string('WToMunu'),
    roles = cms.vstring('muon', 'met'),
    decay = cms.string("selectMuons met")
)

bestWmunu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("WToMunu")
)

WPath = cms.Sequence(selectMuons+WToMunu+bestWmunu)

