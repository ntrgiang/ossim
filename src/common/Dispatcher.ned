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
// Dispatcher.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//



package so.common;

import inet.applications.udpapp.UDPApp;

simple Dispatcher like UDPApp
{
    parameters:
        @class(Dispatcher); 
        
        int localPort = default(9000); // port number to listen onComm
        int destPort  = default(9000);

    gates:
        // To upper layers
        input overlayIn[]   @label(DispatcherControlInfo/down);
        output overlayOut[] @label(DispatcherControlInfo/up);

        // To lower UDP layer
        input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        
//        input GossipIn @labels(GossipControlInfo/up);
//        output GossipOut @labels(GossipControlInfo/down);
        
}
