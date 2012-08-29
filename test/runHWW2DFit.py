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
parser.add_option('--comp', dest='component', default='diboson',
                  help='name of component to fit')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
parser.add_option('--nosig', dest='includeSignal', action='store_false',
                  default=True, help='include signal shape in fit')
parser.add_option('--toy', dest='toy', action='store_true',
                  help='use pseudo-data instead of data file')
parser.add_option('--toyOut', dest='toyOut', help='filename for toy output')
parser.add_option('--seed', dest='seed', type='int', help='random seed')

(opts, args) = parser.parse_args()

import pyroot_logon

#import HWW2DConfig
config = __import__(opts.modeConfig)
import RooWjj2DFitter

from ROOT import TCanvas, RooFit, RooLinkedListIter, TMath, RooRandom

if hasattr(opts, "seed") and (opts.seed >= 0):
    print "random seed:", opts.seed
    RooRandom.randomGenerator().SetSeed(opts.seed)
pars = config.theConfig(Nj = opts.Nj, mH = opts.mH, 
                        isElectron = opts.isElectron, initFile = args,
                        includeSignal = opts.includeSignal)

fitter = RooWjj2DFitter.Wjj2DFitter(pars)

totalPdf = fitter.makeFitter()
#fitter.loadData()
fitter.readParametersFromFile()
fitter.expectedFromPars()
# fitter.WpJExpected = 25660.8
# fitter.topExpected = 3357.8
# fitter.dibosonExpected = 670.5
# fitter.HWWExpected = 225.1
fitter.resetYields()
# fitter.readParametersFromFile('WpJHWW350Parameters.txt')
# fitter.readParametersFromFile('topHWW350Parameters.txt')
# fitter.readParametersFromFile('dibosonHWW350Parameters.txt')
# fitter.readParametersFromFile('HWW350Parameters.txt')

startpars = totalPdf.getParameters(fitter.ws.set('obsSet'))
fitter.ws.defineSet("initialPars", startpars)

if opts.toy:
    #generate toy dataset
    print 'Generated parameters'
    fitter.ws.set('initialPars').Print('v')
    fitter.ws.saveSnapshot("genPars", startpars)

    data = totalPdf.generate(fitter.ws.set('obsSet'), RooFit.Name('data'),
                             RooFit.Extended())
    getattr(fitter.ws, 'import')(data)
else:    
    data = fitter.loadData()

data.Print()
startpars.IsA().Destructor(startpars)

fr = None
fr = fitter.fit()

plot1 = fitter.stackedPlot(pars.var1)
leg1 = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot1)
plot2 = fitter.stackedPlot(pars.var2)
leg2 = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot2)

c1 = TCanvas('c1', fitter.ws.var(pars.var1).GetTitle() + ' plot')
plot1.Draw()
leg1.Draw('same')
c1.Update()

c2 = TCanvas('c2', fitter.ws.var(pars.var2).GetTitle() + ' plot')
plot2.Draw()
leg2.Draw('same')
c2.Update()

ndf = 0

if fr:
    floatVars = [ fr.floatParsFinal().at(i).GetName() \
                      for i in range(0, fr.floatParsFinal().getSize()) ]
    fitter.ws.saveSnapshot("fitPars", ','.join(floatVars))
    ndf = fr.floatParsFinal().getSize() - \
        fitter.ws.set('constraintSet').getSize()
    fr.Print('v')

print '%i free parameters in the fit' % ndf

firstCurve1 = plot1.getObject(1)
# firstCurve1.Print()

chi2_1 = plot1.chiSquare(firstCurve1.GetName(), 'theData')*plot1.GetNbinsX()

firstCurve2 = plot2.getObject(1)
# firstCurve2.Print()

chi2_2 = plot2.chiSquare(firstCurve2.GetName(), 'theData')*plot2.GetNbinsX()

chi2 = chi2_1 + chi2_2
ndf = plot1.GetNbinsX()+plot2.GetNbinsX()-ndf

print 'chi2: (%.2f + %.2f)/%i = %.2f' % (chi2_1, chi2_2, ndf, (chi2/ndf))
print 'chi2 probability: %.4g' % (TMath.Prob(chi2, ndf))

if opts.toyOut:
    outFile = open(opts.toyOut, 'a', 1)
    fitter.ws.loadSnapshot("genPars")

    line = '%s %.6g %.6g %.6g '
    outFile.write('nll %f covQual %i edm %.4g ' % (fr.minNll(), fr.covQual(),
                                                   fr.edm())
                  )
    outFile.write('chi2Prob %f ' % (TMath.Prob(chi2, ndf)))
    for cycle in range(0, fr.numStatusHistory()):
        outFile.write('%s %i ' % (fr.statusLabelHistory(cycle),
                                  fr.statusCodeHistory(cycle))
                      )
    for i in range(0, fr.floatParsFinal().getSize()):
        outFile.write(line % (fr.floatParsFinal().at(i).GetName(),
                              fr.floatParsFinal().at(i).getVal(),
                              fr.floatParsFinal().at(i).getError(),
                              fitter.ws.var(fr.floatParsFinal().at(i).GetName()).getVal()
                              )
                      )
    outFile.write('\n')
    outFile.close()
