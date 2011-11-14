import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.WpJDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewReducedQCDTrees/"
    fitterPars.DataDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.NewPhysicsDirectory = "root://cmssrv32.fnal.gov//store/user/lnujj/WjjTrees/NewKfitRDTree/RD_"
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_2p1invfb.root'
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_2p1invfb_SinglE_EleH.root'

    fitterPars.initParamsFile = initFile

    fitterPars.minMass = 60.
    fitterPars.maxMass = 300.
    fitterPars.nbins = 24
    fitterPars.truncRange = True
    fitterPars.minTrunc = 130.
    fitterPars.maxTrunc = 170.
    fitterPars.njets = Nj

    fitterPars.cuts = '(W_mt > 50.) ' + \
                      '&& (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) ' + \
                      '&& (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor<0.7) '

    return fitterPars
