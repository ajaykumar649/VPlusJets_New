#!/bin/bash
cfgtag=jakeHWWFit24July2013
DIR=/uscms_data/d2/andersj/Wjj/2012/CMSSW_5_3_3_patch2/src/ElectroWeakAnalysis/VPlusJets/test/HWWFit24July2013

for mass in 170 180 190 200 250 300 350 400 450 500 550 600
do
  for flavor in electron muon
  do
    ./makeDataCardFromRooWorkspace.exe ${DIR}/HWW${mass}lnujj_${flavor}_2jets_1D2Fit_output.root $cfgtag
  done
done

