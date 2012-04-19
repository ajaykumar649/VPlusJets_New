#! /usr/bin/env python
import pyroot_logon

def plot2BodyDist(theFitter, pars, chi2, ndf, 
                  Err = -1, NP = False, Prefix = "Mjj", Left = False):
    from ROOT import gPad, TLatex, TCanvas, kRed, kCyan, kBlue, \
         RooFit, RooPlot, RooCurve, RooAbsReal, TGraphErrors, TLine, \
         RooWjjMjjFitter

    if pars.includeMuons and pars.includeElectrons:
        modeString = ''
    elif pars.includeMuons:
        modeString = 'Muon'
    elif pars.includeElectrons:
        modeString = 'Electron'
    else:
        modeString = ''

    mf = theFitter.stackedPlot(False, RooWjjMjjFitter.mjj, Left)
    mf.SetName("%s_Stacked" % (Prefix));
    sf = theFitter.residualPlot(mf, "h_background", "dibosonPdf", False)
    sf.SetName("%s_Subtracted" % (Prefix));
    pf = theFitter.residualPlot(mf, "h_total", "", True)
    pf.SetName("%s_Pull" % (Prefix))
    pf2 = pf.emptyClone("%s_Pull_Corrected" % (Prefix))
    pf2.SetMinimum(-5.)
    pf2.SetMaximum(5.)
    corrPull = False
    lf = theFitter.stackedPlot(True, RooWjjMjjFitter.mjj, Left)
    lf.SetName("%s_Stacked_Log" % (Prefix));

    if Err > 0:
        totalPdf = theFitter.getWorkSpace().pdf('totalPdf')
        ## Ntotal = totalPdf.expectedEvents(iset)

        ## print 'Ntotal:',Ntotal
        h_dibosonPdf = sf.getCurve('h_dibosonPdf')
        totalPdf.plotOn(sf,
                        RooFit.ProjWData(theFitter.getWorkSpace().data('data')),
                        RooFit.Normalization(Err, RooAbsReal.Raw),
                        #RooFit.AddTo('h_dibosonPdf', 1., 1.),
                        #RooFit.Invisible(),
                        RooFit.Name('h_ErrUp'),
                        RooFit.Range('RangeForPlot'),
                        RooFit.NormRange('RangeForPlot'),
                        RooFit.LineColor(kRed), RooFit.LineStyle(3))
        h_ErrUp = sf.getCurve('h_ErrUp')
        sf.remove('h_ErrUp', False)

        ErrBand = TGraphErrors(h_dibosonPdf.GetN(), h_dibosonPdf.GetX(),
                               h_dibosonPdf.GetY())
        for pt in range(1, ErrBand.GetN()):
            ErrBand.SetPointError(pt, 0,
                                  h_ErrUp.interpolate(ErrBand.GetX()[pt]))
        ErrBand.SetName("ErrBand")
        ErrBand.SetTitle("Uncertainty")
        ErrBand.SetLineColor(kRed)
##         ErrBand.SetLineWidth(0)
##         ErrBand.SetLineStyle(0)
        ErrBand.SetFillColor(kRed)
        ErrBand.SetFillStyle(3353)

        
        #ErrBand.Draw('ap3')
        #h_ErrUp.Draw('lp')
        #gPad.Update()
        #gPad.WaitPrimitive()
##         h_ErrUp.Draw("al")
##         h_ErrUp.GetXaxis().Set(36, 40., 400.)
##         gPad.Update()
##         gPad.WaitPrimitive()
##         h_UpBand = RooCurve("h_UpBand", "Uncertainty", h_dibosonPdf, h_ErrUp,
##                             1., 1.)
##         h_UpBand.SetLineStyle(3)
##         h_UpBand.SetLineColor(kBlue+1)
##         h_DownBand = RooCurve("h_DownBand", "Uncertainty", h_dibosonPdf, h_ErrUp,
##                               1., -1.)
##         h_DownBand.SetLineStyle(3)
##         h_DownBand.SetLineColor(kBlue+1)

