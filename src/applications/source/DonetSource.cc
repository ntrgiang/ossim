//#include "VideoSource.h"

#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"

#include "DonetSource.h"
#include "DpControlInfo_m.h"
#include "MeshPeerStreamingPacket_m.h"

Define_Module(DonetSource);

DonetSource::DonetSource() {}
DonetSource::~DonetSource()
{
    if (timer_sendBufferMap  != NULL) { delete cancelEvent(timer_sendBufferMap);     timer_sendBufferMap = NULL; }
}

void DonetSource::initialize(int stage)
{
    if (stage != 3)
        return;

    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    findNodeAddress();

    param_maxNOP = par("maxNOP");

    // -------------------------------------------------------------------------
    // -------------------------------- Timers ---------------------------------
    // -------------------------------------------------------------------------
    timer_sendBufferMap = new cMessage("MESH_SOURCE_TIMER_SEND_BUFFERMAP");

    // -- Register itself to the Active Peer Table
    m_apTable->addPeerAddress(getNodeAddress());

    // -- Schedule events
    scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);

    // --- For logging variables
    m_arrivalTime = -1.0;
    m_joinTime = -1.0;
    m_video_startTime = -1.0;
    m_head_videoStart = -1L;
    m_begin_videoStart = -1L;
    m_threshold_videoStart = m_bufferMapSize_chunk/2;
    m_nChunkRequestReceived = 0L;
    m_nChunkSent = 0L;

    // -------------------------------------------------------------------------
    // -------------------------------- WATCH ----------------------------------
    // -------------------------------------------------------------------------
    WATCH(m_localAddress);
    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_interval_bufferMap);
    WATCH(param_upBw);
    WATCH(param_downBw);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);
    WATCH(param_maxNOP);

    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_BufferMapPacketSize_bit);

    WATCH(m_appSetting);
    WATCH(m_apTable);
    WATCH(m_partnerList);
    WATCH(m_videoBuffer);
}

void DonetSource::finish()
{
    //    if (m_videoBuffer != NULL) delete m_videoBuffer;

    if (timer_sendBufferMap  != NULL) { delete cancelEvent(timer_sendBufferMap);     timer_sendBufferMap = NULL; }

/*
    Partnership p;
        p.address = getNodeAddress();
        p.arrivalTime = 0.0;
        p.joinTime = 0.0;
        p.nPartner = m_partnerList->getSize();
        p.video_startTime = -1.0;
        p.head_videoStart = -1;
        p.begin_videoStart = -1;
        p.threshold_videoStart = -1;
    m_meshOverlayObserver->writeToFile(p);
*/
    //reportStatus();
}

/**
 * ----------------
 * Helper functions
 * ----------------
 */

void DonetSource::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_sendBufferMap)
    {
        m_videoBuffer->printStatus();

        sendBufferMap();
        scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);

    }
}

/*
 * This function DELETE the message
 */
void DonetSource::processPacket(cPacket *pkt)
{
    Enter_Method("processPacket(pkt)");

    // -- Get the address of the source node of the Packet
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress sourceAddress = controlInfo->getSrcAddr();

    PeerStreamingPacket *appMsg = dynamic_cast<PeerStreamingPacket *>(pkt);

    if (appMsg == NULL)
        return;
    if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
    {
        throw cException("Wrong packet type!");
    }

    MeshPeerStreamingPacket *meshMsg = dynamic_cast<MeshPeerStreamingPacket *>(appMsg);
    switch (meshMsg->getPacketType())
    {
    case MESH_PARTNERSHIP_REQUEST:
    {
        processPartnershipRequest(pkt);
        break;
    }
    case MESH_BUFFER_MAP:
    {
        // Does NOTHING! Video Source does not process Buffer Map
        break;
    }
    case MESH_CHUNK_REQUEST:
    {
        processChunkRequest(pkt);
        break;
    }
    default:
    {
        // Should be some errors happen
        EV << "Errors when receiving unexpected message!" ;
        break;
    }
    } // End of switch

    delete pkt;
}

/*
 * 1. Store the address into its neighbor list
 * 2. Send the explicit ACCEPT request
 * 3. Check if this is the first neighbor
 */
void DonetSource::acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress, double upBw_remotePeer)
{
    Enter_Method("acceptJoinRequest(reqPeerAddress)");
    // 2. Store the address into its neighbor list
    m_partnerList->addAddress(reqPeerAddress, upBw_remotePeer);

    // Debug:
    m_partnerList->print();

    MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();

    // -- Send the packet
    sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);
}

void DonetSource::processPartnershipRequest(cPacket *pkt)
{
    EV << "Processing partnership request:" << endl;
    // -- Get the IP-address of the peer
    IPvXAddress requesterAddress;
    int requesterPort;
    getSender(pkt, requesterAddress, requesterPort);
    EV << "\tRequester: " << requesterAddress << ":" << requesterPort << endl; // Debug

    if (!canAcceptMorePartner())
    {
        MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
        sendToDispatcher(rejectPkt, m_localPort, requesterAddress, requesterPort);

        return;
    }

    // 1.1. Get the upBw of the remote peer
    MeshPartnershipRequestPacket *memPkt = dynamic_cast<MeshPartnershipRequestPacket *>(pkt);
    double upBw_remotePeer = memPkt->getUpBw();
    EV << "up load bandwidth: " << upBw_remotePeer << endl;

    m_partnerList->addAddress(requesterAddress, upBw_remotePeer);

    // m_partnerList->print(); // Debug:

    MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
    sendToDispatcher(acceptPkt, m_localPort, requesterAddress, requesterPort);

}

/*
bool DonetSource::canHaveMorePartner(void)
{
    if (m_partnerList->getSize() < param_maxNOP)
        return (true);

    return (false);
}
*/
