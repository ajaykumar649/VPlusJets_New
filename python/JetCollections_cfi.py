import FWCore.ParameterSet.Config as cms

mcFlag = False
                 

print "################### Reading Jet Collections ##################"
print "isMC = ",
if mcFlag:
    print "Yup"
else:
    print "Nope"
print "##############################################################"

from ElectroWeakAnalysis.VPlusJets.WenuCollections_cfi import looseElectrons
from ElectroWeakAnalysis.VPlusJets.WenuCollections_cfi import looseMuons

##########################################################################
##################### Clean CaloJets
ic5CaloJetsClean = cms.EDProducer("CaloJetCleaner",
    srcJets = cms.InputTag("iterativeCone5CaloJets"),
    module_label = cms.string(""),
    idLevel = cms.int32(0),
    etaMax  =  cms.double(-2.4),
    ptMin   =  cms.double(0.0),                    
    srcObjects = cms.VInputTag(cms.InputTag("looseElectrons"),cms.InputTag("looseMuons")),
    deltaRMin = cms.double(0.3)
)
kt4CaloJetsClean = ic5CaloJetsClean.clone()
kt4CaloJetsClean.srcJets = cms.InputTag("kt4CaloJets")

kt6CaloJetsClean = ic5CaloJetsClean.clone()
kt6CaloJetsClean.srcJets = cms.InputTag("kt6CaloJets")

ak5CaloJetsClean = ic5CaloJetsClean.clone()
ak5CaloJetsClean.srcJets = cms.InputTag("ak5CaloJets")
############################################
CaloJetPath = cms.Sequence(looseElectrons +
                           looseMuons +
##                            ic5CaloJetsClean +
##                            kt4CaloJetsClean +
                           ak5CaloJetsClean)
##########################################################################
##########################################################################
##################### Corrected CaloJets
from JetMETCorrections.Configuration.DefaultJEC_cff import *
if mcFlag:
    ic5CaloJetsCor = ic5CaloJetsL2L3.clone()
    kt4CaloJetsCor = kt4CaloJetsL2L3.clone()
    kt6CaloJetsCor = kt6CaloJetsL2L3.clone()
    ak5CaloJetsCor = ak5CaloJetsL2L3.clone()
else:
    ic5CaloJetsCor = ic5CaloJetsL2L3Residual.clone()
    kt4CaloJetsCor = kt4CaloJetsL2L3Residual.clone()
    kt6CaloJetsCor = kt6CaloJetsL2L3Residual.clone()
    ak5CaloJetsCor = ak5CaloJetsL2L3Residual.clone()
    

ic5CaloJetsCor.src = "ic5CaloJetsClean"
ic5CaloJetsCorClean = cms.EDFilter("CaloJetSelector",  
    src = cms.InputTag("ic5CaloJetsCor"),
    cut = cms.string('pt > 0')
)

kt4CaloJetsCor.src = "kt4CaloJetsClean"
kt4CaloJetsCorClean = ic5CaloJetsCorClean.clone() 
kt4CaloJetsCorClean.src = cms.InputTag("kt4CaloJetsCor")

kt6CaloJetsCor.src = "kt6CaloJetsClean"
kt6CaloJetsCorClean = ic5CaloJetsCorClean.clone() 
kt6CaloJetsCorClean.src = cms.InputTag("kt6CaloJetsCor")


ak5CaloJetsCor.src = "ak5CaloJetsClean"
ak5CaloJetsCorClean = ic5CaloJetsCorClean.clone() 
ak5CaloJetsCorClean.src = cms.InputTag("ak5CaloJetsCor")
##########################################
CorJetPath = cms.Sequence(
    looseElectrons +
    looseMuons +
##     ic5CaloJetsL2L3 +    
##     ic5CaloJetsL2L3Residual +
##     ic5CaloJetsClean +
##     ic5CaloJetsCor +
##     ic5CaloJetsCorClean +    
##     kt4CaloJetsL2L3 +    
##     kt4CaloJetsL2L3Residual +
##     kt4CaloJetsClean +
##     kt4CaloJetsCor +
##     kt4CaloJetsCorClean +    
    ak5CaloJetsL2L3 +    
    ak5CaloJetsL2L3Residual +
    ak5CaloJetsClean +
    ak5CaloJetsCor +
    ak5CaloJetsCorClean
    )
