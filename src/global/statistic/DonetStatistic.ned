

package so.global.statistic;

simple DonetStatistic like IStatisticBase
{
   parameters:

      string gstatLog = default("..//results//gstatlog.txt");
      double interval_reportCI @unit(s) = default(0.5s);

      // --- Signals
        @signal[Signal_ChunkHit](type="long");
        @signal[Signal_ChunkMiss](type="long");
        @signal[Signal_ChunkNeed](type="long");
        
        // --- Statistics
        @statistic[ChunkHit](source=Signal_ChunkHit; record=count);
        @statistic[ChunkMiss](source=Signal_ChunkMiss; record=count);
        @statistic[ChunkSeek](title="Chunk Seek"; source=Signal_ChunkSeek; record=count);
        @statistic[Rebuffering](source=Signal_Rebuffering; record=count);
        @statistic[Stall](source=Signal_Stall; record=count);
        
        //@statistic[CI](source="count(Signal_ChunkHit)/(count(Signal_ChunkHit)+count(Signal_ChunkMiss))"; record=last,vector);
        //@statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkNeed)"; record=last,vector);

        // works
        //@statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkSeek)"; record=last,vector);

        // -- Join time
        @statistic[MeshJoin](source=Signal_MeshJoin; record=vector);
        
        @statistic[NumberOfPartner](title="Number Of Partner"; source=Signal_NumberOfPartner; record=vector);

      @statistic[NumberOfJoin](title="Number Of Join"; source=Signal_nJoin; record=sum,vector);

        @statistic[requestedChunk](source=Signal_RequestedChunk; record=count);
        @statistic[receivedChunk](source=Signal_ReceivedChunk; record=count);
        @statistic[lateChunk](source=Signal_LateChunk; record=count);
        @statistic[inrangeChunk](source=Signal_InrangeChunk; record=count);
        @statistic[duplicatedChunk](source=Signal_DuplicatedChunk; record=count);

      //@statistic[newCI](source="count(Signal_InrangeChunk)/count(Signal_ReceivedChunk)"; record=last);
      //@statistic[newCI](source="count(Signal_InrangeChunk)/(count(Signal_ReceivedChunk)-count(Signal_DuplicatedChunk))"; record=last);


        // -------------------------- Starting from here -----------------------
        // new stat - periodically
        // @statistic[CI](source="Signal_CI"; record=last,vector);
        @statistic[globalCI](source="Signal_CI"; record=last,mean,vector);

        @statistic[SkipChunk](source="Signal_SkipChunk"; record=last,vector);

        @statistic[StallDuration](source="Signal_StallDuration"; record=last,vector);

        @statistic[Rebuffering](source="Signal_Rebuffering"; record=last,vector);


}
