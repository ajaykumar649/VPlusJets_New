#ifndef HWWUTILS_H
#define HWWUTILS_H

#include <map>
#include <string>
#include "TString.h"

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

  double qcdscale_ggh0j;
  double qcdscale_ggh1in0j;
  double qcdscale_ggh1in1j;
  double qcdscale_ggh2in1j;
  double ueps_0j;
  double ueps_1j;
};

#ifndef HWWUTILS_C
extern std::map<int,HdataPerMassPt> m_signals; // signal (Higgs) data per mass point

void readHxsTable(const TString& fname, double scale=1.0);
void readBRtable(const std::string& fname);
void readJetBinErrTable(const std::string& fname);
int findMasspt2use(int massgev);

void makeTheoretUncert4Sig(int massgev,
			   const TString& procname,
			   std::pair<double,double>& pdfunc,   // down/up std::pair to put in card
			   std::pair<double,double>& scaleunc0, // down/up std::pair to put in card
			   std::pair<double,double>& scaleunc1,
			   std::pair<double,double>& scaleunc2,
			   std::pair<double,double>& scaleunc3,
			   std::pair<double,double>& ueps0j,
			   std::pair<double,double>& ueps1j);
#endif

#endif // HWWUTILS_H
