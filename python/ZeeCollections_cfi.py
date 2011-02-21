import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
    "ecalDrivenSeed==1 && (abs(superCluster.eta)<2.5)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (ecalEnergy*sin(superClusterPosition.theta)>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits <= 1)"
    " && ((isEB"
    " && ( dr03TkSumPt/p4.Pt < 0.15 && dr03EcalRecHitSumEt/p4.Pt < 2.0 && dr03HcalTowerSumEt/p4.Pt < 0.12 )" 
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
    " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
    " && (hadronicOverEm<0.15)"
    ")"
    " || (isEE"
    " && (dr03TkSumPt/p4.Pt < 0.08 && dr03EcalRecHitSumEt/p4.Pt < 0.06  && dr03HcalTowerSumEt/p4.Pt < 0.05 )"  
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )"
    " && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
    " && (hadronicOverEm<0.07) "
    "))"
    )
)



ZToEE = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToEE'),
    roles = cms.vstring('electron1', 'electron2'),
    decay = cms.string('selectElectrons@+ selectElectrons@-')
)

bestZee = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToEE")
)

ZPath = cms.Sequence(selectElectrons+ZToEE+bestZee)

