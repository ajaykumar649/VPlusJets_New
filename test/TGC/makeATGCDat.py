import os,sys
import string, re
from time import gmtime, localtime, strftime

ConfigFile = "inputWWToLnuJJ.DAT"
## parameters   = ["Delta_g1(Z)", "Delta_K(Z)", "Delta_K(gamma)", "Lambda(Z)", "Lambda(gamma)"]
## parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
## Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 




def changeConfigFile(change1, change2):
    subdir = "Lambda_Z_" + str(change1) + "Delta_Kgamma_" + str(change2)  
    dirName = "/uscmst1b_scratch/lpc1/3DayLifetime/kalanand/" +  subdir
    child = os.system("mkdir "+dirName)
    child0 = os.system("ln -s "+dirName + " " + subdir)
    child1 = os.system("cd "+dirName)    
    child2 = os.system( "ln -s ../Pdfdata " +dirName + "/Pdfdata")
    child3 = os.system( "ln -s ../process.DAT " +dirName + "/process.DAT")
    child4 = os.system("cd ..")       
    fin  = open(ConfigFile)
    pset_cfg      = "aTGC_" + subdir +".DAT"
    fout = open(dirName + "/"+ pset_cfg,"w")
    for line in fin.readlines():
        if  line.find("Lambda(Z)")!=-1:
            line=line.replace("0.0d0", str(change1)+"d0" )
        if  line.find("Lambda(gamma)")!=-1:
            line=line.replace("0.0d0", str(change1)+"d0" )
        if  line.find("Delta_K(gamma)")!=-1:
            line=line.replace("0.0d0", str(change2)+"d0" )
        if  line.find("Delta_K(Z)")!=-1:
            line=line.replace("0.0d0", str(-0.3009 * change2)+"d0")            
        fout.write(line)
    print pset_cfg + " has been written.\n"
    command = "./submitToCondor.py -c " + subdir + "/"+ pset_cfg 
    print "Submitting job: " + command + "\n"
    child9 = os.system(command)     

###################
for i in range(-3,4):
    for j in range(-3,4):
        changeConfigFile(0.2*i, 0.2*j)
