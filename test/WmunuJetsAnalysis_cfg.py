import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   W-->munu Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.WmunuCollections_cfi")

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
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0004/544F6FC2-A52B-DF11-8C2F-0018F3D09706.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0004/40E547FE-2C2C-DF11-84B4-0026189438FE.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/6A3D9E72-A42B-DF11-94EA-00304867BFB2.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/1E608FCE-A32B-DF11-BC7B-001731A2876F.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/0674C1E0-A42B-DF11-A01C-001A92811716.root') )


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
    srcVectorBoson = cms.InputTag("bestWmunu"),
    VBosonType     = cms.string('W'),
    LeptonType     = cms.string('muon'),                          
    runningOverMC = cms.untracked.bool(False),
    srcFlavorByValue = cms.VInputTag(
      cms.InputTag("ic5tagJet"),
      cms.InputTag("sc5tagJet"),
      cms.InputTag("ak5tagJet"),
      ),   
    triggerSummaryLabel = cms.InputTag( "hltTriggerSummaryAOD","","HLT" ), 
    hltTag = cms.InputTag("HLT_Mu9", "","HLT"),                   
    HistOutFile = cms.string('demo.root'),
    TreeName    = cms.string('WJet')                          
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



process.p = cms.Path( process.ak5CaloJets * 
                      process.CaloJetPath * process.CorJetPath *
                      process.GenJetPath * process.PFJetPath *
                      process.JPTJetPath * process.WPath *
                      process.TagJetPath * process.VpusJets
                      )






