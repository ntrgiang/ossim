

package so.common;

import inet.applications.udpapp.UDPApp;

simple Dispatcher like UDPApp
{
    parameters:
        @class(Dispatcher); 
        
        int localPort = default(9000); // port number to listen onComm
        int destPort  = default(9000);

    gates:
        // To upper layers
        input overlayIn[]   @label(DispatcherControlInfo/down);
        output overlayOut[] @label(DispatcherControlInfo/up);

        // To lower UDP layer
        input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        
//        input GossipIn @labels(GossipControlInfo/up);
//        output GossipOut @labels(GossipControlInfo/down);
        
}
