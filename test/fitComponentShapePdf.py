from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-m', '--mode', default="HWW2DConfig", dest='modeConfig',
                  help='which config to select look at HWW2DConfig.py for ' +\
                      'an example.  Use the file name minus the .py extension.'
                  )
parser.add_option('-H', '--mH', dest='mH', default=350, type='int',
                  help='Higgs Mass Point')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('--bn', dest='bn', default='ShapeParameters',
                  help='basis name for the output files.')
parser.add_option('--comp', dest='component', default='diboson',
                  help='name of component to fit')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
parser.add_option('--makeFree', dest='makeConstant', action='store_false',
                  default=True, help='make parameters free in output')

(opts, args) = parser.parse_args()


import pyroot_logon

#import HWW2DConfig
config = __import__(opts.modeConfig)
#import VBF2DConfig
import RooWjj2DFitter
import HWWSignalShapes
#from RooWjj2DFitterUtils import Wjj2DFitterUtils

from ROOT import RooFit, TCanvas, RooArgSet, TFile, RooAbsReal, RooAbsData, \
    RooHist, TMath

pars = config.theConfig(Nj = opts.Nj, mH = opts.mH, 
                        isElectron = opts.isElectron, initFile = args)

files = getattr(pars, '%sFiles' % opts.component)
models = getattr(pars, '%sModels' % opts.component)

# print 'eff lumi WW: %.1f invpb' % (pars.dibosonFiles[0][1]/pars.dibosonFiles[0][2])
# print 'eff lumi WZ: %.1f invpb' % (pars.dibosonFiles[1][1]/pars.dibosonFiles[1][2])

#pars.DataFile = files[0][0]
fitter = RooWjj2DFitter.Wjj2DFitter(pars)

#fitter.makeFitter()
#data = fitter.loadData(True)
#data.Print()
data = None
sumNExp = 0.
weighted = True
cutOverride = None
if opts.component == 'multijet':
    weighted = False
    cutOverride = pars.multijet_cuts
cpw = False
if opts.component == "ggH":
    cpw = True
for (ifile, (filename, ngen, xsec)) in enumerate(files):
    tmpData = fitter.utils.File2Dataset(filename, 'data%i' % ifile, fitter.ws,
                                        weighted = weighted, CPweight = cpw,
                                        cutOverride = cutOverride)
    tmpData.Print()
    expectedYield = xsec*pars.integratedLumi*tmpData.sumEntries()/ngen
    print filename,'A x eff: %.3g' % (tmpData.sumEntries()/ngen)
    print filename,'expected yield: %.1f' % expectedYield
    sumNExp += expectedYield
    if not data:
        scale = 1.0
        data = fitter.utils.reduceDataset(tmpData, scale)
        refEffLumi = ngen/xsec
    else:
        scale = refEffLumi/(ngen/xsec)
        reducedData = fitter.utils.reduceDataset(tmpData, scale)
        data.append(reducedData)

print opts.component,'total expected yield: %.1f' % sumNExp

sigPdf = fitter.makeComponentPdf(opts.component, files, models)

if fitter.ws.var('c_diboson_%s' % pars.var[1]):
    fitter.ws.var('c_diboson_%s' % pars.var[1]).setVal(-0.012)
if fitter.ws.var('c_diboson_%s_tail' % pars.var[0]):
    fitter.ws.var('c_diboson_%s_tail' % pars.var[0]).setVal(-0.015)
if fitter.ws.var('f_diboson_%s_core' % pars.var[0]):
    fitter.ws.var('f_diboson_%s_core' % pars.var[0]).setVal(0.6)
if fitter.ws.var('mean_diboson_%s_core' % pars.var[0]):
    fitter.ws.var('mean_diboson_%s_core' % pars.var[0]).setVal(86)
if fitter.ws.var('mean_diboson_%s_tail' % pars.var[0]):
    fitter.ws.var('mean_diboson_%s_tail' % pars.var[0]).setVal(40)
if fitter.ws.var('sigma_diboson_%s_core' % pars.var[0]):
    fitter.ws.var('sigma_diboson_%s_core' % pars.var[0]).setVal(10)
