import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),                               
    cut = cms.string("ecalDrivenSeed"
                     " && (abs(superCluster.eta)<2.5) && !(1.4442<abs(superCluster.eta)<1.566)"
                     " && (ecalEnergy*sin(superClusterPosition.theta)>15.0)")
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

