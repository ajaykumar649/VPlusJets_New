import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.WpJDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewReducedQCDTrees/"
    fitterPars.DataDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.NewPhysicsDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_2p1invfb.root'
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_2p1invfb_SinglE_EleH.root'

    fitterPars.initParamsFile = initFile

    fitterPars.minMass = 50.
    fitterPars.maxMass = 120.
    fitterPars.nbins = 14
    fitterPars.truncRange = True
    fitterPars.minTrunc = 65.
    fitterPars.maxTrunc = 95.
    fitterPars.njets = Nj

    return fitterPars
