
package simpleoverlay.global;

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
        
        @statistic[aptSize](title="Active Peer Table Size"; source=Signal_Size; record=vector);
}

