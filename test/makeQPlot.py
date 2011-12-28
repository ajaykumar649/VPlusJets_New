#! /usr/bin/env python

def theConfig(fitterPars, initFile = 'lastMjjSigYield.txt'):

    fitterPars.var = "(MassV2j_PFCor-Mass2j_PFCor-80.399)"

    fitterPars.cuts += '&& (Mass2j_PFCor > %f) && (Mass2j_PFCor < %f) ' % (fitterPars.minTrunc, fitterPars.maxTrunc)

    fitterPars.initParamsFile = initFile
    fitterPars.minMass = 0.
    fitterPars.maxMass = 400.
    fitterPars.nbins = 50
    fitterPars.truncRange = False

    fitterPars.binEdges.clear()
    
    print "mass range:",fitterPars.minMass,'-',fitterPars.maxMass

    return fitterPars

def qPlot(fitterPars):
    from ROOT import kRed, kBlack, kGreen, kOrange, kMagenta, kCyan, THStack, \
         RooWjjMjjFitter, RooWjjFitterUtils, TCanvas
    
    fitUtils = RooWjjFitterUtils(fitterPars)
    theFitter = RooWjjMjjFitter(fitterPars)

    thePdf = theFitter.makeFitter(False)
    theFitter.loadParameters(fitterPars.initParamsFile)

    theFitter.getWorkSpace().Print()

    q = theFitter.getWorkSpace().var(fitterPars.var)
    q.SetTitle("Q = m_{l#nujj}-m_{jj}-M_{W}")

    pdfs = thePdf.pdfList()
    coefs = thePdf.coefList()
    shapeHist = THStack('shapeHist', 'shapeHist')

    sumYield = 0.
    for i in range(pdfs.getSize()-1, -1, -1):
        tmpHist = pdfs[i].createHistogram('h_' + pdfs[i].GetName(), q)
        theName = pdfs[i].GetName()[:3]
        tmpHist.SetFillStyle(1001)
        if theName == 'dib':
            tmpHist.SetFillColor(kOrange)
            tmpHist.SetLineColor(kOrange)
        elif theName == 'WpJ':
            tmpHist.SetFillColor(kRed)
            tmpHist.SetLineColor(kRed)
        elif theName == 'ttP':
            tmpHist.SetFillColor(kBlack)
            tmpHist.SetLineColor(kBlack)
        elif theName == 'stP':
            tmpHist.SetFillColor(kBlack)
            tmpHist.SetLineColor(kBlack)
        elif theName == 'qcd':
            tmpHist.SetFillColor(kGreen)
            tmpHist.SetLineColor(kGreen)
        elif theName == 'ZpJ':
            tmpHist.SetFillColor(kMagenta)
            tmpHist.SetLineColor(kMagenta)
        
##         tmpHist.Draw()
##         gPad.Update()
##         gPad.WaitPrimitive()

        tmpHist.Scale(coefs[i].getVal())
        sumYield += coefs[i].getVal()
        if (coefs[i].getVal() > 0.):
            shapeHist.Add(tmpHist)

    totalShapeHist = thePdf.createHistogram("totalShapeHist", q)
    totalShapeHist.Print()

    dataHist = fitUtils.newEmptyHist("theData")
    if fitterPars.includeMuons:
        tmpHist = fitUtils.File2Hist(fitterPars.DataDirectory+fitterPars.muonData,
                                     "tmpData", False, 0, False, 1.0,
                                     fitUtils.fullCuts())
        dataHist.Add(tmpHist)

    if fitterPars.includeElectrons:
        tmpHist = fitUtils.File2Hist(fitterPars.DataDirectory+fitterPars.electronData,
                                     "tmpData", True, 0, False, 1.0,
                                     fitUtils.fullCuts())
        dataHist.Add(tmpHist)

    totalShapeHist.Scale(sumYield)
##     shapeHist.SetLineColor(kCyan+3)
##     shapeHist.SetMarkerColor(kCyan+3)

    cq = TCanvas('cq', 'Q')
    shapeHist.Draw('')
    shapeHist.GetXaxis().SetTitle(q.GetTitle() + ' (GeV)')
    shapeHist.GetYaxis().SetTitle('Events / (%0.0f GeV)' % ((fitterPars.maxMass-fitterPars.minMass)/fitterPars.nbins))
    dataHist.Draw('same')
    cq.Modified()
    #totalShapeHist.Draw('same')
    dataHist.Chi2Test(totalShapeHist,"UUNORMP")
    print "KS test:",dataHist.KolmogorovTest(totalShapeHist)
    return (cq, shapeHist, totalShapeHist)

if (__name__ == "__main__"):
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

    from ROOT import gROOT
    ## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
    gROOT.ProcessLine('.L EffTableReader.cc+')
    gROOT.ProcessLine('.L EffTableLoader.cc+')
    gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
    gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')

    from ROOT import RooFit, RooMsgService

    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

    fitterPars = theConfig(config.theConfig(opts.Nj, opts.mcdir,
                                            opts.startingFile),
                           opts.startingFile)
    (cq, shapeHist, totalHist) = qPlot(fitterPars)
