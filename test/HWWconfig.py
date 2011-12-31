import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams


def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = "/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree/RD_"
    fitterPars.WpJDirectory = fitterPars.MCDirectory
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = False
    fitterPars.QCDDirectory = "/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree/"
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    fitterPars.DataDirectory = fitterPars.MCDirectory 
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_4p7invfb.root'
    fitterPars.includeMuons = True    
    fitterPars.electronData = 'WenuJets_DataAllSingleElectronTrigger_GoldenJSON_4p7invfb.root'
    fitterPars.includeElectrons = True
   
    fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.minMass = 50.
    fitterPars.maxMass = 120.
    fitterPars.nbins = 14
    fitterPars.intLumi = 4700.

    
    fitterPars.truncRange = True
    fitterPars.blind = False
    fitterPars.minTrunc = 65.
    fitterPars.maxTrunc = 95.
    fitterPars.minWmass = fitterPars.minTrunc
    fitterPars.maxWmass = fitterPars.maxTrunc
    fitterPars.minFit = 60
    fitterPars.maxFit = fitterPars.maxMass
    
    print 'truncate:',fitterPars.truncRange,':', fitterPars.minTrunc,'-',\
          fitterPars.maxTrunc
    print 'fit range:',fitterPars.minFit,'-',fitterPars.maxFit
    
    fitterPars.njets = Nj
    fitterPars.constrainDiboson = True

    
    fitterPars.doEffCorrections = True
    fitterPars.muIdEffFiles.push_back("EffTableDir/muonEffsRecoToIso_ScaleFactors.txt")
    fitterPars.muHLTEffFiles.push_back("EffTableDir/muonEffsIsoToHLT_data_LP_LWA.txt")
    fitterPars.lumiPerEpochMuon.push_back(fitterPars.intLumi)

    
    fitterPars.eleIdEffFiles.push_back("EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTableDir/eleEffsSCToReco_ScaleFactors.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTableDir/eleEffsSingleElectron.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/FullyEfficient.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/FullyEfficient_Jet2NoJet1.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/FullyEfficient_MHT.txt")
    fitterPars.lumiPerEpochElectron.push_back(fitterPars.intLumi)

    fitterPars.constrainDiboson = True
    fitterPars.useExternalMorphingPars = False
    fitterPars.cuts = '(fit_status==0) && (ggdevt==%d) ' %(Nj)
    
    return fitterPars

def the4BodyConfig(Nj, mcdir = '', initFile = '', alpha=1.):
    fitterPars = theConfig(Nj, mcdir, initFile)

    fitterPars.do4body = True
    twoBCut = fitterPars.cuts
    print twoBCut
    if len(fitterPars.cuts) > 0:
        fitterPars.cuts += " && "
    fitterPars.cuts += '(({0} > {1:f}) && ({0} < {2:f}))'.format(fitterPars.var,fitterPars.minTrunc, fitterPars.maxTrunc)

    fitterPars.minSBHi = fitterPars.maxWmass
    fitterPars.maxSBHi = fitterPars.maxWmass + 20.
    fitterPars.SBHicut = twoBCut
    if len(twoBCut) > 0:
        fitterPars.SBHicut += " && "
    fitterPars.SBHicut += '(({0} > {1:f}) && ({0} < {2:f}))'.format(fitterPars.var,fitterPars.minSBHi, fitterPars.maxSBHi)
    print 'SBHicut',fitterPars.SBHicut
    
    fitterPars.minSBLo = fitterPars.minWmass - 10.
    fitterPars.maxSBLo = fitterPars.minWmass
    fitterPars.SBLocut = twoBCut
    if len(twoBCut) > 0:
        fitterPars.SBLocut += " && "
    fitterPars.SBLocut += '(({0} > {1:f}) && ({0} < {2:f}))'.format(fitterPars.var,fitterPars.minSBLo, fitterPars.maxSBLo)
    print 'SBLocut',fitterPars.SBLocut

    fitterPars.var = 'fit_mlvjj'
    fitterPars.minMass = 140.
    fitterPars.maxMass = 800.
    fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/10 + 0.5)
    fitterPars.truncRange = False


    twoJets = [(fitterPars.minMass, fitterPars.maxMass, alpha, 1.)]
    threeJets = [(fitterPars.minMass, fitterPars.maxMass, alpha, 1.)]

    if Nj == 3:
        ranges = threeJets
    else:
        ranges = twoJets

    for line in ranges:
        fitterPars.minMasses.push_back(line[0])
        if len(line) > 1:
            fitterPars.falphas.push_back(line[1])

    return fitterPars
