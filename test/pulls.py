def createPull(theData, curve, curveUp = None, curveDown = None):
    from ROOT import TGraphErrors, TMath

    pullGraph = TGraphErrors(theData.GetN())

    for datapt in range(0, theData.GetN()):
        binmin = theData.GetX()[datapt] - theData.GetEXlow()[datapt]
        binmax = theData.GetX()[datapt] + theData.GetEXhigh()[datapt]

        binN = theData.GetY()[datapt]*(binmax-binmin)

        curveN = curve.average(binmin, binmax)*(binmax-binmin)

        diffUp = 0.
        diffDown = 0.
        if curveUp and curveDown:
            diffUp = curveUp.average(binmin,binmax)*(binmax-binmin) - curveN
            diffDown = curveN - curveDown.average(binmin,binmax)*(binmax-binmin)

            if diffUp < 0:
                #print diffUp
                hold = diffDown
                diffDown = -1.*diffUp
                diffUp = -1.*diffDown
                
        
        if (binN > curveN):
            errN = TMath.Sqrt((theData.GetEYlow()[datapt]*(binmax-binmin))**2 \
                                  + diffUp**2)
        else:
            errN = TMath.Sqrt((theData.GetEYhigh()[datapt]*(binmax-binmin))**2 \
                                  + diffDown**2)

        pull = 0.
        if errN > 0:
            pull = (binN - curveN)/errN

        # print 'bin: (', binmin, ',', binmax, ') N:', binN, '+/-', errN,
        # print 'curve N:', curveN

        pullGraph.SetPoint(datapt, theData.GetX()[datapt], pull)
        pullGraph.SetPointError(datapt, 0., 1.)

    return pullGraph


if __name__ == '__main__':
    pass
