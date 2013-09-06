#!/bin/bash
cfgtag=jakeHWWHCPscan
#DIR=/uscms_data/d2/andersj/Wjj/2012/CMSSW_5_3_3_patch2/src/ElectroWeakAnalysis/VPlusJets/test/HWWFit24July2013
#DIR=/uscms_data/d2/andersj/Wjj/2013/CMSSW_6_2_0/src/ElectroWeakAnalysis/VPlusJets/test/HWWTest_HCP
#DIR=/uscms_data/d2/andersj/Wjj/2013/CMSSW_6_2_0/src/ElectroWeakAnalysis/VPlusJets/test/HWWTweakFit
#DIR=/uscms_data/d2/andersj/Wjj/2013/CMSSW_6_2_0/src/ElectroWeakAnalysis/VPlusJets/test/HWWHCP_NewFit
DIR=/uscms_data/d2/andersj/Wjj/2013/CMSSW_6_2_0/src/ElectroWeakAnalysis/VPlusJets/test/HWWHCP_scan

for mass in 170 180 190 200 250 300 350 400 450 500 550 600
do
  for flavor in electron muon
  do
    ./makeDataCardFromRooWorkspace.exe ${DIR}/HWW${mass}lnujj_${flavor}_2jets_1D2Fit_output.root $cfgtag
  done
# combine the flavors:
  ./movem 's#M=#M-#g' datacard_8TeV_hww??nu2j_${cfgtag}-M=${mass}.txt
  hwwelnu2j=datacard_8TeV_hwwelnu2j_${cfgtag}-M-${mass}.txt
  hwwmunu2j=datacard_8TeV_hwwmunu2j_${cfgtag}-M-${mass}.txt
  echo "combineCards.py hwwmunu2j=${hwwmunu2j} hwwelnu2j=${hwwelnu2j} >datacard_8TeV_hww2chan_${cfgtag}-M=${mass}.txt"
  combineCards.py hwwmunu2j=${hwwmunu2j} hwwelnu2j=${hwwelnu2j} >datacard_8TeV_hww2chan_${cfgtag}-M=${mass}.txt
  ./movem 's#M-#M=#g' datacard_8TeV_hww??nu2j_${cfgtag}-M-${mass}.txt
done

