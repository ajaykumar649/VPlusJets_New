import FWCore.ParameterSet.Config as cms

# Jet corrections.
# Define the correction services for each algorithm.
# Service
ZSPJetCorrectorIcone5 = cms.ESSource("ZSPJetCorrectionService",
    tagName = cms.vstring('ZSP_CMSSW332_Iterative_Cone_05_PU0'),
    tagNameOffset = cms.vstring(),
    label = cms.string('ZSPJetCorrectorIcone5'),
    PU = cms.int32(-1),
    FixedPU = cms.int32(0)
)
ZSPJetCorrectorSiscone5 = cms.ESSource("ZSPJetCorrectionService",
    tagName = cms.vstring('ZSP_CMSSW332_Iterative_Cone_05_PU0'),
    tagNameOffset = cms.vstring(),
    label = cms.string('ZSPJetCorrectorSiscone5'),
    PU = cms.int32(-1),
    FixedPU = cms.int32(0)
)
ZSPJetCorrectorAntiKt5 = cms.ESSource("ZSPJetCorrectionService",
    tagName = cms.vstring('ZSP_CMSSW332_Iterative_Cone_05_PU0'),
    tagNameOffset = cms.vstring(),
    label = cms.string('ZSPJetCorrectorAntiKt5'),
    PU = cms.int32(-1),
    FixedPU = cms.int32(0)
)

# Modules
#   Define the producers of corrected jet collections for each algorithm.
#
ZSPJetCorJetIcone5 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ZSPJetCorrectorIcone5'),
    alias = cms.untracked.string('ZSPJetCorJetIcone5')
)
ZSPJetCorJetSiscone5 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("sisCone5CaloJets"),
    correctors = cms.vstring('ZSPJetCorrectorSiscone5'),
    alias = cms.untracked.string('ZSPJetCorJetSiscone5')
)
ZSPJetCorJetAntiKt5 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("antikt5CaloJets"),
    correctors = cms.vstring('ZSPJetCorrectorAntiKt5'),
    alias = cms.untracked.string('ZSPJetCorJetAntiKt5')
)


# remove electrons from the jet collection
ic5JPTJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ZSPJetCorJetIcone5"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


sc5JPTJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ZSPJetCorJetSiscone5"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)

ak5JPTJetsClean = cms.EDFilter("JetViewCleaner",
    srcJets = cms.InputTag("ZSPJetCorJetAntiKt5"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)



#
#  Define a sequence to make all corrected jet collections at once.
#
JPTJetPath = cms.Sequence(ZSPJetCorJetIcone5 * ic5JPTJetsClean +
                          ZSPJetCorJetSiscone5 * sc5JPTJetsClean + 
                          ZSPJetCorJetAntiKt5 * ak5JPTJetsClean)

