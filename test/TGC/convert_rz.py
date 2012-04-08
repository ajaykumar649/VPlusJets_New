import os,sys
import string, re
from time import gmtime, localtime, strftime

parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 


def clean_up_output(index, change):
    dirName = parameters_wo_brackets[index] + change 
    child = os.system("./convert_rz.csh " + dirName)
    print "The .rz files have been converted.\n"

###################
for i in range(len(parameters_wo_brackets)):
    for j in range(len(Values)):
        clean_up_output(i, Values[j])
                    
