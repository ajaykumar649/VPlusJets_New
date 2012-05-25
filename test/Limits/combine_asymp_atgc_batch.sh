#!/bin/sh

cd /uscms/home/pdudero/Limits/
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scram runtime -sh`
cd -

#cp -p $* .
cp -p /uscms/home/pdudero/Limits/new*tag.root .
cp -p /uscms/home/pdudero/Limits/combine_asymp_atgc.sh .

./combine_asymp_atgc.sh $*
