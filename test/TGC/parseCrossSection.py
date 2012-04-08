import os,sys
import string, re
from time import gmtime, localtime, strftime

parameters   = ["Delta_g1(Z)", "Delta_K(Z)", "Delta_K(gamma)", "Lambda(Z)", "Lambda(gamma)"]
parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 



def change(index, change):
    dirName = "/uscms_data/d3/ksiehl/MCFM/Bin/condor/logs/"
    infile = dirName + "aTGC_" + parameters_wo_brackets[index] + change + ".stdout"
    fin  = open(infile)
    fout = open("cross_sections_atgc.txt","a")
    for line in fin.readlines():
        if  line.find("Value of final tota integral is      ")!=-1:
            line=line.replace("Value of final tota integral is", parameters_wo_brackets[index] + change + ": ")
            fout.write(line)


###################
for i in range(len(parameters)):
    for j in range(len(Values)):
        change(i, Values[j])
