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
// MeshPeerStreamingPacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

cplusplus {{
#include <PeerStreamingPacket_m.h>
}}

class PeerStreamingPacket;

enum MeshPeerStreamingPacketType
{
    MESH_PARTNERSHIP    = 0;
    MESH_KEEP_ALIVE     = 1;
    MESH_BUFFER_MAP     = 2;
    MESH_CHUNK_REQUEST  = 3;
    MESH_CHUNK_REPLY    = 4;
    MESH_VIDEO_CHUNK    = 5;
    // ------------------------------ Partnership
    MESH_PARTNERSHIP_REQUEST    = 10;
    MESH_PARTNERSHIP_ACCEPT     = 11;
    MESH_PARTNERSHIP_REJECT     = 12;
    //MESH_PARTNERSHIP_ACCEPT_ACK = 13;
    MESH_PARTNERSHIP_LEAVE      = 13;
}

packet MeshPeerStreamingPacket extends PeerStreamingPacket
{
    packetGroup = PACKET_GROUP_MESH_OVERLAY;
    short packetType @getter(getPacketType) @setter(setPacketType) @enum(MeshPeerStreamingPacketType) ;
}

packet MeshPartnershipRequestPacket extends MeshPeerStreamingPacket
{
    packetType = MESH_PARTNERSHIP_REQUEST;
    double upBw @default(-1.0);
}

packet MeshPartnershipAcceptPacket extends MeshPeerStreamingPacket
{
    packetType = MESH_PARTNERSHIP_ACCEPT;
    double upBw @default(-1.0);
}

packet MeshPartnershipRejectPacket extends MeshPeerStreamingPacket
{
    packetType = MESH_PARTNERSHIP_REJECT;
}

//packet MeshPartnershipAcceptAckPacket extends MeshPeerStreamingPacket
//{
//    packetType = MESH_PARTNERSHIP_ACCEPT_ACK;
//}

packet MeshPartnershipLeavePacket extends MeshPeerStreamingPacket
{
    packetType = MESH_PARTNERSHIP_LEAVE;
}


packet MeshBufferMapPacket extends MeshPeerStreamingPacket
{
    packetType = MESH_BUFFER_MAP;
    // long idStart; // should be of type SEQUENCE_NUMBER_T
    // long idEnd; // should be of type SEQUENCE_NUMBER_T
    
    long bmStart_seqNum     @getter(getBmStartSeqNum)   @setter(setBmStartSeqNum);
    long bmEnd_seqNum       @getter(getBmEndSeqNum)     @setter(setBmEndSeqNum);
    long head_seqNum        @getter(getHeadSeqNum)      @setter(setHeadSeqNum);
    
    bool bufferMap[];
}

packet MeshChunkRequestPacket extends MeshPeerStreamingPacket
{
    packetType = MESH_CHUNK_REQUEST;
    
    long seqNum_mapStart     @getter(getSeqNumMapStart)     @setter(setSeqNumMapStart);
    long seqNum_mapEnd       @getter(getSeqNumMapEnd)       @setter(setSeqNumMapEnd);
    long seqNum_mapHead      @getter(getSeqNumMapHead)      @setter(setSeqNumMapHead);
    
    bool requestMap[];
}



//packet MeshVideoChunkPacket extends MeshPeerStreamingPacket
//{
//    packetType = MESH_VIDEO_CHUNK;
//    long seqNumber @getter(getSeqNumber) @setter(setSeqNumber);
//    double timeStamp @getter(getTimeStamp) @setter(setTimeStamp);
//}

// ------------- new parts ------------


// ----- Old stuff - obsolete

//enum MeshPeerStreamingPacketType
//{
//    MESH_PARTNERSHIP    = 0;
//    MESH_KEEP_ALIVE     = 1;
//    MESH_BUFFER_MAP     = 2;
//    MESH_CHUNK_REQUEST  = 3;
//    MESH_CHUNK_REPLY    = 4;
//    MESH_VIDEO_CHUNK    = 5;
//}
//

//packet MeshPartnershipPacket extends MeshPeerStreamingPacket
//{
//    packetType = MESH_PARTNERSHIP;
//    short command enum(MeshPartnershipCommandType);
//    double upBw @default(-1.0);
//}

//packet MeshKeepAlivePacket extends MeshPeerStreamingPacket
//{
//    packetType = MESH_KEEP_ALIVE;
//}

//packet MeshChunkReplyPacket extends MeshPeerStreamingPacket
//{
//    packetType = MESH_CHUNK_REPLY;
//}

//enum MeshPartnershipCommandType
//{
//    CMD_MESH_PARTNERSHIP_REQUEST    = 0;
//    CMD_MESH_PARTNERSHIP_ACCEPT     = 1;
//    CMD_MESH_PARTNERSHIP_REJECT     = 2;
//}

