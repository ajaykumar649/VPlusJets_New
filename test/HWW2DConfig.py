from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import kRed, kAzure, kGreen, kBlue, kCyan, kViolet

# dictionaries of tuples keyed on the Higgs mass.  The tuple structure is
# (0: mavVariableName, 1: mvaCut, 2: min4BodyMass, 3: max4BodyMass, 
#  4: n4BodyBins)
mu2Pars = {
    170: ( "mva2j170mu", 0.300, 165.0, 250.0, 17 ),
    180: ( "mva2j180mu", 0.600, 165.0, 250.0, 17 ),
    190: ( "mva2j190mu", 0.600, 165.0, 250.0, 17 ),
    200: ( "mva2j200mu", 0.600, 165.0, 250.0, 17 ),
    250: ( "mva2j250mu", 0.650, 200.0, 400.0, 20 ),
    300: ( "mva2j300mu", 0.600, 240.0, 400.0, 16 ),
    # 350: ( "mva2j350mu", 0.600, 300.0, 780.0, 24 ),
    350: ( "mva2j350mu", 0.600, 250., 450., 20 ),
    400: ( "mva2j400mu", 0.550, 300.0, 780.0, 24 ),
    450: ( "mva2j450mu", 0.600, 340.0, 780.0, 22 ),
    500: ( "mva2j500mu", 0.500, 340.0, 780.0, 22 ),
    550: ( "mva2j550mu", 0.550, 340.0, 780.0, 22 ),
    600: ( "mva2j600mu", 0.650, 340.0, 780.0, 22 ),
    }
mu3Pars = {
    170: ( "mva3j170mu", 0.300, 165.0, 250.0, 17 ),
    180: ( "mva3j180mu", 0.300, 165.0, 250.0, 17 ),
    190: ( "mva3j190mu", 0.300, 165.0, 250.0, 17 ),
    200: ( "mva3j200mu", 0.300, 165.0, 250.0, 17 ),
    250: ( "mva3j250mu", 0.400, 200.0, 400.0, 20 ),
    300: ( "mva3j300mu", 0.600, 240.0, 400.0, 16 ),
    350: ( "mva3j350mu", 0.650, 300.0, 780.0, 24 ),
    400: ( "mva3j400mu", 0.500, 300.0, 780.0, 24 ),
    450: ( "mva3j450mu", 0.550, 340.0, 780.0, 22 ),
    500: ( "mva3j500mu", 0.650, 340.0, 780.0, 22 ),
    550: ( "mva3j550mu", 0.500, 340.0, 780.0, 22 ),
    600: ( "mva3j600mu", 0.600, 340.0, 780.0, 22 ),
    }
el2Pars = {
    170: ( "mva2j170el", 0.300, 165.0, 250.0, 17 ),
    180: ( "mva2j180el", 0.550, 165.0, 250.0, 17 ),
    190: ( "mva2j190el", 0.550, 165.0, 250.0, 17 ),
    200: ( "mva2j200el", 0.600, 165.0, 250.0, 17 ),
    250: ( "mva2j250el", 0.650, 200.0, 400.0, 20 ),
    300: ( "mva2j300el", 0.600, 240.0, 400.0, 16 ),
    350: ( "mva2j350el", 0.600, 300.0, 780.0, 24 ),
    400: ( "mva2j400el", 0.550, 300.0, 780.0, 24 ),
    450: ( "mva2j450el", 0.500, 340.0, 780.0, 22 ),
    500: ( "mva2j500el", 0.500, 340.0, 780.0, 22 ),
    550: ( "mva2j550el", 0.550, 340.0, 780.0, 22 ),
    600: ( "mva2j600el", 0.600, 340.0, 780.0, 22 ),
    }
el3Pars = {
    170: ( "mva3j170el", 0.300, 165.0, 250.0, 17 ),
    180: ( "mva3j180el", 0.300, 165.0, 250.0, 17 ),
    190: ( "mva3j190el", 0.300, 165.0, 250.0, 17 ),
    200: ( "mva3j200el", 0.400, 165.0, 250.0, 17 ),
    250: ( "mva3j250el", 0.400, 200.0, 400.0, 20 ),
    300: ( "mva3j300el", 0.500, 240.0, 400.0, 16 ),
    350: ( "mva3j350el", 0.650, 300.0, 780.0, 24 ),
    400: ( "mva3j400el", 0.600, 300.0, 780.0, 24 ),
    450: ( "mva3j450el", 0.550, 340.0, 780.0, 22 ),
    500: ( "mva3j500el", 0.600, 340.0, 780.0, 22 ),
    550: ( "mva3j550el", 0.500, 340.0, 780.0, 22 ),
    600: ( "mva3j600el", 0.600, 340.0, 780.0, 22 ),
    }


