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

//const double intlumipbinv = 5020.0;
const double intlumipbinv = 560.6;

const double W2qqBR    = 0.6770;
const double W2taunuBR = 0.1125;

const double global_scale = 1.0;
//const double global_scale = 5000.0/intlumipbinv;
const double scalefrom7to8tev = 1.3;

//#define INTERPPTS
#undef INTERPPTS

#ifdef INTERPPTS
const int NUMMASSPTS=23;
#else
//const int NUMMASSPTS=12;
const int NUMMASSPTS=11;
#endif

//--------------------------------------------------

const int masspts[NUMMASSPTS] = {
  170,180,190,200,
  250,300,

#ifdef INTERPPTS
  305,310,315,320,325, // new interpolated points
  330,335,340,345,     // new interpolated points
#endif

  350,400,

#ifdef INTERPPTS
  420,440,             // new interpolated points
#endif

  450,500,550 // ,600
};

//--------------------------------------------------

const int xminpermass[NUMMASSPTS] = {
  170,170,170,180,
  200,240,

#ifdef INTERPPTS
  240,240,240,240,240, // new interpolated points
  300,300,300,300,     // new interpolated points
#endif

  300,300,

#ifdef INTERRPTS
  300,340,             // new interpolated points
#endif

  340,340,340 // ,340
};

//--------------------------------------------------

const int xmaxpermass[NUMMASSPTS] = {
  250,250,250,250,
  400,400,

#ifdef INTERPPTS
  400,400,400,400,400, // new interpolated points
  780,780,780,780,     // new interpolated points
#endif

  780,780,

#ifdef INTERPPTS
  780,780,             // new interpolated points
#endif

  780,780,780 // ,780
};

//--------------------------------------------------

const int binwpermass[NUMMASSPTS] = {
  10,10,10,10,
  20,20,

#ifdef INTERPPTS
  20,20,20,20,20,      // new interpolated points
  40,40,40,40,         // new interpolated points
#endif

  40,40,

#ifdef INTERPPTS
  40,40,               // new interpolated points
#endif

  40,40,40 // ,40
};

//--------------------------------------------------

// inputs are normalized to 1/nent

// FILE/HISTO STRUCTURE: assumed same name objects contained in different files for the different channels

// ordered the same as the channels
//
const char *dir =
  "/uscms_data/d2/andersj/Wjj/2012/CMSSW_4_2_3_patch5/src/ElectroWeakAnalysis/VPlusJets/test/HWW0p6Result";

const char *inputfilesfmtstr[NUMCHAN] = {
  "H%d_Electron_2Jets_Fit_Shapes.root",
  "H%d_Electron_3Jets_Fit_Shapes.root",
  "H%d_Muon_2Jets_Fit_Shapes.root",
  "H%d_Muon_3Jets_Fit_Shapes.root",
};

const char *dataobjname = "theData";
const char *bkgdobjname = "h_total";

// First limit of 2012, 560/pb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   0.007,   0.012,   0.005,   0.008,
  /*180*/   0.013,   0.012,   0.011,   0.009,
  /*190*/   0.014,   0.012,   0.013,   0.009,
  /*200*/   0.015,   0.021,   0.013,   0.012,
  /*250*/   0.021,   0.020,   0.012,   0.020,
  /*300*/   0.023,   0.031,   0.013,   0.037,
  /*350*/   0.024,   0.044,   0.024,   0.047,
  /*400*/   0.020,   0.040,   0.028,   0.039,
  /*450*/   0.026,   0.102,   0.040,   0.100,
  /*500*/   0.035,   0.081,   0.041,   0.132,
  /*550*/   0.036,   0.056,   0.020,   0.097,
  //  /*600*/   0.047,	  ,   0.040,   0.138,
};

#if 0
// With MVA+Q/G, lumi=5.0/fb
double backnormerr[NUMMASSPTS*NUMCHAN] = {
  /*         el2j     el3j     mu2j     mu3j */
  /*170*/   1.003,   1.004,   1.002,   1.003,
  /*180*/   1.005,   1.005,   1.004,   1.003,
  /*190*/   1.008,   1.005,   1.007,   1.004,
  /*200*/   1.009,   1.008,   1.006,   1.005,
  /*250*/   1.009,   1.009,   1.006,   1.007,
  /*300*/   1.006,   1.012,   1.008,   1.011,

#ifdef INTERPPTS
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
#ifdef INTERPPTS
  /*420*/   1.008,   1.028,   1.008,   1.018, // interpolated point

  /*440*/   1.009,   1.030,   1.009,   1.032, // interpolated point
#endif
  /*450*/   1.009,   1.030,   1.009,   1.032,
  /*500*/   1.010,   1.031,   1.009,   1.042,
  /*550*/   1.013,   1.044,   1.010,   1.031,
  /*600*/   1.015,   1.054,   1.017,   1.048,
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
#ifdef INTERPPTS
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
#ifdef INTERPPTS
  // interpolated points
  /*420*/   7.00,  7.00,  7.00,  7.00,
  /*440*/   7.00,  7.00,  7.00,  7.00,
#endif
  /*450*/   7.00,  7.00,  7.00,  7.00,
  /*500*/  13.00, 13.00, 13.00, 13.00,
  /*550*/  13.00, 13.00, 13.00, 13.00,
  //  /*600*/  13.00, 13.00, 13.00, 13.00
};

double ggsigaccptsyst[NUMMASSPTS] = {
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
  /*200*/ 1.02,
  /*250*/ 1.015,
  /*300*/ 1.020,
#ifdef INTERPPTS
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
#ifdef INTERPPTS
  /*420*/ 1.024, // interpolated point

  /*440*/ 1.027, // interpolated point
#endif
  /*450*/ 1.027,
  /*500*/ 1.029,
  /*550*/ 1.032,
  //  /*600*/ 1.036
};

double qqsigaccptsyst[NUMMASSPTS] = {
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
  /*200*/ 1.02,
  /*250*/ 1.011,
  /*300*/ 1.009,
#ifdef INTERPPTS
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
#ifdef INTERPPTS
  /*420*/ 1.006, // interpolated point
  /*440*/ 1.007, // interpolated point
#endif
  /*450*/ 1.007,
  /*500*/ 1.009,
  /*550*/ 1.009,
  // /*600*/ 1.007
};


#endif // _HWWCHANNELS_H
