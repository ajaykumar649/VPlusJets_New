import pyroot_logon
import HWWconfig

## list of tuples that have the optimized inputs for each mass point
## tuple is (0: mass, 1: mvaVariableName, 2: mvaCut, 3: min4bodymass,
##           4: max4bodymass, 5: nbins, 6: alpha, 7: alphaDown, 8: alphaUp,
##           9: lowSideband, 10: highSideband, 11: qglCut)

optimalPars2 = [
( 170, "mva2j170el", 0.500, 170.0, 230.0, 12, 0.010, 0.000, 0.100, (50.0,65.0), (95.0,110.0), 0. ),
( 180, "mva2j180el", 0.550, 170.0, 230.0, 12, 0.00, 0.000, 0.100, (50.0,65.0), (95.0,110.0), 0. ),
( 190, "mva2j190el", 0.550, 170.0, 250.0, 16, 0.050, 0.000, 0.120, (55.0,65.0), (110.0,125.0), 0. ),
( 200, "mva2j200el", 0.600, 170.0, 250.0, 16, 0.220, 0.160, 0.280, (55.0,65.0), (95.0,115.0), 0. ),
( 250, "mva2j250el", 0.650, 220.0, 400.0, 9, 0.220, 0.150, 0.290, (55.0,65.0), (95.0,200.0), 0. ),
( 300, "mva2j300el", 0.600, 240.0, 400.0, 8, 0.440, 0.330, 0.550, (55.0,65.0), (95.0,200.0), 0. ),
( 350, "mva2j350el", 0.600, 300.0, 780.0, 12, 0.030, 0.000, 0.220, (55.0,65.0), (95.0,200.0), 0. ),
( 400, "mva2j400el", 0.550, 300.0, 780.0, 12, 0.000, 0.000, 0.260, (55.0,65.0), (95.0,200.0), 0. ),
( 450, "mva2j450el", 0.500, 340.0, 780.0, 11, 0.000, 0.000, 0.160, (55.0,65.0), (95.0,200.0), 0. ),
( 500, "mva2j500interferencenominalel", 0.500, 420.0, 740.0, 8, 0.500, 0.250, 0.720, (55.0,65.0), (95.0,200.0), 0.2 ),
( 550, "mva2j550interferencenominalel", 0.550, 420.0, 740.0, 8, 0.500, 0.250, 0.520, (40.0,65.0), (95.0,200.0), 0.35 ),
( 600, "mva2j600interferencenominalel", 0.600, 420.0, 740.0, 8, 0.500, 0.250, 0.750, (55.0,65.0), (95.0,200.0), 0.45 ),
( 700, "mva2j700interferencenominalel", 0.60, 350.0, 870.0, 13, 0.200, 0.100, 0.290, (55.0,65.0), (95.0,200.0), 0.45 ),
( 800, "mva2j800interferencenominalel", 0.60, 350.0, 990.0, 16, 0.200, 0.100, 0.290, (55.0,65.0), (95.0,200.0), 0.45 ),
( 900, "mva2j900interferencenominalel", 0.60, 470.0, 1110.0, 16, 0.200, 0.100, 0.290, (55.0,65.0), (95.0,200.0), 0.45 ),
( 1000, "mva2j1000interferencenominalel", 0.60, 470.0, 1590.0, 28, 0.200, 0.100, 0.290, (55.0,65.0), (95.0,200.0), 0.45 ),
]

optimalPars3 = [
( 170, "mva3j170el", 0.300, 170.0, 230.0, 12, 0.190, 0.000, 0.310, (55.0,65.0), (95.0,120.0), 0. ),
( 180, "mva3j180el", 0.300, 170.0, 230.0, 12, 0.190, 0.090, 0.300, (55.0,65.0), (95.0,115.0), 0. ),
( 190, "mva3j190el", 0.300, 170.0, 250.0, 16, 0.220, 0.120, 0.320, (55.0,65.0), (95.0,115.0), 0. ),
( 200, "mva3j200el", 0.400, 170.0, 250.0, 16, 0.270, 0.140, 0.410, (55.0,65.0), (95.0,115.0), 0. ),
( 250, "mva3j250el", 0.400, 220.0, 400.0, 9,  0.500, 0.410, 0.590, (55.0,65.0), (95.0,200.0), 0. ),
( 300, "mva3j300el", 0.500, 240.0, 400.0, 8,  0.660, 0.450, 0.940, (55.0,65.0), (95.0,200.0), 0. ),
( 350, "mva3j350el", 0.650, 300.0, 780.0, 12, 0.860, 0.630, 1.000, (55.0,65.0), (95.0,200.0), 0. ),
( 400, "mva3j400el", 0.600, 300.0, 780.0, 12, 0.000, 0.000, 0.190, (55.0,65.0), (95.0,200.0), 0. ),
( 450, "mva3j450el", 0.550, 340.0, 780.0, 11, 0.380, 0.080, 0.620, (55.0,65.0), (95.0,200.0), 0. ),
( 500, "mva3j500el", 0.600, 420.0, 740.0, 8, 0.430, 0.240, 0.610, (55.0,65.0), (95.0,200.0), 0.2 ),
( 550, "mva3j550el", 0.500, 420.0, 740.0, 8, 0.300, 0.100, 0.490, (40.0,65.0), (95.0,200.0), 0.35 ),
( 600, "mva3j600el", 0.600, 420.0, 740.0, 8, 0.190, 0.000, 0.500, (40.0,65.0), (95.0,200.0), 0.45 ),
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

    fitterPars.cuts += '&& (abs(JetPFCor_dphiMET[0])>0.8) ' + \
        '&& (W_electron_pt > 35) '
    fitterPars.QCDcuts = fitterPars.cuts + '&& (W_electron_pfIsoEA > 0.3) '

    if (mvaCut):
        mvaCutValues = mvaCut
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues) + \
                      '&& (fit_mlvjj > %0.1f) && (fit_mlvjj < %0.1f) ' % \
                      (optPars[3], optPars[4])
    
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
    if (mH <= 200): # and (twoBodyConfig.njets == 2):
        fitterPars.model = 7
    if (mH <= 170):
        fitterPars.model = 7
    if (mH <= 180) and (twoBodyConfig.njets == 2):
        fitterPars.model = 5
    if (mH <= 250) and (twoBodyConfig.njets == 3):
        fitterPars.model = 5

    return fitterPars
