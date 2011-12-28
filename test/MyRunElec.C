{
  gSystem->Load("libPhysicsToolsKinFitter.so");
  gROOT->ProcessLine(".include ../../../");
  gROOT->ProcessLine(".L Resolution.cc+");
  gROOT->ProcessLine(".L ../src/METzCalculator.cc+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_170_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_180_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_190_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_200_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_250_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_300_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_350_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_400_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_450_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_500_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_550_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_600_nJ2_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_170_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_180_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_190_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_200_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_250_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_300_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_350_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_400_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_450_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_500_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_550_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_600_nJ3_el_Likelihood.class.C+");
  gROOT->ProcessLine(".L kanaelec.C+");
  gROOT->ProcessLine("kanaelec runover");
  ///Set true/false for isQCD
  gROOT->ProcessLine("runover.myana(-104,false)");
}
