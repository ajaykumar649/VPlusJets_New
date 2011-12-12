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
parser.add_option('-d', '--dir', dest='mcdir', default='',
                  help='directory to pick up the W+jets shapes')
parser.add_option('--start', type='float', default=-0.99, dest='start',
                  help='where fSU should start the scan')
parser.add_option('--end', type='float', default = 1.00, dest='end',
                  help='where fSU should end the scan')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

optVars = { 'fSU' : 0.0, 'fMU' : 0.0 }
## optVars = ['fSU']

import pyroot_logon
config = __import__(opts.modeConfig)
from ROOT import gROOT
## gROOT.ProcessLine('.L RooWjjFitterParams.h+');
gROOT.ProcessLine('.L EffTableReader.cc+')
gROOT.ProcessLine('.L EffTableLoader.cc+')
gROOT.ProcessLine('.L RooWjjFitterUtils.cc+');
gROOT.ProcessLine('.L RooWjjMjjFitter.cc+');
from ROOT import RooWjjMjjFitter, RooFitResult, \
     RooMsgService, RooFit

RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)
tmpInitFile = opts.startingFile

fitterPars = config.theConfig(opts.Nj, opts.mcdir, tmpInitFile)
theFitter = RooWjjMjjFitter(fitterPars)

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

def printPts(optVar, newVal):
    for tmpVar in optVars:
        print tmpVar,'=',
        if tmpVar == optVar:
            print newVal,
        else:
            print optVars[tmpVar],
        print ',',
    print

def sumVars(theVars):
    theSum = 0.
    for tmpVar in theVars:
        theSum += abs(theVars[tmpVar])
    return theSum
    
step = 0.02

optVars["fSU"] = opts.start
optVars["fMU"] = -0.99
print optVars

while (optVars["fSU"] < opts.end):
    while (optVars["fMU"] <= 1.0):
##         print optVars
        if (sumVars(optVars) <= 1.00):
            writeValToFile("fMU", optVars["fMU"], opts.startingFile,
                           theVars = optVars)

            printPts("",0)

            fr = theFitter.fit()
            nll = fr.minNll()
            print 'nll:',nll,'\n'
            sys.stdout.flush()
        optVars["fMU"] += step

    optVars["fSU"] += step
    optVars["fMU"] = -0.99

print "done"