if mcFlag:
    CorJetPath.remove (ak5CaloJetsL2L3Residual )
##########################################################################
##################### Clean PFJets
ic5PFJetsClean = cms.EDProducer("PFJetCleaner",
    srcJets = cms.InputTag("iterativeCone5PFJets"),
    module_label = cms.string(""),
    idLevel = cms.int32(1),
    etaMax  =  cms.double(2.4),
    ptMin   =  cms.double(0.0),                                 
    srcObjects = cms.VInputTag(cms.InputTag("looseElectrons"),cms.InputTag("looseMuons")),
    deltaRMin = cms.double(0.3)
)
kt4PFJetsClean = ic5PFJetsClean.clone()
kt4PFJetsClean.srcJets = cms.InputTag("kt4PFJets")

kt6PFJetsClean = ic5PFJetsClean.clone()
kt6PFJetsClean.srcJets = cms.InputTag("kt6PFJets")


##-------------------- Import the Jet RECO modules -----------------------
##-------------------- Turn-on the FastJet density calculation -----------------------
from RecoJets.JetProducers.kt4PFJets_cfi import *
kt6PFJets = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJets.Rho_EtaMax = cms.double(4.5)

# to compute FastJet rho to correct isolation (note: EtaMax restricted to 2.5)
kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

##--------- Turn-on the FastJet jet area calculation for your favorite algorithm ----
from RecoJets.JetProducers.ak5PFJets_cfi import *
ak5PFJets.Rho_EtaMax = cms.double(4.5)
ak5PFJets.doAreaFastjet = True
##########################################################################
##-------- Remove electrons and muons from jet collection ----------------------

ak5PFJetsClean = ic5PFJetsClean.clone()
ak5PFJetsClean.srcJets = cms.InputTag("ak5PFJets")

##-------- Apply JetId ----------------------
ak5PFJetsLooseId = cms.EDFilter("PFJetSelector",
    src     = cms.InputTag( "ak5PFJetsClean" ),                                     
    cut = cms.string(
    "neutralHadronEnergyFraction<0.99"
    " && neutralEmEnergyFraction<0.99"
    " && chargedMultiplicity>0"
    " && nConstituents>1"
    " && chargedHadronEnergyFraction>0.0"
    " && chargedEmEnergyFraction<0.99"
    )
)
############################################
PFJetPath = cms.Sequence(
    looseElectrons +
    looseMuons +
##     ic5PFJetsClean +
##     kt4PFJetsClean +
    kt6PFJets +
    kt6PFJetsForIsolation +
    ak5PFJets +
    ak5PFJetsClean +
    ak5PFJetsLooseId 
    )
##################### Corrected PFJets
ak5PFL1Fastjet.srcRho = cms.InputTag('kt6PFJets', 'rho')
#if mcFlag:
ic5PFJetsCor = ic5PFJetsL1FastL2L3.clone()
kt4PFJetsCor = kt4PFJetsL1FastL2L3.clone()
kt6PFJetsCor = kt6PFJetsL1FastL2L3.clone()
ak5PFJetsCor = ak5PFJetsL1FastL2L3.clone()
## else:
##     ic5PFJetsCor = ic5PFJetsL1FastL2L3Residual.clone()
##     kt4PFJetsCor = kt4PFJetsL1FastL2L3Residual.clone()
##     kt6PFJetsCor = kt6PFJetsL1FastL2L3Residual.clone()
##     ak5PFJetsCor = ak5PFJetsL1FastL2L3Residual.clone()
    

