import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   Z-->mumu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.ZmumuCollections_cfi")

#  Jet Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.CaloJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.CorrectedCaloJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.GenJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.PFJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.JPTJetCollections_cfi")
process.load("ElectroWeakAnalysis.VPlusJets.TagJetCollections_cfi")



#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0004/88BA8DF5-2C2C-DF11-8D9A-00261894382D.root',
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0003/AEAC9F78-852B-DF11-9EB9-002618943932.root',
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0003/AA7437FF-7F2B-DF11-8622-001731A2845B.root',
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0003/86053022-892B-DF11-9BE0-002618943902.root',
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0003/70E67EE1-822B-DF11-9DC8-001A92810AAA.root',
'/store/relval/CMSSW_3_5_4/RelValZMM/GEN-SIM-RECO/START3X_V24-v1/0003/3A22B825-872B-DF11-AAFB-00304867920C.root') )


process.VpusJets = cms.EDFilter("VplusJetsAnalysis",
    srcGen  = cms.VInputTag( 
       cms.InputTag("ic5GenJetsClean"),
       cms.InputTag("sc5GenJetsClean"),
       cms.InputTag("ak5GenJetsClean"),
       ),
    srcCaloCor = cms.VInputTag(
       cms.InputTag("ic5CaloJetsCorClean"),
       cms.InputTag("sc5CaloJetsCorClean"),
       cms.InputTag("ak5CaloJetsCorClean"),
       ),
    srcCalo = cms.VInputTag(
      cms.InputTag("ic5CaloJetsClean"),
      cms.InputTag("sc5CaloJetsClean"),
      cms.InputTag("ak5CaloJetsClean"),
      ),                         
    srcPFJets =cms.VInputTag(
      cms.InputTag("ic5PFJetsClean"),
      cms.InputTag("sc5PFJetsClean"),
      cms.InputTag("ak5PFJetsClean"),
      ),
    srcJPTJets =cms.VInputTag(
      cms.InputTag("ic5JPTJetsClean"),
      cms.InputTag("sc5JPTJetsClean"),
      cms.InputTag("ak5JPTJetsClean"),
      ),                              
    srcVectorBoson = cms.InputTag("bestZmumu"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('muon'),                          
    runningOverMC = cms.untracked.bool(True),
    srcFlavorByValue = cms.VInputTag(
      cms.InputTag("ic5tagJet"),
      cms.InputTag("sc5tagJet"),
      cms.InputTag("ak5tagJet"),
      ),  
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    hltTag = cms.InputTag("HLT_Mu9", "","HLT"),                  
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('ZJet')                          
)


process.ak5CaloJets = cms.EDFilter("CaloJetSelector",   
    src = cms.InputTag("antikt5CaloJets"),
    cut = cms.string('pt > 0.0')
)


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



process.p = cms.Path( process.genParticles * process.ak5CaloJets * 
                      process.CaloJetPath * process.CorJetPath *
                      process.GenJetPath * process.PFJetPath *
                      process.JPTJetPath * process.ZPath *
                      process.TagJetPath * process.VpusJets
                      )






