#!/bin/bash

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
if stat -t  $DIR/hww-histo-shapes-${1}[-_]M=??[05].root >/dev/null 2>&1
then
    echo "Making cards"
    for rootf in ${DIR}/hww-histo-shapes-${1}[-_]M=??[05].root
    do
      ./makeDataCardShapes.exe $rootf
    done
fi
fi
#
# add any argument beyond 3 to inhibit datacard making and running combine...
if [ $# -lt 4 ] 
then
if stat -t ${DIR}/datacard_${1}[-_]M=??[05].txt >/dev/null 2>&1
then
    echo "Running combine"
    ./combine_asymp.sh ${DIR}/datacard_${1}[-_]M=??[05].txt
fi
fi
#
# add any argument beyond 4 to inhibit datacard making, combine, and parsing
# (just plot)
if [ $# -lt 5 ] 
then
if stat -t  ${DIR}/limit_${1}[-_]M=??[05].log >/dev/null 2>&1
then
    echo "Parsing output"
    ./parseAsym.sh ${DIR}/limit_${1}[-_]M=??[05].log
fi
fi
#
if [ -e ${DIR}/limit_$1.tab ]
then
    echo "plotting limit"
    root -n -b -q "plotLimit.C(\"${DIR}/limit_$1.tab\")"
fi
