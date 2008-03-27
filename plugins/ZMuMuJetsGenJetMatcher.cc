
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace reco;
using namespace std;
 
static const char * kGenJets = "genJets";
 
class   ZMuMuJetsGenJetMatcher: public EDProducer {
public:
  typedef vector<GenJet> GenJetCollection; 
  ZMuMuJetsGenJetMatcher(const ParameterSet&);
private:
  void produce( Event& evt, const EventSetup& evtsetup);
  InputTag partons_, genJets_;
};

ZMuMuJetsGenJetMatcher:: ZMuMuJetsGenJetMatcher(const ParameterSet& cfg) {
  
  partons_ = cfg.getParameter<InputTag>("partons");
  genJets_ = cfg.getParameter<InputTag>("genJets");

  
  produces < CandidateBaseRefVector > ( ).setBranchAlias( kGenJets );

  
}


void ZMuMuJetsGenJetMatcher::produce(Event& e, const EventSetup& esetup) {
  Handle<CandidateView> partons;
  e.getByLabel(partons_, partons);
  Handle< CandidateView > genJets;
  e.getByLabel(genJets_,genJets) ;
  
 
  
  auto_ptr <CandidateBaseRefVector > GenJets (new CandidateBaseRefVector );
 
  
  
  size_t nGenJets = genJets->size();
  
  for(size_t i = 0; i < partons->size() ; i++){
    const Candidate & p = (*partons)[i];
    if( nGenJets > 0 ) {
      size_t j = 10000;
      double dMin = 10000.;
      for (size_t k = 0; k < genJets->size() ; k++ ) {
        const Candidate & jet = (*genJets)[k];
        double d = reco::deltaR(p, jet);
        if(d < dMin) {
          dMin = d;
          j = k;
        }
      }
      GenJets->push_back(CandidateBaseRef(genJets,j));
    }
    

    
                                                                              
  }
  //  e.put(Partons,kPartons);
 e.put(GenJets);

}
                                                                                
                                                                                
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( ZMuMuJetsGenJetMatcher );

