#! /usr/bin/env python

import subprocess

def runCommand(cmd):
    print ' '.join(cmd)
    subprocess.call(cmd)

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-H', '--mH', dest='mH', default=350, type='int',
                  help='Higgs Mass Point')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
parser.add_option('--reuse', dest='reuse', action='store_true',
                  default=False, help='use previous fit to get data')
parser.add_option('--sig', dest='sig', action='store_true',
                  default=False, help='include signal hypothesis in fit')
(opts, args) = parser.parse_args()

import os
import fnmatch

commonCmd = [ 'python', 'runHWW2DFit.py', '-b', '-j', str(2), 
              '-m', 'HWW2DConfig', '--mH', str(opts.mH) ]

searchString = '*HWW%iParameters.txt' % opts.mH
if opts.isElectron:
    searchString = '*HWW%iParameters_el.txt' % opts.mH
    commonCmd += [ '--electrons' ]
if opts.sig:
    pass
else:
    commonCmd += [ '--nosig' ]
inputPars = [ '2DFit/%s' % n for n in fnmatch.filter(os.listdir('2DFit/'), 
                                                     searchString) ]
#print inputPars
muCmd = list(commonCmd) + inputPars

if opts.mH >= 400:
    muCmd += [ '2DFit/' + searchString.replace('*', 'ggH').replace('meters', 'meters_up'),
               '2DFit/' + searchString.replace('*', 'ggH').replace('meters', 'meters_down')
               ]
if opts.reuse:
    flavor = 'electron' if opts.isElectron else 'muon'
    muCmd += [ '--ws', 'HWWlnujjH%i_%s_2jets_output.root' % (opts.mH, flavor) ]

runCommand(muCmd)
