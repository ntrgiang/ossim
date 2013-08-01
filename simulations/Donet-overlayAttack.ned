package so.simulations;

import ned.DatarateChannel;
import inet.nodes.inet.Router;
import inet.networklayer.autorouting.FlatNetworkConfigurator;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.statistic.IStreamingStatistic;
import so.global.AppSettingDonet;
import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.mesh.donet.DonetPeerNodeTopo;
import so.mesh.donet.DonetSourceNodeTopo;

import so.global.topology.OverlayTopology;
import so.global.attacker.AttackerModule;

network Donet_oneRouter_overlayAttack
{
    parameters:
        int nNodePerRouter = default(1);
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");

    types:
        channel Channel_source_up extends ned.DatarateChannel
        {
            datarate = 9Mbps;
        }
        channel Channel_source_down extends ned.DatarateChannel
        {
            datarate = 40Mbps;
        }
        channel Channel_peer_up extends ned.DatarateChannel
        {
            datarate = 1100kbps;
        }
        channel Channel_peer_down extends ned.DatarateChannel
        {
            datarate = 2Mbps;
        }

    submodules:
        sourceNode: DonetSourceNodeTopo {
            @display("p=388,50;i=device/server");
        }

        peerNode[nNodePerRouter]: DonetPeerNodeTopo;

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

        meshObserver: MeshOverlayObserver {
            @display("p=184,29");
        }

        logger: Logger {
        }

        topoObserver: OverlayTopology {
        }

        attacker: AttackerModule {
        }

    connections:
        sourceNode.pppg$o++ --> Channel_source_up --> router.pppg$i++;
        sourceNode.pppg$i++ <-- Channel_source_down <-- router.pppg$o++;
        for i=0..nNodePerRouter-1 {
            peerNode[i].pppg$o++ --> Channel_peer_up --> router.pppg$i++;
            peerNode[i].pppg$i++ <-- Channel_peer_down <-- router.pppg$o++;
        }
}
