package so.mesh.donet;

import so.mesh.donet.DonetBase;

simple DonetSource extends DonetBase
{
    parameters:
        @class(DonetSource); 
        
        // Timer
        //double bufferMapInterval  @unit(s) = default(0.5s);
        
        // Partnership
        int maxNOP = default(5);

        // Statistics
        @statistic[nPartner](source=Signal_nPartner; record=last,vector);


}


