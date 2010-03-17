import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")



#   Z-->ee Collection ##########
process.load("ElectroWeakAnalysis.VPlusJets.ZeeCollections_cfi")

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
'/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3_7TeV-v1/0001/FC26E6E8-9A9C-DE11-AE71-001D0967D288.root',
'/store/mc/Summer09/ZeeJet_Pt80to120/GEN-SIM-RECO/MC_31X_V3_7TeV-v1/0001/F2000DF7-179D-DE11-A812-001D0967DAB7.root') )


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
##  srcPFJets =cms.untracked.VInputTag(
##    cms.InputTag("L2L3CorJetIC5PF"),
##    cms.InputTag("L2L3CorJetAKT5PF"),
##    ),                              
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
    srcVectorBoson = cms.InputTag("bestZee"),
    VBosonType     = cms.string('Z'),
    LeptonType     = cms.string('electron'),                          
    runningOverMC = cms.untracked.bool(True),
    srcFlavorByValue = cms.VInputTag(
      cms.InputTag("ic5tagJet"),
      cms.InputTag("sc5tagJet"),
      cms.InputTag("ak5tagJet"),
      ),                    
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






