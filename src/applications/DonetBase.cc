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
    cDatarateChannel *channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());
    param_upBw = channel->getDatarate();

    // -- problem unresolved!!!
//    channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
//            ->getTransmissionChannel());
//    param_downBw = channel->getDatarate();
}

/*
 * Consider whether having "enough" number of partners
 */
bool DonetBase::canHaveMorePartner(void)
{
    if (m_partnerList->getSize() < param_numberOfPartner)
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

    // Old code, replaced by a function
    MeshBufferMapPacket *bmPkt = new MeshBufferMapPacket("MESH_PEER_BUFFER_MAP");
        bmPkt->setBufferMapArraySize(m_bufferMapSize_chunk);
        bmPkt->setBitLength(m_BufferMapPacketSize_bit);
        m_videoBuffer->fillBufferMapPacket(bmPkt);

    // Not neccessary to have a separate function! The sendBufferMap is simple enough
    //    MeshBufferMapPacket *bmPkt = generateBufferMapPacket();

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

        #if(__DONET_PEER_DEBUG__)
            EV << "A buffer map has been sent to " << *iter << endl;
        #endif
    }
    delete bmPkt;
}

void DonetBase::bindToMeshModule(void)
{
    cModule *temp = getParentModule()->getModuleByRelativePath("partnerList");
    m_partnerList = dynamic_cast<PartnerList *>(temp);
    if (m_partnerList == NULL) throw cException("m_partnerList == NULL is invalid");

    temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
    if (m_videoBuffer == NULL) throw cException("m_videoBuffer == NULL is invalid");

    temp = getParentModule()->getModuleByRelativePath("forwarder");
    m_forwarder = dynamic_cast<Forwarder *>(temp);
    if (m_forwarder == NULL) throw cException("m_forwarder == NULL is invalid");

}

void DonetBase::bindToGlobalModule(void)
{
    // -- Recall the same function at the base class
    CommBase::bindToGlobalModule();

    // -- Some thing new to the function
    cModule *temp = simulation.getModuleByPath("appSetting");
    m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
    if (m_appSetting == NULL) throw cException("NULL pointer to module AppSetting");

    temp = simulation.getModuleByPath("meshObserver");
    m_meshOverlayObserver = dynamic_cast<MeshOverlayObserver *>(temp);
    if (m_meshOverlayObserver == NULL) throw cException("NULL pointer to module MeshOverlayObserver");

    temp = simulation.getModuleByPath("logger");
    m_logger = dynamic_cast<Logger *>(temp);
    if (m_logger == NULL) throw cException("NULL pointer to module Logger");

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
    IPvXAddress senderAddress;
    int senderPort;
    getSender(pkt, senderAddress, senderPort);
    // EV << "A chunk request packet has just been received from " << sourceAddress << " ! prepare to print packet's content" << endl;
    // EV << "----------------------------------------------------------" << endl;
    MeshChunkRequestPacket *reqPkt = check_and_cast<MeshChunkRequestPacket *>(pkt);

    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
        EV << "Chunk request received from " << senderAddress << ": " << endl;
        EV << "Start: " << reqPkt->getSeqNumMapStart()
            << " -- End: " << reqPkt->getSeqNumMapEnd()
            << " -- Head: " << reqPkt->getSeqNumMapHead()
            << " -- ";
        for (int i=0; i < m_bufferMapSize_chunk; ++i)
        {
            EV << reqPkt->getRequestMap(i);
        }
        EV << endl;
    #endif
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

    // -- TODO: Need reply to chunk request here
    // -- Find the id of the requested chunk
    SEQUENCE_NUMBER_T start = reqPkt->getSeqNumMapStart();
    for (int offset=0; offset < m_bufferMapSize_chunk; ++offset)
    {
        if (reqPkt->getRequestMap(offset) == true)
        {
            SEQUENCE_NUMBER_T seqNum_requestedChunk = offset + start;

            // -- Look up to see if the requested chunk is available in the Video Buffer
            if (m_videoBuffer->isInBuffer(seqNum_requestedChunk) ==  true)
            {
                // -- If YES, duplicate the chunk and send to the requesting peer

                //MeshVideoChunkPacket *copyPkt = m_videoBuffer->getChunk(seqNum_requestedChunk)->dup();
                m_forwarder->sendChunk(seqNum_requestedChunk, senderAddress, senderPort);

                //sendToDispatcher(copyPkt, m_localPort, senderAddress, m_destPort);
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
