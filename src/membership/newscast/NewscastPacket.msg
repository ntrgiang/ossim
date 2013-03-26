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
// NewscastPacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

cplusplus {{
#include <PeerStreamingPacket_m.h>
#include "NewscastCache.h"
//#include <string>
}}



class PeerStreamingPacket;
class NewscastCache;

// Messages for the NEWSCAST-Protocol
// @author Thorsten Jacobi

enum NewscastPacketType
{
	NEWSCAST_REQUEST            = 0;
	NEWSCAST_REPLY              = 1;
}

packet NewscastPacket extends PeerStreamingPacket
{
    packetGroup = PACKET_GROUP_GOSSIP_OVERLAY;
    
    short packetType     @getter(getPacketType) @setter(setPacketType) @enum(NewscastPacketType) ;
    NewscastCache cache @getter(getCache)      @setter(setCache);       
}

packet NewscastRequestPacket extends NewscastPacket
{
	packetType = NEWSCAST_REQUEST;
}

packet NewscastReplyPacket extends NewscastPacket
{
	packetType = NEWSCAST_REPLY;
}
