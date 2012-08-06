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

(opts, args) = parser.parse_args()


import pyroot_logon

#import HWW2DConfig
config = __import__(opts.modeConfig)
#import VBF2DConfig
import RooWjj2DFitter
#from RooWjj2DFitterUtils import Wjj2DFitterUtils

from ROOT import RooFit, TCanvas, RooArgSet, TFile, RooAbsReal, RooAbsData, \
    RooHist

pars = config.theConfig(Nj = opts.Nj, mH = opts.mH, 
                        isElectron = opts.isElectron, initFile = '')

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
if opts.component == 'multijet':
    weighted = False
for (ifile, (filename, ngen, xsec)) in enumerate(files):
    tmpData = fitter.utils.File2Dataset(filename, 'data%i' % ifile, fitter.ws,
                                        weighted = weighted)
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

sigPdf = fitter.makeComponentPdf(opts.component, files, models)[0][0]

fitter.ws.Print()

if fitter.ws.var('c_diboson_%s' % pars.var2):
    fitter.ws.var('c_diboson_%s' % pars.var2).setVal(-0.012)
if fitter.ws.var('c_diboson_%s_tail' % pars.var1):
    fitter.ws.var('c_diboson_%s_tail' % pars.var1).setVal(-0.015)
if fitter.ws.var('f_diboson_%s_core' % pars.var1):
    fitter.ws.var('f_diboson_%s_core' % pars.var1).setVal(0.6)
if fitter.ws.var('mean_diboson_%s_core' % pars.var1):
    fitter.ws.var('mean_diboson_%s_core' % pars.var1).setVal(86)
if fitter.ws.var('mean_diboson_%s_tail' % pars.var1):
    fitter.ws.var('mean_diboson_%s_tail' % pars.var1).setVal(40)
if fitter.ws.var('sigma_diboson_%s_core' % pars.var1):
    fitter.ws.var('sigma_diboson_%s_core' % pars.var1).setVal(10)
if fitter.ws.var('sigma_diboson_%s_tail' % pars.var1):
    fitter.ws.var('sigma_diboson_%s_tail' % pars.var1).setVal(55)
if fitter.ws.var('mean_top_%s_core' % pars.var1):
    fitter.ws.var('mean_top_%s_core' % pars.var1).setVal(84.)
if fitter.ws.var('mean_top_%s_tail' % pars.var1):
    fitter.ws.var('mean_top_%s_tail' % pars.var1).setVal(130.)
if fitter.ws.var('sigma_top_%s_core' % pars.var1):
    fitter.ws.var('sigma_top_%s_core' % pars.var1).setVal(11.)
if fitter.ws.var('sigma_top_%s_tail' % pars.var1):
    fitter.ws.var('sigma_top_%s_tail' % pars.var1).setVal(40.)
if fitter.ws.var('f_top_%s_core' % pars.var1):
    fitter.ws.var('f_top_%s_core' % pars.var1).setVal(0.25)
if fitter.ws.var('c_top_%s' % pars.var2):
    fitter.ws.var('c_top_%s' % pars.var2).setVal(-0.01)
if fitter.ws.var('c_WpJ_%s' % pars.var2):
    fitter.ws.var('c_WpJ_%s' % pars.var2).setVal(-0.014)
if fitter.ws.var('power_WpJ_%s' % pars.var1):
    fitter.ws.var('power_WpJ_%s' % pars.var1).setVal(5.1)
if fitter.ws.var('c_WpJ_%s' % pars.var1):
    fitter.ws.var('c_WpJ_%s' % pars.var1).setVal(-0.02)
if fitter.ws.var('offset_WpJ_%s' % pars.var1):
    fitter.ws.var('offset_WpJ_%s' % pars.var1).setVal(65)
if fitter.ws.var('width_WpJ_%s' % pars.var1):
    fitter.ws.var('width_WpJ_%s' % pars.var1).setVal(25)
if fitter.ws.var('c_HWW_%s_tail' % pars.var1):
    fitter.ws.var('c_HWW_%s_tail' % pars.var1).setVal(-0.015)
if fitter.ws.var('f_HWW_%s_core' % pars.var1):
    fitter.ws.var('f_HWW_%s_core' % pars.var1).setVal(0.9)
if fitter.ws.var('mean_HWW_%s_core' % pars.var1):
    fitter.ws.var('mean_HWW_%s_core' % pars.var1).setVal(85)
if fitter.ws.var('sigma_HWW_%s_core' % pars.var1):
    fitter.ws.var('sigma_HWW_%s_core' % pars.var1).setVal(10)
if fitter.ws.var('sigma_HWW_%s_tail' % pars.var1):
    fitter.ws.var('sigma_HWW_%s_tail' % pars.var1).setVal(50)
if fitter.ws.var('sigma_HWW_%s_core' % pars.var2):
    fitter.ws.var('sigma_HWW_%s_core' % pars.var2).setVal(10)
if fitter.ws.var('sigma_HWW_%s_tail' % pars.var2):
    fitter.ws.var('sigma_HWW_%s_tail' % pars.var2).setVal(35)
if fitter.ws.var('f_HWW_%s_core' % pars.var2):
    fitter.ws.var('f_HWW_%s_core' % pars.var2).setVal(0.7)
if fitter.ws.var('mean_HWW_%s' % pars.var1):
    fitter.ws.var('mean_HWW_%s' % pars.var1).setVal(84)
if fitter.ws.var('mean_HWW_%s' % pars.var2):
    fitter.ws.var('mean_HWW_%s' % pars.var2).setVal(opts.mH)
if fitter.ws.var('mean_HWW_%s_core' % pars.var2):
    fitter.ws.var('mean_HWW_%s_core' % pars.var2).setVal(opts.mH)
if fitter.ws.var('mean_HWW_%s_tail' % pars.var2):
    fitter.ws.var('mean_HWW_%s_tail' % pars.var2).setVal(opts.mH)

fr = None
fr = sigPdf.fitTo(data, RooFit.Save(), 
                   RooFit.SumW2Error(True))

c1 = TCanvas('c1', pars.var1)
sigPlot = fitter.ws.var(pars.var1).frame(RooFit.Name('%s_Plot' % pars.var1))
dataHist = RooAbsData.createHistogram(data,'dataHist_%s' % pars.var1,
                                      fitter.ws.var(pars.var1),
                                      RooFit.Binning('%sBinning' % pars.var1))
theData = RooHist(dataHist, 1., 1, RooAbsData.SumW2, 1.0, True)
theData.SetName('theData')
theData.SetTitle('data')
sigPlot.addPlotable(theData, 'pe', False, True)
sigPdf.plotOn(sigPlot)

sigPlot.GetYaxis().SetTitle('Events / GeV')

sigPlot.Draw()
c1.Update()

c2 = TCanvas('c2', pars.var2)
sigPlot2 = fitter.ws.var(pars.var2).frame(RooFit.Name('%s_Plot' % pars.var2))
data.plotOn(sigPlot2, RooFit.Binning('%sBinning' % (pars.var2)))
sigPdf.plotOn(sigPlot2)
sigPlot2.Draw()
c2.Update()

if fr:
    fr.Print('v')
    finalPars = RooArgSet(fr.floatParsFinal())

    sigFile = TFile('%s.root' % opts.bn, 'recreate')
    fr.Write('fr')
    sigPlot.Write()
    sigPlot2.Write()

    sigFile.Close()

    finalPars.setAttribAll('Constant', True)
    finalPars.writeToFile("%s.txt" % opts.bn)
