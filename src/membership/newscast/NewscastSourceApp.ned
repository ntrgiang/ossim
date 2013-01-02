//package giang.simpleOverlay.tracker;
package so.membership.newscast;

//import inet.applications.udpapp.UDPBasicApp;
import so.membership.newscast.NewscastSource;
import so.common.Dispatcher;


module NewscastSourceApp
{
    parameters:
        @display("bgb=107,175");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        gossip: NewscastSource {
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
