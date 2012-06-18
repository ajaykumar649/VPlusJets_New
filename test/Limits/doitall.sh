#!/bin/bash

#PREFIX=hww-histo-shapes
PREFIX=hwwlvjj.input_8TeV

if [ $# -lt 1 ]
then
    echo "Usage: $0 nametag [workingdir] [inhibit datacard-making] [inhibit combine] [inhibit parsing, just plot]"
    exit
fi

#echo "Making shapes"
#root -n -b -q "hwwshapesWrapper.C(\"$1\",\"$2\")"
if [ $# -lt 2 ]
then
    DIR=.
else
    DIR=$2
fi

# add any argument beyond 2 to inhibit datacard making
if [ $# -lt 3 ] 
then
if stat -t  $DIR/${PREFIX}-${1}[-_]M=???.root >/dev/null 2>&1
then
    echo "Making cards"
    for rootf in ${DIR}/${PREFIX}-${1}[-_]M=???.root
    do
      ./makeDataCardShapes.exe $rootf
    done
fi
fi
#
# add any argument beyond 3 to inhibit datacard making and running combine...
if [ $# -lt 4 ] 
then
if stat -t ${DIR}/datacard_8TeV-${1}[-_]M=???.txt >/dev/null 2>&1
then
    echo "Running combine"
    ./combine_asymp.sh ${DIR}/datacard_8TeV-${1}[-_]M=???.txt
fi
fi
#
# add any argument beyond 4 to inhibit datacard making, combine, and parsing
# (just plot)
if [ $# -lt 5 ] 
then
if stat -t  ${DIR}/limit_8TeV-${1}[-_]M=???.log >/dev/null 2>&1
then
    echo "Parsing output"
    ./parseAsym.sh ${DIR}/limit_8TeV-${1}[-_]M=???.log
fi
fi
#
if [ -e ${DIR}/limit_8TeV-$1.tab ]
then
    echo "plotting limit"
    root -n -b -q "plotLimit.C(\"${DIR}/limit_8TeV-$1.tab\")"
fi
