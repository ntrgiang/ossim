

package so.membership.scamp;

import simpleoverlay.applications.ScampBase;

simple ScampPeer extends ScampBase
{
    parameters:
        @class(ScampPeer);
        @display("i=block/app;bgb=250,410");
                
        // Debug
        bool moduleDebug = default(false);
        
        int numberOfPartner = default(4);
        
}
