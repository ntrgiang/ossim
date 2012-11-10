

package so.membership.scamp;

import so.membership.MembershipBase;

//simple ScampBase extends CommBase
simple ScampBase extends MembershipBase
{
    parameters:
        @class(ScampBase);

        // Scamp Timer
        double isoCheckInterval     @unit(s) = default(0.5s);
        double heartbeatInterval    @unit(s) = default(0.5s);
        double appMessageInterval   @unit(s) = default(0.5s);
        
        // Join
        double startTime            @unit(s) = default(uniform(0.5s,1s));

        // SCAMP protocol parameter
        int c = default(0); 
//
//    gates:
//        // -- Gates to the Dispatcher module
//        input dpIn @labels(DpControlInfo/up);
//        output dpOut @labels(DpControlInfo/down);
}
