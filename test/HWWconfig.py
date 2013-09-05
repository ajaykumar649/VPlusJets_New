import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

minMlvjj = 160.
maxMlvjj = 780.

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.smoothingOrder = 1
    # fitterPars.MCDirectory = "root://cmseos.fnal.gov:1094//eos/uscms/store/user/lnujj/HCP2012METfix/ReducedTrees/RD_"
    # fitterPars.MCDirectory = "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_"

    fitterPars.MCDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/Moriond2013/ReducedTrees/RD_'
    fitterPars.WpJDirectory = fitterPars.MCDirectory
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = False
    fitterPars.QCDDirectory = fitterPars.MCDirectory[:-3]
    #fitterPars.QCDDirectory = 'root://cmseos:1094//eos/uscms/store/user/lnujj/2012_RDTrees_PAT/'
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    # fitterPars.DataDirectory = fitterPars.MCDirectory
    # fitterPars.DataDirectory = "/uscms_data/d1/andersj/Wjj/2012/data/ICHEP2012/ReducedTrees/RD_"
    fitterPars.DataDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/Moriond2013/HWWTrees/RD_'
    #fitterPars.muonData = 'SingleMu_0p56fb_Golden.root'
    #fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_5p1invfb.root'
    # fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_12p0invfb.root'
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_19p3invfb.root'
    fitterPars.includeMuons = True    
    #fitterPars.electronData = 'WenuJets_DataSingleElectron_GoldenJSON_2012_xxpxinvfb.root'
    #fitterPars.electronData = 'WenuJets_DataAllSingleElectronTrigger_GoldenJSON_5p1invfb.root'
    fitterPars.electronData = 'WenuJets_DataAllSingleElectronTrigger_GoldenJSON_11p9invfb.root'
    fitterPars.includeElectrons = True
   
    fitterPars.NewPhysicsDirectory = fitterPars.MCDirectory
    fitterPars.minMass = 45.
    #fitterPars.minMass = 50.
    fitterPars.maxMass = 200.
    fitterPars.nbins = 17
    fitterPars.intLumi = 19200.
    # fitterPars.intLumi = 3500.
    # fitterPars.intLumi = 539.385+21.297
    # fitterPars.intLumi = 297.848+21.297+48.465
    
    fitterPars.binEdges.push_back(fitterPars.minMass)
    #fitterPars.binEdges.push_back(40.)
    #fitterPars.binEdges.push_back(45.)
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
    fitterPars.externalConstraints = False
    
    fitterPars.doEffCorrections = True
    fitterPars.muIdEffFiles.push_back("EffTable2012/scaleFactor-Run2012ABC-RecoToIso.txt")
    fitterPars.muHLTEffFiles.push_back("EffTable2012/efficiency-Run2012ABC-IsoToIsoMuHLT.txt")
    fitterPars.lumiPerEpochMuon.push_back(fitterPars.intLumi)

    
    fitterPars.eleIdEffFiles.push_back("EffTable2012/scaleFactor-Run2012ABC-GsfElectronToId.txt")
    fitterPars.eleRecoEffFiles.push_back("EffTable2012/scaleFactor-Run2012ABC-SCToElectron.txt")
    fitterPars.eleHLTEffFiles.push_back("EffTable2012/efficiency-Run2012ABC-WP80ToHLTEle.txt")
    fitterPars.eleJ30EffFiles.push_back("EffTableDir/FullyEfficient.txt")
    fitterPars.eleJ25NoJ30EffFiles.push_back("EffTableDir/FullyEfficient_Jet2NoJet1.txt")
    fitterPars.eleMHTEffFiles.push_back("EffTableDir/FullyEfficient_MHT.txt")
    fitterPars.eleWMtEffFiles.push_back("EffTableDir/FullyEfficient.txt")
    fitterPars.lumiPerEpochElectron.push_back(fitterPars.intLumi)

    fitterPars.constrainDiboson = True
    fitterPars.useExternalMorphingPars = False

    jetCut = '(ggdevt == %i)' % (Nj)
    if (Nj < 2):
        jetCut = '((ggdevt == 2) || (ggdevt == 3))'

    fitterPars.cuts = '(fit_status==0) ' + \
                      '&& %s ' % (jetCut) + \
                      '&& (W_mt > 30) ' 

    return fitterPars

def the4BodyConfig(twoBodyConfig, alpha=1., lowSideBand = (55., 65.),
                   highSideBand = (95., 115), alphaRange = (0., 1.)):
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
    fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/40 + 0.5)
    if (fitterPars.nbins < 8):
        fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/10 + \
                               0.5)
    fitterPars.truncRange = False


    ranges = [(fitterPars.minMass, fitterPars.maxMass, alpha, 1., alphaRange)]

    for line in ranges:
        fitterPars.minMasses.push_back(line[0])
        fitterPars.maxMasses.push_back(line[1])
        fitterPars.alphas.push_back(line[2])
        fitterPars.falphas.push_back(line[3])
        fitterPars.alphaDowns.push_back(line[4][0])
        fitterPars.alphaUps.push_back(line[4][1])
##         if len(line) > 1:
##             fitterPars.falphas.push_back(line[1])

    return fitterPars


def getOptimalPars(mH, parList):
    for pars in parList:
        if mH == pars[0]:
            return pars
