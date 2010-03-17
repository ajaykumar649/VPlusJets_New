import FWCore.ParameterSet.Config as cms

ic5CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("iterativeCone5CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


sc5CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone5CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

sc7CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone7CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


kt4CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt4CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt6CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt6CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

ak5CaloJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ak5CaloJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

############################################

## CaloJetPath = cms.Sequence(ic5CaloJetsClean + sc5CaloJetsClean +
##                            sc7CaloJetsClean + kt4CaloJetsClean +
##                            kt6CaloJetsClean + ak5CaloJetsClean)

CaloJetPath = cms.Sequence(ic5CaloJetsClean + sc5CaloJetsClean +
                           ak5CaloJetsClean)
