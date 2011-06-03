#!/bin/tcsh

# copy data files from FNAL dCache resilient to local area
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJet_MC_Fall2010/
# cd /pnfs/cms/WAX/resilient/kalanand/ZeeJets_Data2011/

# cd /pnfs/cms/WAX/resilient/kalanand/WenuJets_Data2011/
# cd /pnfs/cms/WAX/resilient/kalanand/WenuJets_MC/

cd /pnfs/cms/WAX/11/store/user/kalanand/WenuJets_Data2011-DCSMay09/

##  `ls ZeeJets_*root`
##  `ls WmunuJets_*root`
##  `ls WenuJets_WJets_*root`
##  ls WmunuJets_WJets_*root
##  `ls WenuJets_WWToLNuQQ_M-*.root` 


foreach datafile ( `ls WenuJets_Prompt-*.root` ) 
	echo "submitting: dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_5/src/ElectroWeakAnalysis/VPlusJets/test/$datafile" 

	 dccp $datafile /uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_1_5/src/ElectroWeakAnalysis/VPlusJets/test/$datafile
    end
    echo ""


