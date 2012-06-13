import FWCore.ParameterSet.Config as cms
import pprint
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
    process.GlobalTag.globaltag = 'GR_R_42_V19::All'
else:
    process.GlobalTag.globaltag = 'START42_V13::All'

OutputFileName = "demo.root"
numEventsToRun = -1
############################################
########################################################################################
########################################################################################
# Configure to use PF2PAT jets instead of reco::Jets
from PhysicsTools.PatAlgos.tools.pfTools import *
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=isMC, postfix="")
process.pfPileUp.Enable = True
process.pfPileUp.checkClosestZVertex = cms.bool(False)
process.pfPileUp.Vertices = cms.InputTag('primaryVertex')
process.pfJets.doAreaFastjet = True
process.pfJets.doRhoFastjet = False

########################################################################################
########################################################################################

##---------  W-->munu Collection ------------
process.load("ElectroWeakAnalysis.VPlusJets.WmunuCollections_cfi")

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
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
        '/store/data/Run2011A/SingleMu/AOD/05Aug2011-v1/0000/000D68E1-8DC0-E011-AEFE-00248C55CC40.root',
        '/store/data/Run2011A/SingleMu/AOD/05Aug2011-v1/0000/00223CB6-7AC0-E011-9B40-00261894393E.root',

############ Techni-color samples ############################
##        '/store/user/andersj/Technirho_Wjj_4_2_3_SIM/Technirho_Wjj_4_2_3_AODSIM/f71d043e41acd38c60e3392468355a0e/tc_AODSIM_5_1_vVp.root',
##        '/store/user/andersj/Technirho_Wjj_4_2_3_SIM/Technirho_Wjj_4_2_3_AODSIM/f71d043e41acd38c60e3392468355a0e/tc_AODSIM_4_1_Yg1.root',
    
############ Summer11 W+jets samples ############################
##        '/store/mc/Summer11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0001/FED96BE1-859A-E011-836E-001A92971B56.root',
##        '/store/mc/Summer11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0001/FED7DD5E-9D9A-E011-A3BD-002618943954.root',
) )




##-------- Muon events of interest --------
process.HLTMu =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring('HLT_IsoMu24_*','HLT_IsoMu30_*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
     throw = cms.bool(False) # throw exception on unknown path names
 )





##-------- Save V+jets trees --------
process.VlpusJets = cms.EDAnalyzer("VplusJetsAnalysis", 
    srcPFCor = cms.InputTag("ak5PFJetsCorClean"), 
    srcPFCorVBFTag = cms.InputTag("ak5PFJetsCorCleanVBFTag"), 
    srcVectorBoson = cms.InputTag("bestWmunu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('muon'),                          
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('WJet'),
    srcPrimaryVertex = cms.InputTag("primaryVertex"),                               
    runningOverMC = cms.bool(isMC),			
    srcGen  = cms.InputTag("ak5GenJets"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),                           
)



# Add the KT6 producer to the sequence
getattr(process,"patPF2PATSequence").replace(
    getattr(process,"pfNoElectron"),
    getattr(process,"pfNoElectron")*process.kt6PFJets )


process.myseq = cms.Sequence(
    process.TrackVtxPath * 
    getattr(process,"patPF2PATSequence") *
    process.HLTMu *
    process.WPath *
    process.GenJetPath *
    process.btagging * 
    process.TagJetPath *
    process.PFJetPath *
    process.CorPFJetPath
    )


if isMC:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HLTMu)
else:
    process.myseq.remove ( process.GenJetPath)
    process.myseq.remove ( process.TagJetPath)


##---- if do not want to require >= 2 jets then disable that filter ---
##process.myseq.remove ( process.RequireTwoJets)  

process.outpath.remove(process.out)
process.p = cms.Path( process.myseq  * process.VlpusJets)

