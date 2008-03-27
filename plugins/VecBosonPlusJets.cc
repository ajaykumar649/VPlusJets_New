
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
// $Id: VecBosonPlusJets.cc,v 1.2 2008/03/20 18:04:07 segoni Exp $
//
//


// system include files
#include "ElectroWeakAnalysis/VPlusJets/interface/VecBosonPlusJets.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"



VecBosonPlusJets::VecBosonPlusJets(const edm::ParameterSet& iConfig)
{
 if(verbose) std::cout<<"In VecBosonPlusJets::VecBosonPlusJets"<<std::endl;


  
  useGlobalWeight = iConfig.getParameter<bool>( "useGlobalWeight" );  
  EventWeight=1.0 ;
  processIDInt=0;
  processIDIntOriginal=0;
  numTriggers=0;
  selectStage[0]="NoFilter";
  selectStage[1]="EWKFilter";
  selectStage[2]="JetFilter";


  EventWeight=iConfig.getParameter<double>("GlobalEventWeight");
 
  soupType_ = iConfig.getParameter<std::string>( "soupType" ); 
  outputHistoFileName = iConfig.getParameter<std::string>( "hostoRootName" ); 
  verbose = iConfig.getParameter<bool>( "verbose" ); 
  
  WSelection  =iConfig.getParameter<bool>( "RunWSelection" );  
  ZSelection  =iConfig.getParameter<bool>( "RunZSelection" ); 
  METSelection=iConfig.getParameter<bool>( "RunMETSelection" ); 
  JetSelection=iConfig.getParameter<bool>( "RunJETSelection" ); 
  triggerResultsTag_ = iConfig.getParameter<edm::InputTag>("TriggerResults");
  
  ///Initialization fo Vec Boson
  Wmuons_ = iConfig.getParameter<edm::InputTag>( "Wmuons_src" ); 
  ZCand_    = iConfig.getParameter<edm::InputTag>( "ZCand" );
  UseBestCandidate= iConfig.getParameter<bool>( "UseZBestCandidate" );

  ZRecoFlagNumber=iConfig.getUntrackedParameter<uint32_t>("ZFlag_to_set"); 
  for(int ii=0;ii<3;++ii) foundZCandidate[ii]=false;
  checkOtherRecoTypes   =  iConfig.getParameter<bool>( "checkOtherRecoTypes" );
  ZMassRegion=iConfig.getParameter<std::vector<double> >( "ZMassRegion" );
  if(verbose) std::cout<<"ZMassRegion: "<<ZMassRegion[0]<<" " <<ZMassRegion[1]<<std::endl;
  
  ///initialization forMET:
  _src_met    = iConfig.getParameter<edm::InputTag>( "src_met" ); 
  
  
  _src_gen_met    = iConfig.getParameter<edm::InputTag>( "src_gen_met" ); 
  _src_gen_met_nonu    = iConfig.getParameter<edm::InputTag>( "src_gen_met_nonu" ); 

  metMinCut= iConfig.getUntrackedParameter<double>( "minMET",5 );
  metMaxCut= iConfig.getUntrackedParameter<double>( "maxMET" ,10000);

  ///Initialization for Jets
  jetAlgos=iConfig.getUntrackedParameter<uint32_t>("jet_algos",1);
  src_jet.clear();
  for(unsigned int index=0;index<jetAlgos;++index){
	char algoName[20];
	sprintf(algoName,"src_jet_%d",index);
	src_jet.push_back(iConfig.getParameter<std::string>(algoName) ) ;
  }
  
  jetExclusive=iConfig.getParameter<bool>( "ExclusiveJetMult" );  
  jetMultiplicityCut=iConfig.getUntrackedParameter<uint32_t>( "jet_mult",2) ;
  maxCandidatesCut=iConfig.getUntrackedParameter<uint32_t>( "maxCandidatess",IlaVecBos::MaxPossibleCand) ;
  if(maxCandidatesCut>IlaVecBos::MaxPossibleCand) std::cout<<"Correct maxCandidatess to be less than "<<IlaVecBos::MaxPossibleCand<<std::endl;
  jetPtCut=iConfig.getUntrackedParameter<double>( "jet_pt",60) ;
  jetEtaMinCut=iConfig.getUntrackedParameter<double>( "jet_eta_min",0) ;
  jetEtaMaxCut=iConfig.getUntrackedParameter<double>( "jet_eta_max",2) ;
  
  ///Initialization for Muons
  DaughterIso1_    = iConfig.getParameter<edm::InputTag>( "isolation1" ); 
  DaughterIso2_    = iConfig.getParameter<edm::InputTag>( "isolation2" ); 
  LeptonLegsPtCut    = iConfig.getParameter<std::vector<double> >( "LeptonsPtCut" );
  LeptonLegsEtaCut    = iConfig.getParameter<std::vector<double> >( "LeptonsEtaCut" );

 


}


