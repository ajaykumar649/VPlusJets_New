#! /usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
(opts, args) = parser.parse_args()

import pyroot_logon
from ROOT import *

c1 = TCanvas('c1', 'c1')

def showCheck(f):
    kiter = f.GetListOfKeys().MakeIterator()
    key = kiter.Next()
    while key:
        print 'key:',key.GetName(),key.GetClassName()
        if key.GetClassName() == 'RooPlot':
            f.Get(key.GetName()).Draw()
            c1.SetGridy(False)
            c1.Update()
            c1.WaitPrimitive()
        if (key.GetClassName() == 'TGraphErrors') or \
           (key.GetClassName() == 'TGraph'):
            f.Get(key.GetName()).Draw('ap')
            c1.SetGridy()
            c1.Update()
            c1.WaitPrimitive()
        key = kiter.Next()

for fname in args:
    print fname
    f = TFile(fname)
    if f.IsOpen():
        showCheck(f)
