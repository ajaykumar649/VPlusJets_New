#ifndef _HWWCHANNELS_H
#define _HWWCHANNELS_H

#define ISHWW

//#define SEVENTEV
#undef SEVENTEV

//#define DO_INTERP
#undef DO_INTERP

//================================================================================
// High-level variables

#ifdef SEVENTEV
const bool BLINDING = false;
const double intlumipbinv = 5020.0;
const int beamcomenergytev = 7;

// ordered the same as the channels
//
const char *dir = "/uscms_data/d2/pdudero/HWW/ICHEP2012/7tevInterp";

#else

//const bool BLINDING = true;
const bool BLINDING = false;

const double intlumipbinv = 3500.0;
const int beamcomenergytev = 8;
const char *dir =
  //"/uscms_data/d2/andersj/Wjj/2012/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW1p6Result";
  "/uscms_data/d2/andersj/Wjj/2012/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW3p5ResultUnblind";
  //"/uscms_data/d2/andersj/Wjj/2012/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW3p5tightMuons";
  //"/uscms_data/d2/pdudero/HWW/ICHEP2012/8tevInterp";

const double global_scale = 1.0;
//const double global_scale = 5000.0/intlumipbinv;
//const double scalefrom7to8tev = 1.3;

const double scaleBRforTau = 1.5;

#endif

//================================================================================

#define NUMCHAN 4

// must be put in lexicographical order
//
#if 1
// for combination:
const char *channames[NUMCHAN] = {
  "hwwelnu2j"
  ,"hwwelnu3j"
  ,"hwwmunu2j"
  ,"hwwmunu3j"
};
#define ELORMUCHAR 3
#else
const char *channames[NUMCHAN] = {
  "el2jetCMS"
  ,"el3jetCMS"
  ,"mu2jetCMS"
  ,"mu3jetCMS"
};
#define ELORMUCHAR 0
#endif

const char *channames2[NUMCHAN] = {
  "Electron"
  ,"Electron"
  ,"Muon"
  ,"Muon"
};

const double mutrigeff      = 1.0;
const double eltrigeff      = 1.0;
const double sigtrigeffunc  = 0.01;
const double siglepteffunc  = 0.02;
const double siglumiunc     = 0.022;

const double W2qqBR    = 0.6770;
const double W2taunuBR = 0.1125;


//--------------------------------------------------

const int NUMMASSPTS=8;
const int masspts[NUMMASSPTS] = {
  //170, 180, 190, 200, 250, 300, 350, 400, 450, 500, 550, 600
  180, 200, 300, 400, 450, 500, 550, 600
};

//--------------------------------------------------

const int interpolatedmasspts[] = {
  172,174,176,178,    // 170-180
  182,184,186,188,    // 180-190
  192,194,196,198,    // 190-200
  202,204,206,208,210,
  212,214,216,218,220,
  222,224,226,228,230,
  232,234,236,238,240,
  242,244,246,248,    // 200-250
  252,254,256,258,260,
  262,264,266,268,270,
  272,274,276,278,280,
  282,284,286,288,290,
  295,                // 250-300
  305,310,315,320,325,
  330,335,340,345,    // 300-350
  360,370,380,390,
  420,440,            // 400-450
  460,480,            // 450-500
  520,540,            // 500-550
  560,580,            // 550-600
  -1
};

//--------------------------------------------------

// inputs are normalized to 1/nent

// FILE/HISTO STRUCTURE: assumed same name objects contained in different files for the different channels
// ordered the same as the channels
//
const char *inputfilesfmtstr[NUMCHAN] = {
  "H%d_Electron_2Jets_Fit_Shapes.root",
  "H%d_Electron_3Jets_Fit_Shapes.root",
  "H%d_Muon_2Jets_Fit_Shapes.root",
  "H%d_Muon_3Jets_Fit_Shapes.root",
};

const char *dataobjname = "theData";
const char *bkgdobjname = "h_total";

