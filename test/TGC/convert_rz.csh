#!/bin/tcsh

# convert all .rz files in a given directory to .root file and then hadd them.
source setup_h2root
cd $1


foreach datafile ( `ls *.rz` ) 
	echo "submitting: h2root $datafile" 
	 h2root $datafile
    end

    hadd $1.root *.root
    echo ""



