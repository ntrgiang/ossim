package so.payload;

simple ChunkGenerator
{
    parameters:
        @class(ChunkGenerator); 
        
        // Timer
		double videoStartTime     @unit(s) = default(5.0s);
		
		// -- Possible signals to collect
		@signal[chunkSeqNum](type=long);
		// Statistics
		//@statistic[dropCount](source="count(sig_chunkSeqNum)"; record=last,vector?);
		//@statistic[chunkSeqNum](source=chunkSeqNumSignal; record=count);
        //@statistic[chunkSeqNum](record=min, max, vector);

}


