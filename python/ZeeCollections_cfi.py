import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),                               
    cut = cms.string('((abs( caloPosition.eta ) < 1.4442) | (1.560 < abs( caloPosition.eta ) < 2.5)) &  (( caloEnergy * sin( caloPosition.theta ) ) > 20.0)')
)



ZToEE = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToEE'),
    roles = cms.vstring('electron1', 
        'electron2'),
    decay = cms.string('selectElectrons@+ selectElectrons@-')
)

bestZee = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToEE")
)

ZPath = cms.Sequence(selectElectrons+ZToEE+bestZee)

