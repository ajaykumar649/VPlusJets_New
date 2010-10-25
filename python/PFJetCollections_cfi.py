import FWCore.ParameterSet.Config as cms

ic5PFJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("iterativeCone5PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt4PFJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("kt4PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt6PFJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("kt6PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

ak5PFJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("ak5PFJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#######################################################
PFJetPath = cms.Sequence(ic5PFJetsClean + kt4PFJetsClean +
                         ak5PFJetsClean)


