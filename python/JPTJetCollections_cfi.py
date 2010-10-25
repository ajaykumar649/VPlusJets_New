import FWCore.ParameterSet.Config as cms
from JetMETCorrections.Configuration.DefaultJEC_cff import *
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *

ak5JPTL2Relative.useCondDB = False
ak5JPTL3Absolute.useCondDB = False
ak5JPTResidual.useCondDB = False


# remove electrons from the jet collection
ak5JPTJetsClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


#####################
ak5JPTJetsCorClean = cms.EDProducer("JetViewCleaner",
    srcJets = cms.InputTag("ak5JPTJetsL2L3"),
    module_label = cms.string(""),
    srcObjects = cms.VInputTag(cms.InputTag("gsfElectrons")),
    deltaRMin = cms.double(0.3)
)


#
#  Define a sequence to make all corrected jet collections at once.
#
JPTJetPath = cms.Sequence(ak5JPTJetsClean + ak5JPTJetsL2L3*ak5JPTJetsCorClean)