if fitter.ws.var('sigma_diboson_%s_tail' % pars.var[0]):
    fitter.ws.var('sigma_diboson_%s_tail' % pars.var[0]).setVal(55)
if fitter.ws.var('mean_top_%s_core' % pars.var[0]):
    fitter.ws.var('mean_top_%s_core' % pars.var[0]).setVal(84.)
if fitter.ws.var('mean_top_%s_tail' % pars.var[0]):
    fitter.ws.var('mean_top_%s_tail' % pars.var[0]).setVal(130.)
if fitter.ws.var('sigma_top_%s_core' % pars.var[0]):
    fitter.ws.var('sigma_top_%s_core' % pars.var[0]).setVal(11.)
if fitter.ws.var('sigma_top_%s_tail' % pars.var[0]):
    fitter.ws.var('sigma_top_%s_tail' % pars.var[0]).setVal(40.)
if fitter.ws.var('f_top_%s_core' % pars.var[0]):
    fitter.ws.var('f_top_%s_core' % pars.var[0]).setVal(0.25)
if fitter.ws.var('c_top_%s' % pars.var[1]):
    fitter.ws.var('c_top_%s' % pars.var[1]).setVal(-0.01)
if fitter.ws.var('c_WpJ_%s' % pars.var[1]):
    fitter.ws.var('c_WpJ_%s' % pars.var[1]).setVal(-0.014)
if fitter.ws.var('power_WpJ_%s' % pars.var[0]):
    fitter.ws.var('power_WpJ_%s' % pars.var[0]).setVal(5.1)
if fitter.ws.var('c_WpJ_%s' % pars.var[0]):
    fitter.ws.var('c_WpJ_%s' % pars.var[0]).setVal(-0.02)
if fitter.ws.var('offset_WpJ_%s' % pars.var[0]):
    fitter.ws.var('offset_WpJ_%s' % pars.var[0]).setVal(65)
if fitter.ws.var('width_WpJ_%s' % pars.var[0]):
    fitter.ws.var('width_WpJ_%s' % pars.var[0]).setVal(25)
if fitter.ws.var('c_ggH_%s_tail' % pars.var[0]):
    fitter.ws.var('c_ggH_%s_tail' % pars.var[0]).setVal(-0.015)
if fitter.ws.var('f_ggH_%s_core' % pars.var[0]):
    fitter.ws.var('f_ggH_%s_core' % pars.var[0]).setVal(0.9)
if fitter.ws.var('mean_ggH_%s_core' % pars.var[0]):
    fitter.ws.var('mean_ggH_%s_core' % pars.var[0]).setVal(85)
if fitter.ws.var('sigma_ggH_%s_core' % pars.var[0]):
    fitter.ws.var('sigma_ggH_%s_core' % pars.var[0]).setVal(10)
if fitter.ws.var('sigma_ggH_%s_tail' % pars.var[0]):
    fitter.ws.var('sigma_ggH_%s_tail' % pars.var[0]).setVal(50)
if fitter.ws.var('sigma_ggH_%s_core' % pars.var[1]):
    fitter.ws.var('sigma_ggH_%s_core' % pars.var[1]).setVal(10)
if fitter.ws.var('sigma_ggH_%s_tail' % pars.var[1]):
    fitter.ws.var('sigma_ggH_%s_tail' % pars.var[1]).setVal(100)
if fitter.ws.var('f_ggH_%s_core' % pars.var[1]):
    fitter.ws.var('f_ggH_%s_core' % pars.var[1]).setVal(0.7)
if fitter.ws.var('mean_ggH_%s' % pars.var[0]):
    fitter.ws.var('mean_ggH_%s' % pars.var[0]).setVal(84)
if fitter.ws.var('mean_ggH_%s' % pars.var[1]):
    fitter.ws.var('mean_ggH_%s' % pars.var[1]).setVal(opts.mH)
if fitter.ws.var('mean_ggH_%s_core' % pars.var[1]):
    fitter.ws.var('mean_ggH_%s_core' % pars.var[1]).setVal(opts.mH)
if fitter.ws.var('mean_ggH_%s_tail' % pars.var[1]):
    fitter.ws.var('mean_ggH_%s_tail' % pars.var[1]).setVal(opts.mH)
