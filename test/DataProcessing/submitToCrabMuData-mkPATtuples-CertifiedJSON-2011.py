#!/usr/bin/python
import os,sys
import string, re
from time import gmtime, localtime, strftime

physMode   = "WmunuJets_"
ConfigFile = "pat.py"

# To control which samples to run over, don't comment out here; rather
# uncomment/edit the line at the bottom: "#if i<X: continue" and set
# the condition appropriately.
#
dataset    = [
    "/Mu/Run2010A-Apr21ReReco-v1/AOD",
    "/Mu/Run2010B-Apr21ReReco-v1/AOD",
    "/SingleMu/Run2011A-May10ReReco-v1/AOD",
    "/SingleMu/Run2011A-PromptReco-v4/AOD",
    "/SingleMu/Run2011A-05Aug2011-v1/AOD",
    "/SingleMu/Run2011A-PromptReco-v6/AOD",
    "/SingleMu/Run2011B-PromptReco-v1/AOD"]
channels   = [
    "Mu-Run2010A-Apr21ReReco-v1-FNAL-v0",
    "Mu-Run2010B-Apr21ReReco-v1-FNAL-v0",
    "SingleMu-Run2011A-May10ReReco-v1-FNAL-v0",
    "SingleMu-Run2011A-PromptReco-v4-FNAL-v0",
    "SingleMu-Run2011A-05Aug2011-v1-FNAL-v0",
    "SingleMu-Run2011A-PromptReco-v6-FNAL-v0",
    "SingleMu-Run2011B-PromptReco-v1-FNAL-v0"]
RunRange   = [
    "136033-149442",
    "136033-149442",
    "160404-163869",
    "165088-167913",
    "170249-172619",
    "172620-173692",
    "175832-180252"]

JSON       = [
    "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
    "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
    "Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt"]
condor     = [1,1,1,1,1,1,1] # Total jobs 7 now

MyResilientArea = "/store/user/lnujj/CMSSW428/" +physMode

## ------------------------------------------

def changeMainConfigFile():
    fin  = open(ConfigFile)
    pset_cfg      = "pat_42x_data.py"
    fout = open(pset_cfg,"w")
    for line in fin.readlines(): 
        if  line.find("outputFile = ")!=-1:
            fout.write("outputFile = 'pat_42x_data.root'\n")
        else:
            fout.write(line)
    print pset_cfg + " has been written.\n"

def changeCrabTemplateFile(outfile, index, nowJSON, nowRang):
    fin  = open("crabTemplatePAT.cfg")
    pset_cfg      = "pat_42x_data.py"
    pset_crab     = physMode + "cb_" + outfile + ".cfg"
    outfile_root  = physMode + outfile + ".root"
    fout = open(pset_crab,"w")
    for line in fin.readlines():
        if  line.find("mydataset")!=-1:
            line=line.replace("mydataset",dataset[index])
            fout.write("\n")
            fout.write("runselection="+RunRange[index]+"\n")
            fout.write("lumi_mask="+nowJSON+"\n")
        if line.find("myanalysis")!=-1:
            line=line.replace("myanalysis",pset_cfg)    
        if line.find("myworkingdir")!=-1:
            line=line.replace("myworkingdir",dataset[index])    
        if line.find("mypublishname")!=-1:
            line=line.replace("mypublishname",physMode+outfile)    
        if  line.find("myrootfile")!=-1:
            line=line.replace("myrootfile",outfile_root)
        if  line.find("myresilient")!=-1:
            line=line.replace("myresilient",MyResilientArea+nowRang+"_"+outfile)    
        if line.find("glite")!=-1 and condor[index]!=0:
            line=line.replace("glite", "condor")        
        fout.write(line)        
    if condor[index]!=0:
        fout.write("ce_white_list = cmssrm.fnal.gov")
      
    print pset_crab + " has been written.\n"

    
###################
changeMainConfigFile()
for i in range(len(channels)):
    changeCrabTemplateFile(channels[i],i,JSON[i],RunRange[i])

for i in range(len(channels)):
    #if i<9: continue
    submitcommand = "crab -create -cfg " + physMode + "cb_" + channels[i] + ".cfg"
    child   = os.system(submitcommand)
    child2   = os.system("crab -submit -c crabDir/"+dataset[i])
