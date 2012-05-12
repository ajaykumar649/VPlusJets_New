#ifndef _HWWCHANNELS_H
#define _HWWCHANNELS_H

#define ISHWW

#define NUMCHAN 4

// must be put in lexicographical order
//
#if 0
// for combination:
const char *channames[NUMCHAN] = {
  "hwwelnu2j"
  ,"hwwelnu3j"
  ,"hwwmunu2j"
  ,"hwwmunu3j"
};
#else
const char *channames[NUMCHAN] = {
  "el2jetCMS"
  ,"el3jetCMS"
  ,"mu2jetCMS"
  ,"mu3jetCMS"
};
#endif

const char *channames2[NUMCHAN] = {
  "Electron"
  ,"Electron"
  ,"Muon"
  ,"Muon"
};

const double mutrigeff      = 0.975;
const double eltrigeff      = 0.821;
const double sigtrigeffunc  = 0.01;
const double siglepteffunc  = 0.02;
//const double siglumiunc     = 0.045;
//const double siglumiunc     = 0.036;
const double siglumiunc     = 0.022;

//const double intlumipbinv = 1200.0;
//const double intlumipbinv = 2100.0;
//const double intlumipbinv = 4700.0;
const double intlumipbinv = 5020.0;

const double W2qqBR    = 0.6770;
const double W2taunuBR = 0.1125;

const int NUMMASSPTS=23;
//const int NUMMASSPTS=12;
const int masspts[NUMMASSPTS] = {
  170,180,190,200,
  250,300,
  305,310,315,320,325, // new interpolated points
  330,335,340,345,     // new interpolated points
  350,400,
  420,440,             // new interpolated points
  450,500,550,600
};

const int xminpermass[NUMMASSPTS] = {
  170,170,170,180,
  200,240,
  240,240,240,240,240, // new interpolated points
  300,300,300,300,     // new interpolated points
  300,300,
  300,340,             // new interpolated points
  340,340,340,340
};

const int xmaxpermass[NUMMASSPTS] = {
  250,250,250,250,
  400,400,
  400,400,400,400,400, // new interpolated points
  780,780,780,780,     // new interpolated points
  780,780,
  780,780,             // new interpolated points
  780,780,780,780
};

const int binwpermass[NUMMASSPTS] = {
  10,10,10,10,
  20,20,
  20,20,20,20,20,      // new interpolated points
  40,40,40,40,         // new interpolated points
  40,40,
  40,40,               // new interpolated points
  40,40,40,40
};

// inputs are normalized to 1/nent

// FILE/HISTO STRUCTURE: assumed same name objects contained in different files for the different channels

// ordered the same as the channels
//
const char *dir =
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWWplots/";
  // "/uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_2_3_patch3/src/ElectroWeakAnalysis/VPlusJets/test/HWWplots/";
  //"/uscms_data/d2/kalanand/junk/vplusjets/CMSSW_4_2_3_patch3/src/ElectroWeakAnalysis/VPlusJets/test/HWWplotsNewOpt/";
  //"/uscms_data/d2/pdudero/MVAnewOptPointByPointInputFiles/";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW4p7NoQGLResults";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW4p7PATNoGQL";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW4p7PATNoGQL_tweaked";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW4p7PATNoGQL_tweak2";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW4p7PATWithQGL";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0Result";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInject";
  //"/uscms_data/d2/andersj/Wjj/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInject400x5";
  //"/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0Result";
  //"/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInjectx0";
  //"/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInjectx1";
  //"/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInjectx2";
  //"/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInjectx5";
  "/uscms_data/d2/andersj/Wjj/HWW/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW5p0SigInjectx10";

const char *inputfilesfmtstr[NUMCHAN] = {
  "H%d_Electron_2Jets_Fit_Shapes.root",
  "H%d_Electron_3Jets_Fit_Shapes.root",
  "H%d_Muon_2Jets_Fit_Shapes.root",
  "H%d_Muon_3Jets_Fit_Shapes.root",
};

const char *dataobjname = "theData";
const char *bkgdobjname = "h_total";

#if 0
// signal injection 400x5
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   0.003,   0.004,   0.002,   0.003,
  /*180*/   0.005,   0.005,   0.004,   0.003, 
  /*190*/   0.008,   0.005,   0.007,   0.004, 
  /*200*/   0.009,   0.008,   0.006,   0.005, 
  /*250*/   0.009,   0.010,   0.006,   0.009, 
  /*300*/   0.006,   0.012,   0.008,   0.011, 
  /*350*/   0.009,   0.032,   0.010,   0.024, 
  /*400*/   0.008,   0.028,   0.008,   0.019, 
  /*450*/   0.009,   0.055,   0.009,   0.035, 
  /*500*/   0.010,   0.035,   0.009,   0.091, 
  /*600*/   0.015,   0.053,   0.018,   0.049, 
};

