import pyroot_logon
from ROOT import gROOT
gROOT.ProcessLine('.L RooWjjFitterParams.h+');
from ROOT import RooWjjFitterParams

def theConfig(Nj, mcdir = '', initFile = '', toydataFile = ''):
    fitterPars = RooWjjFitterParams()
    fitterPars.MCDirectory = '/uscms_data/d2/kalanand/WjjTrees/Full2011DataFall11MC/ReducedTree/RD_'
    fitterPars.WpJDirectory = fitterPars.MCDirectory
    if (len(mcdir) > 0):
        fitterPars.WpJDirectory = mcdir
        fitterPars.toyWpJ = True
    fitterPars.QCDDirectory = '/uscms_data/d2/kalanand/WjjTrees/NewReducedQCDTrees/'
    fitterPars.initParamsFile = initFile
    # fitterPars.constraintParamsFile = "HWWConstraints2Jets.txt";
    fitterPars.DataDirectory = fitterPars.MCDirectory
    fitterPars.muonData = 'WmunuJets_DataAll_GoldenJSON_4p7invfb.root'
    fitterPars.includeMuons = False
    fitterPars.electronData = 'WenuJets_DataAll_GoldenJSON_4p7invfb.root'
    fitterPars.includeElectrons = True
    fitterPars.NewPhysicsDirectory = '/uscms_data/d2/kalanand/WjjTrees/ReducedTree/NewKfitRDTree/RD_'
    fitterPars.fitToyDataset = False
    fitterPars.ToyDatasetDirectory = '/uscms_data/d1/ilyao/KinematicFitterS11/ErrorScans/1KMCSets/'
    fitterPars.toydataFile = toydataFile
    fitterPars.minMass = 30.
    fitterPars.maxMass = 400.
    fitterPars.nbins = 34
    fitterPars.intLumi = 4700.

    fitterPars.binEdges.push_back(fitterPars.minMass)

    binEdge = fitterPars.minMass
    print binEdge,' ',
    while (binEdge <= fitterPars.maxMass):
        binEdge += round(0.15*binEdge)
        fitterPars.binEdges.push_back(binEdge)
        print binEdge,' ',

    fitterPars.binEdges[fitterPars.binEdges.size()-1] = fitterPars.maxMass
##     fitterPars.maxMass = fitterPars.binEdges[fitterPars.binEdges.size()-1]
    
    print "mass range:",fitterPars.minMass,'-',fitterPars.maxMass
    
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

    fitterPars.minFit = 60.
    binMin = fitterPars.minMass
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.minFit:
            fitterPars.minFit = binMin
            break
        binMin = binEdge

    fitterPars.maxFit = 400.
    for binEdge in fitterPars.binEdges:
        if binEdge > fitterPars.maxFit:
            fitterPars.maxFit = binEdge
            break

    print 'truncate:',fitterPars.truncRange,':', fitterPars.minTrunc,'-',\
          fitterPars.maxTrunc
    print 'fit range:',fitterPars.minFit,'-',fitterPars.maxFit

    fitterPars.njets = Nj
    fitterPars.constrainDiboson = True

    fitterPars.doEffCorrections = True

    fitterPars.useExternalMorphingPars = False
       
    fitterPars.cuts = '(W_mt > 50.) ' + \
                      '&& (abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2) ' + \
                      '&& (sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor>0.3) ' + \
                      '&& (JetPFCor_Pt[1]/Mass2j_PFCor<0.7) ' + \
                      '&& (JetPFCor_Pt[0] > 40) '

    return fitterPars
