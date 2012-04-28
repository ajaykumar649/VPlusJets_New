void makeATGCGraph() {

  float x10[10]= {-0.25, -0.2, -0.15, -0.1, -0.05, 0.05, 0.1, 0.15, 0.2, 0.25};
  float errx10[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  float Delta_K_gamma_val[10] = {715.286,  712.681, 704.529, 696.965, 
				 692.775, 689.879, 690.957 , 692.173, 
				 691.701, 695.953};
  float Delta_K_gamma_err[10] = {1.958, 1.969, 2.133, 1.837, 1.667,
				 1.748, 1.896, 2.121, 1.754, 1.882 };
  GetCrossSectionChange(Delta_K_gamma_val, Delta_K_gamma_err);


  float Delta_K_Z_val[10] = {793.863, 756.142, 730.164, 712.810, 700.142, 
			     694.202, 696.469, 709.119, 727.232, 755.345};
  float Delta_K_Z_err[10] = {2.141, 1.832, 1.399, 1.700, 2.044, 2.052, 
			     1.700, 1.922, 1.833, 1.981};
  GetCrossSectionChange(Delta_K_Z_val, Delta_K_Z_err);


  float Lambda_Z_val[10] = {788.894, 746.479, 718.517, 700.0, 689.782, 
			    700.448, 720.156, 751.032, 793.941, 842.683};
  float Lambda_Z_err[10] = {2.013, 1.851, 1.831, 1.810, 1.713, 1.751,
			    2.095, 1.977, 1.803, 1.637};
  GetCrossSectionChange(Lambda_Z_val, Lambda_Z_err);


  float Lambda_gamma_val[10] = {706.943, 700.210, 696.097, 691.994, 691.558,  
				692.655, 692.777, 703.369, 711.104,  720.000};
  float Lambda_gamma_err[10] = {1.693, 1.923, 1.912, 2.053, 1.667, 1.568, 
				2.107, 1.878, 1.743, 2.000};
  GetCrossSectionChange(Lambda_gamma_val, Lambda_gamma_err);


  float Delta_g1_val[10] = {697.238, 695.873, 692.746, 692.702, 693.278, 
			    693.544, 693.238, 696.764, 697.837, 700.000};
  float Delta_g1_err[10] = {1.855, 2.016, 1.630, 1.988, 2.005, 1.676, 
			    1.897, 2.096, 1.691, 2.000};
  GetCrossSectionChange(Delta_g1_val, Delta_g1_err);


  TGraphErrors* Delta_K_gamma = new TGraphErrors(10, x10, Delta_K_gamma_val, 
						 errx10, Delta_K_gamma_err);
  Delta_K_gamma->SetMarkerColor(kGreen-2);
  Delta_K_gamma->SetLineColor(kGreen-2);



  TGraphErrors* Delta_K_Z = new TGraphErrors(10, x10, Delta_K_Z_val, 
						 errx10, Delta_K_Z_err);
  Delta_K_Z->SetMarkerColor(2);
  Delta_K_Z->SetLineColor(2);



  TGraphErrors* Delta_g1 = new TGraphErrors(10, x10, Delta_g1_val, 
						 errx10, Delta_g1_err);
  Delta_g1->SetMarkerColor(6);
  Delta_g1->SetLineColor(6);


  TGraphErrors* Lambda_gamma = new TGraphErrors(10, x10, Lambda_gamma_val, 
						 errx10, Lambda_gamma_err);
  Lambda_gamma->SetMarkerColor(4);
  Lambda_gamma->SetLineColor(4);



  TGraphErrors* Lambda_Z = new TGraphErrors(10, x10, Lambda_Z_val, 
						 errx10, Lambda_Z_err);
  Lambda_Z->GetXaxis()->SetTitle("Variation in TGC parameter");
  Lambda_Z->GetYaxis()->SetTitle("Change in cross section");






  gStyle->SetPadLeftMargin(0.18);
  /////////////////////////////////////////////////////////
  TCanvas *can_atgc_xsec  = new TCanvas("can_atgc_xsec",
					    "",500, 500);
  Lambda_Z->Draw("apl");
  Delta_K_Z->Draw("plsame");
  Delta_K_gamma->Draw("plsame");
  Lambda_gamma->Draw("plsame");
  Delta_g1->Draw("plsame");

  TLegend* legend2 = new TLegend(0.45,0.6,0.7,0.9);
  legend2->AddEntry( Lambda_Z, "#lambda_{Z}", "lpe");  
  legend2->AddEntry( Delta_K_Z, "#Delta#kappa_{Z}", "lpe");
  legend2->AddEntry( Delta_K_gamma, "#Delta#kappa_{#gamma}", "lpe");
  legend2->AddEntry( Lambda_gamma, "#lambda_{#gamma}", "lpe");  
  legend2->AddEntry( Delta_g1, "#Deltag1", "lpe");

  legend2->SetFillColor(0);
  legend2->Draw();
  cmsPrelim();
  can_atgc_xsec->SaveAs("atgc_xsec.png");
}





////CMS Preliminary label and lumu -- upper left corner
void cmsPrelim()
{
   TLatex latex;
   latex.SetNDC();
   latex.SetTextSize(0.04);

   latex.SetTextAlign(11); // align left
   latex.DrawLatex(0.65,0.96,"CMS preliminary");
}





void GetCrossSectionChange(float y[], float yerr[]) {
  const float xsec0 = 693.;
  int N = 10;

  for (int i=0; i< N; ++i) {
    y[i] = y[i]/xsec0 - 1.0;
    yerr[i] /= xsec0;
  }

}
