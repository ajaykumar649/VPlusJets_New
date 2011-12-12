import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, e_FSU, e_FMU, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.WpJDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = '/uscms_data/d1/kalanand/WjjTrees/NewReducedQCDTrees/'
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    fitterPars.DataDirectory = '/uscms_data/d1/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_2p1invfb.root'
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_2p1invfb_SinglE_EleH.root'
    fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.fitToyDataset = False
    fitterPars.ToyDatasetDirectory = '/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSets/'
    fitterPars.toydataFile = toydataFile
    fitterPars.minMass = 60.
    fitterPars.maxMass = 400.
    fitterPars.nbins = 34

##     fitterPars.binEdges.push_back(fitterPars.minMass)

##     binEdge = fitterPars.minMass
##     print binEdge,' ',
##     while (binEdge <= fitterPars.maxMass):
##         binEdge += round(0.1*binEdge)
##         fitterPars.binEdges.push_back(binEdge)
##         print binEdge,' ',

##     fitterPars.maxMass = fitterPars.binEdges[fitterPars.binEdges.size()-1]

    print "mass range:",fitterPars.minMass,'-',fitterPars.maxMass
    
##     binEdge = fitterPars.minMass + 5.
##     while (binEdge <= 80.) and (binEdge <= fitterPars.maxMass):
##         fitterPars.binEdges.push_back(binEdge)
##         binEdge += 5.

##     binEdge += 5.
##     while (binEdge <= 170.) and (binEdge <= fitterPars.maxMass):
##         fitterPars.binEdges.push_back(binEdge)
##         binEdge += 10.

##     binEdge += 5.
##     while (binEdge <= 240) and (binEdge <= fitterPars.maxMass):
##         fitterPars.binEdges.push_back(binEdge)
##         binEdge += 15.
    
##     binEdge += 5.
##     while (binEdge <= 400) and (binEdge <= fitterPars.maxMass):
##         fitterPars.binEdges.push_back(binEdge)
##         binEdge += 20.

##     binEdge += 5.
##     while (binEdge <= fitterPars.maxMass):
##         fitterPars.binEdges.push_back(binEdge)
##         binEdge += 25.

    fitterPars.truncRange = True
    fitterPars.minTrunc = 130.

    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minTrunc:
            fitterPars.minTrunc = binMin
            break
        binMin = binEdge

    fitterPars.maxTrunc = 170.
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxTrunc:
            fitterPars.maxTrunc = binEdge
            break

    print 'truncate:',fitterPars.truncRange,':', fitterPars.minTrunc,'-',\
          fitterPars.maxTrunc

    fitterPars.njets = Nj
    fitterPars.constrainDiboson = True

    fitterPars.doEffCorrections = True

    fitterPars.useExternalMorphingPars = False
    fitterPars.e_fSU = e_FSU
    fitterPars.e_fMU = e_FMU
       
    fitterPars.cuts = '(W_mt > 50.) ' + \
                      '&& (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) ' + \
                      '&& (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor<0.7) ' + \
                      '&& (JetPFCor_Pt[0] > 40) '

    return fitterPars
