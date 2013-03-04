

package so.global.statistic;

simple ScampStatistic like IStatisticBase
{
   parameters:

   // --- Statistics
   // @statistic[ChunkHit](source=Signal_ChunkHit; record=count);
   @statistic[pvSize](source=Signal_pvSize; record=histogram,vector);


}
