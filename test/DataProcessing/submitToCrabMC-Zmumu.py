import os,sys
import string, re
from time import gmtime, localtime, strftime

physMode   = "ZmumuJets_"
ConfigFile = "ZmumuJetsAnalysis_cfg.py"

dataset    = [
    "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/AODSIM",
    "/TTTo2L2Nu2B_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-130_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-150_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-190_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-200_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-210_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-220_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-230_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-250_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-275_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-300_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-325_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-350_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-375_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-400_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-425_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-450_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-475_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-500_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-525_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-550_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-575_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/GluGluToHToZZTo2L2Q_M-600_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM",
    "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/Tbar_TuneZ2_tW-channel-DS_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM"
    "/T_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/T_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
    "/T_TuneZ2_tW-channel-DS_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM"
    ]
channels   = [
    "WJets",
    "WWtoAnything",
    "WZtoAnything",    
    "ZZtoAnything",    
    "TTJets",
    "TTToLNu2Q2B",            
    "ZZTo2L2Q_M-130",
    "ZZTo2L2Q_M-150",
    "ZZTo2L2Q_M-190",
    "ZZTo2L2Q_M-200",
    "ZZTo2L2Q_M-210",
    "ZZTo2L2Q_M-220",
    "ZZTo2L2Q_M-230",
    "ZZTo2L2Q_M-250",
    "ZZTo2L2Q_M-275",
    "ZZTo2L2Q_M-300",
    "ZZTo2L2Q_M-325",
    "ZZTo2L2Q_M-350",
    "ZZTo2L2Q_M-375",
    "ZZTo2L2Q_M-400",
    "ZZTo2L2Q_M-425",
    "ZZTo2L2Q_M-450",
    "ZZTo2L2Q_M-475",
    "ZZTo2L2Q_M-500",
    "ZZTo2L2Q_M-525",
    "ZZTo2L2Q_M-550",
    "ZZTo2L2Q_M-575",
    "ZZTo2L2Q_M-600",
    "QCD_Pt-20_MuEnrichedPt-15",
    "DYJetsToLL",
    "DYToTauTau",
    "Tbar_s-channel",
    "Tbar_t-channel",
    "Tbar_tW-channel",
    "T_s-channel_powheg",
    "T_t-channel_powheg",
    "T_tW-channel_powheg"
]
condor   = [1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1]
MyResilientArea = "/pratima/" + physMode +"CMSSW423-Summer11MC"

blacklist = []
#blacklist = [ "wisc.edu,uerj.br" ]

def changeMainConfigFile(trigpath):
    fin  = open(ConfigFile)
    pset_cfg      = "py_" + physMode + trigpath + ".py"
    outfile_root  = physMode + trigpath + ".root"
    fout = open(pset_cfg,"w")
    for line in fin.readlines():
        if  line.find("isMC = False")!=-1:
            line=line.replace("isMC = False", "isMC = True")       
        if  line.find("demo.root")!=-1:
            line=line.replace("demo.root",outfile_root)
        fout.write(line)
    print pset_cfg + " has been written.\n"


def changeCrabTemplateFile(outfile, index):
    fin  = open("crabTemplate.cfg")
    pset_cfg      = "py_" + physMode + outfile + ".py"
    pset_crab     = "crabjob_" + physMode + outfile + ".cfg"
    outfile_root  = physMode + outfile + ".root"
    fout = open(pset_crab,"w")
    for line in fin.readlines():
        if  line.find("mydataset")!=-1:
            line=line.replace("mydataset",dataset[index])
            fout.write("\n")
        if  line.find("total_number_of_lumis")!=-1:
            line=line.replace("total_number_of_lumis=-1","total_number_of_events=-1")
        if  line.find("lumis_per_job")!=-1:
            line=line.replace("lumis_per_job = 100","events_per_job = 50000")     
        if line.find("myanalysis")!=-1:
            line=line.replace("myanalysis",pset_cfg)    
        if  line.find("myrootfile")!=-1:
            line=line.replace("myrootfile",outfile_root)
        if  line.find("myresilient")!=-1:
            line=line.replace("myresilient",MyResilientArea)    
        if line.find("glite")!=-1 and condor[index]!=0:
            line=line.replace("glite", "condor")
        fout.write(line)        
    if condor[index]!=0:
        fout.write("ce_white_list = cmssrm.fnal.gov")
    if len(blacklist)>0:
        fout.write("ce_black_list = ");
        for i in range(len(blacklist)):
            fout.write(blacklist[i])
        fout.write("\n");
    print pset_crab + " has been written.\n"

    
###################
for i in range(len(channels)):
    changeMainConfigFile(channels[i])
    changeCrabTemplateFile(channels[i],i)

for i in range(len(channels)):
    #if i<9: continue
    submitcommand = "crab -create -cfg " + "crabjob_" + physMode + channels[i] + ".cfg"
    child   = os.system(submitcommand)
    child2   = os.system("crab -submit")
