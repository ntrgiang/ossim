//  
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// ScampBase.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


package so.membership.scamp;

import so.membership.MembershipBase;

simple ScampBase extends MembershipBase
{
    parameters:
        @class(ScampBase);

        // Scamp Timer
        double isoCheckInterval     @unit(s) = default(0.5s);
        double heartbeatInterval    @unit(s) = default(0.5s);
        double appMessageInterval   @unit(s) = default(0.5s);
        
        // Join
        volatile double startTime   @unit(s) = default(uniform(0.5s,1s));

        // SCAMP protocol parameter
        int c = default(0); 

        // Statistic reporting
        double time_reportPvSize @unit(s) = default(0.5s);
//
//    gates:
//        // -- Gates to the Dispatcher module
//        input dpIn @labels(DpControlInfo/up);
//        output dpOut @labels(DpControlInfo/down);
}
