import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.VPlusJets.CorrectedCaloJetCollections_cfi import *

myPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)

###############
ic5flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("iterativeCone5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

ic5tagJet = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("ic5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
kt4flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("kt4CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

kt4tagJet = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("kt4flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
kt6flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("kt6CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

kt6tagJet = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("kt6flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
ak5flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

ak5tagJet = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("ak5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
#############################################
#############################################
TagJetPath = cms.Sequence(myPartons * 
                          (ic5flavourByRef*ic5tagJet +
                          kt4flavourByRef*kt4tagJet +
                          ak5flavourByRef*ak5tagJet ) )
