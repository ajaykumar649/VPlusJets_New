NgenHWW = {# 160 : (109992,9.080,0.133),
           # 170 : (109989,7.729,0.141),
           # 180 : (109325,6.739,0.137),
           # 190 : (109986,5.896,0.115),
           200 : (196006,7.7641,0.108*1.5),
           # 250 : (109992,3.312,0.103),
           300 : (190924,3.9071,0.101*1.5),
           # 350 : (109313,2.306,0.099),
           400 : (166440,3.086,0.0852*1.5),
           450 : (197487,2.1268,0.0808*1.5),
           500 : (131182,1.37797,0.0799*1.5),
           # 550 : (107870,0.528,0.0806),
           600 : (42735,0.58071,0.0818*1.5)
           }

NgenVBFHWW = {160 : (169351),
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

NgenHWWTauNu = {160 : (109993),
                170 : (102459),
                180 : (105475),
                190 : (109985),
                200 : (93789),
                250 : (105936),
                300 : (102570),
                350 : (105270),
                400 : (106818),
                450 : (104259),
                500 : (104268),
                550 : (106658),
                600 : (109970)
                }

# modes = ['HWW', 'VBFHWW', 'HWWTauNu']
# Ngens = [NgenHWW, NgenVBFHWW, NgenHWWTauNu]
modes = ['HWW']
Ngens = [NgenHWW]

Ngen = dict(zip(modes,Ngens))

from ROOT import RooWjjFitterUtils

fitUtils = None

def NgenHiggs(mH, mode):
    if type(Ngen[mode][mH]) == type(2):
        halfVal = Ngen[mode][mH]/2
    else:
        halfVal = Ngen[mode][mH][0]/2
    return (halfVal, Ngen[mode][mH])

def makeHiggsHist(mH, pars, mode):
    global fitUtils

    if pars.includeMuons and pars.includeElectrons:
        modeString = ''
    elif pars.includeMuons:
        modeString = 'Muon'
    elif pars.includeElectrons:
        modeString = 'Electron'
    else:
        modeString = ''

    if not fitUtils:
        fitUtils = RooWjjFitterUtils(pars)

    hist = fitUtils.newEmptyHist('%s%i_%s_shape' % (mode, mH,modeString))

    # higgsDir = '/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree_PAT/RD_'
    higgsDir = pars.MCDirectory

    if pars.includeMuons:
        thehist = fitUtils.File2Hist(higgsDir + \
                                         'mu_%sMH%i_CMSSW525_private.root' % \
                                         (mode, mH),
                                     '%s%i_mu' % (mode, mH), False, 1, False)
        hist.Add(thehist)
    if pars.includeElectrons:
        thehist = fitUtils.File2Hist(higgsDir + \
                                         'el_%sMH%i_CMSSW525_private.root' % \
                                         (mode, mH),
                                     '%s%i_mu' % (mode, mH), True, 1, False)
        hist.Add(thehist)

    return hist

def GenHiggsHists(pars, mH):
    hists = []

    for mode in modes:
        tmpHist = makeHiggsHist(mH, pars, mode)
        if type(Ngen[mode][mH]) == type(2):
            tmpHist.Scale(1/float(Ngen[mode][mH]/2), 'width')
        else:
            tmpHist.Scale(1/float(Ngen[mode][mH][0]/2), 'width')            
        hists.append(tmpHist)
        
    return dict(zip(modes, hists))
