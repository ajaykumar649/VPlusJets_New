#!/bin/bash
if [ $# -lt 1 ]
then
    echo "dir=."
    DIR=.
else
    echo "dir=$1"
    DIR=$1
fi

for sig in zp wh tc gs cdf
#for sig in cdf
do
  ./makeDataCardShapes.exe ${DIR}/mjj-histo-shapes-${sig}.root
#  cat mjjsystall.txt >>datacard_${sig}.txt
done
