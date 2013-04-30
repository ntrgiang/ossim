
//
//  StatisticRouter.ned
//
//       Created on: 23.07.2009
//           Author: mkissmann
//
//


//package p2pstreaming.underlay;
package so.underlay.inet;

import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.linklayer.ppp.PPPInterface;
import inet.linklayer.ethernet.EthernetInterface;
import inet.transport.udp.UDP;
import inet.nodes.inet.NetworkLayer;

//import p2pstreaming.base.inet.InterfaceTableMobility;
//import p2pstreaming.base.inet.NetworkLayerMobility;
//import p2pstreaming.modules.Statistic.RoutingCostTable;
//import p2pstreaming.modules.Statistic.PacketCountTable;
//import p2pstreaming.modules.Statistic.HostStatistics;
//import p2pstreaming.modules.ALM_RouterSupport.ARS_Controller;

import so.underlay.InterfaceTableMobility;
//import so.underlay.NetworkLayerMobility;
//import so.underlay.Statistic.RoutingCostTable;
//import p2pstreaming.modules.Statistic.PacketCountTable;
//import p2pstreaming.modules.Statistic.HostStatistics;
//import so.underlay.ALM_RouterSupport.ARS_Controller;


//
// \IP router.
//
module StatisticRouter
{
    parameters:
        string routingFile;
		bool isTransit = default(false);
        @node;

    gates:
        input in[];
        output out[];
        input ethIn[];
        output ethOut[];

    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,60;i=block/control");
        }
        interfaceTable: InterfaceTableMobility {
            parameters:
                @display("p=170,60;i=block/table");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "auto";
                routingFile = routingFile;
                @display("p=280,60;i=block/table");
        }
		networkLayer: NetworkLayerMobility {
            parameters:
                @display("p=230,140;i=block/fork;q=queue");
            gates:
                ifIn[sizeof(out)+sizeof(ethOut)];
                ifOut[sizeof(out)+sizeof(ethOut)];
        }
        ppp[sizeof(out)]: PPPInterface {
            parameters:
                @display("p=90,307,row,110;q=queue;i=block/ifcard");
        }
        eth[sizeof(ethOut)]: EthernetInterface {
            parameters:
                @display("p=230,307,row,110;q=queue;i=block/ifcard");
        }
        routingCostTable: RoutingCostTable {
            parameters:
                @display("p=390,60;i=block/table");
        }
        packetCountTable: PacketCountTable {
            parameters:
                @display("p=500,60;i=block/table");
        }
        statistics[sizeof(out)]: HostStatistics {
            parameters:
                @display("p=90,207,row,110;i=block/network2");

            gates:
                ifIn[sizeof(out)];
                ifOut[sizeof(out)];
        }
//        arsController: ARS_Controller {
//            parameters:
//                @display("p=430,140;i=block/uparrow");
//        }
        udp: UDP {
        	parameters:
        		@display("i=block/transport");
        	gates:
        		appOut[2];
        		appIn[2];
        }
        
    connections allowunconnected:
        // connections to network outside
        for i=0..sizeof(out)-1 {
            in[i] --> ppp[i].phys$i;
            out[i] <-- ppp[i].phys$o;
            ppp[i].netwOut --> statistics[i].ifIn[i];
            ppp[i].netwIn <-- statistics[i].ifOut[i];
            statistics[i].to_network --> networkLayer.ifIn[i];
            statistics[i].from_network <-- networkLayer.ifOut[i];

        }

        for i=0..sizeof(ethOut)-1 {
            ethIn[i] --> eth[i].phys$i;
            ethOut[i] <-- eth[i].phys$o;
            eth[i].netwOut --> networkLayer.ifIn[sizeof(out)+i];
            eth[i].netwIn <-- networkLayer.ifOut[sizeof(out)+i];
        }
		
		udp.ipIn <-- networkLayer.udpOut;
		udp.ipOut --> networkLayer.udpIn;
//		udp.appOut[0] --> arsController.from_udp;
//		udp.appIn[0] <-- arsController.to_udp;
		
}
