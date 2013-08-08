#!/bin/sh

#HYPOTESTTREEDIR="${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/test/plotting"
HTTDIR="."

# inputfile could be datacard text files or root workspaces.
for inputfile in $*
do
#  DIR=`dirname $inputfile`
  DIR=.
  COMBINE_ARGS="-M HybridNew --testStat=TEV --frequentist --singlePoint 1 --saveHybridResult --fork 4 -T 50000 -i 1 --clsAcc 0 --fullBToys"

#  file=`basename $inputfile .txt`
  file=`basename $inputfile .root`
  suffix=${file#[a-zA_Z0-9]*_}

  COMBINE_ARGS="$COMBINE_ARGS -n _atgc_$suffix"

  echo "combine $COMBINE_ARGS $inputfile 2>&1 | tee ${DIR}/limit_${suffix}.log >/dev/null"
  combine $COMBINE_ARGS $inputfile 2>&1 | tee ${DIR}/limit_${suffix}.log >/dev/null

#  root -n -q -b higgs*_${suffix}*.root "${HTTDIR}/hypoTestResultTree.cxx(\"qtree_atgc_${suffix}.root\",120)"
  root -n -q -b higgs*_${suffix}*.root "${HTTDIR}/hypoTestResultTree.cxx(\"qtree_atgc_${suffix}.root\",120,1,\"x\")"
done
#
root -n -q -b "extractATGClimStatsWrapper.C(\"qtree*.root\")"
