from RooWjj2DFitterUtils import Wjj2DFitterUtils
from ROOT import RooWorkspace, RooAddPdf, RooAbsReal, RooFit, RooCmdArg, \
    RooBinning, RooAbsData, RooHist, RooArgList, RooArgSet, \
    kRed, kBlue, kGreen, kYellow, kGray, kAzure, kCyan
from math import sqrt
from array import array
import sys

colorwheel = [ kAzure+8,
               kRed,
               kGreen+2,
               kGray+1,
               kYellow,
               kCyan+2
               ]
class Wjj2DFitter:

    def __init__ (self, pars):
        self.pars = pars
        self.ws = RooWorkspace('wjj2dfitter')
        self.utils = Wjj2DFitterUtils(self.pars)

        var1 = self.ws.factory('%s[%f,%f]' % (self.pars.var1, self.pars.v1min, 
                                              self.pars.v1max))
        var1.setUnit('GeV')
        try:
            var1.SetTitle(self.pars.v1title)
        except AttributeError:
            var1.SetTitle('m_{jj}')
        var1.setPlotLabel(var1.GetTitle())
        if len(self.pars.v1binEdges) > 1:
            v1binning = RooBinning(len(self.pars.v1binEdges) - 1, 
                                   array('d', self.pars.v1binEdges),
                                   '%sBinning' % self.pars.var1)
            var1.setBinning(v1binning)
            #var1.Print('v')
        else:
            var1.setBins(self.pars.v1nbins)
        var2 = self.ws.factory('%s[%f,%f]' % (self.pars.var2, self.pars.v2min, 
                                              self.pars.v2max))
        var2.setUnit('GeV')
        try:
            var2.SetTitle(self.pars.v1title)
        except AttributeError:
            var2.SetTitle('m_{WW}')
        var2.setPlotLabel(var2.GetTitle())
        if len(self.pars.v2binEdges) > 1:
            v2binning = RooBinning(len(self.pars.v2binEdges) - 1, 
                                   array('d', self.pars.v2binEdges),
                                   '%sBinning' % self.pars.var2)
            var2.setBinning(v2binning)
        else:
            var2.setBins(self.pars.v2nbins)
        self.ws.defineSet('obsSet', '%s,%s' % (self.pars.var1, self.pars.var2))
    
    # put together a fitting model and return the pdf
    def makeFitter(self):
        if self.ws.pdf('totalPdf'):
            return self.ws.pdf('totalPdf')

        compPdfs = []
        compYields = []
        for component in self.pars.backgrounds:
            compFiles = getattr(self.pars, '%sFiles' % component)
            compModels = getattr(self.pars, '%sModels' % component)
            for compPdf in self.makeComponentPdf(component, compFiles, 
                                                 compModels):
                compPdfs.append(compPdf[0])
                compYields.append(compPdf[1])

        for component in self.pars.signals:
            compFile = getattr(self.pars, '%sFiles' % component)
            compModels = getattr(self.pars, '%sModels' % component)
            for compPdf in self.makeComponentPdf(component, compFiles,
                                                 compModels):
                compPdfs.append(compPdf[0])
                self.ws.factory("expr::f_n_%s('@0*@1',n_%s,mu_%s[1.0])" % \
                                    (component, component, component)
                                )
                compPdf[1].setConstant(True)
                self.ws.var('mu_%s' % component).setConstant(False)
                compYields.append(self.ws.arg('f_n_%s' % component))

        #print compPdfs
        #print compYields
        prodList = [ '%s*%s' % (compYields[idx].GetName(), pdf.GetName()) \
                         for (idx, pdf) in enumerate(compPdfs) ]
        self.ws.factory('SUM::totalPdf(%s)' % (','.join(prodList)))

        return self.ws.pdf('totalPdf')

    def makeConstraints(self):

        if self.ws.set('constraintSet'):
            return self.ws.set('constraintSet')

        constraints = []
        constrainedParameters = []
        for constraint in self.pars.yieldConstraints:
            theYield = self.ws.var('n_%s' % constraint)
            if not theYield.isConstant():
                self.ws.factory('RooGaussian::%s_const(%s, %f, %f)' % \
                                    (constraint, theYield.GetName(),
                                     theYield.getVal(), 
                                     theYield.getVal()*self.pars.yieldConstraints[constraint])
                                )
                constraints.append('%s_const' % constraint)
                constrainedParameters.append(theYield.GetName())

        if hasattr(self.pars, 'constrainShapes'):
            for component in self.pars.constrainShapes:
                pc = self.ws.pdf('%sPdf' % component).getParameters(self.ws.set('obsSet'))
                parIter = pc.createIterator()
                par = parIter.Next()
                while par:
                    if not par.isConstant():
                        theConst = self.ws.factory('RooGaussian::%s_const' % \
                                                       (par.GetName()) + \
                                                       '(%s, %f, %f)' % \
                                                       (par.GetName(),
                                                        par.getVal(),
                                                        par.getError())
                                                   )
                        constraints.append(theConst.GetName())
                        constrainedParameters.append(par.GetName())
                    par = parIter.Next()

        self.ws.defineSet('constraintSet', ','.join(constraints))
        self.ws.defineSet('constrainedSet', ','.join(constrainedParameters))

        return self.ws.set('constraintSet')


    # fit the data using the pdf
    def fit(self):
        print 'construct fit pdf ...'
        fitter = self.makeFitter()

        print 'load data ...'
        data = self.loadData()

        self.resetYields()

        constraintSet = self.makeConstraints()

        constraintCmd = RooCmdArg.none()
        if constraintSet.getSize() > 0:
            # constraints.append(fitter.GetName())
            # fitter = self.ws.factory('PROD::totalFit_const(%s)' % \
            #                              (','.join(constraints))
            #                          )
            # constraintCmd = RooFit.Constrained()
            constraintCmd = RooFit.ExternalConstraints(self.ws.set('constraintSet'))

        self.readParametersFromFile()

        self.ws.Print()

        # print constraints, self.pars.yieldConstraints
        print '\nfit constraints'
        constIter = constraintSet.createIterator()
        constraint = constIter.Next()
        while constraint:
            constraint.Print()
            constraint = constIter.Next()
            
        # for constraint in pars.constraints:
        #     self.ws.pdf(constraint).Print()
        # print

        print 'fitting ...'
        fr = fitter.fitTo(data, RooFit.Save(True),
                          RooFit.Extended(True),
                          RooFit.Minos(False),
                          RooFit.PrintEvalErrors(-1),
                          RooFit.Warnings(False),
                          constraintCmd)
        fr.Print()

        return fr

    # determine the fitting model for each component and return them
    def makeComponentPdf(self, component, files, models):
        if not self.ws.var('n_%s' % component):
            self.ws.factory('n_%s[-1000, 1e6]' % component)
        if (models[0] == -1):
            thePdf = self.makeComponentHistPdf(component, files)
        elif (models[0] == -2):
            return self.makeComponentMorphingPdf(component, files)
        else:
            thePdf = self.makeComponentAnalyticPdf(component, models)
        return [(thePdf, self.ws.var('n_%s' % component))]
            
    #create a simple 2D histogram pdf
    def makeComponentHistPdf(self, component, files):
        if self.ws.pdf('%sPdf' % component):
            return self.ws.pdf('%sPdf' % component)

        compHist = self.utils.newEmptyHist('hist%s' % component)
        sumYields = 0.
        sumxsec = 0.
        sumExpected = 0.
        for (idx,fset) in enumerate(files):
            filename = fset[0]

            tmpHist = self.utils.File2Hist(filename, 
                                           'hist%s_%i' % (component, idx))
            sumYields += tmpHist.Integral()
            sumxsec += fset[2]
            compHist.Add(tmpHist, self.pars.integratedLumi*fset[2]/fset[1])
            sumExpected += tmpHist.Integral()*fset[2]* \
                self.pars.integratedLumi/fset[1]
            print filename,'acc x eff: %.3g' % (tmpHist.Integral()/fset[1])
            print filename,'N_expected: %.1f' % \
                (tmpHist.Integral()*fset[2]*self.pars.integratedLumi/fset[1])
            #tmpHist.Print()

        #compHist.Print()
        print '%s acc x eff: %.3g' % \
            (component, sumExpected/sumxsec/self.pars.integratedLumi)
        print 'Number of expected %s events: %.1f' % (component, sumExpected)
        setattr(self, '%sExpected' % component, sumExpected)

        return self.utils.Hist2Pdf(compHist, '%sPdf' % (component), 
                                   self.ws, self.pars.order)

    # create a pdf using the "template morphing" technique
    def makeComponentMorphingPdf(self, component, files):
        # TODO: implement template morphing if needed.
        return []

    # create a pdf using an analytic function.
    def makeComponentAnalyticPdf(self, component, models):
        if self.ws.pdf('%sPdf' % (component)):
            return self.ws.pdf('%sPdf' % (component))

        pdfList = []
        for (idx,model) in enumerate(models):
            var = self.pars.var1 if idx==0 else self.pars.var2
            if abs(model) > 9:
                subList = []
                for (subidx,submodel) in enumerate(str(model)):
                    subList.append(self.utils.analyticPdf(self.ws, var, 
                                                          int(submodel),
                                                          '%sPdf_%s_%i' % \
                                                              (component,var,
                                                               subidx),
                                                          '%s_%s_%i' % \
                                                              (component, var, 
                                                               subidx)
                                                          )
                                   )
                pdfList += subList
                self.ws.factory('PROD::%sPdf_%s(%s)' % \
                                    (component, var, 
                                     ','.join([ pdf.GetName() \
                                                    for pdf in subList ])
                                     )
                                )
            else:
                pdfList.append(self.utils.analyticPdf(self.ws, var, model, 
                                                      '%sPdf_%s' % \
                                                          (component,var), 
                                                      '%s_%s'%(component,var)
                                                      )
                               )
        
        pdfList = [ pdf.GetName() for pdf in pdfList ]
        self.ws.factory('PROD::%sPdf(%s)' % (component, ','.join(pdfList)))
                        
        return self.ws.pdf('%sPdf' % component)

    def loadData(self, weight = False):
        if self.ws.data('data'):
            return self.ws.data('data')

        data = self.utils.File2Dataset(self.pars.DataFile, 'data', self.ws,
                                       weighted = weight)
        if hasattr(self, 'relMultijet'):
            self.multijetExpected = data.sumEntries()*self.relMultijet
            self.multijetError = data.sumEntries()*self.relMultijetErr

        return data

    def stackedPlot(self, var, logy = False, pdfName = None):
        if not pdfName:
            pdfName = 'totalPdf'

        xvar = self.ws.var(var)

        sframe = xvar.frame()
        sframe.SetName("%s_stacked" % var)
        pdf = self.ws.pdf(pdfName)

        if isinstance(pdf, RooAddPdf):
            compList = RooArgList(pdf.pdfList())
        else:
            compList = None

        data = self.ws.data('data')
        nexp = pdf.expectedEvents(self.ws.set('obsSet'))

        print pdf.GetName(),'expected: %.0f' % (nexp)
        print 'data events: %.0f' % (data.sumEntries())

        if nexp < 1:
            nexpt = data.sumEntries()
        theComponents = self.pars.backgrounds + self.pars.signals
        # data.plotOn(sframe, RooFit.Invisible(),
        #             RooFit.Binning('%sBinning' % (var)))
        dataHist = RooAbsData.createHistogram(data,'dataHist_%s' % var, xvar,
                                              RooFit.Binning('%sBinning' % var))
        #dataHist.Scale(1., 'width')
        invData = RooHist(dataHist, 1., 1, RooAbsData.SumW2, 1.0, True)
        #invData.Print('v')
        sframe.addPlotable(invData, 'pe', True, True)
        for (idx,component) in enumerate(theComponents):
            print '\nplotting',component,'...'
            if hasattr(self.pars, '%sPlotting' % (component)):
                plotCharacteristics = getattr(self.pars, '%sPlotting' % \
                                                  (component))
            else:
                plotCharacteristics = {'color' : colorwheel[idx%6],
                                       'title' : component }

            compCmd = RooCmdArg.none()
            if compList:
                compSet = RooArgSet(compList)
                compCmd = RooFit.Components(compSet)
                removals = compList.selectByName('%sPdf*' % component)
                compList.remove(removals)

            sys.stdout.flush()
            pdf.plotOn(sframe, RooFit.ProjWData(data),
                       RooFit.DrawOption('LF'), RooFit.FillStyle(1001),
                       RooFit.FillColor(plotCharacteristics['color']),
                       RooFit.LineColor(plotCharacteristics['color']),
                       RooFit.VLines(),
                       #RooFit.Normalization(nexp, RooAbsReal.Raw),
                       compCmd
                       )
            tmpCurve = sframe.getCurve()
            tmpCurve.SetName(component)
            tmpCurve.SetTitle(plotCharacteristics['title'])

        theData = RooHist(dataHist, 1., 1, RooAbsData.SumW2, 1.0, True)
        theData.SetName('theData')
        theData.SetTitle('data')
        sframe.addPlotable(theData, 'pe')

        if (logy):
            sframe.SetMinimum(0.01)
            sframe.SetMaximum(1.0e6)

        sframe.GetYaxis().SetTitle('Events / GeV')

        return sframe

    def readParametersFromFile(self, fname=None):
        if (not fname):
            fname = self.pars.initialParametersFile
        
        if len(fname) > 0:
            print 'loading parameters from file',fname
            obs = self.ws.argSet('%s,%s' % (self.pars.var1, self.pars.var2))
            params = self.ws.pdf('totalPdf').getParameters(obs)
            params.readFromFile(fname)
            params.IsA().Destructor(params)

    def resetYields(self):
        if self.ws.data('data'):
            Ndata = self.ws.data('data').sumEntries()
        else:
            Ndata = 10000.
        print 'resetting yields...'
        components = self.pars.signals + self.pars.backgrounds
        for component in components:
            theYield = self.ws.var('n_%s' % component)
            if hasattr(self, '%sExpected' % component):
                theYield.setVal(getattr(self, '%sExpected' % component))
            else:
                print 'no expected value for',component
                theYield.setVal(Ndata/len(components))
            if component in self.pars.yieldConstraints:
                theYield.setError(theYield.getVal() * \
                                      self.pars.yieldConstraints[component])
            else:
                theYield.setError(sqrt(theYield.getVal()))
            theYield.Print()
                

