{
  gSystem->Load("libPhysicsToolsKinFitter.so");
  gROOT->ProcessLine(".include ../../../");
  gROOT->ProcessLine(".L Resolution.cc++");
  gROOT->ProcessLine(".L ../src/METzCalculator.cc++");
  gROOT->ProcessLine(".L kanaelec.C++");
  gROOT->ProcessLine("kanaelec runover");
  ///Set true/false for isQCD
  gROOT->ProcessLine("runover.myana(0,false)");
}
