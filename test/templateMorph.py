utils = None

def scaleUnwidth(hist):
    for binx in range(1, hist.GetNbinsX()+1):
        hist.SetBinContent(binx, 
                           hist.GetBinContent(binx)*hist.GetBinWidth(binx))
        hist.SetBinError(binx, hist.GetBinError(binx)*hist.GetBinWidth(binx))

    return hist

def morph(hist1, hist2, mass1, mass2, targetMass, fitter, params,
          mode = "HWW",
          debug = False):
    from ROOT import RooRealVar, RooIntegralMorph,\
        kRed,kBlue,kViolet
    import re

    if mass1<mass2:
        histLow = hist1
        massLow = mass1
        histHigh = hist2
        massHigh = mass2
    else:
        histLow = hist2
        massLow = mass2
        histHigh = hist1
        massHigh = mass1

    mAlpha = 1.0 - float(targetMass-massLow)/float(massHigh-massLow)
    print 'alpha:',mAlpha,
    newIntegral = histLow.Integral() + \
        (targetMass - massLow) * \
        (histHigh.Integral()-histLow.Integral()) / \
        (massHigh-massLow)
    print 'low integral:', histLow.Integral(), \
        'high integral:', histHigh.Integral(), \
        'new integral:', newIntegral
    sigHistLow = utils.Hist2Pdf(histLow, "sigHistLow_%s" % (mode), 
                                fitter.getWorkSpace())
    sigHistHigh = utils.Hist2Pdf(histHigh, "sigHistHigh_%s" % (mode),
                                 fitter.getWorkSpace())
    x = fitter.getWorkSpace().var(params.var)
    #x.Print("v")
    #x.setBins(params.nbins, "cache")
    x.setBins(1000, "cache")
    alpha_morph = RooRealVar("alpha_morph", "#alpha_{morph}", mAlpha, 0., 1.)

    sigModel = RooIntegralMorph("sigModel", "sigModel", sigHistLow, 
                                sigHistHigh, x, alpha_morph)

    if (debug):
        from ROOT import gPad, RooFit
        frame = x.frame()
        sigHistLow.plotOn(frame, RooFit.LineColor(kRed+1),
                          RooFit.LineStyle(2))
        sigHistHigh.plotOn(frame, RooFit.LineColor(kBlue+1),
                           RooFit.LineStyle(2))
        sigModel.plotOn(frame, RooFit.LineColor(kViolet+1),
                        RooFit.LineStyle(9))
        frame.Draw()
        gPad.Update()
        gPad.WaitPrimitive()

    morphHist = sigModel.createHistogram(re.sub(r'\d+', '%i' % targetMass,
                                                hist1.GetName()), x)
    morphHist.Scale(newIntegral/morphHist.Integral())
    morphHist.SetName(re.sub(r'\d+', '%i' % targetMass, hist1.GetName()))
    
    # if debug:
    #     morphHist.Print()

    return morphHist

