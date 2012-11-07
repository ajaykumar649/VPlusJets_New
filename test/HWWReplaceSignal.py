from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select look at HWWconfig.py ' + \
                      'for an example.  Use the file name minus the ' + \
                      '.py extension.')
parser.add_option('-H', '--mH', dest='mH', default=400, type='int',
                  help='Higgs Mass Point')
parser.add_option('-d', '--dir', dest='mcdir',
                  help='directory to pick up the MC shapes')
(opts, args) = parser.parse_args()

assert(len(args)==1)

import pyroot_logon
from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
config = __import__(opts.modeConfig)

gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')

from ROOT import RooWjjMjjFitter, RooFitResult, RooWjjFitterUtils, \
     RooMsgService, RooFit, TLatex, TMatrixDSym, RooArgList, RooArgSet, gPad, \
     kBlue, TH1D, TMath, gDirectory, RooWjjFitterParams
import HWWSignalShapes

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
fitterPars = config.theConfig(opts.Nj, mH = opts.mH)

if opts.mcdir:
    fitterPars.MCDirectory = opts.mcdir

fitterPars.WpJfunction = 0

if fitterPars.includeMuons and fitterPars.includeElectrons:
    modeString = ''
elif fitterPars.includeMuons:
    modeString = 'Muon'
elif fitterPars.includeElectrons:
    modeString = 'Electron'
else:
    modeString = ''

pars4 = config.the4BodyConfig(fitterPars, opts.mH)
pars4.mHiggs = opts.mH
pars4.wHiggs = HWWSignalShapes.HiggsWidth[opts.mH]

fitUtils = RooWjjFitterUtils(pars4)
iwt = 0
if (opts.mH >= 500):
    iwt = 1
sigHists = HWWSignalShapes.GenHiggsHists(pars4, opts.mH, fitUtils, iwt = iwt)

newFilename = 'H{2}_{1}_{0}Jets_Fit_Shapes.root'.format(opts.Nj,
                                                        modeString,
                                                        opts.mH)
print 'old file:',args[0],'new file:',newFilename
assert(not (newFilename == args[0]))
#assert(False)

oldFile = TFile(args[0])
oldFile.ls()
newFile = TFile(newFilename, 'recreate')

oldFile.Get('h_total').Write()
oldFile.Get('theData').Write()
oldFile.Get('h_total_up').Write()
oldFile.Get('h_total_down').Write()

for mode in sigHists:
    sigHists[mode].Write()

sigHists['HWW'].Print()
nomIntegral = sigHists['HWW'].Integral()
if iwt == 1:
    pars4up = RooWjjFitterParams(pars4)
    if opts.Nj == 2:
        pars4up.cuts = pars4.cuts.replace('interferencenominal', 
                                          'interferenceup')
    fitUtilsUp = RooWjjFitterUtils(pars4up)
    sigHistsUp = HWWSignalShapes.GenHiggsHists(pars4up, opts.mH, fitUtilsUp, 
                                               iwt = 2)
    sigHistsUp['HWW'].SetName(sigHistsUp['HWW'].GetName() + '_up')
    newFile.cd()
    sigHistsUp['HWW'].Write()
    sigHistsUp['HWW'].Print()
    upIntegral = sigHistsUp['HWW'].Integral()
    pars4down = RooWjjFitterParams(pars4)
    if opts.Nj == 2:
        pars4down.cuts = pars4.cuts.replace('interferencenominal', 
                                            'interferencedown')
    fitUtilsDown = RooWjjFitterUtils(pars4down)
    sigHistsDown = HWWSignalShapes.GenHiggsHists(pars4down, opts.mH, 
                                                 fitUtilsDown, iwt = 3)
    sigHistsDown['HWW'].SetName(sigHistsDown['HWW'].GetName() + '_down')
    newFile.cd()
    sigHistsDown['HWW'].Write()
    sigHistsDown['HWW'].Print()
    downIntegral = sigHistsDown['HWW'].Integral()

# HiggsHist.Write()
# VBFHiggsHist.Write()
# TauNuHiggsHist.Write()

newFile.cd()
newFile.ls()
newFile.Close()
oldFile.Close()

if iwt == 1:
    print "down norm change:", (downIntegral/nomIntegral)
    print "up norm change:", (upIntegral/nomIntegral)
