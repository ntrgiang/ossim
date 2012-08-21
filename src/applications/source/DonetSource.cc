//#include "VideoSource.h"

#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"

#include "DonetSource.h"
#include "DpControlInfo_m.h"
#include "MeshPeerStreamingPacket_m.h"

#include <assert.h>

Define_Module(DonetSource);

DonetSource::DonetSource() {}
DonetSource::~DonetSource() {}

void DonetSource::initialize(int stage)
{
    if (stage != 3)
        return;

    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    param_bufferMapInterval     = par("bufferMapInterval").doubleValue();
    param_numberOfPartner       = par("numberOfPartner");

    // -------------------------------------------------------------------------
    // -------------------------------- Timers ---------------------------------
    // -------------------------------------------------------------------------
    timer_sendBufferMap = new cMessage("MESH_SOURCE_TIMER_SEND_BUFFERMAP");

    // -- Register itself to the Active Peer Table
    // -- This function call has problem
    m_apTable->addPeerAddress(getNodeAddress());

    // TODO: verification!!!
    // m_videoBuffer = new VideoBuffer(m_bufferMapSize_chunk);

    // -- Schedule events
    scheduleAt(simTime() + param_bufferMapInterval, timer_sendBufferMap);

    // -------------------------------------------------------------------------
    // -------------------------------- WATCH ----------------------------------
    // -------------------------------------------------------------------------
    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_numberOfPartner);

    WATCH(param_upBw);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);

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

    // TODO Why should I have this variable so that I have to delete it!
    //    if (m_bufferMap != NULL) delete m_bufferMap;

    // Timer
    //    if (timer_sendBufferMap)    delete cancelEvent(timer_sendBufferMap);
    if (timer_sendBufferMap) cancelAndDelete(timer_sendBufferMap);

    // already moved the functionality out of the module
    //    if (timer_startVideo)       delete cancelEvent(timer_startVideo);
    //    if (timer_newChunk)         delete cancelEvent(timer_newChunk);

    // Packets
    //if (m_bmPacket) delete m_bmPacket;
}

/**
 * ----------------
 * Helper functions
 * ----------------
 */

