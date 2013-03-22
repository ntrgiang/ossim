

package so.payload;

import so.common.CommBase;

simple Forwarder extends CommBase
{
    parameters:
        @class(Forwarder);
        
    //gates:
    //    output bufferOut @labels(VideoChunk/up);
    //    input bufferIn @labels(VideoChunk/down);

}
