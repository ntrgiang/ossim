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
// CoolstreamingSourceApp.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

package so.mesh.coolstreaming;

import so.common.Dispatcher;
import so.membership.MCache;
import so.mesh.coolstreaming.CoolstreamingSource;
import so.mesh.PartnerList;
import so.payload.ChunkGenerator;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.membership.newscast.NewscastPeer;
import so.membership.dummy.DummyMembership;

module CoolstreamingSourceApp
{
    parameters:
        @display("bgb=287,308");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        mesh: CoolstreamingSource {
            @display("p=133,179;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=212,179;i=block/rxtx");
        }
        mCache: MCache {
            @display("p=52,110;i=abstract/cache");
        }
        videoBuffer: VideoBuffer {
            @display("p=212,110;i=block/buffer2");
        }
        chunkGenerator: ChunkGenerator {
            @display("p=212,37;i=block/source");
        }
        partnerList: PartnerList {

            @display("p=133,110");
        }
        
        //gossipProtocol : NewscastPeer;
        gossipProtocol : DummyMembership;
        
        dispatcher: Dispatcher {
            @display("p=133,253;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=41,191");
    //        }
    connections:
        // Connect Forwarder module to the NewscastPeer
        dispatcher.overlayIn++ <-- gossipProtocol.dpOut;
        dispatcher.overlayOut++ --> gossipProtocol.dpIn;
        
        // Connect Mesh module to the Dispatcher
        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        // Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;
        
        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