if fitter.ws.var('width_ggH_%s' % pars.var[1]):
    fitter.ws.var('width_ggH_%s' % pars.var[1]).setVal(HWWSignalShapes.HiggsWidth[opts.mH])
if fitter.ws.var('resolution_ggH_%s_tail' % pars.var[1]):
    fitter.ws.var('resolution_ggH_%s_tail' % pars.var[1]).setVal(opts.mH*0.11)

params = sigPdf.getParameters(data)
parCopy = params.snapshot()
for filename in args:
    parCopy.readFromFile(filename)
    params.assignValueOnly(parCopy)

parCopy.IsA().Destructor(parCopy)
    
if fitter.ws.var('npow_ggH_Mass2j_PFCor'):
    # fitter.ws.var('npow_ggH_Mass2j_PFCor').setConstant(False)
    # fitter.ws.var('alpha_ggH_Mass2j_PFCor').setVal(1.0)
    # fitter.ws.var('alpha_ggH_Mass2j_PFCor').setConstant(True)
    pass

if fitter.ws.var('npow_diboson_Mass2j_PFCor'):
    fitter.ws.var('npow_diboson_Mass2j_PFCor').setConstant(True)
    fitter.ws.var('npow_diboson_Mass2j_PFCor').setError(0.5)

fitter.ws.Print()


fr = None
fr = sigPdf.fitTo(data, RooFit.Save(), 
                  RooFit.SumW2Error(False)
                  )

c1 = TCanvas('c1', pars.var[0])
sigPlot = fitter.ws.var(pars.var[0]).frame(RooFit.Name('%s_Plot' % pars.var[0]))
dataHist = RooAbsData.createHistogram(data,'dataHist_%s' % pars.var[0],
                                      fitter.ws.var(pars.var[0]),
                                      RooFit.Binning('%sBinning' % pars.var[0]))
theData = RooHist(dataHist, 1., 1, RooAbsData.SumW2, 1.0, True)
theData.SetName('theData')
theData.SetTitle('data')
sigPlot.addPlotable(theData, 'pe', False, True)
sigPdf.plotOn(sigPlot, RooFit.Name('fitCurve'))

sigPlot.GetYaxis().SetTitle('Events / GeV')

sigPlot.Draw()
c1.Update()

c2 = TCanvas('c2', pars.var[1])
sigPlot2 = fitter.ws.var(pars.var[1]).frame(RooFit.Name('%s_Plot' % pars.var[1]))
data.plotOn(sigPlot2, RooFit.Binning('%sBinning' % (pars.var[1])),
            RooFit.Name('theData'))
sigPdf.plotOn(sigPlot2, RooFit.Name('fitCurve'))
sigPlot2.Draw()
c2.Update()

chi2_1 = sigPlot.chiSquare('fitCurve', 'theData')*sigPlot.GetNbinsX()
chi2_2 = sigPlot2.chiSquare('fitCurve', 'theData')*sigPlot2.GetNbinsX()
ndf = 0

if fr:
    fr.Print('v')
    finalPars = params.snapshot()

    sigFile = TFile('%s.root' % opts.bn, 'recreate')
    fr.Write('fr')
    sigPlot.Write()
    sigPlot2.Write()
    fitter.ws.Write()

    sigFile.Close()

    if opts.makeConstant:
        finalPars.setAttribAll('Constant', True)
    finalPars.writeToFile("%s.txt" % opts.bn)

    if opts.component != 'multijet':
        paramsFile = open('%s.txt' % opts.bn, 'a')
        paramsFile.write('n_%s = %.1f +/- %.1f C\n' % (opts.component, sumNExp, 
                                                       TMath.Sqrt(sumNExp))
                         )
        paramsFile.close()

    ndf = fr.floatParsFinal().getSize()
    finalPars.IsA().Destructor(finalPars)

params.IsA().Destructor(params)

print '%i free parameters in the fit' % ndf
chi2 = chi2_1 + chi2_2
ndf = sigPlot.GetNbinsX()+sigPlot2.GetNbinsX()-ndf

print 'chi2: (%.2f + %.2f)/%i = %.2f' % (chi2_1, chi2_2, ndf, (chi2/ndf))
print 'chi2 probability: %.4g' % (TMath.Prob(chi2, ndf))
