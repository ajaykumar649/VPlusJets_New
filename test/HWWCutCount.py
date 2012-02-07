import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

minMlvjj = 160.
maxMlvjj = 780.

optimalPars2 = [
    ( 180, 170., 250., 8, 0.220, 0., 1.,
      (55., 65.), (95.,115.) ),
    ( 250, 200., 500., 15, 0.420, 0., 1.,
      (55., 65.), (95., 115.) ),
    ( 400, 260., 780., 13, 0.260, 0., 1.,
      (45., 55.), (105.,125.) )
    ]

def theConfig(Nj, mcdir = '', initFile = '', mH = 400):
    optPars = getOptimalPars(mH, optimalPars2)
    minMlvjj = optPars[1]
    maxMlvjj = optPars[2]
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
    fitterPars.includeElectrons = False
   
    fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.minMass = 40.
    fitterPars.maxMass = 200.
    fitterPars.nbins = 30
    fitterPars.intLumi = 4700.

    
    fitterPars.binEdges.push_back(fitterPars.minMass)
    #fitterPars.binEdges.push_back(40.)
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
                      (minMlvjj, maxMlvjj) + \
                      '&& (ang_hs > -0.8) && (ang_hs < 0.8) '
    if mH == 400:
        fitterPars.cuts += '&& (ang_hb < 0.6) '

    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400, syst=0):
##     fitterPars = theConfig(Nj, mcdir, initFile)
    optPars = getOptimalPars(mH, optimalPars2)
    minMlvjj = optPars[1]
    maxMlvjj = optPars[2]

    alpha = optPars[4+syst]

    print 'alpha:',alpha
    lowSideBand = optPars[7]
    highSideBand = optPars[8]

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

    fitterPars.minSBHi = highSideBand[0]
    fitterPars.maxSBHi = highSideBand[1]
    fitterPars.SBHicut = twoBCut
    if len(twoBCut) > 0:
        fitterPars.SBHicut += " && "
    fitterPars.SBHicut += '((%s > %f) && (%s < %f))' % (fitterPars.var,
                                                        fitterPars.minSBHi,
                                                        fitterPars.var,
                                                        fitterPars.maxSBHi)
    print 'SBHicut',fitterPars.SBHicut
    
    fitterPars.minSBLo = lowSideBand[0]
    fitterPars.maxSBLo = lowSideBand[1]
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
    fitterPars.nbins = optPars[3]
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


def getOptimalPars(mH, parList):
    for pars in parList:
        if mH == pars[0]:
            return pars
