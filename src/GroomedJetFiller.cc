/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VplusJetSubstructure
 *
 *
 * Authors:
 *   Nhan V Tran, Fermilab - kalanand@fnal.gov
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   To fill groomed jet related quantities into a specified TTree
 *   Works with jets in PAT data format.
 * History:
 *   
 *
 * Copyright (C) 2012 FNAL 
 *****************************************************************************/


    // user include files
#include "ElectroWeakAnalysis/VPlusJets/interface/GroomedJetFiller.h" 

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 


#include "DataFormats/PatCandidates/interface/Jet.h"

#include <fastjet/ClusterSequence.hh>
//#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/GhostedAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/GhostedAreaSpec.hh"

#include "ElectroWeakAnalysis/VPlusJets/interface/Nsubjettiness.h"

ewk::GroomedJetFiller::GroomedJetFiller(const char *name, 
					TTree* tree, 
					const std::string jetLabel,
                                        const std::string srcGroomedJet,
					const edm::ParameterSet& iConfig)
{
  tree_     = tree;
  jetLabel_ = jetLabel;

  // Declare all the branches of the tree
  SetBranch( jetpt_uncorr, "GroomedJet_" + jetLabel_ + "_pt_uncorr");
  SetBranch( jetmass_uncorr, "GroomedJet_" + jetLabel_ + "_mass_uncorr");
  SetBranch( jetmass_tr_uncorr, "GroomedJet_" + jetLabel_ + "_mass_tr_uncorr");
  SetBranch( jetmass_ft_uncorr, "GroomedJet_" + jetLabel_ + "_mass_ft_uncorr");
  SetBranch( jetmass_pr_uncorr, "GroomedJet_" + jetLabel_ + "_mass_pr_uncorr");
  SetBranch( tau2tau1, "GroomedJet_" + jetLabel_ + "_tau2tau1");
  SetBranch( tau1, "GroomedJet_" + jetLabel_ + "_tau1");
  SetBranch( tau2, "GroomedJet_" + jetLabel_ + "_tau2");
  SetBranch( tau3, "GroomedJet_" + jetLabel_ + "_tau3");
  SetBranch( tau4, "GroomedJet_" + jetLabel_ + "_tau4");
  SetBranch( massdrop_pr_uncorr, "GroomedJet_" + jetLabel_ + "_massdrop_pr_uncorr");

  SetBranch( jetpt, "GroomedJet_" + jetLabel_ + "_pt");
  SetBranch( jeteta, "GroomedJet_" + jetLabel_ + "_eta");
  SetBranch( jetphi, "GroomedJet_" + jetLabel_ + "_phi");
  SetBranch( jete, "GroomedJet_" + jetLabel_ + "_e");
  SetBranch( jetpt_tr_uncorr, "GroomedJet_" + jetLabel_ + "_pt_tr_uncorr");
  SetBranch( jetpt_tr, "GroomedJet_" + jetLabel_ + "_pt_tr");
  SetBranch( jeteta_tr, "GroomedJet_" + jetLabel_ + "_eta_tr");
  SetBranch( jetphi_tr, "GroomedJet_" + jetLabel_ + "_phi_tr");
  SetBranch( jete_tr, "GroomedJet_" + jetLabel_ + "_e_tr");
  SetBranch( jetpt_ft_uncorr, "GroomedJet_" + jetLabel_ + "_pt_ft_uncorr");
  SetBranch( jetpt_ft, "GroomedJet_" + jetLabel_ + "_pt_ft");
  SetBranch( jeteta_ft, "GroomedJet_" + jetLabel_ + "_eta_ft");
  SetBranch( jetphi_ft, "GroomedJet_" + jetLabel_ + "_phi_ft");
  SetBranch( jete_ft, "GroomedJet_" + jetLabel_ + "_e_ft");
  SetBranch( jetpt_pr_uncorr, "GroomedJet_" + jetLabel_ + "_pt_pr_uncorr");
  SetBranch( jetpt_pr, "GroomedJet_" + jetLabel_ + "_pt_pr");
  SetBranch( jeteta_pr, "GroomedJet_" + jetLabel_ + "_eta_pr");
  SetBranch( jetphi_pr, "GroomedJet_" + jetLabel_ + "_phi_pr");
  SetBranch( jete_pr, "GroomedJet_" + jetLabel_ + "_e_pr");

  SetBranch( jetmass, "GroomedJet_" + jetLabel_ + "_mass");
  SetBranch( jetmass_tr, "GroomedJet_" + jetLabel_ + "_mass_tr");
  SetBranch( jetmass_ft, "GroomedJet_" + jetLabel_ + "_mass_ft");
  SetBranch( jetmass_pr, "GroomedJet_" + jetLabel_ + "_mass_pr");
  SetBranch( massdrop_pr, "GroomedJet_" + jetLabel_ + "_massdrop_pr");
  SetBranch( jetarea, "GroomedJet_" + jetLabel_ + "_area");
  SetBranch( jetarea_tr, "GroomedJet_" + jetLabel_ + "_area_tr");
  SetBranch( jetarea_ft, "GroomedJet_" + jetLabel_ + "_area_ft");
  SetBranch( jetarea_pr, "GroomedJet_" + jetLabel_ + "_area_pr");

 
  ////////////////////////////////////
  // CORRECTIONS ON THE FLY
  ////////////////////////////////////     
  //// --- groomed jet label -------
  if(  iConfig.existsAs<std::string>("srcGroomedJet") )
    mGroomedJet = iConfig.getParameter<std::string>("srcGroomedJet"); 
  else mGroomedJet =  "pfInputsCA8";

  mGroomedJet =  srcGroomedJet;

  // --- Are we running over Monte Carlo ? --- 
  if( iConfig.existsAs<bool>("runningOverMC") ) 
    runningOverMC_=iConfig.getParameter< bool >("runningOverMC");
  else runningOverMC_= false;

  // --- Are we applying AK7 JEC to our groomed jet ? --- 
  if( iConfig.existsAs<bool>("applyJECToGroomedJets") ) 
    applyJECToGroomedJets_=iConfig.getParameter< bool >("applyJECToGroomedJets");
  else applyJECToGroomedJets_ = false;



  //// --- fastjet rho label -------
  JetsFor_rho =  iConfig.getParameter<std::string>("srcJetsforRho") ; 
  if(applyJECToGroomedJets_)
    JEC_GlobalTag_forGroomedJet 
      =  iConfig.getParameter<std::string>("JEC_GlobalTag_forGroomedJet") ; 


  //// --- primary vertex -------
  if(  iConfig.existsAs<edm::InputTag>("srcPrimaryVertex") )
    mPrimaryVertex = iConfig.getParameter<edm::InputTag>("srcPrimaryVertex"); 
  else mPrimaryVertex =  edm::InputTag("offlinePrimaryVertices");



  // ---- setting up the jec on-the-fly from text files...    
  std::string fDir = "JEC/" + JEC_GlobalTag_forGroomedJet;   
  std::vector< JetCorrectorParameters > jecPars;
  std::vector< std::string > jecStr;
   
  if(applyJECToGroomedJets_) {
    jecStr.push_back( fDir + "_L1FastJet_AK7PFchs.txt" );
    jecStr.push_back( fDir + "_L2Relative_AK7PFchs.txt" );
    jecStr.push_back( fDir + "_L3Absolute_AK7PFchs.txt" );
    if (!runningOverMC_)
      jecStr.push_back( fDir + "_L2L3Residual_AK7PFchs.txt" );


    for (unsigned int i = 0; i < jecStr.size(); ++i){
      JetCorrectorParameters* ijec = new JetCorrectorParameters( jecStr[i] );
      jecPars.push_back( *ijec );
    }
    
    jec_ = new FactorizedJetCorrector(jecPars);
    jecUnc_ = new JetCorrectionUncertainty( fDir + "_Uncertainty_AK7PFchs.txt" );
  }

}