void DonetSource::handleTimerMessage(cMessage *msg)
{
/*
    if (msg == timer_startVideo)
    {
        // Start the video stream
//        m_chunkSeqNumber_upper = 0L;
//        m_chunkSeqNumber_lower = 0L;
//        m_id_bmStart = 0L;
//        m_id_bmEnd = 0L;

        m_id_newChunk = 0L;
        // Schedule the next chunk
        scheduleAt(simTime() + m_newChunkTimerInterval, timer_newChunk);

        MeshVideoChunkPacket *chunk = new MeshVideoChunkPacket("MESH_VIDEO_CHUNK_PACKET");
            chunk->setBitLength(param_chunkSize);
            chunk->setSeqNumber(0L);
            chunk->setTimeStamp(simTime().dbl()); // Set the time-stamp of the chunk to current time
        ++m_id_newChunk;

//        m_videoBuffer->insertPacket(chunk, current_time);
        m_videoBuffer->insertPacket(chunk);
        // TODO: revision needed -- possible duplication of storing timeStamps
    }
    else if (msg == timer_newChunk)
    {
//        ++m_chunkSeqNumber_upper;
//        m_chunkSeqNumber_lower = std::max(0L, m_chunkSeqNumber_upper - m_bufferMapSize_chunk);
//        r_latestID_vector.record(m_chunkSeqNumber_upper);

        // -- Module validation
        // r_latestID_vector.record(m_id_newChunk);

        // -- Create a new Chunk Message and attach it to the Buffer
        MeshVideoChunkPacket *chunk = new MeshVideoChunkPacket("MESH_VIDEO_CHUNK_PACKET");
            chunk->setBitLength(param_chunkSize);
            chunk->setSeqNumber(m_id_newChunk);
            chunk->setTimeStamp(simTime().dbl());
        ++m_id_newChunk;
//        m_videoBuffer->insertPacket(chunk, current_time);
        m_videoBuffer->insertPacket(chunk);

        // Schedule the next chunk
        scheduleAt(simTime() + m_newChunkTimerInterval, timer_newChunk);

        // -- output the status of the VideoStreamBuffer
        // EV << "[" << m_videoBuffer->getID_bufferStart() << ", " << m_videoBuffer->getID_bufferEnd() << "]" << endl;

        // -- records status of the video Buffer
        // r_idBmStart_vector.record(m_videoBuffer->getID_bufferStart());
        // r_idBmEnd_vector.record(m_videoBuffer->getID_bufferEnd());

//        EV << "[" << m_chunkSeqNumber_lower << ", " << m_chunkSeqNumber_upper << "]" << endl;
//        EV << "[" << m_id_bmStart << ", " << m_id_bmEnd << "]" << endl;
    }
    else if (msg == timer_sendBufferMap)
*/
    if (msg == timer_sendBufferMap)
    {
//        m_videoBuffer->printStatus();
        sendBufferMap();
        scheduleAt(simTime() + param_bufferMapInterval, timer_sendBufferMap);

        // Result recording
//        r_idBmStart_vector.record(m_bufferMap->getID_bmStart());
//        r_idBmEnd_vector.record(m_bufferMap->getID_bmEnd());
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
//    if (appMsg->getOverlayType() == MESH_STREAMING_OVERLAY)
    if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
    {
        //EV << "Wrong packet type!" << endl;
        throw cException("Wrong packet type!");
    }

    MeshPeerStreamingPacket *meshMsg = dynamic_cast<MeshPeerStreamingPacket *>(appMsg);
    switch (meshMsg->getPacketType())
    {
//            case MESH_PARTNERSHIP:
//            {
//                MeshPartnershipPacket *pMsg = dynamic_cast<MeshPartnershipPacket *>(appMsg);
//                if (pMsg->getCommand() == CMD_MESH_PARTNERSHIP_REQUEST)
//                {
//                    // -- Get the IP-address of the peer
//                    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pMsg->getControlInfo());
//                    IPvXAddress reqPeerAddress = controlInfo->getSrcAddr();
//
//
//                    // 1.1. Get the upBw of the remote peer
//                    MeshPartnershipPacket *meshPkt = dynamic_cast<MeshPartnershipPacket *>(pkt);
//                    double upBw_remotePeer = meshPkt->getUpBw();
//
//                    // -- problem here
//                    acceptJoinRequestFromPeer(reqPeerAddress, upBw_remotePeer);
//
//                    // 4. Check if this is the first neighbor,
//                    // if YES --> trigger the Heartbeat/BufferMap module (to periodically start sending the hearbeat)
//                    if (m_partnerList->getSize() == 1)
//                    {
//                        // Start sending bufferMap packets
//                        sendBufferMap();
//
//                        scheduleAt(simTime() + param_bufferMapInterval, timer_sendBufferMap);
//                    }
//                }
//                break;
//            }
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
/*
        MeshChunkRequestPacket *reqPkt = check_and_cast<MeshChunkRequestPacket *>(appMsg);

        //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
            EV << "Chunk request received from " << sourceAddress << ": " << endl;
            EV << "Start: " << reqPkt->getSeqNumMapStart()
                << " -- End: " << reqPkt->getSeqNumMapEnd()
                << " -- Head: " << reqPkt->getSeqNumMapHead()
                << " --- ";
            for (int i=0; i < m_bufferMapSize_chunk; ++i)
            {
                EV << reqPkt->getRequestMap(i);
            }
            EV << endl;
        //////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

        EV << "Source's VideoBuffer: ";
        m_videoBuffer->printStatus();

        // -- Find the id of the requested chunk
        SEQUENCE_NUMBER_T start = reqPkt->getSeqNumMapStart();
        for (int offset=0; offset < m_bufferMapSize_chunk; ++offset)
        {
            if (reqPkt->getRequestMap(offset) == true)
            {
                SEQUENCE_NUMBER_T seqNum_requestdChunk = offset + start;

                // -- Look up to see if the requested chunk is available in the Video Buffer
                if (m_videoBuffer->isInBuffer(seqNum_requestdChunk) ==  true)
                {
                    // -- If YES, duplicate the chunk and send to the requesting peer

                    MeshVideoChunkPacket *copyPkt = m_videoBuffer->getChunk(seqNum_requestdChunk)->dup();

                    sendToDispatcher(copyPkt, m_localPort, sourceAddress, m_destPort);
                }
            }
        }
*/
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
/*
void DonetSource::acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress)
{
    Enter_Method("acceptJoinRequest(reqPeerAddress)");

    // 2. Store the address into its neighbor list
    m_partnerList->addAddress(reqPeerAddress);

    // Debug:
    m_partnerList->print();

    // 3. Send the explicit ACCEPT request
    MeshPartnershipPacket *acceptPkt = new MeshPartnershipPacket("MESH_PEER_JOIN");
    acceptPkt->setPacketType(MESH_PARTNERSHIP);
    acceptPkt->setCommand(CMD_MESH_PARTNERSHIP_ACCEPT);
    sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);
}
*/

void DonetSource::acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress, double upBw_remotePeer)
{
    Enter_Method("acceptJoinRequest(reqPeerAddress)");
    // 2. Store the address into its neighbor list
    // m_partnerList->addAddress(reqPeerAddress);
    m_partnerList->addAddress(reqPeerAddress, upBw_remotePeer);

    // 2. Store the address into its neighbor list
    // m_partnerList->addAddress(reqPeerAddress);

    // Debug:
    m_partnerList->print();

    MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
        acceptPkt->setUpBw(param_upBw);

    // 3. Send the explicit ACCEPT request
//    MeshPartnershipPacket *acceptPkt = new MeshPartnershipPacket("MESH_PEER_JOIN_ACCEPT");
//        acceptPkt->setPacketType(MESH_PARTNERSHIP);
        // !!! has problem
//        acceptPkt->setCommand(CMD_MESH_PARTNERSHIP_ACCEPT);
//        acceptPkt->setUpBw(param_upBw);

    // -- Send the packet
    sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);
}

/*
 *  1. Send the Buffer Map
 *  2. Schedule the send the next BufferMap()
 */

/*
void DonetSource::startSendingBufferMap()
{
    // TODO: might change bm into local data member, to avoid frequent creation and deletion of the same object
    sendBufferMap();

    // Set TIMER for the next interval
    scheduleAt(simTime() + m_newChunkTimerInterval,
            new cMessage("MESH_SOURCE_TIMER_BUFFERMAP"));

}
*/

/* ****************************************************************************
 *   0. Confirmation: assert that the neighborList is not empty
 *   1. Create the BufferMap packet
 *   2. Browse through the local NeighborList
 *     . Send the BufferMap Packet to all of the neighbors in the list
  * ****************************************************************************
 */
/*
void DonetSource::sendBufferMap()
{
    // 0. Confirmation: assert that the neighborList is not empty
//    if (m_partnerList->size() == 0)
    if (m_partnerList->getSize() == 0)
    {
        return;
    }

//    r_idStart_vector.record(m_id_bmStart);
//    r_idEnd_vector.record(m_id_bmEnd);

    // 1. Create the BufferMap packet
    // BufferMap *bm = new BufferMap(m_bufferMapSize_chunk);
//    m_bufferMap->reset();
//    m_videoBuffer->captureVideoStreamBuffer(m_bufferMap); // this function has problem!!!

    // Debug
    // Print out the current BufferMap
//    EV << "BM size: " << m_bufferMap->getSize() <<
//            "[" << m_bufferMap->getID_bmStart() << ", " << m_bufferMap->getID_bmEnd() << "]" << endl;
//    m_bufferMap->printBM();

    // -- Create the BufferMap Packet
     MeshBufferMapPacket *bmPkt = new MeshBufferMapPacket("MESH_PEER_BUFFER_MAP");
    bmPkt->setBufferMapArraySize(8);
    m_videoBuffer->fillBufferMapPacket(bmPkt);
    //m_videoBuffer->fillBufferMapPacket(m_bmPacket);

    // Debug

    for (int i = 0; i<bmPkt->getBufferMapArraySize(); i++)
    {
        r_bmPkt.record(bmPkt->getBufferMap(i)==true?1:0);
    }


    // 2. Get the Neighbor List
    std::vector<IPvXAddress> nList = m_partnerList->getAddressList();

    // 3. Browse through the local NeighborList
    //    . Send the BufferMap Packet to all of the neighbors in the list
    std::vector<IPvXAddress>::iterator iter;
    for (iter = nList.begin(); iter != nList.end(); ++iter)
    {
        // Create the BufferMap packet
        MeshBufferMapPacket *bmPkt_copy = bmPkt->dup();

        // send the packet to this neighbor
        sendToDispatcher(bmPkt_copy, m_localPort, *iter, m_destPort);
    }

    // delete bmPkt;
}
*/

/*
void DonetSource::sourceJoin()
{
    Enter_Method("sourceJoin()");

    EV << "@ DonetSource::join()-------------" << endl;

    EV << "localPort = " << localPort << endl;
    EV << "destPort = " << destPort << endl;
    EV << "destAddress = " << destAddresses[0] << endl;
    EV << "packet sent!!! ----------" << endl;


//    sendToDispatcher(new cPacket("dummy_join"), localPort, destAddresses[0], destPort);
}
*/


/*void DonetSource::handlePeerMessage(cMessage *msg)
{
    if (opp_strcmp(msg->getName(), "MESH_PEER_JOIN") == 0)
    {
        if (msg->getKind() == MESH_PEER_JOIN_REQUEST)
        {
            acceptJoinRequest(msg);
        }
    }

//    delete msg;
}*/


void DonetSource::processPartnershipRequest(cPacket *pkt)
{
    // -- Get the IP-address of the peer
    IPvXAddress requesterAddress;
    int requesterPort;
    getSender(pkt, requesterAddress, requesterPort);
    EV << "Requester: " << requesterAddress << ":" << requesterPort << endl; // Debug

    if (!canHaveMorePartner())
    {
        //MeshPartnershipRejectPacket *rejPkt = new MeshPartnershipRejectPacket("MESH_PARTNERSHIP_REJECT");
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
//    MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
//        acceptPkt->setUpBw(param_upBw);

    MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
    sendToDispatcher(acceptPkt, m_localPort, requesterAddress, requesterPort);

}

bool DonetSource::canHaveMorePartner(void)
{
    bool ret = (m_partnerList->getSize() < param_numberOfPartner)?true:false;
    return ret;
}
