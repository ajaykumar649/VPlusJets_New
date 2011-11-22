// -*- mode: C++ -*-

#ifndef RooWjjFitterParams_h
#define RooWjjFitterParams_h

#include "TString.h"

class RooWjjFitterParams {
public:
  RooWjjFitterParams();
  RooWjjFitterParams(RooWjjFitterParams const& other);
  virtual ~RooWjjFitterParams() { }

  TString MCDirectory;
  TString WpJDirectory;
  TString DataDirectory;
  TString NewPhysicsDirectory;
  TString QCDDirectory;
  int nbins;
  double minMass;
  double maxMass;
  int njets;
  double JES_scale1;
  double JES_scale2;
  bool includeMuons;
  bool includeElectrons;
  bool truncRange;
  double minTrunc;
  double maxTrunc;
  double minWmass;
  double maxWmass;
  bool doNewPhysics;
  bool doAllPlots;
  TString initParamsFile;
  TString constraintParamsFile;
  TString cuts;
  TString var;
  TString treeName;
  bool toyWpJ;
  TString muonData;
  TString electronData;
  double intLumi;
  bool constrainDiboson;

  ///External Parameters
  bool useExternalMorphingPars;
  double e_fSU;
  double e_fMU;

  ///Use for toyDataset Fitting
  bool fitToyDataset;
  TString ToyDatasetDirectory;
  TString toydataFile;

};

RooWjjFitterParams::RooWjjFitterParams() :
  MCDirectory(""), WpJDirectory(""), DataDirectory(""), 
  NewPhysicsDirectory(""),QCDDirectory(""), nbins(24),
  minMass(60.), maxMass(300.), njets(2), JES_scale1(0.),
  JES_scale2(0.), includeMuons(true), includeElectrons(true),
  truncRange(false), minTrunc(130.), maxTrunc(170.), 
  minWmass(65.), maxWmass(95.), doNewPhysics(false),
  doAllPlots(false), initParamsFile(""), constraintParamsFile(""),
  cuts(""), var("Mass2j_PFCor"), treeName("WJet"), toyWpJ(false),
  muonData("WmunuJets_DataAll_GoldenJSON_2p1invfb.root"),
  electronData("WenuJets_DataAll_GoldenJSON_2p1invfb.root"),
  intLumi(2100.), constrainDiboson(true),

  useExternalMorphingPars(false), e_fSU(-100.0), e_fMU(-100.0),
  fitToyDataset(false), ToyDatasetDirectory(""), toydataFile("")
{
}

RooWjjFitterParams::RooWjjFitterParams(RooWjjFitterParams const& other) :
  MCDirectory(other.MCDirectory), WpJDirectory(other.WpJDirectory),
  DataDirectory(other.DataDirectory), 
  NewPhysicsDirectory(other.NewPhysicsDirectory),
  QCDDirectory(other.QCDDirectory),
  nbins(other.nbins), minMass(other.minMass), maxMass(other.maxMass),
  njets(other.njets), includeMuons(other.includeMuons),
  includeElectrons(other.includeElectrons), truncRange(other.truncRange),
  minTrunc(other.minTrunc), maxTrunc(other.maxTrunc),
  minWmass(other.minWmass), maxWmass(other.maxWmass),
  doNewPhysics(other.doNewPhysics), doAllPlots(other.doAllPlots),
  initParamsFile(other.initParamsFile),
  constraintParamsFile(other.constraintParamsFile),
  cuts(other.cuts), var(other.var), treeName(other.treeName),
  toyWpJ(other.toyWpJ), muonData(other.muonData), 
  electronData(other.electronData), intLumi(other.intLumi),
  constrainDiboson(other.constrainDiboson),

  useExternalMorphingPars(other.useExternalMorphingPars), e_fSU(other.e_fSU), e_fMU(other.e_fMU),
  fitToyDataset(other.fitToyDataset), ToyDatasetDirectory(other.ToyDatasetDirectory), toydataFile(other.toydataFile)
{
}

#endif