##         sf.addPlotable(h_UpBand, "L")
##         sf.addPlotable(h_DownBand, "L")
        sf.addObject(ErrBand, "3")
        #sf.Print("v")
        sf.drawAfter('h_dibosonPdf', 'ErrBand')
        #sf.Print("v")
        sf.drawAfter('ErrBand', 'theData')
        #sf.Print("v")
        sf.findObject('theLegend').AddEntry(ErrBand, 'Uncertainty', 'f')
        sf.findObject('theLegend').SetY1NDC(sf.findObject('theLegend').GetY1NDC() - 0.057)
        sf.findObject('theLegend').SetY1(sf.findObject('theLegend').GetY1NDC())

        corrPull = True
        pf2.addObject(sub2pull(sf.getHist('theData'),
                               sf.findObject('ErrBand')),
                      'p0')
        for item in range(0, int(pf.numItems())):
            firstItem = pf.getObject(item)
            if (type(firstItem) == TLine):
                newLine = TLine(firstItem)
                newLine.SetY1(4.)
                newLine.SetY2(-4.)
                pf2.addObject(newLine, 'l')
                #SetOwnership(newLine, False)


    if NP:
        NPPdf = theFitter.makeNPPdf();
        NPNorm = 4.*0.11*46.8/12.*pars.intLumi

        if (modeString == 'Electron'):
            if pars.njets == 2:
                NPNorm *= 0.0381
            elif pars.njets == 3:
                NPNorm *= 0.0123
        else:
            if pars.njets == 2:
                NPNorm *= 0.0550
            elif pars.njets == 3:
                NPNorm *= 0.0176

        print '**** N_NP:', NPNorm,'****'

        NPPdf.plotOn(sf, RooFit.ProjWData(theFitter.getWorkSpace().data('data')),
                     RooFit.Normalization(NPNorm, RooAbsReal.Raw),
                     RooFit.AddTo('h_dibosonPdf', 1., 1.),
                     RooFit.Name('h_NP'),
                     RooFit.Range('RangeForPlot'),
                     RooFit.NormRange('RangeForPlot'),
                     RooFit.LineColor(kBlue), RooFit.LineStyle(2))

        h_NP = sf.getCurve('h_NP')

        sf.drawBefore('h_dibosonPdf', 'h_NP')
        #sf.Print("v")
        sf.findObject('theLegend').AddEntry(h_NP, "CDF-like Signal", "L")
        sf.findObject('theLegend').SetY1NDC(sf.findObject('theLegend').GetY1NDC() - 0.057)
        sf.findObject('theLegend').SetY1(sf.findObject('theLegend').GetY1NDC())

    l = TLatex()
    l.SetNDC()
    l.SetTextSize(0.045)
    l.SetTextFont(42)

    cstacked = TCanvas("cstacked", "stacked")
    mf.Draw()
    if (chi2 > 0):
        l.DrawLatex(0.55, 0.49,
                    '#chi^{2}/dof = %0.3f/%d' % (chi2, ndf)
                    )
    pyroot_logon.cmsLabel(cstacked, pars.intLumi/1000, prelim = True)
    cstacked.Print('Wjj_%s_%s_%ijets_Stacked.pdf' % (Prefix, modeString,
                                                     pars.njets))
    cstacked.Print('Wjj_%s_%s_%ijets_Stacked.png' % (Prefix, modeString,
                                                     pars.njets))
    c2 = TCanvas("c2", "stacked_log")
    c2.SetLogy()
    lf.Draw()
    pyroot_logon.cmsPrelim(c2, pars.intLumi/1000)
    c2.Print('Wjj_%s_%s_%ijets_Stacked_log.pdf' % (Prefix, modeString,
                                                    pars.njets))
    c2.Print('Wjj_%s_%s_%ijets_Stacked_log.png' % (Prefix, modeString,
                                                    pars.njets))
    c3 = TCanvas("c3", "subtracted")
    sf.Draw()
    pyroot_logon.cmsLabel(c3, pars.intLumi/1000, prelim = True)
    c3.Print('Wjj_%s_%s_%ijets_Subtracted.pdf' % (Prefix, modeString,
                                                  pars.njets))
    c3.Print('Wjj_%s_%s_%ijets_Subtracted.png' % (Prefix, modeString,
                                                  pars.njets))
    c4 = TCanvas("c4", "pull")
    pf.Draw()
    c4.SetGridy()
    pyroot_logon.cmsPrelim(c4, pars.intLumi/1000)
    c4.Print('Wjj_%s_%s_%ijets_Pull.pdf' % (Prefix, modeString, pars.njets))
    c4.Print('Wjj_%s_%s_%ijets_Pull.png' % (Prefix, modeString, pars.njets))

    c5 = None
    if corrPull:
        c5 = TCanvas("c5", "corrected pull")
        pf2.Draw()
        c5.SetGridy()
        pyroot_logon.cmsPrelim(c5, pars.intLumi/1000)
        c5.Print('Wjj_%s_%s_%ijets_Pull_Corrected.pdf' % (Prefix, modeString,
                                                          pars.njets))
        c5.Print('Wjj_%s_%s_%ijets_Pull_Corrected.png' % (Prefix, modeString,
                                                          pars.njets))

    return ([mf,sf,pf2,lf],[cstacked,c2,c3,c5])

