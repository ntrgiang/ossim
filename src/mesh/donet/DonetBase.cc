#include "DonetBase.h"
#include "DpControlInfo_m.h"

DonetBase::DonetBase() {}

DonetBase::~DonetBase() {}

void DonetBase::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        processPacket(PK(msg));
    }
}

void DonetBase::getAppSetting(void)
{
    EV << "DonetBase::getAppSetting" << endl;

    // -- AppSetting
    m_localPort = getLocalPort();
    m_destPort = getDestPort();

    param_interval_bufferMap     = m_appSetting->getBufferMapInterval();

    param_videoStreamBitRate    = m_appSetting->getVideoStreamBitRate();
    param_chunkSize             = m_appSetting->getChunkSize();
    param_bufferMapSize_second  = m_appSetting->getBufferMapSizeSecond();

    m_videoStreamChunkRate      = m_appSetting->getVideoStreamChunkRate();
    m_bufferMapSize_chunk       = m_appSetting->getBufferMapSizeChunk();
    m_BufferMapPacketSize_bit   = m_appSetting->getPacketSizeBufferMap() * 8;
//    m_BufferMapPacketSize_bit   = m_appSetting->getBufferMapPacketSizeBit();
}

void DonetBase::readChannelRate(void)
{
//    cDatarateChannel *channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());
//    param_upBw = channel->getDatarate();

    // -- problem unresolved!!!
//    channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
//            ->getTransmissionChannel());
//    param_downBw = channel->getDatarate();

    // -- Trying new functions
    param_upBw = getUploadBw();
    param_downBw = getDownloadBw();
}

double DonetBase::getUploadBw()
{
    cDatarateChannel *channel = check_and_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());

    return channel->getDatarate();
}

double DonetBase::getDownloadBw()
{
    double rate;
    cModule* nodeModule = getParentModule()->getParentModule();

    int gateSize = nodeModule->gateSize("pppg$i");
    for (int i=0; i<gateSize; i++)
    {
        cGate* currentGate = nodeModule->gate("pppg$i",i);
        if (currentGate->isConnected())
        {
            rate = check_and_cast<cDatarateChannel *>(currentGate->getPreviousGate()->getChannel())->getDatarate();
//                capacity += check_and_cast<cDatarateChannel *>
//                    (currentGate->getPreviousGate()->getChannel())->getDatarate()
//                    - uniform(0,800000);
        }
    }
    //channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
                //->getTransmissionChannel());

    return rate;
}

/*
 * Consider whether having "enough" number of partners
 */
bool DonetBase::canAcceptMorePartner(void)
{
    //Enter_Method("canAcceptMorePartner");

    EV << "Current number of partners: " << m_partnerList->getSize() << endl;
    EV << "Max number of partners to have: " << param_maxNOP << endl;
    if (m_partnerList->getSize() < param_maxNOP)
        return true;

    return false;
}

void DonetBase::sendBufferMap(void)
{
    if (m_partnerList->size() == 0)
    {
        EV << "No destination to send Buffer Map" << endl;
        return;
    }

    MeshBufferMapPacket *bmPkt = new MeshBufferMapPacket("MESH_PEER_BUFFER_MAP");
        bmPkt->setBufferMapArraySize(m_bufferMapSize_chunk);
        bmPkt->setBitLength(m_BufferMapPacketSize_bit);
        m_videoBuffer->fillBufferMapPacket(bmPkt);

/*
    // 2. Get the partner List
    std::vector<IPvXAddress> nList = m_partnerList->getAddressList();

    // 3. Browse through the local NeighborList
    //    . Send the BufferMap Packet to all of the neighbors in the list
    std::vector<IPvXAddress>::iterator iter;
    for (iter = nList.begin(); iter != nList.end(); ++iter)
    {
        // Create copies of the BufferMap packet
        MeshBufferMapPacket *bmPkt_copy = bmPkt->dup();

        // send the packet to this neighbor
        sendToDispatcher(bmPkt_copy, m_localPort, *iter, m_destPort);

        EV << "A buffer map has been sent to " << *iter << endl;
    }
*/

    // 3. Browse through the local NeighborList
    //    & Send the BufferMap Packet to all of the neighbors in the list
    //map<IPvXAddress, NeighborInfo *>::iterator iter;
    map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        // send the packet to this neighbor
        //sendToDispatcher(bmPkt_copy, m_localPort, iter->first, m_destPort);
        sendToDispatcher(bmPkt->dup(), m_localPort, iter->first, m_destPort);

        EV << "A buffer map has been sent to " << iter->first << endl;
    }

    delete bmPkt; bmPkt = NULL;
}

