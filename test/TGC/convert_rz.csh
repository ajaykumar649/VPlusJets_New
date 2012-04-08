#!/bin/tcsh

# convert all .rz files in a given directory to .root file and then hadd them.
cd $1
source ../setup_h2root

foreach datafile ( `ls *.rz` ) 
	echo "submitting: h2root $datafile" 
	 h2root $datafile
    end

    hadd $1.root *.root
    echo ""



