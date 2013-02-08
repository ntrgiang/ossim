package so.mesh.coolstreaming;

import so.mesh.coolstreaming.CoolstreamingBase;

simple CoolstreamingPeer extends CoolstreamingBase
{
    parameters:
        @class(CoolstreamingPeer);

        int coolstreaming_Ts = default(100);
        int coolstreaming_Tp = default(150);
        double coolstreaming_Ta @unit(s) = default(10.0s);
}


