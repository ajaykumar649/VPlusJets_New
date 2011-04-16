void AddVariableToTree() {
  AddVariableToTree("ZeeJets_Pt_0to15.root");
  AddVariableToTree("ZeeJets_Pt_15to20.root");
  AddVariableToTree("ZeeJets_Pt_20to30.root");
  AddVariableToTree("ZeeJets_Pt_30to50.root");
  AddVariableToTree("ZeeJets_Pt_50to80.root");
  AddVariableToTree("ZeeJets_Pt_80to120.root");
  AddVariableToTree("ZeeJets_Pt_120to170.root");
  AddVariableToTree("ZeeJets_Pt_170to230.root");
  AddVariableToTree("ZeeJets_Pt_230to300.root");
  AddVariableToTree("ZeeJets_Pt_300_Inf.root");
}


void AddVariableToTree(char* inFile)
{
  const char* treeName = "ZJet";
  const int lumi = 36.0;

  unsigned int index=100;
  const int nMAX = 10;
  Float_t crosssection[nMAX] =  { 4434.0, 145.4, 131.8, 84.38, 32.35, 
				  9.981, 2.760, 0.7241, 0.1946, 0.07627 };
  char* pthatBin[nMAX] =  {"0to15","15to20","20to30","30to50","50to80",
			   "80to120","120to170","170to230","230to300","300_Inf"};
  TString str(inFile);

  for(int i=0; i< nMAX; ++i) {
    if( str.Contains(pthatBin[i]) ) index = i;
  }
  if(index==100) std::cout << "***** Error: incorrect file name" << std::endl;

   TFile fin(inFile, "read");
   TTree* in_tree = (TTree*) fin.Get(treeName);

  Long64_t nentries = in_tree->GetEntries();


   TString prefix = "new_";
   TFile fout( prefix + TString(inFile), "recreate");
   TTree *newtree = in_tree->CloneTree();
   nentries = newtree->GetEntries();


  Float_t weight = lumi * crosssection[index] / nentries;
  TBranch *weight_branch = newtree->Branch("weight", &weight,"weight/F");

  for (Int_t i = 0; i < nentries; i++) {
    newtree->GetEntry(i);
    weight_branch->Fill();
  }

  fout.cd();
  newtree->Write(treeName);
  fout.Close();
}
