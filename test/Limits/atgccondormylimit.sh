#!/bin/bash
executable=${PWD}/combine_asymp_atgc_batch.sh

if [ $# -eq 1 ]
then
    echo "Usage: $0 datacard*lz*.txt"
    exit
fi

submit=submit.txt

printheader() {
cat >$submit <<EOF
Executable = $executable
Universe = vanilla
Requirements = Memory > 250 && FileSystemDomain=="fnal.gov" && Disk > 500000 && Arch=="X86_64"
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
EOF
}

printheader

lzlist=`ls $* | egrep -o "lz[0-9\.\-]+" | sed 's#lz##g'|sort -u`
for lz in $lzlist
do
  arglist=`ls ${PWD}/datacard*_lz${lz}*.txt|xargs`
  echo "Error = atgclimit_lz${lz}.err" >>$submit
  echo "Arguments = $arglist" >>$submit
  echo "Queue" >>$submit
done
