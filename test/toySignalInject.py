def curveToHist(curve, hist, debug = False):
    from math import sqrt
    #hist.Sumw2()
    for binx in range(1, hist.GetNbinsX()+1):
        low = hist.GetBinLowEdge(binx)
        high = low + hist.GetBinWidth(binx)
        hist.SetBinContent(binx, curve.average(low,high))
        #hist.SetBinError(binx, sqrt(hist.GetBinContent(binx)))
    if debug:
        print 'drawing background histogram...'
        curve.Draw('al')
        hist.Draw('same')
        gPad.Update()
        gPad.WaitPrimitive()

    return hist

def RooHistToHist(hist_in, hist_out, debug = False):
    for binx in range(1, hist_out.GetNbinsX()+1):
        hist_out.SetBinContent(binx, hist_in.GetY()[binx-1])

    return hist_out
    
def intHist(hist):
    
    for binx in range(1, hist.GetNbinsX()+1):
        hist.SetBinContent(binx, int(hist.GetBinContent(binx)+0.5))


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
    parser.add_option('--mHbasis', dest='mHbasis', type='int', 
                      help='H mass for basis')
    parser.add_option('--factor', dest='factor', default=1, type='float',
                      help='multiplication factor')
    (opts, args) = parser.parse_args()

    import pyroot_logon

    config = __import__(opts.modeConfig)

    assert (opts.syst >= 0)
    assert (opts.syst <= 2)
    assert (opts.mHbasis)

    from ROOT import TFile, gROOT, kRed, kBlue, kViolet, RooMsgService, RooFit
    #gROOT.ProcessLine('.L RooWjjFitterParams.h+')
    gROOT.ProcessLine('.L EffTableReader.cc+')
    gROOT.ProcessLine('.L EffTableLoader.cc+')
    gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
    gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')
    from ROOT import RooWjjMjjFitter, RooWjjFitterUtils, gPad, RooHist, \
        RooCurve, TH1D, RooAbsData
    import re
    import HWWSignalShapes
    import templateMorph

    
    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
    mHbasis = opts.mHbasis

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
    # fitter4 = RooWjjMjjFitter(pars4)

    (halfGen, otherdata) = HWWSignalShapes.NgenHiggs(opts.mH, 'HWW')
    SignalHist = HWWSignalShapes.makeHiggsHist(opts.mH, pars4, 'HWW')
    SignalHist.Scale(otherdata[1]*otherdata[2]*pars4.intLumi* \
                         opts.factor/otherdata[0])
    print "signal only"
    SignalHist.Print()
    if opts.debug:
        print 'drawing signal histogram...'
        SignalHist.Draw()
        gPad.Update()
        gPad.WaitPrimitive()

    basisFilename = 'H%i_%s_%iJets_Fit_Shapes.root' % (mHbasis, modeString,
                                                       opts.Nj)
    print 'opening', basisFilename
    bfile = TFile(basisFilename)
    bgCurve = bfile.Get('h_total')
    theData = bfile.Get('theData')
    if opts.debug:
        oldDataHist = TH1D(SignalHist)
        oldDataHist.Reset('ices')
        oldDataHist.SetName('oldDataHist')
        oldDataHist.SetTitle('oldDataHist')
        oldDataHist = RooHistToHist(theData, oldDataHist, opts.debug)

        print 'drawing old data...'
        oldDataHist.Draw()
        theData.Draw("pz")
        gPad.Update()
        gPad.WaitPrimitive()

        oldDataHist = templateMorph.scaleUnwidth(oldDataHist)

        oldDataHist.Print()

    bgHist = TH1D(SignalHist)
    bgHist.Reset('ices')
    bgHist.SetName(theData.GetName())
    bgHist.SetTitle(theData.GetTitle())
    bgHist = curveToHist(bgCurve, bgHist, debug = opts.debug)

    tmpData = RooHist(bgHist, 0, 1, RooAbsData.SumW2)
    tmpHist = TH1D(bgHist)

    # if opts.debug:
    #     print 'drawing toy data bg residuals before signal addition...'
    #     resid = tmpData.makeResidHist(bfile.Get('h_total'))
    #     resid.Draw('apz')
    #     gPad.Update()
    #     gPad.WaitPrimitive()

    bgHist = templateMorph.scaleUnwidth(bgHist)
    print 'background only'
    bgHist.Print()

    bgHist.Add(SignalHist)

    #intHist(bgHist)

    if opts.debug:
        print 'drawing background + signal histogram...'
        bgHist.Draw()
        gPad.Update()
        gPad.WaitPrimitive()

    print 'background + signal injection'
    bgHist.Print()
    bgHist.Scale(1., 'width')
    tmpfilename = 'tmp_H%i_%s_%iJets.root' % (opts.mHbasis, modeString, opts.Nj)
    foutput = TFile(tmpfilename, 'recreate')

    newData = RooHist(bgHist, 0, 1, RooAbsData.SumW2)
    #newData = RooHist(bgHist)
    newData.SetName(theData.GetName())
    newData.SetTitle(theData.GetTitle())

    if opts.debug:
        print 'drawing toy data bg residuals...'
        tmpHist.Add(bgHist, -1)
        tmpHist.Draw()
        gPad.Update()
        gPad.WaitPrimitive()

    newData.Write()
    bfile.Get('h_total').Write()
    bfile.Get('h_total_up').Write()
    bfile.Get('h_total_down').Write()
    for mode in HWWSignalShapes.modes:
        bfile.Get('%s%i_%s_shape' % (mode, mHbasis, modeString)).Write()

    bfile.Close()
    foutput.Close()

    import os
    print 'renaming',tmpfilename,'to',basisFilename
    os.rename(tmpfilename, basisFilename)
