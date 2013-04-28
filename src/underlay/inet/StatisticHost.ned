//package p2pstreaming.underlay;
package so.underlay.inet;

import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.applications.tcpapp.TCPApp;
import inet.transport.tcp.TCP;
import inet.transport.udp.UDP;
import inet.applications.udpapp.UDPApp;
import inet.applications.pingapp.PingApp;
import inet.linklayer.ppp.PPPInterface;
import inet.linklayer.ethernet.EthernetInterface;

import p2pstreaming.base.inet.RoutingTableMobility;
import p2pstreaming.base.inet.InterfaceTableMobility;
import p2pstreaming.modules.Statistic.RoutingCostTable;
import p2pstreaming.base.inet.NetworkLayerMobility;
import p2pstreaming.modules.Application;
import p2pstreaming.modules.Statistic.HostStatistics;
import p2pstreaming.modules.Statistic.PacketCountTable;

//
// \IP host with TCP, UDP layers and applications.
//
module StatisticHost
{
    parameters:
        double numUdpApps;
        string udpAppType;
        bool IPForward;
        string routingFile;
        @node;

    gates:

        input in[];
        output out[];
        input ethIn[];
        output ethOut[];

    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,70;i=block/control");
        }
        routingTable: RoutingTableMobility {
            parameters:
                IPForward = IPForward;
                routerId = "";
                routingFile = routingFile;
                @display("p=60,230;i=block/table");
        }

        interfaceTable: InterfaceTableMobility {
            parameters:
                @display("p=60,150;i=block/table");
        }

        routingCostTable: RoutingCostTable {
            parameters:
                @display("p=60,310;i=block/table");
        }
        packetCountTable: PacketCountTable {
            parameters:
                @display("p=60,390;i=block/table");
        }
        udpApp[numUdpApps]: <udpAppType> like UDPApp {
            parameters:
                @display("i=block/app;p=311,59");
        }
        udp: UDP {
            parameters:
                @display("p=311,150;i=block/transport");
        }
        networkLayer: NetworkLayerMobility {
            parameters:
                proxyARP = false;
                @display("p=311,247;i=block/fork;q=queue");
            gates:
                ifIn[sizeof(out)+sizeof(ethOut)];
                ifOut[sizeof(out)+sizeof(ethOut)];
        }
        ppp[sizeof(out)]: PPPInterface {
            parameters:
                @display("p=189,350,row,90;q=txQueue;i=block/ifcard");
        }
        eth[sizeof(ethOut)]: EthernetInterface {
            parameters:
                @display("p=311,350,row,90;q=txQueue;i=block/ifcard");
        }
        statistics[sizeof(out)]: HostStatistics {
            parameters:
                @display("i=block/network2;p=189,247");
            gates:
                ifIn[sizeof(out)];
                ifOut[sizeof(out)];
        }
    connections allowunconnected:

        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;

        // connections to network outside
        for i=0..sizeof(out)-1 {
            in[i] --> ppp[i].phys$i;
            ppp[i].netwOut --> statistics[i].ifIn[i];
            statistics[i].to_network --> networkLayer.ifIn[i];

            out[i] <-- ppp[i].phys$o;
            ppp[i].netwIn <-- statistics[i].ifOut[i];
            statistics[i].from_network <-- networkLayer.ifOut[i];
        }

        for i=0..sizeof(ethOut)-1 {
            ethIn[i] --> eth[i].phys$i;
            ethOut[i] <-- eth[i].phys$o;
            eth[i].netwOut --> networkLayer.ifIn[sizeof(out)+i];
            eth[i].netwIn <-- networkLayer.ifOut[sizeof(out)+i];
        }
}

