#!/bin/sh

for datacard in $*
do
#  DIR=`dirname $datacard`
  DIR=.
  COMBINE_ARGS="-v0 -M ProfileLikelihood"

  file=`basename $datacard .txt`
  suffix=${file##datacard_}

  COMBINE_ARGS="$COMBINE_ARGS -n _obs_$suffix"
#  COMBINE_ARGS="$COMBINE_ARGS --bruteForce -n _obs_$suffix"

  echo "combine $COMBINE_ARGS $datacard 2>&1 | tee ${DIR}/limit_plobs_${suffix}.log >/dev/null"
  combine $COMBINE_ARGS $datacard 2>&1 | tee ${DIR}/limit_plobs_${suffix}.log >/dev/null
done
