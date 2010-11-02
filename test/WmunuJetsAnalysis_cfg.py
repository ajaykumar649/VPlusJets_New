import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   W-->munu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.WmunuCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")



#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(

       '/store/relval/CMSSW_3_8_5/RelValWM/GEN-SIM-RECO/START38_V12-v1/0041/70AB981F-D2D2-DF11-A733-002618943870.root',
       '/store/relval/CMSSW_3_8_5/RelValWM/GEN-SIM-RECO/START38_V12-v1/0040/74F4115C-E9D1-DF11-B69D-00304867929E.root',
       '/store/relval/CMSSW_3_8_5/RelValWM/GEN-SIM-RECO/START38_V12-v1/0040/626A5615-23D2-DF11-898F-003048678D52.root',
       '/store/relval/CMSSW_3_8_5/RelValWM/GEN-SIM-RECO/START38_V12-v1/0039/DAC9D739-E7D1-DF11-8F0B-003048678BAE.root',
       '/store/relval/CMSSW_3_8_5/RelValWM/GEN-SIM-RECO/START38_V12-v1/0039/4EA99A1B-E5D1-DF11-9DCF-0018F3D09630.root') )


process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis",
    runningOverMC = cms.untracked.bool(False),                            
##     srcGen  = cms.VInputTag( 
##        cms.InputTag("ic5GenJetsClean"),
##        cms.InputTag("kt4GenJetsClean"),
##        cms.InputTag("ak5GenJetsClean"),
##        ),
##     srcFlavorByValue = cms.VInputTag(
##       cms.InputTag("ic5tagJet"),
##       cms.InputTag("kt4tagJet"),
##       cms.InputTag("ak5tagJet"),
##       ),   
                                  
    srcCaloCor = cms.VInputTag(
       cms.InputTag("ic5CaloJetsCorClean"),
       cms.InputTag("kt4CaloJetsCorClean"),
       cms.InputTag("ak5CaloJetsCorClean"),
       ),
    srcCalo = cms.VInputTag(
      cms.InputTag("ic5CaloJetsClean"),
      cms.InputTag("kt4CaloJetsClean"),
      cms.InputTag("ak5CaloJetsClean"),
      ),                              
    srcPFJets =cms.VInputTag(
      cms.InputTag("ic5PFJetsClean"),
      cms.InputTag("kt4PFJetsClean"),
      cms.InputTag("ak5PFJetsClean"),
      ),
    srcJPTJets =cms.VInputTag(
      cms.InputTag("ak5JPTJetsClean"),
      ),                              
    srcVectorBoson = cms.InputTag("bestWmunu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('muon'),                          
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    hltTag = cms.InputTag("HLT_Mu9", "","HLT"),                   
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('WJet')                          
)




process.p = cms.Path( #process.genParticles *
                      #process.GenJetPath * process.TagJetPath * 
                      process.CaloJetPath * process.CorJetPath *
                      process.PFJetPath *
                      process.JPTJetPath * process.WPath *
                      process.VpusJets
                      )






