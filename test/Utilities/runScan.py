#! /usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='TestWjjFitParams.txt',
                  help='File to use as the initial template')
parser.add_option('-d', '--dir', dest='dir', default='./',
                  help='directory for files')
parser.add_option('--start', type='float', default=-0.99, dest='start',
                  help='where fSU should start the scan')
parser.add_option('--end', type='float', default = 1.00, dest='end',
                  help='where fSU should end the scan')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

import os
import subprocess

cwd = os.getcwd()
newDir = cwd + '/' + opts.dir
print 'cwd:',cwd
print 'newDir:',newDir
os.chdir(newDir)
print 'cwd:',os.getcwd()

toyCmd = ['python', 'scanOptimize.py', '-j', str(opts.Nj),
          '-i', opts.startingFile, '--start', str(opts.start),
          '--end', str(opts.end), '-m', opts.modeConfig ]
print ' '.join(toyCmd)
subprocess.call(toyCmd)
