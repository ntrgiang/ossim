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
// AppSettingDonet.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef APPSETTINGDONET_H_
#define APPSETTINGDONET_H_

#include "AppSetting.h"

class AppSettingDonet : public AppSetting {
public:
    AppSettingDonet();
    virtual ~AppSettingDonet();

protected:
    void handleMessage(cMessage* msg);
    virtual void initialize();
    virtual void finish();

public:
    // -- Direct parameters
    int getVideoStreamBitRate(void);
    int getChunkSize(void);
    int getBufferMapSizeSecond(void);
    double getBufferMapInterval(void);

    // -- Indirect parameters
    int getVideoStreamChunkRate(void);
    int getBufferMapSizeChunk(void);
    double getIntervalNewChunk(void);

    int getPacketSizePartnershipRequest(void) const;
    int getPacketSizePartnershipAccept(void) const;
    int getPacketSizePartnershipReject(void) const;
    int getPacketSizePartnershipAcceptAck(void) const;
    int getPacketSizePartnershipLeave(void) const;
    int getPacketSizeBufferMap(void) const;
    int getPacketSizeChunkRequest(void) const;

    int getPacketSizeVideoChunk(void) const;

protected:
    int param_videoStreamBitRate; //par("videoStreamBitRate").longValue();
    int param_chunkSize;
    int param_bufferMapSize_second;

    int m_videoStreamChunkRate;
    int m_bufferMapSize_chunk;
    int m_bufferMapPacketSize_bit;

    double m_interval_newChunk;
    double param_interval_bufferMap;

    int constant_packet_size_mesh_partnership_request;
    int constant_packet_size_mesh_partnership_accept;
    int constant_packet_size_mesh_partnership_reject;
    int constant_packet_size_mesh_partnership_accept_ack;
    int constant_packet_size_mesh_partnership_leave;
    int constant_packet_size_mesh_buffer_map;
    int constant_packet_size_mesh_chunk_request;

    int constant_packet_size_video_chunk;
};

#endif /* APPSETTINGDONET_H_ */
