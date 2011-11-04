#! /usr/bin/env python

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
(opts, args) = parser.parse_args()

import pyroot_logon

config = __import__(opts.modeConfig)
from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+');
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit, TLatex, TMatrixDSymEigen, RooArgList, RooArgSet
from math import sqrt

# RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

fitterPars = config.theConfig(opts.Nj, opts.mcdir, opts.startingFile)
theFitter = RooWjjMjjFitter(fitterPars)

fr = theFitter.fit()
chi2 = Double(0.)
#ndf = Long(2)
ndf = Long(fr.floatParsFinal().getSize()/2. + 0.5)
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
c1.Print('HWW_Mjj_{0}jets_Stacked.pdf'.format(opts.Nj))
c2 = TCanvas("c2", "stacked_log")
c2.SetLogy()
lf.Draw()
pyroot_logon.cmsPrelim(c2, fitterPars.intLumi/1000)
c2.Print('HWW_Mjj_{0}jets_Stacked_log.pdf'.format(opts.Nj))
c3 = TCanvas("c3", "subtracted")
sf.Draw()
pyroot_logon.cmsPrelim(c3, fitterPars.intLumi/1000)
c3.Print('HWW_Mjj_{0}jets_Subtracted.pdf'.format(opts.Nj))
c4 = TCanvas("c4", "pull")
pf.Draw()
pyroot_logon.cmsPrelim(c4, fitterPars.intLumi/1000)
c4.Print('HWW_Mjj_{0}jets_Pull.pdf'.format(opts.Nj))

mass = theFitter.getWorkSpace().var(fitterPars.var)
mass.setRange('signal', fitterPars.minTrunc, fitterPars.maxTrunc)
yields = RooArgList(theFitter.makeFitter().coefList())
iset = RooArgSet(mass)
sigInt = theFitter.makeFitter().createIntegral(iset, 'signal')
## print "*** yield vars ***"
## yields.Print("v")
eigen = TMatrixDSymEigen(fr.covarianceMatrix())

usig2 = 0.
totalYield = 0.
for i in range(0, yields.getSize()):
    usig2 += yields.at(i).getError()*yields.at(i).getError()
    totalYield += yields.at(i).getVal()


sig2 = 0.
for eigVal in eigen.GetEigenValues():
    sig2 += eigVal

fr.Print()
#print 'total yield error (uncorrelated): {0:0.1f}'.format(sqrt(usig2))
print 'total yield: {0:0.0f} +/- {1:0.0f}'.format(totalYield, sqrt(sig2))
print 'sqrt(total): {0:0.0f}'.format(sqrt(totalYield))
print 'yield in signal box: {0:0.0f} +/- {1:0.0f}'.format(totalYield*sigInt.getVal(), sigInt.getVal()*sqrt(sig2))
