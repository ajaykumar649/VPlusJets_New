#!/bin/tcsh

# copy data files from FNAL local to FNAL store/user/ using srmcp
mkdir /pnfs/cms/WAX/11/store/user/kalanand/aTGC/$1
cd $1

foreach datafile ( `ls *root *.rz` ) 
	echo "submitting: file://localhost//uscms_data/d2/kalanand/software/MCFM/Bin/$1/$datafile  srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/kalanand/aTGC/$1/$datafile" 

	 srmcp -2 -debug=true "file://localhost//uscms_data/d2/kalanand/software/MCFM/Bin/$1/$datafile" "srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/kalanand/aTGC/$1/$datafile"
    end
    echo ""



