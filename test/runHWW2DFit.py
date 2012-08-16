import pyroot_logon

import HWW2DConfig
import RooWjj2DFitter

from ROOT import TCanvas, RooFit, RooLinkedListIter, TMath

pars = HWW2DConfig.theConfig(Nj = 2, mH = 350, isElectron = False, 
                             initFile = '')

fitter = RooWjj2DFitter.Wjj2DFitter(pars)

totalPdf = fitter.makeFitter()
#fitter.loadData()
fitter.WpJExpected = 25660.8
fitter.topExpected = 3357.8
fitter.dibosonExpected = 670.5
fitter.HWWExpected = 225.1
fitter.resetYields()
fitter.readParametersFromFile('WpJHWWShapeParameters.txt')
fitter.readParametersFromFile('topHWWShapeParameters.txt')
fitter.readParametersFromFile('dibosonHWWShapeParameters.txt')
fitter.readParametersFromFile('sigHWWShapeParameters.txt')

# #generate toy dataset
# data = totalPdf.generate(fitter.ws.set('obsSet'), RooFit.Name('data'),
#                          RooFit.Extended())
# data.Print()
# getattr(fitter.ws, 'import')(data)

data = fitter.loadData()

data.Print()

fr = None
fr = fitter.fit()

plot1 = fitter.stackedPlot(pars.var1)
plot2 = fitter.stackedPlot(pars.var2)

c1 = TCanvas('c1', fitter.ws.var(pars.var1).GetTitle() + ' plot')
plot1.Draw()
c1.Update()

c2 = TCanvas('c2', fitter.ws.var(pars.var2).GetTitle() + ' plot')
plot2.Draw()
c2.Update()

ndf = 0

if fr:
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
