//  
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// DonetPeer.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

package so.mesh.donet;

import so.mesh.donet.DonetBase;

simple DonetPeer extends DonetBase
{
    parameters:
        @class(DonetPeer);
        
        // Debug
        // bool moduleDebug = default(false);
        
        // Timer
        volatile double startTime @unit(s);
        
        double interval_chunkScheduling             @unit(s) = default(0.0s); // to replace the above params
        double interval_findMorePartner             @unit(s) = default(0.0s);
        double interval_waitingPartnershipResponse  @unit(s) = default(0.5s);
        
        double interval_startPlayer             @unit(s) = default(0.5s); //

        double interval_rejoin                  @unit(s) = default(1s);
        double interval_timeout_joinReqAck      @unit(s) = default(1s);

        double interval_partnershipRefinement   @unit(s) = default(0.5s);
        double interval_partnerlistCleanup      @unit(s) = default(0.5s);

        double interval_reportStatistic         @unit(s) = default(30s);

        double interval_reportActive            @unit(s) = default(0.1s);
        
        // double keepAliveInterval             @unit(s) = default(1s);
        
        // Thresholds
        // int nNeighbor = default(4);
        // int threshold_nNeighbor = default(1);
        // double threshold_idleDuration_buffermap @unit(s) = default(0.5s);
        
        // The following two are just for testing
        int nNeighbor_SchedulingStart = default(1);
        double waitingTime_SchedulingStart @unit(s) = default(0.25s);
        double factor_requestList = default(1);
        
        double requestFactor_moderate = default(1.0);
        double requestFactor_aggressive = default(2.0);
        double threshold_scarity = default(1);
        
        int maxNOP = default(2); // Max number of partners
        int offsetNOP = default(1); // The difference between minNOP and maxNOP

        // ---------------------------------------------------------------------
        // ------------------------------ Statistics ---------------------------
        // ---------------------------------------------------------------------

        // -- For debug
        // @statistic[localCI](source="Signal_LocalCI"; record=min,max,mean,last,vector);

        // -- Old metrics
        // -- Possible signals to collect
        @signal[Signal_chunkRequest](type=long);
        //@statistic[chunkRequest](title="Sequence Number of chunk requests"; source=Signal_chunkRequest; record=count, vector);


        // -- title="Number of new chunk for scheduling per cycle";
        //@statistic[newChunkForRequest](source=Signal_nNewChunkForRequestPerCycle; record=vector);

        // -- title="Number of requested chunks per cycle";
        //@statistic[nChunkRequested](source=Signal_nChunkRequested; record=mean,vector);
        
        // -- title="Number of partners";
        //@statistic[nPartner](source=Signal_nPartner; record=last,vector);

        // -- Trying new signals
        //@statistic[joinTime](title="Joining time of a individual peer"; source=Signal_joinTime; record=last,vector?);            
        
        //@statistic[playerStartTime](title="Start time of player"; source=Signal_playerStartTime; record=last,vector?);            
        
        //@statistic[pRejectReceived](title="Partnership reject received"; source=Signal_pRejectReceived; record=count,last,vector);

      //@statistic[NumberOfJoin](title="Number of Joins"; source=Signal_Join; record=count);

// ----------- Partnership request --------------
    //@statistic[pRequestSent](title="Partnership request sent"; source=Signal_pRequestSent; record=count,vector);
    //@statistic[pRequestRecv](title="Partnership request received"; source=Signal_pRequestRecv; record=count,vector);
    //@statistic[pRejectSent](title="Partnership reject sent"; source=Signal_pRejectRecv; record=count,vector);
    //@statistic[pRejectRecv](title="Partnership reject received"; source=Signal_pRejectRecv; record=count,vector);
    //@statistic[pRequestRecv_whileWaiting](title="Number of timeout events"; source=Signal_pRequestRecv_whileWaiting; record=count,vector);


// ----------- Ranges of the received Buffer maps and current playback point
    //@statistic[minStart](source=Signal_MinStart; record=vector);
    //@statistic[maxStart](source=Signal_MaxStart; record=vector);
    //@statistic[minHead](source=Signal_MinHead; record=vector);
    //@statistic[maxHead](source=Signal_MaxHead; record=vector);
    //@statistic[curPbPoint](source=Signal_CurrentPlaybackPoint; record=vector);
    //@statistic[bufferStart](source=Signal_BufferStart; record=vector);
    //@statistic[bufferHead](source=Signal_BufferHead; record=vector);

    //@statistic[schedWinStart](source=Signal_SchedWin_start; record=vector);
    //@statistic[schedWinEnd](source=Signal_SchedWin_end; record=vector);

    //@statistic[inThroughput](source=Signal_InThroughput; record=mean,vector);

    //@statistic[rateBufferMapReceived](source=Signal_nBufferMapReceived; record=mean,vector);


    // Template::
    //@statistic[n]();

}



