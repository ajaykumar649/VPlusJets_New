#! /usr/bin/env python

import re
import sys

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='TestWjjFitParams.txt',
                  help='File to use as the initial template')
parser.add_option('-p', '--precision', dest='P', default=3, type='int',
                  help='precision to find minimum 10^-P')
parser.add_option('-d', '--dir', dest='mcdir', default='',
                  help='directory to pick up the W+jets shapes')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

if opts.P > 3:
    print 'precision cannot be better than 10^-3'
    opts.P = 3

optVars = { 'fSU' : 0.0, 'fMU' : 0.0 }
## optVars = ['fSU']
minPoint = optVars.copy()
g_minFOM = 100000.

import pyroot_logon
import WjjFitterConfigs
from ROOT import TGraph, TF1, gPad, TFile, Double, Long, gROOT
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+');
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
tmpInitFile = opts.startingFile

fitterPars = WjjFitterConfigs.__dict__[opts.modeConfig](opts.Nj, opts.mcdir,
                                                        tmpInitFile)
theFitter = RooWjjMjjFitter(fitterPars)

Npts = 7

def optimizeVar (optVar, start, step, iteration, tryFit = True,
                 theVars = optVars):

    global g_minFOM, minPoint
    
    optGraph = TGraph(Npts)
    optGraph.SetName('graph_{0}_{1}'.format(optVar, iteration))
    SetPoints = 0
    minFOM = 10000.
    minVal = 0.
    for point in range(0, Npts):
        newVal = start + step*point

        writeValToFile(optVar, newVal, opts.startingFile, tmpInitFile,
                       theVars = theVars)

        printPts(optVar, newVal, theVars = theVars)

        fr = theFitter.fit()
        nll = fr.minNll()
        chi2 = Double(0.)
        ndf = Long(fr.floatParsFinal().getSize())
        theFitter.computeChi2(chi2, ndf)
        print 'chi2:', chi2, 'dof:', ndf, 'nll:',nll
        FOM = nll
        if FOM < minFOM:
            minFOM = FOM
            minVal = newVal
        if FOM < g_minFOM:
            g_minFOM = FOM
            for tmpVar in minPoint:
                minPoint[tmpVar] = theVars[tmpVar]
            minPoint[optVar] = newVal
            
        optGraph.SetPoint(SetPoints, newVal, FOM)
        SetPoints += 1
        sys.stdout.flush()

    optGraph.Set(SetPoints)
    bestVal = minVal
    if tryFit and (SetPoints > 2):
        parabolaFit = TF1("parabFit", "x*x++x++1", start, newVal)
        optGraph.Fit(parabolaFit)
        if (parabolaFit.GetParameter(0) > 0):
            bestVal = -1.*parabolaFit.GetParameter(1)/2./parabolaFit.GetParameter(0)
        else:
            bestVal = minVal
    print 'extrapolated optimum for',optVar,':',bestVal
    if bestVal > 1.0:
        bestVal = 1.0
    if bestVal < -1.:
        bestVal = -1.
    outf.cd()
    optGraph.Write()

    return round(bestVal,3)

def readValFromFile( filename, theVars = optVars):
    initFile = open(filename).readlines()
    for optVar in theVars:
        compliment = optVar.replace('U','D')
        retVal = 0.
        compVal = 0.
        for line in initFile:
            found = re.search('^{0} = [ ]*([-0-9\.]*)'.format(optVar), line)
            if found:
                retVal = float(found.group(1))
            found = re.search('^{0} = [ ]*([-0-9\.]*)'.format(compliment),line)
            if found:
                compVal = float(found.group(1))
        if (compVal > retVal):
            retVal = -1*compVal
        theVars[optVar] = retVal

    
def writeValToFile(optVar, newVal, filename, outname = None,
                   theVars = optVars):
    if outname == None:
        outname = filename
    newStart = open(filename).readlines()
    for tmpVar in theVars:
        compliment = tmpVar.replace('U', 'D')
        if optVar == tmpVar:
            theVal = newVal
        else:
            theVal = theVars[tmpVar]
        if (theVal >= 0):
            newStart = [re.sub('^{0} = [ ]*[-0-9\.]*'.format(tmpVar),
                               '{0} = {1:.3f}'.format(tmpVar, theVal), x) \
                        for x in newStart]
            newStart = [re.sub('^{0} = [ ]*[-0-9\.]*'.format(compliment),
                               '{0} = {1:.3f}'.format(compliment, 0.), x) \
                        for x in newStart]
        else:
            newStart = [re.sub('^{0} = [ ]*[-0-9\.]*'.format(compliment),
                               '{0} = {1:.3f}'.format(compliment, -1*theVal), x) \
                        for x in newStart]
            newStart = [re.sub('^{0} = [ ]*[-0-9\.]*'.format(tmpVar),
                               '{0} = {1:.3f}'.format(tmpVar, 0.), x) \
                        for x in newStart]
    #print ''.join(newStart)
    newFile = open(outname, 'w')
    newFile.writelines(newStart)
    newFile.close()

def sumPts(optVar, newVal, theVars = optVars):
    theSum = 0.
    for tmpVar in theVars:
        if tmpVar == optVar:
            theSum += newVal
        else:
            theSum += theVars[tmpVar]
    return theSum

def printPts(optVar, newVal, theVars = optVars):
    for tmpVar in theVars:
        print tmpVar,'=',
        if tmpVar == optVar:
            print newVal,
        else:
            print theVars[tmpVar],
        print ',',
    print
    
outf = TFile('optimization{0}.root'.format(opts.Nj), 'recreate')
keepIterating = True
iteration = 0

start = -0.5
step = 0.05
Npts = 20

scanVars = optVars.copy()

for optVar in scanVars:
    start = -0.5
    step = 0.05
    oldVal = scanVars[optVar]
    bestVal = optimizeVar(optVar, start, step, iteration, False, scanVars)

    print 'old minimum:',oldVal,'new minimum:', bestVal
    optVars[optVar] = bestVal
    sys.stdout.flush()

writeValToFile("", 0, opts.startingFile, theVars=optVars)
#readValFromFile(opts.startingFile)
print optVars

maxIterations = 9
Npts = 7

## while (keepIterating) and (iteration < maxIterations):
while (keepIterating):
    iteration += 1
    keepIterating = False
    if iteration > 1:
        step = round(step/2., 3)
    if step < 10**(-opts.P)*3.:
        step = 10**(-opts.P)*3.
    for optVar in optVars:

        start = optVars[optVar]
        oldVal = start
        
        start -= 3*step

        if start < -1.:
            start = -1.

        bestVal = optimizeVar(optVar, start, step, iteration)

        print 'old minimum:',oldVal,'new minimum:', bestVal
        optVars[optVar] = bestVal
        writeValToFile(optVar, bestVal, opts.startingFile)
        if abs(oldVal-bestVal) > 10**(-opts.P):
            keepIterating = True
        sys.stdout.flush()

    ##     optGraph.Draw('ap*')
    ##     gPad.WaitPrimitive()

outf.Close()

print '*** best point:',
printPts(' ', 0., minPoint)
print '*** final optimization:',
printPts(' ', 0.)
