import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.VPlusJets.CorrectedCaloJetCollections_cfi import *

myPartons = cms.EDFilter("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)

###############
ic5flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("iterativeCone5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

ic5tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("ic5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
sc5flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("sisCone5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

sc5tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("sc5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
sc7flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("sisCone7CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

sc7tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("sc7flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############

kt4flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("kt4CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

kt4tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("kt4flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
kt6flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("kt6CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

kt6tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("kt6flavourByRef"),
    physicsDefinition = cms.bool(False)
)
###############
ak5flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("ak5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

ak5tagJet = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("ak5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
#############################################
#############################################


## TagJetPath = cms.Sequence(myPartons * 
##                           (ic5flavourByRef*ic5tagJet +
##                           sc5flavourByRef*sc5tagJet +
##                           sc7flavourByRef*sc7tagJet +
##                           kt4flavourByRef*kt4tagJet +
##                           kt6flavourByRef*kt6tagJet +
##                           ak5flavourByRef*ak5tagJet ) )

TagJetPath = cms.Sequence(myPartons * 
                          (ic5flavourByRef*ic5tagJet +
                          sc5flavourByRef*sc5tagJet +
                          ak5flavourByRef*ak5tagJet ) )
