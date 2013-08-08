#ifndef _HWWCHANNELS_H
#define _HWWCHANNELS_H

#define ISHWW

//#define ISVBF
#undef ISVBF

//#define SEVENTEV
#undef SEVENTEV

//#define DO_INTERP
#undef DO_INTERP

namespace{ // makes all constants contained herein "file-private" to avoid link errors from multiple linked includes

//================================================================================
// High-level variables

#ifdef SEVENTEV
const bool BLINDING = false;
const double intlumipbinv = 5020.0;
const double global_scale = 1.0;
const int beamcomenergytev = 7;

#else

//const bool BLINDING = true;
const bool BLINDING = false;

//const double intlumipbinv_mu = 5100.0;
//const double intlumipbinv_el = 5100.0;
//const double intlumipbinv_mu = 6900.0;
//const double intlumipbinv_el = 6800.0;
//const double intlumipbinv_mu = 12000.0;
//const double intlumipbinv_el = 11900.0;
const double intlumipbinv_mu = 19300.0;
const double intlumipbinv_el = 19200.0;

const int beamcomenergytev = 8;

const double global_scale = 1.0;
//const double global_scale = 5000.0/intlumipbinv;
//const double scalefrom7to8tev = 1.3;

const double scaleBRforTau = 1.5; // because the twiki page does not include tau in the quoted BR for lvqq!

#endif

//================================================================================

//----------------------------------------
#ifdef ISVBF
#define NUMCHAN 2

// must be put in lexicographical order
//
// for combination:
const char *channames[NUMCHAN] = {
  "vbfhwwelnu2j"
  ,"vbfhwwmunu2j"
};
#define ELORMUCHAR 6

const char *channames2[NUMCHAN] = {
  "Electron"
  ,"Muon"
};
//----------------------------------------
#else
#define NUMCHAN 2

// must be put in lexicographical order
//
#if 1
// for combination:
const char *channames[NUMCHAN] = {
  "hwwelnu2j"
  //,"hwwelnu3j"
  ,"hwwmunu2j"
  //,"hwwmunu3j"
};
#define ELORMUCHAR 3
#else
const char *channames[NUMCHAN] = {
  "el2jetCMS"
  //,"el3jetCMS"
  ,"mu2jetCMS"
  //,"mu3jetCMS"
};
#define ELORMUCHAR 0
#endif

const char *channames2[NUMCHAN] = {
  "Electron"
  //,"Electron"
  ,"Muon"
  //,"Muon"
};
#endif
//----------------------------------------

const double mutrigeff      = 1.0;
const double eltrigeff      = 1.0;
const double sigtrigeffunc  = 0.01;
const double siglepteffunc  = 0.02;
const double siglumiunc     = 0.044;

const double W2qqBR    = 0.6770;
const double W2taunuBR = 0.1125;


//--------------------------------------------------

const int NUMMASSPTS=12;
const int masspts[NUMMASSPTS] = {
  170, 180, 190, 200, 250, 300, 350, 400, 450, 500, 550, 600
  //180, 190, 200, 250, 300, 350, 400, 450, 500, 550, 600
  //300, 350, 400, 450, 500, 550, 600
  //700,800,900
  //170,180,190,
  //200,500,
  //200,250,300,350,400,450,500,550,600
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
#ifdef ISVBF
const char *inputfilesfmtstr[NUMCHAN] = {
  "H%d_Electron_2Jets_Fit_Shapes.root",
  "H%d_Muon_2Jets_Fit_Shapes.root",
};
#else
const char *inputfilesfmtstr[NUMCHAN] = {
  "HWW%dlnujj_electron_2jets_1D2Fit_output.root",
  //"HWWlnujjH%d_electron_3jets_output.root",
  "HWW%dlnujj_muon_2jets_1D2Fit_output.root",
  //"HWWlnujjH%d_muon_3jets_output.root",
};
const char *perlrecapturefmt = "(.*/)?HWW([0-9]{3})lnujj_(.*?)_([23])jets_.*?.root";
#endif

const char *dataobjname = "theData";
const char *bkgdobjname = "h_total";

#ifdef SEVENTEV
const double sigselefferrpctlomass =  7.0; // 2011 7TeV
const double sigselefferrpcthimass = 13.0; // 2011 7TeV
#else
const double sigselefferrpct8tev = 10.0; // 2012 8TeV
#endif

double ggsigaccptsyst[NUMMASSPTS] = {
#if 1
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
#endif
  /*200*/ 1.02,
#if 1
  /*250*/ 1.015,
  /*300*/ 1.020,
  /*350*/ 1.023,
  /*400*/ 1.024,
  /*450*/ 1.027,
#endif
  /*500*/ 1.029,
#if 1
  /*550*/ 1.032,
  /*600*/ 1.036,
  // /*700*/ 1.046,
  // /*800*/ 1.040,
  // /*900*/ 1.040,
#endif
};

double qqsigaccptsyst[NUMMASSPTS] = {
#if 1
  /*170*/ 1.02,
  /*180*/ 1.02,
  /*190*/ 1.02,
#endif
  /*200*/ 1.02,
#if 1
  /*250*/ 1.011,
  /*300*/ 1.009,
  /*350*/ 1.008,
  /*400*/ 1.006,
  /*450*/ 1.007,
#endif
  /*500*/ 1.009,
#if 1
  /*550*/ 1.009,
  /*600*/ 1.007,
  //  /*700*/ 1.010,
  //  /*800*/ 1.010,
  //  /*900*/ 1.010,
#endif
};

typedef struct {
  double lo; double hi;
} lohi_t;

 const lohi_t interfggH[NUMMASSPTS] = {
   /*170*/ {0.0,0.0},
   /*180*/ {0.0,0.0},
   /*190*/ {0.0,0.0},
   /*200*/ {0.0,0.0},
   /*250*/ {0.0,0.0},
   /*300*/ {0.0,0.0},
   /*350*/ {0.0,0.0},
   /*400*/ {1.00,1.00},
   /*450*/ {0.99,1.01},
   /*500*/ {0.99,1.01},
   /*550*/ {0.97,1.04},
   /*600*/ {0.95,1.07}
 };
} // end unnamed namespace

#endif // _HWWCHANNELS_H
