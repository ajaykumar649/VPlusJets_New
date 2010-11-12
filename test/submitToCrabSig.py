import os,sys
import string, re
from time import gmtime, localtime, strftime


channels  = ["HLT_Photon10_L1R",
            "HLT_Photon15_Cleaned_L1R",
            "HLT_Ele15_SW_CaloEleId_L1R",
            "HLT_Ele17_SW_CaloEleId_L1R",
            "HLT_Ele17_SW_TightEleId_L1R",
            "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2",
            "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3"]

dataset  = ["/EG/Run2010A-Sep17ReReco_v2/RECO",
           "/EG/Run2010A-Sep17ReReco_v2/RECO",
           "/EG/Run2010A-Sep17ReReco_v2/RECO",
           "/Electron/Run2010B-PromptReco-v2/RECO",
           "/Electron/Run2010B-PromptReco-v2/RECO",
           "/Electron/Run2010B-PromptReco-v2/RECO",
           "/Electron/Run2010B-PromptReco-v2/RECO"
            ]

RunRange = ["132440-137028", #~0.1/pb
            "138564-140401", #~0.2/pb
            "141956-144114", #~3/pb
            "146428-147116", #~7/pb
            "147196-148058", #
            "148819-149064", #
            "149181-149442"  #
            ]

JSON = "Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON.txt"
condor  = [1,1,1,1,1,1,1,1,1,1]

MyResilientArea = "/kalanand/ZeeJet_Data2010"


def changeMainConfigFile(trigpath):
    fin  = open("ZeeJetsAnalysis_cfg.py")
    pset_cfg      = "py_" + trigpath + ".py"
    outfile_root  = "ZeeJets_" + trigpath + ".root"
    fout = open(pset_cfg,"w")
    for line in fin.readlines():
        if  line.find("HLT_Ele17_SW_TightEleId_L1R")!=-1:
            line=line.replace("HLT_Ele17_SW_TightEleId_L1R",trigpath)       
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
            fout.write("\n")
            fout.write("runselection="+RunRange[index]+"\n")
            fout.write("lumi_mask="+JSON+"\n")
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
