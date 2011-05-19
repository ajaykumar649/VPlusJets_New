import os,sys
import string, re
from time import gmtime, localtime, strftime

##------ Please set ONLY one of the four flags to True -------
physMode   = "WmunuJets_"
ConfigFile = "WmunuJetsAnalysis_cfg.py"
dataset    = ["/SingleMu/Run2011A-PromptReco-v1/AOD",
              "/SingleMu/Run2011A-PromptReco-v2/AOD",
              "/SingleMu/Run2011A-PromptReco-v2/AOD"
              ]
channels   = ["Prompt-V1-1",
              "Prompt-V2-1",
              "Prompt-V2-2"]
RunRange   = ["160329-161312",
              "162718-163754",
              "163755-163869"
              ]
condor     = [1,
              1,
              1]
JSON       = "Cert_160404-163869_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"
MyResilientArea = "/yangf/" +physMode +"CMSSW415-Data2011-May13CTMuonJSON"
## ------------------------------------------


def changeMainConfigFile(trigpath):
    fin  = open(ConfigFile)
    pset_cfg      = "py_" + trigpath + ".py"
    outfile_root  = physMode + trigpath + ".root"
    fout = open(pset_cfg,"w")
    for line in fin.readlines(): 
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
