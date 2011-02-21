import FWCore.ParameterSet.Config as cms

mcFlag = False
                 

print "################### Reading Jet Collections ##################"
print "isMC = ",
if mcFlag:
    print "Yup"
else:
    print "Nope"
print "##############################################################"

################# Good Electrons 
goodElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("(abs(superCluster.eta)<2.5)"
                     " && !(1.4442<abs(superCluster.eta)<1.566)"
                     " && (gsfTrack.trackerExpectedHitsInner.numberOfHits ==0)"
                     " && ((isEB"
                     " && ( (dr03TkSumPt + max(0., dr03EcalRecHitSumEt - 1.) + dr03HcalTowerSumEt)/(p4.Pt) < 0.07 )"
                     " && (sigmaIetaIeta<0.01)"
                     " && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
                     " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
                     " && (hadronicOverEm<0.15)"
                     ")"
                     " || (isEE"
                     " && ( (dr03TkSumPt + dr03EcalRecHitSumEt + dr03HcalTowerSumEt)/(p4.Pt) < 0.06 )"
                     " && (sigmaIetaIeta<0.03)"
                     " && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )"
                     " && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
                     " && (hadronicOverEm<0.07) "
                     "))"
                     )    
)
################# Good Muons
goodMuons = cms.EDFilter("MuonRefSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string("isGlobalMuon && isTrackerMuon"
    " && abs(eta)<2.5 && abs(innerTrack().dxy)<1.0 && isolationR03().sumPt<3.0")        
)
##################### Clean CaloJets
ic5CaloJetsClean = cms.EDProducer("CaloJetCleaner",
    srcJets = cms.InputTag("iterativeCone5CaloJets"),
    module_label = cms.string(""),
    idLevel = cms.int32(0),
    etaMax  =  cms.double(-2.4),
    ptMin   =  cms.double(0.0),                    
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)
kt4CaloJetsClean = ic5CaloJetsClean.clone()
kt4CaloJetsClean.srcJets = cms.InputTag("kt4CaloJets")

kt6CaloJetsClean = ic5CaloJetsClean.clone()
kt6CaloJetsClean.srcJets = cms.InputTag("kt6CaloJets")

ak5CaloJetsClean = ic5CaloJetsClean.clone()
ak5CaloJetsClean.srcJets = cms.InputTag("ak5CaloJets")
############################################
CaloJetPath = cms.Sequence(goodElectrons +
                           goodMuons +
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
    cut = cms.string('pt > 20')
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
    goodElectrons +
    goodMuons +
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
##########################################################################
##################### Clean PFJets
ic5PFJetsClean = cms.EDProducer("PFJetCleaner",
    srcJets = cms.InputTag("iterativeCone5PFJets"),
    module_label = cms.string(""),
    idLevel = cms.int32(1),
    etaMax  =  cms.double(2.4),
    ptMin   =  cms.double(0.0),                                 
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
    deltaRMin = cms.double(0.3)
)
kt4PFJetsClean = ic5PFJetsClean.clone()
kt4PFJetsClean.srcJets = cms.InputTag("kt4PFJets")

kt6PFJetsClean = ic5PFJetsClean.clone()
kt6PFJetsClean.srcJets = cms.InputTag("kt6PFJets")

ak5PFJetsClean = ic5PFJetsClean.clone()
ak5PFJetsClean.srcJets = cms.InputTag("ak5PFJets")
############################################
PFJetPath = cms.Sequence(
    goodElectrons +
    goodMuons +
##     ic5PFJetsClean +
##     kt4PFJetsClean +
    ak5PFJetsClean
    )
##################### Corrected PFJets
if mcFlag:
    ic5PFJetsCor = ic5PFJetsL2L3.clone()
    kt4PFJetsCor = kt4PFJetsL2L3.clone()
    kt6PFJetsCor = kt6PFJetsL2L3.clone()
    ak5PFJetsCor = ak5PFJetsL2L3.clone()
else:
    ic5PFJetsCor = ic5PFJetsL2L3Residual.clone()
    kt4PFJetsCor = kt4PFJetsL2L3Residual.clone()
    kt6PFJetsCor = kt6PFJetsL2L3Residual.clone()
    ak5PFJetsCor = ak5PFJetsL2L3Residual.clone()
    

ic5PFJetsCor.src = "ic5PFJetsClean"
ic5PFJetsCorClean = cms.EDFilter("PtMinPFJetSelector",  
    src = cms.InputTag("ic5PFJetsCor"),
    ptMin = cms.double(20)
)
kt4PFJetsCor.src = "kt4PFJetsClean"
kt4PFJetsCorClean = ic5PFJetsCorClean.clone()
kt4PFJetsCorClean.src = cms.InputTag("kt4PFJetsCor")

kt6PFJetsCor.src = "kt6PFJetsClean"
kt6PFJetsCorClean = ic5PFJetsCorClean.clone()
kt6PFJetsCorClean.src = cms.InputTag("kt6PFJetsCor")

ak5PFJetsCor.src = "ak5PFJetsClean"
ak5PFJetsCorClean = ic5PFJetsCorClean.clone()
ak5PFJetsCorClean.src = cms.InputTag("ak5PFJetsCor")
##########################################
CorPFJetPath = cms.Sequence(
    goodElectrons +
    goodMuons +
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
    ak5PFJetsL2L3Residual +
    ak5PFJetsClean +
    ak5PFJetsCor +
    ak5PFJetsCorClean
    )
if mcFlag:
    CorPFJetPath.remove( ak5PFJetsL2L3Residual )
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
    srcObjects = cms.VInputTag(cms.InputTag("goodElectrons"),cms.InputTag("goodMuons")),
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
    cut = cms.string('pt > 20')    
)
##########################################
JPTJetPath = cms.Sequence(
    goodElectrons +
    goodMuons +
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
