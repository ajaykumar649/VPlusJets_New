import FWCore.ParameterSet.Config as cms
process = cms.Process("SKIM")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START39_V8::All'
process.load('RecoBTag.SecondaryVertex.combinedSecondaryVertexBJetTags_cfi')
process.load("RecoBTag.Configuration.RecoBTag_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )




#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#############   Define the source file ###############

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/FE9CF7A8-5A2D-E011-9775-0030486790B0.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F2EDDFEA-592D-E011-97B7-0030486791F2.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/F0ECB760-5A2D-E011-8CF0-002618943811.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E8927780-5A2D-E011-AEB1-00304867BEE4.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/E0D2C67B-5A2D-E011-BA9A-002618943833.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/D6AF4377-5A2D-E011-908C-00261894380D.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/D2F58F75-5A2D-E011-A9B5-0030486790B0.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/CE36D953-5A2D-E011-B51C-0026189438B9.root',
       '/store/mc/Winter10/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0062/CC67C85D-5A2D-E011-A66C-002618943914.root',
    
)
)


#############   Path       ###########################
#process.load('HLTrigger.Configuration.HLT_FULL_cff')
#from HLTrigger.Configuration.HLT_FULL_cff import *
## hltHT150U, hltHT160U, hltHT200U
## hltMET65, hltMET80, hltMET100, hltMET160



process.HLTHT150 = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag  = cms.InputTag("TriggerResults","","REDIGI39X"),
    HLTPaths           = cms.vstring("HLT_HT150U_v3"),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(True)
)


process.HLTHT160 = process.HLTHT150.clone()
process.HLTHT160.HLTPaths = cms.vstring("HLT_HT160U_v3")


process.HLTHT200 = process.HLTHT150.clone()
process.HLTHT200.HLTPaths = cms.vstring("HLT_HT200U_v3")



##-------- Electrons Triggers --------------
process.HLTElectrons = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag  = cms.InputTag("TriggerResults","","HLT"),
    #HLTPaths           = cms.vstring("HLT_Ele15_SC10_LW_L1R"),
    HLTPaths           = cms.vstring("HLT_Ele15_SW_LooseTrackIso_L1R"),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(True)
)



#  electrons  ################
process.goodElectrons = cms.EDFilter("GsfElectronRefSelector",
    src = cms.InputTag( "gsfElectrons" ), 
    cut = cms.string(
    "ecalDrivenSeed==1 && (abs(superCluster.eta)<2.4)"
    " && !(1.4442<abs(superCluster.eta)<1.566)"
    " && (ecalEnergy*sin(superClusterPosition.theta)>20.0)"
    " && (gsfTrack.trackerExpectedHitsInner.numberOfHits <= 1)"
    " && ( (dr03TkSumPt+dr03EcalRecHitSumEt+dr03HcalTowerSumEt)/p4.Pt<0.1)"     
    " && ((isEB"
    " && (sigmaIetaIeta<0.01)"
    " && ( -0.8<deltaPhiSuperClusterTrackAtVtx<0.8 )"
    " && ( -0.007<deltaEtaSuperClusterTrackAtVtx<0.007 )"
    " && (hadronicOverEm<0.15)"
    ")"
    " || (isEE"
    " && (sigmaIetaIeta<0.03)"
    " && ( -0.7<deltaPhiSuperClusterTrackAtVtx<0.7 )"
    " && ( -0.01<deltaEtaSuperClusterTrackAtVtx<0.01 )"
    " && (hadronicOverEm<0.07) "
    "))"
    )
)  ## WP95, combined relative isolation, |eta|<2.4



process.WToEnu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("goodElectrons pfMet"),
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
    cut = cms.string('pt>150  && daughter(1).pt>20 && 50 < sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))< 100'),
    checkCharge = cms.bool(False),
)


process.AtLeast1GoodWToEnu = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("WToEnu"),                  
)


##################### Corrected CaloJets
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
from JetMETCorrections.Configuration.DefaultJEC_cff import *
process.ak5PFJetsCor = cms.EDFilter("PtMinPFJetSelector",  
    src = cms.InputTag("ak5PFJetsL2L3"),
    ptMin = cms.double(20)
)

process.Exactly2Jets = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(2),
    src = cms.InputTag("ak5PFJetsCor"),                      
)


process.TwoJetsSumPt150 = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("ak5PFJetsCor ak5PFJetsCor"), 
    checkCharge = cms.bool(False),                           
    cut   = cms.string("mass > 0 && (daughter(0).pt + daughter(1).pt) >150"),
)


process.countTwoJetsSumPt150 = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("TwoJetsSumPt150"),                      
)


process.Btags = cms.EDProducer("JetValueMapSelector",
   input     = cms.InputTag( "ak5PFJetsCor" ),                             
   selection = cms.InputTag("combinedSecondaryVertexBJetTags"),
   cut = cms.double(0.52)
)                             

process.countBtags  = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(100),
    src = cms.InputTag("Btags"),                      
)
                                  

#############
process.AnalysisModule = cms.Sequence(process.ak5PFJetsL2L3*
                                      #process.impactParameterTagInfos * 
                                      #process.secondaryVertexTagInfos *
                                      #process.combinedSecondaryVertexBJetTags *
                                      process.ak5PFJetsCor *
                                      process.Exactly2Jets *
                                      process.TwoJetsSumPt150 *
                                      #process.Btags *
                                      #process.countBtags *
                                      process.countTwoJetsSumPt150 * 
                                      process.goodElectrons *
                                      process.WToEnu * 
                                      process.AtLeast1GoodWToEnu
                                      )


process.AnalysisPath = cms.Path(process.AnalysisModule)
process.pathHT150 = cms.Path(process.AnalysisModule * process.HLTHT150)
process.pathHT160 = cms.Path(process.AnalysisModule * process.HLTHT160)
process.pathHT200 = cms.Path(process.AnalysisModule * process.HLTHT200)


#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

