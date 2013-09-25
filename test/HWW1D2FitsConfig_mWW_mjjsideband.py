from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import kRed, kAzure, kGreen, kBlue, kCyan, kViolet, kGray
import HWWSignalShapes
import HWW1D2FitsConfig
from HWW1D2FitsConfig import mu2Pars, el2Pars

def theConfig(**kwargs):
    # (Nj, mH, isElectron = False, initFile = [], includeSignal = True,
    #           MVACutOverride = None):
    pars_mjj = HWW1D2FitsConfig.theConfig(**kwargs)
    pars = Wjj2DFitterPars()

    pars.MCDirectory = pars_mjj.MCDirectory
    pars.DataDirectory = pars_mjj.DataDirectory
    pars.DataFile = pars_mjj.DataFile
    pars.integratedLumi = pars_mjj.integratedLumi
    pars.effToDo = pars_mjj.effToDo
    pars.leptonEffFiles = pars_mjj.leptonEffFiles
    pars.lumiPerEpoch = pars_mjj.lumiPerEpoch

    pars.isElectron = kwargs['isElectron']
    pars.initialParametersFile = kwargs['initFile']

    pars.backgrounds = pars_mjj.backgrounds
    pars.includeSignal = kwargs['includeSignal']
    pars.signals = pars_mjj.signals
    pars.yieldConstraints = pars_mjj.yieldConstraints
    #pars.yieldConstraints = {}
    pars.constrainShapes = []

    pars.Njets = kwargs['Nj']
    pars.mHiggs = kwargs['mH']

    modePars = mu2Pars
    if isElectron:
        flavorString = 'el'
        if pars.Njets == 3:
            modePars = el3Pars
        else:
            modePars = el2Pars
    else:
        flavorString = 'mu'
        if pars.Njets == 3:
            modePars = mu3Pars
        else:
            modePars = mu2Pars

    pars.btagVeto = pars_mjj.btagVeto
    
    regionLow = pars_mjj.exclude[pars_mjj.var[0]][0]
    regionHigh = pars_mjj.exclude[pars_mjj.var[0]][1]
    pars.cuts = pars_mjj.cuts
    pars.cuts += '&&(%s>%.0f)&&(%s<%.0f)' % \
        (pars_mjj.var[0], regionLow,
         pars_mjj.var[0], regionHigh)

    pars.dibosonFiles = pars_mjj.dibosonFiles
    pars.dibosonModels = [ modePars[pars.mHiggs][5]['diboson'][1] ]
 
    pars.WpJFiles = pars_mjj.WpJFiles
    pars.WpJModels = [ modePars[pars.mHiggs][5]['WpJ'][1] ]
    if (len(modePars[pars.mHiggs][5]['WpJ']) > 3):
        pars.WpJAuxModels = [ modePars[pars.mHiggs][5]['WpJ'][3] ]

    pars.topFiles = pars_mjj.topFiles
    pars.topModels = [ modePars[pars.mHiggs][5]['top'][1] ]

    # ngen = HWWSignalShapes.NgenHiggs(pars.mHiggs, 'ggH')
    pars.ggHFiles = pars_mjj.ggHFiles
    pars.ggHModels = [ modePars[pars.mHiggs][5]['ggH'][1] ]
    pars.ggHdoSystMult = False
    if pars.mHiggs >= 400:
        pars.ggHInterference = True
        pars.ggHSystMult = 'interf_ggH'

    # ngen = HWWSignalShapes.NgenHiggs(pars.mHiggs, 'qqH')
    pars.qqHFiles = pars_mjj.qqHFiles
    pars.qqHModels = [ modePars[pars.mHiggs][5]['qqH'][1] ]

    pars.QCDFiles = pars_mjj.QCDFiles
    pars.QCDModels = pars_mjj.QCDModels
    pars.QCDFracOfData = pars_mjj.QCDFracOfData

    pars.dibosonPlotting = pars_mjj.dibosonPlotting
    pars.WpJPlotting = pars_mjj.WpJPlotting
    pars.topPlotting = pars_mjj.topPlotting
    pars.ggHPlotting = pars_mjj.ggHPlotting
    pars.qqHPlotting = pars_mjj.qqHPlotting

    pars.var = ['fit_mlvjj', 'Mass2j_PFCor']
    pars.varRanges = pars_mjj.varRanges
    pars.plotRanges = pars_mjj.plotRanges
    pars.varTitles = pars_mjj.varTitles
    pars.exclude = pars_mjj.exclude
    pars.doExclude = False
    pars.blind = pars_mjj.blind

    pars.binData = pars_mjj.binData
    pars.binData = True

    return pars
