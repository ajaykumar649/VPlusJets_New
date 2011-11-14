#! /usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('-i', '--init', dest='startingFile',
                  default='TestWjjFitParams.txt',
                  help='File to use as the initial template')
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

commands = []

start = -0.99
step = 0.02

while (start <= 1.0):
    end = start + step*10
    cmd = 'python tardir/runScan.py -j {0} -i {1} -d tardir/ --start {2:0.3f} --end {3:0.3f} --m {4}'.format(opts.Nj, opts.startingFile, start, end, opts.modeConfig)
    commands.append(cmd)
    print cmd
    start = end
    
outFile = open("commands.txt", "w")
outFile.writelines(commands)

