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
// Forwarder.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "Forwarder.h"
#include "DpControlInfo_m.h"

Define_Module(Forwarder)

void RecordCountChunk::print()
{
   EV << "Record of count chunk: " << endl;
   EV << "\t m_chunkSent = " << m_chunkSent << endl;
   EV << "\t m_chunkReceived = " << m_chunkReceived << endl;
   EV << "\t m_prev_chunkSent = " << m_prev_chunkSent << endl;
   EV << "\t m_prev_chunkReceived = " << m_prev_chunkReceived << endl;
   EV << "\t m_oriTime = " << m_oriTime << endl;
}

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

    // -------------------------------------------------------------------------
    // -- For incoming chunks
    // -------------------------------------------------------------------------

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

    // -- Local record of incoming chunks
    updateReceivedChunkRecord(senderAddress);
    ++m_count_totalChunk_incoming;
}

void Forwarder::initialize(int stage)
{
    if (stage != 3)
        return;

    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);

    m_count_totalChunk_incoming = 0L;
}

void Forwarder::finish()
{

}

void Forwarder::sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort)
{
    Enter_Method("sendChunk");

    VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(seq)->dup();

    sendToDispatcher(chunkPkt, getLocalPort(), destAddress, destPort);

    //updateSentChunkRecord(destAddress);

}

void Forwarder::updateSentChunkRecord(IPvXAddress& destAddress)
{
    // -- Update the record about sent Chunk
    std::map<IPvXAddress, RecordCountChunk>::iterator iter;
    iter = m_record_countChunk.find(destAddress);

    if (iter == m_record_countChunk.end()) // New entry
    {
       addRecord(destAddress);
    }
    else
    {
       iter->second.m_chunkSent += 1;
    }
}

void Forwarder::updateReceivedChunkRecord(IPvXAddress& senderAddress)
{
    // -- Update the record about received Chunk
    std::map<IPvXAddress, RecordCountChunk>::iterator iter;
    iter = m_record_countChunk.find(senderAddress);

    if (iter == m_record_countChunk.end()) // New entry
    {
       addRecord(senderAddress);
    }
    else
    {
       iter->second.m_chunkReceived += 1;
    }
}

void Forwarder::getRecordChunk(IPvXAddress &addr, RecordCountChunk& record)
{
   std::map<IPvXAddress, RecordCountChunk>::iterator iter;
   iter = m_record_countChunk.find(addr);

   if (iter == m_record_countChunk.end())
   {
      addRecord(addr);
      record = m_record_countChunk[addr];
   }
   else
   {
      record = iter->second;
   }
}

const RecordCountChunk & Forwarder::getRecordChunk(IPvXAddress &addr)
{
   std::map<IPvXAddress, RecordCountChunk>::iterator iter = m_record_countChunk.find(addr);

   if (iter == m_record_countChunk.end())
   {
      // -- Record for such address does not exist
      addRecord(addr);
      return m_record_countChunk[addr];
   }
   else
   {
      return iter->second;
   }
}


void Forwarder::removeRecord(const IPvXAddress &address)
{
   std::map<IPvXAddress, RecordCountChunk>::iterator iter;
   iter = m_record_countChunk.find(address);

   if (iter == m_record_countChunk.end())
   {
      EV << "No element with address " << address << " found!" << endl;
      return;
   }

   m_record_countChunk.erase(iter);
}

void Forwarder::addRecord(const IPvXAddress & address)
{
   RecordCountChunk newRecord;
   newRecord.m_chunkSent = 0L;
   newRecord.m_chunkReceived = 0L;
   newRecord.m_prev_chunkSent = 0L;
   newRecord.m_prev_chunkReceived = 0L;
   newRecord.m_oriTime = simTime().dbl();

   m_record_countChunk.insert(std::pair<IPvXAddress, RecordCountChunk>(address, newRecord));
}

