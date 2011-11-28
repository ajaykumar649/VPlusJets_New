{
  gROOT->ProcessLine(".L RooWjjFitterParams.h+");
  gROOT->ProcessLine(".L RooWjjFitterUtils.cc+");
  gROOT->ProcessLine(".L RooWjjMjjFitter.cc+");
  gROOT->ProcessLine(".x RooWjj4BodyFitter.C+");
}
