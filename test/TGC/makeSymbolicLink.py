import os,sys
import string, re
from time import gmtime, localtime, strftime

parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 



def changeConfigFile(index, change):
    fName = "/uscmst1b_scratch/lpc1/3DayLifetime/kkrylova/MCFM/WW_" + parameters_wo_brackets[index] + change 
    child0 = os.system("ln -s "+fName + " " + parameters_wo_brackets[index] + change)
    print  fName+ " has been written.\n"

###################
for i in range(len(parameters_wo_brackets)):
    for j in range(len(Values)):
        changeConfigFile(i, Values[j])
