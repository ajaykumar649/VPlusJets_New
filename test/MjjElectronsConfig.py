#import pyroot_logon
#from ROOT import gROOT
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
import MjjConfig
#from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = MjjConfig.theConfig(Nj, mcdir, initFile, toydataFile)
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True

    fitterPars.cuts = fitterPars.cuts + \
                      '&& (JetPFCor_Pt[0] > 40) ' + \
                      '&& (JetPFCor_Pt[1] > 30) '

    return fitterPars
