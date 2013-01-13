
package so.global;

//
// Stores the (IP) addresses of active peers.
//
// This module has no gates; all functionality can be accessed via member
// functions of the C++ module class. For detailed info, please see the C++
// documentation of the class (Doxygen).
//
// @see InterfaceTable
// @author: Giang Nguyen
//
simple ActivePeerTable
{
    parameters:
        @display("i=block/table");
        double interval_reportSize @unit(s) = default(0.5s);
        
        @signal[Signal_Size](type="long");
        @statistic[aptSize](title="Size of Active Peer Table"; source=Signal_Size; record=last,vector);
}

