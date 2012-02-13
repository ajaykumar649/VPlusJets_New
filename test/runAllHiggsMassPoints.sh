#!/bin/bash
for i in 170 180 190 200 250 300 350 400 450 500 550 600
do
   #echo "Now running Higgs mass point $i muons 2 jet"
   #python runHWWFitter.py -m HWWMuonsConfig -j 2 -H $i
   #echo "Now running Higgs mass point $i muons 3 jet"  
   #python runHWWFitter.py -m HWWMuonsConfig -j 3 -H $i
   echo "Now running Higgs mass point $i electrons 2 jet"
   python runHWWFitter.py -m HWWElectronsConfig -j 2 -H $i
   #echo "Now running Higgs mass point $i electrons 3 jet"  
   #python runHWWFitter.py -m HWWElectronsConfig -j 3 -H $i
done
