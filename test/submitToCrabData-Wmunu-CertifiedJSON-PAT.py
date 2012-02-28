import os,sys
import string, re
from time import gmtime, localtime, strftime

physMode   = "PAT-WmunuJets_"
ConfigFile = "WmunuJetsAnalysisPAT_cfg.py"
DefTrig    = "'HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15_v*','HLT_Mu17_v*','HLT_Mu24_v*','HLT_Mu30_v*','HLT_IsoMu17_v*','HLT_IsoMu24_v*','HLT_IsoMu30_v*'"

dataset    = [
    "/PAT-WMu/Run2010A-Apr21ReReco-v1/AOD",
    "/PAT-WMu/Run2010B-Apr21ReReco-v1/AOD",
    "/PAT-WSingleMu/Run2011A-May10ReReco-v1/AOD",
    "/PAT-WSingleMu/Run2011A-PromptReco-v4/AOD",
    "/PAT-WSingleMu/Run2011A-PromptReco-v4/AOD",
    "/PAT-WSingleMu/Run2011A-05Aug2011-v1/AOD",
    "/PAT-WSingleMu/Run2011A-05Aug2011-v1/AOD",
    "/PAT-WSingleMu/Run2011A-PromptReco-v6/AOD",
    "/PAT-WSingleMu/Run2011A-PromptReco-v6/AOD",
    "/PAT-WSingleMu/Run2011B-PromptReco-v1/AOD",
    "/PAT-WSingleMu/Run2011B-PromptReco-v1/AOD",
    "/PAT-WSingleMu/Run2011B-PromptReco-v1/AOD",
    "/PAT-WSingleMu/Run2011B-PromptReco-v1/AOD"]
channels   = [
    "PAT-WMu-Run2010A-Apr21ReReco-v1-trigv2010",
    "PAT-WMu-Run2010B-Apr21ReReco-v1-trigv2010",
    "PAT-WSingleMu-Run2011A-May10ReReco-v1-trig5E32",
    "PAT-WSingleMu-Run2011A-PromptReco-v4-trig1E33",
    "PAT-WSingleMu-Run2011A-PromptReco-v4-trig1p4E33",
    "PAT-WSingleMu-Run2011A-05Aug2011-v1-trigv2E33v11",
    "PAT-WSingleMu-Run2011A-05Aug2011-v1-trigv2E33v12",
    "PAT-WSingleMu-Run2011A-PromptReco-v6-trigv2E33v12",
    "PAT-WSingleMu-Run2011A-PromptReco-v6-trigv3E33v00",
    "PAT-WSingleMu-Run2011B-PromptReco-v1-trigv3E33v20",
    "PAT-WSingleMu-Run2011B-PromptReco-v1-trigv3E33v23",
    "PAT-WSingleMu-Run2011B-PromptReco-v1-trigv5E33v14",
    "PAT-WSingleMu-Run2011B-PromptReco-v1-trigv5E33v22"]
trigname   = [
    "'HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15_v*','HLT_Mu17_v*','HLT_Mu24_v*'",
    "'HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15_v*','HLT_Mu17_v*','HLT_Mu24_v*'",
    "'HLT_IsoMu17_v*','HLT_Mu30_v*'",
    "'HLT_IsoMu17_v*','HLT_Mu30_v*'",
    "'HLT_IsoMu17_v*','HLT_Mu30_v*'",
    "'HLT_IsoMu17_v13','HLT_IsoMu20_v8','HLT_IsoMu24_v8','HLT_Mu30_v7','HLT_Mu40_v5'",
    "'HLT_IsoMu17_v13','HLT_IsoMu20_v8','HLT_IsoMu24_v8','HLT_Mu30_v7','HLT_Mu40_v5'",
    "'HLT_IsoMu17_v13','HLT_IsoMu20_v8','HLT_IsoMu24_v8','HLT_Mu30_v7','HLT_Mu40_v5'",
    "'HLT_IsoMu20_v9','HLT_Mu40_eta2p1_v1'",
    "'HLT_IsoMu30_eta2p1_v3','HLT_IsoMu24_eta2p1_v3','HLT_IsoMu24_v9','HLT_IsoMu20_v9','HLT_Mu40_eta2p1_v1','HLT_Mu40_v6'",
    "'HLT_IsoMu30_eta2p1_v3','HLT_IsoMu24_eta2p1_v3','HLT_IsoMu24_v9','HLT_IsoMu20_v9','HLT_Mu40_eta2p1_v1','HLT_Mu40_v6'",
    "'HLT_IsoMu30_eta2p1_v6','HLT_IsoMu24_eta2p1_v6','HLT_IsoMu24_v12','HLT_IsoMu30_eta2p1_v7','HLT_IsoMu24_eta2p1_v7','HLT_IsoMu24_v13','HLT_Mu40_eta2p1_v4','HLT_Mu40_v9'",
    "'HLT_IsoMu30_eta2p1_v6','HLT_IsoMu24_eta2p1_v6','HLT_IsoMu24_v12','HLT_IsoMu30_eta2p1_v7','HLT_IsoMu24_eta2p1_v7','HLT_IsoMu24_v13','HLT_Mu40_eta2p1_v5','HLT_Mu40_v10'"]
RunRange   = [
    "136033-149442",
    "136033-149442",
    "160404-163869",
    "165088-167038",
    "167039-167913",
    "170249-170825",
    "170826-172619",
    "172620-173235",
    "173236-173692",
    "175832-176460",
    "176461-178419",
    "178420-179958",
    "179959-180252"]
JSON       = [
    "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
    "Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
    "Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt",
    "Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt",
    "Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt"]
condor     = [1,1,1,1,1,1,1,1,1,1,1,1,1] # Total jobs 13 now

MyResilientArea = "/yangf/CMSSW428/" +physMode

## ------------------------------------------


def changeMainConfigFile(trigpath,nowtrigname):
    fin  = open(ConfigFile)
    pset_cfg      = physMode + "py_" + trigpath + ".py"
    outfile_root  = physMode + trigpath + ".root"
    fout = open(pset_cfg,"w")
    for line in fin.readlines(): 
        if  line.find("demo.root")!=-1:
            line=line.replace("demo.root",outfile_root)
        if  line.find(DefTrig)!=-1:
            line=line.replace(DefTrig,nowtrigname)
        fout.write(line)
    print pset_cfg + " has been written.\n"


def changeCrabTemplateFile(outfile, index, nowJSON, nowRang):
    fin  = open("crabTemplate.cfg")
    pset_cfg      = physMode + "py_" + outfile + ".py"
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
            fout.write("dbs_url = http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet"+"\n")
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
for i in range(len(channels)):
    changeMainConfigFile(channels[i],trigname[i])
    changeCrabTemplateFile(channels[i],i,JSON[i],RunRange[i])

for i in range(len(channels)):
    #if i<9: continue
    submitcommand = "crab -create -cfg " + physMode + "cb_" + channels[i] + ".cfg"
    child   = os.system(submitcommand)
    child2   = os.system("crab -submit")
