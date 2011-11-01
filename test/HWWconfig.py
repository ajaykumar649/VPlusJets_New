import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.WpJDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = '/uscms_data/d1/kalanand/WjjTrees/NewReducedQCDTrees/'
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    fitterPars.DataDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.NewPhysicsDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/RD_'
    fitterPars.minMass = 50.
    fitterPars.maxMass = 120.
    fitterPars.nbins = 14
    fitterPars.truncRange = True
    fitterPars.minTrunc = 65.
    fitterPars.maxTrunc = 95.
    fitterPars.njets = Nj

    return fitterPars
