#include <iostream>
#include <iomanip>
#include "stdlib.h"
#include "math.h"
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TTree.h"
#include "TF1.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include "THStack.h"
#include "TLatex.h"

const TString inData = "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/";
static const int TotI = 52;

static const double DatLumi_mu     = 11500;
static const double DatLumi_el     = 11900;

static const double TrigEff_mu     = 1.000;
static const double TrigEff_el     = 1.000;

static const double XSecs[TotI][4] ={ 
  //xsecs pb-1, filter eff,     events,         k-factor 
  {36257.2,             1,           18353019,      1}, // 0  "W Jets Madgraph",                              // 0
  {225.197,               1,           6893735,       1}, // 1  "TTbar Madgrap",                                // 1
  {3.79,                1,           259960,        1}, // 2  "Single Top T S Channel Powheg",                // 2
  {1.76,                1,           139974,        1}, // 3  "Single Top Tbar S Channel Powheg",             // 3
  {56.4,               1,           3758221,       1}, // 4  "Single Top T T Channel Powheg",                // 4
  {30.7,               1,           1935066,       1}, // 5  "Single Top Tbar T Channel Powheg",             // 5
  {11.1,                1,           497657,        1}, // 6  "Single Top T TW Channel Powheg",               // 6
  {11.1,                1,           493458,        1}, // 7  "Single Top Tbar TW Channel Powheg",            // 7
  {3503.71,              1,           30209426,      1}, // 8  "DY Jets Madgraph",                             // 8
  {296900000.0,         0.0002684,   0.000000,      1}, // 9  "QCD Pythia6",                                  // 9
  {57.1,                1,            9450414,       1}, // 10 "WW Pythia6",                                   // 10
  {32.3,                1,           10000267,       1}, // 11 "WZ Pythia6",                                   // 11
  {11439,               0.773,       10430764,      1}, // 12 "W Jets Matching Up Madgraph",                  // 12
  {42352,               0.201,       9846896,       1}, // 13 "W Jets Matching Down Madgraph",                // 13
  {17859,               0.556,       9724842,       1}, // 14 "W Jets Scale Up Madgraph",                     // 14
  {20137,               0.555,       9950856,       1}, // 15 "W Jets Scale Down Madgraph",                   // 15
  {31500.0,             1,           62474300,      1}, // 16 "W Jets Sherpa",                                // 16 
  { 1.0,                1.0,         109996,        1}, // 17 HWW120
  { 1.0,                1.0,         109992,        1}, // 18 HWW130
  { 1.0,                1.0,         109990,        1}, // 19 HWW140
  { 1.0,                1.0,         109993,        1}, // 20 HWW150
  { 2.415,              1.0,         109992,        1}, // 21 HWW160
  { 4.7334,              1.0,         199986,        1}, // 22 HWW170
  { 4.0508,              1.0,         197496,        1}, // 23 HWW180
  { 3.0298,              1.0,         194504,        1}, // 24 HWW190
  {7.97110,             0.108*2,       199982,        1}, // 25 HWW200
  { 5.38220,            0.103*2,       194513,        1}, // 26 HWW250
  { 4.04680,            0.101*2,       197284,        1}, // 27 HWW300
  { 3.72600,            0.0990*2,      199800,        1}, // 28 HWW350
  { 3.17830,            0.0852*2,      199485,        1}, // 29 HWW400
  { 2.20320,            0.0810*2,      198969,        1}, // 30 HWW450
  { 1.43910,            0.0799*2,      198470,        1}, // 31 HWW500
  { 0.93640,            0.0806*2,      199471,        1}, // 32 HWW550
  { 0.61988,            0.0818*2,      197170,        1}, // 33 HWW600
  { 1.0,                1.0,         217674,        1}, // 34 VBFHWW120
  { 1.0,                1.0,         216316,        1}, // 35 VBFHWW130
  { 1.0,                1.0,         219691,        1}, // 36 VBFHWW140
  { 1.0,                1.0,         219727,        1}, // 37 VBFHWW150
  { 1.0,                1.0,         169351,        1}, // 38 VBFHWW160
  { 1.0,                1.0,         161768,        1}, // 39 VBFHWW170
  { 1.0,                1.0,         219267,        1}, // 40 VBFHWW180
  { 1.0,                1.0,         213014,        1}, // 41 VBFHWW190
  { 1.0,                1.0,         217423,        1}, // 42 VBFHWW200
  { 1.0,                1.0,         218679,        1}, // 43 VBFHWW250
  { 1.0,                1.0,         217427,        1}, // 44 VBFHWW300
  { 1.0,                1.0,         213673,        1}, // 45 VBFHWW350
  { 1.0,                1.0,         204894,        1}, // 46 VBFHWW400
  { 1.0,                1.0,         164553,        1}, // 47 VBFHWW450
  { 1.0,                1.0,         218520,        1}, // 48 VBFHWW500
  { 1.0,                1.0,         215938,        1}, // 49 VBFHWW550
  { 1.0,                1.0,         214510,        1}, // 50 VBFHWW600
  { 1.0,                1.0,         1.0000,        1}  //
};

