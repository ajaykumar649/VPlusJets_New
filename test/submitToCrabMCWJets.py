import os,sys
import string, re
from time import gmtime, localtime, strftime


physMode = "VH_"
ConfigFile = "WenuJetsAnalysis_cfg.py"


channels  = [
    "WJets",
    "WWtoAnything",
    "WZtoAnything",    
    "TTToLNu2Q2B",            
    "WH_WToLNu_HToBB_M-115",
    "WH_WToLNu_HToBB_M-120",
    "WH_WToLNu_HToBB_M-125", 
    "WH_WToLNu_HToBB_M-130",
    "WH_WToLNu_HToBB_M-135",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-140",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-150",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-160",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-170",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-180",
##             "WH_ZH_TTH_HToZZTo2L2Q_M-190",
##             "WH_ZH_TTH_HToWW_M-120",
##             "WH_ZH_TTH_HToWW_M-130",
##             "WH_ZH_TTH_HToWW_M-140",
##             "WH_ZH_TTH_HToWW_M-150",
##             "WH_ZH_TTH_HToWW_M-160",
##             "WH_ZH_TTH_HToWW_M-170",
##             "WH_ZH_TTH_HToWW_M-180",
##             "WH_ZH_TTH_HToWW_M-190",
             ]

dataset  = [
    "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Winter10-E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/AODSIM",
    "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
    "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/AODSIM",    
    #"/TTToLNu2Q2B_7TeV-powheg-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM"
    "/TTToLNu2Q2B_7TeV-powheg-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",            
    "/WH_WToLNu_HToBB_M-115_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
    "/WH_WToLNu_HToBB_M-120_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
    "/WH_WToLNu_HToBB_M-125_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM", 
    "/WH_WToLNu_HToBB_M-130_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
    "/WH_WToLNu_HToBB_M-135_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
            
##             "/ZH_ZToNuNu_HToBB_M-115_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM ",
##             "/ZH_ZToNuNu_HToBB_M-120_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/ZH_ZToNuNu_HToBB_M-125_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/ZH_ZToNuNu_HToBB_M-130_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/ZH_ZToNuNu_HToBB_M-135_7TeV-powheg-herwigpp/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM", 
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-140_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-150_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-160_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-170_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-180_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##            "/WH_ZH_TTH_HToZZTo2L2Q_M-190_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-120_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-130_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-140_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-150_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-160_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-170_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-180_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
##             "/WH_ZH_TTH_HToWW_M-190_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
            ]


condor  = [1,0,0,0,1,1,1,1,1]
MyResilientArea = "/kalanand/" + physMode +"MC"


def changeMainConfigFile(trigpath):
    fin  = open(ConfigFile)
    pset_cfg      = "py_" + trigpath + ".py"
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
    pset_cfg      = "py_" + outfile + ".py"
    pset_crab     = "crabjob_" + outfile + ".cfg"
    outfile_root  = physMode + outfile + ".root"
    fout = open(pset_crab,"w")
    for line in fin.readlines():
        if  line.find("mydataset")!=-1:
            line=line.replace("mydataset",dataset[index])
            fout.write("\n")
        if  line.find("total_number_of_lumis")!=-1:
            line=line.replace("total_number_of_lumis=-1","total_number_of_events=-1")
        if  line.find("lumis_per_job")!=-1:
            line=line.replace("lumis_per_job = 200","events_per_job = 100000")     
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
      
    print pset_crab + " has been written.\n"


    
###################
for i in range(len(channels)):
    changeMainConfigFile(channels[i])
    changeCrabTemplateFile(channels[i],i)

for i in range(len(channels)):
    #if i<9: continue
    submitcommand = "crab -create -cfg " + "crabjob_" + channels[i] + ".cfg"
    child   = os.system(submitcommand)
    child2   = os.system("crab -submit")