#ifndef SEVENTEV
// 2012 ICHEP pre-unblinding, 14June, 3.50/pb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*180*/   1.006,   1.005,   1.004,   1.004, // added after unblinding
  /*200*/   1.007,   1.009,   1.004,   1.005,
  /*300*/   1.006,   1.009,   1.010,   1.009, 
  /*400*/   1.009,   1.021,   1.007,   1.018, 
  /*450*/   1.011,   1.022,   1.014,   1.031, 
  /*500*/   1.011,   1.000,   1.014,   1.000,
  /*550*/   1.010,   1.031,   1.015,   1.000, // added after unblinding
  /*600*/   1.012,   1.040,   1.013,   1.000, 
};

#else

// With MVA+Q/G, lumi=5.0/fb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003,
  /*180*/   1.005,   1.005,   1.004,   1.003,
  /*190*/   1.008,   1.005,   1.007,   1.004,
  /*200*/   1.009,   1.008,   1.006,   1.005,
  /*250*/   1.009,   1.009,   1.006,   1.007,
  /*300*/   1.006,   1.012,   1.008,   1.011,

  /*350*/   1.009,   1.036,   1.010,   1.025,
  /*400*/   1.008,   1.028,   1.008,   1.018,

  /*450*/   1.009,   1.030,   1.009,   1.032,
  /*500*/   1.010,   1.031,   1.009,   1.042,
  /*550*/   1.013,   1.044,   1.010,   1.031,
  /*600*/   1.015,   1.054,   1.017,   1.048,
};
#endif

#if 0

// 2012 ICHEP preapproval freeze, 1.60/pb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.005,   1.006,   1.000,    1.006,
  /*180*/   1.009,   1.007,   1.006,    1.005,
  /*190*/   1.009,   1.007,   1.006,    1.006,
  /*200*/   1.009,   1.013,   1.006,    1.008,
  /*250*/   1.008,   1.018,   1.008,    1.010,
  /*300*/   1.008,   1.014,   1.013,    1.015,
  /*350*/   1.012,   1.032,   1.017,    1.025,
  /*400*/   1.010,   1.035,   1.013,    1.025,
  /*450*/   1.012,   1.056,   1.019,    1.045,
  /*500*/   1.011,   1.050,   1.018,    1.050,
  /*550*/   1.012,   1.035,   1.020,    1.035,
  /*600*/   1.013,   1.049,   1.020,    1.049,
};

// First limit of 2012, 560/pb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.007,   1.012,   1.005,   1.008,
  /*180*/   1.013,   1.012,   1.011,   1.009,
  /*190*/   1.014,   1.012,   1.013,   1.009,
  /*200*/   1.015,   1.021,   1.013,   1.012,
  /*250*/   1.021,   1.020,   1.012,   1.020,
  /*300*/   1.023,   1.031,   1.013,   1.037,
  /*350*/   1.024,   1.044,   1.024,   1.047,
  /*400*/   1.020,   1.040,   1.028,   1.039,
  /*450*/   1.026,   1.102,   1.040,   1.100,
  /*500*/   1.035,   1.081,   1.041,   1.132,
  /*550*/   1.036,   1.056,   1.020,   1.097,
  //  /*600*/   1.047,	  ,   1.040,   1.138,
};

#endif

#ifdef SEVENTEV
const double sigselefferrpctlomass =  7.0; // 2011 7TeV
const double sigselefferrpcthimass = 13.0; // 2011 7TeV
#else
const double sigselefferrpct8tev = 10.0; // 2012 8TeV
#endif

double ggsigaccptsyst[NUMMASSPTS] = {
  //  /*170*/ 1.02,
  /*180*/ 1.02,
  //  /*190*/ 1.02,
  /*200*/ 1.02,
  //  /*250*/ 1.015,
  /*300*/ 1.020,
  //  /*350*/ 1.023,
  /*400*/ 1.024,
  /*450*/ 1.027,
  /*500*/ 1.029,
  /*550*/ 1.032,
  /*600*/ 1.036
};

double qqsigaccptsyst[NUMMASSPTS] = {
  //  /*170*/ 1.02,
  /*180*/ 1.02,
  //  /*190*/ 1.02,
  /*200*/ 1.02,
  //  /*250*/ 1.011,
  /*300*/ 1.009,
  //  /*350*/ 1.008,
  /*400*/ 1.006,
  /*450*/ 1.007,
  /*500*/ 1.009,
  /*550*/ 1.009,
  /*600*/ 1.007
};

#endif // _HWWCHANNELS_H
