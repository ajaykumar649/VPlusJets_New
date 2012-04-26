import pyroot_logon
import VBFHWWConfig

## list of tuples that have the optimized inputs for each mass point
## tuple is (0: mass, 1: mvaVariableName, 2: mvaCut, 3: min4bodymass,
##           4: max4bodymass, 5: nbins, 6: alpha, 7: alphaDown, 8: alphaUp,
##           9: lowSideband, 10: highSideband, 11: qglCut)

optimalPars2 = [
( 170, "mva2j170el", 0.300, 170.0, 250.0, 8, 0.070, 0.050, 0.090, (55.0,65.0), (95.0,115.0), 0. ),
( 180, "mva2j180el", 0.600, 170.0, 250.0, 8, 0.050, 0.000, 0.100, (55.0,65.0), (95.0,115.0), 0. ),
( 190, "mva2j190el", 0.600, 170.0, 250.0, 8, 0.110, 0.000, 0.150, (55.0,65.0), (95.0,105.0), 0. ),
( 200, "mva2j200el", 0.600, 180.0, 250.0, 7, 0.180, 0.120, 0.230, (55.0,65.0), (95.0,115.0), 0. ),
( 250, "mva2j250el", 0.650, 200.0, 400.0, 10, 0.290, 0.230, 0.350, (55.0,65.0), (95.0,200.0), 0. ),
( 300, "mvavbf300el", 0.3, 280.0, 720.0, 11, 0.0, 0.0, 0.50, (55.0,65.0), (95.0,160.), 0. ),
( 350, "mva2j350el", 0.600, 300.0, 780.0, 12, 0.280, 0.140, 0.410, (55.0,65.0), (95.0,200.0), 0. ),
( 400, "mva2j400el", 0.550, 300.0, 780.0, 12, 0.020, 0.000, 0.190, (55.0,65.0), (95.0,200.0), 0. ),
( 450, "mva2j450el", 0.600, 340.0, 780.0, 11, 0.210, 0.000, 0.440, (55.0,65.0), (95.0,200.0), 0. ),
( 500, "mvavbf500el", 0.2, 280.0, 720.0, 11, 0.0, 0.0, 0.50, (55.0,65.0), (95.0,160.), 0. ),
( 550, "mva2j550el", 0.550, 340.0, 780.0, 11, 0.160, 0.070, 0.250, (40.0,65.0), (95.0,200.0), 0.2 ),
( 600, "mva2j600el", 0.650, 340.0, 780.0, 11, 0.200, 0.100, 0.290, (40.0,65.0), (95.0,200.0), 0.45 ),
]

def theConfig(Nj, mcdir = '', initFile = '', mH=400, mvaCut = None,
              qgCut = None):

    optPars = VBFHWWConfig.getOptimalPars(mH, optimalPars2)
    
    mvaVarNames  = optPars[1]
    mvaCutValues = optPars[2]
    VBFHWWConfig.minMlvjj = optPars[3]
    VBFHWWConfig.maxMlvjj = optPars[4]

    fitterPars = VBFHWWConfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True

    if mvaCut:
        mvaCutValues = mvaCut
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues)
    # if not qgCut:
    #     qgCut = optPars[11]
    # fitterPars.cuts += '&& (%s > %f) ' % ("qgld_Summer11CHS[1]",qgCut)

    print '2-body cuts:', fitterPars.cuts
    
    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400, syst=0, alphaOverride = None):
    optPars = VBFHWWConfig.getOptimalPars(mH, optimalPars2)
    VBFHWWConfig.minMlvjj = optPars[3]
    VBFHWWConfig.maxMlvjj = optPars[4]
    alpha = optPars[6+syst]
    alphaDown = optPars[6+1]
    alphaUp = optPars[6+2]
    if alphaOverride:
        alpha = alphaOverride
        alphaDown = alphaOverride
        alphaUp = alphaOverride
    fitterPars = VBFHWWConfig.the4BodyConfig(twoBodyConfig, alpha, optPars[9],
                                          optPars[10],
                                          alphaRange = (alphaDown,alphaUp))
    fitterPars.nbins = optPars[5]
    fitterPars.model = 1
    if (mH <= 200) and (twoBodyConfig.njets == 2):
        fitterPars.model = 4
    if (mH == 190) and (twoBodyConfig.njets == 2):
        fitterPars.model = 1
    return fitterPars
