#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace reco;
using namespace std;


class ZMuMuJetsJetsIsolator : public EDProducer{
public:
  ZMuMuJetsJetsIsolator(const ParameterSet & cfg);
  void produce(Event & evt,const EventSetup & evtsetup); 
private:
  InputTag jets_, muons_;
  double deltar_; 
};

ZMuMuJetsJetsIsolator::ZMuMuJetsJetsIsolator(const ParameterSet & cfg){
  jets_ = cfg.getParameter<InputTag  >("jets");
  muons_ = cfg.getParameter<InputTag  >("muons");
  deltar_ = cfg.getParameter<double  >("deltar");
  produces< CandidateBaseRefVector  >().setBranchAlias("isolatedJets");
  
}


void ZMuMuJetsJetsIsolator::produce(Event & evt,const EventSetup& evtsetup){
  Handle<View<Candidate >  > jets;
  evt.getByLabel(jets_, jets); 
  Handle<View<Candidate >  > muons;
  evt.getByLabel(muons_, muons); 
  
  double deltamin=deltar_;
  
   auto_ptr< CandidateBaseRefVector  > IsolatedJets( new CandidateBaseRefVector  );
 
  
  for(size_t i = 0 ; i < jets->size() ; i++ ){
    const Candidate & jet = (*jets)[i];
    bool isolated = true;
    for(size_t k = 0; k < muons->size(); k++){
      const Candidate & mu = (*muons)[k];
      double d = deltaR(mu, jet);
      if(d<deltamin)
	{isolated=false;
	break;}
    }
    if(isolated == true){
       IsolatedJets->push_back(CandidateBaseRef(jets,i));
     } 
  }
  evt.put(IsolatedJets);
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ZMuMuJetsJetsIsolator);
