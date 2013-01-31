package so.payload;

simple PlayerSimpleSkip like PlayerBase
{
   parameters:
      @class(PlayerSimpleSkip);


      double interval_recheckVideoBuffer @unit(s) = default(0.1s);
      double percent_buffer_low = default(0.0);
      double percent_buffer_high = default(1.0);
      int max_skipped_chunk = default(3);
      double interval_probe_playerStart @unit(s) = default(1s);
        
		// --- Possible signals to collect
        @signal[rebuffering_Local](type=long);
        @signal[Signal_ChunkHit](type=long);
        @signal[Signal_ChunkMiss](type=long);
        @signal[Signal_ChunkSeek](type=long);
		
		// --- Statistics
      //@statistic[ChunkHit](source=Signal_ChunkHit; title="Number of chunk hit"; record=count,vector);
      //@statistic[ChunkMiss](source=Signal_ChunkMiss; title="Number of chunk Miss"; record=count,vector);
      //@statistic[ChunkSeek](source=Signal_ChunkSeek; title="Number of chunk Seek"; record=count,vector);
      //@statistic[CI](title="Continuity Index"; source="count(Signal_ChunkHit)/count(Signal_ChunkSeek)"; record=last);

      //@statistic[rebuffering](title="Number of Re-buffering events"; source=Signal_Rebuffering; record=sum,vector);
      //@statistic[stall](title="Number of stall events"; source=Signal_Stall; record=sum,vector);

}


