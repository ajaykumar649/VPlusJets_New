NgenHWW = {# 160 : (109992,9.080,0.133),
           170 : (199986,11.218,0.141*1.5*2, 1.0),
           180 : (197496,9.8560,0.137*1.5*2, 1.00690568528),
           190 : (194504,8.7819,0.115*1.5*2, 1.00436986424),
           200 : (199982,7.9711,0.108*1.5*2, 1.0064984894),
           250 : (194513,5.3822,0.103*1.5*2, 1.04781870103),
           300 : (197284,4.0468,0.101*1.5*2, 1.03953336721),
           350 : (199800,3.7260,0.099*1.5*2, 1.05195969977),
           400 : (199485,3.1783,0.0852*1.5*2, 1.09643113407),
           450 : (198969,2.2032,0.0808*1.5*2, 1.120898086),
           500 : (198470,1.4391,0.0799*1.5*2, 1.13138773778),
           550 : (199471,0.9364,0.0806*1.5*2, 1.13255668803),
           600 : (197170,0.67988,0.0818*1.5*2, 1.128128288),
           700 : (197291,0.29210,0.0846*1.5*2, 1.12667978),
           800 : (198750,0.15315,0.0872*1.5*2,1.12068474),
           900 : (198932,0.08848,0.0894*1.5*2,1.709855318),
           1000 : (198396,0.05562,0.0912*1.5*2,1.09438085556),
           }

NgenVBF = {160 : (169351),
           170 : (161768),
           180 : (219267),
           190 : (213014),
           200 : (217423),
           250 : (218679),
           300 : (217427),
           350 : (213673),
           400 : (204894),
           450 : (164553),
           500 : (218520),
           550 : (215938),
           600 : (214510)
           }

HiggsWidth = {
    125: 0.00407,
    160: 0.0831,
    170: 0.380,
    180: 0.631,
    190: 1.04,
    200: 1.43,
    250: 4.04,
    300: 8.43,
    350: 15.2,
    400: 29.2,
    450: 46.8,
    500: 68.0,
    550: 93.0,
    600: 123.,
    700: 199.,
    800: 304.,
    900: 449.,
    1000: 647,
    }

# modes = ['HWW', 'VBFHWW', 'HWWTauNu']
# Ngens = [NgenHWW, NgenVBFHWW, NgenHWWTauNu]
modes = ['HWW']
Ngens = [NgenHWW]

Ngen = dict(zip(modes,Ngens))

#from ROOT import RooWjjFitterUtils

# fitUtils = None

def makeSignalFilename(mH, mode, isElectron):
    
    filename = 'RD_%s_%sMH%i_CMSSW532_private.root' % \
        ('el' if isElectron else 'mu', mode, mH)

    return filename

def NgenHiggs(mH, mode):
    if type(Ngen[mode][mH]) == type(2):
        halfVal = Ngen[mode][mH]/2
    else:
        halfVal = Ngen[mode][mH][0]/2
    return Ngen[mode][mH]

def makeHiggsHist(mH, pars, mode, fitUtils = None, cpw = True, iwt = 0):

    if pars.includeMuons and pars.includeElectrons:
        modeString = ''
    elif pars.includeMuons:
        modeString = 'Muon'
    elif pars.includeElectrons:
        modeString = 'Electron'
    else:
        modeString = ''

    # if not fitUtils:
    #     fitUtils = RooWjjFitterUtils(pars)

    hist = fitUtils.newEmptyHist('%s%i_%s_shape' % (mode, mH,modeString))

    # higgsDir = '/uscmst1b_scratch/lpc1/3DayLifetime/pdudero/RD_'
    higgsDir = pars.MCDirectory

    if pars.includeMuons:
        thehist = fitUtils.File2Hist(higgsDir + \
                                         'mu_%sMH%i_CMSSW532_private.root' % \
                                         (mode, mH),
                                     '%s%i_mu' % (mode, mH), False, 1, False,
                                     1, "", cpw, iwt)
        hist.Add(thehist)
    if pars.includeElectrons:
        thehist = fitUtils.File2Hist(higgsDir + \
                                         'el_%sMH%i_CMSSW532_private.root' % \
                                         (mode, mH),
                                     '%s%i_mu' % (mode, mH), True, 1, False,
                                     1, "", cpw, iwt)
        hist.Add(thehist)

    return hist

def GenHiggsHists(pars, mH, utils = None, cpw = True):
    hists = []

    # if not utils:
    #     utils = RooWjjFitterUtils(pars)

    for mode in modes:
        tmpHist = makeHiggsHist(mH, pars, mode, utils, cpw)
        if type(Ngen[mode][mH]) == type(2):
            tmpHist.Scale(1/float(Ngen[mode][mH]/2), 'width')
        else:
            tmpHist.Scale(1/float(Ngen[mode][mH][0]*(Ngen[mode][mH][3] if cpw else 1.0)/2.), 
                          'width')            
        hists.append(tmpHist)
        
    return dict(zip(modes, hists))

from ROOT import gSystem, gROOT

# gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libMMozerpowhegweight.so")
gROOT.ProcessLine(".L CPWeighter.cc+")

from ROOT import getCPweight

def HiggsCPWeight(m_h, m, BWflag = 1):
    return getCPweight(m_h, HiggsWidth[int(m_h)], m, BWflag)

def runningWidthWeight(m_h, m):
   s = m*m
   s_h = m_h*m_h
   decay_width = HiggsWidth[int(m_h)]
   weight_BW = s/s_h * ((s-s_h)**2 + (m_h*decay_width)**2) \
       / ((s-s_h)**2 + (m_h*decay_width*s/s_h)**2);
   if (weight_BW < 0):
       print "weightVBF<0 = %f" % (weight_BW)
       weight_BW=1.0
   return weight_BW;
