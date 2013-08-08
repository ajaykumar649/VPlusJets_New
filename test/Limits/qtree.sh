#!/bin/bash

for file in $*
do
  base=`basename $file .root`
  suffix=${base##higgsCombine_}
  root -n -q -b $file "${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/test/plotting/hypoTestResultTree.cxx(\"qtree_${suffix}.root\",120)"
done

