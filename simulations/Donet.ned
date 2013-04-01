package so.simulations;

import ned.DatarateChannel;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.churn.StaticChurn;
//import so.global.statistic.GlobalStatistic;
//import so.global.statistic.IStatisticBase;
import so.global.statistic.IStreamingStatistic;

import so.global.statistic.Statistic;
import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.global.AppSettingDonet;

import so.mesh.donet.DonetPeerNode;
import so.mesh.donet.DonetSourceNode;

import inet.nodes.inet.Router;
import inet.nodes.ethernet.EtherSwitch;
import inet.networklayer.autorouting.FlatNetworkConfigurator;

network Donet_Homo_oneRouter_Network extends BasicNetwork
{
    parameters:
        int nNodePerRouter = default(1);
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");


    types:
        channel Channel_9M_up extends ned.DatarateChannel
        {
            datarate = 9Mbps;
        }
        channel Channel_9M_down extends ned.DatarateChannel
        {
            datarate = 40Mbps;
        }
        channel Channel_homo_up extends ned.DatarateChannel
        {
            datarate = 900kbps;
        }
        channel Channel_homo_down extends ned.DatarateChannel
        {
            datarate = 2Mbps;
        }
    submodules:
        sourceNode: DonetSourceNode {
            @display("p=388,50;i=device/server");
        }

        peerNode[nNodePerRouter]: DonetPeerNode;
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

        //globalStatistic: GlobalStatistic {
        //globalStatistic: <statisticModule> like IStatisticBase {
        globalStatistic: <statisticModule> like IStreamingStatistic {
            @display("p=77,248;i=device/disk");
        }
        //        globalStatisic: Statistic {
        //            @display("p=179,328");
        //        }
        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }
        meshObserver: MeshOverlayObserver {
            @display("p=184,29");
        }
        logger: Logger {
        }
    connections:
        // -- old set-up
//        sourceNode.pppg$o++ --> Channel0_up --> router.pppg$i++;
//        sourceNode.pppg$i++ <-- Channel0_down <-- router.pppg$o++;
//        for i=0..nNodePerRouter-1 {
//            peerNode[i].pppg++ <--> ethernetline <--> router.pppg++;
//        }

        // -- new set-up
        sourceNode.pppg$o++ --> Channel_9M_up --> router.pppg$i++;
        sourceNode.pppg$i++ <-- Channel_9M_down <-- router.pppg$o++;
        for i=0..nNodePerRouter-1 {
            peerNode[i].pppg$o++ --> Channel_homo_up --> router.pppg$i++;
            peerNode[i].pppg$i++ <-- Channel_homo_down <-- router.pppg$o++;
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