static const char * PlainNames[TotI]= { 
  "W Jets",                                       // 0
  "TTbar",                                        // 1
  "Single Top T S",                               // 2
  "Single Top Tbar S",                            // 3
  "Single Top T T",                               // 4
  "Single Top Tbar T",                            // 5
  "Single Top T TW",                              // 6
  "Single Top Tbar TW",                           // 7
  "DY Jets",                                      // 8
  "QCD-DataDriven",                               // 9
  "WW",                                           // 10
  "WZ",                                           // 11
  "W Jets Matching Up",                           // 12
  "W Jets Matching Down",                         // 13
  "W Jets Scale Up",                              // 14
  "W Jets Scale Down",                            // 15
  "W Jets Sherpa",                                // 16 
  "HWW Higgs Mass 120",                           // 17 
  "HWW Higgs Mass 130",                           // 18 
  "HWW Higgs Mass 140",                           // 19 
  "HWW Higgs Mass 150",                           // 20 
  "HWW Higgs Mass 160",                           // 21 
  "HWW Higgs Mass 170",                           // 22 
  "HWW Higgs Mass 180",                           // 23 
  "HWW Higgs Mass 190",                           // 24 
  "HWW Higgs Mass 200",                           // 25 
  "HWW Higgs Mass 250",                           // 26 
  "HWW Higgs Mass 300",                           // 27 
  "HWW Higgs Mass 350",                           // 28 
  "HWW Higgs Mass 400",                           // 29 
  "HWW Higgs Mass 450",                           // 30 
  "HWW Higgs Mass 500",                           // 31 
  "HWW Higgs Mass 550",                           // 32 
  "HWW Higgs Mass 600",                           // 33 
  "VBFHWW Higgs Mass 120",                        // 34 
  "VBFHWW Higgs Mass 130",                        // 35 
  "VBFHWW Higgs Mass 140",                        // 36 
  "VBFHWW Higgs Mass 150",                        // 37 
  "VBFHWW Higgs Mass 160",                        // 38 
  "VBFHWW Higgs Mass 170",                        // 39 
  "VBFHWW Higgs Mass 180",                        // 40 
  "VBFHWW Higgs Mass 190",                        // 41 
  "VBFHWW Higgs Mass 200",                        // 42
  "VBFHWW Higgs Mass 250",                        // 43
  "VBFHWW Higgs Mass 300",                        // 44
  "VBFHWW Higgs Mass 350",                        // 45
  "VBFHWW Higgs Mass 400",                        // 46
  "VBFHWW Higgs Mass 450",                        // 47
  "VBFHWW Higgs Mass 500",                        // 48
  "VBFHWW Higgs Mass 550",                        // 49
  "VBFHWW Higgs Mass 600",                        // 50
  "Data",                                         // 51
};


static const char *Mu_InFileNames[TotI] = {
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_TTbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopS_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopS_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopT_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopT_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopTW_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_STopTW_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_ZpJ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RDQCD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WW_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WZ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJmatchingup_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJmatchingdown_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJscaleup_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJscaledown_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_WpJsherpa_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH120_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH130_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH140_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH150_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH160_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH170_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH180_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH190_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH200_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH250_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH300_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH350_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH400_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH450_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH500_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH550_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_HWWMH600_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH120_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH130_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH140_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH150_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH160_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH170_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH180_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH190_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH200_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH250_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH300_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH350_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH400_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH450_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH500_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH550_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_mu_VBFHWWMH600_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_WmunuJets_DataAll_GoldenJSON_11p5invfb.root"
};

static const char *El_InFileNames[TotI] = {
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_TTbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopS_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopS_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopT_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopT_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopTW_T_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_STopTW_Tbar_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_ZpJ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RDQCD_WenuJets_DataAll_GoldenJSON_2p1invfb.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WW_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WZ_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJmatchingup_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJmatchingdown_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJscaleup_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJscaledown_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_WpJsherpa_CMSSW532.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH120_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH130_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH140_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH150_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH160_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH170_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH180_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH190_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH200_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH250_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH300_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH350_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH400_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH450_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH500_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH550_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_HWWMH600_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH120_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH130_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH140_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH150_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH160_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH170_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH180_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH190_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH200_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH250_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH300_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH350_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH400_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH450_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH500_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH550_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_el_VBFHWWMH600_CMSSW532_private.root",
  "/uscms_data/d1/andersj/Wjj/2012/data/ReducedTrees/RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_11p9invfb.root"
};

