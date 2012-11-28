#include <iostream>
#include <stdio.h>
#include <map>
#include "TH1D.h"
#include "TGraph.h"
#include "TFile.h"
#include "RooCurve.h"

#include "hwwinputs.h"
#include "hwwutils.C"

using namespace std;

struct DataBack_t {
  TGraph *data;
  RooCurve *backnm;
  RooCurve *backup;
  RooCurve *backdn;
};

DataBack_t databack;

//================================================================================

void determineBinning(TH1    *inhist,
		      double *xmin,
		      double *xmax,
		      int    *binwidth)
{
  *xmin = inhist->GetXaxis()->GetXmin();
  *xmax = inhist->GetXaxis()->GetXmax();
  *binwidth = (int)((*xmax)-(*xmin))/inhist->GetNbinsX();
}

//================================================================================

void writesig(TFile *allHistFile,
	      TH1   *hggHnom,
	      TH1   *hggHup,
	      TH1   *hggHdn,
	      int    massgev,
	      int    ichan,
	      double xmin,
	      double xmax,
	      int    binwidth)
{
  //int rebin = binwidth/5; // assumes 5GeV binning on higgs histos
  int rebin = 1;
  int nbins = (int)(xmax-xmin)/binwidth;

  assert(nbins);

  bool iselectron = (channames[ichan][ELORMUCHAR] == 'e');

  TString name    = Form("ggH_%s_Mass_%d",channames[ichan],massgev);
  TH1D   *hggHout = new TH1D(name,name,nbins,xmin,xmax);

#if 1
  cout
    <<"M="<<massgev<<",xmin="<<xmin<<",xmax="<<xmax<<",binw="<<binwidth
    <<",rebin="<<rebin<<",nbins="<<nbins<<endl;
#endif

  std::map<int,HdataPerMassPt>::const_iterator it =  m_signals.find(massgev);
  if (it == m_signals.end()) {
    cerr << "Mass " << massgev << "GeV not represented in signal tables." << endl;
    exit(-1);
  }
    
  const HdataPerMassPt& hd = it->second;

  double intlumipbinv = (iselectron ? intlumipbinv_el:intlumipbinv_mu);

  // normalize signal histograms to their respective sigma x BR x lumi
  //double ovflwcor = hin->Integral(1,hin->GetNbinsX())/hin->Integral(0,hin->GetNbinsX()+1);
  double norm     = 
    (hd.ggHcspb + hd.vbfcspb)* // VBF contribution ~accounted for by scaling ggH output
    //ovflwcor*
    global_scale* 
    intlumipbinv*
    scaleBRforTau*
    hd.br2lnujj / 2; // DIV 2 because Jake divides Ngen by two!!
    
  printf("norm=%f * %f * %f\n", (hd.ggHcspb+hd.vbfcspb), intlumipbinv, hd.br2lnujj);
  hggHnom->Scale(norm);

  // make copies of the histograms that are restricted to the bin range lobin-hibin
  int lobin = hggHnom->FindFixBin(xmin);
  int hibin = hggHnom->FindFixBin(xmax); // one higher than we want

  printf("xmin,xmax=(%f %f) lobin,hibin=(%d %d)\n", xmin, xmax, lobin, hibin);

  for (int ibin=lobin; ibin<hibin; ) {
    double sumggh=0; // ,sumvbf=0;
    int newbin = 1+((ibin-lobin)/rebin);
    for (int j=0; j<rebin; j++,ibin++) {
      sumggh += binwidth*hggHnom->GetBinContent(ibin);
    }
    hggHout->SetBinContent(newbin,sumggh);
  }
  
  allHistFile->WriteTObject(hggHout);

  if (hggHup && hggHdn) {
    name = Form("ggH_%s_Mass_%d_interf_ggH",
		channames[ichan],massgev);

    TH1D *sigoutup = new TH1D(name+"Up",  name+"Up",  nbins,xmin,xmax);
    TH1D *sigoutdn = new TH1D(name+"Down",name+"Down",nbins,xmin,xmax);

    hggHup->Scale(norm);
    hggHdn->Scale(norm);

    for (int ibin=lobin; ibin<hibin; ) {
      double sumgghup=0, sumgghdn=0; // ,sumvbf=0;
      int newbin = 1+((ibin-lobin)/rebin);
      for (int j=0; j<rebin; j++,ibin++) {
	sumgghup += binwidth*hggHup->GetBinContent(ibin);
	sumgghdn += binwidth*hggHdn->GetBinContent(ibin);
      }
      sigoutup->SetBinContent(newbin,sumgghup);
      sigoutdn->SetBinContent(newbin,sumgghdn);
    }
  
    allHistFile->WriteTObject(sigoutup);
    allHistFile->WriteTObject(sigoutdn);
  }

}                                                                      // writesig

