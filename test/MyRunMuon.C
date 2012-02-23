void MyRunMuon(double myflag=20112250, bool isQCD=false)
{
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  gSystem->Load("libPhysicsToolsKinFitter.so");
  gROOT->ProcessLine(".include ../../../");
  gROOT->ProcessLine(".L Resolution.cc+");
  gROOT->ProcessLine(".L ../src/METzCalculator.cc+");
  gROOT->ProcessLine(".L ../src/QGLikelihoodCalculator.C+");
  gROOT->ProcessLine(".L EffTableReader.cc+");
  gROOT->ProcessLine(".L EffTableLoader.cc+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_170_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_180_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_190_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_200_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_250_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_300_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_350_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_400_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_450_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_500_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_550_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_600_nJ2_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_170_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_180_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_190_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_200_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_250_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_300_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_350_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_400_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_450_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_500_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_550_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_600_nJ3_mu_Likelihood.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_withqg_nJ2_mu_BDT.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_withqg_nJ3_mu_BDT.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_noqg_nJ2_mu_BDT.class.C+");
  gROOT->ProcessLine(".L ClassifierOut/TMVAClassification_noqg_nJ3_mu_BDT.class.C+");
  gROOT->ProcessLine(".L kanamuon.C+");
  gROOT->ProcessLine("kanamuon runover");
  //Set true/false for isQCD
  char mycmd[500]; sprintf(mycmd,"runover.myana(%.d,%i)",myflag, isQCD); cout << "running :: "<<mycmd << endl;
  gROOT->ProcessLine(mycmd);
}
