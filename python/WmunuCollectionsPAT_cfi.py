import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.VPlusJets.WenuCollectionsPAT_cfi import looseElectrons
from ElectroWeakAnalysis.VPlusJets.WenuCollectionsPAT_cfi import looseMuons
isQCD = False

isolationCutString = cms.string("")
if isQCD:
    isolationCutString = "(isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)/pt> 0.1"
else:
    isolationCutString = "(isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)/pt< 0.3"

tightMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("selectedPatMuonsPFlow"),
    cut = cms.string("pt>20 && isGlobalMuon && isTrackerMuon && abs(eta)<2.4"
                     " && globalTrack().normalizedChi2<10"
                     " && innerTrack().numberOfValidHits>10"
                     " && globalTrack().hitPattern().numberOfValidMuonHits>0"
                     " && globalTrack().hitPattern().numberOfValidPixelHits>0"
                     " && numberOfMatches>1"
                     " && " + isolationCutString
                     )
)



WToMunu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("tightMuons patMETsPFlow"),
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
    cut = cms.string('daughter(0).pt >20 && daughter(1).pt >20  && sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>40'), 
    checkCharge = cms.bool(False),
)



bestWmunu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(10),
    src = cms.InputTag("WToMunu")
)


electronFilter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(0),
    src = cms.InputTag("looseElectrons")                     
)


muonFilter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1),
    src = cms.InputTag("looseMuons")                     
)


WSequence = cms.Sequence(tightMuons *
                         WToMunu *
                         bestWmunu
                         )

VetoSequence = cms.Sequence( looseElectrons *
                             electronFilter *
                             looseMuons *
                             muonFilter
                             )

WPath = cms.Sequence(WSequence*VetoSequence)



