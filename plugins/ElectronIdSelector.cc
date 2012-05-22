
/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   - Selects "loose" and "tight" electrons needed for semi-leptonic analysis.
 *   - Saves collection of the reference vectors of electrons passing the 
 *     required electron ID.
 * History:
 *   
 *
 *****************************************************************************/
////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"



#include <memory>
#include <vector>
#include <sstream>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class ElectronIdSelector : public edm::EDProducer
{
public:
  // construction/destruction
  ElectronIdSelector(const edm::ParameterSet& iConfig);
  virtual ~ElectronIdSelector();
  
  // member functions
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
  void endJob();

private:  
  // member data
  edm::InputTag  src_;
  std::string    moduleLabel_;
  std::string    idLabel_;  
  bool           useMVAbasedID_;
  bool           useDetectorIsolation_;
  bool           applyTightID_;
  bool           applyLooseID_;
  bool           applyQcdID_;

  unsigned int nTot_;
  unsigned int nPassed_;
};



////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
template<typename T>
ElectronIdSelector<T>::ElectronIdSelector(const edm::ParameterSet& iConfig)
  : src_    (iConfig.getParameter<edm::InputTag>     ("src"))
  , moduleLabel_(iConfig.getParameter<std::string>   ("@module_label"))
  , idLabel_(iConfig.existsAs<std::string>("idLabel") ? iConfig.getParameter<std::string>("idLabel") : "loose")
  , useMVAbasedID_(iConfig.existsAs<bool>("useMVAbasedID") ? iConfig.getParameter<bool>("useMVAbasedID") : false)
  , useDetectorIsolation_(iConfig.existsAs<bool>("useDetectorIsolation") ? iConfig.getParameter<bool>("useDetectorIsolation") : false)
  , nTot_(0)
  , nPassed_(0)
{
  produces<std::vector<T> >();

  /// ------- Decode the ID criteria --------
  applyTightID_ = false;
  applyLooseID_ = false;
  applyQcdID_ = false;

  if( (idLabel_.compare("tight")==0) || 
      (idLabel_.compare("Tight")==0) || 
      (idLabel_.compare("TIGHT")==0) ||
      (idLabel_.compare("WP70")==0) ||
      (idLabel_.compare("wp70")==0) )  
    applyTightID_ = true;
  else if( (idLabel_.compare("loose")==0) || 
      (idLabel_.compare("Loose")==0) || 
      (idLabel_.compare("LOOSE")==0) ||
      (idLabel_.compare("WP90")==0) ||
      (idLabel_.compare("wp90")==0) )  applyLooseID_ = true;
  else if( (idLabel_.compare("qcd")==0) || 
      (idLabel_.compare("Qcd")==0) || 
      (idLabel_.compare("QCD")==0) ||
      (idLabel_.compare("QCDid")==0) ||
      (idLabel_.compare("QcdId")==0) )  applyQcdID_ = true;
}


//______________________________________________________________________________
template<typename T>
ElectronIdSelector<T>::~ElectronIdSelector(){}



