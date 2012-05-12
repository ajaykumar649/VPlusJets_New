#ifndef _MJJCHANNELS_H
#define _MJJCHANNELS_H

// never uncomment this line!
//#define ISHWW // now you've done it!

#define NUMMASSPTS 1
const int masspts[NUMMASSPTS] = { 150 };

#define NUMCHAN 4

const char *channames[NUMCHAN] = {
  "el2jet",
  "el3jet",
  "mu2jet",
  "mu3jet"
};

const TString AnalStr       = "Mjj";

const double mutrigeff      = 1.;
const double eltrigeff      = 1.;
const double sigtrigeffunc  = 0.01;
const double siglepteffunc  = 0.02;
//const double siglumiunc     = 0.045;
const double siglumiunc     = 0.022;

// numbers taken from WH/ZH eff x acc:

// ordered the same as the channels: updated KM: Dec 27, 2011
//
const double gseffxacc[NUMCHAN] = {
  0.03803   * eltrigeff,          
  0.01316   * eltrigeff,          
  0.05985   * mutrigeff,          
  0.01916   * mutrigeff           
};

const double whscale = 100;
const double whkfactor = 1.3;

//const double intlumipbinv = 1200.0;
//const double intlumipbinv = 2100.0;

// inputs are normalized to 1/fb
//const double sigscaleto   = 4730.0/1000.0;
const double sigscaleto   = 5020.0/1000.0;
const double whsigscaleto = whkfactor*whscale*sigscaleto; // for WH signal


// FILE/HISTO STRUCTURE: assumed same name objects contained in different files for the different channels

const char *dir = 
  "/uscms_data/d3/ilyao/KinematicFitterS11/5p02fbFit/FitResults/";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/Mjjplots/";

const char *sigdir = 
  //"/uscms_data/d3/ilyao/KinematicFitterS11/4p7fbFit/FitResults/";
  "/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/Mjjplots/";

// ordered the same as the channels
//
const char *siginputfiles[NUMCHAN] = {
  "Mjj_Electron_2Jets_NewPhysics_1fb.root",
  "Mjj_Electron_3Jets_NewPhysics_1fb.root",
  "Mjj_Muon_2Jets_NewPhysics_1fb.root",
  "Mjj_Muon_3Jets_NewPhysics_1fb.root"
};

// ordered the same as the channels
//
const char *backdatainputfiles[NUMCHAN] = {
  "Mjj_Electron_2Jets_Fit_Shapes.root",
  "Mjj_Electron_3Jets_Fit_Shapes.root",
  "Mjj_Muon_2Jets_Fit_Shapes.root",
  "Mjj_Muon_3Jets_Fit_Shapes.root"
};

#define NUMSIG 4
const char *sigobjnames[NUMSIG][2] = {
  { "zp", "Zprime_combined" },
  { "tc", "Technicolor_combined" },
  { "wh", "WH_combined" },
  { "gs", "gaus_combined" }
};

const char *dataobjname = "theData";
const char *bkgdobjname = "h_total";

double backnormerr[NUMMASSPTS*NUMCHAN] = {
  1.00612, 1.01249, 1.00489, 1.01182
  //1.00612, 1.01244, 1.00489, 1.01244
  //1.00612, 1.01249, 1.00489, 1.01178
  //1.0061, 1.0124, 1.0048, 1.0098
  //1.0079, 1.0150, 1.0064, 1.0123
};

#if 0
const double sumwinmin = 150-15*2;
const double sumwinmax = 150+15*2;
#elif 0
const double sumwinmin = 140;
const double sumwinmax = 170;
#else
const double sumwinmin = 60;
const double sumwinmax = 300;
#endif

#endif // _MJJCHANNELS_H
