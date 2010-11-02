import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   Z-->mumu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.ZmumuCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")


#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/AAA0E19F-38C9-DF11-AF32-002481ACF3B0.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/26814776-3BC9-DF11-8807-0017A4770808.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/062A828A-2CC9-DF11-B3F6-00237DA13F9E.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/FAF27C05-BEC8-DF11-8E72-001E0B486164.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/F826E92C-C1C8-DF11-8CD9-00237DA15CE6.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/E4F9FD66-B8C8-DF11-B522-0025B3E01FC2.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/CE878A66-BEC8-DF11-A762-00237DA28240.root',
       '/store/mc/Fall10/ZJetToMuMu_Pt_170to230_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/AE096133-B9C8-DF11-AE08-0017A477040C.root',    
) )


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
    srcVectorBoson = cms.InputTag("bestZmumu"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('muon'),                          
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    hltTag = cms.InputTag("HLT_Mu11", "","HLT"),                  
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('ZJet')                          
)




process.p = cms.Path( #process.genParticles *
                      #process.GenJetPath * process.TagJetPath * 
                      process.CaloJetPath * process.CorJetPath *
                      process.PFJetPath *
                      process.JPTJetPath * process.ZPath *
                      process.VpusJets
                      )






