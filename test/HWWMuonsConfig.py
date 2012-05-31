import pyroot_logon
import HWWconfig

## list of tuples that have the optimized inputs for each mass point
## tuple is (0: mass, 1: mvaVariableName, 2: mvaCut, 3: min4bodymass,
##           4: max4bodymass, 5: nbins, 6: alpha, 7: alphaDown, 8: alphaUp,
##           9: lowSideband, 10: highSideband, 11: qglCut)

optimalPars2 = [
( 170, "mva2j170mu", 0.300, 160.0, 250.0, 18, 0.070, 0.050, 0.090, (55.0,65.0), (95.0,115.0), 0. ),
( 180, "mva2j180mu", 0.600, 160.0, 250.0, 18, 0.050, 0.000, 0.100, (55.0,65.0), (95.0,115.0), 0. ),
( 190, "mva2j190mu", 0.600, 160.0, 250.0, 18, 0.110, 0.000, 0.150, (55.0,65.0), (95.0,105.0), 0. ),
( 200, "mva2j200mu", 0.600, 160.0, 250.0, 18, 0.180, 0.120, 0.230, (55.0,65.0), (95.0,115.0), 0. ),
( 250, "mva2j250mu", 0.650, 200.0, 400.0, 10, 0.290, 0.230, 0.350, (55.0,65.0), (95.0,200.0), 0. ),
( 300, "mva2j300mu", 0.600, 240.0, 400.0, 8, 0.400, 0.280, 0.530, (55.0,65.0), (95.0,200.0), 0. ),
( 350, "mva2j350mu", 0.600, 300.0, 780.0, 12, 0.280, 0.140, 0.410, (55.0,65.0), (95.0,200.0), 0. ),
( 400, "mva2j400mu", 0.550, 300.0, 780.0, 12, 0.020, 0.000, 0.190, (55.0,65.0), (95.0,200.0), 0. ),
( 450, "mva2j450mu", 0.600, 340.0, 780.0, 11, 0.210, 0.000, 0.440, (55.0,65.0), (95.0,200.0), 0. ),
( 500, "mva2j500mu", 0.500, 340.0, 780.0, 11, 0.120, 0.000, 0.280, (55.0,65.0), (95.0,200.0), 0.1 ),
( 550, "mva2j550mu", 0.550, 340.0, 780.0, 11, 0.160, 0.070, 0.250, (40.0,65.0), (95.0,200.0), 0.2 ),
( 600, "mva2j600mu", 0.650, 340.0, 780.0, 11, 0.200, 0.100, 0.290, (40.0,65.0), (95.0,200.0), 0.45 ),
]

optimalPars3 = [
( 170, "mva3j170mu", 0.300, 160.0, 250.0, 9, 0.340, 0.270, 0.410, (55.0,65.0), (95.0,115.0), 0. ),
( 180, "mva3j180mu", 0.300, 160.0, 250.0, 9, 0.320, 0.260, 0.390, (55.0,65.0), (95.0,115.0), 0. ),
( 190, "mva3j190mu", 0.300, 160.0, 250.0, 9, 0.360, 0.250, 0.490, (55.0,65.0), (95.0,115.0), 0. ),
( 200, "mva3j200mu", 0.300, 160.0, 250.0, 9, 0.380, 0.170, 0.590, (55.0,65.0), (95.0,115.0), 0. ),
( 250, "mva3j250mu", 0.400, 200.0, 400.0, 10, 0.310, 0.230, 0.400, (55.0,65.0), (95.0,200.0), 0. ),
( 300, "mva3j300mu", 0.600, 240.0, 400.0, 8, 0.640, 0.350, 1.000, (55.0,65.0), (95.0,200.0), 0. ),
( 350, "mva3j350mu", 0.650, 300.0, 780.0, 12, 0.000, 0.000, 0.220, (55.0,65.0), (95.0,200.0), 0. ),
( 400, "mva3j400mu", 0.500, 300.0, 780.0, 12, 0.670, 0.370, 0.840, (55.0,65.0), (95.0,200.0), 0. ),
( 450, "mva3j450mu", 0.550, 340.0, 780.0, 11, 0.000, 0.000, 0.100, (55.0,65.0), (95.0,200.0), 0. ),
( 500, "mva3j500mu", 0.650, 340.0, 780.0, 11, 0.000, 0.000, 0.270, (55.0,65.0), (95.0,200.0), 0.2 ),
( 550, "mva3j550mu", 0.500, 340.0, 780.0, 11, 0.000, 0.000, 0.160, (40.0,65.0), (95.0,200.0), 0.2 ),
( 600, "mva3j600mu", 0.600, 340.0, 780.0, 11, 0.210, 0.000, 0.450, (40.0,65.0), (95.0,200.0), 0.45 ),
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

    fitterPars.cuts += '&& (abs(JetPFCor_dphiMET[0])>0.4) '
    fitterPars.QCDcuts = fitterPars.cuts

    if mvaCut:
        mvaCutValues = mvaCut
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues)

    # if not qgCut:
    #     qgCut = optPars[11]
    # fitterPars.cuts += '&& (%s > %f) ' % ("qgld_Summer11CHS[1]",qgCut)

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
    alphaDown = optPars[6+1]
    alphaUp = optPars[6+2]
    if alphaOverride:
        alpha = alphaOverride
        alphaDown = alphaOverride
        alphaUp = alphaOverride
    fitterPars = HWWconfig.the4BodyConfig(twoBodyConfig, alpha, optPars[9],
                                          optPars[10],
                                          alphaRange = (alphaDown,alphaUp))
    fitterPars.nbins = optPars[5]
    fitterPars.model = 1
    if (mH <= 200) and (twoBodyConfig.njets == 2):
        fitterPars.model = 5
    # if (mH == 190) and (twoBodyConfig.njets == 2):
    #     fitterPars.model = 1
    return fitterPars
