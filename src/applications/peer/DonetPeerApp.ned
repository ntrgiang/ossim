package simpleoverlay.applications.peer;

import simpleoverlay.applications.DummyMembership;
import simpleoverlay.applications.peer.DonetPeer;
import simpleoverlay.applications.peer.Player;
import simpleoverlay.applications.common.Forwarder;
import simpleoverlay.applications.common.membership.MCache;
import simpleoverlay.applications.common.streaming.VideoBuffer;
import simpleoverlay.applications.common.streaming.PartnerList;
import simpleoverlay.applications.common.Dispatcher;
//import simpleoverlay.global.AppSettingDonet;

module DonetPeerApp
{
    parameters:
        @display("bgb=270,294");
        
//        @statistic[hitCount_global](source=chunkHit_Global; record=count);

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        membership: DummyMembership {
            @display("p=51,173;i=block/users");
        }
        mesh: DonetPeer {
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
        player: Player {
            @display("p=211,44;i=device/terminal");
        }
        partnerList: PartnerList {

            @display("p=132,104");
        }
        dispatcher: Dispatcher {
            @display("p=132,247;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=34,178;i=block/cogwheel");
    //        }
    connections:
        // Connect Gossip module to the Dispatcher
        dispatcher.overlayIn++ <-- membership.dpOut;
        dispatcher.overlayOut++ --> membership.dpIn;

        // Connect Mesh module to the Dispatcher
        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        // Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
