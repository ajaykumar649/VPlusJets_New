#!/bin/bash

cfgtag=jakeHWWFit24July2013

for mass in 170 180 190 200 250 300 350 400 450 500 550 600
#for mass in 200 250 300 350 400 450 500 550 600
#for mass in 170 180 190
#for mass in 200 500
do
  ./movem 's#M=#M-#g' datacard_8TeV_hww??nu2j_${cfgtag}-M=${mass}.txt
  hwwelnu2j=datacard_8TeV_hwwelnu2j_${cfgtag}-M-${mass}.txt
  hwwmunu2j=datacard_8TeV_hwwmunu2j_${cfgtag}-M-${mass}.txt
  echo "combineCards.py hwwmunu2j=${hwwmunu2j} hwwelnu2j=${hwwelnu2j} >datacard_8TeV_hww2chan_${cfgtag}-M=${mass}.txt"
  combineCards.py hwwmunu2j=${hwwmunu2j} hwwelnu2j=${hwwelnu2j} >datacard_8TeV_hww2chan_${cfgtag}-M=${mass}.txt
  ./movem 's#M-#M=#g' datacard_8TeV_hww??nu2j_${cfgtag}-M-${mass}.txt
done
