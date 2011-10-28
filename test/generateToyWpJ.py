
import pyroot_logon
import WjjFitterConfigs
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
from ROOT import RooWjjFitterUtils, TH1, TH1D

def generate (outdir, Nj, mode = "HWWconfig"):
    fitParams = WjjFitterConfigs.__dict__[mode](Nj)
    fitUtils = RooWjjFitterUtils(fitParams.nbins, fitParams.minMass,
                                 fitParams.maxMass, fitParams.njets,
                                 fitParams.cuts, fitParams.var,
                                 fitParams.treeName)
    fitUtils.setJES_scale(0, fitParams.JES_scale1);
    fitUtils.setJES_scale(0, fitParams.JES_scale2);

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
