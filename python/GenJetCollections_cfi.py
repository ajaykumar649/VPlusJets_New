import FWCore.ParameterSet.Config as cms

ic5GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("iterativeCone5GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

sc5GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone5GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

sc7GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sisCone7GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


kt4GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt4GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt6GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt6GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

# ak5GenJets are NOT there: First load the needed modules
# from RecoJets.Configuration.GenJetParticles_cff import *
# from RecoJets.JetProducers.ak5GenJets_cfi import *

ak5GenJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("antikt5GenJets"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#############################################################

## GenJetPath = cms.Sequence(ic5GenJetsClean + sc5GenJetsClean +
##                           sc7GenJetsClean + kt4GenJetsClean +
##                           kt6GenJetsClean + ak5GenJetsClean)

## GenJetPath = cms.Sequence( ic5GenJetsClean + sc5GenJetsClean +
##                            genParticlesForJets * ak5GenJets *
##                            ak5GenJetsClean)

GenJetPath = cms.Sequence( ic5GenJetsClean + sc5GenJetsClean +
                           ak5GenJetsClean)
