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
(opts, args) = parser.parse_args()

import pyroot_logon
from WjjFitterConfigs import HWWconfig

from ROOT import gPad, TFile, Double, Long, gROOT
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+');
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

fitterPars = HWWconfig(opts.Nj, opts.mcdir, opts.startingFile)
theFitter = RooWjjMjjFitter(fitterPars)

fr = theFitter.fit()
chi2 = Double(0.)
ndf = Long(fr.floatParsFinal().getSize())
mf = theFitter.computeChi2(chi2, ndf)
mf.Draw()
