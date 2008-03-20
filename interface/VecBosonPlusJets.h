#ifndef VecBosonPlusJetsIla_h
#define VecBosonPlusJetsIla_h
// -*- C++ -*-
//
// Package:    VecBosonPlusJets
// Class:      VecBosonPlusJets
// 
/**\class VecBosonPlusJets VecBosonPlusJets.cc ElectroWeakAnalysis/VecBosonPlusJets/src/VecBosonPlusJets.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ilaria Segoni
//         Created:  Thu May 24 15:59:54 CEST 2007
// $Id$
//
//

#include <memory>
#include <iostream>
#include <string>
#include <math.h>
#include <map>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"



// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

// system include files

#include "PhysicsTools/RecoCandAlgos/interface/CandidateProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"

///Jets
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

///MET
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"


namespace IlaVecBos{

 const unsigned int MaxPossibleCand = 5;
}

class TFile;
class TH1D;
class TH2D;


class VecBosonPlusJets : public edm::EDAnalyzer {
   public:
      explicit VecBosonPlusJets(const edm::ParameterSet&);
      ~VecBosonPlusJets();


   private:


      virtual void beginJob(const edm::EventSetup&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();

      void bookProcessIDHistoes();

      void PlotsBeforeSelection(const edm::Event& iEvent);
      bool doZed(const edm::Event& iEvent);
      bool doW(const edm::Event& iEvent);
      bool doMET(const edm::Event& iEvent);
      bool doRecoJets(const edm::Event& iEvent);

      void fillHistoes();
    

	///CONFIGURABLE MEMBER DATA
	
	std::string soupType_;
	
	edm::InputTag  Wmuons_;
	edm::InputTag  ZCand_; 
	edm::InputTag  ZCandSameSignPlus_; 
	edm::InputTag  ZCandSameSignMinus_;
	edm::InputTag  triggerResultsTag_;
	
	bool checkOtherRecoTypes;
	std::string Zsrc_string;
	bool useGlobalWeight;
	bool UseBestCandidate;

	edm::InputTag  DaughterIso1_;
	edm::InputTag  DaughterIso2_;

	std::string    outputHistoFileName;
	bool verbose; bool WSelection; bool ZSelection; bool METSelection; bool JetSelection;
	
	uint32_t jetAlgos;
	bool jetExclusive;
	unsigned int jetMultiplicityCut;
	unsigned int maxCandidatesCut;
	
	edm::InputTag  _src_met;    
	edm::InputTag  _src_gen_met;	
	edm::InputTag  _src_gen_met_nonu;

	double metMinCut;
	double metMaxCut;

      // STANDARD MEMBER DATA

	std::vector<const reco::Candidate *> ZCandidates; 
	unsigned int NumberoOfZedCand;
	static bool foundZCandidate[3];
	static float TypeOfZCounter[3];
	uint32_t ZRecoFlagNumber;
	std::vector<double> ZMassRegion;

	edm::Handle<CaloMETCollection> calo;  
	//edm::Handle<CaloMETCollection> recmetMu;
	edm::Handle<GenMETCollection> genMetColl;  
	edm::Handle<GenMETCollection> genMetCollnoNU; 
	reco::CaloMET   met;
	reco::CaloMET   metMuCorr;
	reco::GenMET genMet;
	reco::GenMET genMetnoNU;
 
	std::vector<float> TransWMass;
	unsigned int TotalEventsProcessed;
	unsigned int EventsAfterJetSelection;
	unsigned int EventsAfterZedSelection;
	
	edm::TriggerNames trigNames;
	edm::Handle<edm::TriggerResults> hltresults;
	unsigned int numTriggers;

	edm::Handle<CandDoubleAssociations> DaughterIso[2];
	std::vector<double> LeptonLegsPtCut;
	std::vector<double> LeptonLegsEtaCut;

	std::vector<std::string> src_jet;
	double jetPtCut;
	double jetEtaMinCut;
	double jetEtaMaxCut;
	edm::Handle<CaloJetCollection> recoJets;  
        std::map<std::string, std::vector<const reco::CaloJet *> > highPtJets;
	
	
	double EventWeight; 
	float EventWeightFloat;
	int processIDInt;

      // STANDARD MEMBER DATA & HISTOGRAMS

     	TFile*	  fOutputFile ;
	Float_t   WNumberofCandidates;
	Float_t	  JetMultiplicity;
	Float_t   processIDIntOriginal;
	Float_t   caloMET,caloMETx,caloMETy;


        
	TH1D*       ProcessIDBeforeCuts;  
 	
     	std::map<int, TH1D*>  NumberOfWCandidates;
	std::map<int, TH1D*>  WTransvMass;  
 
     	std::map<int, TH1D*>  ZCandidatesMass;
     	std::map<int, TH1D*>  ZCandidatesPt;
     	std::map<int, TH1D*>  NumberOfZCandidates;
     	
     	std::map<int, TH1D*>  muPt[2];
     	std::map<int, TH1D*>  muEta[2];
     	std::map<int, TH1D*>  muPhi[2];
     	std::map<int, TH1D*>  muIso[2];

	std::map<int, TH1D*>  TypeOfZCandidates;

     	std::map<int, TH1D*>  MET;
     	std::map<int, TH1D*>  METx;
     	std::map<int, TH1D*>  METy;

      	std::map<int, TH1D*>  JetMult;     	 
     	std::map<int, TH1D*>  JetPTAll;    	 
     	std::map<int, TH1D*>  JetEtaAll;   	 
     	std::map<int, TH1D*>  JetPhiAll;   	 
     	std::map<int, TH2D*>  JetEtaVSPTAll;	 
};


bool VecBosonPlusJets::foundZCandidate[3];
float  VecBosonPlusJets::TypeOfZCounter[3];

#endif
