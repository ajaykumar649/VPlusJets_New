#!/bin/bash
for dcfile in $*
do
    echo $dcfile
    base=`basename $dcfile .txt`
    suffix=${base##datacard_}
    # picks the "fixed mu" method by default
    text2workspace.py $dcfile -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs -o wkspace_${suffix}.root
done
