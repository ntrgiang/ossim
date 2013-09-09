package so.tree.multitree;

import so.common.Dispatcher;
import so.payload.StripeGenerator;
import so.payload.VideoBuffer;
import so.payload.Forwarder;
import so.membership.dummy.DummyMembership;

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


module MultitreeSourceNode
{
    parameters:
        @node();
        @labels(node,ethernet-node);
        int numExtInterfaces = default(0);
        bool IPForward = default(false);
        string routingFile = default("");

    gates:
        inout pppg[] @labels(PPPFrame-conn);
        inout ethg[] @labels(EtherFrame-conn);

    submodules:
        tree: MultitreeSource {
            @display("p=325,185;i=block/network2");
        }
        mesh: DummyMembership { // TODO: This should be a dummy module
            @display("p=241,185;i=block/network2");
        }

        dispatcher: Dispatcher {
            @display("p=280,277;i=block/dispatch");
        }
        udp: UDP {
            parameters:
                @display("p=404,277");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=63,105");
        }
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=63,25");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = IPForward;
                routerId = "";
                routingFile = routingFile;
                @display("p=63,185");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=404,366;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=280,525,row,90;q=txQueue");
        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=404,525,row,90;q=txQueue");
        }
        tcpdump: TCPDump {
            parameters:
                @display("p=404,438;i=abstract/cache_s");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                in2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                out2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }

        chunkGenerator: StripeGenerator {
            @display("p=404,43;i=block/source");
        }
		videoBuffer: VideoBuffer {
			@display("p=404,116;i=block/buffer2");
		}
        forwarder: Forwarder {
            @display("p=404,185;i=block/rxtx");
        }
        membership: DummyMembership {
            @display("p=162,185;i=block/users");
        }
		partnerList: MultitreePartnerList {
			@display("p=404,165;i=block/rxtx");
		}

    connections:
        dispatcher.overlayIn++ <-- membership.dpOut;
        dispatcher.overlayOut++ --> membership.dpIn;

        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        dispatcher.overlayIn++ <-- tree.dpOut;
        dispatcher.overlayOut++ --> tree.dpIn;

        dispatcher.udpOut --> udp.appIn++;
        dispatcher.udpIn <-- udp.appOut++;

        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;

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

		// Because all gates should be connected...?!
        udp.ipv6Out --> udp.ipv6In;
        networkLayer.tcpOut --> networkLayer.tcpIn;
        networkLayer.sctpOut --> networkLayer.sctpIn;
        networkLayer.rsvpOut --> networkLayer.rsvpIn;
        networkLayer.ospfOut --> networkLayer.ospfIn;
        networkLayer.pingOut --> networkLayer.pingIn;
}
