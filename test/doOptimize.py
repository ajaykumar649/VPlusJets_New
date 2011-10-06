#! /usr/bin/env python

import subprocess
import re

cmdRoot = ['root', '-l', '-b', '-q', 'RooWjjFitterNew.C+(0,0.,true,false,false,2)']
cmdGrepChi = ['grep', '^ \*\*\* chi']

optVars = ['fSU', 'fMU']
## optVars = ['fSU']

from ROOT import TGraph, TF1, gPad, TFile

outf = TFile('optimization.root', 'recreate')
for iteration in range(0, 2):
    Npts = 6
    start = 0.
    step = 0.1
    if iteration > 0:
        step = step/10.
    for optVar in optVars:

        initFile = open('TestWjjFitParams.txt').readlines()
        for line in initFile:
            if re.search('{0} = [ ]*([-0-9\.]*)'.format(optVar), line):
                start = float(re.search('{0} = [ ]*([-0-9\.]*)'.format(optVar),
                                        line).group(1))

        if iteration > 0:
            start -= 2*step

        optGraph = TGraph(Npts)
        optGraph.SetName('graph_{0}_{1}'.format(optVar, iteration))
        for point in range(0, Npts):
            newVal = start + step*point
            testFile = [re.sub(optVar+' = [ ]*[-0-9\.]*',
                               '{0} = {1:.3f}'.format(optVar, newVal), x) \
                        for x in initFile]
            print ''.join(testFile)
            tmpInit = open('tmpInit.txt', 'w')
            tmpInit.writelines(testFile)
            tmpInit.close()

            p1 = subprocess.Popen(cmdRoot, stdout=subprocess.PIPE)
            p2 = subprocess.Popen(cmdGrepChi, stdin=p1.stdout,
                                  stdout=subprocess.PIPE)
            p1.stdout.close()
            output = p2.communicate()[0]

            print output
            chi2match = re.search('dof = ([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)/(\d+)', output)
            chi2 = float(chi2match.group(1))
            dof = float(chi2match.group(5))
            prob = float(re.search('probability = ([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)', output).group(1))
            print 'chi2:', chi2, 'dof:', dof, 'prob:', prob
            optGraph.SetPoint(point, newVal, chi2)
        parabolaFit = TF1("parabFit", "x*x++x++1", start, newVal)
        optGraph.Fit(parabolaFit)
        bestVal = -1.*parabolaFit.GetParameter(1)/2./parabolaFit.GetParameter(0)
        print 'minimum:', bestVal

        newStart = [re.sub('{0} = [ ]*[-0-9\.]*'.format(optVar),
                           '{0} = {1:.3f}'.format(optVar, bestVal), x) \
                    for x in open('TestWjjFitParams.txt').readlines()]
        newFile = open('TestWjjFitParams.txt', 'w')
        newFile.writelines(newStart)
        newFile.close()
        optGraph.Write()
    ##     optGraph.Draw('ap*')
    ##     gPad.WaitPrimitive()

outf.Close()
