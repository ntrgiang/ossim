package so.mesh.donet;

import so.common.Dispatcher;
import so.membership.dummy.DummyMembership;
import so.membership.oracle.OracleMembership;
import so.membership.MCache;

import so.mesh.donet.DonetPeer;
import so.mesh.PartnerList;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.payload.Player;
import so.payload.PlayerSimpleSkip;
//import simpleoverlay.global.AppSettingDonet;

module GenericPeerApp
{
    parameters:
        @display("bgb=270,294");
//        @statistic[hitCount_global](source=chunkHit_Global; record=count);

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        memMan: DummyMembership {
            //membership: OracleMembership {
            @display("p=51,173;i=block/users");
        }
        partnerMan: DonetPeer {
            @display("p=132,173;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=211,173;i=block/rxtx");
        }
        videoBuffer: VideoBuffer {
            @display("p=211,104;i=block/buffer2");
        }
        //player: Player {
        player: PlayerSimpleSkip {
            //player: <playerModule> like PlayerBase {
            @display("p=211,44;i=device/terminal");
        }
        dispatcher: Dispatcher {
            @display("p=132,247;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=34,178;i=block/cogwheel");
    //        }
    connections:
        // Connect Gossip module to the Dispatcher
        dispatcher.overlayIn++ <-- memMan.dpOut;
        dispatcher.overlayOut++ --> memMan.dpIn;

        // Connect Mesh module to the Dispatcher
        dispatcher.overlayIn++ <-- partnerMan.dpOut;
        dispatcher.overlayOut++ --> partnerMan.dpIn;

        // Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;
        
        // Connect Forwarder to Video Buffer
        //forwarder.bufferOut --> videoBuffer.forwarderIn;
        //forwarder.bufferIn <-- videoBuffer.forwarderOut;

        // Connect Video Buffer to Player
        //videoBuffer.playerOut --> player.bufferIn;
}
