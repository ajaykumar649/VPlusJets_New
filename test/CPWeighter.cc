//check out and build the needed code with the Fortran library
//  cd $CMSSW_BASE/src
//  cvs co -d MMozer/powhegweight UserCode/MMozer/powhegweight
//  cd MMozer/powhegweight
//  scram b
//
//in ROOT before loading this file you must issue
//  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libMMozerpowhegweight.so")
//then you can
//  .L CPWeighter.cc+
//
// mH : mass of Higgs (input mass to the generator)
// wH : width of Higgs (input width to the generator)
// m : generated mass of a particular Higgs event
// BWflag : generator parameter 0 for fixed width, 1 for running width
// 172.5 : top mass

#include "MMozer/powhegweight/interface/pwhg_wrapper.h"

double getCPweight(double mH, double wH, double m, int BWflag = 1) {
  static pwhegwrapper phw;
  return phw.getweight(mH, wH, 172.5, m, BWflag);
}
