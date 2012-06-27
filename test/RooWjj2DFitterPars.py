class Wjj2DFitterPars:
    # filenames and paths
    MCDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/'

    DataFile = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/RD_WmunuJets_DataAll_GoldenJSON_3p5invfb.root'

    treeName = 'WJet'
    isElectron = False

    #backgrounds = ['WpJ', 'ttbar', 'st', 'multijet', 'diboson']
    #signals = ['HWW']

    # cuts
    cuts = '(ggdevt==2)&&(fit_status==0)&&(W_mt>30)&&(abs(W_muon_eta)<2.1)'
    QCDcuts = ''

    # variables
    var1 = 'Mass2j_PFCor'
    v1nbins = 13
    v1min = 50.
    v1max = 150.
    v1binEdges = [50.,55.,60.,65.,70.,75.,80.,85.,95.,105.,115.,125.,135.,150.]

    var2 = 'fit_mlvjj'
    v2nbins = 20
    v2min = 200.
    v2max = 400.
    v2binEdges = []
    
    # lumi
    integratedLumi = 3500.

    # fit control
    fitmin = -999
    fitmax = -999

    # efficiency corrections
    doEffCorrections = False
    effToDo = ['lepton', 'met', 'mt']
    leptonEffFiles = {}
    #jetEffFiles = {}
    metEffFiles = {}
    mtEffFiles = {}
    lumiPerEpoch = []

import copy
def copyPars(pars):
    newPars = Wjj2DFitterPars()
    if type(pars) == type(newPars):
        newPars.__dict__ = copy.deepcopy(pars.__dict__)
    return newPars