void DonetBase::bindToMeshModule(void)
{
    cModule *temp = getParentModule()->getModuleByRelativePath("partnerList");
    m_partnerList = check_and_cast<PartnerList *>(temp);
    EV << "Binding to PartnerList is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
    EV << "Binding to VideoBuffer is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("forwarder");
    m_forwarder = check_and_cast<Forwarder *>(temp);
    EV << "Binding to Forwarder is completed successfully" << endl;

}

void DonetBase::bindToGlobalModule(void)
{
    // -- Recall the same function at the base class
    CommBase::bindToGlobalModule();

    // -- Some thing new to the function
    cModule *temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingDonet *>(temp);
    EV << "Binding to AppSettingDonet is completed successfully" << endl;

    temp = simulation.getModuleByPath("meshObserver");
    m_meshOverlayObserver = check_and_cast<MeshOverlayObserver *>(temp);
    EV << "Binding to MeshOverlayObserver is completed successfully" << endl;

    temp = simulation.getModuleByPath("logger");
    m_logger = check_and_cast<Logger *>(temp);
    EV << "Binding to Logger is completed successfully" << endl;

}

void DonetBase::getSender(cPacket *pkt, IPvXAddress &senderAddress, int &senderPort)
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        senderAddress   = controlInfo->getSrcAddr();
        senderPort      = controlInfo->getSrcPort();
}

void DonetBase::getSender(cPacket *pkt, IPvXAddress &senderAddress)
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        senderAddress   = controlInfo->getSrcAddr();
}

const IPvXAddress& DonetBase::getSender(const cPacket *pkt) const
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        return controlInfo->getSrcAddr();
}

void DonetBase::processChunkRequest(cPacket *pkt)
{
    EV << "---------- Process chunk request ------------------------------------" << endl;
    // Debug
    ++m_nChunkRequestReceived;

    IPvXAddress senderAddress;
    int senderPort;
    getSender(pkt, senderAddress, senderPort);
    MeshChunkRequestPacket *reqPkt = check_and_cast<MeshChunkRequestPacket *>(pkt);

    EV << "Chunk request received from " << senderAddress << ": " << endl;
    printChunkRequestPacket(reqPkt);

    // -- TODO: Need reply to chunk request here
    // -- Find the id of the requested chunk
    SEQUENCE_NUMBER_T start = reqPkt->getSeqNumMapStart();
    int size = reqPkt->getRequestMapArraySize();
    //for (int offset=0; offset < m_bufferMapSize_chunk; ++offset)
    for (int offset=0; offset < size; ++offset)
    {
        if (reqPkt->getRequestMap(offset) == true)
        {
            SEQUENCE_NUMBER_T seqNum_requestedChunk = offset + start;
            EV << "-- Chunk on request: " << seqNum_requestedChunk << endl;

            // -- Look up to see if the requested chunk is available in the Video Buffer
            if (m_videoBuffer->isInBuffer(seqNum_requestedChunk) ==  true)
            {
                EV << "  -- in buffer" << endl;
                // -- If YES, send the chunk to the requesting peer VIA Forwarder

                m_forwarder->sendChunk(seqNum_requestedChunk, senderAddress, senderPort);

                // Debug
                ++m_nChunkSent;
            }
            else
            {
                EV << "\t\t not in buffer" << endl;
            }
        }
    }
}

MeshPartnershipAcceptPacket *DonetBase::generatePartnershipRequestAcceptPacket()
{
    MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
        acceptPkt->setUpBw(param_upBw);
        acceptPkt->setBitLength(m_appSetting->getPacketSizePartnershipAccept());

    return acceptPkt;
}

MeshPartnershipRejectPacket *DonetBase::generatePartnershipRequestRejectPacket()
{
    MeshPartnershipRejectPacket *rejectPkt = new MeshPartnershipRejectPacket("MESH_PEER_JOIN_REJECT");
        rejectPkt->setBitLength(m_appSetting->getPacketSizePartnershipReject());

    return rejectPkt;
}

void DonetBase::reportStatus()
{
    Partnership p;
        p.address           = getNodeAddress();
        p.arrivalTime       = m_arrivalTime;
        p.joinTime          = m_joinTime;
        p.nPartner          = m_partnerList->getSize();
        p.video_startTime   = m_video_startTime;
        p.head_videoStart   = m_head_videoStart;
        p.begin_videoStart  = m_begin_videoStart;
        p.threshold_videoStart = m_threshold_videoStart;
        p.nChunkRequestReceived = m_nChunkRequestReceived;
        p.nChunkSent = m_nChunkSent;
        p.nBMrecv = m_nBufferMapRecv;
        p.partnerList = m_partnerList->getAddressList();
    m_meshOverlayObserver->writeToFile(p);
}

