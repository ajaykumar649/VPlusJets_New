import pyroot_logon
import HWWconfig

#list of tuples that have the optimized inputs for each mass point
#tuple is (mass, mvaVariableName, mvaCut, min4bodymass, max4bodymass, nbins,
#          alpha, alphaDown, alphaUp, lowSideband, highSideband)

optimalPars2 = [
( 170, "mva2j170mu", 0.300, 170.0, 250.0, 8, 0.120, 0.100, 0.140, (55.0,65.0), (95.0,115.0) ),
( 180, "mva2j180mu", 0.600, 170.0, 250.0, 8, 0.140, 0.100, 0.180, (55.0,65.0), (95.0,115.0) ),
( 190, "mva2j190mu", 0.600, 180.0, 250.0, 7, 0.080, 0.000, 0.160, (55.0,65.0), (95.0,115.0) ),
( 200, "mva2j200mu", 0.600, 180.0, 250.0, 7, 0.210, 0.150, 0.270, (55.0,65.0), (95.0,115.0) ),
( 250, "mva2j250mu", 0.650, 200.0, 500.0, 15, 0.160, 0.090, 0.220, (55.0,65.0), (95.0,200.0) ),
( 300, "mva2j300mu", 0.600, 240.0, 500.0, 13, 0.290, 0.180, 0.410, (55.0,65.0), (95.0,200.0) ),
( 350, "mva2j350mu", 0.600, 300.0, 780.0, 12, 0.260, 0.120, 0.380, (55.0,65.0), (95.0,200.0) ),
( 400, "mva2j400mu", 0.550, 300.0, 780.0, 12, 0.130, 0.000, 0.310, (55.0,65.0), (95.0,200.0) ),
( 450, "mva2j450mu", 0.600, 300.0, 780.0, 12, 0.330, 0.180, 0.480, (55.0,65.0), (95.0,200.0) ),
( 500, "mva2j500mu", 0.500, 300.0, 780.0, 12, 0.100, 0.000, 0.260, (55.0,65.0), (95.0,200.0) ),
( 550, "mva2j550mu", 0.550, 300.0, 780.0, 12, 0.180, 0.050, 0.300, (40.0,65.0), (95.0,200.0) ),
( 600, "mva2j600mu", 0.650, 300.0, 780.0, 12, 0.000, 0.000, 0.110, (40.0,65.0), (95.0,200.0) ),
]

optimalPars3 = [
( 170, "mva3j170mu", 0.300, 170.0, 250.0, 8, 0.210, 0.150, 0.260, (55.0,65.0), (95.0,115.0) ),
( 180, "mva3j180mu", 0.300, 170.0, 250.0, 8, 0.200, 0.150, 0.260, (55.0,65.0), (95.0,115.0) ),
( 190, "mva3j190mu", 0.300, 180.0, 250.0, 7, 0.300, 0.220, 0.390, (55.0,65.0), (95.0,115.0) ),
( 200, "mva3j200mu", 0.300, 180.0, 250.0, 7, 0.290, 0.210, 0.380, (55.0,65.0), (95.0,115.0) ),
( 250, "mva3j250mu", 0.400, 200.0, 500.0, 15, 0.460, 0.350, 0.570, (55.0,65.0), (95.0,200.0) ),
( 300, "mva3j300mu", 0.600, 240.0, 500.0, 13, 0.540, 0.340, 0.820, (55.0,65.0), (95.0,200.0) ),
( 350, "mva3j350mu", 0.650, 300.0, 780.0, 12, 0.360, 0.000, 0.670, (55.0,65.0), (95.0,200.0) ),
( 400, "mva3j400mu", 0.500, 300.0, 780.0, 12, 1.000, 1.000, 1.000, (55.0,65.0), (95.0,200.0) ),
( 450, "mva3j450mu", 0.550, 300.0, 780.0, 12, 0.030, 0.000, 0.330, (55.0,65.0), (95.0,200.0) ),
( 500, "mva3j500mu", 0.650, 300.0, 780.0, 12, 1.000, 1.000, 1.000, (55.0,65.0), (95.0,200.0) ),
( 550, "mva3j550mu", 0.500, 300.0, 780.0, 12, 0.000, 0.000, 0.240, (40.0,65.0), (95.0,200.0) ),
( 600, "mva3j600mu", 0.600, 300.0, 780.0, 12, 0.590, 0.330, 0.850, (40.0,65.0), (95.0,200.0) ),
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
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False

    if mvaCut:
        mvaCutValues = mvaCut
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues)
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
    return fitterPars
