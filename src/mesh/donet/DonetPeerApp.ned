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
// DonetPeerApp.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

package so.mesh.donet;

import so.common.Dispatcher;
import so.membership.dummy.DummyMembership;
import so.membership.oracle.OracleMembership;
import so.membership.MCache;

import so.mesh.donet.DonetPeer;
import so.mesh.PartnerList;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.payload.Player;
import so.payload.PlayerSimpleSkip;
//import simpleoverlay.global.AppSettingDonet;

module DonetPeerApp
{
    parameters:
        @display("bgb=270,294");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        membership: DummyMembership {
            @display("p=51,173;i=block/users");
        }
        mesh: DonetPeer {
            @display("p=132,173;i=block/network2");
        }
        partnerList: PartnerList {
            @display("p=132,104;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=211,173;i=block/rxtx");
        }
        videoBuffer: VideoBuffer {
            @display("p=211,104;i=block/buffer2");
        }
        //player: Player {
        player: PlayerSimpleSkip {
            //player: <playerModule> like PlayerBase {
            @display("p=211,44;i=device/terminal");
        }
        dispatcher: Dispatcher {
            @display("p=132,247;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=34,178;i=block/cogwheel");
    //        }
    connections:
        // Connect Gossip module to the Dispatcher
        //
        dispatcher.overlayIn++ <-- membership.dpOut;
        dispatcher.overlayOut++ --> membership.dpIn;

        // Connect Mesh module to the Dispatcher
        //
        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        // Connect Forwarder module to the Dispatcher
        //
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // Connect Dispatcher to module's ports
        //
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;
        
        // Connect Forwarder to Video Buffer
        //forwarder.bufferOut --> videoBuffer.forwarderIn;
        //forwarder.bufferIn <-- videoBuffer.forwarderOut;

        // Connect Video Buffer to Player
        //videoBuffer.playerOut --> player.bufferIn;
}
