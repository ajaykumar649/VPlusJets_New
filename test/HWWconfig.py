import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = "/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_"
    fitterPars.WpJDirectory = "/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_"
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = "/uscms_data/d1/kalanand/WjjTrees/NewReducedQCDTrees/"
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    fitterPars.DataDirectory = "/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_"
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_2p1invfb.root'
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_2p1invfb_SinglE_EleH.root'
    fitterPars.NewPhysicsDirectory = "/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_"
    fitterPars.minMass = 50.
    fitterPars.maxMass = 120.
    fitterPars.nbins = 14
    fitterPars.truncRange = True
    fitterPars.minTrunc = 65.
    fitterPars.maxTrunc = 95.
    fitterPars.minWmass = fitterPars.minTrunc
    fitterPars.maxWmass = fitterPars.maxTrunc
    fitterPars.njets = Nj

    return fitterPars

def the4BodyConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = theConfig(Nj, mcdir, initFile)

    fitterPars.do4body = True
    twoBCut = fitterPars.cuts
    print twoBCut
    if len(fitterPars.cuts) > 0:
        fitterPars.cuts += " && "
    fitterPars.cuts += '(({0} > {1:f}) && ({0} < {2:f}))'.format(fitterPars.var,fitterPars.minTrunc, fitterPars.maxTrunc)

    fitterPars.minSBHi = fitterPars.maxWmass
    fitterPars.maxSBHi = fitterPars.maxWmass + 10.
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

    fitterPars.var = 'fi2_mlvjj'
    fitterPars.minMass = 140.
    fitterPars.maxMass = 800.
    fitterPars.nbins = int((fitterPars.maxMass-fitterPars.minMass)/10 + 0.5)
    fitterPars.truncRange = False

    twoJets = [(140., 220., 0., 0.326313),
               (220., 400., 0.2, 0.589244),
               (400., 800., 0.13)]
    threeJets = [(140., 220., 0.06, 0.250198),
                 (220., 400., 0.28, 0.605075),
                 (400., 800., 0.22)]

    if Nj == 3:
        ranges = threeJets
    else:
        ranges = twoJets

    for line in ranges:
        fitterPars.minMasses.push_back(line[0])
        fitterPars.maxMasses.push_back(line[1])
        fitterPars.alphas.push_back(line[2])
        if len(line) > 3:
            fitterPars.falphas.push_back(line[3])

    return fitterPars