if __name__ == '__main__':
    wpjhelp = '''Parameterization for W+jets fit ---
-1 : histograms with floating fMU and fSU
 0 : erf * (power law with two params)
 1 : erf * (power law ith one param) * exponential
 2 : (fermi turn on) * (power law with two params)
 3 : (fermi turn on) * exponential
 4 : exponential * (power law with one param)
 5 : exponential * (power law with two params)
 6 : erf * (power law with one param)
everything else : same as zero
'''
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                      help='Number of jets.')
    parser.add_option('-d', '--dir', dest='mcdir', default='',
                      help='directory to pick up the W+jets shapes')
    parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                      help='which config to select look at HWWconfig.py ' + \
                          'for an example.  Use the file name minus the ' + \
                          '.py extension.')
    parser.add_option('-H', '--mH', dest='mH', default=400, type='int',
                      help='Higgs Mass Point')
    parser.add_option('-s', '--syst', dest='syst', type='int', default=0,
                      help='alpha systematic 0: none, 1: down, 2: up')
    parser.add_option('-W', '--WpJ', dest='ParamWpJ', type='int',
                      default=-1, help=wpjhelp)
    parser.add_option('--debug', action='store_true', dest='debug', 
                      default=False, help='stop when things aren\'t good')
    parser.add_option('--mvaCut', dest='mvaCut', type='float',
                      help='override the mva cut with this value')
    parser.add_option('--alpha', dest='alpha', type='float',
                      help='override the alpha with this value')
    parser.add_option('--qgCut', dest='qgCut', type='float',
                      help='cut value on the product of QGLikelihood')
    parser.add_option('--basisFile', dest='basisFile', help='basis file for' + \
                          ' morphing')
    parser.add_option('--mHmorph', dest='mHmorph', type='int', 
                      help='H mass for morphing with basis')
    (opts, args) = parser.parse_args()

    import pyroot_logon

    config = __import__(opts.modeConfig)

    assert (opts.syst >= 0)
    assert (opts.syst <= 2)
    assert (opts.basisFile)
    assert (opts.mHmorph)

    from ROOT import TFile, gROOT, kRed, kBlue, kViolet, RooMsgService, RooFit
    #gROOT.ProcessLine('.L RooWjjFitterParams.h+')
    gROOT.ProcessLine('.L EffTableReader.cc+')
    gROOT.ProcessLine('.L EffTableLoader.cc+')
    gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
    gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')
    from ROOT import RooWjjMjjFitter, RooWjjFitterUtils, gPad
    import re
    import HWWSignalShapes

    
    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
    mHbasis = int(re.search(r'H([0-9]+)', opts.basisFile).group(1))
    mHmorph = opts.mHmorph

    fitterPars = config.theConfig(opts.Nj, opts.mcdir, '', 
                                  mHbasis, opts.mvaCut, opts.qgCut)

    fitterPars.WpJfunction = opts.ParamWpJ
    if opts.ParamWpJ<0:
        fitterPars.smoothingOrder = 0

    if fitterPars.includeMuons and fitterPars.includeElectrons:
        modeString = ''
    elif fitterPars.includeMuons:
        modeString = 'Muon'
    elif fitterPars.includeElectrons:
        modeString = 'Electron'
    else:
        modeString = ''

    pars4 = config.the4BodyConfig(fitterPars, mHbasis, opts.syst, 
                                  opts.alpha)
    fitter4 = RooWjjMjjFitter(pars4)

    higgsModes = HWWSignalShapes.modes
    morphedHists = []

    utils = RooWjjFitterUtils(pars4)

    print 'basis Higgs', mHbasis, 'morphing with Higgs', mHmorph,
    print 'to make Higgs', opts.mH

    higgsHists = HWWSignalShapes.GenHiggsHists(pars4, mHmorph)

    fbasis = TFile(opts.basisFile)
    # fmorph = TFile(opts.morphFile)
    gROOT.cd()

    for higgsMode in higgsModes:

        basisName = '%s%i_%s_shape' % (higgsMode, mHbasis, modeString)
        morphName = '%s%i_%s_shape' % (higgsMode,  mHmorph, modeString)

        print 'basis hist name:',basisName,'morph hist name:',morphName

        histbasis = scaleUnwidth(fbasis.Get(basisName))
        histmorph = scaleUnwidth(higgsHists[higgsMode])

        gROOT.cd()
        morphedHists.append(morph(histbasis, histmorph, mHbasis, mHmorph, 
                                  targetMass = opts.mH, mode = higgsMode,
                                  fitter = fitter4, params = pars4, 
                                  debug = opts.debug))

        if opts.debug:
            histbasis.SetLineColor(kBlue)
            histmorph.SetLineColor(kRed)
            if mHbasis < mHmorph:
                histbasis.Draw('hist')
                histmorph.Draw('histsame')
            else:
                histmorph.Draw('hist')
                histbasis.Draw('histsame')                
                
            morphedHists[-1].SetLineStyle(9)
            morphedHists[-1].Draw('histsame')
            gPad.Update()
            gPad.WaitPrimitive()


    foutput = TFile('H%i_%s_%iJets_Fit_Shapes.root' % (opts.mH, modeString,
                                                       opts.Nj), 'recreate')

    fbasis.Get('h_total').Write()
    fbasis.Get('theData').Write()
    fbasis.Get('h_total_up').Write()
    fbasis.Get('h_total_down').Write()
    for hist in morphedHists:
        hist.Scale(1., 'width')
        hist.Write()

    foutput.Close()
