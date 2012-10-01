package simpleoverlay.applications.peer;

import simpleoverlay.applications.DonetBase;

simple DonetPeer extends DonetBase
{
    parameters:
        @class(DonetPeer);
        
        // Debug
        // bool moduleDebug = default(false);
        
        // Timer
        volatile double startTime @unit(s);
        
        double interval_chunkScheduling @unit(s) = default(0.0s); // to replace the above params
        double interval_findMorePartner @unit(s) = default(0.0s);
        
        double interval_startPlayer     @unit(s) = default(0.5s); // 

        double interval_rejoin               @unit(s) = default(1s);
        double interval_timeout_joinReqAck   @unit(s) = default(1s);
        
        // double keepAliveInterval        @unit(s) = default(1s);
        
        // Thresholds
        // int nNeighbor = default(4);
        // int threshold_nNeighbor = default(1);
        
        // The following two are just for testing
        int nNeighbor_SchedulingStart = default(1);
        double waitingTime_SchedulingStart @unit(s) = default(0.25s);
        double factor_requestList = default(1);
        
        double requestFactor_moderate = default(1.0);
        double requestFactor_aggressive = default(2.0);
        double threshold_scarity = default(1);
        
        int maxNOP = default(2); // Max number of partners
        int offsetNOP = default(1); // The difference between minNOP and maxNOP

         // ------------------------------ Statistics ------------------------------------
        
        // -- Possible signals to collect
        @signal[Signal_chunkRequest](type=long);
        @statistic[chunkRequest](title="Sequence Number of chunk requests"; source=Signal_chunkRequest; record=count, vector);

        @signal[Signal_nNewChunkForRequestPerCycle](type=long);    
        @statistic[newChunkForRequest](title="Number of new chunk for scheduling per cycle"; source=Signal_nNewChunkForRequestPerCycle; record=vector);
        
        @signal[Signal_nPartner](type=integer);    
        @statistic[nPartner](title="Number of partners"; source=Signal_nPartner; record=last,vector);            

        // -- Trying new signals
        @statistic[joinTime](title="Joining time of a individual peer"; source=Signal_joinTime; record=last,vector?);            
        
        @statistic[playerStartTime](title="Start time of player"; source=Signal_playerStartTime; record=last,vector?);            
        
        @statistic[pRequestSent](title="Partnership request sent"; source=Signal_pRequestSent; record=count,last,vector);            
        
        @statistic[pRejectReceived](title="Partnership reject received"; source=Signal_pRejectReceived; record=count,last,vector);

      @statistic[NumberOfJoin](title="Number of Joins"; source=Signal_Join; record=count,vector);

}


