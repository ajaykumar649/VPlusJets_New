import FWCore.ParameterSet.Config as cms


################# Good Electrons 
goodElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("(abs(superCluster.eta)<2.5)"
                     " && !(1.4442<abs(superCluster.eta)<1.566)"
                     " && (gsfTrack.trackerExpectedHitsInner.numberOfHits <= 1)"
                     " && ((isEB"
                     " && ( (dr03TkSumPt + max(0., dr03EcalRecHitSumEt - 1.) + dr03HcalTowerSumEt)/(p4.Pt) < 0.07 )"
                     " && (sigmaIetaIeta<0.01)"
                     " && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
                     " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
                     " && (hadronicOverEm<0.15)"
                     ")"
                     " || (isEE"
                     " && ( (dr03TkSumPt + dr03EcalRecHitSumEt + dr03HcalTowerSumEt)/(p4.Pt) < 0.06 )"
                     " && (sigmaIetaIeta<0.03)"
                     " && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )"
                     " && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
                     " && (hadronicOverEm<0.07) "
                     "))"
                     )    
)
################# Good Muons
goodMuons = cms.EDFilter("MuonRefSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string('isGlobalMuon && isTrackerMuon && abs(eta)<2.5 && abs(innerTrack().dxy)<1.0 && isolationR03().sumPt<3.0')        
)


##################### Clean CaloJets
ic5CaloJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("iterativeCone5CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)

kt4CaloJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("kt4CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)

kt6CaloJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("kt6CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)

ak5CaloJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("ak5CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)


############################################
CaloJetPath = cms.Sequence(goodElectrons +
                           goodMuons +
                           ic5CaloJetsClean +
                           kt4CaloJetsClean +
                           ak5CaloJetsClean)