////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
template<typename T>
void ElectronIdSelector<T>::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{

  /////// Pileup density "rho" in the event from fastJet pileup calculation /////
  edm::Handle<double> rho;
  const edm::InputTag eventrho("kt6PFJets", "rho");
  iEvent.getByLabel(eventrho,rho);
  double fastJetRho = *rho;


  std::auto_ptr<std::vector<T> > passingElectrons(new std::vector<T >);

  edm::Handle<edm::View<T> > electrons;
  iEvent.getByLabel(src_,electrons);
  
  bool* isPassing = new bool[electrons->size()];
  for(unsigned int iElec=0;iElec<electrons->size();iElec++) { 

    isPassing[iElec]=false;

    const T& ele = electrons->at(iElec);


    // -------- Make sure that the electron is within acceptance ------
    bool isEB = ele.isEB();
    bool isEE = ele.isEE();
    bool inAcceptance = (isEB || isEE) && (ele.ecalDrivenSeed()==1);
    float pt  = ele.p4().Pt();


    // -------- Compute isolation ------
   const double PI = 4.0*atan(1.0);
   float detector_isolation = (ele.dr03TkSumPt() + 
			       std::max(0.,ele.dr03EcalRecHitSumEt()-1.0) + 
			       ele.dr03HcalTowerSumEt() - 
			       PI*0.3*0.3*fastJetRho) / pt;

   ////// The pfIso are done in dR=0.4 
   float pf_isolation = (ele.pfIsolationVariables().chargedHadronIso + 
			 ele.pfIsolationVariables().neutralHadronIso + 
			 ele.pfIsolationVariables().photonIso  - 
			 PI*0.4*0.4*fastJetRho) / pt;

   float isolation = 0.;
   if(useDetectorIsolation_) isolation = detector_isolation;
   else isolation = pf_isolation;

    // -------- Compute ID ------
   Double_t sihih   = fabs(ele.sigmaIetaIeta());
   Double_t Dphi    = fabs(ele.deltaPhiSuperClusterTrackAtVtx());
   Double_t Deta    = fabs(ele.deltaEtaSuperClusterTrackAtVtx());
   //Double_t HoE     = fabs(electron.hadronicOverEm());
   Double_t abseta  = fabs(ele.eta());

    Int_t innerHits = ele.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
 
    Double_t dist = ele.convDist(); // default value is -9999 if conversion partner not found
    Double_t dcot = ele.convDcot(); // default value is -9999 if conversion partner not found
    Bool_t isConv = fabs(dist) < 0.02 && fabs(dcot) < 0.02;
    Double_t mvaOut = -2;
     const pat::Electron* patele = dynamic_cast<const pat::Electron *>( &ele );
     if(patele->isElectronIDAvailable("mvaTrigV0"))
           mvaOut =  patele->electronID("mvaTrigV0");

    bool isTight = false;  /////// <--- equivalent to WP70
    bool isLoose = false;  /////// <--- equivalent to WP90
    bool isQCD   = false;  /////// <--- invert the isolation cut

    // -------- if cut-based ID -----------------
    if( !useMVAbasedID_ ) {
      isTight = (pt>30.) && inAcceptance && (innerHits==0) && 
	(isolation<0.05) && (!isConv) && 
	((isEB && sihih<0.01 && Dphi<0.03 && Deta<0.004) || 
	 (isEE && sihih<0.03 && Dphi<0.02 && Deta<0.005)); 
      
      isLoose = (pt>20.) && inAcceptance && (innerHits<=1) && 
	(isolation<0.1) && (!isConv) && 
	((isEB && sihih<0.01 && Dphi<0.8 && Deta<0.007) || 
	 (isEE && sihih<0.03 && Dphi<0.07 && Deta<0.005)); 

      isQCD = (pt>20.) && inAcceptance && (innerHits<=1) && 
	(isolation>0.1) && (!isConv) && 
	((isEB && sihih<0.01 && Dphi<0.8 && Deta<0.007) || 
	 (isEE && sihih<0.03 && Dphi<0.07 && Deta<0.005)); 
    }
    //-------- if MVA-based ID -----------------
      isTight = (pt>30.) && ( 
        ( abseta <= 0.8 && mvaOut > 0.977&& pf_isolation < 0.093) ||
        ( abseta > 0.8 && abseta <= 1.479 && mvaOut > 0.956 && pf_isolation < 0.095) ||
        ( abseta > 1.479 && mvaOut > 0.966 && pf_isolation < 0.171) );

      isLoose = (pt>20.) && ( 
        ( abseta <= 0.8 && mvaOut > 0.877 && pf_isolation < 0.177 ) ||
        ( abseta > 0.8 && abseta <= 1.479 && mvaOut > 0.794 && pf_isolation <0.180 ) ||
        ( abseta > 1.479 && mvaOut > 0.846 && pf_isolation < 0.244) );

      isQCD = (pt>20.) && ( 
        ( abseta <= 0.8 && mvaOut > -1. && pf_isolation > 0.177 ) ||
        ( abseta > 0.8 && abseta <= 1.479 && mvaOut > -1. && pf_isolation >0.180 ) ||
        ( abseta > 1.479 && mvaOut > -1. && pf_isolation > 0.244) );

    /// ------- Finally apply selection --------
    if(applyTightID_ && isTight) isPassing[iElec]= true;
    if(applyLooseID_ && isLoose) isPassing[iElec]= true;
    if(applyQcdID_ && isQCD) isPassing[iElec]= true;
 }


   
  unsigned int counter=0;
  typename edm::View<T>::const_iterator tIt, endcands = electrons->end();
  for (tIt = electrons->begin(); tIt != endcands; ++tIt, ++counter) {
    if(isPassing[counter]) passingElectrons->push_back( *tIt );  
  }

  nTot_  +=electrons->size();
  nPassed_+=passingElectrons->size();

  delete [] isPassing;  
  iEvent.put(passingElectrons);
}


//______________________________________________________________________________
template<typename T>
void ElectronIdSelector<T>::endJob()
{
  std::stringstream ss;
  ss<<"nTot="<<nTot_<<" nPassed="<<nPassed_
    <<" effPassed="<<100.*(nPassed_/(double)nTot_)<<"%\n";
  std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"
	   <<"\n"<<moduleLabel_<<"(ElectronIdSelector) SUMMARY:\n"<<ss.str()
	   <<"++++++++++++++++++++++++++++++++++++++++++++++++++"
	   << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////
typedef ElectronIdSelector<reco::GsfElectron>     GsfElectronIdSelector;
typedef ElectronIdSelector<pat::Electron>         PATElectronIdSelector;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GsfElectronIdSelector);
DEFINE_FWK_MODULE(PATElectronIdSelector);