def sub2pull(dataHist, pdfwErrs):
    from ROOT import TGraphErrors
    from math import sqrt

    ThePull = TGraphErrors(dataHist.GetN())

    pdfi = 0
    for i in range(0, ThePull.GetN()):
        while (dataHist.GetX()[i] > pdfwErrs.GetX()[pdfi]):
            pdfi += 1
        #print 'pull point:',i,'pdfi:',pdfi
        diff = dataHist.GetY()[i] - pdfwErrs.GetY()[pdfi]
        if (diff < 0):
            err2 = pdfwErrs.GetErrorY(pdfi)**2 + dataHist.GetErrorYhigh(i)**2
        else:
            err2 = pdfwErrs.GetErrorY(pdfi)**2 + dataHist.GetErrorYlow(i)**2
        if (err2>0):
            pull = diff/(sqrt(err2)*1.2)
        else:
            pull = 0

        ThePull.SetPoint(i, dataHist.GetX()[i], pull)
        ThePull.SetPointError(i, 0., 1.)

    ThePull.SetName(dataHist.GetName() + "_pull")
    ThePull.SetTitle("data")

    return ThePull
        

if __name__ == '__main__':
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option('-b', action='store_true', dest='noX', default=False,
                      help='no X11 windows')
    parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                      help='Number of jets.')
    parser.add_option('--TD', dest='toydataFile', default='',
                      help='a file corresponding to a toy dataset')
    parser.add_option('-i', '--init', dest='startingFile',
                      default='MjjNominal2Jets.txt',
                      help='File to use as the initial template')
    parser.add_option('-d', '--dir', dest='mcdir', default='',
                      help='directory to pick up the W+jets shapes')
    parser.add_option('-m', '--mode', default="MjjOptimizeConfig",
                      dest='modeConfig',
                      help='which config to select look at HWWconfig.py for '+\
                      'an example.  Use the file name minus the .py extension.')
    parser.add_option('--NP', action='store_true', dest='NP', default=False,
                      help='put NP on the plot')
    parser.add_option('--Err', dest='Err', default=-1., type='float',
                      help='error band level')
    (opts, args) = parser.parse_args()

    import pyroot_logon
    config = __import__(opts.modeConfig)

    from ROOT import TFile, Double, Long, gROOT, TCanvas
    ## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
    gROOT.ProcessLine('.L EffTableReader.cc+')
    gROOT.ProcessLine('.L EffTableLoader.cc+')
    gROOT.ProcessLine('.L RooWjjFitterUtils.cc+')
    gROOT.ProcessLine('.L RooWjjMjjFitter.cc+')
    from ROOT import RooWjjMjjFitter, RooFit, \
         RooMsgService, RooArgList, RooArgSet
    from math import sqrt

    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

    fitterPars = config.theConfig(opts.Nj, opts.mcdir, opts.startingFile,
                                  opts.toydataFile )

    theFitter = RooWjjMjjFitter(fitterPars)

    theFitter.makeFitter(False)
    theFitter.loadData()

    theFitter.loadParameters(opts.startingFile)

    (frames, cans) = plot2BodyDist(theFitter, fitterPars,
                                   0, 3, opts.Err, opts.NP)
