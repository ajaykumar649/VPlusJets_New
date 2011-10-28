#! /usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='TestWjjFitParams.txt',
                  help='File to use as the initial template')
parser.add_option('-p', '--precision', dest='P', default=3, type='int',
                  help='precision to find minimum 10^-P')
parser.add_option('-d', '--dir', dest='dir', default='./',
                  help='directory for files')
parser.add_option('-s', '--seed', dest='seed', type='int', default=4567,
                  help='random seed for generating toys.')
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

for i in range(0, 10):
    currSeed = opts.seed + i
    toyCmd = ['python', 'toyOptimize.py', '-j', str(opts.Nj),
              '-p', str(opts.P), '-i', opts.startingFile, '-d', './',
              '-s', str(currSeed), '-m', opts.modeConfig ]
    print ' '.join(toyCmd)
    subprocess.call(toyCmd)
