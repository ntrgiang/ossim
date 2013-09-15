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
// DonetSource.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


//#include "IPv4InterfaceData.h"
//#include "InterfaceTableAccess.h"
//#include "MeshPeerStreamingPacket_m.h"

#include "DonetSource.h"
#include "DpControlInfo_m.h"

Define_Module(DonetSource)

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "::" << getFullName() << " @ " << simTime().dbl() << ": "
#endif

DonetSource::DonetSource() {}

DonetSource::~DonetSource()
{
   if (timer_sendBufferMap  != NULL)
   {
      delete cancelEvent(timer_sendBufferMap);    timer_sendBufferMap = NULL;
   }

   if (timer_reportStatistic != NULL)
   {
      delete cancelEvent(timer_reportStatistic);
   }
}

void DonetSource::initialize(int stage)
{
   if (stage == 0)
   {
      DonetBase::initialize();
   }

   if (stage != 3)
      return;

   bindToMeshModule();
   bindToGlobalModule();
   bindtoStatisticModule();

   getAppSetting();
   readChannelRate();

   findNodeAddress();

   param_maxNOP = par("maxNOP");
   param_interval_partnerlistCleanup = par("interval_partnerlistCleanup");
   param_threshold_idleDuration_buffermap = par("threshold_idleDuration_buffermap");
   param_interval_reportStatistic = par("interval_reportStatistic");

   timer_sendBufferMap     = new cMessage("MESH_SOURCE_TIMER_SEND_BUFFERMAP");
   timer_reportStatistic   = new cMessage("MESH_SOURCE_TIMER_REPORT_STATISTIC");
   //    timer_sendReport    = new cMessage("MESH_SOURCE_TIMER_SEND_REPORT");

   // -- Register itself to the Active Peer Table
   //m_apTable->addSourceAddress(getNodeAddress(), param_maxNOP);
   //m_apTable->addSourceAddress(getNodeAddress());
   m_apTable->addAddress(getNodeAddress());
   m_memManager->addSourceAddress(getNodeAddress(), param_maxNOP);

   // -------------------------------------------------------------------------
   // -------------------------------- Timers ---------------------------------
   // -------------------------------------------------------------------------
   // -- Schedule events
   scheduleAt(simTime() + uniform(0.0, 1.0), timer_sendBufferMap);
   scheduleAt(simTime() + param_interval_reportStatistic, timer_reportStatistic);

   // -- Report Logged Statistic to global module
   // scheduleAt(getSimTimeLimit() - uniform(0.05, 0.95), timer_sendReport);

   // -- States
   m_state = MESH_JOIN_STATE_ACTIVE;

   //sig_pRequestRecv = registerSignal("Signal_pRequestRecv");
   sig_pRejectSent = registerSignal("Signal_pRejectSent");

   sig_pRequestRecv_whileWaiting = registerSignal("Signal_pRequestRecv_whileWaiting");

   // --- For logging variables
   //    m_arrivalTime = -1.0;
   m_joinTime = -1.0;
   m_video_startTime = -1.0;
   m_head_videoStart = -1L;
   m_begin_videoStart = -1L;
   m_threshold_videoStart = m_bufferMapSize_chunk/2;
   m_nChunkRequestReceived = 0L;
   m_nChunkSent = 0L;

   sig_nPartner = registerSignal("Signal_nPartner");

   // -------------------------------------------------------------------------
   // -------------------------------- WATCH ----------------------------------
   // -------------------------------------------------------------------------
   WATCH(m_localAddress);
   WATCH(m_localPort);
   WATCH(m_destPort);

   WATCH(param_interval_bufferMap);
   WATCH(param_interval_reportStatistic);
   WATCH(param_threshold_idleDuration_buffermap);
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

   m_gstat->reportNumberOfPartner(m_partnerList->getSize());

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

   debugOUT << "Peer " << getNodeAddress() << " has " << m_partnerList->getSize() << " partners" << endl;
   m_partnerList->print2();

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
      scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);

      m_videoBuffer->printStatus();
      sendBufferMap();
      failureDetection();

      // -- Doing some statistical reporting stuff
      emit(sig_nPartner, m_partnerList->getSize());

   }
   else if (msg == timer_reportStatistic)
   {
      scheduleAt(simTime() + param_interval_reportStatistic, timer_reportStatistic);
      handleTimerReportStatistic();
   }
}

