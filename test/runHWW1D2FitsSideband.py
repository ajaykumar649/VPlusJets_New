from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-m', '--mode', default="HWW1D2FitsConfig", 
                  dest='modeConfig',
                  help='which config to select look at HWW2DConfig.py for ' +\
                      'an example.  Use the file name minus the .py extension.'
                  )
parser.add_option('-H', '--mH', dest='mH', default=350, type='int',
                  help='Higgs Mass Point')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
parser.add_option('--nosig', dest='includeSignal', action='store_false',
                  default=True, help='include signal shape in fit')
parser.add_option('--toy', dest='toy', action='store_true',
                  help='use pseudo-data instead of data file')
parser.add_option('--toyOut', dest='toyOut', help='filename for toy output')
parser.add_option('--seed', dest='seed', type='int', help='random seed')
parser.add_option('--ws', dest='ws', 
                  help='filename that contains workspace to be used cloned ' +\
                      'for use')
parser.add_option('--debug', dest='debug', action='store_true', default=False,
                  help='turn on extra debugging information')
parser.add_option('--limit', dest='doLimit', type='int',
                  default=0,
                  help='This option is not used.')
parser.add_option('--obsLimit', dest='obsLimit', action='store_true',
                  default=False,
                  help='This option is not used.')
parser.add_option('--mva', dest='mvaCut', type='float',
                  help='override cut value for mva')
parser.add_option('--sideband', dest='sb', type='int',
                  default=1, help='use sideband dataset and model instead')

(opts, args) = parser.parse_args()

import pyroot_logon

#import HWW2DConfig
config = __import__(opts.modeConfig)
import RooWjj2DFitter

from ROOT import TCanvas, RooFit, RooLinkedListIter, TMath, RooRandom, TFile, \
    RooDataHist, RooMsgService, TStopwatch, RooAbsPdf, TBox, kBlack, kRed, \
    kBlue, kOrange, kDashed, RooAbsCollection, RooArgSet, RooStats, Double, \
    RooAbsReal, RooAbsData, TH1F
import pulls

timer = TStopwatch()
timer.Start()

#RooAbsPdf.defaultIntegratorConfig().setEpsRel(1e-9)
#RooAbsPdf.defaultIntegratorConfig().setEpsAbs(1e-9)
if not opts.debug:
    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

if hasattr(opts, "seed") and (opts.seed >= 0):
    print "random seed:", opts.seed
    RooRandom.randomGenerator().SetSeed(opts.seed)

mvaCutOverride = None
if hasattr(opts, "mvaCut"):
    mvaCutOverride = opts.mvaCut

mjjArgs = []
sideArgs = []
mWWArgs = []
for arg in args:
    if arg[-8:] == '_mjj.txt':
        mjjArgs.append(arg)
    elif arg[-8:] == '_mWW.txt':
        mWWArgs.append(arg)
    else:
        mWWArgs.append(arg)

print mjjArgs
pars = config.theConfig(Nj = opts.Nj, mH = opts.mH, 
                        isElectron = opts.isElectron, initFile = mjjArgs,
                        includeSignal = False, MVACutOverride = mvaCutOverride)
# pars.blind = False
fitter = RooWjj2DFitter.Wjj2DFitter(pars)
fitter.ws.SetName("w_mjj")

totalPdf = fitter.makeFitter()

if opts.ws:
    fitter.loadWorkspaceFromFile(opts.ws, getFloatPars = False,
                                 wsname = 'w_mjj')

fitter.readParametersFromFile()
fitter.expectedFromPars()
fitter.resetYields()

startpars = totalPdf.getParameters(fitter.ws.set('obsSet'))
fitter.ws.defineSet("params", startpars)
fitter.ws.saveSnapshot("initPars", startpars)

if opts.toy:
    #generate toy dataset
    print 'Generated parameters'
    fitter.ws.set('params').Print('v')
    fitter.ws.saveSnapshot("genPars", startpars)

    data = totalPdf.generate(fitter.ws.set('obsSet'), RooFit.Name('data_obs'),
                             RooFit.Extended())
    if fitter.pars.binData:
        data = RooDataHist('data_obs', 'data_obs', fitter.ws.set('obsSet'),
                           data)
        data.Print('v')
    getattr(fitter.ws, 'import')(data)
else:    
    data = fitter.loadData()

data.Print()
startpars.IsA().Destructor(startpars)

