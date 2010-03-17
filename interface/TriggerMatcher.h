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
 * Description: perform trigger matching with RECO object
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef TRIGGERMATCHER_H
#define TRIGGERMATCHER_H

// CMS includes
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"


namespace ewk {

  class TriggerMatcher {
  public:

    TriggerMatcher(const edm::ParameterSet iConfig ) {
      // get appropriate trigger object names / labels
      const edm::InputTag dSummaryObj( "hltTriggerSummaryAOD","","HLT" );
      triggerSummaryLabel_ = 
	iConfig.getUntrackedParameter<edm::InputTag>("triggerSummaryLabel",dSummaryObj); 
      const edm::InputTag dHLTTag("HLT_Ele15_LW_L1R", "","HLT");
      hltTag_ = iConfig.getUntrackedParameter<edm::InputTag>("hltTag",dHLTTag);
    }
    

    /// default constructor
    TriggerMatcher() {};


    /// Destructor, does nothing 
    ~TriggerMatcher() {};


  protected:
    edm::InputTag  triggerSummaryLabel_;
    edm::InputTag  hltTag_;
    HLTConfigProvider hltConfig_;


  public:

    /// To be called once per event / candidate
    bool CheckTriggerMatch( const edm::Event& ev, double eta, double phi) {

      // ------------ trigger objects 
      edm::Handle<trigger::TriggerEvent> triggerObj;
      ev.getByLabel(triggerSummaryLabel_,triggerObj); 
      if(!triggerObj.isValid()) { 
	edm::LogInfo("TriggerEvent") << " objects not found"; 
      }

      edm::InputTag filterTag;
      bool changed = false;
      if(! hltConfig_.init(ev, hltTag_.process(), changed) ){
	edm::LogError("TriggerMatcher") << "Error! Can't initialize HLTConfigProvider ";
	return false;
      }


      std::vector<std::string> filters = hltConfig_.moduleLabels( hltTag_.label() );

      // loop over all trigger filters associated with this path
      for(std::vector<std::string>::iterator filter =
	    filters.begin(); filter!= filters.end(); ++filter ) {
       
	edm::InputTag testTag(*filter,"", hltTag_.process() );       
	int testindex = triggerObj->filterIndex(testTag);
	if ( !(testindex >= triggerObj->sizeFilters()) ) 
	  filterTag=testTag;
      }
     
      int trigindex = triggerObj->filterIndex( filterTag );
      if ( trigindex >= triggerObj->sizeFilters() ) return false; 

      const trigger::Keys & l1k = triggerObj->filterKeys( trigindex );
      if ( l1k.size() <= 0 ) return false; 

      /////// ---------- do the actual trigger match    
      bool result = false;
      const trigger::TriggerObjectCollection & toc(triggerObj->getObjects());
      for (trigger::Keys::const_iterator ki = l1k.begin(); ki !=l1k.end(); ++ki ) {
    
	result = reco::deltaR( eta, phi, toc[*ki].eta(),toc[*ki].phi() ) < 0.3; 
	if( result) break;             
      }
  
      return result;
    }
  };


}
#endif


