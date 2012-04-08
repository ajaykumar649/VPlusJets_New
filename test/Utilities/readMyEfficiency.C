#include <iostream>
#include "RooGlobalFunc.h"

void readMyEfficiency() {

  gROOT->ProcessLine(".L EffTableReader.cc+");
  gROOT->ProcessLine(".L EffTableLoader.cc+");

  const std::string fDir = "/uscms/home/kalanand/cms/documents/notes/AN-11-266/trunk/";
   EffTableLoader effEle(fDir + "eleEffsHLTEle2jPfMht_data_LWA_Ele.txt");
   EffTableLoader effJ30(fDir + "eleEffsHLTEle2jPfMht_data_LWA_Jet30.txt");
   EffTableLoader effJ25NoJ30(fDir + "eleEffsHLTEle2jPfMht_data_LWA_Jet25Not30.txt");
   EffTableLoader effMHT(fDir + "eleEffsHLTEle2jPfMht_data_LWA_PfMht.txt");


   // ------- Example: Read efficiency for three different events I picked randomly ---
   float ele_pt1 = 32., ele_eta1 = 0.6, j1_pt1 = 89., j1_eta1 = -0.9, j2_pt1 = 48., j2_eta1 = 1.8, met1 = 41.;
   float ele_pt2 = 42., ele_eta2 = -1.2, j1_pt2 = 62., j1_eta2 = 1.3, j2_pt2 = 56., j2_eta2 = -0.4, met2 = 52.;
   float ele_pt3 = 50., ele_eta3 = 0.8, j1_pt3 = 58., j1_eta3 = 0.1, j2_pt3 = 49., j2_eta3 = 0.0, met3 = 45.;


   // ======== first print out efficiency for online electrons cuts
   std::cout << "ele eff(" << ele_pt1 << "," << ele_eta1 << ") = " 
             <<  effEle.GetEfficiency( ele_pt1, ele_eta1 )  
             << "  +-  " << effEle.GetError( ele_pt1, ele_eta1 )  << std::endl;

   std::cout << "ele eff(" << ele_pt2 << "," << ele_eta2 << ") = " 
             <<  effEle.GetEfficiency( ele_pt2, ele_eta2 )  
             << "  +-  " << effEle.GetError( ele_pt2, ele_eta2 )  << std::endl;

   std::cout << "ele eff(" << ele_pt3 << "," << ele_eta3 << ") = " 
             <<  effEle.GetEfficiency( ele_pt3, ele_eta3 )  
             << "  +-  " << effEle.GetError( ele_pt3, ele_eta3 )  << std::endl;


   // ======== next print out efficiency for online jet1 cut
   std::cout << "jet1 J30 eff(" << j1_pt1 << "," << j1_eta1 << ") = " 
             <<  effJ30.GetEfficiency( j1_pt1, j1_eta1 )  
             << "  +-  " << effJ30.GetError( j1_pt1, j1_eta1 )  << std::endl;
   std::cout << "jet1 J25!J30 eff(" << j1_pt1 << "," << j1_eta1 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j1_pt1, j1_eta1 )  
             << "  +-  " << effJ25NoJ30.GetError( j1_pt1, j1_eta1 )  << std::endl;


   std::cout << "jet1 J30 eff(" << j1_pt2 << "," << j1_eta2 << ") = " 
             <<  effJ30.GetEfficiency( j1_pt2, j1_eta2 )  
             << "  +-  " << effJ30.GetError( j1_pt2, j1_eta2 )  << std::endl;
   std::cout << "jet1 J25!J30 eff(" << j1_pt2 << "," << j1_eta2 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j1_pt2, j1_eta2 )  
             << "  +-  " << effJ25NoJ30.GetError( j1_pt2, j1_eta2 )  << std::endl;


   std::cout << "jet1 J30 eff(" << j1_pt3 << "," << j1_eta3 << ") = " 
             <<  effJ30.GetEfficiency( j1_pt3, j1_eta3 )  
             << "  +-  " << effJ30.GetError( j1_pt3, j1_eta3 )  << std::endl;
   std::cout << "jet1 J25!J30 eff(" << j1_pt3 << "," << j1_eta3 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j1_pt3, j1_eta3 )  
             << "  +-  " << effJ25NoJ30.GetError( j1_pt3, j1_eta3 )  << std::endl;


   // ======== then print out efficiency for online jet2FailJet1 cut
   std::cout << "jet2 J30 eff(" << j2_pt1 << "," << j2_eta1 << ") = " 
             <<  effJ30.GetEfficiency( j2_pt1, j2_eta1 )  
             << "  +-  " << effJ30.GetError( j2_pt1, j2_eta1 )  << std::endl;
   std::cout << "jet2 J25!J30 eff(" << j2_pt1 << "," << j2_eta1 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j2_pt1, j2_eta1 )  
             << "  +-  " << effJ25NoJ30.GetError( j2_pt1, j2_eta1 )  << std::endl;

   std::cout << "jet2 J30 eff(" << j2_pt2 << "," << j2_eta2 << ") = " 
             <<  effJ30.GetEfficiency( j2_pt2, j2_eta2 )  
             << "  +-  " << effJ30.GetError( j2_pt2, j2_eta2 )  << std::endl;
   std::cout << "jet2 J25!J30 eff(" << j2_pt2 << "," << j2_eta2 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j2_pt2, j2_eta2 )  
             << "  +-  " << effJ25NoJ30.GetError( j2_pt2, j2_eta2 )  << std::endl;

   std::cout << "jet2 J30 eff(" << j2_pt3 << "," << j2_eta3 << ") = " 
             <<  effJ30.GetEfficiency( j2_pt3, j2_eta3 )  
             << "  +-  " << effJ30.GetError( j2_pt3, j2_eta3 )  << std::endl;
   std::cout << "jet2 J25!J30 eff(" << j2_pt3 << "," << j2_eta3 << ") = " 
             <<  effJ25NoJ30.GetEfficiency( j2_pt3, j2_eta3 )  
             << "  +-  " << effJ25NoJ30.GetError( j2_pt3, j2_eta3 )  << std::endl;




   // ======== finally print out efficiency for passing online Mht cut
   std::cout << "MET eff(" << met1 << ") = " 
             <<  effMHT.GetEfficiency( met1, 0. )  
             << "  +-  " << effMHT.GetError( met1, 0. )  << std::endl;

   std::cout << "MET eff(" << met2 << ") = " 
             <<  effMHT.GetEfficiency( met2, 0. )  
             << "  +-  " << effMHT.GetError( met2, 0. )  << std::endl;

   std::cout << "MET eff(" << met3 << ") = " 
             <<  effMHT.GetEfficiency( met3, 0. )  
             << "  +-  " << effMHT.GetError( met3, 0. )  << std::endl;



   // ========= At long last, we have total trigger efficiency :-) ========
   std::cout << "Total trigger eff for event 1 = "
             << effEle.GetEfficiency( ele_pt1, ele_eta1 ) * 
      dijetEffFor2jets(effJ30.GetEfficiency(j1_pt1, j1_eta1), 
      effJ25NoJ30.GetEfficiency(j1_pt1, j1_eta1),
      effJ30.GetEfficiency(j2_pt1, j2_eta1), effJ25NoJ30.GetEfficiency(j2_pt1, j2_eta1)) *
      effMHT.GetEfficiency( met1, 0. ) << endl;


   std::cout << "Total trigger eff for event 2 = "
             << effEle.GetEfficiency( ele_pt2, ele_eta2 ) * 
      dijetEffFor2jets(effJ30.GetEfficiency(j1_pt2, j1_eta2), 
      effJ25NoJ30.GetEfficiency(j1_pt2, j1_eta2),
      effJ30.GetEfficiency(j2_pt2, j2_eta2), effJ25NoJ30.GetEfficiency(j2_pt2, j2_eta2)) *
      effMHT.GetEfficiency( met2, 0. ) << endl;

   std::cout << "Total trigger eff for event 3 = "
             << effEle.GetEfficiency( ele_pt3, ele_eta3 ) * 
      dijetEffFor2jets(effJ30.GetEfficiency(j1_pt3, j1_eta3), 
      effJ25NoJ30.GetEfficiency(j1_pt3, j1_eta3),
      effJ30.GetEfficiency(j2_pt3, j2_eta3), effJ25NoJ30.GetEfficiency(j2_pt3, j2_eta3)) *
      effMHT.GetEfficiency( met3, 0. ) << endl;
}


