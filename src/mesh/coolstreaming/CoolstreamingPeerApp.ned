package so.mesh.coolstreaming;

import so.common.Dispatcher;
import so.membership.dummy.DummyMembership;
import so.membership.MCache;

import so.mesh.coolstreaming.CoolstreamingPeer;
import so.mesh.PartnerList;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.payload.PlayerSimpleSkip;


//import so.membership.oracle.OracleMembership;
//import so.payload.Player;
//import simpleoverlay.global.AppSettingDonet;
//import so.membership.newscast.NewscastPeer;

module CoolstreamingPeerApp
{
    parameters:
        @display("bgb=270,294");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        mesh: CoolstreamingPeer {
            @display("p=132,173;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=211,173;i=block/rxtx");
        }
        mCache: MCache {
            @display("p=51,104;i=abstract/cache");
        }
        videoBuffer: VideoBuffer {
            @display("p=211,104;i=block/buffer2");
        }
        //player: Player {
        player: PlayerSimpleSkip {
            @display("p=211,44;i=device/terminal");
        }
        partnerList: PartnerList {

            @display("p=132,104");
        }
        
        //gossipProtocol : NewscastPeer;
        gossipProtocol: DummyMembership;
        
        dispatcher: Dispatcher {
            @display("p=132,247;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=34,178;i=block/cogwheel");
    //        }
    connections:
        // - Connect Forwarder module to the NewscastPeer
        dispatcher.overlayIn++ <-- gossipProtocol.dpOut;
        dispatcher.overlayOut++ --> gossipProtocol.dpIn;
        
        // - Connect Mesh module to the Dispatcher
        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        // - Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // - Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;
}
