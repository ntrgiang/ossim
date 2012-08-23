package simpleoverlay.applications.peer;

simple Player
{
    parameters:
        @class(Player); 
        
        double interval_recheckVideoBuffer @unit(s) = default(0.1s);
        
		// --- Possible signals to collect
		@signal[rebuffering_Local](type=long);
		@signal[Signal_ChunkHit](type=long);
		@signal[Signal_ChunkMiss](type=long);
		
		// --- Statistics
//        @statistic[rebuffering_local](title="Number of Re-buffering events"; source=rebuffering_Local; record=sum,vector);
//        @statistic[ChunkHit](source=Signal_ChunkHit; title="Number of chunk hit";record=count,vector);
//        @statistic[ChunkMiss](source=Signal_ChunkMiss; title="Number of chunk Miss";record=count,vector);

}


