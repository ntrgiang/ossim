package so.simulations;

import ned.IdealChannel;
import ned.DatarateChannel;

// -------------------- Channel 0 ----------------------
// -- Channel for the Video source
channel Channel0_up extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 1Mbps;
}

channel Channel0_down extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 4Mbps;
}

// -------------------- Channel 1 ----------------------
channel Channel1_up extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 3Mbps;
}

channel Channel1_down extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 5Mbps;
}

// -------------------- Channel 2 ----------------------

channel Channel2_up extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 1Mbps;
}

channel Channel2_down extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 2Mbps;
}

// -------------------- Channel 3 ----------------------
channel Channel3_up extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 384kbps;
}

channel Channel3_down extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 1.5Mbps;
}

// -------------------- Channel 4 ----------------------
channel Channel4_up extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 128kbps;
}

channel Channel4_down extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 768kbps;
}


channel ethernetline extends DatarateChannel
{
    parameters:
        delay = 0.1us;
        //datarate = 1Mbps;
        datarate = 400kbps;
}

// ----------------- For inter-connecting Routers -----------
channel fiberline extends ned.DatarateChannel {
    delay = 0.01s;
    datarate = 1Gbps;
}