print 'Time elapsed: %.1f sec' % timer.RealTime()
print 'CPU time used: %.1f sec' % timer.CpuTime()
print 'starting fitting routine'
timer.Continue()
fitter.ws.var('diboson_nrm').setConstant()
fitter.ws.var('top_nrm').setConstant()
fitter.ws.var('r_signal').setVal(1.0)
fitter.ws.var('r_signal').setError(0.04)
fr = None
fr = fitter.fit()

plot1 = fitter.stackedPlot(pars.var[0])

sigPdf = fitter.ws.pdf('ggH')
n_sig = fitter.ws.var('n_ggH').getVal() + fitter.ws.var('n_qqH').getVal()
sigScaler = 2.
print "N HWW:", n_sig

sigPdf.plotOn(plot1, RooFit.Range('plotRange'),
              RooFit.NormRange('plotRange'),
              RooFit.Normalization(n_sig*sigScaler, RooAbsReal.NumEvent),
              RooFit.Name('signal_HWW'),
              RooFit.LineColor(kBlue+2))
plot1.getCurve().SetTitle("H(%i)#times%.0f" % (opts.mH, sigScaler))

leg1 = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot1)

c1 = TCanvas('c1', pars.var[0] + ' plot')
plot1.addObject(leg1)
plot1.Draw()
c1.Update()

ndf = 0

if fr:
    floatVars = [ fr.floatParsFinal().at(i).GetName() \
                      for i in range(0, fr.floatParsFinal().getSize()) ]
    fitter.ws.defineSet('floatingParams', ','.join(floatVars))
    fitter.ws.saveSnapshot("fitPars", ','.join(floatVars))
    ndf = fr.floatParsFinal().getSize() - \
        fitter.ws.set('constraintSet').getSize()
    # fr.Print('v')

print '%i free parameters in the fit' % ndf

firstCurve1 = plot1.getObject(1)
# firstCurve1.Print()

(chi2_1, ndf_1) = pulls.computeChi2(plot1.getHist('theData'), firstCurve1)
pull1 = pulls.createPull(plot1.getHist('theData'), firstCurve1)
    

chi2 = chi2_1
ndf = ndf_1-ndf

cp1 = TCanvas("cp1", pars.var[0] + ' pull')
pull1.Draw('ap')
pull1.SetName(pars.var[0] + "_pull")
cp1.SetGridy()
cp1.Update()
pull1.GetXaxis().SetLimits(pars.varRanges[pars.var[0]][1], 
                           pars.varRanges[pars.var[0]][2])
pull1.GetXaxis().SetTitle(fitter.ws.var(pars.var[0]).getTitle(True).Data())
pull1.GetYaxis().SetTitle("pull (#sigma)")

blinder = plot1.findObject('TBox')
blinder2 = None
if blinder:
    blinder2 = TBox(pars.exclude[pars.var[0]][0], pull1.GetYaxis().GetXmin(),
                    pars.exclude[pars.var[0]][1], pull1.GetYaxis().GetXmax())
    blinder2.SetFillColor(blinder.GetFillColor())
    blinder2.SetFillStyle(blinder.GetFillStyle())

if blinder2:
    #blinder2.Print()
    blinder2.Draw()

cp1.Update()

if opts.toyOut:
    outFile = open(opts.toyOut, 'a', 1)
    fitter.ws.loadSnapshot("genPars")

    line = '%s %.6g %.6g %.6g '
    outFile.write('nll %f covQual %i edm %.4g ' % (fr.minNll(), fr.covQual(),
                                                   fr.edm())
                  )
    outFile.write('chi2Prob %f ' % (TMath.Prob(chi2, ndf)))
    for cycle in range(0, fr.numStatusHistory()):
        outFile.write('%s %i ' % (fr.statusLabelHistory(cycle),
                                  fr.statusCodeHistory(cycle))
                      )
    for i in range(0, fr.floatParsFinal().getSize()):
        outFile.write(line % (fr.floatParsFinal().at(i).GetName(),
                              fr.floatParsFinal().at(i).getVal(),
                              fr.floatParsFinal().at(i).getError(),
                              fitter.ws.var(fr.floatParsFinal().at(i).GetName()).getVal()
                              )
                      )
    outFile.write('\n')
    outFile.close()

mode = 'muon'
if opts.isElectron:
    mode = 'electron'

