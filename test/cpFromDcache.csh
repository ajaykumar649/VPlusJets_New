#!/bin/tcsh

# copy data files from FNAL dCache resilient to local area

cd /pnfs/cms/WAX/resilient/kalanand/ZeeJet_MC_Fall2010/

foreach datafile ( `ls ZeeJets_*root` ) 
	echo "submitting: dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_3_8_5_patch2/src/ElectroWeakAnalysis/VPlusJets/test/$datafile" 

	 dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_3_8_5_patch2/src/ElectroWeakAnalysis/VPlusJets/test/$datafile
    end
    echo ""


