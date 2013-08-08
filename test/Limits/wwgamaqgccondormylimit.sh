#!/bin/bash
SRCDIR=/uscms/home/pdudero/work/CMSSW_6_1_1/src/ElectroWeakAnalysis/VPlusJets/test/Limits

executable=${SRCDIR}/batchit.sh
cmbscript1=${SRCDIR}/combine_plobs.sh
cmbscript2=${SRCDIR}/combine_plexp.sh

FILESUFFIX=".txt"

ADDTLFILES=",$cmbscript1,$cmbscript2"

if [ $# -lt 1 ]
then
    echo "Usage: $0 <fileglob> ( e.g., datacard*.txt, qtree*.root, etc.)"
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

queuecards() {
      echo "transfer_input_files = $xferlist" >>$submit
      echo "Error = aqgclimit_${cfgtag}.err" >>$submit
      echo "Output = aqgclimit_${cfgtag}.out" >>$submit
      echo "Arguments = $arglist" >>$submit
      echo "Queue" >>$submit
}

printheader

for file in $*
do
  ROOTFILES=`grep -o "wwgaminputdir.*root" $file | sort -u |xargs|sed 's# #,#g'`
  ADDTLFILES=",$cmbscript1,$ROOTFILES"
  xferlist="$file${ADDTLFILES}"
  scriptbase=`basename $cmbscript1`
  arglist="ln -s . wwgaminputdir; ./$scriptbase $file"
  base=`basename $file ${FILESUFFIX}`
  cfgtag=${base##datacard_}
  queuecards
  ADDTLFILES=",$cmbscript2,$ROOTFILES"
  xferlist="$file${ADDTLFILES}"
  scriptbase=`basename $cmbscript2`
  arglist=./$scriptbase $file
  queuecards
done
