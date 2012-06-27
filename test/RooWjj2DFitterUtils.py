from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import TH2D, TFile, gDirectory, TTreeFormula, RooDataHist, \
    RooHistPdf, RooArgList, RooArgSet, RooFit, RooDataSet
from array import array
from EffLookupTable import EffLookupTable
import random

import warnings
warnings.filterwarnings( action='ignore', category=RuntimeWarning, message='creating converter for unknown type "const char\*\*".*' )

class Wjj2DFitterUtils:
    # initialization for the class
    def __init__(self, pars):
        self.pars = pars

        self.files2EffTables()
        print 'full cuts:',self.fullCuts()

    # read files and store efficiency tables
    def files2EffTables(self):
        numLumiEpochs = None
        for eff in self.pars.effToDo:
            files = getattr(self.pars, '%sEffFiles' % eff)
            setattr(self, '%sEffTables' % eff, {})
            for key in files:
                getattr(self, '%sEffTables' % eff)[key] = []
                for file in files[key]:
                    getattr(self, '%sEffTables' % eff)[key].append(EffLookupTable(files[key]))
                if (numLumiEpochs == None):
                    numLumiEpochs = len(getattr(self, '%sEffTables' % eff)[key])
                else:
                    if (len(getattr(self, '%sEffTables' % eff)[key]) != numLumiEpochs):
                        assert False, "Number of lumi epocs mismatch"

    # create a new empty 2D histogram with the appropriate binning
    def newEmptyHist(self, histName):
        newHist = None
        if (len(self.pars.v1binEdges) > 1) and (len(self.pars.v2binEdges) > 1):
            newHist = TH2D(histName, histName, 
                           len(self.pars.v1binEdges)-1, 
                           array('d', self.pars.v1binEdges),
                           len(self.pars.v2binEdges)-1, 
                           array('d', self.pars.v2binEges))
        elif (len(self.pars.v1binEdges) > 1) and \
                (len(self.pars.v2binEdges) < 2):
            newHist = TH2D(histName, histName, 
                           len(self.pars.v1binEdges)-1, 
                           array('d', self.pars.v1binEdges),
                           self.pars.v2nbins, self.pars.v2min, self.pars.v2max)
        elif (len(self.pars.v1binEdges) < 2) and \
                (len(self.pars.v2binEdges) > 1):
            newHist = TH2D(histName, histName, 
                           self.pars.v2nbins, self.pars.v1min, self.pars.v1max,
                           len(self.pars.v2binEdges)-1, 
                           array('d', self.pars.v2binEges))
        else:
            newHist = TH2D(histName, histName, 
                           self.pars.v2nbins, self.pars.v1min, self.pars.v1max,
                           self.pars.v2nbins, self.pars.v2min, self.pars.v2max)

        return newHist

    # the cuts to be applied to any data
    def fullCuts(self):
        theCut = '(%s>%0.3f)&&(%s<%0.3f)' % (self.pars.var1, self.pars.v1min,
                                              self.pars.var1, self.pars.v1max)
        theCut += '&&(%s>%0.3f)&&(%s<%0.3f)' % (self.pars.var2, self.pars.v2min,
                                                self.pars.var2, self.pars.v2max)
        if len(self.pars.cuts) > 0:
            theCut += '&&(%s)' % self.pars.cuts
        return '(%s)' % theCut
            
    # generator function for looping over an event tree and applying the cuts
    def TreeLoopFromFile(self, fname, noCuts = False,
                         cutOverride = None):

        # open file and get tree
        treeFile = TFile.Open(fname)
        theTree = treeFile.Get(self.pars.treeName)

        if not theTree:
            print 'failed to find tree %s in file %s' % (self.pars.treeName, 
                                                         fname)
            return

        # get the right cuts
        if cutOverride:
            theCuts = cutOverride
        elif noCuts:
            theCuts = ''
        else:
            theCuts = self.fullCuts()

        # create an entry list which apply the cuts to the tree
        if gDirectory.Get('cuts_evtList'):
            gDirectory.Delete('cuts_evtList')
        theTree.Draw('>>cuts_evtList', theCuts, 'entrylist')
        theList = gDirectory.Get('cuts_evtList')

        # create fomulae for the variables of interest
        v1val = TTreeFormula('var1', self.pars.var1, theTree)
        v2val = TTreeFormula('var2', self.pars.var2, theTree)

        # loop over the selected events calculate their weight and yield
        # the two variable values and the weight for each selected event.
        print "selected events:",theList.GetN()
        while theTree.GetEntry(theList.Next()):
            if self.pars.isElectron:
                lep_pt = theTree.W_electron_pt
                lep_eta = theTree.W_electron_eta
            else:
                lep_pt = theTree.W_muon_pt
                lep_eta = theTree.W_muon_eta
            jet_pt = []
            jet_eta = []
            for (idx, pt) in enumerate(theTree.JetPFCor_Pt):
                if pt > 0:
                    jet_pt.append(pt)
                    jet_eta.append(theTree.JetPFCor_Eta[idx])
            effWgt = self.effWeight(lepton_pt = lep_pt, lepton_eta = lep_eta, 
                                    #jet_pt = jet_pt, jet_eta,
                                    mt_pt = theTree.W_mt, mt_eta = lep_eta,
                                    met_pt = theTree.event_met_pfmet, 
                                    met_eta = 0.)
            yield (v1val.EvalInstance(), v2val.EvalInstance(), effWgt)

        return
        

    # from a file fill a 2D histogram
    def File2Hist(self, fname, histName, noCuts = False, 
                  cutOverride = None):
        theHist = self.newEmptyHist(histName)

        print 'filename:',fname
        doEffWgt = (self.pars.doEffCorrections and (len(cutOverride)<1))
        
        for (v1val, v2val, effWgt) in self.TreeLoopFromFile(fname, 
                                                            noCuts, 
                                                            cutOverride):
            #print 'entry:',v1val,v2val,effWgt
            if not doEffWgt:
                effWgt = 1.0
            theHist.Fill(v1val, v2val, effWgt)

        return theHist

    # take a 2D histogram and create a RooHistPdf
    def Hist2Pdf(self, hist, pdfName, ws, order = 0):
        if ws.pdf(pdfName):
            return ws.pdf(pdfName)

        varList = RooArgList(ws.var(self.pars.var1), ws.var(self.pars.var2))
        newHist = RooDataHist(pdfName + '_hist', pdfName + '_hist',
                              varList, hist)
        
        obsSet = RooArgSet(varList)
        thePdf = RooHistPdf(pdfName, pdfName, obsSet, 
                            newHist, order)
        getattr(ws, 'import')(thePdf)

        return ws.pdf(pdfName)

    # from a file fill and return a RooDataSet
    def File2Dataset(self, fname, dsName, ws, noCuts = False, 
                     weighted = False):
        if ws.data(dsName):
            return ws.data(dsName)

        v1 = ws.var(self.pars.var1)
        v2 = ws.var(self.pars.var2)
        cols = RooArgSet(v1, v2)
        if weighted:
            evtWgt = RooRealVar('evtWgt', 'evtWgt', 1.0)
            cols.add(evtWgt)
            ds = RooDataSet(dsName, dsName, cols, 'evtWgt')
        else:
            ds = RooDataSet(dsName, dsName, cols)

        for (v1val, v2val, effWgt) in self.TreeLoopFromFile(fname, isElectron,
                                                            noCuts):
            v1.setVal(v1val)
            v2.setVal(v2val)
            ds.add(cols, effWgt)

        getattr(ws, 'import')(ds)

        return ws.data(dsName)

    # calculate the efficiency weight for a give event using the supplied
    # tables.
    def effWeight(self, **kinematics):
        # select which lumi/trigger epoch we are going to use for the 
        # computation
        epochSelector = random.random()
        sumLumi = 0.
        epoch = -1
        while (epochSelector > sumLumi/self.pars.integratedLumi) and \
                (epoch < len(self.pars.lumiPerEpoch)-1):
            epoch += 1
            sumLumi += self.pars.lumiPerEpoch[epoch]

        evtWgt = 1.0
        for eff in self.pars.effToDo:
            evtWgt *= self.computeEff(kinematics['%s_pt' % (eff)],
                                      kinematics['%s_eta' % (eff)],
                                      epoch, **(getattr(self.pars, 
                                                        '%sEffFiles' % (eff)))
                                      )

        return evtWgt

    # take a dictionary of efficiency tables and compute the efficiency
    def computeEff(self, pt, eta, epoch, **tables):
        eff = 1.0
        for key in tables:
            eff *= tables[key][epoch].GetEfficiency(pt, eta)

        return eff

if __name__ == '__main__':

    import pyroot_logon
    from ROOT import gPad, RooWorkspace

    params = Wjj2DFitterPars()
    utils = Wjj2DFitterUtils(params)

    dataHist = utils.File2Hist(params.MCDirectory + \
                                   'RD_mu_HWWMH250_CMSSW525_private.root',
                               'dataHist')
    dataHist.Print()
    # dataHist.Draw('colz')
    # gPad.Update()
    # gPad.WaitPrimitive()

    theWS = RooWorkspace()
    theWS.factory('%s[%f,%f]' % (params.var1, params.v1min, params.v1max))
    theWS.factory('%s[%f,%f]' % (params.var2, params.v2min, params.v2max))
    theWS.defineSet('obsSet', '%s,%s' % (params.var1, params.var2))

    #theWS.Print()
    utils.Hist2Pdf(dataHist, "H250SignalHist", theWS)
    #theWS.Print()

    utils.File2Dataset(params.MCDirectory + \
                           'RD_mu_HWWMH250_CMSSW525_private.root', 
                       "H250SignalData", theWS)
    theWS.Print()
