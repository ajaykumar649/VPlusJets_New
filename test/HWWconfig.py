import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

minMlvjj = 170.
maxMlvjj = 770.

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.smoothingOrder = 1
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
    fitterPars.minMass = 35.
    fitterPars.maxMass = 200.
    fitterPars.nbins = 30
    fitterPars.intLumi = 4700.

    
    fitterPars.binEdges.push_back(fitterPars.minMass)
    fitterPars.binEdges.push_back(40.)
    fitterPars.binEdges.push_back(45.)
    fitterPars.binEdges.push_back(50.)
    fitterPars.binEdges.push_back(55.)
    fitterPars.binEdges.push_back(60.)
    fitterPars.binEdges.push_back(65.)
    fitterPars.binEdges.push_back(70.)
    fitterPars.binEdges.push_back(75.)
    fitterPars.binEdges.push_back(80.)
    fitterPars.binEdges.push_back(85.)
    fitterPars.binEdges.push_back(95.)
    fitterPars.binEdges.push_back(105.)
    fitterPars.binEdges.push_back(115.)
    fitterPars.binEdges.push_back(125.)
    fitterPars.binEdges.push_back(135.)
    fitterPars.binEdges.push_back(145.)
    fitterPars.binEdges.push_back(160.)
    fitterPars.binEdges.push_back(175.)
    fitterPars.binEdges.push_back(200.)

##     binEdge = fitterPars.minMass
##     print binEdge,' ',
##     while (binEdge <= fitterPars.maxMass):
##         binEdge += round(0.14*binEdge)
##         fitterPars.binEdges.push_back(binEdge)
##         print binEdge,' ',

##     fitterPars.binEdges[fitterPars.binEdges.size()-1] = fitterPars.maxMass

    print "mass range:",fitterPars.minMass,'-',fitterPars.maxMass
    
    fitterPars.truncRange = True
    fitterPars.blind = False
    fitterPars.minTrunc = 65.

    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minTrunc:
            fitterPars.minTrunc = binMin
            break
        binMin = binEdge

    fitterPars.maxTrunc = 94.
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxTrunc:
            fitterPars.maxTrunc = binEdge
            break

    fitterPars.minFit = fitterPars.minMass
    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minFit:
            fitterPars.minFit = binMin
            break
        binMin = binEdge

    fitterPars.maxFit = fitterPars.maxMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxFit:
            fitterPars.maxFit = binEdge
            break


    fitterPars.minWmass = fitterPars.minTrunc
    fitterPars.maxWmass = fitterPars.maxTrunc
    
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
    fitterPars.eleWMtEffFiles.push_back("EffTableDir/WMt50TriggerEfficiency.txt")
    fitterPars.lumiPerEpochElectron.push_back(fitterPars.intLumi)

    fitterPars.constrainDiboson = True
    fitterPars.useExternalMorphingPars = False

    jetCut = '(ggdevt == %i)' % (Nj)
    if (Nj < 2):
        jetCut = '((ggdevt == 2) || (ggdevt == 3))'

    fitterPars.cuts = '(fit_status==0) ' + \
                      '&& %s ' % (jetCut) + \
                      '&& (fit_mlvjj > %0.1f) && (fit_mlvjj < %0.1f) ' % \
                      (minMlvjj, maxMlvjj)

    return fitterPars

def the4BodyConfig(twoBodyConfig, alpha=1.):
##     fitterPars = theConfig(Nj, mcdir, initFile)

    fitterPars = RooWjjFitterParams(twoBodyConfig)
    fitterPars.binEdges.clear()
    fitterPars.smoothingOrder = 1
    fitterPars.smoothWpJ = 0
    fitterPars.do4body = True
    twoBCut = fitterPars.cuts
    print twoBCut
    if len(fitterPars.cuts) > 0:
        fitterPars.cuts += " && "
    fitterPars.cuts += '((%s > %f) && (%s < %f))' % (fitterPars.var,
                                                     fitterPars.minTrunc,
                                                     fitterPars.var,
                                                     fitterPars.maxTrunc)

    fitterPars.minSBHi = fitterPars.maxWmass
    fitterPars.maxSBHi = fitterPars.maxWmass + 20.
    fitterPars.SBHicut = twoBCut
    if len(twoBCut) > 0:
        fitterPars.SBHicut += " && "
    fitterPars.SBHicut += '((%s > %f) && (%s < %f))' % (fitterPars.var,
                                                        fitterPars.minSBHi,
                                                        fitterPars.var,
                                                        fitterPars.maxSBHi)
    print 'SBHicut',fitterPars.SBHicut
    
    fitterPars.minSBLo = fitterPars.minWmass - 10.
    fitterPars.maxSBLo = fitterPars.minWmass
    fitterPars.SBLocut = twoBCut
    if len(twoBCut) > 0:
        fitterPars.SBLocut += " && "
    fitterPars.SBLocut += '((%s > %f) && (%s < %f))' % (fitterPars.var,
                                                        fitterPars.minSBLo,
                                                        fitterPars.var,
                                                        fitterPars.maxSBLo)
    print 'SBLocut',fitterPars.SBLocut

    print 'minMlvjj:',minMlvjj,'maxMlvjj',maxMlvjj
    fitterPars.var = 'fit_mlvjj'
    fitterPars.minMass = minMlvjj
    fitterPars.maxMass = maxMlvjj
    fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/40 + 0.5)
    if (fitterPars.nbins < 8):
        fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/10 + \
                               0.5)
    fitterPars.truncRange = False


    twoJets = [(fitterPars.minMass, fitterPars.maxMass, alpha, 1.)]
    threeJets = [(fitterPars.minMass, fitterPars.maxMass, alpha, 1.)]

    if fitterPars.njets == 3:
        ranges = threeJets
    else:
        ranges = twoJets

    for line in ranges:
        fitterPars.minMasses.push_back(line[0])
        fitterPars.maxMasses.push_back(line[1])
        fitterPars.alphas.push_back(line[2])
        fitterPars.falphas.push_back(line[3])
##         if len(line) > 1:
##             fitterPars.falphas.push_back(line[1])

    return fitterPars


def NgenHiggs(mH, includeElectron, includeMuon):
    Ngen = {160 : 109992,
            170 : 109989,
            180 : 109325,
            190 : 109986,
            200 : 109315,
            250 : 109992,
            300 : 109990,
            350 : 109313,
            400 : 107879,
            450 : 107158,
            500 : 107169,
            550 : 107870,
            600 : 108561}

    retVal = 0
    if includeElectron:
        retVal += Ngen[mH]/2
    if includeMuon:
        retVal += Ngen[mH]/2

    return retVal

def getOptimalPars(mH, parList):
    for pars in parList:
        if mH == pars[0]:
            return pars