ic5PFJetsCor.src = "ic5PFJetsClean"
ic5PFJetsCorClean = cms.EDFilter("PtMinPFJetSelector",  
    src = cms.InputTag("ic5PFJetsCor"),
    ptMin = cms.double(20.0)
)
kt4PFJetsCor.src = "kt4PFJetsClean"
kt4PFJetsCorClean = ic5PFJetsCorClean.clone()
kt4PFJetsCorClean.src = cms.InputTag("kt4PFJetsCor")

kt6PFJetsCor.src = "kt6PFJetsClean"
kt6PFJetsCorClean = ic5PFJetsCorClean.clone()
kt6PFJetsCorClean.src = cms.InputTag("kt6PFJetsCor")

ak5PFJetsCor.src = "ak5PFJetsLooseId"
ak5PFJetsCorClean = ic5PFJetsCorClean.clone()
ak5PFJetsCorClean.src = cms.InputTag("ak5PFJetsCor")
##########################################
## Filter to require at least two jets in the event
RequireTwoJets = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("ak5PFJetsCorClean"),                      
)

CorPFJetPath = cms.Sequence(
    looseElectrons +
    looseMuons + 
    ak5PFJetsClean +
##     ic5PFJetsL2L3 +    
##     ic5PFJetsL2L3Residual +
##     ic5PFJetsClean +
##     ic5PFJetsCor +
##     ic5PFJetsCorClean +    
##     kt4PFJetsL2L3 +    
##     kt4PFJetsL2L3Residual +
##     kt4PFJetsClean +
##     kt4PFJetsCor +
##     kt4PFJetsCorClean +
    ak5PFJetsL2L3 +
    ak5PFJetsL1FastL2L3 +    
##     ak5PFJetsL1FastL2L3Residual +
    ak5PFJetsCor +
    ak5PFJetsCorClean +
    RequireTwoJets
    )
## if mcFlag:
##     CorPFJetPath.remove( ak5PFJetsL1FastL2L3Residual )
##########################################################################
##########################################################################
##################### Cleaned JPTJets
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
ak5JPTJetsClean = cms.EDProducer("JPTJetCleaner",
    srcJets = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    module_label = cms.string(""),
    idLevel = cms.int32(0),
    etaMax  =  cms.double(2.4),
    ptMin   =  cms.double(0.0),
    srcObjects = cms.VInputTag(cms.InputTag("looseElectrons"),cms.InputTag("looseMuons")),
    deltaRMin = cms.double(0.3)
)
##################### Corrected JPTJets
if mcFlag:
    ak5JPTJetsCor = ak5JPTJetsL2L3.clone()    
else:
    ak5JPTJetsCor = ak5JPTJetsL2L3Residual.clone()
    
ak5JPTJetsCor.src = "ak5JPTJetsClean"
ak5JPTJetsCorClean = ak5JPTJetsCor.clone()
ak5JPTJetsCorClean = cms.EDFilter("JPTJetSelector",
    src = cms.InputTag("ak5JPTJetsCor"),
    cut = cms.string('pt > 0')    
)
##########################################
JPTJetPath = cms.Sequence(
    looseElectrons +
    looseMuons +
    ak5JPTJetsClean +
    ak5JPTJetsL2L3 +
    ak5JPTJetsL2L3Residual +
    ak5JPTJetsCor +
    ak5JPTJetsCorClean
    )
if mcFlag:
    JPTJetPath.remove( ak5JPTJetsL2L3Residual )
##########################################################################
#############  Jets in Monte Carlo  #############
##########################################################################
# ak5GenJets are NOT there: First load the needed modules
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.JetProducers.ak5GenJets_cfi import *
GenJetPath = cms.Sequence( genParticlesForJets + ak5GenJets )


##################### Tag jets: Needed for MC flavor matching
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
TagJetPath = cms.Sequence(
    myPartons + 
##     ic5flavourByRef*ic5tagJet +
##     kt4flavourByRef*kt4tagJet +
    ak5flavourByRef*ak5tagJet
    ) 
#############################################
#############################################
