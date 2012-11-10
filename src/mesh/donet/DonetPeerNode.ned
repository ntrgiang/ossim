


package so.mesh.donet;

import so.mesh.donet.DonetPeerApp;

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ext.ExtInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.transport.udp.UDP;
import inet.util.NAMTraceWriter;
import inet.util.TCPDump;
import inet.nodes.inet.NetworkLayer;

module DonetPeerNode
{
    parameters:
        @node();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgb=425,456");
        bool IPForward = default(false);
        int namid = default(-1);
        string routingFile = default("");
        int numExtInterfaces = default(0);
    gates:
        inout pppg[] @labels(PPPFrame-conn);
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        //        namTrace: NAMTraceWriter {
        //            parameters:
        //                namid = namid;
        //                @display("p=57,294");
        //        }
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=57,54");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=57,134");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = IPForward;
                routerId = "";
                routingFile = routingFile;
                @display("p=57,214");
        }
        peerApp: DonetPeerApp {
            parameters:
                @display("p=163,67;i=block/app");
        }
        udp: UDP {
            parameters:
                @display("p=336,141");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=336,230;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=186,390,row,90;q=txQueue");
        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=336,390,row,90;q=txQueue");
        }
        tcpdump: TCPDump {
            parameters:
                @display("p=336,302;i=abstract/cache_s");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                in2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                out2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }
    connections allowunconnected:

        peerApp.udpOut --> udp.appIn++;
        peerApp.udpIn <-- udp.appOut++;

        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;

        // connections to network outside
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> ppp[i].phys;
            ppp[i].netwOut --> tcpdump.ifIn[i];
            tcpdump.out2[i] --> networkLayer.ifIn[i];
            ppp[i].netwIn <-- tcpdump.ifOut[i];
            tcpdump.in2[i] <-- networkLayer.ifOut[i];
        }

        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> eth[i].phys;
            eth[i].netwOut --> tcpdump.ifIn[sizeof(pppg)+i];
            tcpdump.out2[sizeof(pppg)+i] --> networkLayer.ifIn[sizeof(pppg)+i];
            eth[i].netwIn <-- tcpdump.ifOut[sizeof(pppg)+i];
            tcpdump.in2[sizeof(pppg)+i] <-- networkLayer.ifOut[sizeof(pppg)+i];
        }

//        for i=0..numExtInterfaces-1 {
//            //            ext[i].netwOut --> tcpdump.ifIn[sizeof(pppg)+sizeof(ethg)+i];
//            tcpdump.out2[sizeof(pppg)+sizeof(ethg)+i] --> networkLayer.ifIn[sizeof(pppg)+sizeof(ethg)+i];
//            //            ext[i].netwIn <-- tcpdump.ifOut[sizeof(pppg)+sizeof(ethg)+i];
//            tcpdump.in2[sizeof(pppg)+sizeof(ethg)+i] <-- networkLayer.ifOut[sizeof(pppg)+sizeof(ethg)+i];
//        }
}
