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
parser.add_option('-m', '--mode', default="HWWconfig", dest='modeConfig',
                  help='which config to select from WjjFitterConfigs')
(opts, args) = parser.parse_args()

commands = []
seed = 12345678900

for i in range(0,100):
    cmd = 'python tardir/run10Toys.py -j {0} -i {1} -d tardir/ -s {2} -p {3} -m {4}\n'.format(opts.Nj, opts.startingFile, seed, opts.P, opts.modeConfig)
    commands.append(cmd)
    seed += 10

    
outFile = open("commands.txt", "w")
outFile.writelines(commands)

