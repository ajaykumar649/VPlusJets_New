#!/bin/bash
SRCDIR=/uscms/home/pdudero/work/CMSSW_6_1_1/src/ElectroWeakAnalysis/VPlusJets/test/Limits
HYPOTESTTREEDIR="${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit/test/plotting"

executable=${SRCDIR}/batchit.sh
#cmbscript1=${SRCDIR}/combine_plobs_atgc.sh
#cmbscript2=${SRCDIR}/combine_plexp_atgc.sh
#cmbscript1=${SRCDIR}/combine_asymp_atgc.sh
#cmbscript1=${SRCDIR}/combine_fc_atgc.sh
cmbscript1=${SRCDIR}/combine_hypotest_atgc.sh
#cmbscript1=${SRCDIR}/extract.sh
#cmbscript1=${SRCDIR}/atgctxt2wkspace.sh

FILESUFFIX=".root"
#FILESUFFIX=".txt"

#ADDTLFILES=",$cmbscript1,el_boosted.root,mu_boosted.root"
ADDTLFILES=",$cmbscript1,${HYPOTESTTREEDIR}/hypoTestResultTree.cxx,${SRCDIR}/extractATGClimStatsWrapper.C,${SRCDIR}/extractATGClimStats.C"
#ADDTLFILES=",$cmbscript1,${SRCDIR}/extractATGClimStatsWrapper.C,${SRCDIR}/extractATGClimStats.C"

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
      echo "Error = atgclimit_${cfgtag}.err" >>$submit
      echo "Output = atgclimit_${cfgtag}.out" >>$submit
      echo "Arguments = $arglist" >>$submit
      echo "Queue" >>$submit
}

printheader

# lzlist=`ls $* | grep lz | egrep -o "lz_[0-9\.\-]+" | sed 's#lz_##g'|sort -u`
# dkglist=`ls $* | grep dkg | egrep -o "dkg_[0-9\.\-]+" | sed 's#dkg_##g'|sort -u`

# if [[ "$lzlist" != "" ]]
# then
#     echo "lzlist =" $lzlist
#     for lz in $lzlist
#     do
#       filelist4lz=`ls $* | grep "lz_${lz}" | xargs`
#       xferlist=`echo $filelist4lz | sed 's# #,#g'`
#       xferlist="$xferlist${ADDTLFILES}"
#       scriptbase=`basename $cmbscript1`
#       arglist="./$scriptbase $filelist4lz"
#       cfgtag="lz${lz}"
#       queuecards 

#       if [[ "$cmbscript2" != "" ]]
#       then
# 	  if [[ `echo "scale=4; $lz <= 0.003 && $lz >= -0.004" | bc` -ne 0 ]]
# 	  then
#               # for these, allocate one file per job
# 	      filelist=`ls $* | grep "_\$lz"`
# 	      for file in $filelist
#               #for file in $*
# 	      do
# 		base=`basename $file ${FILESUFFIX}`
# 		cfgtag=${base##datacard_}
# 		cfgtag="${cfgtag}_exp"
# 		xferlist="$file${ADDTLFILES}"
# 		arglist="$cmbscript2 $file"

# 		queuecards
# 	      done
# 	  else
# 	      $arglist="$cmbscript2 $cardlist"
# 	      queuecards
# 	  fi
#       fi
#     done

# elif [[ "$dkglist" != "" ]]
# then
#     echo "dkglist =" $dkglist

#     for dkg in $dkglist
#     do
#       filelist4dkg=`ls $* | grep "dkg_${dkg}" | xargs`
#       xferlist=`echo $filelist4dkg|sed 's# #,#g'`
#       xferlist="$xferlist${ADDTLFILES}"
#       scriptbase=`basename $cmbscript1`
#       arglist="./$scriptbase $filelist4lz"
#       cfgtag="dkg${dkg}"

#       queuecards 
      
#       if [[ "$cmbscript2" != "" ]]
#       then
# 	  arglist="$cmbscript2 $cardlist"
# 	  queuecards
#       fi
#     done
# else
    for file in $*
    do
      xferlist="$file${ADDTLFILES}"
      scriptbase=`basename $cmbscript1`
      arglist="./$scriptbase $file"
      base=`basename $file ${FILESUFFIX}`
      cfgtag=${base##wkspace_}
      queuecards
    done
# fi


# for dc in $*
# do
#   file=`basename $dc .txt`
#   cfgtag=${file##datacard_}
#   echo "transfer_input_files = $dc,el_dijet.root,el_boosted.root,mu_dijet.root,mu_boosted.root,${cmbscript}" >>$submit
# #  echo "transfer_input_files = $dc,el_noBtag.root,mu_noBtag.root,${cmbscript}" >>$submit
#   echo "Error  = limit_${cfgtag}.log" >>$submit
#   echo "Output = limit_${cfgtag}.log" >>$submit
#   echo "Arguments = $dc" >>$submit
#   echo "Queue" >>$submit
# done
