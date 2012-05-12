#include <iostream>
#include <stdio.h>
#include <map>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TVectorD.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TString.h"
#include "TKey.h"
#include "TCollection.h"
#include "TROOT.h"
#include "TF1.h"

typedef std::map<TString,std::vector<TH1D *> > SigData_t;

#include "mjjinputs.h"

//================================================================================

void getItAll(SigData_t& m_sigdata,
	      std::map<TString,TGraph*>& m_bkgds)
{
  TString proc;
  TObject *obj;
  const char *objname;

  // get signals
  for(int isig=0; isig<NUMSIG; isig++) {
    TString sigmodel(sigobjnames[isig][0]);
    std::vector<TH1D *> v_chans(NUMCHAN,(TH1D*)0);

    for (int ichan=0; ichan<NUMCHAN; ichan++) {
      TString fname = TString(sigdir) + TString(siginputfiles[ichan]);
      TFile *rootfp = new TFile(fname.Data());
      if (rootfp->IsZombie()) {
	cerr << "Couldn't open file " << siginputfiles[ichan] << endl;
	exit(-1);
      }
      rootfp->cd();
      proc    = Form("Signal%s_%s",sigmodel.Data(),channames[ichan]);
      objname = sigobjnames[isig][1];
      obj     = rootfp->Get(objname);

      if (!obj) {
	cerr << "Couldn't get object " << objname << endl;
	exit(-1);
      }

      cout << "Got process " << proc << endl;
      v_chans[ichan] =  (TH1D *)obj;

    } // channel loop

    m_sigdata[sigmodel] = v_chans;

  } // sig model loop

  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    TString fname = TString(dir) + TString(backdatainputfiles[ichan]);
    TFile *rootfp = new TFile(fname.Data());
    if (rootfp->IsZombie()) {
      cerr << "Couldn't open file " << backdatainputfiles[ichan] << endl;
      exit(-1);
    }

    // get data
    proc    = Form("data_obs_%s", channames[ichan]);
    objname = dataobjname;
    obj     = rootfp->Get(objname);

    if (!obj) {
      cerr << "Couldn't get object " << objname << endl;
      exit(-1);
    } else {
      cout << "Got process " << proc << endl;
      m_bkgds[proc] = (TGraph *)obj;
    }

    // Get background(s)
    proc    = Form("Bckgrndtot_%s", channames[ichan]);
    objname = bkgdobjname;
    obj     = rootfp->Get(objname);
    
    if (!obj) {
      cerr << "Couldn't get object " << objname << endl;
      exit(-1);
    } else {
      cout << "Got process " << proc << endl;
      m_bkgds[proc] = (TGraph *)obj;
    }
  } // channel loop
}                                                                      // getItAll

//================================================================================

const double gaussian_mean_mass_gev   = 150.0;
//const double gaussian_mass_sigma_gev  = 15.0;
const double gaussian_mass_sigma_gev  = 30.0;

void  makeGaussianSignals(SigData_t& m_sigdata)
{
  //std::vector<TH1D *> vgsh(NUMCHAN);
  std::vector<TH1D *> vcdfh(NUMCHAN);

  if( m_sigdata.find("gs") == m_sigdata.end() ) {
    cerr << "Gaussian signal data not found, not making CDF signal!" << endl;
    return;
  }

  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    TH1D *cdfh;

    TString channame(channames[ichan]);
    TString name;

    TH1D * gsh = m_sigdata["gs"].at(ichan);

    assert(gsh) ;

#if 0
    name = "Signalgs_"+channame;
    gsh = (TH1D *)tch->Clone(name.Data());

    assert(gsh);

    gsh->SetTitle("Gaussian signal");
    
    gsh->Reset();

    TF1 *g = (TF1 *)gROOT->GetFunction("gaus");
    g->SetParameters(1,gaussian_mean_mass_gev,gaussian_mass_sigma_gev);
    gsh->FillRandom("gaus",100000);

    // norm to 1pb signal with 1/fb integrated luminosity
    double norm = 1000 * gseffxacc[ichan]/gsh->Integral(0,gsh->GetNbinsX()+1,"width");

    //gsh->Scale(norm/eff_fudgefactor); // kludge: pre-undo the fudge in the next module
    gsh->Scale(norm);

    vgsh[ichan] = gsh;
#endif

    // New CDF bump, same as Gauss but set to CDF (obs/theor)*(LHC theor) = 3.43pb
    cdfh = (TH1D *)gsh->Clone("CDFbump");

    cdfh->Scale(3.43);

    vcdfh[ichan] = cdfh;

    cdfh->Draw();

    gsh->Draw("same");

  } // channel loop

  //m_sigdata["gs"]  = vgsh;
  m_sigdata["cdf"] = vcdfh;
}                                                           // makeGaussianSignals

//================================================================================

