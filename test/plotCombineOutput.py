
def plotAVariable(var, fitter):
    import RooWjj2DFitter
    plot = fitter.stackedPlot(var)
    leg = RooWjj2DFitter.Wjj2DFitter.legend4Plot(plot)
    plot.addObject(leg)

    return plot


from optparse import OptionParser

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=False,
                  help='no X11 windows')
parser.add_option('-m', '--mode', default="HWW2DConfig", dest='modeConfig',
                  help='which config to select look at HWW2DConfig.py for ' +\
                      'an example.  Use the file name minus the .py extension.'
                  )
parser.add_option('-H', '--mH', dest='mH', default=350, type='int',
                  help='Higgs Mass Point')
parser.add_option('-j', '--Njets', dest='Nj', default=2, type='int',
                  help='Number of jets.')
parser.add_option('--electrons', dest='isElectron', action='store_true',
                  default=False, help='do electrons instead of muons')
parser.add_option('--mlfit', dest='mlfitname', default='mlfitHWW2lnujjObs.root',
                  help='output filename from ML fit of combine.')
parser.add_option('--ws', dest='ws', 
                  default='wkspace_8TeV_hwwmunu2j_moriond_19p3fbinv_M=350.root',
                  help='filename for combine workspace.')
parser.add_option('--result', dest='result', 
                  default='HWWlnujjH350_muon_2jets_output.root',
                  help='filename for the result that is the input to the ' +\
                      'combine fitter.')
parser.add_option('--debug', dest='debug', action='store_true', default=False,
                  help='turn on extra debugging information')

(opts, args) = parser.parse_args()

import pyroot_logon
from ROOT import *
config = __import__(opts.modeConfig)
import RooWjj2DFitter
import pulls

outputFilename = opts.mlfitname
outputFile = TFile(outputFilename)
combineInputFilename = opts.ws
combineInputFile = TFile(combineInputFilename)
inputFilename = opts.result
inputFile = TFile(inputFilename)

pars = config.theConfig(Nj = opts.Nj, mH = opts.mH, 
                        isElectron = opts.isElectron, initFile = args,
                        includeSignal = True)
if not opts.debug:
    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

fitter = RooWjj2DFitter.Wjj2DFitter(pars)

totalPdf = fitter.makeFitter()
fitter.loadWorkspaceFromFile(inputFilename, getFloatPars = False)
RooAbsCollection.__assign__(fitter.ws.allVars(), inputFile.Get('w').allVars())
w = combineInputFile.Get('w')

plots = []
cans = []

for sig in ['b', 's']:
    fit_res = outputFile.Get('fit_%s' % sig)
    RooAbsCollection.__assign__(w.allVars(), fit_res.floatParsFinal())
    RooAbsCollection.__assign__(fitter.ws.allVars(), fit_res.floatParsFinal())

    funcs = RooArgSet(w.allFunctions())
    funcIter = funcs.createIterator()
    func = funcIter.Next()
    while func:
        funcName = func.GetName()
        if funcName[0:5] == 'n_exp':
            parts = funcName.split('_')
            n = fitter.ws.var('n_%s' % parts[-1])
            nrm = fitter.ws.var('%s_nrm' % parts[-1])
            if nrm:
                print 'using', funcName, '=', func.getVal(),
                print 'to change', nrm.GetName(), nrm.getVal(), '->', 
                nrm.setVal(func.getVal()/n.getVal())
                print nrm.getVal()
        
        func = funcIter.Next()

    fitter.ws.var('r_signal').setVal(w.var('r').getVal())
    if sig == 'b':
        fitter.ws.var('r_signal').setVal(0.)

    print 'using r for signal:',fitter.ws.var('r_signal').getVal(),
    print 'value from combine:',w.var('r').getVal()
    print

    for v in pars.var:
        can = TCanvas('can_%s_%s' % (v,sig) , '%s plot %s' % (v,sig))
        plot = plotAVariable(v, fitter)
        plot.SetName('%s_%s' % (plot.GetName(), sig))
        plot.Draw()
        can.Update()
        pullplot = pulls.createPull(plot.getHist('theData'), plot.getObject(1))
        pullplot.SetName('%s_pull_%s' % (v, sig))
        canpull = TCanvas('can_%s_pull_%s' % (v,sig), '%s pulls %s' % (v,sig))
        canpull.SetGridy()
        pullplot.Draw('ap')
        canpull.Update()
        pullplot.GetXaxis().SetLimits(pars.varRanges[v][1], pars.varRanges[v][2])
        pullplot.GetXaxis().SetTitle(fitter.ws.var(v).getTitle(True).Data())
        pullplot.GetYaxis().SetTitle('pull')
        canpull.Update()
        plots += [plot, pullplot]
        cans += [can, canpull]
    
mode = 'muon'
if opts.isElectron:
    mode = 'electron'

plotOutput = TFile("HWWlnujjH%i_%s_%ijets_PostCombinePlots.root" % \
                       (opts.mH, mode, opts.Nj),
                   'recreate')
for plot in plots:
    plot.Write()

plotOutput.ls()
plotOutput.Close()
