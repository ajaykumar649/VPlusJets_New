#! /usr/bin/env python

#######################################################
## ---> Run using the command line like: 
##       python runHWWFitter.py -m HWWMuonsConfig -j 2 -H 300
######################################################


from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='HWWNominal2Jets.txt',
                  help='File to use as the initial template')
parser.add_option('-d', '--dir', dest='mcdir', default='',
                  help='directory to pick up the W+jets shapes')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select look at HWWconfig.py for an '+ \
                  'example.  Use the file name minus the .py extension.')
parser.add_option('-H', '--mH', dest='mH', default=400, type='int',
                  help='Higgs Mass Point')
(opts, args) = parser.parse_args()

import pyroot_logon

config = __import__(opts.modeConfig)

#import HWWwideSideband

from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L EffTableReader.cc+')
gROOT.ProcessLine('.L EffTableLoader.cc+')
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit, TLatex, TMatrixDSym, RooArgList, RooArgSet, gPad
from math import sqrt

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
fitterPars = config.theConfig(opts.Nj, opts.mcdir, opts.startingFile, opts.mH)

if fitterPars.includeMuons and fitterPars.includeElectrons:
    modeString = ''
elif fitterPars.includeMuons:
    modeString = 'Muon'
elif fitterPars.includeElectrons:
    modeString = 'Electron'
else:
    modeString = ''


theFitter = RooWjjMjjFitter(fitterPars)

#theFitter.getWorkSpace().Print()
theFitter.makeFitter(False)

#theFitter.getWorkSpace().Print()

fr = theFitter.fit()
chi2 = Double(0.)
#ndf = Long(2)
ndf = Long(3)
theFitter.computeChi2(chi2, ndf)
mf = theFitter.stackedPlot()
sf = theFitter.residualPlot(mf, "h_background", "dibosonPdf", False)
pf = theFitter.residualPlot(mf, "h_total", "", True)
lf = theFitter.stackedPlot(True)

l = TLatex()
l.SetNDC()
l.SetTextSize(0.035);
l.SetTextFont(42);

c1 = TCanvas("c1", "stacked")
mf.Draw()
l.DrawLatex(0.22, 0.85,
            '#chi^{{2}}/dof = {0:0.3f}/{1} = {2:0.3f}'.format(chi2, ndf,
                                                              chi2/ndf)
            )
pyroot_logon.cmsPrelim(c1, fitterPars.intLumi/1000)
c1.Print('H{2}_Mjj_{0}_{1}jets_Stacked.pdf'.format(modeString, opts.Nj, opts.mH))
c1.Print('H{2}_Mjj_{0}_{1}jets_Stacked.png'.format(modeString, opts.Nj, opts.mH))

c2 = TCanvas("c2", "stacked_log")
c2.SetLogy()
lf.Draw()
pyroot_logon.cmsPrelim(c2, fitterPars.intLumi/1000)
c2.Print('H{2}_Mjj_{0}_{1}jets_Stacked_log.pdf'.format(modeString, opts.Nj, opts.mH))
c2.Print('H{2}_Mjj_{0}_{1}jets_Stacked_log.png'.format(modeString, opts.Nj, opts.mH))


c3 = TCanvas("c3", "subtracted")
sf.Draw()
pyroot_logon.cmsPrelim(c3, fitterPars.intLumi/1000)
c3.Print('H{2}_Mjj_{0}_{1}jets_Subtracted.pdf'.format(modeString, opts.Nj, opts.mH))
c3.Print('H{2}_Mjj_{0}_{1}jets_Subtracted.png'.format(modeString, opts.Nj, opts.mH))

c4 = TCanvas("c4", "pull")
pf.Draw()
pyroot_logon.cmsPrelim(c4, fitterPars.intLumi/1000)
c4.Print('H{2}_Mjj_{0}_{1}jets_Pull.pdf'.format(modeString, opts.Nj, opts.mH))
c4.Print('H{2}_Mjj_{0}_{1}jets_Pull.png'.format(modeString, opts.Nj, opts.mH))


h_total = mf.getCurve('h_total')
theData = mf.getHist('theData')

mass = theFitter.getWorkSpace().var(fitterPars.var)
mass.setRange('signal', fitterPars.minTrunc, fitterPars.maxTrunc)
yields = fr.floatParsFinal()
iset = RooArgSet(mass)
sigInt = theFitter.makeFitter().createIntegral(iset, 'signal')
sigFullInt = theFitter.makeFitter().createIntegral(iset)
dibosonInt = theFitter.makeDibosonPdf().createIntegral(iset, 'signal')
dibosonFullInt = theFitter.makeDibosonPdf().createIntegral(iset)
WpJInt = theFitter.makeWpJPdf().createIntegral(iset, 'signal')
WpJFullInt = theFitter.makeWpJPdf().createIntegral(iset)
ttbarInt = theFitter.makettbarPdf().createIntegral(iset, 'signal')
ttbarFullInt = theFitter.makettbarPdf().createIntegral(iset)
SingleTopInt = theFitter.makeSingleTopPdf().createIntegral(iset, 'signal')
SingleTopFullInt = theFitter.makeSingleTopPdf().createIntegral(iset)
QCDInt = theFitter.makeQCDPdf().createIntegral(iset, 'signal')
QCDFullInt = theFitter.makeQCDPdf().createIntegral(iset)
ZpJInt = theFitter.makeZpJPdf().createIntegral(iset, 'signal')
ZpJFullInt = theFitter.makeZpJPdf().createIntegral(iset)
## print "*** yield vars ***"
## yields.Print("v")
covMatrix = TMatrixDSym(fr.covarianceMatrix())

