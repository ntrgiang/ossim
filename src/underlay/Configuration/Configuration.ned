//package p2pstreaming.modules.Configuration;
package so.underlay.Configuration;

simple Configuration
{
    parameters:
        volatile double randomSeed;
        volatile string coordType = default("euclid");
        volatile double coordDimensions;
        volatile double coordMin;
        volatile double coordMax;
        volatile double streamStripes;
        volatile double streamPacketFrequency;
        volatile double streamPacketSize;
        volatile double clients;
        volatile double run;
        volatile double unstable;
        volatile double hosts;
        double rts;
        string networkAddress;
        int coordinator = default(0);
}
