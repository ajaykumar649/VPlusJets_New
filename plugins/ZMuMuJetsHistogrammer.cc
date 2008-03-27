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
  InputTag partons_, genJets_, caloJets_ , z_ , muons_, zMC_,  matchedCaloJets_, matchedGenJets_ ;    
  
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
  
  partons_=cfg.getParameter<InputTag>("partons");
  genJets_=cfg.getParameter<InputTag>("genJets");
  caloJets_=cfg.getParameter<InputTag>("caloJets");
  muons_=cfg.getParameter<InputTag>("muons");
  zMC_=cfg.getParameter<InputTag>("zMC");
  z_=cfg.getParameter<InputTag>("z");
  matchedCaloJets_=cfg.getParameter<InputTag>("matchedCaloJets");
  matchedGenJets_=cfg.getParameter<InputTag>("matchedGenJets");
  
  Service<TFileService> fs;
  
  genJetsPt = fs->make<TH1D>("genJetsPt","genJetsPt",400,0,500);
  genJetsEta = fs->make<TH1D>("genJetsEta","genJetsEta",200,-3,3);
  genJetsEnergy = fs->make<TH1D>("genJetsEnergy","genJetsEnergy",200,0,1000);
  genJetsPhi =  fs->make<TH1D>("genJetsPhi","genJetsPhi",200,0,7);
  
  genJetsTotalPt =  fs->make<TH1D>("genJetsTotalPt","genJetsTotalPt",400,0,500);
  genJetsTotalEnergy =  fs->make<TH1D>("genJetsTotalEnergy","genJetsTotalEnergy",200,0,2000);
  genJetsNumber = fs->make<TH1D>("genJetsNumber","genJetsNumber",10,0,10);
  genJetsTotalPhi =  fs->make<TH1D>("genJetsTotalPhi","genJetsTotalPhi",200,0,7);

  matchedGenJetsPt = fs->make<TH1D>("matchedGenJetsPt","matchedGenJetsPt",400,0,500);
  matchedGenJetsEta = fs->make<TH1D>("matchedGenJetsEta","matchedGenJetsEta",200,-3,3);
  matchedGenJetsEnergy = fs->make<TH1D>("matchedGenJetsEnergy","matchedGenJetsEnergy",200,0,1000);
  matchedGenJetsPhi =  fs->make<TH1D>("matchedGenJetsPhi","matchedGenJetsPhi",200,0,7);
  
  matchedGenJetsTotalPt =  fs->make<TH1D>("matchedGenJetsTotalPt","matchedGenJetsTotalPt",400,0,500);
  matchedGenJetsTotalEnergy =  fs->make<TH1D>("matchedGenJetsTotalEnergy","matchedGenJetsTotalEnergy",200,0,2000);
  matchedGenJetsNumber = fs->make<TH1D>("matchedGenJetsNumber","matchedGenJetsNumber",10,0,10);
  
  caloJetsPt = fs->make<TH1D>("caloJetsPt","caloJetsPt",400,0,500);
  caloJetsEta = fs->make<TH1D>("caloJetsEta","caloJetsEta",200,-3,3);
  caloJetsEnergy = fs->make<TH1D>("caloJetsEnergy","caloJetsEnergy",200,0,1000); 
  caloJetsPhi =  fs->make<TH1D>("caloJetsPhi","caloJetsPhi",200,0,7);
   
  caloJetsTotalPt =  fs->make<TH1D>("caloJetsTotalPt","caloJetsTotalPt",400,0,500);
  caloJetsTotalEnergy =  fs->make<TH1D>("caloJetsTotalEnergy","caloJetsTotalEnergy",200,0,2000);
  caloJetsNumber = fs->make<TH1D>("caloJetsNumber","caloJetsNumber",10,0,10);
  caloJetsTotalPhi =  fs->make<TH1D>("caloJetsTotalPhi","caloJetsTotalPhi",200,0,7);
 
  matchedCaloJetsPt = fs->make<TH1D>("matchedCaloJetsPt","matchedCaloJetsPt",400,0,500);
  matchedCaloJetsEta = fs->make<TH1D>("matchedCaloJetsEta","matchedCaloJetsEta",200,-3,3);
  matchedCaloJetsEnergy = fs->make<TH1D>("matchedCaloJetsEnergy","matchedCaloJetsEnergy",200,0,1000);
  matchedCaloJetsPhi =  fs->make<TH1D>("matchedCaloJetsPhi","matchedCaloJetsPhi",200,0,7);

  matchedCaloJetsTotalPt =  fs->make<TH1D>("matchedCaloJetsTotalPt","matchedCaloJetsTotalPt",400,0,500);
  matchedCaloJetsTotalEnergy =  fs->make<TH1D>("matchedCaloJetsTotalEnergy","matchedCaloJetsTotalEnergy",200,0,2000);
  matchedCaloJetsNumber = fs->make<TH1D>("matchedCaloJetsNumber","matchedCaloJetsNumber",10,0,10);

  partonsPt = fs->make<TH1D>("partonsPt","partonsPt",400,0,500);
  partonsEta = fs->make<TH1D>("partonsEta","partonsEta",200,-3,3);
  partonsEnergy = fs->make<TH1D>("partonsEnergy","partonsEnergy",200,0,1000);
  partonsPhi = fs->make<TH1D>("partonsPhi","partonsPhi",200,0,7);
  partonsId = fs->make<TH1D>("partonsId","partonsId",35,-10,25);

  partonsTotalPt = fs->make<TH1D>("partonsTotalPt","partonsTotalPt",400,0,500);
  partonsTotalEnergy = fs->make<TH1D>("partonsTotalEnergy","partonsTotalEnergy",200,0,2000);
  partonsNumber = fs->make<TH1D>("partonsNumber","partonsNumber",10,0,10);

  muonsPt = fs->make<TH1D>("muonsPt","muonsPt",400,0,500);
  muonsEta = fs->make<TH1D>("muonsEta","muonsEta",200,-3,3);
  muonsEnergy = fs->make<TH1D>("muonsEnergy","muonsEnergy",200,0,1000);
  muonsPhi = fs->make<TH1D>("muonsPhi","muonsPhi",200,0,7);
   
  totalMuonsPt = fs->make<TH1D>("totalMuonsPt","totalMuonsPt",400,0,500);
  totalMuonsEta = fs->make<TH1D>("totalMuonsEta","totalMuonsEta",200,-3,3);
  totalMuonsEnergy = fs->make<TH1D>("totalMuonsEnergy","totalMuonsEnergy",200,0,1000);
  totalMuonsPhi = fs->make<TH1D>("totalMuonsPhi","totalMuonsPhi",200,0,7);
  totalMuonsMass = fs->make<TH1D>("totalMuonsMass","totalMuonsMass",200,0,150);
    
  zMass = fs->make<TH1D>("zMass","zMass",400,0,150);
  zPt = fs->make<TH1D>("zPt","zPt",200,0,500);
  zEta = fs->make<TH1D>("zEta","zEta",200,-3,3);
  zEnergy = fs->make<TH1D>("zEnergy","zEnergy",200,0,1000);
  zPhi = fs->make<TH1D>("zPhi","zPhi",200,0,7);
  zY = fs->make<TH1D>("zRapidity","zRapidity",300,-7,7);

  zMCMass = fs->make<TH1D>("zMCMass","zMCMass",400,0,150);
  zMCPt = fs->make<TH1D>("zMCPt","zMCPt",200,0,7000);
  zMCEta = fs->make<TH1D>("zMCEta","zMCEta",200,-3,3);
  zMCEnergy = fs->make<TH1D>("zMCEnergy","zMCEnergy",200,0,1000);
  zMCPhi = fs->make<TH1D>("zMCPhi","zMCPhi",200,0,7);
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
    partonsEta->Fill(p.eta());
    partonsPhi->Fill(p.phi());
    partonsEnergy->Fill(p.energy());
    partonsPt->Fill(p.pt());  
    partonsId->Fill(p.pdgId());
  }
  
  partonsTotalPt->Fill(sum.pt());
  partonsTotalEnergy->Fill(sum.energy());
  partonsNumber->Fill(nPartons);
  
  sum = XYZTLorentzVector(0,0,0,0);
  for(size_t i = 0; i < nGenJets; i++ ){
    const Candidate & p = (*genJets)[i];
    sum = sum + p.p4();   
    genJetsPhi->Fill(p.phi());
    genJetsEta->Fill(p.eta());
    genJetsEnergy->Fill(p.energy());
    genJetsPt->Fill(p.pt());
  } 
  genJetsTotalEnergy->Fill(sum.energy());
  genJetsTotalPt->Fill(sum.pt());
  genJetsNumber->Fill(nGenJets);
  genJetsTotalPhi->Fill(sum.phi());

  sum = XYZTLorentzVector(0,0,0,0);
  size_t maxIndex = 0;
  for(size_t i = 0; i < nCaloJets; i++ ){
    const Candidate & p = (*caloJets)[i];
    sum = sum + p.p4();    
    if(p.energy() > ((*caloJets)[maxIndex]).energy()) maxIndex = i; 
    caloJetsEnergy->Fill(p.energy());
    caloJetsPt->Fill(p.pt());
    caloJetsEta->Fill(p.eta());
    caloJetsPhi->Fill( p.phi());
  }
  caloJetsTotalEnergy->Fill(sum.energy());
  caloJetsTotalPt->Fill(sum.pt());
  caloJetsNumber->Fill(nCaloJets);
  caloJetsTotalPhi->Fill(sum.phi());
  
  sum = XYZTLorentzVector(0,0,0,0);
  for(size_t i = 0; i < matchedCaloJets->size(); i++ ){
    const Candidate & p = (*matchedCaloJets)[i];
    sum = sum + p.p4();    
    matchedCaloJetsEnergy->Fill(p.energy());
    matchedCaloJetsPt->Fill(p.pt());
    matchedCaloJetsEta->Fill(p.eta());
    matchedCaloJetsPhi->Fill( p.phi());
  }
  matchedCaloJetsTotalEnergy->Fill(sum.energy());
  matchedCaloJetsTotalPt->Fill(sum.pt());
  matchedCaloJetsNumber->Fill(matchedCaloJets->size());
  
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < matchedGenJets->size(); i++ ){
   const Candidate & p = (*matchedGenJets)[i];
   sum = sum + p.p4();    
   matchedGenJetsEnergy->Fill(p.energy());
   matchedGenJetsPt->Fill(p.pt());
   matchedGenJetsEta->Fill(p.eta());
   matchedGenJetsPhi->Fill( p.phi());
 }
 matchedGenJetsTotalEnergy->Fill(sum.energy());
 matchedGenJetsTotalPt->Fill(sum.pt());
 matchedGenJetsNumber->Fill(matchedGenJets->size());
 
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < muons->size(); i++ ){
   const Candidate & p = (*muons)[i];
   sum = sum + p.p4();
   muonsPt->Fill(p.pt());
   muonsEta->Fill(p.eta());
   muonsEta->Fill(p.eta());
   muonsPhi->Fill(p.phi()); 
 } 
 totalMuonsPt->Fill(sum.pt());
 totalMuonsMass->Fill(sum.mass());  
 totalMuonsEta->Fill(sum.eta());
 totalMuonsPhi->Fill(sum.phi());
  
   
 sum = XYZTLorentzVector(0,0,0,0);
 for(size_t i = 0; i < zMC->size(); i++ ){
   const Candidate & p = (*zMC)[i];
   sum = sum + p.p4();
   zMCMass->Fill(p.mass());
   zMCPt->Fill(p.pt());
   zMCY->Fill(p.y());
   zMCPhi->Fill(p.phi());
   zMCPhi->Fill(p.eta());
 }
 
  
 leadingPartonsVSCaloJets->Fill(nPartons,nCaloJets);
 leadingPartonsVSGenJets->Fill(nPartons,nGenJets);
 genJetsVSCaloJets->Fill(nGenJets,nCaloJets);
 
}







#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(zMuMuJetsHistogrammer);



