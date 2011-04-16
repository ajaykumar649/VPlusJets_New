
import FWCore.ParameterSet.Config as cms

###############################
####### Parameters ############
###############################
useData = True


if not useData :
    GLOBAL_TAG = 'START39_V8::All'
    inputFile = 'dcap:///pnfs/cms/WAX/11/store/mc/Fall10/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0005/F24E17D7-F3CD-DF11-89A4-00215E221938.root'
else :
    GLOBAL_TAG = 'GR_R_39X_V4::All'
    inputFile = 'file:/uscms_data/d2/kalanand/dijet-Run2010A-JetMET-Nov4ReReco-9667events.root'        



process = cms.Process("Ana")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag = GLOBAL_TAG

#############   Format MessageLogger #################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10


#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFile)
)
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")




###############################
####### Global Setup ##########
###############################

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )
# HB + HE noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')


process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24), 
                                           maxd0 = cms.double(2) 
                                           )

###############################
########## CA08 Jets #######
###############################
process.load("RecoJets.Configuration.GenJetParticles_cff")
from RecoJets.JetProducers.ca4GenJets_cfi import ca4GenJets
process.ca8GenJets = ca4GenJets.clone( rParam = cms.double(0.8) )

from RecoJets.JetProducers.ca4PFJets_cfi import ca4PFJets
process.ca8PFJets = ca4PFJets.clone( rParam = cms.double(0.8) )


###############################
#### Jet RECO includes ########
###############################

from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.CaloJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.CATopJetParameters_cfi import *



###############################
###### Jet Pruning Setup ######
###############################
# Pruned PF Jets
process.caPrunedPFlow = cms.EDProducer(
    "SubJetProducer",
    PFJetParameters.clone( src = cms.InputTag("particleFlow"),
                           doAreaFastjet = cms.bool(True),
                           doRhoFastjet = cms.bool(True),
                           inputEMin = cms.double(1.0),
                           Ghost_EtaMax = cms.double(6),
                           Rho_EtaMax = cms.double(4.5)
                           ),
    AnomalousCellParameters,
    SubJetParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam = SubJetParameters.jetSize,
    jetCollInstanceName=cms.string("subjets")
    )

process.caPrunedPFlow.nSubjets = cms.int32(2)

# Pruned PF Jets, pileup removed
process.caPrunedPFlowPUSub = cms.EDProducer(
    "SubJetProducer",
    PFJetParameters.clone( src = cms.InputTag("particleFlow"),
                           doAreaFastjet = cms.bool(True),
                           doRhoFastjet = cms.bool(True),
                           inputEMin = cms.double(1.0),
                           Ghost_EtaMax = cms.double(6),
                           Rho_EtaMax = cms.double(4.5)
                           ),
    AnomalousCellParameters,
    SubJetParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam = SubJetParameters.jetSize,
    jetCollInstanceName=cms.string("subjets")
    )

process.caPrunedPFlowPUSub.nSubjets = cms.int32(2)

###############################
#### CATopTag Setup ###########
###############################

# CATopJet PF Jets
# with adjacency 
process.caTopTagPFlow = cms.EDProducer(
    "CATopJetProducer",
    PFJetParameters.clone( src = cms.InputTag("particleFlow"),
                           doAreaFastjet = cms.bool(True),
                           doRhoFastjet = cms.bool(True),
                           Ghost_EtaMax = cms.double(6.0),
                           Rho_EtaMax = cms.double(4.5)
                           ),
    AnomalousCellParameters,
    CATopJetParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam = SubJetParameters.jetSize
    )

process.CATopTagInfosPFlow = cms.EDProducer("CATopJetTagger",
                                    src = cms.InputTag("caTopTagPFlow"),
                                    TopMass = cms.double(171),
                                    TopMassMin = cms.double(0.),
                                    TopMassMax = cms.double(250.),
                                    WMass = cms.double(80.4),
                                    WMassMin = cms.double(0.0),
                                    WMassMax = cms.double(200.0),
                                    MinMassMin = cms.double(0.0),
                                    MinMassMax = cms.double(200.0),
                                    verbose = cms.bool(False)
                                    )


# CATopJet PF Jets
# with adjacency 
process.caTopTagPFlowPUSub = cms.EDProducer(
    "CATopJetProducer",
    PFJetParameters.clone( src = cms.InputTag("particleFlow"),
                           doAreaFastjet = cms.bool(True),
                           doRhoFastjet = cms.bool(True),
                           Ghost_EtaMax = cms.double(6.0),
                           Rho_EtaMax = cms.double(4.5)
                           ),
    AnomalousCellParameters,
    CATopJetParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam = SubJetParameters.jetSize
    )

process.CATopTagInfosPFlowPUSub = cms.EDProducer("CATopJetTagger",
                                    src = cms.InputTag("caTopTagPFlowPUSub"),
                                    TopMass = cms.double(171),
                                    TopMassMin = cms.double(0.),
                                    TopMassMax = cms.double(250.),
                                    WMass = cms.double(80.4),
                                    WMassMin = cms.double(0.0),
                                    WMassMax = cms.double(200.0),
                                    MinMassMin = cms.double(0.0),
                                    MinMassMax = cms.double(200.0),
                                    verbose = cms.bool(False)
                                    )





## # CATopJet PF Jets
## process.CAsequence = ( process.ca8PFJets *
##                        process.caPrunedPFlow *
##                        process.caPrunedPFlowPUSub *
##                        process.caTopTagPFlow *
##                        process.CATopTagInfosPFlow *
##                        process.caTopTagPFlowPUSub *
##                        process.CATopTagInfosPFlowPUSub
##                        )






###############################
#### Selections Setup #########
###############################
process.fatJet = cms.EDFilter("PFJetSelector",  
    src = cms.InputTag("caPrunedPFlow"),
    cut = cms.string('pt > 100.0 && eta<3.0 && eta>-3.0')
)

process.fatJetHistos = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string('fatJet'),
    HistoFileName = cms.string('FatJetHisto.root'),
    NJets         = cms.int32(4)
)


# let it run
if useData == False :
    process.p0 = cms.Path(
        process.scrapingVeto*
        process.primaryVertexFilter*
        process.HBHENoiseFilter*
        process.genJetParticles*
        process.ca8GenJets*
        process.ca8PFJets *
        process.caPrunedPFlow *
        process.caPrunedPFlowPUSub *
        process.caTopTagPFlow *
        process.CATopTagInfosPFlow *
        process.caTopTagPFlowPUSub *
        process.CATopTagInfosPFlowPUSub *
        process.fatJet * process.fatJetHistos
    )
else:    
    process.p0 = cms.Path(
        process.scrapingVeto*
        process.primaryVertexFilter*
        process.HBHENoiseFilter*
        process.ca8PFJets *
        process.caPrunedPFlow *
        process.caPrunedPFlowPUSub *
        process.caTopTagPFlow *
        process.CATopTagInfosPFlow *
        process.caTopTagPFlowPUSub *
        process.CATopTagInfosPFlowPUSub *
        process.fatJet * process.fatJetHistos
        )


