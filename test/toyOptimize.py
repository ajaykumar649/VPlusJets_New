import subprocess

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='TestWjjFitParams.txt',
                  help='File to use as the initial template')
parser.add_option('-p', '--precision', dest='P', default=3, type='int',
                  help='precision to find minimum 10^-P')
parser.add_option('-d', '--dir', dest='mcdir', default='./',
                  help='directory to pick up the W+jets shapes')
parser.add_option('-s', '--seed', dest='seed', type='int', default=0,
                  help='random seed for generating toys.')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

import generateToyWpJ
from ROOT import gRandom

startingFile = 'TempInitFile.txt'
cpCmd = ['cp', '-v', opts.startingFile, startingFile]
optCmd = ['python', 'newOptimize.py', '-b', '-j', str(opts.Nj),
          '-p', str(opts.P), '-m', opts.modeConfig,
          '--init', startingFile, '--dir', opts.mcdir
          ]

print ' '.join(cpCmd)
subprocess.call(cpCmd)
if opts.seed > 0:
    gRandom.SetSeed(opts.seed)
generateToyWpJ.generate(opts.mcdir, opts.Nj, opts.modeConfig)
print ' '.join(optCmd)
subprocess.call(optCmd)
