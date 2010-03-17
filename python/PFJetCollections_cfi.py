import FWCore.ParameterSet.Config as cms

ic5PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("iterativeCone5PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


sc5PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone5PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

sc7PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone7PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


kt4PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt4PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt6PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt6PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

ak5PFJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("antikt5PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#######################################################

## PFJetPath = cms.Sequence(ic5PFJetsClean + sc5PFJetsClean +
##                            sc7PFJetsClean + kt4PFJetsClean +
##                            kt6PFJetsClean + ak5PFJetsClean)

PFJetPath = cms.Sequence(ic5PFJetsClean + sc5PFJetsClean +
                         ak5PFJetsClean)


