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
#from PhysicsTools.PatAlgos.patTemplate_cfg import *


############################################
if not isMC:
    process.GlobalTag.globaltag = 'GR_R_53_V10::All'
else:
    process.GlobalTag.globaltag = 'START53_V7E::All'

OutputFileName = "WmunuJetAnalysisntuple.root"
numEventsToRun = -1
############################################
########################################################################################
########################################################################################

##---------  W-->munu Collection ------------
process.load("ElectroWeakAnalysis.VPlusJets.WmunuCollectionsPAT_cfi")

##---------  Jet Collection ----------------
process.load("ElectroWeakAnalysis.VPlusJets.JetCollectionsPAT_cfi")

##---------  Vertex and track Collections -----------
process.load("ElectroWeakAnalysis.VPlusJets.TrackCollections_cfi")
#


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

#process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound')
#)
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/user/lnujj/PatTuples_8TeV_53X-v1/jdamgov/SingleMu/SQWaT_PAT_53X_2012B-13Jul2012-v1_part1v3/3e4086321697e2c39c90dad08848274b/pat_53x_test_v03_data_9_0_BNS.root'
#       '/store/user/lnujj/PatTuples_8TeV_53X-v1/jdamgov/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/SQWaT_PAT_53X_Summer12_v1/829f288d768dd564418efaaf3a8ab9aa/pat_53x_test_v03_995_1_wBa.root'
) )




##-------- Electron events of interest --------
process.HLTMu =cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
     HLTPaths = cms.vstring('HLT_IsoMu24_*','HLT_IsoMu30_*'),
     eventSetupPathsKey = cms.string(''),
     andOr = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
     throw = cms.bool(False) # throw exception on unknown path names
)

process.primaryVertex.src = cms.InputTag("goodOfflinePrimaryVertices");
process.primaryVertex.cut = cms.string(" ");

##########################################
## Filter to require at least two jets in the event
process.RequireTwoJetsORboostedV = cms.EDFilter("JetsORboostedV",
    minNumber = cms.untracked.int32(2),
    maxNumber = cms.untracked.int32(100),
    srcJets = cms.InputTag("ak5PFJetsLooseId"),
    srcVectorBoson = cms.InputTag("bestWmunu"),
    minVpt = cms.untracked.double(100.)
)


##-------- Save V+jets trees --------
process.VplusJets = cms.EDAnalyzer("VplusJetsAnalysis", 
    jetType = cms.string("PF"),
  #  srcPFCor = cms.InputTag("selectedPatJetsPFlow"),
    srcPFCor = cms.InputTag("ak5PFJetsLooseId"),
    srcPhoton = cms.InputTag("photons"),
    IsoValPhoton = cms.VInputTag(cms.InputTag('phoPFIso:chIsoForGsfEle'),
                                 cms.InputTag('phoPFIso:phIsoForGsfEle'),
                                 cms.InputTag('phoPFIso:nhIsoForGsfEle'),
                                                           ),
    srcPFCorVBFTag = cms.InputTag("ak5PFJetsLooseIdVBFTag"), 
    srcVectorBoson = cms.InputTag("bestWmunu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('muon'),                          
    HistOutFile = cms.string( OutputFileName ),
    TreeName    = cms.string('WJet'),
    srcPrimaryVertex = cms.InputTag("goodOfflinePrimaryVertices"),                               
    runningOverMC = cms.bool(isMC),			
    runningOverAOD = cms.bool(False),			
    srcMet = cms.InputTag("patType1CorrectedPFMet"),
    srcMetMVA = cms.InputTag("pfMEtMVA"),
    srcGen  = cms.InputTag("ak5GenJets"),
    srcMuons  = cms.InputTag("selectedPatMuonsPFlow"),
    srcBeamSpot  = cms.InputTag("offlineBeamSpot"),
    srcCaloMet  = cms.InputTag("patMETs"),
    srcgenMet  = cms.InputTag("genMetTrue"),
    srcGenParticles  = cms.InputTag("genParticles"),
    srcTcMet    = cms.InputTag("patMETsAK5TC"),
    srcJetsforRho = cms.string("kt6PFJetsPFlow"),                               
    srcJetsforRho_lepIso = cms.string("kt6PFJetsForIsolation"),       
    srcJetsforRhoCHS = cms.string("kt6PFJetsChsPFlow"),
    srcJetsforRho_lepIsoCHS = cms.string("kt6PFJetsChsForIsolationPFlow"),
    srcFlavorByValue = cms.InputTag("ak5tagJet"),
    bTagger=cms.string("simpleSecondaryVertexHighEffBJetTags"),

    applyJECToGroomedJets=cms.bool(True),
)

if isMC:
    process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("START53_V7E")
else:
    process.VplusJets.JEC_GlobalTag_forGroomedJet = cms.string("GR_R_53_V10")


process.myseq = cms.Sequence(
    process.TrackVtxPath *
    process.HLTMu *
    process.WPath *
    process.GenJetPath *
##    process.btagging * 
    process.TagJetPath *
    process.PFJetPath *
    process.RequireTwoJetsORboostedV
    )

if isMC:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HBHENoiseFilter)
    process.myseq.remove ( process.HLTMu)
else:
    process.myseq.remove ( process.noscraping)
    process.myseq.remove ( process.HBHENoiseFilter)
    process.myseq.remove ( process.GenJetPath)
    process.myseq.remove ( process.TagJetPath)


##---- if do not want to require >= 2 jets then disable that filter ---
##process.myseq.remove ( process.RequireTwoJets)  

#process.outpath.remove(process.out)
process.p = cms.Path( process.myseq  * process.VplusJets)






