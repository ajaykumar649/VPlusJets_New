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
for channel in hwwelnu2j hwwelnu3j hwwmunu2j hwwmunu3j
do
    echo "Making card for channel $channel"
    for rootf in ${DIR}/hww-histo-shapes-${1}[-_]M=??0.root
    do
      ./makeDataCardShapes.exe $rootf $channel
    done
    movem "s#${1}#${channel}_${1}#g" datacard_${1}*.txt
done
