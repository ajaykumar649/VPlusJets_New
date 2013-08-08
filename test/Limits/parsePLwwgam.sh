#!/bin/bash

DIR=`dirname $1`
file=`basename $1 .log`

printf "var\tobs\texp95lo\texp68lo\texpLim\texp68hi\texp95hi\n" | tee ${DIR}/limits_wwgamaqgc.tab

for i in $*; do
    file=`basename $i .log`
#    suffix=${file##limit_}
    obsfile=`echo $i | sed 's#exp#obs#g'`
    parval=`echo $file | egrep -o ":[0-9-]+" | egrep -o "[0-9-]+"`
    obs=`grep "r <" $obsfile | egrep -o "< [0-9\.-]+ \@" | egrep -o "[0-9\.-]+"`
    median=`grep median $i | egrep -o "< [0-9\.-]+ \@" | egrep -o "[0-9\.-]+"`
    exp68lo=`grep "68% expected band" $i | egrep -o "[0-9\.-]+ < r" | egrep -o "[0-9\.-]+"`
    exp68hi=`grep "68% expected band" $i | egrep -o "r < [0-9\.-]+" | egrep -o "[0-9\.-]+"`
    exp95lo=`grep "95% expected band" $i | egrep -o "[0-9\.-]+ < r" | egrep -o "[0-9\.-]+"`
    exp95hi=`grep "95% expected band" $i | egrep -o "r < [0-9\.-]+" | egrep -o "[0-9\.-]+"`
    printf "%3d\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n" $parval $obs $exp95lo $exp68lo $median $exp68hi $exp95hi
done 2>&1 | sort -n | tee -a ${DIR}/limits_wwgamaqgc.tab
