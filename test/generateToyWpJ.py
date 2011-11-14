
import pyroot_logon

def generate (outdir, Nj, mode = "HWWconfig"):
    config = __import__(mode)
    from ROOT import gROOT
    gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
    from ROOT import RooWjjFitterUtils, TH1, TH1D
    fitParams = config.theConfig(Nj)
    fitUtils = RooWjjFitterUtils(fitParams);
##     fitUtils.setJES_scale(0, fitParams.JES_scale1);
##     fitUtils.setJES_scale(0, fitParams.JES_scale2);

    theModes = ["WpJ_CMSSW428.root", "WpJmatchingup_CMSSW428.root",
                "WpJmatchingdown_CMSSW428.root", "WpJscaleup_CMSSW428.root",
                "WpJscaledown_CMSSW428.root"]
    theChannels = ["mu", "el"]

    for mode in theModes:
        for chan in theChannels:
            outfname = chan + '_' + mode
            infname = fitParams.WpJDirectory + outfname
            outfname = outdir + outfname

            sampleName = mode.split('_')[0] + '_' + chan
            print infname, '->', outfname
            theHist = fitUtils.File2Hist(infname, sampleName, 1);
            ## print theHist
            if (theHist):
                ## theHist.Print()
                fitUtils.hist2RandomTree(theHist, outfname)

if (__name__ == "__main__"):
    generate('./', 2)
