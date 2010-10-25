import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   W-->enu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.WenuCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.CaloJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.CorrectedCaloJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.GenJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.PFJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.JPTJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.TagJetCollections_cfi")



#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/FEFE3E46-CB7B-DF11-8488-0018F3D09678.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/FEA6BEE7-CA7B-DF11-BEB0-00304867BED8.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/FCE7711D-CB7B-DF11-B469-003048678C26.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/FAA0922C-6C7B-DF11-82E6-00304867BFC6.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/FA24AFE0-EE7B-DF11-BDB3-001A92971ACE.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/F8D11B16-297C-DF11-B3EE-002618943950.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/F8B3D8F4-287C-DF11-904A-00261894383B.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/F8854F8C-C67B-DF11-AE1C-002618943899.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/F876C53E-CB7B-DF11-A402-00304867BF18.root',
       '/store/mc/Summer10/WJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v1/0046/F842A696-C67B-DF11-87B4-0026189438F6.root',
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
    srcVectorBoson = cms.InputTag("bestWenu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('electron'),                          
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    hltTag = cms.InputTag("HLT_Ele15_LW_L1R", "","HLT"),
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('WJet')                          
)




process.p = cms.Path( #process.genParticles * 
                      process.CaloJetPath * process.CorJetPath *
                      process.GenJetPath * process.PFJetPath *
                      process.JPTJetPath * process.WPath *
                      process.TagJetPath * process.VpusJets
                      )