print 'Time elapsed: %.1f sec' % timer.RealTime()
print 'CPU time used: %.1f sec' % timer.CpuTime()

print 'starting sideband fit routine'
timer.Continue()

WpJ_params_sideband = RooArgSet('WpJ_params_sideband')

configArgs = {
    'Nj':opts.Nj, 'mH':opts.mH, 
    'isElectron': opts.isElectron, 
    'initFile': mWWArgs,
    'includeSignal': True,
    'MVACutOverride': mvaCutOverride
}

sb = None
if opts.sb > 0:
    sb = 'high'
    configArgs['sideband'] = sb
elif opts.sb < 0:
    sb = 'low'
    configArgs['sideband'] = sb

import HWW1D2FitsConfig_mWW
print mWWArgs
pars_mWW = HWW1D2FitsConfig_mWW.theConfig(**configArgs)
pars_mWW.blind = False

mjj = fitter.ws.var(fitter.pars.var[0])
mjj.setRange('sidebandRegion', pars_mWW.regionLow, pars_mWW.regionHigh)
sigInt = fitter.ws.pdf('WpJ').createIntegral(fitter.ws.set('obsSet'),
                                             fitter.ws.set('obsSet'),
                                             'sidebandRegion')
fullInt = fitter.ws.pdf('WpJ').createIntegral(fitter.ws.set('obsSet'),
                                              fitter.ws.set('obsSet'))
n_WpJ_sig = sigInt.getVal()/fullInt.getVal()*fitter.ws.function('f_WpJ_norm').getVal()
print 'W+jets integral in %s sideband region:' % pars_mWW.region,
print sigInt.getVal()/fullInt.getVal()
print 'W+jets yield in the signal region:',n_WpJ_sig

pars_mWW.yieldConstraints['WpJ'] = fitter.ws.var('WpJ_nrm').getError()

# add systematic errors multipliers to ggH signals
if (opts.mH >= 400):
    pars_mWW.ggHdoSystMult = True

fitter_mWW = RooWjj2DFitter.Wjj2DFitter(pars_mWW)
fitter_mWW.ws.SetName("w_mWW")
totalPdf_mWW = fitter_mWW.makeFitter()
fitter_mWW.readParametersFromFile()
WpJ_mWW = fitter_mWW.ws.var('n_WpJ')
WpJ_mWW.setVal(n_WpJ_sig)
fitter_mWW.expectedFromPars()

mWWCut = '((%s>%.0f)&&(%s<%.0f))' % \
    (pars.var[0], pars_mWW.regionLow,
     pars.var[0], pars_mWW.regionHigh)
print '%s region cut:' % pars_mWW.region, mWWCut
fitter_mWW.loadDataFromWorkspace(fitter.ws, mWWCut)
fitter_mWW.resetYields()
params_mWW = totalPdf_mWW.getParameters(fitter_mWW.ws.set('obsSet'))

predictedPars = params_mWW.snapshot()

params_mWW.Print("v")
#fitter_mWW.ws.Print()

#compute limits
import limits
from array import array

upperHist = None

fitter_mWW.ws.var('r_signal').setVal(0.0)
fitter_mWW.ws.var('r_signal').setConstant(True)
fitter_mWW.ws.var('r_signal').setError(0.1)
fitter_mWW.ws.var('r_signal').setRange(-0.5, 5.)

fr_mWW = None
fr_mWW = fitter_mWW.fit()

if fr_mWW.statusCodeHistory(0) != 0:
    print fr_mWW.statusLabelHistory(0), 'failed'
    fr_mWW = fitter_mWW.fit(True)


# qd_plot = fitter_mWW.ws.var(pars_mWW.var[0]).frame()
# fitter.ws.data('data_obs').plotOn(qd_plot)
# totalPdf_mWW.plotOn(qd_plot)

# cqd = TCanvas('cqd', 'quick plot')
# qd_plot.Draw()
# cqd.Update()

plot_mWW = fitter_mWW.stackedPlot(pars_mWW.var[0])
plot_mWW.SetName('%s_plot_stacked' % (pars_mWW.var[0]))

leg_mWW = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot_mWW)
plot_mWW.addObject(leg_mWW)

