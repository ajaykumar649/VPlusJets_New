
// -*- C++ -*-
//
// Package:    Utilities
// Class:      PuJetIdSelector
// 
/**\class PuJetIdSelector PuJetIdSelector.cc ElectroWeakAnalysis/VPlusJets/plugins/PuJetIdSelector.cc

 Description: 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Leonardo Di Matteo
//         Created:  Wed Sep 05 16:17:22 CEST 2012
//
//
////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include <memory>
#include <vector>
#include <sstream>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class PuJetIdSelector : public edm::EDProducer
{
  public:
    // construction/destruction
    PuJetIdSelector(const edm::ParameterSet& iConfig);
    virtual ~PuJetIdSelector();
  
    // member functions
    void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
    void endJob();

  private:  
    // member data
    edm::InputTag  src_;
    std::string    moduleLabel_;
    std::string    idLabel_;  
    std::string    valueMapLabel_;  
    bool           applyTightID_;
    bool           applyMediumID_;
    bool           applyLooseID_;

    unsigned int nTot_;
    unsigned int nPassed_;
};



////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
template<typename T>
    PuJetIdSelector<T>::PuJetIdSelector(const edm::ParameterSet& iConfig)
  : src_    (iConfig.getParameter<edm::InputTag>     ("src"))
    , moduleLabel_(iConfig.getParameter<std::string>   ("@module_label"))
    , idLabel_(iConfig.existsAs<std::string>("idLabel") ? iConfig.getParameter<std::string>("idLabel") : "loose")
    , valueMapLabel_(iConfig.existsAs<std::string>("valueMapLabel") ? iConfig.getParameter<std::string>("valueMapLabel") : "puJetMvaChs")
    , nTot_(0)
    , nPassed_(0)
{
  produces<std::vector<T> >();

  /// ------- Decode the ID criteria --------
  applyTightID_  = false;
  applyMediumID_ = false;
  applyLooseID_  = false;

  if( (idLabel_.compare("tight")==0) || 
      (idLabel_.compare("Tight")==0) || 
      (idLabel_.compare("TIGHT")==0) )  
    applyTightID_ = true;
  
  else if( (idLabel_.compare("medium")==0) || 
           (idLabel_.compare("Medium")==0) || 
           (idLabel_.compare("MEDIUM")==0) )  
    applyMediumID_ = true;
  
  else if( (idLabel_.compare("loose")==0) || 
           (idLabel_.compare("Loose")==0) || 
           (idLabel_.compare("LOOSE")==0) )  
    applyMediumID_ = true;
  
}


//______________________________________________________________________________
template<typename T>
    PuJetIdSelector<T>::~PuJetIdSelector(){}



////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
template<typename T>
    void PuJetIdSelector<T>::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{

  /////// Pileup jet identification value maps /////
  edm::Handle<edm::ValueMap<float> > puJetIdMVA;
  iEvent.getByLabel(valueMapLabel_,"fullDiscriminant",puJetIdMVA);
  
  edm::Handle<edm::ValueMap<int> > puJetIdFlag;
  iEvent.getByLabel(valueMapLabel_,"fullId",puJetIdFlag);
   
  edm::Handle<edm::View<T> > jetsHandle;
  iEvent.getByLabel( src_, jetsHandle ); 

  std::auto_ptr<std::vector<T> > passingJets(new std::vector<T >);
  
  bool* isPassing = new bool[jetsHandle->size()];
  for(unsigned int iJet=0;iJet<jetsHandle->size();iJet++) { 

    isPassing[iJet]=false;
    
    // Jet Id
    int   idflag = (*puJetIdFlag)[jetsHandle -> refAt(iJet)];

    /// ------- Apply selection --------
    if(applyTightID_ && PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight )) isPassing[iJet]= true;
    if(applyMediumID_ && PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium )) isPassing[iJet]= true;
    if(applyLooseID_ && PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose )) isPassing[iJet]= true;
  }

   
  unsigned int counter=0;
  typename edm::View<T>::const_iterator tIt, endcands = jetsHandle->end();
  for (tIt = jetsHandle->begin(); tIt != endcands; ++tIt, ++counter) {
    if(isPassing[counter]) passingJets->push_back( *tIt );  
  }

  nTot_    += jetsHandle->size();
  nPassed_ += passingJets->size();

  delete [] isPassing;  
  iEvent.put(passingJets);
}


//______________________________________________________________________________
template<typename T>
    void PuJetIdSelector<T>::endJob()
{
  std::stringstream ss;
  ss<<"nTot="<<nTot_<<" nPassed="<<nPassed_
      <<" effPassed="<<100.*(nPassed_/(double)nTot_)<<"%\n";
  std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"
      <<"\n"<<moduleLabel_<<"(PuJetIdSelector) SUMMARY:\n"<<ss.str()
      <<"++++++++++++++++++++++++++++++++++++++++++++++++++"
      << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////
typedef PuJetIdSelector<pat::Jet>         PATPuJetIdSelector;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PATPuJetIdSelector);

