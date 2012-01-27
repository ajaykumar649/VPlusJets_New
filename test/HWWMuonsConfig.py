import pyroot_logon
import HWWconfig

#list of tuples that have the optimized inputs for each mass point
#tuple is (mass, mvaVariableName, mvaCut, min4bodymass, max4bodymass, nbins,
#          alpha, alphaDown, alphaUp)
optimalPars2 = [
    ( 170, "mva2j170mu", 0.630, 170., 250., 8, 0.170, 0.130, 0.220,
      (55., 65.), (95.,115.) ),
    ( 180, "mva2j180mu", 0.720, 170., 250., 8, 0.180, 0.120, 0.230,
      (55., 65.), (95.,115.) ),
    ( 190, "mva2j190mu", 0.770, 170., 250., 8, 0.030, 0.000, 0.170,
      (55., 65.), (95.,115.) ),
    ( 200, "mva2j200mu", 0.800, 170., 250., 8, 0.330, 0.250, 0.410,
      (55., 65.), (95.,115.) ),
    ( 250, "mva2j250mu", 0.830, 200., 500., 15, 0.170, 0.010, 0.310,
      (55., 65.), (95.,115.) ),
    ( 300, "mva2j300mu", 0.850, 200., 500., 15, 0.220, 0.120, 0.330,
      (55., 65.), (95.,115.) ),
    ( 350, "mva2j350mu", 0.870, 260., 780., 13, 0.040, 0.000, 0.240,
      (55., 65.), (95.,115.) ),
    ( 400, "mva2j400mu", 0.850, 260., 780., 13, 0.290, 0.130, 0.430,
      (50., 65.), (100.,115.) ),
    ( 450, "mva2j450mu", 0.850, 260., 780., 13, 0.090, 0.000, 0.270,
      (55., 65.), (95.,115.) ),
    ( 500, "mva2j500mu", 0.760, 260., 780., 13, 0.220, 0.030, 0.340,
      (55., 65.), (95.,115.) ),
    ( 550, "mva2j550mu", 0.890, 260., 780., 13, 0.280, 0.050, 0.470,
      (50., 65.), (100.,115.) ),
    ( 600, "mva2j600mu", 0.890, 260., 780., 13, 0.130, 0.000, 0.400,
      (55., 65.), (95.,115.) )
    ]

optimalPars3 = [
    (170, "mva3j170mu", 0.590, 170., 250., 8, 0.130, 0.050, 0.230,
      (55., 65.), (95.,115.) ),
    (180, "mva3j180mu", 0.600, 170., 250., 8, 0.460, 0.290, 0.670,
      (55., 65.), (95.,115.) ),
    (190, "mva3j190mu", 0.590, 170., 250., 8, 0.510, 0.370, 0.680,
      (55., 65.), (95.,115.) ),
    (200, "mva3j200mu", 0.570, 170., 250., 8, 0.380, 0.300, 0.470,
      (55., 65.), (95.,115.) ),
    (250, "mva3j250mu", 0.750, 200., 500., 15, 0.400, 0.260, 0.560,
      (55., 65.), (95.,115.) ),
    (300, "mva3j300mu", 0.780, 200., 500., 15, 0.500, 0.280, 0.700,
      (55., 65.), (95.,115.) ),
    (350, "mva3j350mu", 0.790, 260., 780., 13, 0.850, 0.540, 1.000,
      (50., 65.), (100.,115.) ),
    (400, "mva3j400mu", 0.770, 260., 780., 13, 0.020, 0.000, 0.330,
      (50., 65.), (100.,115.) ),
    (450, "mva3j450mu", 0.760, 260., 780., 13, 0.630, 0.180, 0.990,
      (50., 65.), (100.,115.) ),
    (500, "mva3j500mu", 0.720, 260., 780., 13, 0.210, 0.000, 0.990,
      (50., 65.), (100.,115.) ),
    (550, "mva3j550mu", 0.680, 260., 780., 13, 0.020, 0.000, 0.250,
      (45., 65.), (95.,105.) ),
    (600, "mva3j600mu", 0.780, 260., 780., 13, 0.170, 0.000, 0.470,
      (55., 65.), (95.,115.) )
    ]

def theConfig(Nj, mcdir = '', initFile = '', mH=400):

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
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues)

    print '2-body cuts:', fitterPars.cuts
    
    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400, syst=0):
    if twoBodyConfig.njets == 3:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars3)
    else:
        optPars = HWWconfig.getOptimalPars(mH, optimalPars2)
    HWWconfig.minMlvjj = optPars[3]
    HWWconfig.maxMlvjj = optPars[4]
    alpha = optPars[6+syst]
    fitterPars = HWWconfig.the4BodyConfig(twoBodyConfig, alpha, optPars[9],
                                          optPars[10])
    fitterPars.nbins = optPars[5]
    return fitterPars
