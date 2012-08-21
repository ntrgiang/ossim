package simpleoverlay.applications.source;

import simpleoverlay.applications.DonetBase;

simple DonetSource extends DonetBase
{
    parameters:
        @class(DonetSource); 
        
        // Timer
        double bufferMapInterval  @unit(s) = default(0.5s);
        int numberOfPartner = default(5);
		// int BufferMapSize = default(60);

}


