import os,sys
import string, re
from time import gmtime, localtime, strftime

##------ Please set ONLY one of the four flags to True -------
physMode   = "WmunuJets_"
ConfigFile = "WmunuJetsAnalysis_cfg.py"
dataset    = ["/Mu/Run2010A-Apr21ReReco-v1/AOD"]
#dataset    = ["/Mu/Run2010B-Apr21ReReco-v1/AOD"]
#dataset    = ["/SingleMu/Run2011A-May10ReReco-v1/AOD"]
#dataset    = ["/SingleMu/Run2011A-PromptReco-v4/AOD"]
channels   = ["2010A-ReReco"]
#channels   = ["2010B-ReReco"]
#channels   = ["2011-May10ReReco"]
#channels   = ["PromptReco-v4"]
RunRange   = ["136033-149442"]
#RunRange   = ["160431-163869"]
#RunRange   = ["163870-167913"]
condor     = [1]
JSON         ="Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt" 
#JSON         ="Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v2.txt" 
#JSON         ="Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON.txt" 

MyResilientArea = "/pratima/CMSSW428/" +physMode +"CMSSW428-Data2010A-Apr21Rereco-CTJSON"
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
