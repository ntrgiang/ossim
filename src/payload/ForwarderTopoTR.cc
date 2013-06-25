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
// ForwarderTopoTR.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "ForwarderTopoTR.h"
#include "DpControlInfo_m.h"
//#include "DonetPeer.h"

Define_Module(ForwarderTopoTR)

long ForwarderTopoTR::m_observedChunk;
int ForwarderTopoTR::m_topoSequence;
//std::vector<long> m_observedChunkList;

ForwarderTopoTR::ForwarderTopoTR() {}

ForwarderTopoTR::~ForwarderTopoTR() {}

void ForwarderTopoTR::initialize(int stage)
{
   Forwarder::initialize(stage);

   if (stage != 3)
      return;

   cModule *temp = simulation.getModuleByPath("topoObserver");
   m_topoObserver = check_and_cast<OverlayTopology*>(temp);
   assert(m_topoObserver != NULL);

   // -- Binding to Traceroute module
   //
   temp = getParentModule()->getParentModule()->getModuleByRelativePath("networkLayer")->getModuleByRelativePath("traceroute");
   m_traceroute = check_and_cast<Traceroute *>(temp);
   EV << "Binding to Traceroute is completed successfully" << endl;

   m_observedChunk = m_topoObserver->getObservedChunk();
   m_topoSequence = m_observedChunk % INT_MAX;

   WATCH(m_observedChunk);
   WATCH(m_topoSequence);
   WATCH(m_topoObserver);
}

void ForwarderTopoTR::finish()
{
}

void ForwarderTopoTR::handleMessage(cMessage* msg)
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

   std::cout << "received a packet to the Forwarder" << endl;

   IPvXAddress senderAddress;
   DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(msg->getControlInfo());
   //   senderAddress = controlInfo->getSrcAddr();

   PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(msg);
   EV << "PacketGroup = " << appMsg->getPacketGroup() << endl;
   if (appMsg->getPacketGroup() != PACKET_GROUP_VIDEO_CHUNK)
      throw cException("Wrong packet type!");

   //   EV << "A video chunk has just been received from " << senderAddress << endl;
   VideoChunkPacket *chunkPkt = check_and_cast<VideoChunkPacket *>(appMsg);

   // -- Local record of incoming chunks
   updateReceivedChunkRecord(senderAddress);
   ++m_count_totalChunk_incoming;

   // -- Report to the topology observer
   //
   long seq = chunkPkt->getSeqNumber();
   //EV << "current chunk: " << chunkSeqNum << " -- expected one: " << m_observedChunk << endl;

   m_topoObserver->addNode(controlInfo->getDestAddr(), seq);
   m_topoObserver->addEdge(seq, controlInfo->getSrcAddr(), controlInfo->getDestAddr());
   m_videoBuffer->insertPacket(chunkPkt);
}

void ForwarderTopoTR::sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort)
{
   Enter_Method("sendChunk()");

   if(isSource())
   {
      std::cout << "Source node: setRoot is called" << endl;
      m_topoObserver->setRoot(getNodeAddress(), seq);
   }

   // -- Start Trace route
   //
   m_traceroute->triggerSendTracerouteMessage(destAddress);

   Forwarder::sendChunk(seq, destAddress, destPort);
}


bool ForwarderTopoTR::isSource() {

   if(this->getFullPath().find("sourceNode") ) {
      return true;
   } else {
      return false;
   }
}


