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
// ChunkGenerator.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "ChunkGenerator.h"
#include <iomanip> // setw()

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "@ " << setprecision(6) << simTime().dbl() << getFullName() << ":: "
#endif

Define_Module(ChunkGenerator)

ChunkGenerator::ChunkGenerator() {
   // TODO Auto-generated constructor stub
}

ChunkGenerator::~ChunkGenerator() {
   // TODO Auto-generated destructor stub
}

//simsignal_t ChunkGenerator::sig_chunkSeqNum;
//simsignal_t ChunkGenerator::chunkSeqNumSignal;

void ChunkGenerator::initialize(int stage)
{
   if (stage == 0)
   {
      m_debug = (hasPar("debug")) ? par("debug").boolValue() : false;

      sig_chunkSeqNum = registerSignal("chunkSeqNum");
      return;
   }

   if (stage != 3)
      return;

   // -- pointing to the Video Buffer
   cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
   m_videoBuffer = check_and_cast<VideoBuffer *>(temp);

   m_id_newChunk = 0L;
   timer_newChunk      = new cMessage("MESH_SOURCE_TIMER_NEW_CHUNK");

   temp = simulation.getModuleByPath("appSetting");
   m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
   if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");

   m_interval_newChunk = m_appSetting->getIntervalNewChunk();
   m_size_chunkPacket  = m_appSetting->getPacketSizeVideoChunk();

   // -- Schedule the first event for the first chunk
   scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newChunk);

   WATCH(m_appSetting);
   WATCH(m_videoBuffer);
   WATCH(m_size_chunkPacket);
   WATCH(m_interval_newChunk);
}

void ChunkGenerator::finish()
{
   if (timer_newChunk) delete cancelEvent(timer_newChunk);
}

void ChunkGenerator::handleMessage(cMessage *msg)
{
   if (!msg->isSelfMessage())
   {
      throw cException("This module does NOT process external messages!");
      return;
   }

   handleTimerMessage(msg);
}

void ChunkGenerator::handleTimerMessage(cMessage *msg)
{
   if (msg != timer_newChunk) throw cException("Wrong timer!");

   scheduleAt(simTime() + m_interval_newChunk, timer_newChunk);

   // -- Signal generation
   m_tsValue.set(simTime(), m_id_newChunk);
   emit(sig_chunkSeqNum, &m_tsValue);
   //        emit(sig_chunkSeqNum, m_id_newChunk);

   VideoChunkPacket *chunk = new VideoChunkPacket("VIDEO_CHUNK_PACKET");
      chunk->setSeqNumber(m_id_newChunk);
      chunk->setOriginalTimeStamp(simTime().dbl());           // Set the time-stamp of the chunk to current time
      chunk->setOverlayHopCount(0);                          // Initialize the hopCount variable
      chunk->setBitLength(m_size_chunkPacket * 8);    // convert the chunk size from Bytes --> bits

   m_videoBuffer->insertPacketDirect(chunk);
   //        m_videoBuffer->printStatus();

   debugOUT << "Chunk " << m_id_newChunk << " was inserted into the Buffer" << endl;

   ++m_id_newChunk;
}
