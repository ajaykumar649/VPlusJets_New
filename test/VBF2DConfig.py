from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import kRed, kAzure, kGreen, kBlue, kCyan, kViolet

# dictionaries of tuples keyed on the Higgs mass.  The tuple structure is
# (0: mavVariableName, 1: mvaCut, 2: min4BodyMass, 3: max4BodyMass, 
#  4: n4BodyBins)
mu2Pars = {
    170: ( "mvavbf170mu", 0.300, 165.0, 250.0, 17 ),
    180: ( "mvavbf180mu", 0.600, 165.0, 250.0, 17 ),
    190: ( "mvavbf190mu", 0.600, 165.0, 250.0, 17 ),
    200: ( "mvavbf200mu", 0.600, 165.0, 250.0, 17 ),
    250: ( "mvavbf250mu", 0.650, 200.0, 400.0, 20 ),
    300: ( "mvavbf300mu", 0.600, 240.0, 400.0, 16 ),
    # 350: ( "mvavbf350mu", 0.600, 300.0, 780.0, 24 ),
    350: ( "mvavbf350mu", 0.300, 250., 450., 20 ),
    400: ( "mvavbf400mu", 0.550, 300.0, 780.0, 24 ),
    450: ( "mvavbf450mu", 0.600, 340.0, 780.0, 22 ),
    500: ( "mvavbf500mu", 0.500, 340.0, 780.0, 22 ),
    550: ( "mvavbf550mu", 0.550, 340.0, 780.0, 22 ),
    600: ( "mvavbf600mu", 0.650, 340.0, 780.0, 22 ),
    }
el2Pars = {
    170: ( "mvavbf170el", 0.300, 165.0, 250.0, 17 ),
    180: ( "mvavbf180el", 0.550, 165.0, 250.0, 17 ),
    190: ( "mvavbf190el", 0.550, 165.0, 250.0, 17 ),
    200: ( "mvavbf200el", 0.600, 165.0, 250.0, 17 ),
    250: ( "mvavbf250el", 0.650, 200.0, 400.0, 20 ),
    300: ( "mvavbf300el", 0.600, 240.0, 400.0, 16 ),
    350: ( "mvavbf350el", 0.600, 300.0, 780.0, 24 ),
    400: ( "mvavbf400el", 0.550, 300.0, 780.0, 24 ),
    450: ( "mvavbf450el", 0.500, 340.0, 780.0, 22 ),
    500: ( "mvavbf500el", 0.500, 340.0, 780.0, 22 ),
    550: ( "mvavbf550el", 0.550, 340.0, 780.0, 22 ),
    600: ( "mvavbf600el", 0.600, 340.0, 780.0, 22 ),
    }


