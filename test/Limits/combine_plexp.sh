#!/bin/sh

for datacard in $*
do
#  DIR=`dirname $datacard`
  DIR=.
  # v2 since for more than 496 toys the standard output craps out with vN, N<2 !
  COMBINE_ARGS="-v2 -M ProfileLikelihood"

  file=`basename $datacard .txt`
  suffix=${file##datacard_}

  COMBINE_ARGS="$COMBINE_ARGS -n _exp_$suffix -t 1000 -s -1"

  echo "combine $COMBINE_ARGS $datacard 2>&1 | tee ${DIR}/limit_plexp_${suffix}.log >/dev/null"
  combine $COMBINE_ARGS $datacard 2>&1 | tee -a ${DIR}/limit_plexp_${suffix}.log >/dev/null
done
