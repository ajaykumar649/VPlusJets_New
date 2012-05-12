#!/bin/bash
#
#
SRCDIR=/uscms/home/pdudero/work/CMSSW_4_2_8/src/ElectroWeakAnalysis/VPlusJets/Limits/ctool
#BASEDIR=/uscms/home/pdudero/data/HWWmvaScan120224
#BASEDIR=/uscms/home/pdudero/data/HWWqgldScan120306
BASEDIR=/uscms/home/pdudero/data/HWWqgldScan120312

cutvalue=$1

makeWrapper() {
cat >hwwshapesWrapper.C <<EOF
void hwwshapesWrapper(const TString& arg1,
		      const TString& arg2)
{
  gROOT->ProcessLine(".L hwwshapesCMScombine.C+");
  hwwshapes(arg1,arg2);
}
EOF
}


############################################################
runCombine1scanPt() {
############################################################

# set up the work area
TGTDIR=$BASEDIR/cutvalue=$cutvalue
# mkdir $TGTDIR

cd $SRCDIR
eval `scramv1 runtime -sh`

#########################################
echo "Making shapes"
#########################################

makeWrapper

#nametag="mvacutvalue=$cutvalue"
nametag="qgldcutvalue=$cutvalue"

root -n -b -q "hwwshapesWrapper.C(\"$nametag\",\"$TGTDIR\")"

if stat -t  $TGTDIR/hww-histo-shapes-${nametag}[-_]M=??0.root >/dev/null 2>&1
then
#########################################
    echo "Making datacards"
#########################################
#    for rootf in ${TGTDIR}/hww-histo-shapes-${nametag}[-_]M=??0.root
    for rootf in ${TGTDIR}/hww-histo-shapes-${nametag}[-_]M=500.root
    do
#      for channel in `egrep -o "[a-zA-Z23]+jetCMS" ${SRCDIR}/hwwinputs.h`
      for channel in mu3jetCMS
      do
	./makeDataCardShapes.exe $rootf $channel
      done
    done
else
    echo "Couldn't find histo shapes root files"
fi

#if stat -t ${TGTDIR}/datacard*${nametag}[-_]M=??0.txt >/dev/null 2>&1
if stat -t ${TGTDIR}/datacard*${nametag}[-_]M=500.txt >/dev/null 2>&1
then
#########################################
    echo "Running combine"
#########################################
    ./combine_asymp.sh ${TGTDIR}/datacard*${nametag}[-_]M=500.txt
else
    echo "Couldn't find datacards"
fi
}

################################################################################
#   S T A R T    H E R E       S T A R T    H E R E       
################################################################################

for cutvalue in `ls -d $BASEDIR/cutvalue=0.* | egrep -o "cutvalue=[0-9.]+" | egrep -o "[0-9.]+"`
do
  runCombine1scanPt > $BASEDIR/runcombine_cutvalue=${cutvalue}_err.log 2>&1 
done

#for masspt in 170 180 190 200 250 300 350 400 450 500 550 600
for masspt in 500
do
#  for channel in `egrep -o "[a-zA-Z23]+jetCMS" ${SRCDIR}/hwwinputs.h`
  for channel in mu3jetCMS
  do
    outputfile="explimvscutval-M=${masspt}_${channel}.tab"
    echo "#Cutval	explim" >$outputfile
    for cutvalue in `ls -d $BASEDIR/cutvalue=0.* | egrep -o "cutvalue=[0-9.]+" | egrep -o "[0-9.]+"`
    do
      LOGFILE=$BASEDIR/cutvalue=${cutvalue}/limit*${channel}*${masspt}*log
      explim=""
      if stat -t $LOGFILE >/dev/null 2>&1
      then
	  explim=`grep -h "Expected 50" $LOGFILE | egrep -o "r < [0-9.]+" | egrep -o "[0-9.]+"`
      fi
      if [ "$explim" = "" ]
      then
	  explim=-1
      fi
      echo "$cutvalue	$explim" >>$outputfile
    done
  done
done
