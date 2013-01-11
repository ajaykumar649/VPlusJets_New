#! /usr/bin/env python

import subprocess

def runCommand(cmd, bn, interf = 0):
    if interf == 2:
        bn += '_up'
    elif interf == 3:
        bn += '_down'
    if opts.isElectron:
        bn += '_el'

    cmd += ['--bn', bn, '--interference', str(interf), bn+'.txt']
    print ' '.join(cmd)
    subprocess.call(cmd)

from optparse import OptionParser

parser = OptionParser()
parser.add_option('-H', '--mH', dest='mH', default=350, type='int',
                  help='Higgs Mass Point')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
(opts, args) = parser.parse_args()

components = [ 'ggH', 'qqH', 'diboson', 'top', 'WpJ' ]

cmdBase = [ 'python', 'fitComponentShapePdf.py', '-b', '-j', '2', 
            '-m', 'HWW2DConfig', '--mH', str(opts.mH) ]

for component in components:
    cmd = list(cmdBase) + [ '--comp', component, ]
    if component == 'WpJ':
        cmd += [ '--makeFree' ]
    bn = '2DFit/%sHWW%iParameters' % (component, opts.mH)
    if (component == 'ggH') and (opts.mH >= 400):
        for i in range(1, 4):
            runCommand(list(cmd), bn, interf = i)
    else:
        runCommand(cmd, bn)
