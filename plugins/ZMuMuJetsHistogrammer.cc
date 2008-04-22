//This module creates histograms from muons, partons, genJets etc.
//
//
//
//Author: Alberto Orso Maria Iorio



#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "SimGeneral/HepPDTRecord/interface/PdtEntry.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <iostream>



using namespace edm;
using namespace reco;
using namespace std;
using namespace math;

class zMuMuJetsHistogrammer : public EDAnalyzer {
  typedef vector<GenJet> GenJetCollection;
public:
 explicit zMuMuJetsHistogrammer( const ParameterSet & );
private:
  void analyze( const Event& , const EventSetup& );
  InputTag partons_, genJets_, caloJets_ , z_ , muons_, zMC_,  matchedCaloJets_, matchedGenJets_ ;  bool hasGlobalWeight_;
  InputTag csa07weight_; 
  
  TH1D *partonsPt, *partonsEta, *partonsEnergy, *partonsPhi, *partonsId,  *partonsNumber , *partonsTotalPt , *partonsTotalEnergy;
   
  TH1D *genJetsPt, *genJetsEta, *genJetsEnergy, *genJetsPhi, *genJetsTotalPt, *genJetsTotalEnergy, *genJetsNumber, *genJetsTotalPhi;
  TH1D *matchedGenJetsPt, *matchedGenJetsEta, *matchedGenJetsEnergy, *matchedGenJetsPhi, *matchedGenJetsTotalPt, *matchedGenJetsTotalEnergy, *matchedGenJetsNumber;
  TH1D *caloJetsPt, *caloJetsEta, *caloJetsEnergy, *caloJetsPhi, *caloJetsTotalPt, *caloJetsTotalEnergy, *caloJetsNumber, *caloJetsTotalPhi;
  TH1D *matchedCaloJetsPt, *matchedCaloJetsEta, *matchedCaloJetsEnergy, *matchedCaloJetsPhi, *matchedCaloJetsTotalPt, *matchedCaloJetsTotalEnergy, *matchedCaloJetsNumber;
  TH1D *muonsPt, *muonsEta, *muonsEnergy, *muonsPhi, *muonsMass ;
  TH1D *totalMuonsPt, *totalMuonsEta, *totalMuonsEnergy, *totalMuonsPhi, *totalMuonsMass;
  TH1D *zPt, *zEta, *zEnergy, *zPhi, *zMass, *zY;
  TH1D *zMCPt, *zMCEta, *zMCEnergy, *zMCPhi, *zMCMass, *zMCY;
  TH1D *caloJetsPhiMinusMuonsPhi,*genJetsPhiMinusMuonsPhi, *partonsPhiMinsuZPhi;

  
  TH2D *leadingPartonsVSCaloJets,  *leadingPartonsVSGenJets,* genJetsVSCaloJets;
 
 
  
};

