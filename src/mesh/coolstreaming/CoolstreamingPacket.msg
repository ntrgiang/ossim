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
// CoolstreamingPacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


cplusplus {{
#include <MeshPeerStreamingPacket_m.h>
}}



class MeshPeerStreamingPacket;

// Messages for the COOLSTREAMING-Protocol
// @author Thorsten Jacobi

enum CoolstreamingPacketType
{
	COOLSTREAMING_BUFFER_MAP = 0;
	COOLSTREAMING_PARTNERSHIP_REQUEST = 1;
	COOLSTREAMING_PARTNERSHIP_ACCEPT = 2;
	COOLSTREAMING_PARTNERSHIP_REVOKE = 3;
	COOLSTREAMING_CHUNK = 4;
}

packet CoolstreamingPacket extends MeshPeerStreamingPacket
{
    packetGroup = PACKET_GROUP_MESH_OVERLAY;
    
    short packetType     @getter(getPacketType) @setter(setPacketType) @enum(CoolstreamingPacketType) ;     
}

packet CoolstreamingBufferMapPacket extends CoolstreamingPacket
{
	packetType = COOLSTREAMING_BUFFER_MAP;
	
	int sequenceNumbers[];
	bool subscribe[];
}

packet CoolstreamingPartnershipRequestPacket extends CoolstreamingPacket
{
	packetType = COOLSTREAMING_PARTNERSHIP_REQUEST;
}

packet CoolstreamingPartnershipAcceptPacket extends CoolstreamingPacket
{
	packetType = COOLSTREAMING_PARTNERSHIP_ACCEPT;
}

packet CoolstreamingPartnershipRevokePacket extends CoolstreamingPacket
{
	packetType = COOLSTREAMING_PARTNERSHIP_REVOKE;
}




packet CoolstreamingChunkPacket extends CoolstreamingPacket
{
	packetType = COOLSTREAMING_CHUNK;
	int sequenceNumber  @getter(getSequenceNumber) @setter(setSequenceNumber);
}
