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
// AppSettingDonet.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "AppSettingDonet.h"

Define_Module(AppSettingDonet)

AppSettingDonet::AppSettingDonet() {}

AppSettingDonet::~AppSettingDonet() {}

void AppSettingDonet::handleMessage(cMessage* msg)
{
    throw cException("AppSettingDonet does not process messages!");
}

void AppSettingDonet::initialize()
{
    // -- Read all parameters

    param_interval_bufferMap         = par("interval_bufferMap").doubleValue();

    param_videoStreamBitRate        = par("videoStreamBitRate").longValue();
    param_chunkSize                 = par("chunkSize").longValue();
    param_bufferMapSize_second      = par("bufferMapSize").longValue();

    m_videoStreamChunkRate = param_videoStreamBitRate / param_chunkSize / 8;
    m_bufferMapSize_chunk = param_bufferMapSize_second * m_videoStreamChunkRate;

//    m_bufferMapPacketSize_bit = 2 * 8 + (int)(m_bufferMapSize_chunk / 8) * 8 + 1 * 8;

    m_interval_newChunk = ((double)param_chunkSize * 8) / param_videoStreamBitRate;



    // --------------------- Packet sizes ------------------------
    // --- Mesh
    constant_packet_size_mesh_partnership_request   = 5;
        // 1 byte: Packet type
        // 4 byte: Information about upload bandwidth of the requester

    constant_packet_size_mesh_partnership_accept    = 5;
        // 1 byte: Packet type
        // 4 byte: Information about upload bandwidth of the accepter

    constant_packet_size_mesh_partnership_reject    = 1;
        // 1 byte: Packet type

    constant_packet_size_mesh_partnership_accept_ack = 1;
        // 1 byte: Packet type

    constant_packet_size_mesh_partnership_leave = 1;
        // 1 byte: Packet type

    if (m_bufferMapSize_chunk % 8 == 0)
        constant_packet_size_mesh_buffer_map = 1 + 4 + m_bufferMapSize_chunk / 8;
            // 1 (Byte): Packet Type
            // 4 (Bytes): Sequence number of the first chunk in the Buffer Map
    else
        constant_packet_size_mesh_buffer_map = 1 + 4 + m_bufferMapSize_chunk / 8 + 1;
            // 1 additional Byte: to store enough information about BM elements

    constant_packet_size_mesh_chunk_request = constant_packet_size_mesh_buffer_map;

    constant_packet_size_video_chunk = param_chunkSize + 4;
        // chunkSize (bytes) for the chunk itself
        // 4 (bytes) for the Sequence number of the chunk


    // ------------------------------- Watches -------------------------
    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_interval_newChunk);
    WATCH(param_interval_bufferMap);

    WATCH(constant_packet_size_mesh_partnership_request);
    WATCH(constant_packet_size_mesh_partnership_accept);
    WATCH(constant_packet_size_mesh_partnership_reject);
    WATCH(constant_packet_size_mesh_buffer_map);
    WATCH(constant_packet_size_mesh_chunk_request);
    WATCH(constant_packet_size_video_chunk);
}

void AppSettingDonet::finish()
{

}
int AppSettingDonet::getVideoStreamBitRate(void)
{
    return param_videoStreamBitRate;
}

int AppSettingDonet::getChunkSize(void)
{
    return param_chunkSize;
}

int AppSettingDonet::getBufferMapSizeSecond(void)
{
    return param_bufferMapSize_second;
}

int AppSettingDonet::getVideoStreamChunkRate(void)
{
    return m_videoStreamChunkRate;
}

int AppSettingDonet::getBufferMapSizeChunk(void)
{
    return m_bufferMapSize_chunk;
}

double AppSettingDonet::getIntervalNewChunk(void)
{
    return m_interval_newChunk;
}

// ----------------------------- Packet sizes ----------------------------------
int AppSettingDonet::getPacketSizePartnershipRequest(void) const
{
    return constant_packet_size_mesh_partnership_request;
}

int AppSettingDonet::getPacketSizePartnershipAccept(void) const
{
    return constant_packet_size_mesh_partnership_accept;
}

int AppSettingDonet::getPacketSizePartnershipReject(void) const
{
    return constant_packet_size_mesh_partnership_reject;
}

int AppSettingDonet::getPacketSizePartnershipAcceptAck(void) const
{
    return constant_packet_size_mesh_partnership_accept_ack;
}

int AppSettingDonet::getPacketSizePartnershipLeave(void) const
{
    return constant_packet_size_mesh_partnership_leave;
}

int AppSettingDonet::getPacketSizeBufferMap(void) const
{
    return constant_packet_size_mesh_buffer_map;
}

int AppSettingDonet::getPacketSizeChunkRequest(void) const
{
    return constant_packet_size_mesh_chunk_request;
}

int AppSettingDonet::getPacketSizeVideoChunk(void) const
{
    return constant_packet_size_video_chunk;
}

double AppSettingDonet::getBufferMapInterval()
{
    return param_interval_bufferMap;
}
