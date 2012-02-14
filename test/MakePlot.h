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

const TString inData = "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/";
static const int TotI = 52;

static const double DatLumi_mu     = 4710.649;
static const double DatLumi_el     = 4717.435;

static const double TrigEff_mu     = 1.000;
static const double TrigEff_el     = 1.000;

static const double XSecs[TotI][4] ={ 
  //xsecs pb-1, filter eff,     events,         k-factor 
  {31500.0,             1,           81352581,      1}, // 0  "W Jets Madgraph",                              // 0
  {157.5,               1,           3701947,       1}, // 1  "TTbar Madgrap",                                // 1
  {3.19,                1,           259971,        1}, // 2  "Single Top T S Channel Powheg",                // 2
  {1.44,                1,           137980,        1}, // 3  "Single Top Tbar S Channel Powheg",             // 3
  {41.92,               1,           3900171,       1}, // 4  "Single Top T T Channel Powheg",                // 4
  {22.65,               1,           1944826,       1}, // 5  "Single Top Tbar T Channel Powheg",             // 5
  {7.87,                1,           795379,        1}, // 6  "Single Top T TW Channel Powheg",               // 6
  {7.87,                1,           787629,        1}, // 7  "Single Top Tbar TW Channel Powheg",            // 7
  {3048.0,              1,           36277961,      1}, // 8  "DY Jets Madgraph",                             // 8
  {296900000.0,         0.0002684,   0.000000,      1}, // 9  "QCD Pythia6",                                  // 9
  {43.0,                1,           4225916,       1}, // 10 "WW Pythia6",                                   // 10
  {18.0,                1,           4265243,       1}, // 11 "WZ Pythia6",                                   // 11
  {11439,               0.773,       10461655,      1}, // 12 "W Jets Matching Up Madgraph",                  // 12
  {42352,               0.201,       9956679,       1}, // 13 "W Jets Matching Down Madgraph",                // 13
  {17859,               0.556,       9784907,       1}, // 14 "W Jets Scale Up Madgraph",                     // 14
  {20137,               0.555,       10022324,      1}, // 15 "W Jets Scale Down Madgraph",                   // 15
  {31500.0,             1,           62474300,      1}, // 16 "W Jets Sherpa",                                // 16 
  { 1.0,                1.0,         109996,        1}, // 17 HWW120
  { 1.0,                1.0,         109992,        1}, // 18 HWW130
  { 1.0,                1.0,         109990,        1}, // 19 HWW140
  { 1.0,                1.0,         109993,        1}, // 20 HWW150
  { 2.415,              1.0,         109992,        1}, // 21 HWW160
  { 2.180,              1.0,         109989,        1}, // 22 HWW170
  { 1.846,              1.0,         109325,        1}, // 23 HWW180
  { 1.356,              1.0,         109986,        1}, // 24 HWW190
  {10.4978,             0.108,       109315,        1}, // 25 HWW200
  { 6.624,              0.103,       109992,        1}, // 26 HWW250
  { 4.844,              0.101,       109990,        1}, // 27 HWW300
  { 4.612,              0.0990,      109313,        1}, // 28 HWW350
  { 4.064,              0.0852,      107879,        1}, // 29 HWW400
  { 2.600,              0.0810,      107158,        1}, // 30 HWW450
  { 1.6982,             0.0799,      107169,        1}, // 31 HWW500
  { 1.04,               0.0806,      107870,        1}, // 32 HWW550
  { 0.6533,             0.0818,      108561,        1}, // 33 HWW600
  { 1.0,                1.0,         217674,        1}, //
  { 1.0,                1.0,         216316,        1}, //
  { 1.0,                1.0,         219691,        1}, //
  { 1.0,                1.0,         219727,        1}, //
  { 1.0,                1.0,         219714,        1}, //
  { 1.0,                1.0,         212126,        1}, //
  { 1.0,                1.0,         219703,        1}, //
  { 1.0,                1.0,         213503,        1}, //
  { 1.0,                1.0,         217880,        1}, //
  { 1.0,                1.0,         219128,        1}, //
  { 1.0,                1.0,         217873,        1}, //
  { 1.0,                1.0,         214082,        1}, //
  { 1.0,                1.0,         205271,        1}, //
  { 1.0,                1.0,         214876,        1}, //
  { 1.0,                1.0,         218921,        1}, //
  { 1.0,                1.0,         216326,        1}, //
  { 1.0,                1.0,         214858,        1}, //
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
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_TTbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopS_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopS_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopT_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopT_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopTW_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_STopTW_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_ZpJ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RDQCD_WmunuJets_DataAll_GoldenJSON_2p1invfb.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WW_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WZ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJmatchingup_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJmatchingdown_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJscaleup_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJscaledown_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_WpJsherpa_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH120_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH130_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH140_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH150_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH160_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH170_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH180_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH190_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH200_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH250_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH300_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH350_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH400_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH450_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH500_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH550_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_HWWMH600_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH120_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH130_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH140_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH150_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH160_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH170_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH180_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH190_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH200_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH250_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH300_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH350_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH400_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH450_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH500_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH550_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_mu_VBFHWWMH600_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_WmunuJets_DataAll_GoldenJSON_4p7invfb.root"
};

static const char *El_InFileNames[TotI] = {
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_TTbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopS_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopS_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopT_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopT_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopTW_T_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_STopTW_Tbar_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_ZpJ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RDQCD_WenuJets_DataAll_GoldenJSON_2p1invfb.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WW_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WZ_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJmatchingup_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJmatchingdown_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJscaleup_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJscaledown_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_WpJsherpa_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH120_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH130_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH140_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH150_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH160_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH170_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH180_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH190_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH200_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH250_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH300_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH350_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH400_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH450_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH500_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH550_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_HWWMH600_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH120_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH130_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH140_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH150_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH160_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH170_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH180_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH190_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH200_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH250_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH300_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH350_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH400_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH450_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH500_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH550_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_el_VBFHWWMH600_CMSSW428.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Full2011Data/RDTree/RD_WenuJets_DataAllSingleElectronTrigger_GoldenJSON_4p7invfb.root"
};

