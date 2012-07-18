from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import kRed, kAzure, kGreen

def theConfig(Nj, mH, isElectron = False, initFile = ''):
    pars = Wjj2DFitterPars()

    pars.MCDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/'

    pars.isElectron = isElectron
    pars.initialParametersFile = initFile

    pars.backgrounds = ['diboson', 'WpJ', 'top']
    #signals = ['HWW']
    signals = []
    yieldConstraints = {'diboson' : 0.034, 'top' : 0.07}

    pars.cuts = '(ggdevt==%i)&&(fit_status==0)&&(W_mt>30)' % Nj

    if isElectron:
        flavorString = 'el'
   else:
        flavorString = 'mu'

   # you need a files entry and a models entry for each of the fit 
   # compoents in backgrounds and signals
   # the files should a list with entries like (filename, Ngen, xsec)
   pars.dibosonFiles = [
       (pars.MCDirectory + 'RD_%s_WW_CMSSW525.root' % (flavorString),
        10000431, 57.1097),
       (pars.MCDirectory + 'RD_%s_WZ_CMSSW525.root' % (flavorString),
        10000283, 32.3161),
       ]
   pars.dibosonModels = (-1, -1)

   pars.WpJFiles = [
       (pars.MCDirectory + 'RD_%s_WpJ_CMSSW525.root' % (flavorString),
        18393090, 36257.2),
       (pars.MCDirectory + 'RD_%s_ZpJ_CMSSW525.root' % (flavorString),
        14427282, 3503.71),
       ]
   pars.WpJModels = (-1, -1)

   pars.topFiles = [
       (pars.MCDirectory + 'RD_%s_TTbar_CMSSW525.root' % (flavorString),
        6736135, 225.197),
       (pars.MCDirectory + 'RD_%s_STopS_Tbar_CMSSW525.root' % (flavorString),
        139974, 1.75776),
       (pars.MCDirectory + 'RD_%s_STopS_T_CMSSW525.root' % (flavorString),
        259971, 3.89394),
       (pars.MCDirectory + 'RD_%s_STopT_Tbar_CMSSW525.root' % (flavorString),
        1935072, 30.0042),
       (pars.MCDirectory + 'RD_%s_STopT_T_CMSSW525.root' % (flavorString),
        23777, 55.531),
       (pars.MCDirectory + 'RD_%s_STopTW_Tbar_CMSSW525.root' % (flavorString),
        493460, 11.1773),
       (pars.MCDirectory + 'RD_%s_STopTW_T_CMSSW525.root' % (flavorString),
        497658, 11.1773),
       ]
   pars.topModels = (-1, -1)

    pars.dibosonPlotting = {'color' : kAzure+8, 'title' : 'WW/WZ'}
    pars.WpJPlotting = { 'color' : kRed, 'title' : 'V+jets'}
    pars.topPlotting = {'color' : kGreen+2, 'title' : 'top'}
    pars.HWWPlotting = {'color' : kBlue, 'title' : "H(%i)" % mH}

    pars.var1 = 'Mass2j_PFCor'
    pars.v1nbins = 12
    pars.v1min = 55.
    pars.v1max = 150.
    pars.v1binEdges = [55.,60.,65.,70.,75.,80.,85.,95.,
                       105.,115.,125.,135.,150.]

    pars.var2 = 'fit_mlvjj'
    pars.v2nbins = 20
    pars.v2min = 200.
    pars.v2max = 400.
    pars.v2binEdges = []
    
    pars.integratedLumi = 5100.

    return customizeElectrons(pars, mH) if isElectron else \
        customizeMuons(pars, mH)

def customizeElectrons(pars, mH):
    pars.DataFile = pars.MCDirectory + 'RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_5p1invfb.root'
    pars.backgrounds.append('multijet')
    return pars

def customizeMuons(pars, mH):
    pars.DataFile = pars.MCDirectory + 'RD_WmunuJets_DataAll_GoldenJSON_5p1invfb.root'

    pars.doEffCorrections = True
    pars.effToDo = ['lepton']
    pars.leptonEffFiles = {
        'id': ["EffTable2012/scaleFactor-2012A-PromptReco-v1-PFMM-RecoToIso.txt"],
        'HLT': ["EffTable2012/efficiency-2012A-PromptReco-v1-PFMM-IsoToIsoMuHLT.txt"]
        }
    pars.lumiPerEpoch = [pars.integratedLumi]
    return pars