// signal injection 400x2
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   0.003,   0.004,   0.002,   0.003,
  /*180*/   0.005,   0.005,   0.004,   0.003,
  /*190*/   0.008,   0.005,   0.007,   0.004,
  /*200*/   0.009,   0.008,   0.006,   0.005,
  /*250*/   0.009,   0.009,   0.006,   0.007,
  /*300*/   0.006,   0.012,   0.008,   0.011,
  /*350*/   0.009,   0.034,   0.010,   0.024,
  /*400*/   0.008,   0.028,   0.008,   0.019,
  /*450*/   0.009,   0.036,   0.009,   0.033,
  /*500*/   0.010,   0.033,   0.009,   0.041,
  /*600*/   0.015,   0.055,   0.018,   0.046,
};

// signal injection 400x20
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   0.003,   0.004,   0.002,   0.003,
  /*180*/   0.005,   0.005,   0.004,   0.003,
  /*190*/   0.008,   0.005,   0.007,   0.004,
  /*200*/   0.009,   0.008,   0.006,   0.005,
  /*250*/   0.008,   0.008,   0.008,   0.009,
  /*300*/   0.006,   0.011,   0.008,   0.010,
  /*350*/   0.009,   0.031,   0.014,   0.026,
  /*400*/   0.008,   0.027,   0.010,   0.022,
  /*450*/   0.012,   0.041,   0.010,   0.034,
  /*500*/   0.011,   0.042,   0.010,   0.064,
  /*600*/   0.015,   0.055,   0.022,   0.048,   
};

#endif

// With MVA+Q/G, lumi=5.0/fb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003,
  /*180*/   1.005,   1.005,   1.004,   1.003,
  /*190*/   1.008,   1.005,   1.007,   1.004,
  /*200*/   1.009,   1.008,   1.006,   1.005,
  /*250*/   1.009,   1.009,   1.006,   1.007,
  /*300*/   1.006,   1.012,   1.008,   1.011,

#if 1
  // interpolated points
  /*305*/   1.006,   1.012,   1.008,   1.011,
  /*310*/   1.006,   1.012,   1.008,   1.011,
  /*315*/   1.006,   1.012,   1.008,   1.011,
  /*320*/   1.006,   1.012,   1.008,   1.011,
  /*325*/   1.006,   1.012,   1.008,   1.011,

  // interpolated points
  /*330*/   1.009,   1.036,   1.010,   1.025,
  /*335*/   1.009,   1.036,   1.010,   1.025,
  /*340*/   1.009,   1.036,   1.010,   1.025,
  /*345*/   1.009,   1.036,   1.010,   1.025,
#endif
  /*350*/   1.009,   1.036,   1.010,   1.025,
  /*400*/   1.008,   1.028,   1.008,   1.018,
#if 1
  /*420*/   1.008,   1.028,   1.008,   1.018, // interpolated point

  /*440*/   1.009,   1.030,   1.009,   1.032, // interpolated point
#endif
  /*450*/   1.009,   1.030,   1.009,   1.032,
  /*500*/   1.010,   1.031,   1.009,   1.042,
  /*550*/   1.013,   1.044,   1.010,   1.031,
  /*600*/   1.015,   1.054,   1.017,   1.048,
};

#if 0
// With Q/G likelihood as a separate rectangular cut
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003, 
  /*180*/   1.005,   1.005,   1.004,   1.003, 
  /*190*/   1.008,   1.005,   1.007,   1.004, 
  /*200*/   1.008,   1.007,   1.006,   1.004, 
  /*250*/   1.009,   1.009,   1.006,   1.007, 
  /*300*/   1.006,   1.011,   1.008,   1.011, 
  /*350*/   1.009,   1.035,   1.010,   1.025, 
  /*400*/   1.008,   1.028,   1.008,   1.019, 
  /*450*/   1.009,   1.030,   1.009,   1.033, 
  /*500*/   1.011,   1.033,   1.009,   1.045, 
  /*550*/   1.013,   1.044,   1.010,   1.032, 
  /*600*/   1.015,   1.054,   1.018,   1.047, 
};

// After switching to PATtuples, no QGLD, and tweaking fit ranges
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003,
  /*180*/   1.005,   1.004,   1.004,   1.003,

  //  /*190*/   1.006,   1.006,   1.005,   1.004, // shape morphing bkgd
  //  /*200*/   1.006,   1.008,   1.004,   1.005, // shape morphing bkgd
  /*190*/   1.008,   1.006,   1.007,   1.004,    // parametric bkgd, extra 170-180 bin
  /*200*/   1.008,   1.008,   1.005,   1.004,    // parametric bkgd, extra 170-180 bin

  /*250*/   1.009,   1.009,   1.006,   1.007,
  /*300*/   1.006,   1.011,   1.007,   1.011,
  /*350*/   1.009,   1.036,   1.010,   1.025,
  /*400*/   1.008,   1.028,   1.008,   1.019,
  /*450*/   1.009,   1.029,   1.009,   1.034,
  /*500*/   1.012,   1.050,   1.009,   1.039,
  /*550*/   1.014,   1.032,   1.009,   1.027,
  /*600*/   1.012,   1.046,   1.011,   1.037,
};

