

package so.mesh.coolstreaming;

import so.common.CommBase;

simple CoolstreamingBase extends CommBase
{
    parameters:
        @class(CoolstreamingBase);
        @display("i=block/app;bgb=250,410");
        
		int substreamCount = default(3); // Count of substreams 
        int maxNOC = default(6); // Max number of children


        // -- Added by Giang to resolve error during initialization
        // Partnership
        int maxNOP = default(5);

        double CheckPartnersIntervall @unit(seconds) = default(1s);
}
