import pyroot_logon
import HWWconfig

#list of tuples that have the optimized inputs for each mass point
#tuple is (mass, mvaVariableName, mvaCut, min4bodymass, max4bodymass,
#          alpha, alphaDown, alphaUp)
optimalPars2 = [
    (170, "mva2j170mu", 0.630, 170., 250., 0.130, 0.100, 0.150),
    (180, "mva2j180mu", 0.760, 170., 250., 0.000, 0.000, 0.040),
    (190, "mva2j190mu", 0.820, 170., 250., 0.010, 0.000, 0.070),
    (200, "mva2j200mu", 0.820, 170., 250., 0.000, 0.000, 0.120),
    (250, "mva2j250mu", 0.870, 200., 500., 0.050, 0.000, 0.130),
    (300, "mva2j300mu", 0.850, 200., 500., 0.170, 0.140, 0.210),
    (350, "mva2j350mu", 0.870, 260., 780., 0.150, 0.110, 0.200),
    (400, "mva2j400mu", 0.850, 280., 780., 0.220, 0.180, 0.260),
    (450, "mva2j450mu", 0.850, 280., 780., 0.210, 0.160, 0.250),
    (500, "mva2j500mu", 0.760, 280., 780., 0.200, 0.170, 0.220),
    (550, "mva2j550mu", 0.890, 280., 780., 0.120, 0.060, 0.170),
    (600, "mva2j500mu", 0.890, 260., 780., 0.280, 0.100, 0.560)
    ]

mvaVarNames3jMu  = ["mva3j170mu","mva3j180mu","mva3j190mu",
                    "mva3j200mu","mva3j250mu","mva3j300mu",
                    "mva3j350mu","mva3j400mu","mva3j450mu",
                    "mva3j500mu","mva3j550mu","mva3j600mu"]
mvaCutValues3jMu = [0.590,0.600,0.590,0.570,0.750,0.780,
                    0.790,0.770,0.760,0.720,0.680,0.780]
alphaValues3jMu  = [0.240,0.270,0.290,0.270,0.390,0.440,
                    0.150,0.170,0.420,0.430,0.240,0.360]
alphaDown3jMu    = [0.190,0.210,0.220,0.220,0.300,0.300,
                    0.050,0.090,0.290,0.350,0.150,0.280]
alphaUp3jMu      = [0.290,0.330,0.370,0.330,0.480,0.560,
                    0.280,0.250,0.540,0.510,0.350,0.440]


def theConfig(Nj, mcdir = '', initFile = '', mH=400):
    index = HiggsMassPointIndex(mH)
    mvaVarNames  = optimalPars2[index][1]
    mvaCutValues = optimalPars2[index][2]
    HWWconfig.minMlvjj = optimalPars2[index][3]
    HWWconfig.maxMlvjj = optimalPars2[index][4]
    if(Nj==3):
        mvaVarNames = mvaVarNames3jMu[index]
        mvaCutValues = mvaCutValues3jMu[index]

    fitterPars = HWWconfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False
    
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues)

    print '2-body cuts:', fitterPars.cuts
    
    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400):
    index = HiggsMassPointIndex(mH)    
    HWWconfig.minMlvjj = optimalPars2[index][3]
    HWWconfig.maxMlvjj = optimalPars2[index][4]
    alpha = optimalPars2[index][5]
    if(twoBodyConfig.njets == 3):
        alpha = alphaValues3jMu[index]   
    fitterPars = HWWconfig.the4BodyConfig(twoBodyConfig, alpha)
    return fitterPars


def HiggsMassPointIndex(mH):
    theIndex = 0
    if(mH==170): theIndex = 0
    if(mH==180): theIndex = 1
    if(mH==190): theIndex = 2
    if(mH==200): theIndex = 3
    if(mH==250): theIndex = 4
    if(mH==300): theIndex = 5
    if(mH==350): theIndex = 6
    if(mH==400): theIndex = 7
    if(mH==450): theIndex = 8
    if(mH==500): theIndex = 9
    if(mH==550): theIndex = 10
    if(mH==600): theIndex = 11
    
    return theIndex
