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
                  default='',
                  help='File to use as the initial template')
parser.add_option('-d', '--dir', dest='mcdir', default='',
                  help='directory to pick up the W+jets shapes')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select look at HWWconfig.py for an '+ \
                  'example.  Use the file name minus the .py extension.')
parser.add_option('-H', '--mH', dest='mH', default=400, type='int',
                  help='Higgs Mass Point')
parser.add_option('-s', '--syst', dest='syst', type='int', default=0,
                   help='alpha systematic 0: none, 1: down, 2: up')
parser.add_option('-W', '--WpJ', dest='ParamWpJ', action='store_true',
                  default=False, help='parameterize the W+jets Mjj shape')
(opts, args) = parser.parse_args()

import pyroot_logon

config = __import__(opts.modeConfig)

assert (opts.syst >= 0)
assert (opts.syst <= 2)

#import HWWwideSideband

from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
#gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L EffTableReader.cc+')
gROOT.ProcessLine('.L EffTableLoader.cc+')
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')
from ROOT import RooWjjMjjFitter, RooFitResult, RooWjjFitterUtils, \
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
theFitter.makeFitter(opts.ParamWpJ)

#theFitter.getWorkSpace().Print()
theFitter.getWorkSpace().var('nDiboson').setConstant(False)

fr = theFitter.fit()

tries = 1
while (fr.covQual() != 3) and (tries < 2):
    print "Fit didn't converge well.  Will try again."
    fr = theFitter.fit(True)
    tries += 1

chi2 = Double(0.)
#ndf = Long(2)
ndf = Long(fr.floatParsFinal().getSize()-5)
theFitter.computeChi2(chi2, ndf)
mf = theFitter.stackedPlot()
## sf = theFitter.residualPlot(mf, "h_background", "dibosonPdf", False)
pf = theFitter.residualPlot(mf, "h_total", "", True)
## lf = theFitter.stackedPlot(True)

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

if (fr.covQual() != 3):
    print "Fit did not converge with a good error matrix. Bailing out."
##     assert(False)

## c2 = TCanvas("c2", "stacked_log")
## c2.SetLogy()
## lf.Draw()
## pyroot_logon.cmsPrelim(c2, fitterPars.intLumi/1000)
## c2.Print('H{2}_Mjj_{0}_{1}jets_Stacked_log.pdf'.format(modeString, opts.Nj, opts.mH))
## c2.Print('H{2}_Mjj_{0}_{1}jets_Stacked_log.png'.format(modeString, opts.Nj, opts.mH))


## c3 = TCanvas("c3", "subtracted")
## sf.Draw()
## pyroot_logon.cmsPrelim(c3, fitterPars.intLumi/1000)
## c3.Print('H{2}_Mjj_{0}_{1}jets_Subtracted.pdf'.format(modeString, opts.Nj, opts.mH))
## c3.Print('H{2}_Mjj_{0}_{1}jets_Subtracted.png'.format(modeString, opts.Nj, opts.mH))

c4 = TCanvas("c4", "pull")
pf.Draw()
pyroot_logon.cmsPrelim(c4, fitterPars.intLumi/1000)
c4.Print('H{2}_Mjj_{0}_{1}jets_Pull.pdf'.format(modeString, opts.Nj, opts.mH))
c4.Print('H{2}_Mjj_{0}_{1}jets_Pull.png'.format(modeString, opts.Nj, opts.mH))

mass = theFitter.getWorkSpace().var(fitterPars.var)
mass.setRange('signal', fitterPars.minTrunc, fitterPars.maxTrunc)
#yields = theFitter.makeFitter().coefList()
finalPars = fr.floatParsFinal()
yields = RooArgList(finalPars)
yields.add(fr.constPars())
iset = RooArgSet(mass)
sigInt = theFitter.makeFitter().createIntegral(iset,iset,'signal')
sigFullInt = theFitter.makeFitter().createIntegral(iset,iset)
print "allBkg","sigInt",sigInt.getVal(),"fullInt",sigFullInt.getVal(),\
      "ratio",sigInt.getVal()/sigFullInt.getVal()
