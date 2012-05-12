#!/bin/bash
executable=${PWD}/mjj_makeTable_allfiles.sh

if [ $# -ne 2 ]
then
    echo "Usage: $0 start*10 stop*10"
    exit
fi
outputdir=~/data/Mjj

submit=submit${1}to${2}.txt

printheader() {
cat >$submit <<EOF
Executable = $executable
Universe = vanilla
Requirements = Memory > 250 && FileSystemDomain=="fnal.gov" && Disk > 500000 && Arch=="X86_64"
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
EOF
}

printargs() {
	echo "Error = $outputdir/mjjtable_r=$1,do=$2.err" >>$submit
	echo "Arguments = $1 $2" >>$submit
	echo "Queue" >>$submit
}

printheader

for (( i=$1; i<=$2; i++ ))
do
  r=`echo "scale=1; $i/10" | bc; exit`
  for (( j=0; j<=5; j++ ))
  do
    printargs $r $j
  done
done