zMuMuJetsHistogrammer::zMuMuJetsHistogrammer(const ParameterSet & cfg)
{
  
  hasGlobalWeight_ = cfg.getParameter<bool>("hasglobalweight");
  if(hasGlobalWeight_==false)csa07weight_ =cfg.getParameter<InputTag>("weight");
  
  partons_=cfg.getParameter<InputTag>("partons");
  genJets_=cfg.getParameter<InputTag>("genJets");
  caloJets_=cfg.getParameter<InputTag>("caloJets");
  muons_=cfg.getParameter<InputTag>("muons");
  zMC_=cfg.getParameter<InputTag>("zMC");
  z_=cfg.getParameter<InputTag>("z");
  matchedCaloJets_=cfg.getParameter<InputTag>("matchedCaloJets");
  matchedGenJets_=cfg.getParameter<InputTag>("matchedGenJets");
  
  Service<TFileService> fs;
  
  genJetsPt = fs->make<TH1D>("genJetsPt","genJetsPt",100,0,400);
  genJetsEta = fs->make<TH1D>("genJetsEta","genJetsEta",100,-3,3);
  genJetsEnergy = fs->make<TH1D>("genJetsEnergy","genJetsEnergy",100,0,500);
  genJetsPhi =  fs->make<TH1D>("genJetsPhi","genJetsPhi",100,0,7);
  
  genJetsTotalPt =  fs->make<TH1D>("genJetsTotalPt","genJetsTotalPt",100,0,400);
  genJetsTotalEnergy =  fs->make<TH1D>("genJetsTotalEnergy","genJetsTotalEnergy",100,0,500);
  genJetsNumber = fs->make<TH1D>("genJetsNumber","genJetsNumber",10,0,10);
  genJetsTotalPhi =  fs->make<TH1D>("genJetsTotalPhi","genJetsTotalPhi",100,0,7);

  matchedGenJetsPt = fs->make<TH1D>("matchedGenJetsPt","matchedGenJetsPt",100,0,400);
  matchedGenJetsEta = fs->make<TH1D>("matchedGenJetsEta","matchedGenJetsEta",100,-3,3);
  matchedGenJetsEnergy = fs->make<TH1D>("matchedGenJetsEnergy","matchedGenJetsEnergy",100,0,500);
  matchedGenJetsPhi =  fs->make<TH1D>("matchedGenJetsPhi","matchedGenJetsPhi",100,0,7);
  
  matchedGenJetsTotalPt =  fs->make<TH1D>("matchedGenJetsTotalPt","matchedGenJetsTotalPt",100,0,500);
  matchedGenJetsTotalEnergy =  fs->make<TH1D>("matchedGenJetsTotalEnergy","matchedGenJetsTotalEnergy",100,0,500);
  matchedGenJetsNumber = fs->make<TH1D>("matchedGenJetsNumber","matchedGenJetsNumber",10,0,10);
  
  caloJetsPt = fs->make<TH1D>("caloJetsPt","caloJetsPt",100,0,400);
  caloJetsEta = fs->make<TH1D>("caloJetsEta","caloJetsEta",100,-3,3);
  caloJetsEnergy = fs->make<TH1D>("caloJetsEnergy","caloJetsEnergy",100,0,500); 
  caloJetsPhi =  fs->make<TH1D>("caloJetsPhi","caloJetsPhi",100,0,7);
   
  caloJetsTotalPt =  fs->make<TH1D>("caloJetsTotalPt","caloJetsTotalPt",100,0,400);
  caloJetsTotalEnergy =  fs->make<TH1D>("caloJetsTotalEnergy","caloJetsTotalEnergy",100,0,500);
  caloJetsNumber = fs->make<TH1D>("caloJetsNumber","caloJetsNumber",10,0,10);
  caloJetsTotalPhi =  fs->make<TH1D>("caloJetsTotalPhi","caloJetsTotalPhi",100,0,7);
 
  matchedCaloJetsPt = fs->make<TH1D>("matchedCaloJetsPt","matchedCaloJetsPt",100,0,400);
  matchedCaloJetsEta = fs->make<TH1D>("matchedCaloJetsEta","matchedCaloJetsEta",100,-3,3);
  matchedCaloJetsEnergy = fs->make<TH1D>("matchedCaloJetsEnergy","matchedCaloJetsEnergy",100,0,500);
  matchedCaloJetsPhi =  fs->make<TH1D>("matchedCaloJetsPhi","matchedCaloJetsPhi",100,0,7);

  matchedCaloJetsTotalPt =  fs->make<TH1D>("matchedCaloJetsTotalPt","matchedCaloJetsTotalPt",100,0,400);
  matchedCaloJetsTotalEnergy =  fs->make<TH1D>("matchedCaloJetsTotalEnergy","matchedCaloJetsTotalEnergy",100,0,500);
  matchedCaloJetsNumber = fs->make<TH1D>("matchedCaloJetsNumber","matchedCaloJetsNumber",10,0,10);

  partonsPt = fs->make<TH1D>("partonsPt","partonsPt",100,0,400);
  partonsEta = fs->make<TH1D>("partonsEta","partonsEta",100,-3,3);
  partonsEnergy = fs->make<TH1D>("partonsEnergy","partonsEnergy",100,0,500);
  partonsPhi = fs->make<TH1D>("partonsPhi","partonsPhi",100,0,7);
  partonsId = fs->make<TH1D>("partonsId","partonsId",35,-10,25);

  partonsTotalPt = fs->make<TH1D>("partonsTotalPt","partonsTotalPt",100,0,400);
  partonsTotalEnergy = fs->make<TH1D>("partonsTotalEnergy","partonsTotalEnergy",100,0,500);
  partonsNumber = fs->make<TH1D>("partonsNumber","partonsNumber",10,0,10);

  muonsPt = fs->make<TH1D>("muonsPt","muonsPt",100,0,400);
  muonsEta = fs->make<TH1D>("muonsEta","muonsEta",100,-3,3);
  muonsEnergy = fs->make<TH1D>("muonsEnergy","muonsEnergy",100,0,500);
  muonsPhi = fs->make<TH1D>("muonsPhi","muonsPhi",100,0,7);
   
  totalMuonsPt = fs->make<TH1D>("totalMuonsPt","totalMuonsPt",100,0,400);
  totalMuonsEta = fs->make<TH1D>("totalMuonsEta","totalMuonsEta",100,-3,3);
  totalMuonsEnergy = fs->make<TH1D>("totalMuonsEnergy","totalMuonsEnergy",100,0,500);
  totalMuonsPhi = fs->make<TH1D>("totalMuonsPhi","totalMuonsPhi",100,0,7);
  totalMuonsMass = fs->make<TH1D>("totalMuonsMass","totalMuonsMass",100,0,150);
    
  zMass = fs->make<TH1D>("zMass","zMass",100,0,200);
  zPt = fs->make<TH1D>("zPt","zPt",100,0,400);
  zEta = fs->make<TH1D>("zEta","zEta",100,-3,3);
  zEnergy = fs->make<TH1D>("zEnergy","zEnergy",100,0,500);
  zPhi = fs->make<TH1D>("zPhi","zPhi",100,0,7);
  zY = fs->make<TH1D>("zRapidity","zRapidity",100,-7,7);

  zMCMass = fs->make<TH1D>("zMCMass","zMCMass",100,0,200);
  zMCPt = fs->make<TH1D>("zMCPt","zMCPt",100,0,400);
  zMCEta = fs->make<TH1D>("zMCEta","zMCEta",100,-3,3);
  zMCEnergy = fs->make<TH1D>("zMCEnergy","zMCEnergy",100,0,500);
  zMCPhi = fs->make<TH1D>("zMCPhi","zMCPhi",100,0,7);
  zMCY = fs->make<TH1D>("zMCRapidity","zMCRapidity",300,-7,7);
  
  
  
  leadingPartonsVSCaloJets = fs->make<TH2D>("leadingPartonsVSCaloJets","leadingPartonsVSCaloJets",10,0,10,10,0,10);
  leadingPartonsVSGenJets = fs->make<TH2D>("leadingPartonsVSGenJets","leadingPartonsVSGenJets",10,0,10,10,0,10);
  genJetsVSCaloJets= fs->make<TH2D>("genJetsVSCaloJets","genJetsVSCaloJets",10,0,10,10,0,10);
 
  }

