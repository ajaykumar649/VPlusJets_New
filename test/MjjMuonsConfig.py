import pyroot_logon
#from ROOT import gROOT
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
import MjjConfig
#from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = MjjConfig.theConfig(Nj, mcdir, initFile, toydataFile)
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False

    return fitterPars
