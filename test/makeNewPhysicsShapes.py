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
parser.add_option('-m', '--mode', default="MjjOptimizeConfig",
                  dest='modeConfig',
                  help='which config to select look at HWWconfig.py for an '+ \
                  'example.  Use the file name minus the .py extension.')
(opts, args) = parser.parse_args()

import pyroot_logon
config = __import__(opts.modeConfig)

from ROOT import gPad, TFile, Double, Long, gROOT, TCanvas
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L EffTableReader.cc+')
gROOT.ProcessLine('.L EffTableLoader.cc+')
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
from ROOT import RooWjjFitterUtils, kCyan, kYellow, kMagenta, kSolid, kDashed,\
     kDashDotted, TH1D, TF1, kBlue

fitterPars = config.theConfig(opts.Nj, opts.mcdir, opts.startingFile)
fitUtils = RooWjjFitterUtils(fitterPars)

if fitterPars.includeMuons and fitterPars.includeElectrons:
    modeString = ''
elif fitterPars.includeMuons:
    modeString = 'Muon'
elif fitterPars.includeElectrons:
    modeString = 'Electron'
else:
    modeString = ''


scalingPb = 1000.
models = [('mu_ZprimeMadGraph_CMSSW428.root', 'el_ZprimeMadGraph_CMSSW428.root', 1.72, "Z'", 'Zprime', kCyan+2, 49998),
          ('mu_TechnicolorPythia_CMSSW428.root', 'el_TechnicolorPythia_CMSSW428.root', 1.58, 'Technicolor', 'Technicolor', kYellow+2, 49800),
          ('mu_WH150qq_CMSSW428.root', 'el_WH150qq_CMSSW428.root', 0.0125, 'WH/ZH', 'WH', kMagenta+2, 50000*1.24669/1.45611),
          ('', '', 1.0, 'generic Gaussian', 'gaus', kBlue+2, 2.*scalingPb)
          ]

hists = []
HiggsEff = 0.
genGaus = TF1('genGaus', 'TMath::Gaus(x, [0], [1])', fitterPars.minMass,
              fitterPars.maxMass)
genGaus.SetParameters(150., 150.*0.1)
for model in models:
    N = 0.
    tmpHist = fitUtils.newEmptyHist(model[4]+'_combined')
    if fitterPars.includeMuons:
        if len(model[0]) > 0:
            thehist = fitUtils.File2Hist(fitterPars.NewPhysicsDirectory + \
                                         model[0],
                                         model[4] + '_mu', False, 1, False)
        else:
            thehist = fitUtils.newEmptyHist(model[4]+'_mu')
            thehist.FillRandom('genGaus', 100000)
            thehist.Scale(model[2]*scalingPb*HiggsEff/thehist.Integral())
            thehist.Print()
        tmpHist.Add(thehist)
        N += model[6]/2.
    if fitterPars.includeElectrons:
        if len(model[1]) > 0:
            thehist = fitUtils.File2Hist(fitterPars.NewPhysicsDirectory + model[1],
                                         model[4] + '_el', True, 1, False)
        else:
            thehist = fitUtils.newEmptyHist(model[4]+'_el')
            thehist.FillRandom('genGaus', 100000)
            thehist.Scale(model[2]*scalingPb*HiggsEff/thehist.Integral())
            thehist.Print()
        tmpHist.Add(thehist)
        N += model[6]/2.

    Npb = model[6]/model[2]
    Nreco = tmpHist.Integral()
    print model[3], 'Ngen: %i Nreco: %0.0f xsec: %0.4f pb' % (model[6], Nreco, model[2]), \
          'equiv integrated lumi: %0.0f pb^(-1)' % (Npb),
    print 'Nreco/(fb^(-1)): %0.3f' % (Nreco*(1000./Npb))
    print model[3], 'acceptance x efficiency = %04.3g' % (Nreco/model[6])
    ## print model[3], 'acceptance x efficiency = %04.3g' % (Nreco/N)
    tmpHist.Scale(1000./Npb, 'width')
    tmpHist.SetLineColor(model[5])
    tmpHist.SetLineWidth(3)


    if model[4] == 'WH':
        HiggsEff = Nreco/N
    
    hists.append(tmpHist)

f = TFile('Mjj_%s_%iJets_NewPhysics_1fb.root' % (modeString, opts.Nj),
          'recreate')
for hist in hists:
    hist.Print()
    hist.Draw('hist')
    hist.SetXTitle("m_{jj} (GeV)")
    hist.SetYTitle("Arbitrary Units")
    gPad.Update()
    gPad.Print("Mjj_%s_%iJets_%s.pdf" % (modeString, opts.Nj, hist.GetName()))
    gPad.Print("Mjj_%s_%iJets_%s.png" % (modeString, opts.Nj, hist.GetName()))
    hist.Write()
f.Close()
