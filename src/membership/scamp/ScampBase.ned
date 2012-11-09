//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package simpleoverlay.applications;

//import simpleoverlay.applications.CommBase;
import simpleoverlay.applications.MembershipBase;

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
