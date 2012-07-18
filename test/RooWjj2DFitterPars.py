class Wjj2DFitterPars:
    # filenames and paths
    MCDirectory = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/'

    DataFile = '/uscms_data/d2/andersj/Wjj/2012/data/RDTrees_PAT/RD_WmunuJets_DataAll_GoldenJSON_3p5invfb.root'

    treeName = 'WJet'
    isElectron = False
    order = 0

    initialParametersFile = ''

    backgrounds = []
    signals = []
    yieldConstraints = {}

    # cuts
    cuts = '(ggdevt==2)&&(fit_status==0)&&(W_mt>30)&&(abs(W_muon_eta)<2.1)'
    QCDcuts = ''

    # variables
    var1 = 'Mass2j_PFCor'
    v1nbins = 12
    v1min = 55.
    v1max = 150.
    v1binEdges = [55.,60.,65.,70.,75.,80.,85.,95.,105.,115.,125.,135.,150.]

    var2 = 'fit_mlvjj'
    v2nbins = 20
    v2min = 200.
    v2max = 400.
    v2binEdges = []
    
    # lumi
    integratedLumi = 3500.

    # efficiency corrections
    doEffCorrections = True
    effToDo = ['lepton']
    leptonEffFiles = {
        'id': ["EffTable2012/scaleFactor-2012A-PromptReco-v1-PFMM-RecoToIso.txt"],
        'HLT': ["EffTable2012/efficiency-2012A-PromptReco-v1-PFMM-IsoToIsoMuHLT.txt"]
        }
    lumiPerEpoch = [integratedLumi]

import copy
def copyPars(pars):
    newPars = Wjj2DFitterPars()
    if type(pars) == type(newPars):
        newPars.__dict__ = copy.deepcopy(pars.__dict__)
    return newPars
