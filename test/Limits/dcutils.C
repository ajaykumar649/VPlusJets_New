#include <iostream>
#include <map>
#include <vector>
#include <stdio.h>
#include "TString.h"

struct ProcData_t {
  ProcData_t() : name(""),procindex(-1) {}
  TString name;
  int procindex;
  std::map<TString,double> channels;
  std::map<TString,vector<pair<double,double> > > systrates; /*  TString = systname
								 vector index over channels
								 pair of doubles for two-sided errors
								 first = DOWN, second = UP
							      */
 };

struct ShapeFiles_t {
  ShapeFiles_t(const TString& inproc,
	       const TString& inchan,
	       const TString& infile,
	       const TString& inhist,
	       const TString& inhistwithsyst="") :
    process(inproc),channel(inchan),file(infile),histo(inhist),histo_with_syst(inhistwithsyst) {}
  TString process;
  TString channel;
  TString file;
  TString histo;
  TString histo_with_syst;
};

struct CardData_t {
  int nsigproc;
  int nbackproc;
  ProcData_t data;   // data is identified separately as "observation"
  std::vector<ShapeFiles_t> shapespecs;
  std::vector<ProcData_t> processes;
  std::map<TString,int> pname2index;
  std::map<TString,TString> systematics; // name, pdf function
};

// Histogram manipulation
const double minimum_signal_content=0.01;

// see documentation for higgs combine tool, data card file specification
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit
//
void fmtDataCardFile(int massgev,
		     const CardData_t& cd,
		     const TString& cfgtag="")
{
  using namespace std;

  int nbins= cd.processes[0].channels.size();
  int jmax = cd.processes.size()-1;

#if 0
  TString outdir;
  if (shapesfile.Contains("/"))
    outdir=shapesfile(0,shapesfile.Last('/'));
  else
    outdir=TString(".");
#endif

  TString fname;
  if (nbins==1) {
    TString channame = cd.processes[0].channels.begin()->first;
    fname = Form("./datacard_%s_%s.txt",
		 //outdir.Data(),
		 channame.Data(),cfgtag.Data());
  } else
    fname = Form("./datacard_%s.txt",
		 //outdir.Data(),
		 cfgtag.Data());

  cout << "Writing "<<fname<<endl;

  FILE* dcFile=fopen(fname.Data(),"wt");
  //FILE* dcFile=stdout;

  assert (dcFile);

  fprintf(dcFile,"imax %d  # number of channels\n",nbins);
  fprintf(dcFile,"jmax %d  # number of processes-1\n",jmax);
  fprintf(dcFile,"kmax *  # number of nuisance parameters\n");

  fprintf(dcFile,"--------------------\n");

  // these are the data loops
  fprintf(dcFile,"bin                                ");
  for (map<TString,double>::const_iterator it=cd.data.channels.begin();
       it != cd.data.channels.end();
       it++) {
    fprintf(dcFile,"%12s",it->first.Data());
  }
  fprintf(dcFile,"\nobservation                        ");
  for (map<TString,double>::const_iterator it=cd.data.channels.begin();
       it != cd.data.channels.end();
       it++) {
    //fprintf(dcFile,"%12d",(int)it->second);
    fprintf(dcFile,"%12g",it->second);
  }
  
  fprintf(dcFile,"\n--------------------\n");

  if (cd.shapespecs.size()) {
    for (size_t i = 0; i < cd.shapespecs.size(); i++)
      fprintf(dcFile,"shapes %s %s %s %s %s\n",
	      cd.shapespecs[i].process.Data(),
	      cd.shapespecs[i].channel.Data(),
	      cd.shapespecs[i].file.Data(),
	      cd.shapespecs[i].histo.Data(),
	      cd.shapespecs[i].histo_with_syst.Data());

    fprintf(dcFile,"--------------------\n");
  }

  /****************************************
   * YIELDS PER BIN/PROCESS
   ****************************************/

  // Example:
  //--------------------
  //bin           chan1    chan1      chan1  
  //process       Signal   WJets      WW 
  //process         0      1      2  
  //rate           53.44 1898.97  268.52 
  //--------------------

  // these are the signal and background loops
  fprintf(dcFile,"bin                                ");
  for (int j=0; j<=jmax; j++)
    for (map<TString,double>::const_iterator it=cd.processes[j].channels.begin();
	 it != cd.processes[j].channels.end();
	 it++)
      fprintf(dcFile,"%12s",it->first.Data());
  fprintf(dcFile,"\n");
  
  map<TString,ProcData_t>::const_iterator pit;

  fprintf(dcFile,"process                            ");
  for (int j=0; j<=jmax; j++)
    for (int k=0; k<nbins; k++)
      fprintf(dcFile,"%12s",cd.processes[j].name.Data());
  fprintf(dcFile,"\n");

  fprintf(dcFile,"process                            ");
  for (int j=0; j<=jmax; j++)
    for (int k=0; k<nbins; k++)
      fprintf(dcFile,"%12d",cd.processes[j].procindex);
  fprintf(dcFile,"\n");

  fprintf(dcFile,"rate                               ");
  for (int j=0; j<=jmax; j++) 
    for (map<TString,double>::const_iterator it=cd.processes[j].channels.begin();
	 it != cd.processes[j].channels.end();
	 it++)
      fprintf(dcFile,"%12.2f",max(it->second,0.001));
  fprintf(dcFile,"\n");

  fprintf(dcFile,"--------------------\n");

  /****************************************
   * SYSTEMATICS
   ****************************************/

  map<TString,TString>::const_iterator sit;
  for (sit  = cd.systematics.begin();
       sit != cd.systematics.end();
       sit++) {
    const TString& name = sit->first;
    const TString& pdf  = sit->second;
    fprintf(dcFile,"%-32s %5s",name.Data(),pdf.Data());
    for (int j=0; j<=jmax; j++) {
      const ProcData_t& pd = cd.processes[j];
      map<TString,vector<pair<double,double> > >::const_iterator rit = pd.systrates.find(sit->first);
      if (rit == pd.systrates.end()) {
	for (int k=0; k<nbins; k++)
	  fprintf(dcFile,"       -    ");
      } else {
	const vector<pair<double,double> >& rates = rit->second;
	for (int k=0; k<nbins; k++) {
	  pair<double,double> rate = rates.at(k);
	  if (rate.second == 0.0)        // expect the UP (second) to contain single-sided error values
	    fprintf(dcFile,"       -    ");
	  else if (rate.first == 0.0)
	    fprintf(dcFile,"  %7.4g   ",rate.second);
	  else
	    fprintf(dcFile," %g/%g ",rate.first,rate.second);
	}
      }
    }
    fprintf(dcFile,"\n");
  }

  fclose(dcFile);
}                                                               // fmtDataCardFile
