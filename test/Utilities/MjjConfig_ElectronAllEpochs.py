import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = RooWjjFitterParams()

    fitterPars.MCDirectory = '/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree/RD_'
    fitterPars.WpJDirectory = fitterPars.MCDirectory
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = '/uscms_data/d2/kalanand/WjjTrees/NewReducedQCDTrees/'
    fitterPars.initParamsFile = initFile

    fitterPars.DataDirectory = fitterPars.MCDirectory
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_4p7invfb.root'
    fitterPars.includeMuons = False
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_4p7invfb.root'
##     fitterPars.electronData = 'SingleEleAndEleTwoPFJetTrigger.root'
##     fitterPars.electronData = 'EleTwoCaloJetTrigger.root'
    fitterPars.includeElectrons = True

    fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'

    fitterPars.fitToyDataset = False
    fitterPars.ToyDatasetDirectory = '/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSets/'
    fitterPars.toydataFile = toydataFile

    fitterPars.minMass = 30.
    fitterPars.maxMass = 400.
    fitterPars.nbins = 34
    fitterPars.intLumi = 4700.

    fitterPars.binEdges.push_back(fitterPars.minMass)

    binEdge = fitterPars.minMass
    print binEdge,' ',
    while (binEdge <= fitterPars.maxMass):
        binEdge += round(0.15*binEdge)
        fitterPars.binEdges.push_back(binEdge)
        print binEdge,' ',

    fitterPars.binEdges[fitterPars.binEdges.size()-1] = fitterPars.maxMass
##     fitterPars.maxMass = fitterPars.binEdges[fitterPars.binEdges.size()-1]
    
    print "mass range:",fitterPars.minMass,'-',fitterPars.maxMass
    
    fitterPars.truncRange = True
    fitterPars.blind = True
    fitterPars.minTrunc = 130.

    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minTrunc:
            fitterPars.minTrunc = binMin
            break
        binMin = binEdge

    fitterPars.maxTrunc = 170.
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxTrunc:
            fitterPars.maxTrunc = binEdge
            break

    fitterPars.minFit = 60.
    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minFit:
            fitterPars.minFit = binMin
            break
        binMin = binEdge

    fitterPars.maxFit = 400.
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxFit:
            fitterPars.maxFit = binEdge
            break

    print 'truncate:',fitterPars.truncRange,':', fitterPars.minTrunc,'-',\
          fitterPars.maxTrunc
    print 'fit range:',fitterPars.minFit,'-',fitterPars.maxFit

    fitterPars.njets = Nj
    fitterPars.constrainDiboson = True

    fitterPars.doEffCorrections = True
    fitterPars.muIdEffFiles.push_back("EffTableDir/muonEffsRecoToIso_ScaleFactors.txt")
    fitterPars.muHLTEffFiles.push_back("EffTableDir/muonEffsIsoToHLT_data_LP_LWA.txt")
    fitterPars.lumiPerEpochMuon.push_back(fitterPars.intLumi)

#### Add the information for Electron each epoch ####
#### Epoch Luminosities are 215.274+664.732=880.006, 927.947, 323.164, 412.359, 246.527, 1909.9970 (Sum Lumi = 4700).
    ## Epochs 1+7
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/FullyEfficient.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/FullyEfficient.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/FullyEfficient_MHT.txt")
    fitterPars.lumiPerEpochElectron.push_back(880.006)
    ## Epoch 2
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet30_FracLumi_PromptReco-v4-PFEE-EleJets.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet25Not30_FracLumi_PromptReco-v4-PFEE-EleJets.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt")
    fitterPars.lumiPerEpochElectron.push_back(927.947)
    ## Epoch 3
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet30_FracLumi_05Aug2011ReReco-v1-2E33.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet25Not30_FracLumi_05Aug2011ReReco-v1-2E33.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt")
    fitterPars.lumiPerEpochElectron.push_back(323.164)
    ## Epoch 4
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet30_FracLumi_PromptReco-v6-2E33.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet25Not30_FracLumi_PromptReco-v6-2E33.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt")
    fitterPars.lumiPerEpochElectron.push_back(412.359)
    ## Epoch 5
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet30_FracLumi_PromptReco-v6-3E33.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet25Not30_FracLumi_PromptReco-v6-3E33.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt")
    fitterPars.lumiPerEpochElectron.push_back(246.527)
    ## Epoch 6
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsWP80ToHLTEle27_May10ReReco.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet30_FracLumi_post-epsCaloLWA.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_Jet25Not30_FracLumi_post-epsCaloLWA.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt")
    fitterPars.lumiPerEpochElectron.push_back(1909.9970)



    fitterPars.useExternalMorphingPars = False
       
    fitterPars.cuts = '(W_mt > 50.) ' + \
                      '&& (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) ' + \
                      '&& (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor<0.7) ' + \
                      '&& (JetPFCor_Pt[0] > 40) '

    return fitterPars
