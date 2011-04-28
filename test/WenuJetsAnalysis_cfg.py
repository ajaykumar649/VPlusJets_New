import FWCore.ParameterSet.Config as cms
import pprint
isMC = False

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
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
    process.GlobalTag.globaltag = 'GR_R_311_V2::All'
else:
    process.GlobalTag.globaltag = 'START311_V2A::All'

OutputFileName = "demo.root"
numEventsToRun = -1
############################################
#   W-->enu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.WenuCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")
#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/312/90646AF9-F957-E011-B0DB-003048F118C4.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/312/749B54BD-E557-E011-AC27-000423D33970.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/312/5A366F5F-7959-E011-AB12-0030487C8E02.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/312/0EF76BBA-0858-E011-AE3A-003048F118C6.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/312/024D65F9-F957-E011-8C5D-003048F024FE.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/311/D270EFF2-BE57-E011-AD5C-0030487C6062.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/311/62248761-C557-E011-9293-001617E30F48.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/311/4EBC876F-CE57-E011-965B-0030487C8CB6.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/311/3EAE027C-5A58-E011-9A86-003048F117B4.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/311/12418487-D557-E011-BCFA-001D09F24E39.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/310/10627686-7859-E011-9B70-003048F1C58C.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/305/A0AC21D5-1E57-E011-B338-003048F1182E.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/303/2C647258-2057-E011-8E65-003048F0258C.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/301/82F74A53-1D57-E011-8227-003048F118C6.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/233/1C904C29-6857-E011-9913-0030487C90EE.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/224/B8355784-B756-E011-A807-001D09F291D7.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/223/BC2FC6CB-095A-E011-83F5-003048F118DE.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/223/407815B5-FD56-E011-8030-001617C3B6DE.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/223/3207187F-0057-E011-B219-001D09F23A84.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/223/14EAC77D-0057-E011-83C5-001617E30D52.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/222/BCF95182-0557-E011-A187-003048F117B6.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/F823151E-0B5A-E011-B1FA-003048F117EC.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/F4729787-0E57-E011-A5C9-003048F118AC.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/E6D98922-1957-E011-A491-003048F11C5C.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/D69EE709-0B57-E011-996D-000423D9A212.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/C6FD8EAE-1057-E011-8E6F-000423D94908.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/BA2F0A7E-EB56-E011-ACD9-000423D94908.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/B241F3D0-FF56-E011-8214-003048F1BF68.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/AEE73320-1957-E011-A4B1-003048F1BF68.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/82073D07-1C57-E011-B045-003048F110BE.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/7C68821E-F156-E011-9B0C-000423D94494.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/32726183-ED56-E011-9633-001617E30CC8.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/1296A997-0557-E011-8B99-0030487A18A4.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/217/08AE9834-1B57-E011-981D-003048F11C28.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/216/222D23A2-7057-E011-88CE-001617C3B654.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/213/963025A8-5B56-E011-A065-0016177CA778.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/212/681ECE7B-5656-E011-BF10-001D09F2438A.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/210/860CA3A7-5856-E011-8FB3-001617C3B778.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/205/7CC15F93-5556-E011-B36F-003048D2BBF0.root',
       '/store/data/Run2011A/SingleElectron/AOD/PromptReco-v1/000/161/176/C451662C-B556-E011-8673-003048F11114.root',


##        '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/FE9CF7A8-5A2D-E011-9775-0030486790B0.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F2EDDFEA-592D-E011-97B7-0030486791F2.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F0ECB760-5A2D-E011-8CF0-002618943811.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E8927780-5A2D-E011-AEB1-00304867BEE4.root',
##               '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E0D2C67B-5A2D-E011-BA9A-002618943833.root',
       
       ) )



##-------- Scraping veto --------
process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)


##-------- Electron events of interest --------
process.HLTEle =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_*"),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
     throw = cms.bool(False) # throw exception on unknown path names
 )




##-------- Require at least two jets --------
process.RequireTwoJets = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("ak5PFJetsCorClean"),                      
)



##-------- Save V+jets trees --------
process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis", 
    srcPFCor = cms.InputTag("ak5PFJetsCorClean"), 
    srcVectorBoson = cms.InputTag("bestWToEnu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('electron'),                          
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('WJet'),
    runningOverMC = cms.bool(isMC),			
    srcGen  = cms.InputTag("ak5GenJets"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),                           
)



process.myseq = cms.Sequence(
    process.GenJetPath *
    process.noscraping *
    process.HLTEle * 
    process.impactParameterTagInfos * 
    process.secondaryVertexTagInfos *
    process.TagJetPath *
    process.CorPFJetPath *
    process.RequireTwoJets * 
    process.WPath *
    process.VpusJets
    )

if isMC:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HLTEle)
else:
    process.myseq.remove ( process.genParticles)
    process.myseq.remove ( process.GenJetPath)
    process.myseq.remove ( process.TagJetPath)
    

process.p = cms.Path( process.myseq )






