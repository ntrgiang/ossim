package so.simulations;

import ned.DatarateChannel;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.churn.StaticChurn;
//import so.global.statistic.GlobalStatistic;
import so.global.statistic.IStatisticBase;


import so.global.statistic.Statistic;
import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.global.AppSettingDonet;

import so.mesh.donet.DonetPeerNode;
import so.mesh.donet.DonetSourceNode;

import inet.nodes.inet.Router;
import inet.nodes.ethernet.EtherSwitch;
import inet.networklayer.autorouting.FlatNetworkConfigurator;


import so.mesh.coolstreaming.CoolstreamingSourceNode;
import so.mesh.coolstreaming.CoolstreamingPeerNode;

network Coolstreaming_Homo_oneRouter_Network extends BasicNetwork
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
            datarate = 500kbps;
        }
        channel Channel_homo_down extends ned.DatarateChannel
        {
            datarate = 2Mbps;
        }
    submodules:
        sourceNode: CoolstreamingSourceNode {
            @display("p=388,50;i=device/server");
        }

        peerNode[nNodePerRouter]: CoolstreamingPeerNode;
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
        globalStatistic: <statisticModule> like IStatisticBase {
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