float dijetEffFor2jets(float eff30J1, float eff25n30J1, 
                       float eff30J2, float eff25n30J2) 
{
   return (eff30J1*eff30J2+eff30J1*eff25n30J2+eff30J2*eff25n30J1); 
}



float dijetEffFor3jets(float eff30J1, float eff25n30J1, 
                       float eff30J2, float eff25n30J2, 
                       float eff30J3, float eff25n30J3) 
{
   float T012 = (1. - eff30J1 - eff25n30J1) * eff25n30J2 * eff30J3;
   float T021 = (1. - eff30J1 - eff25n30J1) * eff30J2 * eff25n30J3;
   float T022 = (1. - eff30J1 - eff25n30J1) * eff30J2 * eff30J3;
   float T102 = eff25n30J1 * (1. - eff30J2 - eff25n30J2) * eff30J3;
   float T112 = eff25n30J1 * eff25n30J2 * eff30J3;
   float T120 = eff25n30J1 * eff30J2 * (1. - eff30J3 - eff25n30J3);
   float T121 = eff25n30J1 * eff30J2 * eff25n30J3;
   float T122 = eff25n30J1 * eff30J2 * eff30J3;
   float T201 = eff30J1 * (1. - eff30J2 - eff25n30J2) * eff25n30J3;
   float T202 = eff30J1 * (1. - eff30J2 - eff25n30J2) * eff30J3;
   float T210 = eff30J1 * eff25n30J2 * (1. - eff30J3 - eff25n30J3);
   float T211 = eff30J1 * eff25n30J2 * eff25n30J3;
   float T212 = eff30J1 * eff25n30J2 * eff30J3;
   float T220 = eff30J1 * eff30J2 * (1. - eff30J3 - eff25n30J3);
   float T221 = eff30J1 * eff30J2 * eff25n30J3;
   float T222 = eff30J1 * eff30J2 * eff30J3;

   return (T012 + T021 + T022 + T102 + T112 + T120 + T121 + 
   T122 + T201 + T202 + T210 + T211 + T212 + T220 + T221 + T222);
}
