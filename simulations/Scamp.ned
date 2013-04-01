package so.simulations;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.churn.StaticChurn;
import so.global.statistic.IDiscoveryStatistic;
import so.global.statistic.ScampStatistic;

import so.membership.scamp.ScampPeerNode;
import so.membership.scamp.ScampSourceNode;

import inet.nodes.inet.Router;
import inet.nodes.ethernet.EtherSwitch;
import inet.networklayer.autorouting.FlatNetworkConfigurator;

network Scamp_Homo_oneRouter_Network extends BasicNetwork
{
    parameters:
        int nNodePerRouter = default(1);
        @display("bgb=472,295");
        string churnModel = default("StaticChurn");
        string statisticModule = default("ScampStatistic");
//        int videoStreamingBitRate @unit(bps) = default(512kbps);
//        int chunkSize @unit(byte) = default(8KiB);

    submodules:
        scampSourceNode: ScampSourceNode {
            @display("p=395,51");
        }

        scampPeerNode[nNodePerRouter]: ScampPeerNode;

        router: Router {
            @display("p=306,150");
        }

        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=77,29");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }

        globalStatistic: <statisticModule> like IDiscoveryStatistic {
            @display("p=77,248");
        }

    connections:
        // -- Connect Source
        scampSourceNode.pppg$o++ --> Channel0_up --> router.pppg$i++;
        scampSourceNode.pppg$i++ <-- Channel0_down <-- router.pppg$o++;

        // -- Connect Peers
        for i=0..nNodePerRouter-1 {
            scampPeerNode[i].pppg++ <--> ethernetline <--> router.pppg++;
        }
}

network Scamp_Homo_RouterLine_Network extends BasicNetwork
{
    parameters:
        @display("bgb=472,295");
        string churnModel = default("StaticChurn");

        int nRouter;
        int nNodePerRouter;

    submodules:
        scampSourceNode: ScampSourceNode {
            @display("p=395,51");
        }

        scampPeerNode[nNodePerRouter*nRouter]: ScampPeerNode {
            //            @display("p=149,235");
            @display("i=device/pc");
        }

        router[nRouter+1]: Router {
            @display("p=306,150");
        }

        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=77,97");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,163");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,240");
        }

    connections:
        // -- Inter-connect Routers
        for i=0..(nRouter-2) {
            router[i].pppg++ <--> fiberline <--> router[i+1].pppg++;
        }

        // -- Connect Source node to the router line
        scampSourceNode.pppg++ <--> ethernetline <--> router[nRouter].pppg++;
        router[nRouter].pppg++ <--> fiberline <--> router[nRouter/2].pppg++;

        // -- Connect Peers to the router line
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1) {
            scampPeerNode[i*nNodePerRouter + j].pppg++ <--> ethernetline <--> router[i].pppg++;
        }

}

network Scamp_Heter_RouterLine_Network extends BasicNetwork
{
    parameters:
        @display("bgb=472,295");
        string churnModel = default("StaticChurn");

        int nRouter;
        int nNodePerRouter; // Should be in orders of 100

        int percentGroup1;
        int percentGroup2;
        int percentGroup3;
        int percentGroup4;

    submodules:
        scampSourceNode: ScampSourceNode {
            @display("p=395,51");
        }

        scampPeerNode[nNodePerRouter*nRouter]: ScampPeerNode {
            //            @display("p=149,235");
            @display("i=device/pc");
        }

        router[nRouter+1]: Router {
            @display("p=306,150");
        }

        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=77,97");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,163");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,240");
        }

    connections:
        // -- Inter-connect Routers
        for i=0..(nRouter-2) {
            router[i].pppg++ <--> fiberline <--> router[i+1].pppg++;
        }

        // -- Connect Source node to the router line
        scampSourceNode.pppg$o++ --> Channel0_up --> router[nRouter].pppg$i++;
        scampSourceNode.pppg$i++ <-- Channel0_down <-- router[nRouter].pppg$o++;

        router[nRouter].pppg++ <--> fiberline <--> router[nRouter/2].pppg++;

        // -- Connect Peers to the router line
        // -- -- Group 1
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j < nNodePerRouter * percentGroup1 {
            scampPeerNode[i*nNodePerRouter + j].pppg$o++ --> Channel1_up --> router[i].pppg$i++;
            scampPeerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel1_down <-- router[i].pppg$o++;
        }

        // -- -- Group 2
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup1, if j < nNodePerRouter * percentGroup2 {
            scampPeerNode[i*nNodePerRouter + j].pppg$o++ --> Channel2_up --> router[i].pppg$i++;
            scampPeerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel2_down <-- router[i].pppg$o++;
        }

        // -- -- Group 3
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup2, if j < nNodePerRouter * percentGroup3 {
            scampPeerNode[i*nNodePerRouter + j].pppg$o++ --> Channel3_up --> router[i].pppg$i++;
            scampPeerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel3_down <-- router[i].pppg$o++;
        }

        // -- -- Group 4
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup3, if j < nNodePerRouter * percentGroup4 {
            scampPeerNode[i*nNodePerRouter + j].pppg$o++ --> Channel4_up --> router[i].pppg$i++;
            scampPeerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel4_down <-- router[i].pppg$o++;
        }
}


// ***************************************
network ScampNetwork extends BasicNetwork
{
    parameters:
        //        int numPeers = default(1);  // already exists in BasicNetwork.ned
        @display("bgb=472,295");
        string churnModel = default("StaticChurn");
//        int videoStreamingBitRate @unit(bps) = default(512kbps);
//        int chunkSize @unit(byte) = default(8KiB);

    submodules:
//        bootstrap: BootstrapNode {
//            @display("p=228,29");
//        }
        //        tracker: TrackerNode {
        //            @display("p=381,29");
        //        }

        scampSourceNode: ScampSourceNode {
            @display("p=395,51");
        }

        scampPeerNode[numPeers]: ScampPeerNode;

        router: Router;

        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=77,97");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,163");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,240");
        }
    connections:
        for i=0..numPeers-1 {
            scampPeerNode[i].pppg++ <--> ethernetline <--> router.pppg++;
        }

//            peerNode[i].pppg$i++ <-- Channel1_down <-- router.pppg$o++;
//        }
//
//        // Connecting Group2:
//        for i=group1..group1+group2-1 {
//            peerNode[i].pppg$o++ --> Channel2_up --> router.pppg$i++;
//            peerNode[i].pppg$i++ <-- Channel2_down <-- router.pppg$o++;
//        }
//
//        // Connecting Group3:
//        for i=group1+group2..group1+group2+group3-1 {
//            peerNode[i].pppg$o++ --> Channel3_up --> router.pppg$i++;
//            peerNode[i].pppg$i++ <-- Channel3_down <-- router.pppg$o++;
//        }
//
//        // Connecting Group4:
//        for i=group1+group2+group3..group1+group2+group3+group4-1 {
//            peerNode[i].pppg$o++ --> Channel4_up --> router.pppg$i++;
//            peerNode[i].pppg$i++ <-- Channel4_down <-- router.pppg$o++;
//        }

        scampSourceNode.pppg$o++ --> Channel0_up --> router.pppg$i++;
        scampSourceNode.pppg$i++ <-- Channel0_down <-- router.pppg$o++;

//        bootstrap.pppg++ <--> ethernetline <--> router.pppg++;

//        tracker.pppg++ <--> ethernetline <--> router.pppg++;

//        tracker.ethg++ <--> ethernetline <--> router.ethg++;

}
