#!/bin/sh
#
NPTS=51
NJOBS=500
NTOYSDIV500=1000
NITER=5
START=0
STOP=5
#STOP=10
#
DCARD=datacard_gs.txt
#
OPTS="-r -n $NPTS -t $NTOYSDIV500 -j $NJOBS"
#OPTS2PASS="--rule=CLsplusb --freq"
#OPTS2PASS="--rule=CLsplusb"
#OPTS2PASS="--rule=CLs"
OPTS2PASS="--frequentist"
#OPTS2PASS="--rule CLs --testStat LHC"
#
#    toys-per-point:  T * t / I
#    toys-per-job:    T * (t/j) * (n/I)
#
# -T ("toys/iteration") is 500 by default
# -I is 1 by default (all points in every job)
#
# NOTE: crab wants $NTOYSDIV500 >= $NJOBS
#
for (( iter=0; iter<$NITER; iter++ ))
do
  echo "Running model=gs with options $OPTS -O $OPTS2PASS"
  python ./makeGridUsingCrab.py $DCARD $START $STOP $OPTS -O "$OPTS2PASS" -o  crab_limit_mjj_gs_${START}to${STOP}_iter${iter}
done