//================================================================================

void writeGraph2TH1(TGraph *ingr, TH1 *outh, int binwidth, TFile *fout)
{
  for (int ibin=1; ibin <= outh->GetNbinsX(); ibin++)
    outh->SetBinContent(ibin,
			(double)binwidth *
			global_scale *
			ingr->Eval(outh->GetBinCenter(ibin))
			);

  fout->WriteTObject(outh);
}

//================================================================================

void writeRooCurve2TH1(RooCurve *crv, TH1 *outh, int binwidth, TFile *fout)
{
  for (int ibin=1; ibin <= outh->GetNbinsX(); ibin++)
#if 1
    outh->SetBinContent(ibin,
			(double)binwidth *
			global_scale *
			crv->average(outh->GetXaxis()->GetBinLowEdge(ibin),
				     outh->GetXaxis()->GetBinUpEdge(ibin)
				     )
			);
#else
  outh->SetBinContent(ibin,(double)binwidth * crv->Eval(outh->GetXaxis()->GetBinCenter(ibin)));
#endif

  fout->WriteTObject(outh);
}

//================================================================================

void writedataback(TFile *fout,
		   DataBack_t& dbk,
		   int massgev,
		   int ichan,
		   double xmin,
		   double xmax,
		   int binwidth)
{
  int nbins = (int)(xmax-xmin)/binwidth;
  assert(nbins);

  if (global_scale != 1.0) {
    cout << "\n\nWARNING!! global_scale is set to " << global_scale << endl << endl;
  }

  TString name = Form("data_obs_%s_Mass_%d",channames[ichan],massgev);
  TH1D *data   = new TH1D(name,name, nbins,xmin,xmax);

  name = Form("Bkgrdtot_%s_Mass_%d",channames[ichan],massgev);
  
  TH1D *backnm = new TH1D(name,name, nbins,xmin,xmax);

  name = Form("Bkgrdtot_%s_Mass_%d_CMS_%s_shape_back_%dTeV",
	      channames[ichan],massgev,channames[ichan],beamcomenergytev);

  TH1D *backup = new TH1D(name+"Up",  name+"Up",  nbins,xmin,xmax);
  TH1D *backdn = new TH1D(name+"Down",name+"Down",nbins,xmin,xmax);

  if (BLINDING)
    // write background expectation into data histogram
    writeRooCurve2TH1(dbk.backnm, data, binwidth, fout);
  else
    writeGraph2TH1   (dbk.data,   data,   binwidth, fout);

  writeRooCurve2TH1(dbk.backnm, backnm, binwidth, fout);
  writeRooCurve2TH1(dbk.backup, backup, binwidth, fout);
  writeRooCurve2TH1(dbk.backdn, backdn, binwidth, fout);
}                                                                 // writedataback

//================================================================================

