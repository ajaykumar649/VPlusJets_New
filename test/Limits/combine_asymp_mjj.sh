#!/bin/sh

METHOD="Asymptotic"

COMBINE_ARGS="-M ${METHOD} -n MjjObs -v0 --minosAlgo stepping"

for datacard in $*
do
  echo $datacard
  dir=`dirname $datacard`
  file=`basename $datacard`
  dc=${file%%.txt}
  dc=${dc##datacard_}
  combine $COMBINE_ARGS $datacard 2>&1 | tee $dir/limit_${dc}.log >/dev/null
done