dibosonInt = theFitter.makeDibosonPdf().createIntegral(iset,iset,'signal')
dibosonFullInt = theFitter.makeDibosonPdf().createIntegral(iset,iset)
WpJPdf = theFitter.makeWpJPdf()
WpJInt = WpJPdf.createIntegral(iset, iset, 'signal')
WpJFullInt = WpJPdf.createIntegral(iset, iset)
#WpJPdf.Print("v")
print "WpJ","sigInt",WpJInt.getVal(),"fullInt",WpJFullInt.getVal(),\
      "ratio",WpJInt.getVal()/WpJFullInt.getVal()
ttbarInt = theFitter.makettbarPdf().createIntegral(iset, iset, 'signal')
ttbarFullInt = theFitter.makettbarPdf().createIntegral(iset, iset)
SingleTopInt = theFitter.makeSingleTopPdf().createIntegral(iset, iset, 'signal')
SingleTopFullInt = theFitter.makeSingleTopPdf().createIntegral(iset, iset)
QCDInt = theFitter.makeQCDPdf().createIntegral(iset, iset, 'signal')
QCDFullInt = theFitter.makeQCDPdf().createIntegral(iset, iset)
ZpJInt = theFitter.makeZpJPdf().createIntegral(iset, iset, 'signal')
ZpJFullInt = theFitter.makeZpJPdf().createIntegral(iset, iset)
print "ZpJ","sigInt",ZpJInt.getVal(),"fullInt",ZpJFullInt.getVal(),\
      "ratio",ZpJInt.getVal()/ZpJFullInt.getVal()
## print "*** yield vars ***"
## yields.Print("v")
covMatrix = TMatrixDSym(fr.covarianceMatrix())

sig2 = 0.
for v1 in range(0, covMatrix.GetNrows()):
    for v2 in range(0, covMatrix.GetNcols()):
        if ((finalPars[v1].GetName())[0] == 'n') and \
               ((finalPars[v2].GetName())[0] == 'n'):
            sig2 += covMatrix(v1, v2)

usig2 = 0.
totalYield = 0.
sigYield = 0.
sigErrs = {}

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
        sigErrs[theName] = yields.at(i).getError()*theIntegral
        sigYield += yields.at(i).getVal()*theIntegral
    else:
        yieldString = '{0} = {1:0.3f} +/- {2:0.3f}'.format(theName,
                                                           yields.at(i).getVal(),
                                                           yields.at(i).getError())
    sigYieldsFile.write(yieldString + '\n')

sigYieldsFile.close()

sigSig2 = (sqrt(sig2-totalYield)/totalYield*sigYield)**2
sigSig2 += sigYield

print '-------------------------------'
print 'total yield = {0:0.0f} +/- {1:0.0f}'.format(sigYield, sqrt(sigSig2))
print '-------------------------------'
sigSig2 -= sigYield
## print 'sig box all:',totalYield*sigInt.getVal()/sigFullInt.getVal(),\
##       'err:',sqrt(sig2)*sigInt.getVal()/sigFullInt.getVal()
print 'fractional error on background yield not from Poisson stats = %0.3f' % (sqrt(sigSig2)/sigYield)


fr.Print()
nll=fr.minNll()
print '***** nll = ',nll,' ***** \n'
print 'total yield: {0:0.0f} +/- {1:0.0f}'.format(totalYield, sqrt(sig2))

#assert(False)

cdebug = TCanvas('cdebug', 'debug')
pars4 = config.the4BodyConfig(fitterPars, opts.mH, opts.syst)
fitter4 = RooWjjMjjFitter(pars4)

fitter4.makeFitter(True)
fitter4.loadData()
fitter4.make4BodyPdf(theFitter)
fitter4.loadParameters('lastSigYield.txt')

## fitter4.getWorkSpace().Print()

mf4 = fitter4.stackedPlot(False, RooWjjMjjFitter.mlnujj)
## sf4 = fitter4.residualPlot(mf4, "h_background", "dibosonPdf", False)
pf4 = fitter4.residualPlot(mf4, "h_total", "", True)
lf4 = fitter4.stackedPlot(True, RooWjjMjjFitter.mlnujj)

