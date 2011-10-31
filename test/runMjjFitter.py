#! /usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='MjjNominal2Jets.txt',
                  help='File to use as the initial template')
parser.add_option('-d', '--dir', dest='mcdir', default='',
                  help='directory to pick up the W+jets shapes')
(opts, args) = parser.parse_args()

import pyroot_logon
import MjjFitConfig

from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+');
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit, TLatex

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

fitterPars = MjjFitConfig.theConfig(opts.Nj, opts.mcdir, opts.startingFile)
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
c2 = TCanvas("c2", "stacked_log")
c2.SetLogy()
lf.Draw()
pyroot_logon.cmsPrelim(c2, fitterPars.intLumi/1000)
c3 = TCanvas("c3", "subracted")
sf.Draw()
pyroot_logon.cmsPrelim(c3, fitterPars.intLumi/1000)
c4 = TCanvas("c4", "pull")
pf.Draw()
pyroot_logon.cmsPrelim(c4, fitterPars.intLumi/1000)
