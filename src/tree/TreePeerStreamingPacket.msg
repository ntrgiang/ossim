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
// TreePeerStreamingPacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

cplusplus {{
#include <PeerStreamingPacket_m.h>
#include <IPvXAddress.h>

#include <MultitreeMessages.h>

typedef std::vector<ConnectRequest> CRVector;
typedef std::vector<ConnectConfirm> CCVector;
typedef std::vector<DisconnectRequest> DRVector;
typedef std::vector<PassNodeRequest> PNRVector;
typedef std::vector<SuccessorInfo> SIVector;
}}

class PeerStreamingPacket;
class IPvXAddress extends void;

class noncobject CRVector;
class noncobject CCVector;
class noncobject DRVector;
class noncobject PNRVector;
class noncobject SIVector;

enum TreePeerStreamingPacketType
{
    TREE_KEEP_ALIVE     = 0;
    TREE_SUCCESSOR_INFO = 1;

	// ----------------------- Partnership
	TREE_CONNECT_REQUEST = 10;
	TREE_CONNECT_CONFIRM= 11;
	TREE_DISCONNECT_REQUEST = 12;
	TREE_PASS_NODE_REQUEST = 13;
}

packet TreePeerStreamingPacket extends PeerStreamingPacket
{
    packetGroup = PACKET_GROUP_TREE_OVERLAY;
    short packetType @getter(getPacketType) @setter(setPacketType) @enum(TreePeerStreamingPacketType);
}

packet TreeConnectRequestPacket extends TreePeerStreamingPacket
{
    packetType = TREE_CONNECT_REQUEST;

	CRVector requests;
}

packet TreeConnectConfirmPacket extends TreePeerStreamingPacket
{
    packetType = TREE_CONNECT_CONFIRM;

	CCVector confirms;
}

packet TreeDisconnectRequestPacket extends TreePeerStreamingPacket
{
    packetType = TREE_DISCONNECT_REQUEST;

	DRVector requests;
}

packet TreePassNodeRequestPacket extends TreePeerStreamingPacket
{
    packetType = TREE_PASS_NODE_REQUEST;

	PNRVector requests;
}

packet TreeSuccessorInfoPacket extends TreePeerStreamingPacket
{
    packetType = TREE_SUCCESSOR_INFO;

	SIVector updates;
}
