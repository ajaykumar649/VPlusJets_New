#ifndef _ATGCINPUTS_H
#define _ATGCINPUTS_H

// never uncomment this line!
//#define ISHWW // now you've done it!

#define NUMMASSPTS 1
const int masspts[NUMMASSPTS] = { 150 };

#define NUMCHAN 4

const char *channames[NUMCHAN] = {
  "elbtag",
  "elnobtag",
  "mubtag",
  "munobtag",
};

const TString AnalStr       = "ATGC";

const double mutrigeff      = 1.;
const double eltrigeff      = 1.;
const double sigtrigeffunc  = 0.01;
const double siglepteffunc  = 0.02;
const double siglumiunc     = 0.022;
const double signal_xs_unc  = 0.034;

// for cut-and-count limits:
const double dijetptmingev  = 350.;

const float LAMBDAZ_MIN = -0.15;
const float LAMBDAZ_MAX =  0.15;
const float LAMBDAZ_INC = 0.005;

const float dKG_MIN = -0.30;
const float dKG_MAX =  0.30;
const float dKG_INC = 0.01;

// FILE/HISTO STRUCTURE: assumed same name objects contained in different files for the different inputs

//const char *dir = "/uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_2_8/src/ElectroWeakAnalysis/VPlusJets/test/TGC/";
const char *dir = ".";

// ordered the same as the inputs
//
const char *inputfiles[NUMCHAN] = {
  "el_btag.root",
  "el_noBtag.root",
  "mu_btag.root",
  "mu_noBtag.root"
};

const char *signalfmtstr = "lambdaZ_%.3f_deltaKappaGamma_%.3f";

const char *dataobjname = "data";
const char *bkgdobjname = "background";

#endif // _ATGCINPUTS_H