sig2 = 0.
for v1 in range(0, covMatrix.GetNrows()):
    for v2 in range(0, covMatrix.GetNcols()):
        if ((yields[v1].GetName())[0] == 'n') and \
               ((yields[v2].GetName())[0] == 'n'):
            sig2 += covMatrix(v1, v2)

usig2 = 0.
totalYield = 0.

sigYieldsFile = open('lastSigYield.txt', 'w')

print
print '-------------------------------'
print 'Yields in signal box'
print '-------------------------------'
for i in range(0, yields.getSize()):
    theName = yields.at(i).GetName()
    if theName[0] == 'n':
        totalYield += yields.at(i).getVal()
        theIntegral = 1.
        if (theName == 'nDiboson'):
            theIntegral = dibosonInt.getVal()/dibosonFullInt.getVal()
        elif (theName == 'nWjets'):
            theIntegral = WpJInt.getVal()/WpJFullInt.getVal()
        elif (theName == 'nTTbar'):
            theIntegral = ttbarInt.getVal()/ttbarFullInt.getVal()
        elif (theName == 'nSingleTop'):
            theIntegral = SingleTopInt.getVal()/SingleTopFullInt.getVal()
        elif (theName == 'nQCD'):
            theIntegral = QCDInt.getVal()/QCDFullInt.getVal()
        elif (theName == 'nZjets'):
            theIntegral = ZpJInt.getVal()/ZpJFullInt.getVal()

        yieldString = '{0} = {1:0.0f} +/- {2:0.0f}'.format(theName,
                                                  yields.at(i).getVal()*theIntegral,
                                                  yields.at(i).getError()*theIntegral)
        print yieldString
    else:
        yieldString = '{0} = {1:0.3f} +/- {2:0.3f}'.format(theName,
                                                           yields.at(i).getVal(),
                                                           yields.at(i).getError())
    sigYieldsFile.write(yieldString + '\n')
print '-------------------------------'
print 'total yield: {0:0.0f} +/- {1:0.0f}'.format(totalYield*sigInt.getVal()/sigFullInt.getVal(), sigInt.getVal()*sqrt(sig2))
print '-------------------------------'

sigYieldsFile.close()


fr.Print()
nll=fr.minNll()
print '***** nll = ',nll,' ***** \n'
print 'total yield: {0:0.0f} +/- {1:0.0f}'.format(totalYield, sqrt(sig2))

cdebug = TCanvas('cdebug', 'debug')
pars4 = config.the4BodyConfig(fitterPars, opts.mH)
fitter4 = RooWjjMjjFitter(pars4)
#fitter4.makeFitter(False)

fitter4.make4BodyPdf(theFitter)
fitter4.loadData()
fitter4.loadParameters('lastSigYield.txt')

## fitter4.getWorkSpace().Print()

mf4 = fitter4.stackedPlot(False, RooWjjMjjFitter.mlnujj)
sf4 = fitter4.residualPlot(mf4, "h_background", "dibosonPdf", False)
pf4 = fitter4.residualPlot(mf4, "h_total", "", True)
lf4 = fitter4.stackedPlot(True, RooWjjMjjFitter.mlnujj)

c4body = TCanvas('c4body', '4 body stacked')
mf4.Draw()
pyroot_logon.cmsPrelim(c4body, pars4.intLumi/1000)
c4body.Print('H{2}_Mlvjj_{0}_{1}jets_Q.pdf'.format(modeString, opts.Nj, opts.mH))
c4body.Print('H{2}_Mlvjj_{0}_{1}jets_Q.png'.format(modeString, opts.Nj, opts.mH))

## assert(False)

c4body2 = TCanvas("c4body2", "4 body stacked_log")
c4body2.SetLogy()
lf4.Draw()
pyroot_logon.cmsPrelim(c4body2, pars4.intLumi/1000)
c4body2.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked_log.pdf'.format(modeString, opts.Nj, opts.mH))
c4body2.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked_log.png'.format(modeString, opts.Nj, opts.mH))


c4body3 = TCanvas("c4body3", "4 body subtracted")
sf4.Draw()
pyroot_logon.cmsPrelim(c4body3, pars4.intLumi/1000)
c4body3.Print('H{2}_Mlvjj_{0}_{1}jets_Subtracted.pdf'.format(modeString, opts.Nj, opts.mH))
c4body3.Print('H{2}_Mlvjj_{0}_{1}jets_Subtracted.png'.format(modeString, opts.Nj, opts.mH))


c4body4 = TCanvas("c4body4", "4 body pull")
pf4.Draw()
pyroot_logon.cmsPrelim(c4body4, pars4.intLumi/1000)
c4body4.Print('H{2}_Mlvjj_{0}_{1}jets_Pull.pdf'.format(modeString, opts.Nj, opts.mH))
c4body4.Print('H{2}_Mlvjj_{0}_{1}jets_Pull.png'.format(modeString, opts.Nj, opts.mH))



print 'shape file created'
ShapeFile = TFile('H{2}_{1}_{0}Jets_Fit_Shapes.root'.format(opts.Nj,
                                                           modeString,
                                                           opts.mH),
                  'recreate')

h_total = mf4.getCurve('h_total')
theData = mf4.getHist('theData')

h_total.Write()
theData.Write()
ShapeFile.Close()