c4body = TCanvas('c4body', '4 body stacked')
mf4.Draw()
pyroot_logon.cmsPrelim(c4body, pars4.intLumi/1000)
c4body.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked.pdf'.format(modeString, opts.Nj, opts.mH))
c4body.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked.png'.format(modeString, opts.Nj, opts.mH))

## assert(False)

c4body2 = TCanvas("c4body2", "4 body stacked_log")
c4body2.SetLogy()
lf4.Draw()
pyroot_logon.cmsPrelim(c4body2, pars4.intLumi/1000)
c4body2.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked_log.pdf'.format(modeString, opts.Nj, opts.mH))
c4body2.Print('H{2}_Mlvjj_{0}_{1}jets_Stacked_log.png'.format(modeString, opts.Nj, opts.mH))


## c4body3 = TCanvas("c4body3", "4 body subtracted")
## sf4.Draw()
## pyroot_logon.cmsPrelim(c4body3, pars4.intLumi/1000)
## c4body3.Print('H{2}_Mlvjj_{0}_{1}jets_Subtracted.pdf'.format(modeString, opts.Nj, opts.mH))
## c4body3.Print('H{2}_Mlvjj_{0}_{1}jets_Subtracted.png'.format(modeString, opts.Nj, opts.mH))


c4body4 = TCanvas("c4body4", "4 body pull")
pf4.Draw()
pyroot_logon.cmsPrelim(c4body4, pars4.intLumi/1000)
c4body4.Print('H{2}_Mlvjj_{0}_{1}jets_Pull.pdf'.format(modeString, opts.Nj, opts.mH))
c4body4.Print('H{2}_Mlvjj_{0}_{1}jets_Pull.png'.format(modeString, opts.Nj, opts.mH))

cdebug.Print('H%i_Mlvjj_%s_%ijets_WpJShape.pdf' % (opts.mH, modeString,
                                                   opts.Nj))
cdebug.Print('H%i_Mlvjj_%s_%ijets_WpJShape.png' % (opts.mH, modeString,
                                                   opts.Nj))

#assert(False)

fitUtils = RooWjjFitterUtils(pars4)
HiggsHist = fitUtils.newEmptyHist('HWW%i_%s_shape' % (opts.mH,modeString))

tau = fitter4.getWorkSpace().var('tau')
tauNom = tau.getVal()

tau.setVal(tauNom + tau.getError())
mf4_up = fitter4.stackedPlot(False, RooWjjMjjFitter.mlnujj)

tau.setVal(tauNom - tau.getError())
mf4_down = fitter4.stackedPlot(False, RooWjjMjjFitter.mlnujj)

if pars4.includeMuons:
    thehist = fitUtils.File2Hist(fitterPars.MCDirectory + \
                                 'mu_HWWMH%i_CMSSW428.root' % (opts.mH),
                                 'HWW%i_mu' % (opts.mH), False, 1, False)
    HiggsHist.Add(thehist)

if pars4.includeElectrons:
    thehist = fitUtils.File2Hist(fitterPars.MCDirectory + \
                                 'el_HWWMH%i_CMSSW428.root' % (opts.mH),
                                 'HWW%i_el' % (opts.mH), True, 1, False)
    HiggsHist.Add(thehist)

Ngen = config.NgenHiggs(opts.mH, pars4.includeElectrons, pars4.includeMuons)
HiggsHist.Scale(1./float(Ngen), 'width')

print "Ngen Higgs:",Ngen
HiggsHist.Print()


print 'shape file created'
ShapeFile = TFile('H{2}_{1}_{0}Jets_Fit_Shapes.root'.format(opts.Nj,
                                                           modeString,
                                                           opts.mH),
                  'recreate')

h_total = mf4.getCurve('h_total')
h_total_up = mf4_up.getCurve('h_total')
h_total_up.SetName('h_total_up')
h_total_down = mf4_down.getCurve('h_total')
h_total_down.SetName('h_total_down')

theData = mf4.getHist('theData')


h_total.Write()
theData.Write()

h_total_up.Write();
h_total_down.Write();

HiggsHist.Write()
ShapeFile.Close()
