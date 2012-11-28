#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: $0 <datacards>"
    exit
fi

TGTDIR=/uscms_data/d2/pdudero/SVN/hcg/highmass-ichep2012/hwwlvqq
#TGTDIR=/uscms_data/d2/pdudero/SVN/hcg/hcp2012/hwwlvqq

BEAMENERGYTEV=8

for datacard in $*
do
  DIR=`dirname $datacard`
  file=`basename $datacard .txt`
  suffix=${file##datacard_}
  mass=`ls $datacard | egrep -o "M=[0-9]+" | egrep -o "[0-9]+"`
#
  echo "sed 's#TeV-.*\.root#TeV.root#g' $datacard >$TGTDIR/$mass/hwwlvjj_shape_${BEAMENERGYTEV}TeV.txt"
  echo "mv hwwlvjj.input_$suffix.root $TGTDIR/$mass/hwwlvjj.input_${BEAMENERGYTEV}TeV.root"
#
  sed 's#TeV-.*\.root#TeV.root#g' $datacard >$TGTDIR/$mass/hwwlvjj_shape_${BEAMENERGYTEV}TeV.txt
  cp -p ${DIR}/hwwlvjj.input_$suffix.root $TGTDIR/$mass/hwwlvjj.input_${BEAMENERGYTEV}TeV.root
done
