#!/bin/bash

echo "hhmass   obsLimit        exp95lo         exp68lo         expLimit        exp68hi         exp95hi"

for i in $*; do
    dir=`dirname $i`
    file=`basename $i`
    dc=${file%%.log}
    dc=${dc##limit_}
    line=`grep "r <" $i | sed 's#.*r < ##g' | transpose.pl`
    echo "$dc	$line"
done
