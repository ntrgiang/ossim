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
// CoolstreamingBase.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


// @author Thorsten Jacobi
// @brief base class for source and peer of newscast
// @ingroup mesh
// @ingroup coolstreaming

#ifndef CoolstreamingBase_H_
#define CoolstreamingBase_H_

#include "CommBase.h"

#include "PartnerList.h"
#include "VideoBuffer.h"
#include "Forwarder.h"
#include "ActivePeerTable.h"
#include "AppSettingDonet.h"
#include "IChurnGenerator.h"
#include "MeshOverlayObserver.h"
#include "Logger.h"

#include "GossipProtocol.h"

#include "IPvXAddress.h"
#include <fstream>


#include "CoolstreamingPacket_m.h"
#include "CoolstreamingPartner.h"

//class Logger;

//#define MESH_STATE_JOIN_IDLE                    0
//#define MESH_STATE_JOIN_WAITING_ACCEPT          1
//#define MESH_STATE_JOIN_ALL_ACCEPT_RECEIVED     2
//#define MESH_STATE_JOIN_WAITING_ACCEPT_ACK      3

enum Mesh_Join_State {
    MESH_JOIN_STATE_IDLE            = 0,
    MESH_JOIN_STATE_IDLE_WAITING    = 1,
    MESH_JOIN_STATE_ACTIVE          = 2,
    MESH_JOIN_STATE_ACTIVE_WAITING  = 3
};

class CoolstreamingBase : public CommBase, public VideoBufferListener
{
public:
    CoolstreamingBase();
    virtual ~CoolstreamingBase();

    void handleMessage(cMessage *msg);

    // @brief checks if a address is a partner
    bool isPartner(IPvXAddress addr);
    // @brief finds the pointer to a partner for a given address
    CoolstreamingPartner* findPartner(IPvXAddress addr);

//protected:
//    virtual int numInitStages() const { return 4; }

//    virtual void initialize(int stage);
//    virtual void finish();

protected:
    // @brief handles the intiation of this base class
    void initBase();

    virtual void processPacket(cPacket *pkt);
    virtual void handleTimerMessage(cMessage *msg);

    int param_SubstreamCount;

    // @brief pointer to the gossip layer
    GossipProtocol* m_Gossiper;

    // DEBUG ->
    bool debugOutput;
    std::ofstream m_outFileDebug;
    // <- DEBUG

    // partnership managment ->
protected:
    // @brief maximum number of partners
    unsigned int param_maxNOP;
    // @brief timeout for inactive partners
    double param_PartnerTimeout;
    // @brief interval to check the current number of partners
    double param_CheckPartnersInterval;
    // @brief interval  to send the buffermap to partners
    double param_BufferMapInterval;

    // @brief list of current partners
    std::vector<CoolstreamingPartner*> partners;

    cMessage* timer_CheckPartners;
    cMessage* timer_BufferMap;

    // @brief intervall function for checking partners
    virtual void checkPartners();
    // @brief removes inactive partners from list
    void removeTimeoutedPartners();

    // @brief returns a pointer to the parent/partner of a substream
    CoolstreamingPartner* getParent(int substream);

private:
    // @brief handles partnership request/accept/revoke
    void processPartnershipPacket(CoolstreamingPacket *pkt);
    // @brief adds an address as a partner
    CoolstreamingPartner* addPartner(IPvXAddress addr);
protected:
    // @brief removes a partner by address
    void removePartner(IPvXAddress addr);
    // <- partnership

protected:

    // Buffer Map-Handling
    // @brief sends the current buffermap to a given partner
    void sendBufferMap(CoolstreamingPartner* dest);
    // @brief processes a received buffermap
    void processBufferMapPacket(CoolstreamingBufferMapPacket* pkt);

    // Chunk handling
    // @brief send a chunk identified by number to a partner
    void sendChunk(CoolstreamingPartner* dest, int number);
    // @brief gets called whenever a new chunk is received and sends it to his children
    virtual void onNewChunk(int sequenceNumber);


    // -- utility functions
    void getAppSetting(void);
    void readChannelRate(void);
    double getDownloadBw();
    double getUploadBw();

    // @brief gets the latest sequence number with at least 5 connected chunks (checking from end of buffer to start)
    int getLatestSequenceNumber(int substream);
    // @brief gets the latest sequence number of connected chunks from the beginning of the buffer
    int getLatestSequenceNumberB(int substream);

    // -- Overloading functions
    void bindToGlobalModule(void);
    void bindToMeshModule(void);

protected:
    int m_localPort, m_destPort;

    double param_upBw;
    double param_downBw; // Download bandwidth is not neccessary!

    int param_videoStreamBitRate;
    int param_chunkSize;

    double param_interval_bufferMap;

    // -- Variables for settings
    int m_videoStreamChunkRate;     /* in [chunks/second] */
    int m_bufferMapSize_chunk;      /* in [chunks] */

    // -- Pointers to /global/ modules
    AppSettingDonet         *m_appSetting;
    IChurnGenerator         *m_churn;
    MeshOverlayObserver     *m_meshOverlayObserver;
    Logger                  *m_logger;

    // -- Pointers to /local/ modules
    VideoBuffer *m_videoBuffer;
    Forwarder   *m_forwarder;
    MembershipBase *m_memManager;
};

#endif /* CoolstreamingBase_H_ */
