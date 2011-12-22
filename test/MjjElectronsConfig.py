import pyroot_logon
#from ROOT import gROOT
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
import MjjMuonsConfig
#from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = MjjMuonsConfig.theConfig(Nj, mcdir, initFile, toydataFile)
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True
    
    return fitterPars
