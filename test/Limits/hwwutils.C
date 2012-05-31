#include <iostream>
#include <stdio.h>
#include <map>
#include "TH1D.h"

using namespace std;

struct HdataPerMassPt {
  HdataPerMassPt() :
    massgev(0),
    ggHcspb(0),ggHpdfunchi(0),ggHpdfunclo(0),ggHscaleunchi(0),ggHscaleunclo(0),
    vbfcspb(0),vbfpdfunchi(0),vbfpdfunclo(0),vbfscaleunchi(0),vbfscaleunclo(0),
    br2lnujj(0) {}
  double massgev;
  double ggHcspb; // ggH cross section in picobarns
  double ggHpdfunchi;
  double ggHpdfunclo;
  double ggHscaleunchi;
  double ggHscaleunclo;
  double vbfcspb; // vbfH cross section in picobarns
  double vbfpdfunchi;
  double vbfpdfunclo;
  double vbfscaleunchi;
  double vbfscaleunclo;
  double br2lnujj;
  double br2ww;
};

std::map<int,HdataPerMassPt> m_signals; // signal (Higgs) data per mass point

//================================================================================

void readHxsTable(const TString& fname, double scale=1.0)
{
  cout << "Reading cross sections table from " << fname << endl;

  FILE *mapf = fopen(fname.Data(),"r");
  if (!mapf) {
    cerr << "Couldn't open " << fname << endl;
    exit(-1);
  }

  char str[128];
  while (!feof(mapf) && fgets(str,128,mapf)) {
    if (str[0] == '#') continue;
    int mHgev;
    int n = sscanf(str,"%d",&mHgev);
    HdataPerMassPt& hd = m_signals[mHgev];
    if (fname.Contains("vbf")) {
      n += sscanf(str,"%*d %lf %*f %*f %lf %lf %lf %lf",
		  &hd.vbfcspb,
		  &hd.vbfscaleunchi,&hd.vbfscaleunclo,
		  &hd.vbfpdfunchi,&hd.vbfpdfunclo
		  );
      hd.vbfcspb       *= scale;
    } else {
      n += sscanf(str,"%*d %lf %*f %*f %lf %lf %lf %lf",
		  &hd.ggHcspb,
		  &hd.ggHscaleunchi,&hd.ggHscaleunclo,
		  &hd.ggHpdfunchi,&hd.ggHpdfunclo
		  );
      hd.ggHcspb       *= scale;
    }
    if (n != 6) {
      cerr << "Error reading from file " << fname << endl;
      exit(-1);
    }
    hd.massgev = (double)mHgev;
  }
  fclose(mapf);
}                                                                  // readHxsTable

//================================================================================

void readBRtable(const std::string& fname)
{
  cout << "Reading branching ratios table from " << fname << endl;

  FILE *brf = fopen(fname.c_str(),"r");
  if (!brf) {
    cerr << "Couldn't open " << fname << endl;
    exit(-1);
  }

  char str[128];
  while (!feof(brf) && fgets(str,128,brf)) {
    if (str[0] == '#') continue;
    int mHgev;
    double br2lnujj,br2ww;
    int n = sscanf(str,"%d %lf %lf",&mHgev,&br2lnujj,&br2ww);
    if (n != 3) {
      cerr << "Error reading from file " << fname << endl;
      exit(-1);
    }
    m_signals[mHgev].br2lnujj = 2*br2lnujj; // listed as for only one charge lepton
    m_signals[mHgev].br2ww    = br2ww;
  }
  fclose(brf);
}                                                                  // readBRtable

//================================================================================

void makeTheoretUncert4Sig(int imass,
			   const TString& procname,
			   pair<double,double>& pdfunc,   // down/up pair to put in card
			   pair<double,double>& scaleunc) // down/up pair to put in card
{
  int massgev = masspts[imass];

  std::map<int,HdataPerMassPt>::const_iterator it =  m_signals.find(massgev);
  if (it == m_signals.end()) {
    cerr << "Mass " << massgev << "GeV not represented in signal tables." << endl;
    exit(-1);
  }
    
  const HdataPerMassPt& hd = it->second;

  if (procname.Contains("qq") ) { // vbf process
#if 0
    // this version gives two sided inputs to combine
    pdfunc.first    = 1.0 + (hd.vbfpdfunclo/100.0);
    pdfunc.second   = 1.0 + (hd.vbfpdfunchi/100.0);
    scaleunc.first  = 1.0 + (hd.vbfscaleunclo/100.0);
    scaleunc.second = 1.0 + (hd.vbfscaleunchi/100.0);
#else
    // this version takes the geometric mean of low and high to feed as single number
    pdfunc.first    = 0.0;
    pdfunc.second   = sqrt( (1.0+(hd.vbfpdfunchi/100.))/(1.0+(hd.vbfpdfunclo/100.)) );
    scaleunc.first  = 0.0;
    scaleunc.second = sqrt( (1.0+(hd.vbfscaleunchi/100.))/(1.0+(hd.vbfscaleunclo/100.)) );
#endif
    // add signal acceptance effect of PDF variation in quadrature:
    pdfunc.second = 1.0 + sqrt((pdfunc.second-1)*(pdfunc.second-1) +
			       (qqsigaccptsyst[imass]-1)*(qqsigaccptsyst[imass]-1));
  } else { // default gg fusion process
#if 0
    // this version gives two sided inputs to combine
    pdfunc.first    = 1.0 + (hd.ggHpdfunclo/100.0);
    pdfunc.second   = 1.0 + (hd.ggHpdfunchi/100.0);
    scaleunc.first  = 1.0 + (hd.ggHscaleunclo/100.0);
    scaleunc.second = 1.0 + (hd.ggHscaleunchi/100.0);
#else
    // this version takes the geometric mean of low and high to feed as single number
    pdfunc.first    = 0.0;
    pdfunc.second   = sqrt( (1.0+(hd.ggHpdfunchi/100.))/(1.0+(hd.ggHpdfunclo/100.)) );
    scaleunc.first  = 0.0;
    scaleunc.second = sqrt( (1.0+(hd.ggHscaleunchi/100.))/(1.0+(hd.ggHscaleunclo/100.)) );
#endif
    // add signal acceptance effect of PDF variation in quadrature:
    pdfunc.second = 1.0 + sqrt((pdfunc.second-1)*(pdfunc.second-1) +
			       (ggsigaccptsyst[imass]-1)*(ggsigaccptsyst[imass]-1));
  }

}                                                         // makeTheoretUncert4Sig
