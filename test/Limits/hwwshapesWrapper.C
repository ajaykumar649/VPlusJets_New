void hwwshapesWrapper(const TString& arg1,
		      const TString& arg2)
{
  gROOT->ProcessLine(".L hwwshapes.C+");
  hwwshapes(arg1,arg2);
}
