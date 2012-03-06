import pyroot_logon
import HWWconfig

#list of tuples that have the optimized inputs for each mass point
#tuple is (mass, mvaVariableName, mvaCut, min4bodymass, max4bodymass, nbins,
#          alpha, alphaDown, alphaUp, lowSideband, highSideband)

optimalPars2 = [
( 170, "mva2j170el", 0.300, 170.0, 250.0, 8, 0.110, 0.070, 0.140, (55.0,65.0), (95.0,115.0) ),
( 180, "mva2j180el", 0.550, 170.0, 250.0, 8, 0.140, 0.080, 0.200, (55.0,65.0), (95.0,115.0) ),
( 190, "mva2j190el", 0.550, 180.0, 250.0, 7, 0.200, 0.090, 0.310, (55.0,65.0), (95.0,115.0) ),
( 200, "mva2j200el", 0.600, 180.0, 250.0, 7, 0.280, 0.170, 0.390, (55.0,65.0), (95.0,115.0) ),
( 250, "mva2j250el", 0.650, 200.0, 500.0, 15, 0.220, 0.140, 0.310, (55.0,65.0), (95.0,200.0) ),
( 300, "mva2j300el", 0.600, 240.0, 500.0, 13, 0.130, 0.000, 0.270, (55.0,65.0), (95.0,200.0) ),
( 350, "mva2j350el", 0.600, 300.0, 780.0, 12, 0.000, 0.000, 0.080, (55.0,65.0), (95.0,200.0) ),
( 400, "mva2j400el", 0.550, 300.0, 780.0, 12, 0.000, 0.000, 0.050, (55.0,65.0), (95.0,200.0) ),
( 450, "mva2j450el", 0.500, 300.0, 780.0, 12, 0.000, 0.000, 0.050, (55.0,65.0), (95.0,200.0) ),
( 500, "mva2j500el", 0.500, 300.0, 780.0, 12, 0.000, 0.000, 0.060, (55.0,65.0), (95.0,200.0) ),
( 550, "mva2j550el", 0.550, 300.0, 780.0, 12, 0.000, 0.000, 0.050, (40.0,65.0), (95.0,200.0) ),
( 600, "mva2j600el", 0.600, 300.0, 780.0, 12, 0.000, 0.000, 0.030, (40.0,65.0), (95.0,200.0) ),
]

optimalPars3 = [
( 170, "mva3j170el", 0.300, 170.0, 250.0, 8, 0.140, 0.050, 0.230, (55.0,65.0), (95.0,115.0) ),
( 180, "mva3j180el", 0.300, 170.0, 250.0, 8, 0.170, 0.080, 0.260, (55.0,65.0), (95.0,115.0) ),
( 190, "mva3j190el", 0.300, 180.0, 250.0, 7, 0.140, 0.020, 0.240, (55.0,65.0), (95.0,115.0) ),
( 200, "mva3j200el", 0.400, 180.0, 250.0, 7, 0.000, 0.000, 0.150, (55.0,65.0), (95.0,115.0) ),
( 250, "mva3j250el", 0.400, 200.0, 500.0, 15, 0.480, 0.390, 0.590, (55.0,65.0), (95.0,200.0) ),
( 300, "mva3j300el", 0.500, 240.0, 500.0, 13, 0.420, 0.290, 0.580, (55.0,65.0), (95.0,200.0) ),
( 350, "mva3j350el", 0.650, 300.0, 780.0, 12, 0.070, 0.000, 0.440, (55.0,65.0), (95.0,200.0) ),
( 400, "mva3j400el", 0.600, 300.0, 780.0, 12, 0.000, 0.000, 0.140, (55.0,65.0), (95.0,200.0) ),
( 450, "mva3j450el", 0.550, 300.0, 780.0, 12, 0.650, 0.290, 0.960, (55.0,65.0), (95.0,200.0) ),
( 500, "mva3j500el", 0.600, 300.0, 780.0, 12, 0.760, 0.440, 0.990, (55.0,65.0), (95.0,200.0) ),
( 550, "mva3j550el", 0.500, 300.0, 780.0, 12, 0.510, 0.280, 0.730, (40.0,65.0), (95.0,200.0) ),
( 600, "mva3j600el", 0.600, 300.0, 780.0, 12, 1.000, 1.000, 1.000, (40.0,65.0), (95.0,200.0) ),
]

def theConfig(Nj, mcdir = '', initFile = '', mH=400, mvaCut = None,
              qgCut = None):
    if Nj == 3:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars3)
    else:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars2)
    
    mvaVarNames  = optPars[1]
    mvaCutValues = optPars[2]
    HWWconfig.minMlvjj = optPars[3]
    HWWconfig.maxMlvjj = optPars[4]

    fitterPars = HWWconfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True

    if (mvaCut):
        mvaCutValues = mvaCut
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues) + \
                       '&& (W_electron_pt > 35) '
    if qgCut:
        fitterPars.cuts += '&& (%s > %f) ' % ("qgld_Summer11CHS[0]*qgld_Summer11CHS[1]",qgCut)

    print '2-body cuts:', fitterPars.cuts

    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400, syst=0, alphaOverride = None):
    if twoBodyConfig.njets == 3:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars3)
    else:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars2)
    HWWconfig.minMlvjj = optPars[3]
    HWWconfig.maxMlvjj = optPars[4]
    alpha = optPars[6+syst]
    if alphaOverride:
        alpha = alphaOverride
    fitterPars = HWWconfig.the4BodyConfig(twoBodyConfig, alpha, optPars[9],
                                          optPars[10])
    fitterPars.nbins = optPars[5]
    fitterPars.model = 1
    if (mH==190) and (twoBodyConfig.njets == 2):
        fitterPars.model = 4
##     if (mH==180) and (twoBodyConfig.njets == 2):
##         fitterPars.model = 5

    return fitterPars
