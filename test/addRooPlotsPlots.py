
def addPlots(plots):
    from ROOT import RooCurve, RooHist, TLine, TLegend, RooPlot, RooAbsData, \
         SetOwnership
    #print plots
    outplot = plots[0].emptyClone(plots[0].GetName())
    newMax = 0.
    for item in range(0, int(plots[0].numItems())):
        itemName = plots[0].nameOf(item)
        firstItem = plots[0].getObject(item)
        if (type(firstItem) == RooCurve):
            fullCurve = firstItem
            for plot in range(1, len(plots)):
                nextCurve = plots[plot].getCurve(itemName)
                fullCurve = RooCurve(fullCurve.GetName(), fullCurve.GetTitle(),
                                     fullCurve, nextCurve)
            fullCurve.SetLineColor(firstItem.GetLineColor());
            fullCurve.SetLineStyle(firstItem.GetLineStyle());
            fullCurve.SetFillColor(firstItem.GetFillColor());
            fullCurve.SetFillStyle(firstItem.GetFillStyle());
            outplot.addPlotable(fullCurve,
                                plots[0].getDrawOptions(itemName).Data())
            SetOwnership(fullCurve, False)

        if (type(firstItem) == RooHist):
            fullHist = firstItem
            for plot in range(1, len(plots)):
                nextHist = plots[plot].getHist(itemName)
                fullHist = addHists(fullHist, nextHist)
            fullHist.SetName(itemName)
            fullHist.SetTitle(firstItem.GetTitle())
            outplot.addPlotable(fullHist,
                                plots[0].getDrawOptions(itemName).Data())
            SetOwnership(fullHist, False)

        if (type(firstItem) == TLine):
            newLine = TLine(firstItem)
            newLine.SetY2(outplot.GetMaximum())
            SetOwnership(newLine, False)
            outplot.addObject(newLine)
            pass
            
        if (type(firstItem) == TLegend):
            newLeg = TLegend(firstItem)
            newLeg.SetY1NDC(0.92 - \
                            0.05*newLeg.GetListOfPrimitives().GetSize() - \
                            0.02)
            newLeg.SetY1(newLeg.GetY1NDC())
            SetOwnership(newLeg, False)
            outplot.addObject(newLeg)

    for plot in plots:
        newMax += plot.GetMaximum()

    outplot.SetMaximum(outplot.GetMaximum()*1.3)
    outplot.GetYaxis().SetTitle("Events / GeV")
##     outplot.SetMaximum(newMax)
##         outplot.Print("v")

    return outplot

def addHists(hist1, hist2):
    from ROOT import RooHist
    from math import sqrt

    newHist = RooHist(hist1)

    for i in range(0, newHist.GetN()):
        newHist.SetPoint(i, newHist.GetX()[i],
                         hist1.GetY()[i]+hist2.GetY()[i])
        newHist.SetPointError(i, hist1.GetErrorXlow(i), hist1.GetErrorXhigh(i),
                              sqrt(hist1.GetErrorYlow(i)**2 + \
                                   hist2.GetErrorYlow(i)**2),
                              sqrt(hist1.GetErrorYhigh(i)**2 + \
                                   hist2.GetErrorYhigh(i)**2)
                              )
    return newHist

    
if __name__ == "__main__":
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option('-b', action='store_true', dest='noX', default=False,
                      help='no X11 windows')
    parser.add_option('-o', '--output', dest='output',
                      default='combinedPlots.root',
                      help='output file name')
    (opts, args) = parser.parse_args()

    import pyroot_logon

    from ROOT import TFile, TCanvas, gROOT, RooPlot, gPad

    plots = {'Mjj_Stacked': [], 'Mjj_Subtracted': []}
    #print plots
    cans = []

    for f in args:
        inF = TFile.Open(f)
        gROOT.cd()
        for plotName in plots:
            thePlot = inF.Get(plotName)
            if thePlot:
                plots[plotName].append(thePlot.Clone())
        inF.Close()

    print plots

    cplots = []
    outFile = TFile(opts.output, 'recreate')
    for plot in plots:
        outPlot = addPlots(plots[plot])
        can = TCanvas()
        cans.append(can)
        cplots.append(outPlot)
        outPlot.Draw()
        pyroot_logon.cmsPrelim(can, 4700)
        gPad.Update()
        can.Print('%s_combined.png' % (plot))
        can.Print('%s_combined.pdf' % (plot))
        # gPad.WaitPrimitive()

        outPlot.Write()
        
