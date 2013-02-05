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

#include "Forwarder.h"
#include "DpControlInfo_m.h"

Define_Module(Forwarder);

Forwarder::Forwarder() {}

Forwarder::~Forwarder() {}

void Forwarder::handleMessage(cMessage* msg)
{
    Enter_Method("handleMessage");

    if (msg->isSelfMessage())
    {
        throw cException("No timer is used in this module");
        return;
    }

    IPvXAddress senderAddress;
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(msg->getControlInfo());
    senderAddress = controlInfo->getSrcAddr();

    PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(msg);
    EV << "PacketGroup = " << appMsg->getPacketGroup() << endl;
    if (appMsg->getPacketGroup() != PACKET_GROUP_VIDEO_CHUNK)
        throw cException("Wrong packet type!");

    EV << "A video chunk has just been received from " << senderAddress << endl;
    VideoChunkPacket *chunkPkt = check_and_cast<VideoChunkPacket *>(appMsg);

    m_videoBuffer->insertPacket(chunkPkt);

    updateReceivedChunkRecord(senderAddress);
}

void Forwarder::initialize(int stage)
{
    if (stage != 3)
        return;

    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
}

void Forwarder::finish()
{

}

void Forwarder::sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort)
{
    Enter_Method("sendChunk");

    VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(seq)->dup();

    sendToDispatcher(chunkPkt, getLocalPort(), destAddress, destPort);

    updateSentChunkRecord(destAddress);

}

void Forwarder::updateSentChunkRecord(IPvXAddress& destAddress)
{
    // -- Update the record about sent Chunk
    std::map<IPvXAddress, RecordCountChunk>::iterator iter;
    iter = m_record_countChunk.find(destAddress);

    if (iter == m_record_countChunk.end()) // New entry
    {
       RecordCountChunk record;
       record.m_chunkReceived = 0L;
       record.m_chunkSent = 1L;
       record.m_oriTime = simTime().dbl();
       m_record_countChunk.insert(std::pair<IPvXAddress, RecordCountChunk>(destAddress, record));
    }
    else
    {
       iter->second.m_chunkSent += 1;
    }
}

void Forwarder::updateReceivedChunkRecord(IPvXAddress& senderAddress)
{
    // -- Update the record about received Chunk
    //std::map<IPvXAddress, long int>::iterator iter;
    std::map<IPvXAddress, RecordCountChunk>::iterator iter;
    iter = m_record_countChunk.find(senderAddress);

    if (iter == m_record_countChunk.end()) // New entry
    {
       RecordCountChunk record;
       record.m_chunkReceived = 1L;
       record.m_chunkSent = 0L;
       record.m_oriTime = simTime().dbl();
       m_record_countChunk.insert(std::pair<IPvXAddress, RecordCountChunk>(senderAddress, record));
    }
    else
    {
       iter->second.m_chunkReceived += 1;
    }
}

void Forwarder::getRecordChunk(IPvXAddress& addr, long int &nChunkReceived, long int &nChunkSent)
{
   std::map<IPvXAddress, RecordCountChunk>::iterator iter;
   iter = m_record_countChunk.find(addr);

   if (iter == m_record_countChunk.end())
   {
      nChunkReceived = -1L;
      nChunkSent = -1L;
   }
   else
   {
      nChunkReceived = iter->second.m_chunkReceived;
      nChunkSent = iter->second.m_chunkSent;
   }

}

void Forwarder::getRecordChunk(IPvXAddress &addr, RecordCountChunk& record)
{
   std::map<IPvXAddress, RecordCountChunk>::iterator iter;
   iter = m_record_countChunk.find(addr);

   if (iter == m_record_countChunk.end())
   {
      record.m_chunkReceived = -1L;
      record.m_chunkSent = -1L;
      record.m_oriTime = -1L;
   }
   else
   {
      record.m_chunkReceived = iter->second.m_chunkReceived;
      record.m_chunkSent = iter->second.m_chunkSent;
      record.m_oriTime = iter->second.m_oriTime;
   }
}

