

package so.mesh.coolstreaming;

import so.common.CommBase;

simple CoolstreamingBase extends CommBase
{
    parameters:
        @class(CoolstreamingBase);
        @display("i=block/app;bgb=250,410");
        
		int substreamCount = default(3); // Number of substreams 
       
        int maxNOP = default(4); // Max number of Partners
        int minNOP = default(3); // Min number of Partners
        
        double CheckPartnersIntervall @unit(s) = default(3.0s);
        double BufferMapIntervall     @unit(s) = default(1.0s);
        
        bool debugOutput = default(true);
}
