from optparse import OptionParser
parser = OptionParser()
parser.add_option('--lumi', dest='lumi', type='float', default=11500.,
                  help='integrated lumi in pb^-1')
parser.add_option('--noblind', dest='blind', action='store_false', 
                  default=True, help='do not blind')
(opts, args) = parser.parse_args()

import pyroot_logon
from ROOT import *
#import re

fin = TFile(args[0])


fparts = (args[0].split('/'))[-1].split('_')
print fparts

mf4 = fin.Get("Mlvjj_Stacked")

h_total_up = fin.Get('h_total_up')
h_total_up.SetLineColor(kGray+3)
h_total_up.SetLineStyle(2)
h_total_down = fin.Get('h_total_down')
h_total_down.SetLineColor(kGray+3)
h_total_down.SetLineStyle(2)

mf4.addPlotable(h_total_up, "l")
mf4.addPlotable(h_total_down, "l")

tmpLeg = mf4.findObject('theLegend')
tmpLeg.AddEntry(h_total_up, "bkg syst.", "l")

if opts.blind:
    mf4.setInvisible('theData')

c4body = TCanvas('c4body', '4 body stacked')
mf4.Draw()
pyroot_logon.cmsPrelim(c4body, opts.lumi/1000.)

c4body.Update()

c4body.Print("%s_Mlvjj_%s_%s_Stacked.pdf" % (fparts[0], fparts[1], fparts[2]))
c4body.Print("%s_Mlvjj_%s_%s_Stacked.png" % (fparts[0], fparts[1], fparts[2]))

mf4log = fin.Get("Mlvjj_log")

if opts.blind:
    mf4log.setInvisible('theData')

    c4blog = TCanvas("c4vlog", "4 body stacked log")
    c4blog.SetLogy()
    mf4log.Draw()
    pyroot_logon.cmsPrelim(c4blog, opts.lumi/1000.)

    c4blog.Update()

    c4blog.Print("%s_Mlvjj_%s_%s_Stacked_log.pdf" % (fparts[0], fparts[1], 
                                                     fparts[2]))
    c4blog.Print("%s_Mlvjj_%s_%s_Stacked_log.png" % (fparts[0], fparts[1], 
                                                     fparts[2]))

if opts.blind:
    mf = fin.Get("Mjj_Stacked")
    blind = TBox(65., mf.GetMinimum(), 95., mf.GetMaximum())
    blind.SetFillColor(kGray+3)

    cmjj = TCanvas("cmjj", "dijet mass")
    mf.Draw()
    blind.Draw("same")
    pyroot_logon.cmsPrelim(cmjj, opts.lumi/1000.)

    cmjj.Update()
    cmjj.Print("%s_Mjj_%s_%s_Stacked.pdf" % (fparts[0], fparts[1], fparts[2]))
    cmjj.Print("%s_Mjj_%s_%s_Stacked.png" % (fparts[0], fparts[1], fparts[2]))
