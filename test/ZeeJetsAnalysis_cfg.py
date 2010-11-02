import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")


#   Z-->ee Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.ZeeCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.JetCollections_cfi")


#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(

       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/ACDEDA3C-B7D3-DF11-A7A1-0030487C6A66.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/454/223CD93D-B7D3-DF11-885E-0030487CD7B4.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/EAB3E588-B6D3-DF11-8BDC-0030487A3232.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/AA0C5537-B7D3-DF11-9194-0030487CD7C6.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/A28CBA36-B7D3-DF11-9F37-00304879BAB2.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/8C67199B-B1D3-DF11-AAC4-0030487CD7CA.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/823B32EE-B7D3-DF11-B2CB-0030487CAF0E.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/3C66014F-B2D3-DF11-9E18-0030487CD6DA.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/3AA99C36-B7D3-DF11-BB90-0030487CAEAC.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/26B82C89-B6D3-DF11-9584-0030487CD6B4.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/453/0AA663A1-B8D3-DF11-ADD8-0030487CD6B4.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/F08E2485-95D3-DF11-842A-0030486780B8.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/DCF42686-95D3-DF11-8DF0-0030487CD76A.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/5E17B94C-9DD3-DF11-A952-001617E30F58.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/54CDACD8-94D3-DF11-B6A6-001617E30D12.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/346D9037-96D3-DF11-88DF-001617C3B710.root',
       '/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/147/452/029B8885-95D3-DF11-B1FE-001617E30D4A.root',


##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/E82FBD2C-21C9-DF11-B7FB-0022649E7902.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/749FFE35-3AC9-DF11-A44A-001F29C4C3FE.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/72167F8D-24C9-DF11-9AB1-0017A4770C3C.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0001/062E5179-24C9-DF11-98FC-001F29C4C3BA.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/FCFF5A75-B1C8-DF11-BEF6-0017A4770C2C.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/F2604CB0-B5C8-DF11-A2C0-001E0BE922E2.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/EE6077BF-B1C8-DF11-8AD9-001F296A527C.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/D6FF2C65-B6C8-DF11-B219-00237DA13F9E.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/D4016737-AFC8-DF11-95C7-1CC1DE0590E8.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/C81E5552-B3C8-DF11-81F6-001F29C4C3FE.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/C035330C-B2C8-DF11-A52D-0017A4770038.root',
##        '/store/mc/Fall10/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/B29141A3-AFC8-DF11-81E8-1CC1DE0500F0.root',  
) )


process.VpusJets = cms.EDAnalyzer("VplusJetsAnalysis",
    runningOverMC = cms.untracked.bool(False),                                  
##     srcGen  = cms.VInputTag( 
##        cms.InputTag("iterativeCone5GenJets"),
##        cms.InputTag("kt4GenJets"),
##        cms.InputTag("ak5GenJets"),
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
    srcPFCor = cms.VInputTag(
       cms.InputTag("ic5PFJetsCorClean"),
       cms.InputTag("kt4PFJetsCorClean"),
       cms.InputTag("ak5PFJetsCorClean"),
       ),
    srcJPTCor = cms.VInputTag(
       cms.InputTag("ak5JPTJetsCorClean"),
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
    JetIDParams = cms.PSet(
       useRecHits = cms.bool(True),
       hbheRecHitsColl = cms.InputTag("hbhereco"),
       hoRecHitsColl   = cms.InputTag("horeco"),
       hfRecHitsColl   = cms.InputTag("hfreco"),
       ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
       eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
     ),                          
    srcVectorBoson = cms.InputTag("bestZee"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('electron'),                          
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    #hltTag = cms.InputTag("HLT_Ele15_LW_L1R", "","HLT"),
    hltTag = cms.InputTag("HLT_Ele17_SW_TightEleId_L1R", "","HLT"),                    
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('ZJet')                          
)


## process.ak5CaloJets = cms.EDFilter("CaloJetSelector",   
##     src = cms.InputTag("antikt5CaloJets"),
##     cut = cms.string('pt > 0.0')
## )


### work around for the time being: need to change antikt5 => ak5
## process.antikt5CaloJets = cms.EDFilter("CaloJetSelector",   
##     src = cms.InputTag("ak5CaloJets"),
##     cut = cms.string("")
## )

## process.antikt5GenJets = cms.EDFilter("GenJetSelector",   
##     src = cms.InputTag("ak5GenJets"),
##     cut = cms.string("")
## )

## process.antikt5PFJets = cms.EDFilter("PtMinPFJetSelector",   
##     src = cms.InputTag("ak5PFJets"),
##     ptMin = cms.double(0)
## )



process.p = cms.Path( #process.genParticles *
                      #process.GenJetPath * process.TagJetPath * 
                      process.CaloJetPath * process.CorJetPath *
                      process.PFJetPath * process.CorPFJetPath *
                      process.JPTJetPath * process.ZPath *
                      process.VpusJets
                      )