if __name__ == '__main__':
    import RooWjj2DFitterPars

    pars = RooWjj2DFitterPars.Wjj2DFitterPars()
    pars.backgrounds = ['WpJ']
    pars.signals = []
    pars.WpJFiles = [('', 0, 0)]
    pars.WpJModels = [20, 0]
    
    fitter = Wjj2DFitter(pars)
    fitter.makeFitter()
    fitter.loadData()
    fitter.resetYields()

    fitter.ws.var('turnOn_WpJ_Mass2j_PFCor_0').setVal(60.)
    fitter.ws.var('turnOn_WpJ_Mass2j_PFCor_0').setError(10.)
    fitter.ws.var('width_WpJ_Mass2j_PFCor_0').setVal(20.)
    fitter.ws.var('width_WpJ_Mass2j_PFCor_0').setError(2.)
    fitter.ws.var('c_WpJ_Mass2j_PFCor_1').setVal(-0.015)
    fitter.ws.var('c_WpJ_Mass2j_PFCor_1').setError(0.01)
    fitter.ws.var('c_WpJ_fit_mlvjj').setVal(-0.015)
    fitter.ws.var('c_WpJ_fit_mlvjj').setError(0.01)

    #fitter.fit()
    plot1 = fitter.stackedPlot(pars.var1)
    plot2 = fitter.stackedPlot(pars.var2)

    plot1.Draw()

    fitter.ws.Print()
