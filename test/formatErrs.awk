BEGIN {
    cnt = 0;
    print "    ","  electron","   "," muon";
    print "    ","2-jet","3-jet","2-jet","3-jet";
}
{
    if (cnt == 0) printf "%s ", $1;
    cnt++;
    tmpVal = $NF + 1;
    printf "%0.3f ", tmpVal;
    if (cnt == 4) {
	print " ";
	cnt = 0;
    }
    # printf "%s %s %s ", $1, $2, $3;
    # tmpVal = $NF + 1;
    # printf "%0.3f ", tmpVal;
    # print " ";
}
