

package so.global.statistic;

simple GlobalStatistic
{
   parameters:

      string gstatLog = default("..//results//gstatlog.txt");

      // --- Signals
        @signal[Signal_ChunkHit](type="long");
        @signal[Signal_ChunkMiss](type="long");
        @signal[Signal_ChunkNeed](type="long");
        
        // --- Statistics
        @statistic[ChunkHit](source=Signal_ChunkHit; record=count);
        @statistic[ChunkMiss](source=Signal_ChunkMiss; record=count);
        @statistic[ChunkSeek](source=Signal_ChunkSeek; record=count);
        @statistic[Rebuffering](source=Signal_Rebuffering; record=count);
        @statistic[Stall](source=Signal_Stall; record=count);
        
        //@statistic[CI](source="count(Signal_ChunkHit)/(count(Signal_ChunkHit)+count(Signal_ChunkMiss))"; record=last);
        @statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkNeed)"; record=last);
        
        // -- Join time
        @statistic[MeshJoin](source=Signal_MeshJoin; record=vector);
        
        @statistic[NumberOfPartner](title="Number Of Partner"; source=Signal_NumberOfPartner; record=vector);

      @statistic[NumberOfJoin](title="Number Of Join"; source=Signal_nJoin; record=sum,vector);

        @statistic[requestedChunk](source=Signal_RequestedChunk; record=count);
        @statistic[receivedChunk](source=Signal_ReceivedChunk; record=count);
        @statistic[lateChunk](source=Signal_LateChunk; record=count);
        @statistic[inrangeChunk](source=Signal_InrangeChunk; record=count);
        @statistic[duplicatedChunk](source=Signal_DuplicatedChunk; record=count);

      @statistic[newCI](source="count(Signal_InrangeChunk)/count(Signal_ReceivedChunk)"; record=last);
//@statistic[newCI](source="count(Signal_InrangeChunk)/(count(Signal_ReceivedChunk)-count(Signal_DuplicatedChunk))"; record=last);

}
