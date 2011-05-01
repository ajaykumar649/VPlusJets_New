import FWCore.ParameterSet.Config as cms


##-------- Scraping veto --------
noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)



###############################
####### DAF PV's     ##########
###############################
offlinePrimaryVerticesDAF = cms.EDProducer("PrimaryVertexProducer",
   verbose = cms.untracked.bool(False),
   algorithm = cms.string('AdaptiveVertexFitter'),
   TrackLabel = cms.InputTag("generalTracks"),
   useBeamConstraint = cms.bool(False),
   beamSpotLabel = cms.InputTag("offlineBeamSpot"),
   minNdof  = cms.double(0.0),
   PVSelParameters = cms.PSet(
       maxDistanceToBeam = cms.double(1.0)
   ),
   TkFilterParameters = cms.PSet(
       algorithm=cms.string('filter'),
       maxNormalizedChi2 = cms.double(20.0),
       minPixelLayersWithHits=cms.int32(2),
       minSiliconLayersWithHits = cms.int32(5),
       maxD0Significance = cms.double(5.0), 
       minPt = cms.double(0.0),
       trackQuality = cms.string("any")
   ),

   TkClusParameters = cms.PSet(
       algorithm   = cms.string("DA"),
       TkDAClusParameters = cms.PSet(
           coolingFactor = cms.double(0.6),  #  moderate annealing speed
           Tmin = cms.double(4.),            #  end of annealing
           vertexSize = cms.double(0.01),    #  ~ resolution / sqrt(Tmin)
           d0CutOff = cms.double(3.),        # downweight high IP tracks 
           dzCutOff = cms.double(4.)         # outlier rejection after freeze-out (T<Tmin)
       )
   )
)


##-------- Primary vertex filter --------
primaryVertex = cms.EDFilter("VertexSelector",
    #src = cms.InputTag("offlinePrimaryVertices"),
    src = cms.InputTag("offlinePrimaryVerticesDAF"),                                     
    cut = cms.string("!isFake && ndof >= 4 && abs(z) <= 24 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
    filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)




TrackVtxPath = cms.Sequence(
    noscraping +
    offlinePrimaryVerticesDAF +
    primaryVertex
)
