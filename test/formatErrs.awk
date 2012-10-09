# BEGIN {
#     cnt = 0;
#     print "    ","  electron","   "," muon";
#     print "    ","2-jet","3-jet","2-jet","3-jet";
# }
{
    printf "%s %s %s ", $1, $2, $3;
    # cnt++;
    tmpVal = $NF + 1;
    printf "%0.3f ", tmpVal;
    print " ";
}
