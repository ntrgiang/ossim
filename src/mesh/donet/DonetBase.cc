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
// DonetBase.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "DonetBase.h"
#include "DpControlInfo_m.h"
#include "assert.h"

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "@" << simTime().dbl() << "::" << getFullName() << ": "
#endif

bool DonetBase::m_trEnabled = false;

DonetBase::DonetBase() {}

DonetBase::~DonetBase() {}

void DonetBase::initialize()
{
   m_debug = (hasPar("debug")) ? par("debug").boolValue() : false;

   //sig_pRequestSent = registerSignal("Signal_pRequestSent");
   // TODO: (Giang) remove the following signals?
   sig_pRequestRecv = registerSignal("Signal_pRequestRecv");
   sig_pRejectSent = registerSignal("Signal_pRejectSent");
   sig_pRequestRecv_whileWaiting = registerSignal("Signal_pRequestRecv_whileWaiting");
}

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

   return true;

   //   EV << "Current number of partners: " << m_partnerList->getSize() << endl;
   //   EV << "Max number of partners to have: " << param_maxNOP << endl;
   //   if (m_partnerList->getSize() < param_maxNOP)
   //      return true;

   //   return false;
}

void DonetBase::sendBufferMap(void)
{
   //debugOUT << "@Peer " << getNodeAddress() << "::" << endl;

   if (m_partnerList->getSize() <= 0)
   {
      debugOUT << "Peer " << getNodeAddress() << " has no partners to send Buffer Map" << endl;
      return;
   }

   //m_partnerList->print2();
   //debugOUT << "@peer " << getNodeAddress() << endl;

   // -- Debug
   //m_videoBuffer->printRange();

   MeshBufferMapPacket *bmPkt = new MeshBufferMapPacket("MESH_PEER_BUFFER_MAP");
   bmPkt->setBufferMapArraySize(m_bufferMapSize_chunk);
   bmPkt->setBitLength(m_BufferMapPacketSize_bit);
   m_videoBuffer->fillBufferMapPacket(bmPkt);

   // 3. Browse through the local NeighborList
   //    & Send the BufferMap Packet to all of the neighbors in the list
   for (map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
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

   temp = getParentModule()->getModuleByRelativePath("membership");
   m_memManager = check_and_cast<MembershipBase *>(temp);
   EV << "Binding to MembershipManager is completed successfully" << endl;
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

void DonetBase::bindToExternalModule()
{
   Enter_Method("bindToExternalModule()");

   // -- Binding to the Traceroute module
   //   cModule *temp = getParentModule()->getParentModule()->getModuleByRelativePath("networkLayer")->getModuleByRelativePath("traceroute");
   //   m_traceroute = check_and_cast<Traceroute *>(temp);
   //   EV << "Binding to Traceroute is completed successfully" << endl;

}

void DonetBase::bindtoStatisticModule()
{
   Enter_Method("bindToStatisticModule()");

   cModule *temp = simulation.getModuleByPath("globalStatistic");
   m_gstat = check_and_cast<DonetStatistic *>(temp);
   EV << "Binding to globalStatistic is completed successfully" << endl;
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
   Enter_Method("processChunkRequest");

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

MeshPartnershipDisconnectPacket *DonetBase::generatePartnershipDisconnectPacket()
{
   MeshPartnershipDisconnectPacket *disPkt = new MeshPartnershipDisconnectPacket("MESH_PEER_DISCONNECT");
   disPkt->setBitLength(m_appSetting->getPacketSizePartnershipDisconnect());

   return disPkt;
}

void DonetBase::reportStatus()
{
   Partnership p;
   p.address           = getNodeAddress();
   //p.arrivalTime       = m_arrivalTime;
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



//void DonetBase::processPartnerLeave(cPacket *pkt)
//{
//   Enter_Method("processPartnerLeave()");

//   EV << endl << "-------- Process partner leave ---------------" << endl;

//   // -- Get the identifier (IP:port) and upBw of the requester
//   IPvXAddress leaveAddress;
//   int leavePort;
//   //MeshPartnershipLeavePacket *memPkt = check_and_cast<MeshPartnershipLeavePacket *>(pkt);
//      getSender(pkt, leaveAddress, leavePort);

//   EV << "Requester: " << endl
//      << "-- Address:\t\t"         << leaveAddress << endl;

//   EV << "Partner list before removing partner: " << endl;
//   m_partnerList->print();

//   m_partnerList->m_map.erase(m_partnerList->m_map.find(leaveAddress));

//   EV << "Partner list after removing partner: " << endl;
//   m_partnerList->print();
//}

void DonetBase::considerAcceptPartner(PendingPartnershipRequest requester)
{
   if (canAcceptMorePartner()) // always returns true, at the moment
   {
      EV << "-- Can accept this request" << endl;
      // -- Debug
      //emit(sig_partnerRequest, m_partnerList->getSize());

      // -- Add peer directly to Partner List
      addPartner(requester.address, requester.upBW);

      EV << "Accepted pRequest from " << requester.address << endl;

      // -- Report to Active Peer Table to update the information
      EV << "Increment number of partner " << endl;
      //m_apTable->incrementNPartner(getNodeAddress());
      m_memManager->incrementNPartner(getNodeAddress());

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
//void DonetBase::handleTimerReport(void)
//{
//   m_gstat->writePartnerList2File(getNodeAddress(), m_partnerList->getAddressList());
//}

void DonetBase::addPartner(IPvXAddress remote, double upbw)
{
   m_partnerList->addAddress(remote, upbw);
   m_forwarder->addRecord(remote);

   //   m_partnerList->addAddress(remote, upbw, 0);
   m_gstat->writePartnership2File(getNodeAddress(), remote);
}

void DonetBase::processPartnershipLeave(cPacket *pkt)
{
   Enter_Method("processPartnershipLeave()");
   EV << endl << "-------- Process partnership Leave --------------------------------" << endl;

   if (m_state == MESH_JOIN_STATE_IDLE)
   {
      debugOUT << "node " << getNodeAddress() << " is idle --> message should be ignored" << endl;
      return;
   }
   // -- Get the identifier (IP:port) and upBw of the requester
   IPvXAddress leavingAddress = IPvXAddress("10.0.0.9");;
   int senderPort = 0;
   getSender(pkt, leavingAddress, senderPort);
   //MeshChunkRequestPacket *reqPkt = check_and_cast<MeshChunkRequestPacket *>(pkt);

   EV << "Leave request received from " << leavingAddress << endl;

   EV << "Partner list before removing partner: " << endl;
   m_partnerList->print();

   debugOUT << "node " << getNodeAddress() << " received LEAVE msg from " << leavingAddress << endl;
   //debugOUT << "numPartners before: " << m_partnerList->getSize() << endl;

   m_partnerList->deleteAddress(leavingAddress);

   //debugOUT << "numPartners after: " << m_partnerList->getSize() << endl;

   //debugOUT << "Partnerlist after delete partner: " << endl;
   //m_partnerList->print();

   EV << "Partner list after removing partner: " << endl;
   m_partnerList->print();
}

//void DonetBase::sendTraceRouteMsg(IPvXAddress addr)
//{
//   StartTraceroute *startmsg = new StartTraceroute();
//   startmsg->setDest(addr);
//   sendToDispatcher(startmsg);
//   //cSimpleModule::send(startmsg,"to_trcrt");
//}
