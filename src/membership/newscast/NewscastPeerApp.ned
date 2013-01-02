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

package so.membership.newscast;

//import inet.applications.udpapp.UDPBasicApp;
import so.membership.newscast.NewscastPeer;
import so.common.Dispatcher;

module NewscastPeerApp
{
    parameters:
        @display("i=block/app;bgb=142,186");

        int videoStreamBitRate @unit(bps) = default(300kbps);
        int chunkSize @unit(byte) = default(1250B);
        int bufferMapSize @unit(s) = default(120s);

    gates:
        input udpIn;
        output udpOut;

    submodules:
        gossip: NewscastPeer {
            @display("p=69,44");
        }
        dispatcher: Dispatcher {

            @display("p=69,123");
        }
    connections:
        // Connect Gossip module to the Dispatcher
        dispatcher.overlayIn++ <-- gossip.dpOut;
        dispatcher.overlayOut++ --> gossip.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}

