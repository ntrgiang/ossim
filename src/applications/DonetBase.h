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

#ifndef DONETBASE_H_
#define DONETBASE_H_

#include "CommBase.h"

#include "PartnerList.h"
#include "VideoBuffer.h"
#include "Forwarder.h"
#include "ActivePeerTable.h"
#include "AppSettingDonet.h"
#include "IChurnGenerator.h"
#include "MeshOverlayObserver.h"
#include "Logger.h"

#include "IPvXAddress.h"
#include <fstream>

//class Logger;

class DonetBase : public CommBase
{
public:
    DonetBase();
    virtual ~DonetBase();

    virtual void handleMessage(cMessage *msg);
    virtual void processPacket(cPacket *pkt) = 0;
    virtual void handleTimerMessage(cMessage *msg) = 0;

protected:
    // -- utility functions
    void getAppSetting(void);
    void readChannelRate(void);
    double getDownloadBw();
    double getUploadBw();

    // -- -- Get the IP address and/or port number
    const IPvXAddress& getSender(const cPacket *pkt) const;
    void getSender(cPacket *pkt, IPvXAddress &senderAddress);
    void getSender(cPacket *pkt, IPvXAddress &senderAddress, int &senderPort);

    // -- Overloading functions
    void bindToGlobalModule(void);

    void bindToMeshModule(void);

    // -- Partner Management
    void acceptJoinRequest(IPvXAddress &reqPeerAddress, double upBw_remotePeer);
    void addToNeighborList(cPacket *pkt);
    bool canAcceptMorePartner(void);

    // -- BufferMap
//    MeshBufferMapPacket *generateBufferMapPacket();
    void sendBufferMap(void);
    inline int getBufferMapSize(void) { return m_bufferMapSize_chunk; }

    void processChunkRequest(cPacket *pkt);

    MeshPartnershipAcceptPacket *generatePartnershipRequestAcceptPacket();
    MeshPartnershipRejectPacket *generatePartnershipRequestRejectPacket();

    // -- Debug
    void reportStatus();

protected:
    int m_localPort, m_destPort;

    // -- Parameters
    int param_maxNOP;
    double param_upBw;
    double param_downBw; // Download bandwidth is not neccessary!

    int param_bufferMapSize_second;
    int param_videoStreamBitRate;
    int param_chunkSize;

    double param_interval_bufferMap;

    // -- Variables for settings
    int m_videoStreamChunkRate;     /* in [chunks/second] */
    int m_bufferMapSize_chunk;      /* in [chunks] */
    int m_BufferMapPacketSize_bit;  /* in [bits]   */

    // -- Timer messages
    cMessage *timer_sendBufferMap;

    // -- Pointers to /global/ modules
    AppSettingDonet         *m_appSetting;
    IChurnGenerator         *m_churn;
    MeshOverlayObserver     *m_meshOverlayObserver;
    Logger                  *m_logger;

    // -- Pointers to /local/ modules
    PartnerList *m_partnerList;
    VideoBuffer *m_videoBuffer;
    Forwarder   *m_forwarder;

    // --- Additional variables for debugging purposes
    double m_arrivalTime;
    double m_joinTime;
    double m_video_startTime;
    SEQUENCE_NUMBER_T m_head_videoStart;
    SEQUENCE_NUMBER_T m_begin_videoStart;
    int m_threshold_videoStart;
    long m_nChunkRequestReceived;
    long m_nChunkSent;
    long m_nBufferMapRecv;

};

#endif /* DONETBASE_H_ */