// After switching to PATtuples, no QGLD, go back to 4.7/fb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003, 
  /*180*/   1.005,   1.004,   1.004,   1.003, 
  /*190*/   1.006,   1.006,   1.005,   1.004, 
  /*200*/   1.006,   1.008,   1.004,   1.005, 
  /*250*/   1.009,   1.008,   1.012,   1.009, 
  /*300*/   1.006,   1.011,   1.010,   1.010, 
  /*350*/   1.009,   1.036,   1.010,   1.025, 
  /*400*/   1.008,   1.028,   1.008,   1.019, 
  /*450*/   1.006,   1.029,   1.007,   1.024, 
  /*500*/   1.007,   1.036,   1.007,   1.030, 
  /*550*/   1.007,   1.025,   1.007,   1.019, 
  /*600*/   1.008,   1.028,   1.009,   1.023, 
};

// after adding Wtaunu,new lumi update
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.004,   1.005,   1.003,   1.003,
  /*180*/   1.007,   1.005,   1.005,   1.003, 
  /*190*/   1.008,   1.005,   1.006,   1.003, 
  /*200*/   1.008,   1.005,   1.006,   1.004, 
  /*250*/   1.008,   1.008,   1.008,   1.007, 
  /*300*/   1.005,   1.010,   1.004,   1.009, 
  /*350*/   1.008,   1.020,   1.008,   1.016, 
  /*400*/   1.007,   1.020,   1.007,   1.014, 
  /*450*/   1.007,   1.021,   1.008,   1.018, 
  /*500*/   1.007,   1.026,   1.007,   1.017, 
  /*550*/   1.010,   1.033,   1.008,   1.022, 
  /*600*/   1.014,   1.023,   1.012,   1.022 
};

// second optimization from brute force scan
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*          el2j      el3j     mu2j     mu3j */
  /*170*/   1.010,   1.005,   1.003,   1.003, 
  /*180*/   1.007,   1.005,   1.005,   1.003, 
  /*190*/   1.008,   1.005,   1.006,   1.003, 
  /*200*/   1.007,   1.005,   1.006,   1.004, 
  /*250*/   1.008,   1.009,   1.008,   1.007, 
  /*300*/   1.005,   1.010,   1.004,   1.009, 
  /*350*/   1.008,   1.020,   1.008,   1.017, 
  /*400*/   1.007,   1.020,   1.007,   1.015, 
  /*450*/   1.007,   1.021,   1.008,   1.019, 
  /*500*/   1.007,   1.026,   1.007,   1.017, 
  /*550*/   1.010,   1.032,   1.008,   1.023, 
  /*600*/   1.014,   1.023,   1.012,   1.024 
};

