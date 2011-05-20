
import FWCore.ParameterSet.Config as cms
isMC = False

process = cms.Process("demo")

##---------  Load standard Reco modules ------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')



##----- this config frament brings you the generator information ----
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("Configuration.StandardSequences.Generator_cff")


##----- Detector geometry : some of these needed for b-tag -------
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")


##----- B-tags --------------
process.load("RecoBTag.Configuration.RecoBTag_cff")


##----- Global tag: conditions database ------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *



############################################
if not isMC:
    process.GlobalTag.globaltag = 'GR_R_41_V0::All'
else:
    process.GlobalTag.globaltag = 'START41_V0::All'

OutputFileName = "demo.root"
numEventsToRun = -1
############################################
########################################################################################
########################################################################################
# Configure to use PF2PAT jets instead of reco::Jets
from PhysicsTools.PatAlgos.tools.pfTools import *
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=False, postfix="")
process.pfPileUp.Enable = True
process.pfPileUp.checkClosestZVertex = cms.bool(False)
process.pfPileUp.Vertices = cms.InputTag('primaryVertex')
process.pfJets.doAreaFastjet = True
process.pfJets.doRhoFastjet = False

########################################################################################
########################################################################################

##---------  Z-->mumu Collection ------------
process.load("ElectroWeakAnalysis.VPlusJets.ZmumuCollections_cfi")

##---------  Jet Collection ----------------
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")

##---------  Vertex and track Collections -----------
process.load("ElectroWeakAnalysis.VPlusJets.TrackCollections_cfi")
#


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/F8AEC745-DF57-E011-8D23-001D09F290BF.root',
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/F625BE01-E057-E011-892F-003048D2C108.root',
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/EEFD6397-E357-E011-978D-001D09F2A465.root',
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/EE67057F-DC57-E011-866E-001617E30D0A.root',
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/EC5B67FF-DF57-E011-8416-003048D2BED6.root',
       '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/EA8C6F9E-D757-E011-9CF4-001D09F295A1.root',
) )


##-------- Muon events of interest --------
process.HLTMu =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring("HLT_Mu5_*", "HLT_Mu7_*", "HLT_Mu9_*", "HLT_Mu11_*", "HLT_Mu15_*", "HLT_Mu24_*"),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
     throw = cms.bool(False) # throw exception on unknown path names
 )




process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis",
    srcPFCor = cms.InputTag("ak5PFJetsCorClean"),
    srcVectorBoson = cms.InputTag("bestZmumu"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('muon'),                          
    runningOverMC = cms.untracked.bool(isMC),                                  
    srcGen  = cms.InputTag("ak5GenJets"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),
    srcPrimaryVertex = cms.InputTag("primaryVertex"),                                    
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('ZJet')                          
)



# Add the KT6 producer to the sequence
getattr(process,"patPF2PATSequence").replace(
    getattr(process,"pfNoElectron"),
    getattr(process,"pfNoElectron")*process.kt6PFJets )


process.myseq = cms.Sequence(
    process.TrackVtxPath *
    getattr(process,"patPF2PATSequence") *     
    process.HLTMu *
    process.ZPath *    
    process.GenJetPath *
    process.impactParameterTagInfos * 
    process.secondaryVertexTagInfos *
    process.TagJetPath *
    process.PFJetPath *
    process.CorPFJetPath 
    )


if isMC:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HLTMu)
else:
    process.myseq.remove ( process.genParticles)
    process.myseq.remove ( process.GenJetPath)
    process.myseq.remove ( process.TagJetPath)


##---- if do not want to require >= 2 jets then disable that filter ---
process.myseq.remove ( process.RequireTwoJets)  

process.outpath.remove(process.out)
process.p = cms.Path( process.myseq * process.VpusJets)










