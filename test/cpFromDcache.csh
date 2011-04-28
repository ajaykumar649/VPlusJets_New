#!/bin/tcsh

# copy data files from FNAL dCache resilient to local area
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJet_MC_Fall2010/
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJets_Data2011/

cd /pnfs/cms/WAX/resilient/kalanand/WenuJets_Data2011/

##  `ls ZeeJets_*root`

foreach datafile ( `ls WenuJets_*root` ) 
	echo "submitting: dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_3_patch2/src/ElectroWeakAnalysis/VPlusJets/test/$datafile" 

	 dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_3_patch2/src/ElectroWeakAnalysis/VPlusJets/test/$datafile
    end
    echo ""