// first optimization, more aggressive MVA cuts
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*          el2j      el3j     mu2j     mu3j */
  /*170*/    1.008,    1.004,   1.003,   1.004,
  /*180*/    1.007,    1.006,   1.006,   1.005,
  /*190*/    1.012,    1.007,   1.010,   1.007,
  /*200*/    1.009,    1.006,   1.007,   1.006,
  /*250*/    1.007,    1.008,   1.010,   1.007,
  /*300*/    1.006,    1.008,   1.006,   1.009,
  /*350*/    1.009,    1.021,   1.010,   1.025,
  /*400*/    1.008,    1.019,   1.008,   1.019,
  /*450*/    1.007,    1.017,   1.008,   1.014,
  /*500*/    1.007,    1.016,   1.006,   1.012,
  /*550*/    1.009,    1.020,   1.011,   1.013,
  /*600*/    1.010,    1.015,   1.012,   1.010,
};
double sigselefferrpct[NUMMASSPTS*NUMCHAN] = {
  /*          el2j      el3j     mu2j     mu3j */
  /*170*/    8.94,     10.13,    6.10,     7.81,
  /*180*/    5.82,      7.53,    3.62,     7.56, 
  /*190*/    4.89,      9.04,    3.81,     8.07, 
  /*200*/    6.86,      2.79,    3.82,     3.73, 
  /*250*/    2.29,      4.81,    2.53,     6.80, 
  /*300*/    1.00,      3.65,    1.29,     4.92, 
  /*350*/    2.73,      0.07,    1.97,     2.75, 
  /*400*/    1.93*2,    4.01*2,  0.55*2,   4.71*2, 
  /*450*/    1.86*2,    4.82*2,  1.34*2,   4.16*2, 
  /*500*/    3.53*2,    7.22*2,  1.54*2,   5.71*2, 
  /*550*/    3.47*2,    7.66*2,  2.10*2,   8.95*2, 
  /*600*/    7.28*2,   11.24*2,  6.63*2,  11.42*2,
};
// from brute force scan by increments of 0.1
double sigselefferrpct[NUMMASSPTS*NUMCHAN] = {
  /*        el2j   el3j   mu2j   mu3j */
  /*170*/   2.43,  2.47,  1.91,  2.52,
  /*180*/   6.90,  1.51,  4.78,  1.01,
  /*190*/   4.19,  2.24,  4.13,  2.49,
  /*200*/   5.69,  2.06,  4.09,  1.66,
  /*250*/   2.04,  3.03,  2.24,  6.02,
  /*300*/   1.63,  3.55,  0.83,  5.29,
  /*350*/   3.18,  1.75,  2.41,  2.79,
  /*400*/   2.38,  2.07,  0.64,  1.88,
  /*450*/   1.28,  0.99,  1.43,  1.39,
  /*500*/   1.27,  4.42,  0.67,  3.96,
  /*550*/   4.04,  5.64,  0.79,  4.21,
  /*600*/   4.24,  6.86,  3.30,  5.56
};
#endif
// Cross-check says it just doesn't matter...so make it simple
double sigselefferrpct[NUMMASSPTS*NUMCHAN] = {
  /*        el2j   el3j   mu2j   mu3j */
  /*170*/   7.00,  7.00,  7.00,  7.00,
  /*180*/   7.00,  7.00,  7.00,  7.00,
  /*190*/   7.00,  7.00,  7.00,  7.00,
  /*200*/   7.00,  7.00,  7.00,  7.00,
  /*250*/   7.00,  7.00,  7.00,  7.00,
  /*300*/   7.00,  7.00,  7.00,  7.00,
#if 1
  // interpolated points
  /*305*/   7.00,  7.00,  7.00,  7.00,
  /*310*/   7.00,  7.00,  7.00,  7.00,
  /*315*/   7.00,  7.00,  7.00,  7.00,
  /*320*/   7.00,  7.00,  7.00,  7.00,
  /*325*/   7.00,  7.00,  7.00,  7.00,
  /*330*/   7.00,  7.00,  7.00,  7.00,
  /*335*/   7.00,  7.00,  7.00,  7.00,
  /*340*/   7.00,  7.00,  7.00,  7.00,
  /*345*/   7.00,  7.00,  7.00,  7.00,
#endif
  /*350*/   7.00,  7.00,  7.00,  7.00,
  /*400*/   7.00,  7.00,  7.00,  7.00,
#if 1
  // interpolated points
  /*420*/   7.00,  7.00,  7.00,  7.00,
  /*440*/   7.00,  7.00,  7.00,  7.00,
#endif
  /*450*/   7.00,  7.00,  7.00,  7.00,
  /*500*/  13.00, 13.00, 13.00, 13.00,
  /*550*/  13.00, 13.00, 13.00, 13.00,
  /*600*/  13.00, 13.00, 13.00, 13.00
};

double ggsigaccptsyst[NUMMASSPTS] = {
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
  /*200*/ 1.02,
  /*250*/ 1.015,
  /*300*/ 1.020,
#if 1
  // interpolated points
  /*305*/ 1.020,
  /*310*/ 1.020,
  /*315*/ 1.020,
  /*320*/ 1.020,
  /*325*/ 1.020,

  // interpolated points
  /*330*/ 1.023,
  /*335*/ 1.023,
  /*340*/ 1.023,
  /*345*/ 1.023,
#endif
  /*350*/ 1.023,
  /*400*/ 1.024,
#if 1
  /*420*/ 1.024, // interpolated point

  /*440*/ 1.027, // interpolated point
#endif
  /*450*/ 1.027,
  /*500*/ 1.029,
  /*550*/ 1.032,
  /*600*/ 1.036
};

double qqsigaccptsyst[NUMMASSPTS] = {
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
  /*200*/ 1.02,
  /*250*/ 1.011,
  /*300*/ 1.009,
#if 1
  // interpolated points
  /*305*/ 1.009,
  /*310*/ 1.009,
  /*315*/ 1.009,
  /*320*/ 1.009,
  /*325*/ 1.009,

  // interpolated points
  /*330*/ 1.008,
  /*335*/ 1.008,
  /*340*/ 1.008,
  /*345*/ 1.008,
#endif
  /*350*/ 1.008,
  /*400*/ 1.006,
#if 1
  /*420*/ 1.006, // interpolated point

  /*440*/ 1.007, // interpolated point
#endif
  /*450*/ 1.007,
  /*500*/ 1.009,
  /*550*/ 1.009,
  /*600*/ 1.007
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

#endif // _HWWCHANNELS_H
