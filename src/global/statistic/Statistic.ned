

package so.global.statistic;

simple Statistic
{
    parameters:
        @signal[chunkHit_Global](type=long);
        @statistic[chunkHit_count_global](source=chunkHit_Global; record=count);

}
