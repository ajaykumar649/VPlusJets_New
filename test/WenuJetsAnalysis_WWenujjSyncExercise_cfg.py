import FWCore.ParameterSet.Config as cms
import pprint
isMC = True

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("Configuration.StandardSequences.Generator_cff")
process.load("RecoBTag.Configuration.RecoBTag_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")


# this config frament brings you the generator information
process.load("Configuration.StandardSequences.Generator_cff")


process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

############################################
if not isMC:
    process.GlobalTag.globaltag = 'GR_R_39X_V5::All'
else:
    process.GlobalTag.globaltag = 'START39_V8::All'

OutputFileName = "demo.root"
numEventsToRun = -1
#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 10



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
      '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/ACDEDA3C-B7D3-DF11-A7A1-0030487C6A66.root',
      '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/223CD93D-B7D3-DF11-885E-0030487CD7B4.root',
      '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/EAB3E588-B6D3-DF11-8BDC-0030487A3232.root',
      '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/AA0C5537-B7D3-DF11-9194-0030487CD7C6.root',

##        '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/FE9CF7A8-5A2D-E011-9775-0030486790B0.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F2EDDFEA-592D-E011-97B7-0030486791F2.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F0ECB760-5A2D-E011-8CF0-002618943811.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E8927780-5A2D-E011-AEB1-00304867BEE4.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E0D2C67B-5A2D-E011-BA9A-002618943833.root',
       
       ) )




############################################
#  Wenu Collection ##########
############################################
#WP80 electrons, only track iso
process.selectElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
    "(ecalDrivenSeed==1) && (abs(superCluster.eta)<2.5)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (et>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits==0 && !(-0.02<convDist<0.02 && -0.02<convDcot<0.02))"
    " && (dr03TkSumPt/p4.Pt <0.1)"    
    " && ((isEB"
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.06<deltaPhiSuperClusterTrackAtVtx<0.06 )"
    " && ( -0.004<deltaEtaSuperClusterTrackAtVtx<0.004 )"
    " && (hadronicOverEm<0.040)"
    ")"
    " || (isEE"
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.03<deltaPhiSuperClusterTrackAtVtx<0.03 )"
    " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
    " && (hadronicOverEm<0.025) "
    "))"
    )
)

process.WToEnu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("selectElectrons pfMet"),
    cut = cms.string('daughter(0).pt >20'),
    checkCharge = cms.bool(False),
)
process.bestWToEnu =cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(10),
    src = cms.InputTag("WToEnu")                 
)
####################################
############################################
#  Jet Collection ##########
############################################
process.ak5PFJetsClean = cms.EDProducer("PFJetCleaner",
    srcJets = cms.InputTag("ak5PFJets"),
    module_label = cms.string(""),
    idLevel = cms.int32(1),
    etaMax  =  cms.double(2.4),
    ptMin   =  cms.double(0.0),                                 
    srcObjects = cms.VInputTag(cms.InputTag("selectElectrons")),
    deltaRMin = cms.double(0.3)
)
##
process.ak5PFJetsLooseId = cms.EDFilter("PFJetSelector",
    src     = cms.InputTag( "ak5PFJetsClean" ),                                     
    cut = cms.string("neutralHadronEnergyFraction<0.99 && neutralEmEnergyFraction<0.99 && chargedMultiplicity>0 && nConstituents>1 && chargedHadronEnergyFraction>0.0 && chargedEmEnergyFraction<0.99")
)
##
## from JetMETCorrections.Configuration.DefaultJEC_cff import *
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")
if isMC:
    process.ak5PFJetsCor = process.ak5PFJetsL2L3.clone()
else:
    process.ak5PFJetsCor = process.ak5PFJetsL2L3Residual.clone()
    
process.ak5PFJetsCor.src = "ak5PFJetsLooseId"
process.ak5PFJetsCorClean = cms.EDFilter("PtMinPFJetSelector",  
    src = cms.InputTag("ak5PFJetsCor"),
    ptMin = cms.double(20.)
)
##

process.RequireTwoJets = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("ak5PFJetsCorClean"),                      
)
# ak5GenJets are NOT there: First load the needed modules
## from RecoJets.Configuration.GenJetParticles_cff import *
## from RecoJets.JetProducers.ak5GenJets_cfi import *
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
##################### Tag jets: Needed for MC flavor matching
process.myPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)
process.ak5flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5PFJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

process.ak5tagJet = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("ak5flavourByRef"),
    physicsDefinition = cms.bool(False)
)
####################################
############################################
#  V+jets Collection ##########
############################################
process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis", 
    srcPFCor = cms.InputTag("ak5PFJetsCorClean"), 
    srcVectorBoson = cms.InputTag("bestWToEnu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('electron'),                          
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('WJet'),
    runningOverMC = cms.bool(False),			
    #srcGen  = cms.InputTag("ak5GenJets"),
    #srcFlavorByValue = cms.InputTag("ak5tagJet"),                           
)



process.myseq = cms.Sequence(
    #process.genParticles *
    process.impactParameterTagInfos * 
    process.secondaryVertexTagInfos *
    process.selectElectrons *
    process.WToEnu *
    process.bestWToEnu *
    process.ak5PFJetsClean * 
    process.ak5PFJetsL2L3 *   
    process.ak5PFJetsL2L3Residual *
    process.ak5PFJetsClean *
    process.ak5PFJetsLooseId * 
    process.ak5PFJetsCor *
    process.ak5PFJetsCorClean *
    process.RequireTwoJets *
    #process.genParticlesForJets *
    #process.ak5GenJets *
    #process.myPartons *
    #process.ak5flavourByRef *
    #process.ak5tagJet *
    process.VpusJets
    )
#if not isMC:
    #process.myseq.remove ( process.genParticles)
    #process.myseq.remove ( process.genParticlesForJets)
    #process.myseq.remove ( process.ak5GenJets)    
    #process.myseq.remove ( process.myPartons)
    #process.myseq.remove ( process.ak5flavourByRef)
    #process.myseq.remove ( process.ak5tagJet)
    

process.p = cms.Path( process.myseq )