void DonetBase::printChunkRequestPacket(MeshChunkRequestPacket *reqPkt)
{
    if (ev.isGUI() == false)
        return;

    EV << "-- Start:\t" << reqPkt->getSeqNumMapStart()  << endl;
    EV << "-- End:\t"   << reqPkt->getSeqNumMapEnd()    << endl;
    EV << "-- Head:\t"  << reqPkt->getSeqNumMapHead()   << endl;

    for (int i=0; i < m_bufferMapSize_chunk; ++i)
    {
        EV << reqPkt->getRequestMap(i);
    }
    EV << endl;
}


void DonetBase::processPartnershipRequest(cPacket *pkt)
{
    EV << endl;
    EV << "-------- Process partnership Request --------------------------------" << endl;

    emit(sig_pRequestRecv, 1);

    // -- Get the identifier (IP:port) and upBw of the requester
    PendingPartnershipRequest requester;
    MeshPartnershipRequestPacket *memPkt = check_and_cast<MeshPartnershipRequestPacket *>(pkt);
        getSender(pkt, requester.address, requester.port);
        requester.upBW = memPkt->getUpBw();

    EV << "Requester: " << endl
       << "-- Address:\t\t"         << requester.address << endl
       << "-- Port:\t\t"            << requester.port << endl
       << "-- Upload BW:\t"  << requester.upBW << endl;

    switch(m_state)
    {
    case MESH_JOIN_STATE_ACTIVE:
    {
        considerAcceptPartner(requester);
        EV << "State remains as MESH_JOIN_STATE_ACTIVE" << endl;

        // -- State remains
        // m_state = MESH_JOIN_STATE_ACTIVE; // state remains
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        EV << "I am waiting for a partnership response. Your request will be stored in a queue." << endl;
        if (m_partnerList->size() + 1 < param_maxNOP)
        {
           //considerAcceptPartner(requester);
           EV << "-- Can accept this request" << endl;

           // -- Add peer directly to Partner List
           //m_partnerList->addAddress(requester.address, requester.upBW);
           addPartner(requester.address, requester.upBW);

           // -- Report to Active Peer Table to update the information
           EV << "Increment number of partner " << endl;
           m_apTable->incrementNPartner(getNodeAddress());

           MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
           sendToDispatcher(acceptPkt, m_localPort, requester.address, requester.port);
        }
        else
        {
           //m_list_partnershipRequestingNode.push_back(requester);

           EV << "-- Enough partners --> cannot accept this request." << endl;
           //emit(sig_partnerRequest, 0);

           // -- Create a Partnership message and send it to the remote peer
           MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
           sendToDispatcher(rejectPkt, m_localPort, requester.address, requester.port);

           emit(sig_pRejectSent, 1);
        }

        emit(sig_pRequestRecv_whileWaiting, 1);

        EV << "State remains as MESH_JOIN_STATE_ACTIVE_WAITING" << endl;

        // -- State changes
        m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        throw cException("JOIN_REQUEST is not expected for unjoined (MESH_JOIN_STATE_IDLE) nodes");
        break;
    }
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        throw cException("JOIN_REQUEST is not expected for unjoined (MESH_JOIN_STATE_IDLE_WAITING) nodes");
        break;
    }
    default:
    {
        throw cException("Uncovered state, check assignment of state variable!");
        break;
    }
    } // switch()

}

void DonetBase::considerAcceptPartner(PendingPartnershipRequest requester)
{
    if (canAcceptMorePartner())
    {
        EV << "-- Can accept this request" << endl;
        // -- Debug
        //emit(sig_partnerRequest, m_partnerList->getSize());

        // -- Add peer directly to Partner List
        //m_partnerList->addAddress(requester.address, requester.upBW);
        addPartner(requester.address, requester.upBW);

        EV << "Accepted pRequest from " << requester.address << endl;

        // -- Report to Active Peer Table to update the information
        EV << "Increment number of partner " << endl;
        m_apTable->incrementNPartner(getNodeAddress());

        // -- Store the peer as a candidate
        // m_candidate = requester;

        MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
        sendToDispatcher(acceptPkt, m_localPort, requester.address, requester.port);
    }
    else
    {
        EV << "-- Enough partners --> cannot accept this request." << endl;
        //emit(sig_partnerRequest, 0);

        // -- Create a Partnership message and send it to the remote peer
        MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
        sendToDispatcher(rejectPkt, m_localPort, requester.address, requester.port);

        emit(sig_pRejectSent, 1);
    }

}

/*******************************************************************************
 *
 * Functions handling timers
 *
 *******************************************************************************
 */
void DonetBase::handleTimerReport(void)
{
   m_gstat->writePartnerList2File(getNodeAddress(), m_partnerList->getAddressList());
}

void DonetBase::addPartner(IPvXAddress remote, double upbw)
{
   m_partnerList->addAddress(remote, upbw);
//   m_partnerList->addAddress(remote, upbw, 0);
   m_gstat->writePartnership2File(getNodeAddress(), remote);
}


