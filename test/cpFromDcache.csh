#!/bin/tcsh

# copy data files from FNAL dCache resilient to local area
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJet_MC_Fall2010/
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJets_Data2011/

# cd /pnfs/cms/WAX/resilient/kalanand/WmunuJets_Data2011/

cd /pnfs/cms/WAX/resilient/kalanand/WmunuJets_MC/



##  `ls ZeeJets_*root`
##  `ls WmunuJets_*root`
##  `ls WenuJets_WJets_*root`

foreach datafile ( `ls WmunuJets_WJets_*root` ) 
	echo "submitting: dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_5/src/ElectroWeakAnalysis/VPlusJets/test/$datafile" 

	 dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_5/src/ElectroWeakAnalysis/VPlusJets/test/$datafile
    end
    echo ""


