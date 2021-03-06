package so.simulations;

import ned.DatarateChannel;

import inet.nodes.inet.Router;
import inet.nodes.ethernet.EtherSwitch;
import inet.networklayer.autorouting.FlatNetworkConfigurator;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.churn.StaticChurn;
import so.global.statistic.IStreamingStatistic;
import so.global.AppSettingDonet;
import so.global.statistic.Statistic;
import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.mesh.donet.DonetPeerNode;
import so.mesh.donet.DonetSourceNode;

network Donet_Homo_oneRouter_Network extends BasicNetwork
{
    parameters:
        int nPeers = default(1);
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");

    types:
        //channel Channel_source_up extends ned.DatarateChannel       { datarate = 10Mbps; }
        channel Channel_source_up extends ned.DatarateChannel       { datarate = 1Mbps; }
        channel Channel_source_down extends ned.DatarateChannel     { datarate = 40Mbps; }

        //channel Channel_peer_up extends ned.DatarateChannel     { datarate = 1000kbps; }
        channel Channel_peer_up extends ned.DatarateChannel     { datarate = 500kbps; }
        channel Channel_peer_down extends ned.DatarateChannel   { datarate = 2Mbps; }

    submodules:
        sourceNode: DonetSourceNode {
            @display("p=388,50;i=device/server");
        }

        peerNode[nPeers]: DonetPeerNode;

        router: Router {
            @display("p=329,136");
        }

        flatNetConfig: FlatNetworkConfigurator {
            @display("p=77,29;i=block/cogwheel");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }

        globalStatistic: <statisticModule> like IStreamingStatistic {
            @display("p=77,248;i=device/disk");
        }

        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }

    connections:
        sourceNode.pppg$o++ --> Channel_source_up --> router.pppg$i++;
        sourceNode.pppg$i++ <-- Channel_source_down <-- router.pppg$o++;

        for i=0..nPeers-1 {
            peerNode[i].pppg$o++ --> Channel_peer_up --> router.pppg$i++;
            peerNode[i].pppg$i++ <-- Channel_peer_down <-- router.pppg$o++;
        }
}

network Donet_Heter_oneRouter_Network extends BasicNetwork
{
    parameters:
        int nPeers = default(1);
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");

        int percent1 = default(1);
        int percent2 = default(0);
        int percent3 = default(0);

    types:
        channel Channel_source_up extends ned.DatarateChannel       { datarate = 10Mbps; }
        channel Channel_source_down extends ned.DatarateChannel     { datarate = 40Mbps; }

        channel Channel_peer1_up extends ned.DatarateChannel     { datarate = 384kbps; }
        channel Channel_peer1_down extends ned.DatarateChannel   { datarate = 1Mbps; }

        channel Channel_peer2_up extends ned.DatarateChannel     { datarate = 1Mbps; }
        channel Channel_peer2_down extends ned.DatarateChannel   { datarate = 6Mbps; }

        channel Channel_peer3_up extends ned.DatarateChannel     { datarate = 10Mbps; }
        channel Channel_peer3_down extends ned.DatarateChannel   { datarate = 10Mbps; }

    submodules:
        sourceNode: DonetSourceNode {
            @display("p=388,50;i=device/server");
        }

        peerNode[nPeers]: DonetPeerNode;

        router: Router {
            @display("p=329,136");
        }

        flatNetConfig: FlatNetworkConfigurator {
            @display("p=77,29;i=block/cogwheel");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }

        globalStatistic: <statisticModule> like IStreamingStatistic {
            @display("p=77,248;i=device/disk");
        }

        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }

    connections:
        // -- Source
        sourceNode.pppg$o++ --> Channel_source_up --> router.pppg$i++;
        sourceNode.pppg$i++ <-- Channel_source_down <-- router.pppg$o++;

//        for i=0..nPeers-1 {
//            peerNode[i].pppg$o++ --> Channel_peer_up --> router.pppg$i++;
//            peerNode[i].pppg$i++ <-- Channel_peer_down <-- router.pppg$o++;
//        }

        // -- Group 1
        //
        for j=0..(nPeers-1), if j < nPeers * percent1 {
            peerNode[j].pppg$o++ --> Channel_peer1_up --> router.pppg$i++;
            peerNode[j].pppg$i++ <-- Channel_peer1_down <-- router.pppg$o++;
        }

        // -- Group 2
        //
        for j=0..(nPeers-1), if j >= nPeers * percent1, if j < nPeers * (percent1 + percent2) {
            peerNode[j].pppg$o++ --> Channel_peer2_up --> router.pppg$i++;
            peerNode[j].pppg$i++ <-- Channel_peer2_down <-- router.pppg$o++;
        }

        // -- Group 3
        //
        for j=0..(nPeers-1), if j >= nPeers * (percent1 + percent2), if j < nPeers {
            peerNode[j].pppg$o++ --> Channel_peer3_up --> router.pppg$i++;
            peerNode[j].pppg$i++ <-- Channel_peer3_down <-- router.pppg$o++;
        }
}


network Donet_Heter_RouterLine_Network extends BasicNetwork
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
        sourceNode: DonetSourceNode {
            @display("p=395,51");
        }

        peerNode[nNodePerRouter*nRouter]: DonetPeerNode {
            //            @display("p=149,235");
            @display("i=device/pc");
        }

        router[nRouter+1]: Router {
            @display("p=329,136");
        }

        flatNetConfig: FlatNetworkConfigurator {
            @display("p=77,29;i=block/cogwheel");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }

        globalStatisic: GlobalStatistic {
            @display("p=77,248;i=device/disk");
        }
        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }
    connections:
        // -- Inter-connect Routers
        for i=0..(nRouter-2) {
            router[i].pppg++ <--> fiberline <--> router[i+1].pppg++;
        }

        // -- Connect Source node to the router line
        sourceNode.pppg$o++ --> Channel0_up --> router[nRouter].pppg$i++;
        sourceNode.pppg$i++ <-- Channel0_down <-- router[nRouter].pppg$o++;

        router[nRouter].pppg++ <--> fiberline <--> router[nRouter/2].pppg++;

        // -- Connect Peers to the router line
        // -- -- Group 1
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j < nNodePerRouter * percentGroup1 {
            peerNode[i*nNodePerRouter + j].pppg$o++ --> Channel1_up --> router[i].pppg$i++;
            peerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel1_down <-- router[i].pppg$o++;
        }

        // -- -- Group 2
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup1, if j < nNodePerRouter * percentGroup2 {
            peerNode[i*nNodePerRouter + j].pppg$o++ --> Channel2_up --> router[i].pppg$i++;
            peerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel2_down <-- router[i].pppg$o++;
        }

        // -- -- Group 3
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup2, if j < nNodePerRouter * percentGroup3 {
            peerNode[i*nNodePerRouter + j].pppg$o++ --> Channel3_up --> router[i].pppg$i++;
            peerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel3_down <-- router[i].pppg$o++;
        }

        // -- -- Group 4
        for i=0..(nRouter-1), for j=0..(nNodePerRouter-1), if j >= nNodePerRouter * percentGroup3, if j < nNodePerRouter * percentGroup4 {
            peerNode[i*nNodePerRouter + j].pppg$o++ --> Channel4_up --> router[i].pppg$i++;
            peerNode[i*nNodePerRouter + j].pppg$i++ <-- Channel4_down <-- router[i].pppg$o++;
        }
}
