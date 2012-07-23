from RooWjj2DFitterPars import Wjj2DFitterPars
from ROOT import TH2D, TFile, gDirectory, TTreeFormula, RooDataHist, \
    RooHistPdf, RooArgList, RooArgSet, RooFit, RooDataSet, RooRealVar, \
    TRandom3
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
        numLumiEpochs = len(self.pars.lumiPerEpoch)
        for eff in self.pars.effToDo:
            setattr(self, '%sEffTables' % eff, {})
            effTables = getattr(self, '%sEffTables' % eff)
            files = getattr(self.pars, '%sEffFiles' % eff)
            for key in files:
                effTables[key] = []
                for theFile in files[key]:
                    print eff,key,':',theFile
                    effTables[key].append(EffLookupTable(theFile))
                if (len(effTables[key]) != numLumiEpochs):
                    assert False, "Number of lumi epocs mismatch"
            # print effTables

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
        doEffWgt = (self.pars.doEffCorrections and not cutOverride)
        
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
    def File2Dataset(self, fnames, dsName, ws, noCuts = False, 
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
            
        if not (type(fnames) == type([])):
            fnames = [fnames]
        for fname in fnames:
            for (v1val, v2val, effWgt) in self.TreeLoopFromFile(fname, noCuts):
                v1.setVal(v1val)
                v2.setVal(v2val)
                ds.add(cols, effWgt)

        getattr(ws, 'import')(ds)

        return ws.data(dsName)

    def reduceDataset(data, scale = 1.0):
        print 'reducing',data.GetName(),'by scale %.4g' % scale
        newData = data.emptyClone(data.GetName() + '_reduced')
        rnd = TRandom3()
        for i in range(0,data.numEntries()):
            if (rnd.Rndm() < scale):
                newData.add(data.get(i), data.weight())
        return newData

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
                                      epoch, **(getattr(self,
                                                        '%sEffTables' % (eff)))
                                      )

        #print 'evtWgt:',evtWgt
        return evtWgt

    # take a dictionary of efficiency tables and compute the efficiency
    def computeEff(self, pt, eta, epoch, **tables):
        eff = 1.0
        #print 'efficiency tables:',tables
        for key in tables:
            eff *= tables[key][epoch].GetEfficiency(pt, eta)

        return eff

    # various analytic models that can be selected easily, the model is 0-9.
    def analyticPdf(self, ws, var, model, pdfName, idString = None, 
                    yieldVar = None):
        if ws.pdf(pdfName):
            return ws.pdf(pdfName)

        #this checks the model is in the right range.  don't go past 9 or
        #it could break the fitter that this supports.
        assert model>=0 and model < 10, "invalid model number: %s" % (model)

        if model==0:
            # exponential model
            ws.factory('RooExponential::%s(%s,c_%s[-10,10])' % \
                           (pdfName, var, idString)
                       )
        elif model==1:
            # power-law model
            ws.factory("EXPR::%s('1./TMath::Power(@0-@2, @1)'" % pdfName + \
                           ",%s,power_%s[-5,5],offset_%s[0])" % \
                           (var, idString, idString)
                       )
        elif model== 2:
            # erf turn on model
            ws.factory("EXPR::%s('(TMath::Erf((@0-@1)/@2)+1)/2.'" % pdfName +\
                           ",%s,turnOn_%s[10, 200],width_%s[0, 100])" %\
                           (var, idString, idString)
                       )
        elif model==3:
            # a Gaussian model
            ws.factory("RooGaussian::%s(%s,mean_%s[0,1000],sigma_%s[0,500])" %\
                           (pdfName, var, idString, idString)
                       )
        elif model==4:
            # a second order polynomial
            ws.factory("RooChebychev::%s(%s,{a1_%s[-10,10],a2_%s[-10,10]})" %\
                           (pdfName, var, idString, idString)
                       )
        elif model==5:
            # a double gaussian
            ws.factory("RooGaussian::%s_core" % pdfName + \
                           "(%s, mean_%s_core[0,1000],sigma_%s_core[0,500])" %\
                           (var, idString, idString)
                       )
            ws.factory("RooGaussian::%s_tail" % pdfName + \
                           "(%s, mean_%s_tail[0,1000],sigma_%s_tail[0,500])" %\
                           (var, idString, idString)
                       )
            ws.factory("SUM::%s(f_%s_core[0.5,0,1] * %s_core, %s_tail)" % \
                           (pdfName, idString, pdfName, pdfName)
                       )
        elif model==6:
            # a double gaussian with same means
            ws.factory("RooGaussian::%s_core" % pdfName + \
                           "(%s, mean_%s[0,1000],sigma_%s_core[0,500])" %\
                           (var, idString, idString)
                       )
            ws.factory("RooGaussian::%s_tail" % pdfName + \
                           "(%s, mean_%s,sigma_%s_tail[0,500])" %\
                           (var, idString, idString)
                       )
            ws.factory("SUM::%s(f_%s_core[0.5,0,1] * %s_core, %s_tail)" % \
                           (pdfName, idString, pdfName, pdfName)
                       )
        else:
            # this is what will be returned if there isn't a model implemented
            # for a given model code.
            ws.factory("RooChebychev::%s(%s,{a1_%s[-10,10],a2_%s[-10,10]})" %\
                           (pdfName, var, idString, idString)
                       )

        return ws.pdf(pdfName)

    reduceDataset = staticmethod(reduceDataset)



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

    dataset = utils.File2Dataset(params.MCDirectory + \
                                     'RD_mu_HWWMH250_CMSSW525_private.root', 
                                 "H250SignalData", theWS)
    dataset.Print()

    for model in range(5):
        utils.analyticPdf(theWS, params.var1, model, 'pdf_%i' % model,
                          '%i_%s' % (model,params.var1))

    theWS.Print()
