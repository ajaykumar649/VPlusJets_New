
def morph(hist1, hist2, mass1, mass2, targetMass, debug = False):
    from ROOT import gROOT, RooRealVar, RooIntegralMorph,\
        kRed,kBlue,kViolet, RooDataHist, RooArgSet,\
        RooArgList, RooHistPdf, RooAbsReal, RooFit
    
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

    x = RooRealVar("x", "x", histLow.GetXaxis().GetXmin(), histLow.GetXaxis().GetXmax())
    mAlpha = 1.0 - float(targetMass-massLow)/float(massHigh-massLow)
    print 'alpha:',mAlpha,
    newIntegral = histLow.Integral() + \
        (targetMass - massLow) * \
        (histHigh.Integral()-histLow.Integral()) / \
        (massHigh-massLow)
    print 'low integral:', histLow.Integral(), \
        'high integral:', histHigh.Integral(), \
        'new integral:', newIntegral



    newHistLow = RooDataHist("sigHistLow_hist", "sigHistLow_hist", RooArgList(x), histLow);
    sigHistLow = RooHistPdf("sigHistLow", "sigHistLow", RooArgList(x),
				 RooArgList(x), newHistLow);

    newHistHigh = RooDataHist("sigHistHigh_hist", "sigHistHigh_hist", RooArgList(x), histHigh);
    sigHistHigh = RooHistPdf("sigHistHigh", "sigHistHigh", RooArgList(x),
				 RooArgList(x), newHistHigh);
    
      
    #x.Print("v")
    x.setBins( histLow.GetNbinsX(), "cache")
    alpha_morph = RooRealVar("alpha_morph", "#alpha_{morph}", mAlpha, 0., 1.)
    alpha_morph.setBins(10,"cache") ;

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
                                                hist1.GetName()), x, RooFit.Binning(histLow.GetNbinsX()))
    morphHist.Scale(newIntegral/morphHist.Integral())
    morphHist.SetName(re.sub(r'\d+', '%i' % targetMass, hist1.GetName()))
    
    # if debug:
    #     morphHist.Print()

    return morphHist



############################################################
############################################################
############################################################

def morphAllLambda(basisFile, lambdaZ, dKappaGamma, morphedHists, debug = False):
    fbasis = TFile(basisFile)
    gROOT.cd()
    
    
    for i in range(1,4): 
        h0Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.1f' % (lambdaZ, dKappaGamma)
        h1Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.1f' % (lambdaZ+0.04, dKappaGamma)
        morphName = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.2f' % (lambdaZ+0.01*i, dKappaGamma)
        

        hist0 = fbasis.Get(h0Name)
        hist1 = fbasis.Get(h1Name)
        if i==lambdaZ:
            morphedHists.append(hist0)
            morphedHists.append(hist1)            

        
        gROOT.cd()
        morphedhist = morph(hist0, hist1, lambdaZ, lambdaZ+0.04, 
                                  lambdaZ+0.01*i, debug)
        morphedhist.SetName(morphName)
        morphedHists.append(morphedhist)

        if debug:
            hist0.SetLineColor(kBlue)
            hist1.SetLineColor(kRed)
            if mHbasis < mHmorph:
                hist0.Draw('hist')
                hist1.Draw('histsame')
            else:
                hist1.Draw('hist')
                hist0.Draw('histsame')                
                
            morphedHists[-1].SetLineStyle(9)
            morphedHists[-1].Draw('histsame')
            gPad.Update()
            gPad.WaitPrimitive()
############################################################
############################################################
############################################################


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
    parser.add_option('-c', '--channel', dest='channel', type='int', default=0,
                      help='channel 0: mu_noBtag, 1: el_noBtag, 2: mu_btag, 3: el_btag')

    parser.add_option('--debug', action='store_true', dest='debug', 
                      default=False, help='stop when things aren\'t good')

    (opts, args) = parser.parse_args()


    assert (opts.channel >= 0)
    assert (opts.channel <= 3)

    basisFile = "mu_noBtag.root"
    if opts.channel == 0:
        basisFile = "mu_noBtag.root"
    if opts.channel == 1:
        basisFile = "el_noBtag.root"
    if opts.channel == 2:
        basisFile = "mu_btag.root"
    if opts.channel == 3:
        basisFile = "el_btag.root"

 
    from ROOT import TFile, gROOT, kRed, kBlue, kViolet, RooMsgService, RooFit
    from ROOT import gPad
    import re

 

    print 'file name: ', basisFile
    morphedHists = []
    dKappaGamma = 0.2
    lambdaZMin  = 0.2
    
    for i in range(1,20):
        lambdaZ = -0.4+0.04*i
        if lambdaZ >= 0.4:
            break
        if lambdaZ > -0.09 and lambdaZ < 0:
            continue
        print '================ lambdaZ: ', lambdaZ
        morphAllLambda(basisFile, lambdaZ, dKappaGamma, morphedHists, opts.debug)
    
    foutput = TFile('new_%s' % (basisFile), 'recreate')
    for hist in morphedHists:
        hist.Write()

    foutput.Close()
