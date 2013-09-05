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
parser.add_option('--expLimit', type='int', default=0, dest='limit',
                  help='number of toys for expected limit, 0 is off')
parser.add_option('--mva', dest='mvaCut', type='float',
                  help='override cut value for mva')
(opts, args) = parser.parse_args()

import os
import fnmatch

commonCmd = [ 'python', 'runHWW1D2FitsFitter.py', '-b', '-j', str(2), 
              '--mH', str(opts.mH) , '--obsLimit']
if opts.limit:
    commonCmd += ['--limit', str(opts.limit)]

if hasattr(opts, "mvaCut") and opts.mvaCut:
    commonCmd.extend(['--mva', str(opts.mvaCut)])

if opts.reuse:
    flavor = 'electron' if opts.isElectron else 'muon'
    commonCmd += [ '--ws', 'HWW%ilnujj_%s_2jets_1D2Fit_output.root' % (opts.mH, flavor) ]

searchString = '*HWW%iParameters_m??.txt' % opts.mH
if opts.isElectron:
    searchString = '*HWW%iParameters_el_m??.txt' % opts.mH
    commonCmd += [ '--electrons' ]
if opts.sig:
    pass
else:
    commonCmd += [ '--nosig' ]
inputPars = [ '1D2FitsParameters/%s' % n for n in fnmatch.filter(
        os.listdir('1D2FitsParameters/'), searchString ) 
              ]
#print inputPars
muCmd = list(commonCmd) + inputPars

if opts.mH >= 400:
    muCmd += [ '1D2FitsParameters/' + searchString.replace('*', 'ggH').replace('m??', 'mWW_up'),
               '1D2FitsParameters/' + searchString.replace('*', 'ggH').replace('m??', 'mWW_down')
               ]

runCommand(muCmd)
