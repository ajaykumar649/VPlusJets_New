import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
    "ecalDrivenSeed==1 && (abs(superCluster.eta)<2.5)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (ecalEnergy*sin(superClusterPosition.theta)>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits==0 && !(-0.02<convDist<0.02 && -0.02<convDcot<0.02))"
    " && ((isEB"
    " && ( dr03TkSumPt/p4.Pt <0.09 && dr03EcalRecHitSumEt/p4.Pt < 0.08 && dr03HcalTowerSumEt/p4.Pt  < 0.1 )"
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.6<deltaPhiSuperClusterTrackAtVtx<0.6 )"
    " && ( -0.006<deltaEtaSuperClusterTrackAtVtx<0.006 )"
    " && (hadronicOverEm<0.04)"
    ")"
    " || (isEE"
    " && ( dr03TkSumPt/p4.Pt <0.05 && dr03EcalRecHitSumEt/p4.Pt < 0.05 && dr03HcalTowerSumEt/p4.Pt  < 0.025 )"
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.04<deltaPhiSuperClusterTrackAtVtx<0.04 )"
    " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
    " && (hadronicOverEm<0.025) "
    "))"
    )
)







WToEnu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("selectElectrons pfMet"),
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
    cut = cms.string('daughter(1).pt >20  && sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>40'),
    checkCharge = cms.bool(False),
)

bestWenu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("WToEnu")
)





##  Define Z->ee candidate selection for veto ######
WP95Electrons = cms.EDFilter("GsfElectronRefSelector",
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


Zee = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("WP95Electrons WP95Electrons"), # charge coniugate states are implied
    checkCharge = cms.bool(False),                           
    cut   = cms.string("80 < mass < 100"),
)


ZeeLargestPt = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("Zee"),
    filter = cms.bool(True),
    minN    = cms.int32(1)                          
)






WSequence = cms.Sequence(selectElectrons+WToEnu+bestWenu)
ZvetoSequence = cms.Sequence( WP95Electrons* Zee * ~ZeeLargestPt)



WPath = cms.Sequence(WSequence*ZvetoSequence)

