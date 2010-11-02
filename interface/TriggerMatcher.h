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
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
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
      triggerSummaryLabel_ = 
	iConfig.getParameter<edm::InputTag>("triggerSummaryLabel"); 
      hltTag_ = iConfig.getParameter<edm::InputTag>("hltTag");
      const edm::InputTag dTriggerResults("TriggerResults","","HLT");
      hlTriggerResults_ = iConfig.getUntrackedParameter<edm::InputTag>("hlTriggerResults", dTriggerResults);
    }
            
      /// default constructor
      TriggerMatcher() {};

    /// Destructor, does nothing 
    ~TriggerMatcher() {};


  protected:
    edm::InputTag  triggerSummaryLabel_;
    edm::InputTag  hltTag_;
    edm::InputTag  hlTriggerResults_;

  public:

    /// To be called once per event / candidate
    bool CheckTriggerMatch( const edm::Event& ev, edm::InputTag& filterName, 
			    double eta, double phi ) 
    {
      // ------------ trigger objects 
      edm::Handle<trigger::TriggerEvent> triggerObj;
      ev.getByLabel(triggerSummaryLabel_,triggerObj); 
      if(!(triggerObj.isValid())) { 
	edm::LogInfo("TriggerEvent") << " objects not found"; 
	return false;
      }

      int trigindex = triggerObj->filterIndex( filterName );
      if ( trigindex >= triggerObj->sizeFilters() ) return false; 

      // ------------ Make sure that the HLT path is not terminated early
      edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
      ev.getByLabel(hlTriggerResults_, hltTriggerResultHandle);
      if(!(hltTriggerResultHandle.isValid())) return false; 


      const edm::TriggerNames & trigNames = ev.triggerNames(*hltTriggerResultHandle);
      bool trigger_fired = false;      
      std::string hltPathName = hltTag_.label();
      for (unsigned int i=0; i<hltTriggerResultHandle->size(); i++) {
	std::string trigName = trigNames.triggerName(i);
	if ( trigName == hltPathName && hltTriggerResultHandle->accept(i)) 
	  trigger_fired = true;
      }   

      if( !trigger_fired ) return false; 



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


