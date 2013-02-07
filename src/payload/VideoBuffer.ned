

package so.payload;

simple VideoBuffer
{
    parameters:
        @class(VideoBuffer);
        @display("i=block/table");
        
        @signal[Signal_ReceivedChunk](type=long);
        
        //@statistic[receivedChunk](title="Sequence Number of received chunks"; source=Signal_ReceivedChunk; record=count,vector);

         @signal[Signal_LateChunk](type=long);
         //@statistic[lateChunk](title="Sequence Number of late chunks"; source=Signal_LateChunk; record=count,vector);

         @signal[Signal_NonsenseChunk](type=long);
         //@statistic[nonsenseChunk](title="Sequence Number of non-sense chunks"; source=Signal_NonsenseChunk; record=count,vector);
        
         @signal[Signal_InrangeChunk](type=long);
         //@statistic[inrangeChunk](title="Sequence Number of in-range chunks"; source=Signal_InrangeChunk; record=count,vector);

         //@statistic[duplicatedChunk](title="Sequence Number of duplicated chunks"; source=Signal_DuplicatedChunk; record=count,vector);
}
