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
    process.GlobalTag.globaltag = 'START311_V2A::All'

OutputFileName = "demo.root"
numEventsToRun = -1
#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 10



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0026/FE40206F-2453-E011-A011-003048F0E1EE.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0026/D62114D9-F952-E011-A486-003048F0E3AE.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0026/6A4D222D-EC52-E011-8021-003048F0E5A2.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0026/1C119FC9-EE52-E011-86CB-0030487F938B.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/F4AE4A60-E652-E011-B495-0030487F92FF.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/C40F587B-E752-E011-A002-0030487F9351.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/BA617B42-E752-E011-8F4B-0030487F9307.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/B80E704E-E652-E011-A6FC-0030487FA629.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/90AC4971-E752-E011-881A-0030487F932D.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/78C05C44-E752-E011-95E5-0030487F9307.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/50E94B31-E752-E011-8FD3-0030487F91DB.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/50CEAD5E-E652-E011-8BC5-0030487F92FF.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/30551D41-E752-E011-AA16-0030487F929B.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/2C6C1D71-E552-E011-B975-002481E945C4.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/1E1D17C5-E552-E011-880C-003048D4DFA8.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/083A7E32-E752-E011-9C7B-0030487F91DB.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0025/069C7D32-E752-E011-B3BD-0030487F91DB.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0024/EAA84FDA-C752-E011-9119-0030487E52A5.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0024/64F298DB-DC52-E011-B579-0030487F9367.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0024/5CF7BEDB-DC52-E011-8612-0030487F933D.root',
       '/store/mc/Spring11/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/AODSIM/PU_S1_START311_V1G1-v1/0024/527222CB-DC52-E011-A990-0030487FA625.root'
    
##       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/ACDEDA3C-B7D3-DF11-A7A1-0030487C6A66.root',
##       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/223CD93D-B7D3-DF11-885E-0030487CD7B4.root',
##       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/EAB3E588-B6D3-DF11-8BDC-0030487A3232.root',
##       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/AA0C5537-B7D3-DF11-9194-0030487CD7C6.root',
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






