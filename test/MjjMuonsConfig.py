import pyroot_logon
#from ROOT import gROOT
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
import MjjConfig
#from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = '', implementTTbarScalingMatchingSystOption = 0, e_minT=-1, e_maxT=-1):
    fitterPars = MjjConfig.theConfig(Nj, mcdir, initFile, toydataFile, implementTTbarScalingMatchingSystOption, e_minT, e_maxT)
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False

    return fitterPars
