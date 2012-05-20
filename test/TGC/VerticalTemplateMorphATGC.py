############################################################
############################################################
############################################################
def morphAllLambda(basisFile, lambdaZ, dKappaGamma, morphedHists):
    fbasis = TFile(basisFile)
    gROOT.cd()
        
    for i in range(0,5):
        if i==0:
            lambdaZ = lambdaZ + 0.0001
            dKappaGamma = dKappaGamma + 0.0001
        h0Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.1f' % (lambdaZ, dKappaGamma)
        h1Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.1f' % (lambdaZ+0.04, dKappaGamma)
        morphName = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.2f' % (lambdaZ+0.01*i, dKappaGamma)
        #print 'h0 name: ', h0Name, ', h1 name: ', h1Name

        hist0 = fbasis.Get(h0Name)
        hist1 = fbasis.Get(h1Name)
        
        if i==0:
            morphedhist = hist0.Clone(morphName)
        elif i==4:
            morphedhist = hist1.Clone(morphName)
        else:
            morphedhist = hist0.Clone(morphName);
            morphedhist.Add(hist0, hist1, 1.0-0.25*i, 0.25*i)
           
        morphedhist.SetName(morphName)
        morphedHists.append(morphedhist)
############################################################
############################################################
############################################################
############################################################
############################################################
############################################################
def morphAllKappa(fbasis, lambdaZ, dKappaGamma, morphedHists):
    #fbasis = TFile(basisFile)
    gROOT.cd()
        
    for i in range(1,4):
        h0Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.2f' % (lambdaZ, dKappaGamma)
        h1Name = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.2f' % (lambdaZ, dKappaGamma+0.2)
        morphName = 'signal_lambdaZ_%.2f_deltaKappaGamma_%.2f' % (lambdaZ, dKappaGamma+0.05*i)
        
        hist0 = fbasis.Get(h0Name)
        hist1 = fbasis.Get(h1Name)        
     
        morphedhist = hist0.Clone(morphName);
        morphedhist.Add(hist0, hist1, 1.0-0.25*i, 0.25*i)
        morphedhist.SetName(morphName)
        morphedHists.append(morphedhist)
############################################################
############################################################
############################################################
        
if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option('-c', '--channel', dest='channel', type='int', default=0,
                      help='channel 0: mu_noBtag, 1: el_noBtag, 2: mu_btag, 3: el_btag')
    (opts, args) = parser.parse_args()

    assert (opts.channel >= 0)
    assert (opts.channel <= 3)

    basisFile = "mu_noBtag.root"
    bkgUphistName = "background_munobtag_backshapeUp"
    bkgDownhistName = "background_munobtag_backshapeDown"
    
    if opts.channel == 0:
        basisFile = "mu_noBtag.root"
        bkgUphistName = "background_munobtag_backshapeUp"
        bkgDownhistName = "background_munobtag_backshapeDown"        
    if opts.channel == 1:
        basisFile = "el_noBtag.root"
        bkgUphistName = "background_elnobtag_backshapeUp"
        bkgDownhistName = "background_elnobtag_backshapeDown"        
    if opts.channel == 2:
        basisFile = "mu_btag.root"
        bkgUphistName = "background_mubtag_backshapeUp"
        bkgDownhistName = "background_mubtag_backshapeDown"        
    if opts.channel == 3:
        basisFile = "el_btag.root"
        bkgUphistName = "background_elbtag_backshapeUp"
        bkgDownhistName = "background_elbtag_backshapeDown"

 
    from ROOT import TFile, gROOT, kRed, kBlue, kViolet, RooMsgService, RooFit
    from ROOT import gPad
    import re

    print 'file name: ', basisFile
    originalHists = []
    morphedHistsInLambda = []
    morphedHistsInKappa = []

    foutput = TFile('new_%s' % (basisFile), 'recreate')
    fbasis = TFile(basisFile)
    originalHists.append(fbasis.Get('data_obs'))
    originalHists.append(fbasis.Get('background'))
    originalHists.append(fbasis.Get(bkgUphistName))
    originalHists.append(fbasis.Get(bkgDownhistName))

        
    for i in range(0,20):
        lambdaZ = -0.4+0.04*i
        if lambdaZ > 0.41:
            break
        print '================ lambdaZ: ', lambdaZ
        for j in range(0,7):
            morphAllLambda(basisFile, lambdaZ, -0.6+0.2*j, morphedHistsInLambda)
    
    foutput.cd()
    for hist in morphedHistsInLambda:
        hist.Write()

    for i in range(0,6):
        kappa = -0.6+0.2*i
        if kappa > 0.61:
            break
        print '================ deltaKappaGamma: ', kappa
        for j in range(0,81):
            morphAllKappa(foutput, -0.4+0.01*j, kappa, morphedHistsInKappa)

    foutput.cd()
    for hist in morphedHistsInKappa:
        hist.Write()

    for hist in originalHists:
        hist.Write()
        
    foutput.Close()
