#import sys
#sys.path.append('/uscms/home/andersj/pyroot')
#del sys
import math
#import root_logon
#import pyroot_fwlite.py
from optparse import OptionParser
parser = OptionParser()
parser.add_option('-H', '--mH', dest='mH', type='float',
                  help='Higgs Mass Point')
(opts, args) = parser.parse_args()

import pyroot_logon
from ROOT import TFile, TH1F, TCanvas, TLorentzVector, TTree, gSystem, gROOT
from DataFormats.FWLite import Events, Handle
from array import array

import HWWSignalShapes

#files = ['WHZH_GENSIM.root']
#from WHZHFiles import files

#files = ['dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/user/andersj/WHZH_Wjj_4_2_3_SIM/WHZH_Wjj_4_2_3_SIM/c3d0044b4728087b67b7d100b833fffe/WHZH_GENSIM_2_1_EUL.root']

# files = ['dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_53X-v1/shuai/GluGluToHToWWToLAndTauNuQQ_M-350_8TeV-powheg-pythia6/SQWaT_PAT_53X_ggH350_central/829f288d768dd564418efaaf3a8ab9aa/pat_53x_v03_60_1_cPR.root']

files = [ x.rstrip() for x in open(args[0]).readlines() ]

print files[0]

events = Events (files)
handle = Handle('vector<reco::GenParticle>')
label = ('genParticles')

def printParticleDecayTree(mom, prefix = '', statusCutoff = 1):
    if mom.status() >= statusCutoff:
        print prefix+'id:',mom.pdgId(),'status:',mom.status(),\
            'nDau:',mom.numberOfDaughters()
        for dau in range(0, mom.numberOfDaughters()):
            daughter = mom.daughter(dau)
            if daughter.status() >= statusCutoff:
                printParticleDecayTree(daughter, '  ' + prefix, statusCutoff)

def getStableDaughter(part):
    if (part.status() == 1):
        return part
    for dau in range(0, part.numberOfDaughters()):
        daughter = part.daughter(dau)
        if (part.pdgId() == daughter.pdgId()):
            if (daughter.status() == 1):
                return daughter
            else:
                getStableDaughter(daughter)
    return None

def resetVars():

    Higgs_mass[0] = -999.
    Event_wgt[0] = -999.
    # Running_wgt[0] = -999.

outputFile = TFile('output.root', 'recreate')
outTree = TTree("outTree", "outTree")

Higgs_mass = array('d', [0.])
Event_wgt = array('d', [0.])
Running_wgt = array('d', [0.])

outTree.Branch('Higgs_mass', Higgs_mass, 'Higgs_mass/D')
outTree.Branch('CP_wgt', Event_wgt, 'CP_wgt/D')
#outTree.Branch('Running_wgt', Running_wgt, 'Running_wgt/D')

decayCnts = {}
sumWgts = 0.
sumRunning = 0.
for (eventN,event) in enumerate(events):
    resetVars()
    event.getByLabel(label, handle)
    genParts = handle.product()

    if (eventN % 100 == 0):
        print "Record:",eventN,
        print "Run:",event.object().id().run(),
        print "event:",event.object().id().event()
    for particle in genParts:
        if (abs(particle.pdgId()) == 25) and (particle.status() == 3): 
            dauMode = abs(particle.daughter(0).pdgId())
            if not (dauMode in decayCnts):
                decayCnts[dauMode] = 0
            decayCnts[dauMode] += 1
	    # printParticleDecayTree(particle, statusCutoff = 3)
            Higgs_mass[0] = particle.mass()
            Event_wgt[0] = HWWSignalShapes.HiggsCPWeight(opts.mH,
                                                         particle.mass())
            sumWgts += Event_wgt[0]

            # Running_wgt[0] = HWWSignalShapes.runningWidthWeight(350, 
            #                                                     particle.mass())
            # sumRunning += Running_wgt[0]
            break

    outTree.Fill()
    if eventN >= 10000:
        break

print decayCnts
#print "n events:",eventN
print "avg CP weight:",sumWgts/(eventN+1)
# print "avg running width weight:", sumRunning/(eventN+1)

outputFile.Write()
outputFile.Close()
