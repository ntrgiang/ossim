

package so.mesh.donet;

import so.common.CommBase;

simple DonetBase extends CommBase
{
    parameters:
        @class(DonetBase);
        @display("i=block/app;bgb=250,410");

        double threshold_idleDuration_buffermap @unit(s) = default(0.5s);
}
