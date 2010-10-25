import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),                               
    cut = cms.string('((abs( caloPosition.eta ) < 1.4442) | (1.560 < abs( caloPosition.eta ) < 2.5)) &  (( caloEnergy * sin( caloPosition.theta ) ) > 20.0)')
)



WToEnu = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string("et>0"),
    checkCharge = cms.bool(False),
    name = cms.string('WToEnu'),
    roles = cms.vstring('electron', 'met'),
    decay = cms.string("selectElectrons met")
)

bestWenu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("WToEnu")
)

WPath = cms.Sequence(selectElectrons+WToEnu+bestWenu)

