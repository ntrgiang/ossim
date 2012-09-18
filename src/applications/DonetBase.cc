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

void DonetBase::addToNeighborList(cPacket *pkt)
{
    Enter_Method("addToNeighborList(pkt)");

    // 1. Get the IP-address of the peer
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress reqPeerAddress = controlInfo->getSrcAddr();

    // 2. Store the address into its neighbor list
    m_partnerList->addAddress(reqPeerAddress);
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
    map<IPvXAddress, NeighborInfo *>::iterator iter;
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

void DonetBase::acceptJoinRequest(IPvXAddress &reqPeerAddress, double upBw_remotePeer)
{
    // 1. Store the address into its neighbor list
    // m_partnerList->addAddress(reqPeerAddress);
    m_partnerList->addAddress(reqPeerAddress, upBw_remotePeer);

    // Debug:
    m_partnerList->print();

    // 2. Send the explicit ACCEPT request
//    MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
//        acceptPkt->setUpBw(param_upBw);

    MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();

    // 3. Send the packet
    sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);

    // 3. Send the explicit ACCEPT request
//    MeshPartnershipPacket *acceptPkt = new MeshPartnershipPacket("MESH_PEER_JOIN_ACCEPT");
//        acceptPkt->setPacketType(MESH_PARTNERSHIP);
        // !!! has problem
//        acceptPkt->setCommand(CMD_MESH_PARTNERSHIP_ACCEPT);
//        acceptPkt->setUpBw(param_upBw);

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


/*
MeshBufferMapPacket *DonetBase::generateBufferMapPacket()
{
    MeshBufferMapPacket *bmPkt = new MeshBufferMapPacket("MESH_PEER_BUFFER_MAP");
        bmPkt->setBufferMapArraySize(m_bufferMapSize_chunk);
        //bmPkt->setBitLength(m_appSetting->getPacketSizeBufferMap() * 8);    // Convert sizes from Bytes --> bits
        bmPkt->setBitLength(m_BufferMapPacketSize_bit);
        m_videoBuffer->fillBufferMapPacket(bmPkt);

    return bmPkt;
}
*/

/*
MeshBufferMapPacket *DonetBase::generateBufferMapPacket()
{
    MeshBufferMapPacket *bmPacket = new MeshBufferMapPacket("MESH_BUFFER_MAP_PACKET");
        bmPacket->setBitLength(m_BufferMapPacketSize_bit);
        bmPacket->setBufferMapArraySize(m_bufferMapSize_chunk);

    return bmPacket;
}*/
