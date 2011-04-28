import FWCore.ParameterSet.Config as cms

tightMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("pt>20 && isGlobalMuon && isTrackerMuon && abs(eta)<2.4"
                     " && abs(innerTrack().dxy)<1.0"
                     " && globalTrack().normalizedChi2<10"
                     " && globalTrack().hitPattern().numberOfValidTrackerHits>10"
                     " && globalTrack().hitPattern().numberOfValidMuonHits>0"
                     " && globalTrack().hitPattern().numberOfValidPixelHits>0"
                     " && numberOfMatches>1"
                     " && (isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)<0.3*pt"
                     )
)



WToMunu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("tightMuons pfMet"),
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
##     cut = cms.string('daughter(0).pt >20 && daughter(1).pt >20  && sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>40'),
                        
    cut = cms.string('daughter(0).pt >20'),                        
    checkCharge = cms.bool(False),
)



WmunuCounter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(2),
    src = cms.InputTag("WToMunu")                     
)

bestWmunu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(10),
    src = cms.InputTag("WToMunu")
)



##  Define loose electron selection for veto ######
## modified WP95
looseElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
    "ecalDrivenSeed==1 && (abs(superCluster.eta)<2.5)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (ecalEnergy*sin(superClusterPosition.theta)>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits == 0)"
    " && (dr03TkSumPt/p4.Pt <0.2)" 
    " && ((isEB"
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
    " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
    ")"
    " || (isEE"
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )"
    " && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
    "))"
    )
)


looseElectronFilter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(0),
    src = cms.InputTag("looseElectrons")                     
)


##  Define loose muon selection for veto ######
looseMuons = cms.EDFilter("MuonRefSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string("isGlobalMuon && isTrackerMuon && abs(eta)<2.4"
    " && abs(innerTrack().dxy)<1.0 && (isolationR03().sumPt)/(p4.Pt)<0.2")        
)

muonFilter cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1),
    src = cms.InputTag("looseMuons")                     
)


WSequence = cms.Sequence(tightMuons *
                         WToMunu *
                         WmunuCounter *
                         bestWToMunu
                         )
VetoSequence = cms.Sequence( looseElectrons *
                             looseElectronFilter *
                             looseMuons *
                             muonFilter
                             )

WPath = cms.Sequence(WSequence*VetoSequence)