//////////////////////////////////////////////////////////////////
/////// Helper for above function ////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ewk::GroomedJetFiller::SetBranchSingle( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/F").c_str() );
  bnames.push_back( name );
}

void ewk::GroomedJetFiller::SetBranchSingle( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/I").c_str() );
  bnames.push_back( name );
}

void ewk::GroomedJetFiller::SetBranch( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6]/F").c_str() );
  bnames.push_back( name );
}


void ewk::GroomedJetFiller::SetBranch( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"[6]/I").c_str() );
  bnames.push_back( name );
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////





// ------------ method called to produce the data  ------------
void ewk::GroomedJetFiller::fill(const edm::Event& iEvent) {
    
  ////----------
  // init
  for (int j =0; j< NUM_JET_MAX; ++j) {
    jetmass_uncorr[j] = -1.;
    jetmass_tr_uncorr[j] = -1.;
    jetmass_ft_uncorr[j] = -1.;
    jetmass_pr_uncorr[j] = -1.;
    tau2tau1[j] = -1.;
    tau1[j] = -1.;
    tau2[j] = -1.;
    tau3[j] = -1.;
    tau4[j] = -1.;
    massdrop_pr_uncorr[j] = -1.; 
    jetpt_uncorr[j] = -1.;
    jetpt[j] = -1.;
    jeteta[j] = -10.;
    jetphi[j] = -10.;
    jete[j] = -1.;
    jetmass[j] = -1.;
    jetmass_tr[j] = -1.;
    jetmass_ft[j] = -1.;
    jetmass_pr[j] = -1.;
    jetarea[j] = -1.;
    jetarea_tr[j] = -1.;
    jetarea_ft[j] = -1.;
    jetarea_pr[j] = -1.;    
    massdrop_pr[j] = -1.;
    jetpt_tr_uncorr[j] = -1.;
    jetpt_tr[j] = -1.;
    jeteta_tr[j] = -10.;
    jetphi_tr[j] = -10.;
    jete_tr[j] = -1.;
    jetpt_ft_uncorr[j] = -1.;
    jetpt_ft[j] = -1.;
    jeteta_ft[j] = -10.;
    jetphi_ft[j] = -10.;
    jete_ft[j] = -1.;
    jetpt_pr_uncorr[j] = -1.;
    jetpt_pr[j] = -1.;
    jeteta_pr[j] = -10.;
    jetphi_pr[j] = -10.;
    jete_pr[j] = -1.;
  }


  // ----- get handles... --------    
  std::string pfinput_ = mGroomedJet;    
  edm::Handle< std::vector<float> > PF_px_handle;
  edm::Handle< std::vector<float> > PF_py_handle;
  edm::Handle< std::vector<float> > PF_pz_handle;
  edm::Handle< std::vector<float> > PF_en_handle;
    
  iEvent.getByLabel( pfinput_, "px" ,    PF_px_handle);
  iEvent.getByLabel( pfinput_, "py" ,    PF_py_handle);
  iEvent.getByLabel( pfinput_, "pz" ,    PF_pz_handle);
  iEvent.getByLabel( pfinput_, "energy", PF_en_handle);
    
    
  // ------ get rho --------    
  rhoVal_ = -99.;
  edm::Handle<double> rho;
  const edm::InputTag eventrho(JetsFor_rho, "rho");
  iEvent.getByLabel(eventrho,rho);
  rhoVal_ = *rho;
    
  // ------ get nPV: primary/secondary vertices------ 
  nPV_ = 0.;
  double nPVval = 0;
  edm::Handle <edm::View<reco::Vertex> > recVtxs;
  iEvent.getByLabel( mPrimaryVertex, recVtxs);
  for(unsigned int ind=0;ind<recVtxs->size();ind++){
    if (!((*recVtxs)[ind].isFake()) && ((*recVtxs)[ind].ndof()>=4) 
	&& (fabs((*recVtxs)[ind].z())<=24.0) &&  
	((*recVtxs)[ind].position().Rho()<=2.0) ) {
      nPVval += 1;
    }
  }
  nPV_ = nPVval;
    

  // ----------------------------
  // ------ start processing ------    
  std::vector<fastjet::PseudoJet> FJparticles;
  for (unsigned i = 0; i < PF_px_handle->size() ; i++){
    FJparticles.push_back( fastjet::PseudoJet( PF_px_handle->at(i), 
					       PF_py_handle->at(i), 
					       PF_pz_handle->at(i), 
					       PF_en_handle->at(i) ) );
  }
  // std::cout << "FJparticles.size() = " << FJparticles.size() << std::endl;
  if (FJparticles.size() < 1) return;

  

  // do re-clustering
  fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, 0.8);
  int activeAreaRepeats = 1;
  double ghostArea = 0.01;
  double ghostEtaMax = 5.0;
  // fastjet::ActiveAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
  fastjet::GhostedAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
  fjActiveArea.set_fj2_placement(true);
  fastjet::AreaDefinition fjAreaDefinition( fastjet::active_area_explicit_ghosts, fjActiveArea );
  fastjet::ClusterSequenceArea thisClustering(FJparticles, jetDef, fjAreaDefinition);
    
  std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(0.0));
    
  // define groomers
  fastjet::Filter trimmer( fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.03)));
  fastjet::Filter filter( fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.3), fastjet::SelectorNHardest(3)));
  fastjet::Pruner pruner(fastjet::cambridge_algorithm, 0.1, 0.5);
    
  std::vector<fastjet::Transformer const *> transformers;
  transformers.push_back(&trimmer);
  transformers.push_back(&filter);
  transformers.push_back(&pruner);
    
  // define n-subjettiness
  NsubParameters paraNsub = NsubParameters(1.0, 0.8);   
  Nsubjettiness routine(nsub_kt_axes, paraNsub);

  for (unsigned j = 0; j < out_jets.size(); j++) {
            
    if( !(j< (unsigned int) NUM_JET_MAX) ) break;            
    jetmass_uncorr[j] = out_jets.at(j).m();
    jetpt_uncorr[j] = out_jets.at(j).pt();
    TLorentzVector jet_corr = getCorrectedJet(out_jets.at(j));
    jetmass[j] = jet_corr.M();
    jetpt[j] = jet_corr.Pt();
    jeteta[j] = jet_corr.Eta();
    jetphi[j] = jet_corr.Phi();
    jete[j]   = jet_corr.Energy();
    jetarea[j] = out_jets.at(j).area();
                
    // pruning, trimming, filtering  
    int transctr = 0;
    for ( std::vector<fastjet::Transformer const *>::const_iterator 
	    itransf = transformers.begin(), itransfEnd = transformers.end(); 
	  itransf != itransfEnd; ++itransf ) {  
                                        
      fastjet::PseudoJet transformedJet = out_jets.at(j);
      transformedJet = (**itransf)(transformedJet);
                                        
      if (transctr == 0){ // trimmed
	jetmass_tr_uncorr[j] = transformedJet.m();
	jetpt_tr_uncorr[j] = transformedJet.pt();
	TLorentzVector jet_tr_corr = getCorrectedJet(transformedJet);
	jetmass_tr[j] = jet_tr_corr.M();
	jetpt_tr[j] = jet_tr_corr.Pt();
	jeteta_tr[j] = jet_tr_corr.Eta();
	jetphi_tr[j] = jet_tr_corr.Phi();
	jete_tr[j]   = jet_tr_corr.Energy();
	jetarea_tr[j] = transformedJet.area();
      }
      else if (transctr == 1){ // filtered
	jetmass_ft_uncorr[j] = transformedJet.m();
	jetpt_ft_uncorr[j] = transformedJet.pt();
	TLorentzVector jet_ft_corr = getCorrectedJet(transformedJet);
	jetmass_ft[j] = jet_ft_corr.M();
	jetpt_ft[j] = jet_ft_corr.Pt();
	jeteta_ft[j] = jet_ft_corr.Eta();
	jetphi_ft[j] = jet_ft_corr.Phi();
	jete_ft[j]   = jet_ft_corr.Energy();
	jetarea_ft[j] = transformedJet.area();                    
      }
      else if (transctr == 2){ // pruned
	jetmass_pr_uncorr[j] = transformedJet.m();
	jetpt_pr_uncorr[j] = transformedJet.pt();
	TLorentzVector jet_pr_corr = getCorrectedJet(transformedJet);
	jetmass_pr[j] = jet_pr_corr.M();
	jetpt_pr[j] = jet_pr_corr.Pt();
	jeteta_pr[j] = jet_pr_corr.Eta();
	jetphi_pr[j] = jet_pr_corr.Phi();
	jete_pr[j]   = jet_pr_corr.Energy();
	jetarea_pr[j] = transformedJet.area();                    
	//decompose into requested number of subjets:
	if (transformedJet.constituents().size() > 1){
	  std::vector<fastjet::PseudoJet> subjets = 
	    transformedJet.associated_cluster_sequence()->exclusive_subjets(transformedJet,2);    
	  // for (unsigned k = 0; k < subjets.size(); k++) {
	  //  std::cout << "subjet " << k << ": mass = " << subjets.at(k).m() << " and pt = " << subjets.at(k).pt() << std::endl;
	  // }
	  massdrop_pr_uncorr[j] = subjets.at(0).m()/transformedJet.m();
	  massdrop_pr[j] = (subjets.at(0).m()/jetmass_pr[j]);                        
	}
      }
      else{ std::cout << "error in number of transformers" << std::endl;}                    
      transctr++;
    }        
                
                
    // n-subjettiness
    tau1[j] = routine.getTau(1, out_jets.at(j).constituents()); 
    tau2[j] = routine.getTau(2, out_jets.at(j).constituents());
    tau3[j] = routine.getTau(3, out_jets.at(j).constituents());
    tau4[j] = routine.getTau(4, out_jets.at(j).constituents());
    tau2tau1[j] = tau2[j]/tau1[j];                      
  }

}  



double ewk::GroomedJetFiller::getJEC(double curJetEta, 
				     double curJetPt, 
				     double curJetE, 
				     double curJetArea){
    
  // -------
  // Jet energy corrections, something like this...
  // -------
  jec_->setJetEta( curJetEta );
  jec_->setJetPt ( curJetPt );
  jec_->setJetE  ( curJetPt );
  jec_->setJetA  ( curJetArea );
  jec_->setRho   ( rhoVal_ );
  jec_->setNPV   ( nPV_ );
  double corr = jec_->getCorrection();
  return corr;
}




TLorentzVector ewk::GroomedJetFiller::getCorrectedJet(fastjet::PseudoJet& jet) {

  double jecVal = 1.0;

  if(applyJECToGroomedJets_) 
    getJEC( jet.eta(), jet.pt(), jet.e(), jet.area() );   
          
  TLorentzVector jet_corr(jet.px() * jecVal, 
			  jet.py() * jecVal, 
			  jet.pz() * jecVal, 
			  jet.e() * jecVal);
  return jet_corr;
}
