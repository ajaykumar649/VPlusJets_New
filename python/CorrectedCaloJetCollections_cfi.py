import FWCore.ParameterSet.Config as cms
from JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_cff import *

ic5CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetIC5Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


ic5CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ic5CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#####################
sc5CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetSC5Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


sc5CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sc5CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#####################

sc7CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetSC7Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

sc7CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("sc7CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#####################

kt4CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetKT4Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


kt4CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt4CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#####################

kt6CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetKT6Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

kt6CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("kt6CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

#####################

ak5CaloJetsCor = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("L2L3CorJetAK5Calo"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

ak5CaloJetsCorClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ak5CaloJetsCor"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

##########################################
##########################################

## CorJetPath = cms.Sequence(
##     L2L3CorJetIC5Calo * ic5CaloJetsCor * ic5CaloJetsCorClean +
##     L2L3CorJetSC5Calo * sc5CaloJetsCor * sc5CaloJetsCorClean +
##     L2L3CorJetSC7Calo * sc7CaloJetsCor * sc7CaloJetsCorClean +
##     L2L3CorJetKT4Calo * kt4CaloJetsCor * kt4CaloJetsCorClean +
##     L2L3CorJetKT6Calo * kt6CaloJetsCor * kt6CaloJetsCorClean +
##     L2L3CorJetAK5Calo * ak5CaloJetsCor * ak5CaloJetsCorClean
##     )


CorJetPath = cms.Sequence(
    L2L3CorJetIC5Calo * ic5CaloJetsCor * ic5CaloJetsCorClean +
    L2L3CorJetSC5Calo * sc5CaloJetsCor * sc5CaloJetsCorClean +
    L2L3CorJetAK5Calo * ak5CaloJetsCor * ak5CaloJetsCorClean
    )

