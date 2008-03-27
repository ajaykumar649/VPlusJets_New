//This module selects the jets collection objects that have the minimum 
//distance in the eta-phi plane with the partons collection objects 
//producing a reco::CandidateBaseRefVector
//
//
//(It actually works for any container of candidates because of the CandidateView implementation )
//
//Author: Alberto Orso Maria Iorio



#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
using namespace edm;
using namespace reco;
using namespace std;
 
//static const char * kJets = "jets";
 
class   ZMuMuJetsMatchedJetsProducer: public EDProducer {
public:
  ZMuMuJetsMatchedJetsProducer(const ParameterSet&);
private:
  void produce( Event& evt, const EventSetup& evtsetup);
  InputTag partons_, jets_;
  string alias_;
};

ZMuMuJetsMatchedJetsProducer:: ZMuMuJetsMatchedJetsProducer(const ParameterSet& cfg) {
  
  partons_ = cfg.getParameter<InputTag>("partons");
  jets_ = cfg.getParameter<InputTag>("jets");
  alias_=  cfg.getParameter<string>("name");
  produces < CandidateBaseRefVector > ( ).setBranchAlias( alias_ );

  
}


void ZMuMuJetsMatchedJetsProducer::produce(Event& e, const EventSetup& esetup) {
  Handle<CandidateView> partons;
  e.getByLabel(partons_, partons);
  Handle< CandidateView > jets;
  e.getByLabel(jets_,jets) ;
  
 
  
  auto_ptr <CandidateBaseRefVector > Jets (new CandidateBaseRefVector );
 
  
  
  size_t nJets = jets->size();
  
  for(size_t i = 0; i < partons->size() ; i++){
    const Candidate & p = (*partons)[i];
    if( nJets > 0 ) {
      size_t j = 10000;
      double dMin = 10000.;
      for (size_t k = 0; k < jets->size() ; k++ ) {
        const Candidate & jet = (*jets)[k];
        double d = reco::deltaR(p, jet);
        if(d < dMin) {
          dMin = d;
          j = k;
        }
      }
      Jets->push_back(CandidateBaseRef(jets,j));
    }
    

    
                                                                              
  }
  //  e.put(Partons,kPartons);
 e.put(Jets);

}
                                                                                
                                                                                
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( ZMuMuJetsMatchedJetsProducer );

