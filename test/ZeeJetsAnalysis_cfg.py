import FWCore.ParameterSet.Config as cms
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
if isMC:
    #process.GlobalTag.globaltag = 'START39_V8::All'
    process.GlobalTag.globaltag = 'START38_V14::All'
else:
    process.GlobalTag.globaltag = 'GR_R_39X_V5::All'
OutputFileName = "demo.root"
numEventsToRun = -1
############################################



#   Z-->ee Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.ZeeCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
##        '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/ACDEDA3C-B7D3-DF11-A7A1-0030487C6A66.root',
##        '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/223CD93D-B7D3-DF11-885E-0030487CD7B4.root',

       '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/E82FBD2C-21C9-DF11-B7FB-0022649E7902.root',
       '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/749FFE35-3AC9-DF11-A44A-001F29C4C3FE.root',
) )



process.RequireOneJet = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("ak5PFJetsCorClean"),                      
)



process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis",
    srcCaloCor = cms.InputTag("ak5CaloJetsCorClean"),
    srcPFCor = cms.InputTag("ak5PFJetsCorClean"),
    srcJPTCor = cms.InputTag("ak5JPTJetsCorClean"),
    srcCalo = cms.InputTag("ak5CaloJetsClean"),
    srcPFJets = cms.InputTag("ak5PFJetsClean"),
    srcJPTJets = cms.InputTag("ak5JPTJetsClean"),
    srcVectorBoson = cms.InputTag("bestZee"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('electron'),                          
    runningOverMC = cms.untracked.bool(isMC),                                  
    srcGen  = cms.InputTag("ak5GenJets"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('ZJet')                          
)





process.myseq = cms.Sequence( 
    process.GenJetPath *
    process.impactParameterTagInfos * 
    process.secondaryVertexTagInfos *
    process.TagJetPath *
    process.CaloJetPath *
    process.CorJetPath *
    process.PFJetPath *
    process.CorPFJetPath *
    process.JPTJetPath *
    process.RequireOneJet * 
    process.ZPath *
    process.VpusJets
    )

if not isMC:
    process.myseq.remove ( process.genParticles)
    process.myseq.remove ( process.GenJetPath)
    process.myseq.remove ( process.TagJetPath)




process.p = cms.Path( process.myseq )