VecBosonPlusJets::~VecBosonPlusJets()
{
 if(verbose) std::cout<<"In VecBosonPlusJets::~VecBosonPlusJets"<<std::endl;
}



void VecBosonPlusJets::bookProcessIDHistoes(){
 if(verbose) std::cout<<"In VecBosonPlusJets::bookProcessIDHistoes"<<std::endl;

    fOutputFile ->cd();
    char histoid[50];
    
 
         
    for(unsigned int selectionTier=0;selectionTier<3;++selectionTier){	
	sprintf(histoid,"WTransvMass_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	WTransvMass[selectionTier][processIDInt] = new TH1D(histoid, "WTransvMass",200,0.,200.);

    	sprintf(histoid,"NumberOfWCandidates_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
    	NumberOfWCandidates[selectionTier][processIDInt] = new TH1D(histoid, "NumberOfWCandidates",10,0.5,10.5);

   
    	sprintf(histoid,"TypeofZReco_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
    	TypeOfZCandidates[selectionTier][processIDInt] = new TH1D(histoid, "TypeofZReco",3,0.5,3.5);

    	sprintf(histoid,"NumberOfZCandidates_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
    	NumberOfZCandidates[selectionTier][processIDInt] = new TH1D(histoid, "NumberOfZCandidates",10,0.5,10.5);
          
    	sprintf(histoid,"ZCandidate_All_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
    	ZCandidatesMass[selectionTier][processIDInt]=  new TH1D(histoid, histoid,120,0.,180.);
    
    	sprintf(histoid,"ZCandidatePt_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
    	ZCandidatesPt[selectionTier][processIDInt]  = new TH1D(histoid, "Z(mu mu) candidate Transverse Momentum",400,0.,400.);
 

    	for(unsigned int muIndex=0; muIndex<2; ++muIndex){
	
	  
		sprintf(histoid,"Muon_%d_FromZ_Pt_Proc%d_%s",muIndex,processIDInt,selectStage[selectionTier].c_str());
		muPt[selectionTier][muIndex][processIDInt] = new TH1D(histoid, histoid,80,0.0,200.0);
	  
		sprintf(histoid,"Muon_%d_FromZ_Eta_Proc%d_%s",muIndex,processIDInt,selectStage[selectionTier].c_str());
		muEta[selectionTier][muIndex][processIDInt] = new TH1D(histoid, histoid,30,-3.0,3.0);
	 
		sprintf(histoid,"Muon_%d_FromZ_Phi_Proc%d_%s",muIndex,processIDInt,selectStage[selectionTier].c_str());
		muPhi[selectionTier][muIndex][processIDInt] = new TH1D(histoid, histoid,30,-3.15,3.15);

		sprintf(histoid,"Muon_%d_FromZ_Isolation_Proc%d_%s",muIndex,processIDInt,selectStage[selectionTier].c_str());
		muIso[selectionTier][muIndex][processIDInt] = new TH1D(histoid, histoid,3000,0,10000);

	}

   
	sprintf(histoid,"Reco MET_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	MET[selectionTier][processIDInt]=new TH1D(histoid, "Reco METt" ,200,0.0,200.0);
	sprintf(histoid,"Reco METx_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	METx[selectionTier][processIDInt]=new TH1D(histoid, "Reco METxx",400,-200.0,200.0);
	sprintf(histoid,"Reco METy_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	METy[selectionTier][processIDInt]=new TH1D(histoid, "Reco METyy",400,-200.0,200.0); 

	sprintf(histoid,"HigPtJetsPT_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	JetPTAll[selectionTier][processIDInt]  = new TH1D(histoid, "JetPTAll",400,0.0,400.0);
	sprintf(histoid,"AHigPtJetsEta_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	JetEtaAll[selectionTier][processIDInt] = new TH1D(histoid, "JetEta",100,-3.0,3.0);
	sprintf(histoid,"HigPtJetsPhi_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	JetPhiAll[selectionTier][processIDInt] = new TH1D(histoid, "JetPhi",100,-3.2,3.2);
	sprintf(histoid,"HigPtJetsMultiplicity_Proc%d_%s",processIDInt,selectStage[selectionTier].c_str());	    
	JetMult[selectionTier][processIDInt]   = new TH1D(histoid, "JetMultiplicity",31,-0.5,30.5);

   }
}


// ------------ method called to for each event  ------------
void
VecBosonPlusJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  if(verbose) std::cout<<"In VecBosonPlusJets::analyze, event: "<< iEvent.id() <<std::endl;
  

processIDInt =0;

if(!useGlobalWeight){
	
	edm::Handle< double> weightHandle;	 
        iEvent.getByLabel ("CSA07weights","weight", weightHandle); 
	EventWeight = * weightHandle ;
	
	int NotAlpgen=soupType_.compare("ALPGEN"); //Returns false if equal
	
	if(NotAlpgen){
		///For PYTHIA SOUPS:
		edm::Handle< int > genProcessID;
		iEvent.getByLabel( "genEventProcID", genProcessID );
		float processID = *genProcessID;
		processIDInt = (int) processID;
		processIDIntOriginal=processIDInt;
	}else{
		///FOR ALPGEN SOUPS:
		edm::Handle< int > AlpgenID_;
		iEvent.getByLabel( "CSA07weights","AlpgenProcessID", AlpgenID_ );
		processIDIntOriginal = *AlpgenID_;    
		if ( processIDIntOriginal<1500) 			      processIDInt =0; //W+Jets
		if ((processIDIntOriginal>1999) & (processIDIntOriginal<2020))  processIDInt =1; //Z+Jets
		if ( processIDIntOriginal>2500) 			      processIDInt =2; //ttbar+Jets
	}
	
}

if(MET[0].find(processIDInt)==MET[0].end()){
	this->bookProcessIDHistoes();
}	

  ///Not used yet
 iEvent.getByLabel(triggerResultsTag_,hltresults);
 if(hltresults.isValid()){
	trigNames.init((edm::TriggerResults&)*hltresults);
	numTriggers =   trigNames.size();
 }else{
	std::cout << "NO TRIGGER INFO AVAILABLE" << std::endl;
 }
	
///// MET

  iEvent.getByLabel(_src_met, calo); 
  const CaloMETCollection * caloMETCol = calo.product();
  met = caloMETCol->front();
  
  caloMET  = met.pt();
  caloMETx = met.px();
  caloMETy = met.py();


 
///////

  this->fillHistoes(0);
 
  foundZCandidate[ZRecoFlagNumber]=false;
  if(checkOtherRecoTypes){
	for(unsigned int previousCandidateTypes = 0; previousCandidateTypes<ZRecoFlagNumber;
	 ++previousCandidateTypes){
		if(foundZCandidate[previousCandidateTypes]) return;
  	}
  }
  
   
     
	
  if(ZSelection){
	  if( !this->doZed(iEvent) ) return;
  }
     
    

  if(METSelection){
	if (!this->doMET(iEvent) ) return;
  }
  
  if(WSelection){
	if( !this->doW(iEvent) ) return;
  }



  this->fillHistoes(1);


  if(JetSelection){	
	  if (!this->doRecoJets(iEvent) ) return;
  }  
 


  this->fillHistoes(2);
 
  foundZCandidate[ZRecoFlagNumber]=true;

}



// ------------ method called once each job just before starting event loop  ------------
void 
VecBosonPlusJets::beginJob(const edm::EventSetup&)
{


  if(verbose) std::cout<<"In VecBosonPlusJets::beginJob "<<std::endl;
  
  
  fOutputFile   = new TFile( outputHistoFileName.c_str(), "RECREATE" ) ;


  for(unsigned int selectionTier=0;selectionTier<3;++selectionTier){
	char histoid[120];
    	sprintf(histoid,"ProcessID_%s",selectStage[selectionTier].c_str());	    
	ProcessIDHisto[selectionTier] = new TH1D(histoid, histoid,4001,-0.5,4000.5);
  }	

  
  NumberoOfZedCand=0;


}




void VecBosonPlusJets::endJob() {

  fOutputFile->Write() ;
  fOutputFile->Close() ;
}



bool VecBosonPlusJets::doW(const edm::Event& iEvent){
   if(verbose) std::cout<<"In VecBosonPlusJets::doW "<<std::endl;
   TransWMass.clear();
   edm::Handle<reco::CandidateCollection> MuonsFromW;
   iEvent.getByLabel(Wmuons_, MuonsFromW);
   double metx=met.px();
   double mety=met.py();
   
   bool WCandFound=false;
   unsigned int countwcand=0;
   if(verbose) std::cout<<"Number of Candidates: "<< MuonsFromW->size() <<std::endl;
   

    for(size_t muonCand = 0; muonCand< MuonsFromW->size(); muonCand++){
           const reco::Candidate & muon = (*MuonsFromW)[muonCand];
           reco::CandidateRef muonRef(MuonsFromW,muonCand);
	   if(muon.pt()>LeptonLegsPtCut[0]){
	   	   double MuPx=muon.px();   
	   	   double MuPy=muon.py();   
	   	   double pxtot2= (MuPx+metx)*(MuPx+metx);
	   	   double pytot2= (MuPy+mety)*(MuPy+mety);
	   	   double tmass= sqrt(pxtot2+pytot2);
	   	   TransWMass.push_back(  tmass );
	   	   ++countwcand;
	   	   WCandFound=true;
	   }

   }	   
   
   if (countwcand>maxCandidatesCut)  WCandFound = false;
  

   return WCandFound;

}


bool VecBosonPlusJets::doZed(const edm::Event& iEvent){
  if(verbose) std::cout<<"In VecBosonPlusJets::doZed "<<std::endl;
   
    ZCandidates.clear();
   
    NumberoOfZedCand=0;   
    edm::Handle<reco::CandidateCollection> AllZCandidates;
    iEvent.getByLabel( ZCand_, AllZCandidates );
    
    NumberoOfZedCand=AllZCandidates->size();
    if(verbose)  std::cout<<"Found "<<NumberoOfZedCand<<" Z Candidate(s)!! "<<  std::endl;
    if(!NumberoOfZedCand) return false;


    iEvent.getByLabel(DaughterIso1_, DaughterIso[0]); //not used yet
    iEvent.getByLabel(DaughterIso2_, DaughterIso[1]); //not used yet


	double PdgZMass=91.3;
	double mass_previous=1000000.;
        reco::Candidate * BestZ;
	bool foundGoodZ=false;
	for(reco::CandidateCollection::const_iterator ZCand = AllZCandidates->begin();ZCand!=AllZCandidates->end();++ZCand){
		int foundGoodLegs = 0;
		for(int dauIndex =0; dauIndex<2;++dauIndex){
			const Candidate * dau = ZCand->daughter(dauIndex);
			if(dau->pt()< LeptonLegsPtCut[dauIndex])  break;  
			if(fabs(dau->eta())> LeptonLegsEtaCut[dauIndex]) break; 
			++foundGoodLegs; 
		}
		if(foundGoodLegs==2){
		
			foundGoodZ = true;
			if(UseBestCandidate){
		 		if( (fabs(PdgZMass-ZCand->mass()))<(fabs(PdgZMass-mass_previous)) ) {
					BestZ = ZCand->clone();
					mass_previous=ZCand->mass();
				}
			}else{
		 		ZCandidates.push_back(ZCand->clone());
			}
		}
	}

	if(UseBestCandidate) {
		if(foundGoodZ) ZCandidates.push_back(BestZ);
	}
	
    return foundGoodZ;
	
}

bool VecBosonPlusJets::doMET(const edm::Event& iEvent){


	if(met.pt()<metMinCut) return false;
	if(met.pt()>metMaxCut) return false;

   	iEvent.getByLabel(_src_gen_met, genMetColl); 
	const GenMETCollection * genMETCol = genMetColl.product();
	genMet = genMETCol->front();

   	iEvent.getByLabel(_src_gen_met_nonu, genMetCollnoNU); 
	const GenMETCollection * genMETColnoNU = genMetCollnoNU.product();
	genMetnoNU = genMETColnoNU->front();

	return true;      
}


bool VecBosonPlusJets::doRecoJets(const edm::Event& iEvent){
   if(verbose) std::cout<<"In VecBosonPlusJets::doRecoJets "<<std::endl;
   highPtJets.clear();
   bool foundJets = false;

   std::vector<std::string>::iterator jetAlgoName;
   for(jetAlgoName=src_jet.begin();jetAlgoName!=src_jet.end();++jetAlgoName){
   	
	iEvent.getByLabel(*jetAlgoName, recoJets); 		
	
	if(recoJets->size()<jetMultiplicityCut) continue;
	unsigned int leadingJets(0);
	std::vector<const reco::CaloJet *> selectedJets;
	for(CaloJetCollection::const_iterator itJet = recoJets->begin();itJet!=recoJets->end();++itJet){
		if(itJet->pt()<jetPtCut)      continue;
 		if(itJet->eta()<jetEtaMinCut) continue;
 		if(itJet->eta()>jetEtaMaxCut) continue;
		selectedJets.push_back(&(*itJet));
		++leadingJets;
  	}     

	if(leadingJets<jetMultiplicityCut)    continue;
	if(jetExclusive) {if(leadingJets>jetMultiplicityCut) continue;}

        foundJets = true;
	highPtJets[*jetAlgoName]=selectedJets;
   }	 

    return foundJets;

}

void VecBosonPlusJets::fillHistoes(const unsigned int selectionTier){
 
 if(verbose) std::cout<<"\n***\nFilling Histograms"<<std::endl;

 ProcessIDHisto[selectionTier]->Fill(processIDIntOriginal,EventWeight);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////                   MET Plots (at all Selection Tiers)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(METSelection){

	if(verbose) std::cout<<"\n***\nFillingMET Histograms"<<std::endl;

	MET[selectionTier][processIDInt]->Fill(caloMET,EventWeight);
	METx[selectionTier][processIDInt]->Fill(caloMETx,EventWeight);
	METy[selectionTier][processIDInt]->Fill(caloMETy,EventWeight);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////                   EWK Plots (from EWK Tier)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 if(selectionTier>0){
  	////W////
  	if(WSelection){
    		WNumberofCandidates=TransWMass.size();
    		NumberOfWCandidates[selectionTier][processIDInt]->Fill(WNumberofCandidates, EventWeight);
    		for(std::vector<float>::iterator Wcand=TransWMass.begin();Wcand!=TransWMass.end();++Wcand){
			WTransvMass[selectionTier][processIDInt]->Fill(*Wcand,EventWeight);
    		}
  	}   
  

  	////Z////  
	if(ZSelection){
		NumberOfZCandidates[selectionTier][processIDInt]->Fill(ZCandidates.size(), EventWeight);
  

		for(std::vector<const reco::Candidate *>::const_iterator it = ZCandidates.begin();it!=ZCandidates.end();++it){
			double DiMuMass= (*it)->mass();
			double Zpt=(*it)->pt();
		

 			unsigned int muCount(0);


			for(reco::Candidate::const_iterator muonFromZ = (*it)->begin(); muonFromZ< (*it)->end(); ++muonFromZ){
				if(muCount>1) {
					std::cout<<"Too Many Muons from Z!!! "<<std::endl;
					break;
				}
				CandidateRef muone = muonFromZ->masterClone().castTo<CandidateRef>();
				double dauIsolation = (*DaughterIso[muCount])[muone];
			
	 			muPt[selectionTier][muCount][processIDInt]->Fill(muonFromZ->pt(),EventWeight);
	  			muEta[selectionTier][muCount][processIDInt]->Fill(muonFromZ->eta(),EventWeight);
	  			muPhi[selectionTier][muCount][processIDInt]->Fill(muonFromZ->phi(),EventWeight);
				muIso[selectionTier][muCount][processIDInt]->Fill(dauIsolation,EventWeight);
				++muCount;
     			}
		
    			ZCandidatesMass[selectionTier][processIDInt]->Fill(DiMuMass,EventWeight); 
    			ZCandidatesPt[selectionTier][processIDInt]->Fill(Zpt,EventWeight); 
   		}
	}


 }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////                   Jets Plots (from Jet Tier)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 if(selectionTier>1){

	if(JetSelection){
		if(verbose) std::cout<<"\n***\nFillingJETS Histograms"<<std::endl;

		std::map<std::string,std::vector<const reco::CaloJet *> >::iterator recoJetCollctnItr;
		for(recoJetCollctnItr=highPtJets.begin();recoJetCollctnItr!=highPtJets.end()
   					;++recoJetCollctnItr){
			std::string jetAlgoName=recoJetCollctnItr->first;
	
			std::vector<const reco::CaloJet *> selectedJets =recoJetCollctnItr->second;
			JetMultiplicity=selectedJets.size();
 			JetMult[selectionTier][processIDInt]->Fill(JetMultiplicity,EventWeight);

			unsigned int jetCount(0);
			std::vector<const reco::CaloJet *>::const_iterator itJet;
			for(itJet=selectedJets.begin(); itJet!=selectedJets.end();++itJet){
				JetPTAll[selectionTier][processIDInt]->Fill((*itJet)->pt(),EventWeight);
       				JetEtaAll[selectionTier][processIDInt]->Fill((*itJet)->eta(),EventWeight);
       				JetPhiAll[selectionTier][processIDInt]->Fill((*itJet)->phi(),EventWeight);
 				jetCount++;
		 	}
	
		}
 	}
 
 }    



}  

//define this as a plug-in
DEFINE_FWK_MODULE(VecBosonPlusJets);