void writeSignalHistosForModel(std::vector<TH1D *>& vsd,
			       const TString& sigmodel,
			       TFile *allHistFile)
{
  for (int ichan=0; ichan<NUMCHAN; ichan++) {
    TH1D * sdh = vsd[ichan];

    // Find limit window from gaussian fit to signal peak.
    //
    double wid  = sdh->GetRMS();
    double mean = sdh->GetMean();

    //TCanvas *c1 = new TCanvas(s,s,300,300);
    TFitResultPtr r = sdh->Fit("gaus","QNS","",mean-2.5*wid,mean+2.5*wid);

    cout<<" mean= "<<mean<<", RMS= "<<wid<<", Fit sigma= "<<r->Parameter(2)<<endl;
    //cout<<r->Parameter(0)<<" "<<r->Parameter(1)<<" "<<r->Parameter(2)<<endl;

    TAxis *xax = sdh->GetXaxis();

#if 0
    int lobin = xax->FindFixBin(r->Parameter(1)-2*r->Parameter(2));
    int hibin = xax->FindFixBin(r->Parameter(1)+2*r->Parameter(2));
    sd.sumwinmin = xax->GetBinLowEdge(lobin);
    sd.sumwinmax = xax->GetBinUpEdge(hibin);
#elif 0
    int lobin = xax->FindFixBin(140);   sd.sumwinmin=140; // 1 bin  left,
    int hibin = xax->FindFixBin(170)-1; sd.sumwinmax=170; // 2 bins right
#else
    int lobin = xax->FindFixBin(sumwinmin);
    int hibin = xax->FindFixBin(sumwinmax)-1;
#endif
    int nbins = hibin-lobin+1;

    // for variable binning - all histos must have the same binning per channel
    TVectorD xbins   = TVectorD(sdh->GetNbinsX(),sdh->GetXaxis()->GetXbins()->GetArray());
    TVectorD xwindow = xbins.GetSub(lobin-1,hibin);

    xax->SetRange(lobin,hibin);

    // Copy contents to window-restricted signal histogram
    // and write to output file.
    //
    TString name = Form("Signal%s_%s",sigmodel.Data(),channames[ichan]);
    printf("Booking TH1D(%s,%s,%d,xwindowarray)\n",
	   name.Data(),sdh->GetTitle(),nbins);
    TH1D *signm = new TH1D(name.Data(),
			   sdh->GetTitle(),
			   nbins,
			   xwindow.GetMatrixArray());

    // make copies of the histograms that are restricted to the
    // bin range lobin-hibin
    //
    for (int ibin=lobin; ibin<=hibin; ibin++) 
      signm->SetBinContent((ibin-lobin+1),
			   sdh->GetBinContent(ibin)
			   *sdh->GetBinWidth(ibin)
			   );

    if (!sigmodel.CompareTo("wh"))
      signm->Scale(whsigscaleto);
    else
      signm->Scale(sigscaleto);

    allHistFile->WriteTObject(signm);

  } // channel loop
}                                                     // writeSignalHistosForModel

//================================================================================

void writeDataBackgroundHistosForModel(const std::map<TString,TGraph*>& m_bkgds,
				       const std::vector<TH1D *>& vchans,
				       TFile  *allHistFile)
{
  for (std::map<TString,TGraph*>::const_iterator it = m_bkgds.begin();
       it != m_bkgds.end();
       it++) {
    const TString& name = it->first;

    // determine binning from the signal histogram for this channel
    // - (sigh) have to find it first...
    //
    TString channame = name.Copy().Remove(0,strlen("Bckgrndtot_"));
    TH1D *sigh=(TH1D*)0;
    for (int ichan=0; ichan<NUMCHAN; ichan++) {
      sigh = vchans.at(ichan);
      if (strstr(sigh->GetName(),channame.Data()))
	break;
    }
    assert (sigh);


    // for variable binning - all histos must have the same binning per channel
    TAxis *xax = sigh->GetXaxis();

    TVectorD xbins   = TVectorD(sigh->GetNbinsX(),xax->GetXbins()->GetArray());

    int lobin = xax->FindFixBin(sumwinmin);
    int hibin = xax->FindFixBin(sumwinmax)-1;
    int nbins = hibin-lobin+1;

    TVectorD xwindow = xbins.GetSub(lobin-1,hibin);
    
    printf("Booking TH1D(%s,%s,%d,xwindowarray)\n",
	   name.Data(),name.Data(),nbins);
    TH1D *h = new TH1D(name.Data(),name.Data(),nbins,xwindow.GetMatrixArray());

    for (int ibin=1; ibin <= nbins; ibin++)
      h->SetBinContent(ibin,
		       it->second->Eval(h->GetBinCenter(ibin))
		       * h->GetBinWidth(ibin)
		       );

    allHistFile->WriteTObject(h);
  }

}                                            //  writeDataBackgroundHistosForModel

//================================================================================

void mjjshapes()
{
  SigData_t m_sigdata;
  std::map<TString,TGraph *> m_backgrounds;

  getItAll(m_sigdata,m_backgrounds);

  makeGaussianSignals(m_sigdata);

  std::map<TString,std::vector<TH1D *> >::iterator it;
  for (it  = m_sigdata.begin();
       it != m_sigdata.end();
       it++) {
    TString sigmodel = it->first;
    TString outfname = "mjj-histo-shapes-"+sigmodel+".root";
    
    TFile *allHistFile = new TFile(outfname.Data(), "RECREATE");

    if (allHistFile->IsZombie())  {
      cerr << "Couldn't open output file " << outfname << endl;
      exit(-1);
    }

    writeSignalHistosForModel         (it->second,sigmodel,allHistFile);
    writeDataBackgroundHistosForModel (m_backgrounds,
				       it->second, // for channel binning
				       allHistFile);

    allHistFile->Close();
    delete allHistFile;
  }
}
