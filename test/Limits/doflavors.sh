#!/bin/bash

#echo "Making shapes"
#root -n -b -q "hwwshapesWrapper.C(\"$1\",\"$2\")"
if [ $# -lt 2 ]
then
    DIR=.
else
    DIR=$2
fi

#for channel in el2jetCMS el3jetCMS mu2jetCMS mu3jetCMS
#for channel in el2jetCMS
for flavor in el mu
#for channel in hwwelnu2jCMS hwwelnu3jCMS hwwmunu2jCMS hwwmunu3jCMS
do
    echo "Making cards for flavor $flavor"
    for rootf in ${DIR}/hww-histo-shapes-${1}[-_]M=??0.root
    do
      ./makeDataCardShapes.exe $rootf ${flavor}2jetCMS ${flavor}3jetCMS
    done
    movem "s#${1}#${flavor}_${1}#g" datacard_${1}*.txt
done
