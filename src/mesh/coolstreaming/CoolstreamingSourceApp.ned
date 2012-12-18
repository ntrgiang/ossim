package so.mesh.coolstreaming;

import so.common.Dispatcher;
import so.membership.MCache;
import so.membership.dummy.DummyMembership;
import so.mesh.coolstreaming.CoolstreamingSource;
import so.mesh.PartnerList;
import so.payload.ChunkGenerator;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.membership.newscast.NewscastPeer;

module CoolstreamingSourceApp
{
    parameters:
        @display("bgb=287,308");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        membership: DummyMembership {
            @display("p=52,179;i=block/users");
        }
        mesh: CoolstreamingSource {
            @display("p=133,179;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=212,179;i=block/rxtx");
        }
        mCache: MCache {
            @display("p=52,110;i=abstract/cache");
        }
        videoBuffer: VideoBuffer {
            @display("p=212,110;i=block/buffer2");
        }
        chunkGenerator: ChunkGenerator {
            @display("p=212,37;i=block/source");
        }
        partnerList: PartnerList {

            @display("p=133,110");
        }
        
        gossipProtocol : NewscastPeer;
        
        dispatcher: Dispatcher {
            @display("p=133,253;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=41,191");
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

        // Connect Forwarder module to the NewscastPeer
        dispatcher.overlayIn++ <-- gossipProtocol.dpOut;
        dispatcher.overlayOut++ --> gossipProtocol.dpIn;
        
        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
