import os,sys
import string, re
from time import gmtime, localtime, strftime

##------ Please set ONLY one of the four flags to True -------
physMode   = "ZmumuJets_"
ConfigFile = "ZmumuJetsAnalysis_cfg.py"

## ----- 2010A
#dataset    =     ["/Mu/Run2010A-Apr21ReReco-v1/AOD"]
#channels   =         ["Run2010A-Apr21ReReco-v1"]
#outputdir  = "CMSSW428-Run2010A-Apr21ReReco-v1"
#RunRange   =     ["136033-149442"]
#JSON       = "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt"


## ----- 2010B
#dataset    =     ["/Mu/Run2010B-Apr21ReReco-v1/AOD"]
#channels   =         ["Run2010B-Apr21ReReco-v1"]
#outputdir  = "CMSSW428-Run2010B-Apr21ReReco-v1"
#RunRange   =     ["136033-149442"]
#JSON       = "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt" 


## ----- 2011A May10ReReco
#dataset    = ["/DoubleMu/Run2011A-May10ReReco-v1/AOD"]
#channels   =           ["Run2011A-May10ReReco-v1"]
#outputdir  =   "CMSSW428-Run2011A-May10ReReco-v1"
#RunRange   =     ["160404-163869"]
#JSON       = "Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt" 


## ----- 2011A PromptReco-v4
#dataset    = ["/DoubleMu/Run2011A-PromptReco-v4/AOD"]
#channels   =           ["Run2011A-PromptReco-v4"]
#outputdir  =   "CMSSW428-Run2011A-PromptReco-v4"
#RunRange   =     ["165071-168437"]
#JSON       = "Cert_160404-173692_7TeV_PromptReco_Collisions11_JSON.txt" 


## ----- 2011A Run2011A-05Aug2011-v1
#dataset    = ["/DoubleMu/Run2011A-05Aug2011-v1/AOD"]
#channels   =           ["Run2011A-05Aug2011-v1"]
#outputdir  =   "CMSSW428-Run2011A-05Aug2011-v1"
#RunRange   =     ["170249-172619"]
#JSON       = "Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.txt" 


# ----- 2011A Run2011A-PromptReco-v6
dataset    = ["/DoubleMu/Run2011A-PromptReco-v6/AOD"]
channels   =           ["Run2011A-PromptReco-v6"]
outputdir  =   "CMSSW428-Run2011A-PromptReco-v6"
RunRange   =     ["172620-175770"]
JSON       = "Cert_160404-173692_7TeV_PromptReco_Collisions11_JSON.txt" 


## ------------------------------------------
condor     = [1]
MyResilientArea = "/yangf/CMSSW428/" +physMode + outputdir
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
