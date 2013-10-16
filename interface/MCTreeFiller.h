/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 * Class:   MCTreeFiller
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill W/Z related MC information into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef ElectroWeakAnalysis_VPlusJets_MCTreeFiller_h
#define ElectroWeakAnalysis_VPlusJets_MCTreeFiller_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace ewk {

	class MCTreeFiller {
		public:
			/// specify the name of the TTree, and the configuration for it
			MCTreeFiller(const char *name, TTree* tree, 
					const edm::ParameterSet iConfig );

			/// default constructor
			MCTreeFiller() {};


			/// Destructor, does nothing 
			~MCTreeFiller() {};


			/// To be called once per event to fill the values for jets
			void fill(const edm::Event &iEvent);


		protected:

			/// To be called once per event, to initialize variable values to default
			void init();
			/// Helper function for main constructor
			void SetBranches(); 
			void SetBranch( float* x, std::string name );
			void SetBranch( int* x, std::string name );
			void SetBranch( bool* x, std::string name );
			void SetBranch( float x, std::string name );

			TTree* tree_;
			const char *  name_;
			std::string Vtype_;
			std::string ptype_;
			int pdgIdDau_;
			edm::InputTag mInputBoson;
			edm::InputTag mInputgenParticles;

		private:
			// private data members

			float Photon_pt_gen;
			float H_mass;
			float H_px;
			float H_py;
			float H_pz;
			float H_E;
			float H_Pt;
			float H_Et;
			float H_Eta;
			float H_Phi;
			float H_Vx;
			float H_Vy;
			float H_Vz;
			float H_Y;
			int H_Id;


			int nVectorBosons;
			float V_mass[6];
			float V_px[6];
			float V_py[6];
			float V_pz[6];
			float V_E[6];
			float V_Pt[6];
			float V_Et[6];
			float V_Eta[6];
			float V_Phi[6];
			float V_Vx[6];
			float V_Vy[6];
			float V_Vz[6];
			float V_Y[6];
			int V_Id[6];


			int ngq;
			float gq_mass[20];
			float gq_px[20];
			float gq_py[20];
			float gq_pz[20];
			float gq_E[20];
			float gq_Pt[20];
			float gq_Et[20];
			float gq_Eta[20];
			float gq_Phi[20];
			float gq_Vx[20];
			float gq_Vy[20];
			float gq_Vz[20];
			float gq_Y[20];
			int gq_Id[20];


			float top_px;
			float top_py; 
			float top_pz;
			float  top_E;
			float  top_Pt;
			float  top_Et;
			float  top_Eta;
			float  top_Charge;
			float  top_Theta;
			float  top_Phi;
			float  top_Vx;
			float  top_Vy;
			float  top_Vz;
			float  top_Y;
			int    top_Id;

			float  atop_px;
			float  atop_py;
			float  atop_pz;
			float  atop_E;
			float  atop_Pt;
			float  atop_Et;
			float  atop_Eta;
			float  atop_Charge;
			float  atop_Theta;
			float  atop_Phi;
			float  atop_Vx;
			float  atop_Vy;
			float  atop_Vz;
			float  atop_Y;
			int    atop_Id;


			int l1Charge;
			int l2Charge;

			float l1px;
			float l1py;
			float l1pz;
			float l1E;
			float l1Et;
			float l1Pt;
			float l1Eta;
			float l1Theta;
			float l1Phi;
			float l1Vx;
			float l1Vy;
			float l1Vz;
			float l1Y;

			///////////////////
			float l2px;
			float l2py;
			float l2pz;
			float l2E;
			float l2Pt;
			float l2Et;
			float l2Eta;
			float l2Theta;
			float l2Phi;
			float l2Vx;
			float l2Vy;
			float l2Vz;
			float l2Y;	  

			///////////////////

			int Parton_Charge[2];
			float Parton_px[2];
			float Parton_py[2];
			float Parton_pz[2];
			float Parton_E[2];
			float Parton_Pt[2];
			float Parton_Et[2];
			float Parton_Eta[2];
			float Parton_Theta[2];
			float Parton_Phi[2];
			float Parton_Vx[2];
			float Parton_Vy[2];
			float Parton_Vz[2];
			float Parton_Y[2];
			int Parton_Id[2];

			///////////////////
			float Lepton_px;
			float Lepton_py;
			float Lepton_pz;
			float Lepton_E;
			float Lepton_Pt;
			float Lepton_Et;
			float Lepton_Eta;
			int   Lepton_Charge;
			float Lepton_Theta;
			float Lepton_Phi;
			float Lepton_Vx;
			float Lepton_Vy;
			float Lepton_Vz;
			float Lepton_Y;
			int Lepton_Id;

			///////////////////
			float Met_px;
			float Met_py;
			float Met_pz;
			float Met_E;
			float Met_Pt;
			float Met_Et;
			float Met_Eta;
			int   Met_Charge;
			float Met_Theta;
			float Met_Phi;
			float Met_Vx;
			float Met_Vy;
			float Met_Vz;
			float Met_Y;
			int Met_Id;

			///////////////////

			int tParton_Charge[2];
			float tParton_px[2];
			float tParton_py[2];
			float tParton_pz[2];
			float tParton_E[2];
			float tParton_Pt[2];
			float tParton_Et[2];
			float tParton_Eta[2];
			float tParton_Theta[2];
			float tParton_Phi[2];
			float tParton_Vx[2];
			float tParton_Vy[2];
			float tParton_Vz[2];
			float tParton_Y[2];
			int tParton_Id[2];

			///////////////////
			float tLepton_px;
			float tLepton_py;
			float tLepton_pz;
			float tLepton_E;
			float tLepton_Pt;
			float tLepton_Et;
			float tLepton_Eta;
			int   tLepton_Charge;
			float tLepton_Theta;
			float tLepton_Phi;
			float tLepton_Vx;
			float tLepton_Vy;
			float tLepton_Vz;
			float tLepton_Y;
			int tLepton_Id;

			///////////////////
			float tMet_px;
			float tMet_py;
			float tMet_pz;
			float tMet_E;
			float tMet_Pt;
			float tMet_Et;
			float tMet_Eta;
			int   tMet_Charge;
			float tMet_Theta;
			float tMet_Phi;
			float tMet_Vx;
			float tMet_Vy;
			float tMet_Vz;
			float tMet_Y;
			int tMet_Id;


			///////////////////
			float tb_px;
			float tb_py;
			float tb_pz;
			float tb_E;
			float tb_Pt;
			float tb_Et;
			float tb_Eta;
			int   tb_Charge;
			float tb_Theta;
			float tb_Phi;
			float tb_Vx;
			float tb_Vy;
			float tb_Vz;
			float tb_Y;
			int tb_Id;


			///////////////////
			float tbbar_px;
			float tbbar_py;
			float tbbar_pz;
			float tbbar_E;
			float tbbar_Pt;
			float tbbar_Et;
			float tbbar_Eta;
			int   tbbar_Charge;
			float tbbar_Theta;
			float tbbar_Phi;
			float tbbar_Vx;
			float tbbar_Vy;
			float tbbar_Vz;
			float tbbar_Y;
			int tbbar_Id;


			///////////////////
			float Hb_px;
			float Hb_py;
			float Hb_pz;
			float Hb_E;
			float Hb_Pt;
			float Hb_Et;
			float Hb_Eta;
			int   Hb_Charge;
			float Hb_Theta;
			float Hb_Phi;
			float Hb_Vx;
			float Hb_Vy;
			float Hb_Vz;
			float Hb_Y;
			int Hb_Id;


			///////////////////
			float Hbbar_px;
			float Hbbar_py;
			float Hbbar_pz;
			float Hbbar_E;
			float Hbbar_Pt;
			float Hbbar_Et;
			float Hbbar_Eta;
			int   Hbbar_Charge;
			float Hbbar_Theta;
			float Hbbar_Phi;
			float Hbbar_Vx;
			float Hbbar_Vy;
			float Hbbar_Vz;
			float Hbbar_Y;
			int Hbbar_Id;

			///////////////////

			int TagQuark_Charge[2];
			float TagQuark_px[2];
			float TagQuark_py[2];
			float TagQuark_pz[2];
			float TagQuark_E[2];
			float TagQuark_Pt[2];
			float TagQuark_Et[2];
			float TagQuark_Eta[2];
			float TagQuark_Theta[2];
			float TagQuark_Phi[2];
			float TagQuark_Vx[2];
			float TagQuark_Vy[2];
			float TagQuark_Vz[2];
			float TagQuark_Y[2];
			int TagQuark_Id[2];

			///////////////////
			//Tag Jet information in EWKW2Jets
			int EWKW_Charge;
			float EWKW_px;
			float EWKW_py;
			float EWKW_pz;
			float EWKW_E;
			float EWKW_Pt;
			float EWKW_Et;
			float EWKW_Eta;
			float EWKW_Theta;
			float EWKW_Phi;
			float EWKW_Vx;
			float EWKW_Vy;
			float EWKW_Vz;
			float EWKW_Y;
			int EWKW_Id;

			int EWKTagQuark_Charge[2];
			float EWKTagQuark_px[2];
			float EWKTagQuark_py[2];
			float EWKTagQuark_pz[2];
			float EWKTagQuark_E[2];
			float EWKTagQuark_Pt[2];
			float EWKTagQuark_Et[2];
			float EWKTagQuark_Eta[2];
			float EWKTagQuark_Theta[2];
			float EWKTagQuark_Phi[2];
			float EWKTagQuark_Vx[2];
			float EWKTagQuark_Vy[2];
			float EWKTagQuark_Vz[2];
			float EWKTagQuark_Y[2];
			int EWKTagQuark_Id[2];

			////////////////////
			//Number of Partons in the W inclusive sample
			int nParton_Winclusive;
	};

} //namespace

#endif