def theConfig(Nj = 2, mH = 350, isElectron = False, initFile = ''):
    pars = Wjj2DFitterPars()

    pars.MCDirectory = '/uscms_data/d1/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree_VBF/'

    pars.isElectron = isElectron
    pars.initialParametersFile = initFile

    pars.backgrounds = ['WpJ', 'top']
    #signals = ['HWW']
    pars.signals = []
    pars.yieldConstraints = {'top' : 0.07}

    modePars = mu2Pars
    if isElectron:
        flavorString = 'el'
        modePars = el2Pars
    else:
        flavorString = 'mu'
        modePars = mu2Pars

    pars.cuts = '(vbf_event==1)' +\
        '&&(JetPFCor_bDiscriminator[vbf_waj_id] < 1.74)' +\
        '&&(JetPFCor_bDiscriminator[vbf_wbj_id] < 1.74)' +\
        '&&(%s>%.3f)' % (modePars[mH][0], modePars[mH][1])


    # you need a files entry and a models entry for each of the fit 
    # compoents in backgrounds and signals
    # the files should a list with entries like (filename, Ngen, xsec)
    pars.dibosonFiles = [
        (pars.MCDirectory + 'RD_%s_WW_CMSSW428.root' % (flavorString),
         4225916, 47.0),
        (pars.MCDirectory + 'RD_%s_WZ_CMSSW428.root' % (flavorString),
         4265243, 18.6),
        ]
    pars.dibosonModels = (5, 0)
 
    pars.WpJFiles = [
        (pars.MCDirectory + 'RD_%s_W4Jets_CMSSW428.root' % (flavorString),
         5000700, 172.6),
        (pars.MCDirectory + 'RD_%s_ZpJ_CMSSW428.root' % (flavorString),
         14427282, 3503.71),
        ]
    pars.WpJModels = (4, 0)

    pars.topFiles = [
        (pars.MCDirectory + 'RD_%s_TTbar_CMSSW428.root' % (flavorString),
         3701947, 163.),
        (pars.MCDirectory + 'RD_%s_STopS_Tbar_CMSSW428.root' % (flavorString),
         137980, 1.44),
        (pars.MCDirectory + 'RD_%s_STopS_T_CMSSW428.root' % (flavorString),
         259971, 3.19),
        (pars.MCDirectory + 'RD_%s_STopT_Tbar_CMSSW428.root' % (flavorString),
         1944826, 22.65),
        (pars.MCDirectory + 'RD_%s_STopT_T_CMSSW428.root' % (flavorString),
         3900171, 41.92),
        (pars.MCDirectory + 'RD_%s_STopTW_Tbar_CMSSW428.root' % (flavorString),
         787629, 7.87),
        (pars.MCDirectory + 'RD_%s_STopTW_T_CMSSW428.root' % (flavorString),
         795379, 7.87),
        ]
    pars.topModels = (5, 0)

    pars.dibosonPlotting = {'color' : kAzure+8, 'title' : 'WW/WZ'}
    pars.WpJPlotting = { 'color' : kRed, 'title' : 'V+jets'}
    pars.topPlotting = {'color' : kGreen+2, 'title' : 'top'}
    pars.HWWPlotting = {'color' : kBlue, 'title' : "H(%i)" % mH}

    pars.var1 = 'vbf_wjj_m'
    pars.v1nbins = 12
    pars.v1min = 55.
    pars.v1max = 150.
    pars.v1binEdges = [55.,60.,65.,70.,75.,80.,85.,95.,
                       105.,115.,125.,135.,150.]

    pars.var2 = 'vbf_lvjj_m'
    pars.v2nbins = modePars[mH][4]
    pars.v2min = modePars[mH][2]
    pars.v2max = modePars[mH][3]
    pars.v2binEdges = []
    
    pars.integratedLumi = 5020.

    return customizeElectrons(pars) if isElectron else \
        customizeMuons(pars)

def customizeElectrons(pars):
    pars.DataFile = pars.MCDirectory + 'RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_4p7invfb.root'
    #pars.backgrounds.append('multijet')

    pars.doEffCorrections = True
    pars.effToDo = ['lepton']
    pars.leptonEffFiles = {
        'id': ["EffTableDir/eleEffsRecoToWP80_ScaleFactors.txt"],
        'reco': ["EffTableDir/eleEffsSCToReco_ScaleFactors.txt"],
        'HLT': ["EffTableDir/eleEffsSingleElectron.txt"]
        }
    pars.lumiPerEpoch = [pars.integratedLumi]

    # pars.cuts += '&&(abs(JetPFCor_dphiMET[0])>0.8)&&(W_electron_pt>35)'
    return pars

def customizeMuons(pars):
    pars.DataFile = pars.MCDirectory + 'RD_WmunuJets_DataAll_GoldenJSON_4p7invfb.root'

    pars.doEffCorrections = True
    pars.effToDo = ['lepton']
    pars.leptonEffFiles = {
        'id': ["EffTableDir/muonEffsRecoToIso_ScaleFactors.txt"],
        'HLT': ["EffTableDir/muonEffsIsoToHLT_data_LP_LWA.txt"]
        }
    pars.lumiPerEpoch = [pars.integratedLumi]

    # pars.cuts += '&&(abs(JetPFCor_dphiMET[0])>0.4)&&(abs(W_muon_eta)<2.1)'
    
    return pars
