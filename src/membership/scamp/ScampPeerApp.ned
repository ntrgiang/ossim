

package so.membership.scamp;

import so.membership.scamp.ScampPeer;
import so.common.Dispatcher;

module ScampPeerApp
{
    parameters:
        @display("i=block/app;bgb=142,186");

        int videoStreamBitRate @unit(bps) = default(300kbps);
        int chunkSize @unit(byte) = default(1250B);
        int bufferMapSize @unit(s) = default(120s);

    gates:
        input udpIn;
        output udpOut;

    submodules:
        gossip: ScampPeer {
            @display("p=69,44");
        }
        dispatcher: Dispatcher {

            @display("p=69,123");
        }
    connections:
        // Connect Gossip module to the Dispatcher
        dispatcher.overlayIn++ <-- gossip.dpOut;
        dispatcher.overlayOut++ --> gossip.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}

