

package so.mesh.coolstreaming;

import so.common.CommBase;

simple CoolstreamingBase extends CommBase
{
    parameters:
        @class(CoolstreamingBase);
        @display("i=block/app;bgb=250,410");
        
		int substreamCount = default(3); // Count of substreams 
        int maxNOC = default(6); // Max number of children
        
        int maxNOP = default(4); // Max number of Partners
        int minNOP = default(4); // Min number of Partners
        
        double CheckPartnersIntervall @unit(s) = default(5.0s);
        double BufferMapIntervall     @unit(s) = default(2.0s);
}
