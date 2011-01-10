import os,sys
import string, re
from time import gmtime, localtime, strftime


channels  = ["Pt_0to15",
            "Pt_15to20",
            "Pt_20to30",
            "Pt_30to50",
            "Pt_50to80",
            "Pt_80to120",
            "Pt_120to170",
            "Pt_170to230",
            "Pt_230to300",
            "Pt_300_Inf"
             ]


dataset  = ["/ZJetToEE_Pt_0to15_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_15to20_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_20to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_170to230_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_230to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
           "/ZJetToEE_Pt_300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO"       
            ]



condor  = [1,1,1,1,1,1,1,1,1,1]

MyResilientArea = "/kalanand/ZeeJet_MC_Fall2010"


def changeMainConfigFile(channel):
    fin  = open("ZeeJetsAnalysis_cfg.py")
    pset_cfg      = "py_" + channel + ".py"
    outfile_root  = "ZeeJets_" + channel + ".root"
    fout = open(pset_cfg,"w")
    for line in fin.readlines():
        if  line.find("isMC")!=-1:
            line=line.replace("False", "True")
        if  line.find("demo.root")!=-1:
            line=line.replace("demo.root",outfile_root)
        fout.write(line)
    print pset_cfg + " has been written.\n"


def changeCrabTemplateFile(outfile, index):
    fin  = open("crabTemplate.cfg")
    pset_cfg      = "py_" + outfile + ".py"
    pset_crab     = "crabjob_" + outfile + ".cfg"
    outfile_root  = "ZeeJets_" + outfile + ".root"
    fout = open(pset_crab,"w")
    for line in fin.readlines():
        if  line.find("mydataset")!=-1:
            line=line.replace("mydataset",dataset[index])
        if  line.find("total_number_of_lumis")!=-1:
            line=line.replace("total_number_of_lumis","total_number_of_events")
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
