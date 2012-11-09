//package giang.simpleOverlay.tracker;
package simpleoverlay.applications.source;

//import inet.applications.udpapp.UDPBasicApp;
import simpleoverlay.applications.source.ScampSource;
import simpleoverlay.applications.common.Dispatcher;


module ScampSourceApp
{
    parameters:
        @display("bgb=107,175");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        gossip: ScampSource {
            @display("p=52,46");
        }

        dispatcher: Dispatcher {

            @display("p=52,123");
        }
    connections:
        // Connect Gossip module to the Dispatcher
        dispatcher.overlayIn++ <-- gossip.dpOut;
        dispatcher.overlayOut++ --> gossip.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