void DonetSource::handleTimerPartnerlistCleanup()
{
   Enter_Method("handleTimerPartnerlistCleanup()");

   if (m_partnerList->getSize() == 0)
   {
      EV << "Empty partner list" << endl;
      return;
   }

   std::vector<IPvXAddress> removeAddressList;

   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      //IPvXAddress address = iter->first;

      double timeDiff = simTime().dbl() - iter->second.getLastRecvBmTime();
      EV << "Partner to be examined " << iter->first << " with time diff " << timeDiff << endl;

      if (timeDiff > param_threshold_idleDuration_buffermap)
      {
         removeAddressList.push_back(iter->first);

         EV << "Partner " << iter->first << " was moved to remove_list" << endl;
      }
   }

   EV << "Number of partners to be removed " << removeAddressList.size() << endl;

   // Delete the list of addresses
   for (std::vector<IPvXAddress>::iterator iter = removeAddressList.begin();
        iter != removeAddressList.end(); ++iter)
   {
      m_partnerList->deleteAddress(*iter);
   }

   EV << "Current size of the partnerlist of the source: " << m_partnerList->getSize() << endl;

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
   case MESH_CHUNK_REQUEST:
   {
      processChunkRequest(pkt);
      break;
   }
   case MESH_BUFFER_MAP:
   {
      // Does NOTHING! Video Source does not process Buffer Map
      processPeerBufferMap(pkt);
      break;
   }
   case MESH_PARTNERSHIP_LEAVE:
   {
      processPartnershipLeave(pkt);
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

void DonetSource::processPeerBufferMap(cPacket *pkt)
{
   Enter_Method("processRecvBufferMap()");

   IPvXAddress senderAddress = getSender(pkt);
   EV << "-- Received a buffer map from " << senderAddress << endl;

   if (m_partnerList->hasAddress(senderAddress) == false)
   {
      EV << "-- Buffer Map is received from a non-partner peer!" << endl;
      return;
   }


   NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);

   // -- Cast to the BufferMap packet
   //   MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

   //    EV << "-- Buffer Map information: " << endl;
   //    EV << "  -- Start:\t"   << bmPkt->getBmStartSeqNum()    << endl;
   //    EV << "  -- End:\t"     << bmPkt->getBmEndSeqNum()      << endl;
   //    EV << "  -- Head:\t"    << bmPkt->getHeadSeqNum()       << endl;

   // -- Copy the BufferMap content to the current record
   //    nbr_info->copyFrom(bmPkt);

   // -- Update the timestamp of the received BufferMap
   nbr_info->setLastRecvBmTime(simTime().dbl());
   EV << "Time stammpt of the received buffer map " << nbr_info->getLastRecvBmTime() << endl;

   // -- Update the range of the scheduling window
   //    m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
   //            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;
   //    EV << "-- Head for the scheduling window: " << m_seqNum_schedWinHead << endl;

   // -- Debug
   //    ++m_nBufferMapRecv;
   //    nbr_info->printRecvBm();
   // m_partnerList->printRecvBm(senderAddress);
}

void DonetSource::processPartnershipRequest(cPacket *pkt)
{
   EV << endl;
   EV << "-------- Process partnership Request --------------------------------" << endl;

   //emit(sig_pRequestRecv, 1);

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
      if (m_partnerList->getSize() < param_maxNOP)
      {
         debugOUT << "list size: " << m_partnerList->getSize() << " -- max: " << param_maxNOP << endl;

         // -- Add peer directly to Partner List
         addPartner(requester.address, requester.upBW);

         // -- Report to Active Peer Table to update the information
         EV << "Increment number of partner " << endl;
         m_memManager->incrementNPartner(getNodeAddress());

         MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
         sendToDispatcher(acceptPkt, m_localPort, requester.address, requester.port);
      }
      else
      {
         // -- Create a Partnership message and send it to the remote peer
         MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
         sendToDispatcher(rejectPkt, m_localPort, requester.address, requester.port);
      }
      break;
   }
   default:
   {
      throw cException("The source node should never in a state different from ACTIVE!");
      break;
   }
   } // switch()
}

void DonetSource::handleTimerReportStatistic()
{
   // -- Number of partners
   emit(sig_nPartner, m_partnerList->getSize());

   debugOUT << "Source node has: " << m_partnerList->getSize() << " partners" << endl;
}
