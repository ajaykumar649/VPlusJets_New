#!/bin/bash

#PREFIX=hww-histo-shapes
PREFIX=hwwlvjj.input_8TeV

#echo "Making shapes"
#root -n -b -q "hwwshapesWrapper.C(\"$1\",\"$2\")"
if [ $# -lt 1 ]
then
    echo "Usage: $0 config-tag [dir-containing-rootfiles]"
    exit -1
fi
if [ $# -lt 2 ]
then
    DIR=.
else
    DIR=$2
fi

for flavor in el mu
do
    echo "Making cards for flavor $flavor"
    for rootf in ${DIR}/${PREFIX}-${1}[-_]M=???.root
    do
#      ./makeDataCardShapes.exe $rootf ${flavor}2jetCMS ${flavor}3jetCMS
      ./makeDataCardShapes.exe $rootf hww${flavor}nu2j hww${flavor}nu3j
    done
    movem "s#${1}#${flavor}_${1}#g" datacard_8TeV-${1}*.txt
done
