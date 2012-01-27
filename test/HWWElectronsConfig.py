import pyroot_logon
import HWWconfig

#list of tuples that have the optimized inputs for each mass point
#tuple is (mass, mvaVariableName, mvaCut, min4bodymass, max4bodymass, nbins,
#          alpha, alphaDown, alphaUp)
optimalPars2 = [
    (170, "mva2j170el", 0.570, 170., 250., 8, 0.110, 0.060, 0.170,
      (55., 65.), (95.,115.) ),
##     (180, "mva2j180el", 0.720, 170., 250., 8, 0.080, 0.000, 0.150,
##       (45., 65.), (105.,115.) ),
    (180, "mva2j180el", 0.720, 170., 250., 8, 0.00, 0.000, 0.150,
      (55., 65.), (95.,115.) ),
    (190, "mva2j190el", 0.760, 170., 250., 8, 0.060, 0.000, 0.170,
      (45., 65.), (105.,115.) ),
    (200, "mva2j200el", 0.790, 170., 250., 8, 0.330, 0.230, 0.430,
      (55., 65.), (95.,115.) ),
    (250, "mva2j250el", 0.780, 200., 500., 15, 0.560, 0.460, 0.660,
      (55., 65.), (95.,115.) ),
    (300, "mva2j300el", 0.800, 200., 500., 15, 0.370, 0.280, 0.450,
      (55., 65.), (95.,115.) ),
    (350, "mva2j350el", 0.820, 260., 780., 13, 0.450, 0.240, 0.680,
      (55., 65.), (95.,115.) ),
    (400, "mva2j400el", 0.800, 260., 780., 13, 0.650, 0.420, 0.820,
      (55., 65.), (95.,115.) ),
    (450, "mva2j450el", 0.770, 260., 780., 13, 0.830, 0.710, 0.930,
      (55., 65.), (95.,115.) ),
    (500, "mva2j500el", 0.750, 260., 780., 13, 0.420, 0.240, 0.560,
      (55., 65.), (105.,115.) ),
    (550, "mva2j550el", 0.820, 260., 780., 13, 0.770, 0.580, 0.910,
      (55., 65.), (95.,115.) ),
    (600, "mva2j600el", 0.820, 260., 780., 13, 0.820, 0.610, 1.000,
      (55., 65.), (95.,115.) )
    ]

optimalPars3 = [
    (170, "mva3j170el", 0.390, 170., 250., 8, 0.120, 0.020, 0.220,
      (55., 65.), (95.,115.) ),
    (180, "mva3j180el", 0.510, 170., 250., 8, 0.180, 0.050, 0.330,
      (55., 65.), (95.,115.) ),
    (190, "mva3j190el", 0.490, 170., 250., 8, 0.200, 0.050, 0.360,
      (55., 65.), (95.,115.) ),
    (200, "mva3j200el", 0.500, 170., 250., 8, 0.440, 0.300, 0.610,
      (55., 65.), (95.,115.) ),
    (250, "mva3j250el", 0.720, 200., 500., 15, 0.500, 0.330, 0.680,
      (55., 65.), (95.,115.) ),
    (300, "mva3j300el", 0.750, 200., 500., 15, 0.190, 0.060, 0.330,
      (55., 65.), (95.,115.) ),
    (350, "mva3j350el", 0.750, 260., 780., 13, 0.990, 0.300, 1.000,
      (55., 65.), (95.,115.) ),
    (400, "mva3j400el", 0.670, 260., 780., 13, 0.240, 0.000, 0.480,
      (50., 65.), (100.,115.) ),
    (450, "mva3j450el", 0.630, 260., 780., 13, 0.130, 0.000, 0.410,
      (50., 65.), (100.,115.) ),
    (500, "mva3j500el", 0.580, 260., 780., 13, 0.600, 0.400, 0.850,
      (50., 65.), (100.,115.) ),
    (550, "mva3j550el", 0.720, 260., 780., 13, 0.340, 0.070, 0.560,
      (55., 65.), (95.,115.) ),
    (600, "mva3j600el", 0.740, 260., 780., 13, 0.990, 0.830, 1.000,
      (50., 65.), (100.,115.) )
    ]

## mvaVarNames2jEl  = ["mva2j170el","mva2j180el","mva2j190el",
##                     "mva2j200el","mva2j250el","mva2j300el",
##                     "mva2j350el","mva2j400el","mva2j450el",
##                     "mva2j500el","mva2j550el","mva2j600el"]
## mvaCutValues2jEl = [0.720,0.720,0.810,0.790,0.780,0.800,
##                     0.820,0.800,0.770,0.750,0.820,0.820]
## alphaValues2jEl  = [0.170,0.080,0.150,0.220,0.260,0.250,
##                     0.260,0.280,0.290,0.230,0.200,0.270]
## alphaDown2jEl    = [0.120,0.040,0.080,0.170,0.220,0.220,
##                     0.220,0.240,0.250,0.200,0.160,0.230]
## alphaUp2jEl      = [0.230,0.130,0.220,0.280,0.300,0.280,
##                     0.300,0.310,0.320,0.260,0.240,0.310]


## mvaVarNames3jEl  = ["mva3j170el","mva3j180el","mva3j190el",
##                     "mva3j200el","mva3j250el","mva3j300el",
##                     "mva3j350el","mva3j400el","mva3j450el",
##                     "mva3j500el","mva3j550el","mva3j600el"]
## mvaCutValues3jEl = [0.390,0.510,0.490,0.500,0.720,0.750,
##                     0.750,0.670,0.630,0.580,0.720,0.740]
## alphaValues3jEl  = [0.320,0.320,0.320,0.320,0.350,0.260,
##                     0.390,0.470,0.360,0.440,0.450,0.400]
## alphaDown3jEl    = [0.280,0.270,0.260,0.260,0.240,0.170,
##                     0.300,0.380,0.290,0.360,0.370,0.310]
## alphaUp3jEl      = [0.360,0.380,0.380,0.380,0.470,0.380,
##                     0.490,0.560,0.430,0.510,0.550,0.510]



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
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues) + \
                       '&& (W_electron_pt > 35) '

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
    fitterPars.model = 1

    return fitterPars