plot_mWW_withErrs = fitter_mWW.stackedPlot(pars_mWW.var[0])
plot_mWW_withErrs.SetName('%s_plot_with_errors' % (pars_mWW.var[0]))
nexp = totalPdf_mWW.expectedEvents(fitter_mWW.ws.set('obsSet'))
totalPdf_mWW.plotOn(plot_mWW_withErrs, 
                    RooFit.VisualizeError(fr_mWW, 1, True),
                    RooFit.Range('plotRange'),
                    RooFit.NormRange('plotRange'),
                    RooFit.Normalization(nexp, RooAbsReal.NumEvent),
                    RooFit.Name('fitErrors'),
                    RooFit.FillColor(kOrange+1),
                    RooFit.FillStyle(3001))
plot_mWW_withErrs.getCurve().SetTitle('Fit errors')
errs = plot_mWW_withErrs.getCurve('fitErrors')
(upper, lower) = pulls.splitErrCurve(errs)

leg_mWW_withErrs = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot_mWW_withErrs)
plot_mWW_withErrs.addObject(leg_mWW_withErrs)

c_mWW = TCanvas('c_mWW', pars_mWW.var[0] + ' plot')
plot_mWW.Draw()
c_mWW.Update()

c_mWW_err = TCanvas('c_mWW_err', 'with errors')
plot_mWW_withErrs.Draw()
c_mWW_err.Update()

(chi2_mWW, ndf_mWW) = pulls.computeChi2(plot_mWW.getHist('theData'), 
                                        plot_mWW.getObject(1))
ndf_mWW -= fr_mWW.floatParsFinal().getSize() - \
    fitter_mWW.ws.set('constraintSet').getSize()
pull_mWW = pulls.createPull(plot_mWW.getHist('theData'), 
                            plot_mWW.getObject(1))
cp_mWW = TCanvas('cp_mWW', pars.var[0] + ' mWW pull')
pull_mWW.SetName('mWW_pull')
pull_mWW.Draw('ap')
cp_mWW.SetGridy()
cp_mWW.Update()
pull_mWW.GetXaxis().SetLimits(
    pars_mWW.varRanges[pars_mWW.var[0]][1], 
    pars_mWW.varRanges[pars_mWW.var[0]][2]
    )
pull_mWW.GetXaxis().SetTitle(
    fitter_mWW.ws.var(pars_mWW.var[0]).getTitle(True).Data()
    )
pull_mWW.GetYaxis().SetTitle("pull")
cp_mWW.Update()

extraTag = '_sideband'
if mvaCutOverride:
    extraTag += '_mvaCut' + str(mvaCutOverride)
output = TFile("HWW%ilnujj_%s_%ijets_1D2Fit%s_output.root" % \
                   (opts.mH,mode, opts.Nj, extraTag),
               "recreate")

fr.SetName('fit_result')
fr.Write()
fr_mWW.SetName('fit_result_mWW')
fr_mWW.Write()
plot1.Write()
pull1.Write()
plot_mWW.Write()
pull_mWW.Write()
plot_mWW_withErrs.Write()
fitter.ws.Write()
fitter_mWW.ws.Write()

output.Close()

print 'Time elapsed: %.1f sec' % timer.RealTime()
print 'CPU time used: %.1f sec' % timer.CpuTime()

print '***',pars.var[0], 'fit ***'
print '%i degrees of freedom' % ndf
print 'chi2: %.2f / %i = %.2f' % (chi2_1, ndf, (chi2/ndf))
print 'chi2 probability: %.4g' % (TMath.Prob(chi2, ndf))

# print '*** low sideband fit ***'
# print '%i degrees of freedom' % ndf_low
# print 'chi2: %.2f / %i = %.2f' % (chi2_low, ndf_low, 
#                                   (chi2_low/ndf_low))
# print 'chi2 probability: %.4g' % (TMath.Prob(chi2_low, ndf_low))

# print '*** high sideband fit ***'
# print '%i degrees of freedom' % ndf_high
# print 'chi2: %.2f / %i = %.2f' % (chi2_high, ndf_high, 
#                                   (chi2_high/ndf_high))
# print 'chi2 probability: %.4g' % (TMath.Prob(chi2_high, ndf_high))

print '*** mWW fit ***'
print 'minimum NLL:', fr_mWW.minNll()
print '%i degrees of freedom' % ndf_mWW
print 'chi2: %.2f / %i = %.2f' % (chi2_mWW, ndf_mWW, 
                                  (chi2_mWW/ndf_mWW))
print 'chi2 probability: %.4g' % (TMath.Prob(chi2_mWW, ndf_mWW))
