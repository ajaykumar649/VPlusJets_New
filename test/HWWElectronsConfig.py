import pyroot_logon
import HWWconfig

mvaVarNames2jEl  = ["mva2j170el","mva2j180el","mva2j190el",
                    "mva2j200el","mva2j250el","mva2j300el",
                    "mva2j350el","mva2j400el","mva2j450el",
                    "mva2j500el","mva2j550el","mva2j600el"]
mvaCutValues2jEl = [0.720,0.720,0.810,0.790,0.780,0.800,
                    0.820,0.800,0.770,0.750,0.820,0.820]
alphaValues2jEl  = [0.170,0.080,0.150,0.220,0.260,0.250,
                    0.260,0.280,0.290,0.230,0.200,0.270]
alphaDown2jEl    = [0.120,0.040,0.080,0.170,0.220,0.220,
                    0.220,0.240,0.250,0.200,0.160,0.230]
alphaUp2jEl      = [0.230,0.130,0.220,0.280,0.300,0.280,
                    0.300,0.310,0.320,0.260,0.240,0.310]


mvaVarNames3jEl  = ["mva3j170el","mva3j180el","mva3j190el",
                    "mva3j200el","mva3j250el","mva3j300el",
                    "mva3j350el","mva3j400el","mva3j450el",
                    "mva3j500el","mva3j550el","mva3j600el"]
mvaCutValues3jEl = [0.390,0.510,0.490,0.500,0.720,0.750,
                    0.750,0.670,0.630,0.580,0.720,0.740]
alphaValues3jEl  = [0.320,0.320,0.320,0.320,0.350,0.260,
                    0.390,0.470,0.360,0.440,0.450,0.400]
alphaDown3jEl    = [0.280,0.270,0.260,0.260,0.240,0.170,
                    0.300,0.380,0.290,0.360,0.370,0.310]
alphaUp3jEl      = [0.360,0.380,0.380,0.380,0.470,0.380,
                    0.490,0.560,0.430,0.510,0.550,0.510]



def theConfig(Nj, mcdir = '', initFile = '', mH=400):
    fitterPars = HWWconfig.theConfig(Nj, mcdir, initFile)
    fitterPars.includeMuons = False
    fitterPars.includeElectrons = True
    
    index = HiggsMassPointIndex(mH)  
    mvaVarNames  = mvaVarNames2jEl[index]
    mvaCutValues = mvaCutValues2jEl[index]
    if(Nj==3):
        mvaVarNames = mvaVarNames3jEl[index]
        mvaCutValues = mvaCutValues3jEl[index]
    fitterPars.cuts += '&& (%s > %f) ' % (mvaVarNames,mvaCutValues) + \
                       '&& (W_electron_pt > 35) '
    return fitterPars

def the4BodyConfig(twoBodyConfig, mH=400):
    index = HiggsMassPointIndex(mH)    
    alpha = alphaValues2jEl[index]
    if(Nj==3):
        alpha = alphaValues3jEl[index]
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