void zMuMuJetsHistogrammer::analyze( const Event& evt, const EventSetup& evtstp)
{


  Handle<View<Candidate> > partons ;
  evt.getByLabel(partons_,partons);
  Handle< View<Candidate> > caloJets ;
  evt.getByLabel(caloJets_,caloJets);
  Handle< View<Candidate> > matchedCaloJets ;
  evt.getByLabel(matchedCaloJets_,matchedCaloJets);
  Handle< View<Candidate> > matchedGenJets ;
  evt.getByLabel(matchedGenJets_,matchedGenJets);
  Handle<View<Candidate> > genJets ;
  evt.getByLabel(genJets_,genJets);
  Handle<View<Candidate> > muons ;
  evt.getByLabel(muons_,muons);
  Handle<View<Candidate> > z ;
  evt.getByLabel(z_,z);
  Handle<View<Candidate> > zMC ;
  evt.getByLabel(zMC_,zMC);

  XYZTLorentzVector sum(0,0,0,0) ;

  double weight=1;
  if(hasGlobalWeight_==false){
   
    Handle<  double   > csaweight  ;   
    evt.getByLabel(csa07weight_,csaweight);
    weight = (*csaweight);
  
}



  size_t nPartons = partons->size();
  size_t nCaloJets = caloJets->size();
  size_t nGenJets = genJets->size();
    
  /*if( n1 > 1  ){
    cout<<"  numero partoni  "<<n1<<endl;
    cout<<"  numero caloJets  "<<n2<<endl; 
    cout<<"  numero genJets  "<<n3<<endl;
    }*/
  
  for(size_t i = 0; i < nPartons; i++ ){
    const Candidate & p = (*partons)[i];
    sum = sum + p.p4();
    partonsEta->Fill(p.eta(),weight);
    partonsPhi->Fill(p.phi(),weight);
    partonsEnergy->Fill(p.energy(),weight);
    partonsPt->Fill(p.pt(),weight);  
    partonsId->Fill(p.pdgId(),weight);
  }
  
  partonsTotalPt->Fill(sum.pt(),weight);
  partonsTotalEnergy->Fill(sum.energy(),weight);
  partonsNumber->Fill(nPartons);
  
  sum = XYZTLorentzVector(0,0,0,0);
  for(size_t i = 0; i < nGenJets; i++ ){
    const Candidate & p = (*genJets)[i];
    sum = sum + p.p4();   
    genJetsPhi->Fill(p.phi(),weight);
    genJetsEta->Fill(p.eta(),weight);
    genJetsEnergy->Fill(p.energy(),weight);
    genJetsPt->Fill(p.pt(),weight);
  } 
  genJetsTotalEnergy->Fill(sum.energy(),weight);
  genJetsTotalPt->Fill(sum.pt(),weight);
  genJetsNumber->Fill(nGenJets);
  genJetsTotalPhi->Fill(sum.phi(),weight);

  sum = XYZTLorentzVector(0,0,0,0);
  size_t maxIndex = 0;
  for(size_t i = 0; i < nCaloJets; i++ ){
    const Candidate & p = (*caloJets)[i];
    sum = sum + p.p4();    
    if(p.energy() > ((*caloJets)[maxIndex]).energy()) maxIndex = i; 
    caloJetsEnergy->Fill(p.energy(),weight);
    caloJetsPt->Fill(p.pt(),weight);
    caloJetsEta->Fill(p.eta(),weight);
    caloJetsPhi->Fill( p.phi(),weight);
  }
  caloJetsTotalEnergy->Fill(sum.energy(),weight);
  caloJetsTotalPt->Fill(sum.pt(),weight);
  caloJetsNumber->Fill(nCaloJets);
  caloJetsTotalPhi->Fill(sum.phi(),weight);
  
  sum = XYZTLorentzVector(0,0,0,0);
  for(size_t i = 0; i < matchedCaloJets->size(); i++ ){
    const Candidate & p = (*matchedCaloJets)[i];
    sum = sum + p.p4();    
    matchedCaloJetsEnergy->Fill(p.energy(),weight);
    matchedCaloJetsPt->Fill(p.pt(),weight);
    matchedCaloJetsEta->Fill(p.eta(),weight);
    matchedCaloJetsPhi->Fill( p.phi(),weight);
  }
  matchedCaloJetsTotalEnergy->Fill(sum.energy(),weight);
  matchedCaloJetsTotalPt->Fill(sum.pt(),weight);
  matchedCaloJetsNumber->Fill(matchedCaloJets->size(),weight);
  
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < matchedGenJets->size(); i++ ){
   const Candidate & p = (*matchedGenJets)[i];
   sum = sum + p.p4();    
   matchedGenJetsEnergy->Fill(p.energy(),weight);
   matchedGenJetsPt->Fill(p.pt(),weight);
   matchedGenJetsEta->Fill(p.eta(),weight);
   matchedGenJetsPhi->Fill( p.phi(),weight);
 }
 matchedGenJetsTotalEnergy->Fill(sum.energy(),weight);
 matchedGenJetsTotalPt->Fill(sum.pt(),weight);
 matchedGenJetsNumber->Fill(matchedGenJets->size(),weight);
 
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < muons->size(); i++ ){
   const Candidate & p = (*muons)[i];
   sum = sum + p.p4();
   muonsPt->Fill(p.pt(),weight);
   muonsEta->Fill(p.eta(),weight);
   muonsEta->Fill(p.eta(),weight);
   muonsPhi->Fill(p.phi(),weight); 
 } 
 totalMuonsPt->Fill(sum.pt(),weight);
 totalMuonsMass->Fill(sum.mass(),weight);  
 totalMuonsEta->Fill(sum.eta(),weight);
 totalMuonsPhi->Fill(sum.phi(),weight);
  
   
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < zMC->size(); i++ ){
   const Candidate & p = (*zMC)[i];
   sum = sum + p.p4();
   zMCMass->Fill(p.mass(),weight);
   zMCPt->Fill(p.pt(),weight);
   zMCY->Fill(p.y(),weight);
   zMCPhi->Fill(p.phi(),weight);
   zMCPhi->Fill(p.eta(),weight);
 }

 for(size_t i = 0; i < z->size(); i++ ){
   const Candidate & p = (*z)[i];
   sum = sum + p.p4();
   zMass->Fill(p.mass(),weight);
   zPt->Fill(p.pt(),weight);
   zY->Fill(p.y(),weight);
   zPhi->Fill(p.phi(),weight);
   zPhi->Fill(p.eta(),weight);
 }
                                                                                                                                                             

  
 leadingPartonsVSCaloJets->Fill(nPartons,nCaloJets);
 leadingPartonsVSGenJets->Fill(nPartons,nGenJets);
 genJetsVSCaloJets->Fill(nGenJets,nCaloJets);
 
}







#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(zMuMuJetsHistogrammer);



