from RooWjj2DFitterUtils import Wjj2DFitterUtils
from ROOT import RooWorkspace

class Wjj2DFitter:

    def __init__ (self, pars):
        self.pars = pars
        self.ws = RooWorkspace('wjj2dfitter')
        self.utils = Wjj2DFitterUtils(self.pars)

        self.ws.factory('%s[%f,%f]' % (self.pars.var1, self.pars.v1min, 
                                       self.pars.v1max))
        self.ws.factory('%s[%f,%f]' % (self.pars.var2, self.pars.v2min, 
                                       self.pars.v2max))
        self.ws.defineSet('obsSet', '%s,%s' % (self.pars.var1, self.pars.var2))

    def makeFitter(self):
        if self.ws.pdf('totalPdf'):
            return self.ws.pdf('totalPdf'):

        components = self.pars.backgrounds + self.pars.signals
        compPdfs = []
        compYields = []
        for component in components:
            compFiles = getattr(self, '%sFiles' % component)
            compModels = getattr(self, '%sModels' % component)
            for compPdf in self.makeComponentPdf(component, compFiles, 
                                                 compModels):
                compPdfs.append(compPdf[0])
                compYields.append(compPdf[1])

        return self.ws.pdf('totalPdf')

    def makeComponentHistPdf(self, component, files, order = 0):
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
            tmpHist.Print()

        compHist.Print()
        print 'Number of expected %s events: %.1f' % (component, sumExpected)
        print '%s acc x eff: %.3g' % \
            (component, sumExpected/sumxsec/self.pars.integratedLumi)

        return self.utils.Hist2Pdf(compHist, '%sPdf', self.ws, order)

if __name__ == '__main__':
    import RooWjj2DFitterPars

    pars = RooWjj2DFitterPars.Wjj2DFitterPars()
    fitter = Wjj2DFitter(pars)

    fitter.ws.Print()
