import pyroot_logon
import HWWconfig

mvaVarNames2jMu  = ["mva2j170mu","mva2j180mu","mva2j190mu",
                    "mva2j200mu","mva2j250mu","mva2j300mu",
                    "mva2j350mu","mva2j400mu","mva2j450mu",
                    "mva2j500mu","mva2j550mu","mva2j600mu"]
mvaCutValues2jMu = [0.630,0.760,0.820,0.820,0.870,0.850,
                    0.870,0.850,0.850,0.760,0.890,0.890]
alphaValues2jMu  = [0.130,0.000,0.010,0.000,0.050,0.170,
                    0.150,0.220,0.210,0.200,0.120,0.120]
alphaDown2jMu    = [0.100,0.000,0.000,0.000,0.000,0.140,
                    0.110,0.180,0.160,0.170,0.060,0.070]
alphaUp2jMu      = [0.150,0.040,0.070,0.120,0.130,0.210,
                    0.200,0.260,0.250,0.220,0.170,0.170]

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
    fitterPars = HWWconfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False
    
    index = HiggsMassPointIndex(mH)  
    mvaVarNames  = mvaVarNames2jMu[index]
    mvaCutValues = mvaCutValues2jMu[index]
    if(Nj==3):
        mvaVarNames = mvaVarNames3jMu[index]
        mvaCutValues = mvaCutValues3jMu[index]
    fitterPars.cuts += '&& (({0} > {1:f}))'.format(mvaVarNames,mvaCutValues)
    return fitterPars

def the4BodyConfig(Nj, mcdir = '', initFile = '', mH=400):
    index = HiggsMassPointIndex(mH)    
    alpha = alphaValues2jMu[index]
    mvaVarNames  = mvaVarNames2jMu[index]
    mvaCutValues = mvaCutValues2jMu[index]
    if(Nj==3):
        alpha = alphaValues3jMu[index]   
        mvaVarNames = mvaVarNames3jMu[index]
        mvaCutValues = mvaCutValues3jMu[index]
    fitterPars = HWWconfig.the4BodyConfig(Nj, mcdir, initFile, alpha)
    fitterPars.includeMuons = True
    fitterPars.includeElectrons = False
    fitterPars.cuts += '&& ({0} > {1:f}) '.format(mvaVarNames, mvaCutValues)
##     fitterpars.cuts += '&& (Mass2j_PFCor > {0:f})'.format(fitterPars.
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
