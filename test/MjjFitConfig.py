import pyroot_logon
## from ROOT import gROOT
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
import MjjOptimizeConfig
from ROOT import RooWjjFitterParams


def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = MjjOptimizeConfig.theConfig(Nj, mcdir, initFile)
    fitterPars.truncRange = False

    return fitterPars
