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
// DonetStatistic.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


package so.global.statistic;

//import package so.global.StreamingStatistic;

//simple DonetStatistic like IStatisticBase
//simple DonetStatistic extends StreamingStatistic
simple DonetStatistic like IStreamingStatistic
{
   parameters:

      string gstatLog = default("..//results//gstatlog.txt");
      double interval_reportCI @unit(s) = default(0.5s);
      double interval_reportSystemSize @unit(s) = default(0.5s);


// -----------------------------------------------------------------------------
// --- Statistics
// -----------------------------------------------------------------------------
        //@statistic[ChunkHit](source=Signal_ChunkHit; record=count);
        //@statistic[ChunkMiss](source=Signal_ChunkMiss; record=count);
        //@statistic[ChunkSeek](title="Chunk Seek"; source=Signal_ChunkSeek; record=count);
        //@statistic[Rebuffering](source=Signal_Rebuffering; record=count);
        //@statistic[Stall](source=Signal_Stall; record=count);
        
        //@statistic[CI](source="count(Signal_ChunkHit)/(count(Signal_ChunkHit)+count(Signal_ChunkMiss))"; record=last,vector);
        //@statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkNeed)"; record=last,vector);

        // works
        //@statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkSeek)"; record=last,vector);

        // -- Join time
        //@statistic[MeshJoin](source=Signal_MeshJoin; record=vector);
        
        //@statistic[NumberOfPartner](title="Number Of Partner"; source=Signal_NumberOfPartner; record=vector);

      //@statistic[NumberOfJoin](title="Number Of Join"; source=Signal_nJoin; record=sum,vector);

        //@statistic[requestedChunk](source=Signal_RequestedChunk; record=count);
        //@statistic[receivedChunk](source=Signal_ReceivedChunk; record=count);
        //@statistic[lateChunk](source=Signal_LateChunk; record=count);
        //@statistic[inrangeChunk](source=Signal_InrangeChunk; record=count);
        //@statistic[duplicatedChunk](source=Signal_DuplicatedChunk; record=count);

      //@statistic[newCI](source="count(Signal_InrangeChunk)/count(Signal_ReceivedChunk)"; record=last);
      //@statistic[newCI](source="count(Signal_InrangeChunk)/(count(Signal_ReceivedChunk)-count(Signal_DuplicatedChunk))"; record=last);


        // -------------------------- Starting from here -----------------------
        // new stat - periodically
        // @statistic[CI](source="Signal_CI"; record=last,vector);
        @statistic[globalCI](source="Signal_CI"; record=last,mean,vector);

        // To get the number active peers in the system
        @statistic[systemSize](source="Signal_SystemSize"; record=mean,vector);

        //@statistic[SkipChunk](source="Signal_SkipChunk"; record=last,vector);

        //@statistic[StallDuration](source="Signal_StallDuration"; record=last,vector);

        //@statistic[Rebuffering](source="Signal_Rebuffering"; record=last,vector);


}
