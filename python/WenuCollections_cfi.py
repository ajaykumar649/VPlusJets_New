import FWCore.ParameterSet.Config as cms

#WP70 electrons
selectElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
    "ecalDrivenSeed==1 && (abs(superCluster.eta)<2.5)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (ecalEnergy*sin(superClusterPosition.theta)>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits==0 && !(-0.02<convDist<0.02 && -0.02<convDcot<0.02))"
    " && ((isEB"
    " && ( dr03TkSumPt/p4.Pt <0.05 && dr03EcalRecHitSumEt/p4.Pt < 0.06 && dr03HcalTowerSumEt/p4.Pt  < 0.03 )"
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.03<deltaPhiSuperClusterTrackAtVtx<0.03 )"
    " && ( -0.004<deltaEtaSuperClusterTrackAtVtx<0.004 )"
    " && (hadronicOverEm<0.025)"
    ")"
    " || (isEE"
    " && ( dr03TkSumPt/p4.Pt <0.025 && dr03EcalRecHitSumEt/p4.Pt < 0.025 && dr03HcalTowerSumEt/p4.Pt  < 0.02 )"
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.02<deltaPhiSuperClusterTrackAtVtx<0.02 )"
    " && ( -0.005<deltaEtaSuperClusterTrackAtVtx<0.005 )"
    " && (hadronicOverEm<0.025) "
    "))"
    )
)





WToEnu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("selectElectrons pfMet"),
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
    cut = cms.string('daughter(0).pt >25 && daughter(1).pt >20  && sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>40'),
    checkCharge = cms.bool(False),
)





WenuCounter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(2),
    src = cms.InputTag("WToEnu")                     
)


bestWToEnu =cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(10),
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
    maxNumber = cms.uint32(10),
    src = cms.InputTag("Zee"),
    filter = cms.bool(True)                      
)






WSequence = cms.Sequence(selectElectrons*WToEnu*WenuCounter*bestWToEnu)
ZvetoSequence = cms.Sequence( WP95Electrons* Zee * ~ZeeLargestPt)



WPath = cms.Sequence(WSequence*ZvetoSequence)

