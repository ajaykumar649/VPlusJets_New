import os,sys
import string, re
from time import gmtime, localtime, strftime

##------ Please set ONLY one of the four flags to True -------
physMode   = "WenuJets_"
ConfigFile = "WenuJetsAnalysis_cfg.py"
dataset    = [
    #"/Zprime_Wjj_4_2_3_SIM/andersj-Zprime_Wjj_4_2_3_AODSIM-f71d043e41acd38c60e3392468355a0e/USER"
    #"/WHZH_Wjj_4_2_3_SIM/andersj-WHZH_Wjj_4_2_3_AODSIM-f71d043e41acd38c60e3392468355a0e/USER"
    "/Technicolor_Wjj_4_2_3_SIM/andersj-Technicolor_Wjj_4_2_3_AODSIM-f71d043e41acd38c60e3392468355a0e/USER"
    ]
channels   = [
    #"Zprime_MadGraph_WenuJJ"
    #"WH150qq_WenuJJ"
    "Technicolor_Pythia_WenuJJ"
    ]
condor   = [1]
## MyResilientArea = "/kalanand/" + physMode +"ZprimeMadGraph"
## MyResilientArea = "/kalanand/" + physMode +"WH150qq"
MyResilientArea = "/kalanand/" + physMode +"Technicolor"


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
            fout.write("dbs_url = http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet")
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