void writeHistosPerMass(int massgev,
			const TString& nametag,
			const TString& dirpar,
			int lochan,
			int hichan)
{
  TString indir, outdir;

  cout << "========================================" << endl;
  cout << "Writing for mass point " << massgev << endl;

  if (dirpar.Length()) {
    indir = outdir = dirpar;
  } else {
    indir = TString(dir);
    outdir = TString(".");
  }

  TFile *fout;
  if (nametag.Length())
    fout = new TFile(Form("%s/hwwlvjj.input_%dTeV-%s-M=%d.root",
			  outdir.Data(),beamcomenergytev,nametag.Data(),massgev),"RECREATE");
  else
    fout = new TFile(Form("%s/hwwlvjj.input_%dTeV-M=%d.root",
			  outdir.Data(),beamcomenergytev,massgev),"RECREATE");

  for (int ichan=lochan; ichan<=hichan; ichan++) {
    TString fname = TString(indir) + "/" + Form(inputfilesfmtstr[ichan],massgev);
    cout << "Reading files from directory: " << indir << endl;
    TFile *fp = new TFile(fname.Data());

    if (fp->IsZombie()) {
      cerr << "Couldn't find root input file " << fname << ", skipping..." << endl;
      continue;
    }

    databack.data   = (TGraph *)fp->Get(dataobjname);
    if (!databack.data) {
      cerr << "Couldn't get " << dataobjname << endl;
      exit(-1);
    }
    databack.backnm = (RooCurve *)fp->Get(bkgdobjname);
    if (!databack.backnm) {
      cerr << "Couldn't get " << bkgdobjname << endl;
      exit(-1);
    }
    TString name=TString(bkgdobjname)+"_up";
    databack.backup = (RooCurve *)fp->Get(name.Data());
    if (!databack.backup) {
      cerr << "Couldn't get " << name << endl;
      exit(-1);
    }
    name=TString(bkgdobjname)+"_down";
    databack.backdn = (RooCurve *)fp->Get(name.Data());
    if (!databack.backdn) {
      cerr << "Couldn't get " << name << endl;
      exit(-1);
    }
    TH1 *ggHsignal = (TH1 *)fp->Get(Form("HWW%d_%s_shape",massgev,channames2[ichan]));
    if (!ggHsignal) {
      cerr << "Couldn't get signal histogram for mass " << massgev << endl;
      exit(-1);
    }
    TH1 *ggHsignalup = (TH1 *)fp->Get(Form("HWW%d_%s_shape_up",massgev,channames2[ichan]));
    if (!ggHsignalup) {
      if (massgev >= 500) {
	cerr << "No 'up' signal histogram for mass " << massgev << ", exiting"<<endl;
	exit(-1); // masses above 500 GeV should have this
      }
    }
    TH1 *ggHsignaldn = (TH1 *)fp->Get(Form("HWW%d_%s_shape_down",massgev,channames2[ichan]));
    if (!ggHsignaldn) {
      if (massgev >= 500) {
	cerr << "No 'down' signal histogram for mass " << massgev << ", exiting"<< endl;
	exit(-1); // masses above 500 GeV should have this
      }
    }

    double xmin,xmax;
    int binwidth;
    determineBinning(ggHsignal,&xmin,&xmax,&binwidth);

    cout<<"Binning for mass "<<massgev<<": "<<xmin<<","<<xmax<<","<<binwidth<<endl;

    writesig      (fout,ggHsignal,ggHsignalup,ggHsignaldn,massgev,ichan,
		   xmin,xmax,binwidth);
    writedataback (fout,databack,massgev,ichan,
		   xmin,xmax,binwidth);
  }

  fout->Close();

#if 0
  // print uncertainty factor for mass
  HdataPerMassPt& hd = m_signals[massgev];
  float unc = std::max(hd.ggunchi,fabs(hd.ggunclo)); // take the larger of the two.
  unc /= 100.0;
  unc += 1.0;
  printf("Mass %d Sigxsec lnN %5.3f\n",massgev,unc);
#endif
}                                                            // writeHistosPerMass

//================================================================================

void hwwshapes(const TString& nametag = "",
	       const TString& dirpar = "",
	       int lomass=masspts[0],
	       int himass=masspts[NUMMASSPTS-1],
	       int lochan=0,
	       int hichan=NUMCHAN-1)
{
  readHxsTable   (Form("ggHtable%dtev.txt",beamcomenergytev));
  readHxsTable   (Form("vbfHtable%dtev.txt",beamcomenergytev)); // , scalefrom7to8tev);
  readBRtable    ("twikiBRtable.txt");

  // Get all inputs
  cout << "lomass="<<lomass<<",himass="<<himass<<",lochan="<<lochan<<",hichan="<<hichan<<endl;
  for (int imass=0; imass<NUMMASSPTS; imass++) {
    int mass = masspts[imass];
    if (mass >= lomass &&
	mass <= himass)
      writeHistosPerMass(mass, nametag, dirpar,lochan,hichan);
  }
#ifdef DO_INTERP
  for (int imass=0; ; imass++) {
    int mass = interpolatedmasspts[imass];
    if (mass < 0) break;
    if (mass >= lomass &&
	mass <= himass)
      writeHistosPerMass(mass, nametag, dirpar,lochan,hichan);
  }
#endif
}
