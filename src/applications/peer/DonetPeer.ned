package simpleoverlay.applications.peer;

import simpleoverlay.applications.DonetBase;

simple DonetPeer extends DonetBase
{
    parameters:
        @class(DonetPeer);
        
        // Debug
        // bool moduleDebug = default(false);
        
        // Timer
        double startTime @unit(s);
        double bufferMapInterval        @unit(s) = default(0.5s);
        double chunkSchedulingInterval  @unit(s) = default(0.5s);
        
        double interval_bufferMap       @unit(s) = default(0.5s); // to replace the above params
        double interval_chunkScheduling @unit(s) = default(1.0s); // to replace the above params
        double interval_findMorePartner @unit(s) = default(1.0s);
        
        double interval_startPlayer     @unit(s) = default(0.5s); // 
        
        // double keepAliveInterval        @unit(s) = default(1s);
        
        // Thresholds
        // int nNeighbor = default(4);
        // int threshold_nNeighbor = default(1);
        
        // The following two are just for testing
        int nNeighbor_SchedulingStart = default(1);
        double waitingTime_SchedulingStart @unit(s) = default(0.25s);
        
        int numberOfPartner = default(4);
        
        // -- Possible signals to collect
        @signal[chunkRequestSeqNum](type=long);
        
        // -- Statistics
//        @statistic[chunkRequestSeqNum](title="Sequence Number of chunk requests"; record=count, vector);
        
//        @statistic[nPartner](source=Signal_nPartner; record=last, vector);
        
//        @statistic[partnerRequest](title="Partnership Request"; source=Signal_PartnerRequest; record=vector);
        
}