def theConfig(Nj, mH, isElectron = False, initFile = ''):
    pars = Wjj2DFitterPars()

    pars.MCDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/'

    pars.isElectron = isElectron
    pars.initialParametersFile = initFile

    pars.backgrounds = ['diboson', 'WpJ', 'top']
    #signals = ['HWW']
    signals = []
    yieldConstraints = {'diboson' : 0.034, 'top' : 0.07}

    modePars = mu2Pars
    if isElectron:
        flavorString = 'el'
        if Nj == 3:
            modePars = el3Pars
        else:
            modePars = el2Pars
    else:
        flavorString = 'mu'
        if Nj == 3:
            modePars = mu3Pars
        else:
            modePars = mu2Pars

    pars.cuts = '(ggdevt==%i)&&(fit_status==0)&&(W_mt>30)' % Nj + \
        '&&(%s>%.3f)' % (modePars[mH][0], modePars[mH][1])

    # you need a files entry and a models entry for each of the fit 
    # compoents in backgrounds and signals
    # the files should a list with entries like (filename, Ngen, xsec)
    pars.dibosonFiles = [
        (pars.MCDirectory + 'RD_%s_WW_CMSSW525.root' % (flavorString),
         10000431, 57.1097),
        (pars.MCDirectory + 'RD_%s_WZ_CMSSW525.root' % (flavorString),
         10000283, 32.3161),
        ]
    pars.dibosonModels = (5, 0)
 
    pars.WpJFiles = [
        (pars.MCDirectory + 'RD_%s_WpJ_CMSSW525.root' % (flavorString),
         18393090, 36257.2),
        (pars.MCDirectory + 'RD_%s_ZpJ_CMSSW525.root' % (flavorString),
         14427282, 3503.71),
        ]
    pars.WpJModels = (21, 0)

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
    pars.topModels = (5, 0)

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
    pars.v2nbins = modePars[mH][4]
    pars.v2min = modePars[mH][2]
    pars.v2max = modePars[mH][3]
    pars.v2binEdges = []
    
    pars.integratedLumi = 5100.

    return customizeElectrons(pars) if isElectron else \
        customizeMuons(pars)

def customizeElectrons(pars):
    pars.DataFile = pars.MCDirectory + 'RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_5p1invfb.root'
    #pars.backgrounds.append('multijet')

    pars.doEffCorrections = True
    pars.effToDo = ['lepton']
    pars.leptonEffFiles = {
        'id': ["EffTable2012/scaleFactor-2012A-PromptReco-v1-GsfElectronToId.txt"],
        'reco': ["EffTable2012/scaleFactor-2012A-PromptReco-v1-SCToElectron.txt"],
        'HLT': ["EffTable2012/efficiency-2012A_PromptReco_v1-WP80ToHLTEle.txt"]
        }
    pars.lumiPerEpoch = [pars.integratedLumi]

    pars.cuts += '&&(abs(JetPFCor_dphiMET[0])>0.8)&&(W_electron_pt>35)'
    return pars

def customizeMuons(pars):
    pars.DataFile = pars.MCDirectory + 'RD_WmunuJets_DataAll_GoldenJSON_5p1invfb.root'

    pars.doEffCorrections = True
    pars.effToDo = ['lepton']
    pars.leptonEffFiles = {
        'id': ["EffTable2012/scaleFactor-2012A-PromptReco-v1-PFMM-RecoToIso.txt"],
        'HLT': ["EffTable2012/efficiency-2012A-PromptReco-v1-PFMM-IsoToIsoMuHLT.txt"]
        }
    pars.lumiPerEpoch = [pars.integratedLumi]

    pars.cuts += '&&(abs(JetPFCor_dphiMET[0])>0.4)&&(abs(W_muon_eta)<2.1)'
    
    return pars
