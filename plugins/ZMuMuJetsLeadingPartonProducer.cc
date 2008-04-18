//This module selects the GenParticles of status 3 that are qwarks and gluons
//wich have no status 3 daughters. 
//and produces a vector of reco::GenParticle
//
//
//
//
//Author: Alberto Orso Maria Iorio
                                                                                                                                          


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace reco;
using namespace std;

class ZMuMuJetsLeadingPartonProducer : public EDProducer {
public:
  ZMuMuJetsLeadingPartonProducer(const ParameterSet&);
private:
  void produce(Event&, const EventSetup&);
  InputTag src_;
};

ZMuMuJetsLeadingPartonProducer::ZMuMuJetsLeadingPartonProducer(const ParameterSet& cfg) :
  src_(cfg.getParameter<InputTag>("src")) {
  produces<GenParticleCollection>().setBranchAlias("leadingPartons");
}

void ZMuMuJetsLeadingPartonProducer::produce(Event& evt, const EventSetup&) {
  Handle<GenParticleCollection> genParticles; 
  evt.getByLabel(src_ ,genParticles);
  auto_ptr<GenParticleCollection> partons( new GenParticleCollection );
  for(size_t i = 0; i < genParticles->size(); i++ ) {
    const GenParticle & p = (*genParticles)[i];
    int status = p.status();
    if(status  < 3) continue;
    int id = abs(p.pdgId());
    if(id > 6 && id != 21 ) continue;
    size_t nDaughters = p.numberOfDaughters();
    //if(nDaughters < 1) continue;
    //size_t nMothers = p.numberOfMothers();
    //if(nMothers < 1) continue;
    bool ok1 = true;
    bool ok2 = true;
    for (size_t ds = 0; ds < nDaughters; ds++) {
      if(p.daughter(ds)->status()==3) {
	ok1 = false; break;
      }
      //size_t nDaughterDaughters = p.daughter(ds)->numberOfDaughters();
      //      for(size_t k = 0; k < nDaughterDaughters && ok2 == false; k++){
      //	int dauId = p.daughter(ds)->daughter(k)->pdgId(); 
      //	if(dauId == 92) ok2 = true;
      //}
    }
    if (ok1 && ok2) partons->push_back((*genParticles)[i]);
    // if (p.pdgId()==23) cout <<"  c'e' una zeta   "<<endl;
    
  }
  evt.put(partons);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ZMuMuJetsLeadingPartonProducer);
