import pyroot_logon

import VBF2DConfig
import RooWjj2DFitter

from ROOT import TCanvas

pars = VBF2DConfig.theConfig(Nj = 2, mH = 350, isElectron = False, 
                             initFile = '')

fitter = RooWjj2DFitter.Wjj2DFitter(pars)

fitter.makeFitter()
fitter.loadData()
fitter.WpJExpected = 383.6
fitter.topExpected = 194.6 + 12.
fitter.dibosonExpected = 7.4
fitter.resetYields()
fitter.readParametersFromFile('WpJShapeParameters.txt')
fitter.readParametersFromFile('ttbarShapeParameters.txt')
fitter.ws.Print()

fitter.fit()

plot1 = fitter.stackedPlot(pars.var1)
plot2 = fitter.stackedPlot(pars.var2)

c1 = TCanvas('c1', pars.var1)
plot1.Draw()
c1.Update()

c2 = TCanvas('c2', pars.var2)
plot2.Draw()
c2.Update()
