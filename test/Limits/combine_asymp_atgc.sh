#!/bin/sh

COMBINE_ARGS="-v0 -M Asymptotic --minosAlgo stepping"

for datacard in $*
do
  DIR=`dirname $datacard`
  file=`basename $datacard .txt`
  suffix=${file##datacard_}
  echo "combine $COMBINE_ARGS -n _atgc_$suffix $datacard > ${DIR}/limit_${suffix}.log 2>&1"
  combine $COMBINE_ARGS -n _atgc_$suffix $datacard > ${DIR}/limit_${suffix}.log 2>&1
done
