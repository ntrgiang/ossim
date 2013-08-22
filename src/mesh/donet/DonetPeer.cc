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
// DonetPeer.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include <set>       // std::set
#include <utility>   // std::pair
#include "IPvXAddress.h"

typedef std::pair<IPvXAddress, double> Ip_Throughput;
typedef std::set<Ip_Throughput> Throughput_Set;

namespace std
{
   template<>
   bool operator< (const Ip_Throughput& l, const Ip_Throughput& r)
   {
      return l.second > r.second;
   }
}

#include "DonetPeer.h"
//#include "TracerouteHandler.h"
#include "DpControlInfo_m.h"
#include <algorithm>
#include <csimulation.h>
#include <assert.h>
#include <iomanip> // setw()

//using namespace std;

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "@" << setprecision(12) << simTime().dbl() << " @Peer " << getNodeAddress() << "::" << getFullName() << ": "
#endif

// ------------------------ Static members -------------------------------------
double DonetPeer::param_interval_reportActive = 0.0;

Define_Module(DonetPeer)

DonetPeer::DonetPeer() {}
DonetPeer::~DonetPeer()
{
   cancelAndDeleteAllTimer();
}

void DonetPeer::initialize(int stage)
{
   if (stage == 0)
   {
      DonetBase::initialize();

      sig_chunkRequestSeqNum  = registerSignal("Signal_chunkRequest");
      //sig_partnerRequest      = registerSignal("Signal_PartnerRequest");

      flag_rangeUpdated = false;

      // -- Reset all range
      m_minStart = -1L;
      m_maxStart = -1L;
      m_minHead = -1L;
      m_maxHead = -1L;

      //        // -- signals for ranges & current playback point
      //        sig_minStart = registerSignal("Signal_MinStart");
      //        sig_maxStart = registerSignal("Signal_MaxStart");
      //        sig_minHead  = registerSignal("Signal_MinHead");
      //        sig_maxHead  = registerSignal("Signal_MaxHead");
      //        sig_currentPlaybackPoint = registerSignal("Signal_CurrentPlaybackPoint");

      sig_e2eDelay = registerSignal("Signal_EndToEndDelay");
      sig_overlayHopCount = registerSignal("Signal_OverlayHopCount");

      return;
   }
   if (stage != 3)
      return;

   bindToMeshModule();
   bindToGlobalModule();
   bindToExternalModule();
   bindtoStatisticModule();

   getAppSetting();
   readChannelRate();

   findNodeAddress();

   // -------------------------------------------------------------------------
   // --------------------------- State variables------------------------------
   // -------------------------------------------------------------------------
   //    m_state_joined = false;
   m_state = MESH_JOIN_STATE_IDLE;
   m_scheduling_started = false;
   // -- Join status
   //    m_joinState = MESH_STATE_JOIN_IDLE;

   // -------------------------------------------------------------------------
   // -------------------------------- Timers ---------------------------------
   // -------------------------------------------------------------------------
   // -- One-time timers
   timer_getJoinTime       = new cMessage("MESH_PEER_TIMER_GET_JOIN_TIME");
   timer_join              = new cMessage("MESH_PEER_TIMER_JOIN");
   timer_leave             = new cMessage("MESH_PEER_TIMER_LEAVE");

   // -- Repeated timers
   timer_chunkScheduling   = new cMessage("MESH_PEER_TIMER_CHUNK_SCHEDULING");
   timer_sendBufferMap     = new cMessage("MESH_PEER_TIMER_SEND_BUFFERMAP");
   timer_findMorePartner   = new cMessage("MESH_PEER_TIMER_FIND_MORE_PARTNER");
   timer_startPlayer       = new cMessage("MESH_PEER_TIMER_START_PLAYER");
   //timer_sendReport        = new cMessage("MESH_PEER_TIMER_SEND_REPORT");
   timer_reportStatistic   = new cMessage("MESH_PEER_TIMER_REPORT_STATISTIC");
   timer_reportActive      = new cMessage("MESH_PEER_TIMER_REPORT_ACTIVE");

   //    timer_rejoin            = new cMessage("MESH_PEER_TIMER_REJOIN");
   timer_partnershipRefinement = new cMessage("MESH_PEER_TIMER_PARTNERSHIP_REFINEMENT");
   timer_checkVideoBuffer = new cMessage("MESH_PEER_TIMER_CHECK_VIDEOBUFFER");

   // Parameters of the module itself
   // param_bufferMapInterval             = par("bufferMapInterval");
   // param_chunkSchedulingInterval       = par("chunkSchedulingInterval");

   m_pRefinementEnabled                  = par("pRefinementEnabled");
   param_interval_chunkScheduling        = par("interval_chunkScheduling");
   param_interval_findMorePartner        = par("interval_findMorePartner");
   param_interval_timeout_joinReqAck     = par("interval_timeout_joinReqAck");
   param_interval_starPlayer             = par("interval_startPlayer");
   param_interval_partnershipRefinement  = par("interval_partnershipRefinement");
   //param_interval_partnerlistCleanup     = par("interval_partnerlistCleanup");
   param_interval_reportStatistic        = par("interval_reportStatistic");

   if (!param_interval_reportActive)
      param_interval_reportActive = par("interval_reportActive");

   param_threshold_idleDuration_buffermap = par("threshold_idleDuration_buffermap");

   param_nNeighbor_SchedulingStart     = par("nNeighbor_SchedulingStart");
   param_waitingTime_SchedulingStart   = par("waitingTime_SchedulingStart");

   //param_requestFactor_moderate        = par("requestFactor_moderate");
   //param_requestFactor_aggresive       = par("requestFactor_aggressive");
   param_maxNOP                        = par("maxNOP");
   param_offsetNOP                     = par("offsetNOP");
   //param_threshold_scarity             = par("threshold_scarity");

   param_minNOP = param_maxNOP - param_offsetNOP;
   param_factor_requestList            = par("factor_requestList").doubleValue();

   m_downloadRate_chunk = (int)((param_downBw / (param_chunkSize * 8)) * param_interval_chunkScheduling);

   param_interval_waitingPartnershipResponse   = par("interval_waitingPartnershipResponse").doubleValue();

   scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

   scheduleAt(simTime() + param_interval_reportStatistic, timer_reportStatistic);
   scheduleAt(simTime() + param_interval_reportActive, timer_reportActive);

   // m_nChunk_perSchedulingInterval = param_interval_chunkScheduling * param_downBw / param_chunkSize / 8;

   m_nChunk_toRequest_perCycle = (int)(m_appSetting->getVideoStreamChunkRate() \
                                       * param_interval_chunkScheduling);
   //* param_baseValue_requestGreedyFactor);
   //    m_bmPacket = generateBufferMapPacket();

   // -- Schedule for writing to log file
   //scheduleAt(getSimTimeLimit() - uniform(0.05, 0.95), timer_sendReport);

   // --------- Debug ------------
   m_joinTime = -1.0;
   m_count_rejoin = 0;
   // -- Init for activity logging:
   //    string path = "..//results//";
   //    string filename = path + getNodeAddress().str();
   //    m_activityLog.open(filename.c_str(), fstream::out);

   //    WATCH(filename.c_str());

   // -- State variable for join process
   m_nRequestSent = m_nRejectSent = 0;

   // -- For scheduling
   m_seqNum_schedWinHead = 0L;
   m_sched_window_moved = false;

   m_prevNChunkReceived = 0L;

   // -------------------------------------------------------------------------
   // -- Signals
   // -------------------------------------------------------------------------
   sig_nJoin               = registerSignal("Signal_Join");

   sig_newchunkForRequest    = registerSignal("Signal_nNewChunkForRequestPerCycle");
   sig_nChunkRequested       = registerSignal("Signal_nChunkRequested");

   // -- Unimportant signals:
   signal_nPartner         = registerSignal("Signal_nPartner");

   sig_nPartner            = registerSignal("Signal_nPartner");
   sig_joinTime            = registerSignal("Signal_joinTime");
   //    sig_playerStartTime     = registerSignal("Signal_playerStartTime");

   sig_pRequestSent        = registerSignal("Signal_pRequestSent");
   sig_pRejectRecv     = registerSignal("Signal_pRejectReceived");

   // Number of requests SENT & RECV
   //    sig_pRequestSent = registerSignal("Signal_pRequestSent");
   sig_pRequestRecv = registerSignal("Signal_pRequestRecv");

   // Number of rejects SENT & RECV
   //    sig_pRejectRecv = registerSignal("Signal_pRejectRecv");
   //    sig_pRejectSent = registerSignal("Signal_pRejectSent");

   //    sig_pRequestRecv_whileWaiting = registerSignal("Signal_pRequestRecv_whileWaiting");

   //    sig_timeout = registerSignal("Signal_timeout");

   // -- signals for ranges & current playback point
   sig_minStart = registerSignal("Signal_MinStart");
   sig_maxStart = registerSignal("Signal_MaxStart");
   sig_minHead  = registerSignal("Signal_MinHead");
   sig_maxHead  = registerSignal("Signal_MaxHead");
   sig_currentPlaybackPoint = registerSignal("Signal_CurrentPlaybackPoint");
   sig_bufferStart = registerSignal("Signal_BufferStart");
   sig_bufferHead = registerSignal("Signal_BufferHead");

   sig_schedWin_start  = registerSignal("Signal_SchedWin_start");
   sig_schedWin_end    = registerSignal("Signal_SchedWin_end");

   sig_localCI = registerSignal("Signal_LocalCI");

   sig_inThroughput = registerSignal("Signal_InThroughput");

   sig_nBufferMapReceived = registerSignal("Signal_nBufferMapReceived");

   // -- Debugging variables
   //    m_arrivalTime = -1.0;
   m_joinTime = -1.0;
   m_video_startTime = -1.0;
   m_head_videoStart = -1L;
   m_begin_videoStart = -1L;
   m_threshold_videoStart = m_bufferMapSize_chunk/2;
   m_nChunkRequestReceived = 0L;
   m_nChunkSent = 0L;
   m_nBufferMapRecv = 0L;

   // -------------------------------------------------------------------------
   // ------------------- Initialize local variables --------------------------
   // -------------------------------------------------------------------------
   m_seqNum_schedWinStart = 0L;
   m_seqNum_schedWinEnd = 0L;
   m_seqNum_schedWinHead = 0L;

   m_count_prev_chunkHit = 0L;
   m_count_prev_chunkMiss = 0L;
   // -------------------------------------------------------------------------
   // --------------------------- WATCH ---------------------------------------
   // -------------------------------------------------------------------------

   WATCH(m_localAddress);
   WATCH(m_localPort);
   WATCH(m_destPort);

   WATCH(param_interval_bufferMap);
   WATCH(param_bufferMapSize_second);
   WATCH(param_chunkSize);
   WATCH(param_videoStreamBitRate);
   WATCH(param_upBw);
   WATCH(param_downBw);
   WATCH(param_interval_chunkScheduling);
   WATCH(param_downBw);
   WATCH(param_minNOP);
   WATCH(param_maxNOP);
   WATCH(param_offsetNOP);
   //WATCH(param_interval_partnerlistCleanup);
   WATCH(param_interval_reportStatistic);
   WATCH(param_interval_reportActive);
   WATCH(param_threshold_idleDuration_buffermap);

   WATCH(m_videoStreamChunkRate);
   WATCH(m_bufferMapSize_chunk);
   WATCH(m_BufferMapPacketSize_bit);
   WATCH(m_nChunk_toRequest_perCycle);
   WATCH(m_downloadRate_chunk);
   //WATCH(m_nChunk_perSchedulingInterval);

   WATCH(m_state);
   WATCH(m_appSetting);
   WATCH(m_apTable);
   WATCH(m_churn);
   WATCH(m_partnerList);
   WATCH(m_videoBuffer);
}

void DonetPeer::finish()
{
   cancelAndDeleteAllTimer();

   // -- clear every thing
   //
   m_list_requestedChunk.clear();

   std::queue<SEQUENCE_NUMBER_T> empty;
   std::swap(m_requestedChunks, empty);
   while(!m_numRequestedChunks.empty()) m_numRequestedChunks.pop();

   // -- Debug
   //m_gstat->reportNumberOfPartner(m_partnerList->getSize());

   //reportStatus();
   debugOUT << "Print Partner List:" << endl;
   m_partnerList->print2();

   m_gstat->collectPlaybackPoint(m_player->getCurrentPlaybackPoint());
}

void DonetPeer::cancelAndDeleteAllTimer()
{
   if (timer_getJoinTime != NULL)
   {
      delete cancelEvent(timer_getJoinTime);
      timer_getJoinTime       = NULL;
   }

   if (timer_join != NULL)
   {
      delete cancelEvent(timer_join);
      timer_join              = NULL;
   }

   if (timer_sendBufferMap != NULL)
   {
      delete cancelEvent(timer_sendBufferMap);
      timer_sendBufferMap     = NULL;
   }

   if (timer_chunkScheduling != NULL)
   {
      delete cancelEvent(timer_chunkScheduling);
      timer_chunkScheduling   = NULL;
   }

   if (timer_findMorePartner != NULL)
   {
      delete cancelEvent(timer_findMorePartner);
      timer_findMorePartner   = NULL;
   }

   if (timer_startPlayer != NULL)
   {
      delete cancelEvent(timer_startPlayer);
      timer_startPlayer       = NULL;
   }

   //    if (timer_sendReport != NULL)
   //    {
   //       delete cancelEvent(timer_sendReport);
   //       timer_sendReport       = NULL;
   //    }

//   if (timer_timeout_waiting_response != NULL)
//   {
//      delete cancelEvent(timer_timeout_waiting_response);
//      timer_timeout_waiting_response       = NULL;
//   }

   if (timer_partnershipRefinement != NULL)
   {
      delete cancelEvent(timer_partnershipRefinement);
      timer_partnershipRefinement = NULL;
   }

   if (timer_reportStatistic != NULL)
   {
      delete cancelEvent(timer_reportStatistic);
      timer_reportStatistic = NULL;
   }

   if (timer_leave != NULL)
   {
      delete cancelEvent(timer_leave);
      timer_leave = NULL;
   }

   if (timer_reportActive != NULL)
   {
      delete cancelEvent(timer_reportActive);
      timer_reportActive = NULL;
   }

   if (timer_checkVideoBuffer != NULL)
   {
      delete cancelEvent(timer_checkVideoBuffer);
      timer_checkVideoBuffer = NULL;
   }
   //    if (timer_reportStatistic != NULL) cancelAndDelete(timer_reportStatistic);

}

void DonetPeer::cancelAllTimer()
{
   cancelEvent(timer_sendBufferMap);
   cancelEvent(timer_chunkScheduling);

   cancelEvent(timer_findMorePartner);
   cancelEvent(timer_partnershipRefinement);

   cancelEvent(timer_reportStatistic);
   cancelEvent(timer_reportActive);

   cancelEvent(timer_checkVideoBuffer);
}

void DonetPeer::handleTimerMessage(cMessage *msg)
{
   Enter_Method("handleTimerMessage()");

   if (msg == timer_sendBufferMap)
   {
      sendBufferMap();
      //if (m_sched_window_moved == true) checkVideoBuffer();
      scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);
   }
   else if (msg == timer_chunkScheduling)
   {
      chunkScheduling();
      scheduleAt(simTime() + param_interval_chunkScheduling, timer_chunkScheduling);
   }
   else if (msg == timer_reportStatistic)
   {
      EV << "hehehehe" << endl;
      handleTimerReportStatistic();
      scheduleAt(simTime() + param_interval_reportStatistic, timer_reportStatistic);
   }
   else if (msg == timer_reportActive)
   {
      handleTimerReportActive();
      scheduleAt(simTime() + param_interval_reportActive, timer_reportActive);
   }
   else if (msg == timer_findMorePartner)
   {
      handleTimerFindMorePartner();

      if (m_partnerList->getSize() < param_maxNOP)
         scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
      else
         scheduleAt(simTime() + 5 * param_interval_findMorePartner, timer_findMorePartner);
   }
   else if (msg == timer_partnershipRefinement)
   {
      handleTimerPartnershipRefinement();
      scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);
   }
//   else if (msg == timer_timeout_waiting_response)
//   {
//      handleTimerTimeoutWaitingAccept();
//   }
   else if (msg == timer_getJoinTime)
   {
      double arrivalTime = m_churn->getArrivalTime();

      EV << "Scheduled arrival time: " << simTime().dbl() + arrivalTime << endl;
      scheduleAt(simTime() + arrivalTime, timer_join);

      double departureTime = m_churn->getDepartureTime();
      if (departureTime > 0.0)
      {
         EV << "Scheduled departure time: " << simTime().dbl() + departureTime << endl;
         scheduleAt(simTime() + departureTime, timer_leave);
      }
      else
      {
         EV << "DepartureTime = " << departureTime << " --> peer won't leave" << endl;
      }
   }
   else if (msg == timer_join)
   {
      handleTimerJoin();
   }
   else if (msg == timer_leave)
   {
      //EV << "Timer leave" << endl;
      handleTimerLeave();
   }
   //    else if (msg == timer_sendReport)
   //    {
   //       handleTimerReport();
   //    }
}

void DonetPeer::handleTimerJoin(void)
{
   Enter_Method("handleTimerJoin()");

   EV << endl;
   EV << "----------------------- Handle timer JOIN ---------------------------" << endl;

   emit(sig_nJoin, 1);

   switch(m_state)
   {
   case MESH_JOIN_STATE_IDLE:
   {
      findPartner();

      break;
   }
   case MESH_JOIN_STATE_ACTIVE:
   {
      throw cException("Join action cannot be done in JOINED state");
      break;
   }
   default:
   {
      throw cException("Not in an expected state");
      break;
   }
   } // switch()
}

void DonetPeer::handleTimerLeave()
{
   Enter_Method("handleTimerLeave()");

   EV << "Handle timer leave" << endl;

   MeshPartnershipLeavePacket *leavePkt = generatePartnershipRequestLeavePacket();

   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      sendToDispatcher(leavePkt->dup(), m_localPort, iter->first, m_destPort);
   }
   delete leavePkt; leavePkt = NULL;

   // - clear the partnerlist
   m_partnerList->m_map.clear();

   // - Report to Active Peer Table
   m_apTable->removeAddress(getNodeAddress());

   // - Report to Discovery Layer
   m_memManager->deletePeerAddress(getNodeAddress());

   // - Report to statistic module
   //   m_gstat->reportNumberOfPartner(getNodeAddress(), 0); // left the system
   m_gstat->reportNumberOfPartner(getNodeAddress(), m_partnerList->m_map.size()); // leaving

   cancelAllTimer();

   m_state = MESH_JOIN_STATE_IDLE;
}

void DonetPeer::gracefulLeave(void)
{
   Enter_Method("handleTimerLeave()");

   debugOUT<< "Handle timer leave" << endl;

   MeshPartnershipLeavePacket *leavePkt = generatePartnershipRequestLeavePacket();

   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      sendToDispatcher(leavePkt->dup(), m_localPort, iter->first, m_destPort);
   }
   delete leavePkt; leavePkt = NULL;

   // - clear the partnerlist
   m_partnerList->m_map.clear();

   // - Report to Active Peer Table
   m_apTable->removeAddress(getNodeAddress());

   // - Report to Discovery Layer
   m_memManager->deletePeerAddress(getNodeAddress());

   // - Report to statistic module
   //   m_gstat->reportNumberOfPartner(getNodeAddress(), 0); // left the system
   m_gstat->reportNumberOfPartner(getNodeAddress(), m_partnerList->m_map.size()); // leaving

   cancelAllTimer();

   m_state = MESH_JOIN_STATE_IDLE;
}

void DonetPeer::handleTimerFindMorePartner(void)
{
   switch(m_state)
   {
   case MESH_JOIN_STATE_ACTIVE:
   {
      findPartner();
      break;
   }
   case MESH_JOIN_STATE_IDLE:
   {
      EV << "Cannot find new partner when state is IDLE" << endl;
      break;
   }
   default:
   {
      throw cException("Not in an expected state");
      break;
   }
   } // switch()
}

void DonetPeer::handleTimerPartnershipRefinement()
{
   Enter_Method("handleTimerPartnershipRefinement");

   debugOUT << "Partnership refinement ***************" << endl;

   // --------------------------------------------------------------------------
   // --- Update the records
   // --------------------------------------------------------------------------
   updateDataExchangeRecord(param_interval_partnershipRefinement);

   // temporarily commented
   //   if (m_partnerList->getSize() <= 1)
   //   {
   //      EV << "Minimum partnership size, should not cleanup now" << endl;
   //      return;
   //   }

   //if (m_player->playerStarted() == false)
   if (m_player->getState() != PLAYER_STATE_PLAYING)
   {
      EV << "Player has not started yet --> no refinement!" << endl;
      return;
   }

   // Temporarily commented
   //   if (m_player->getContinuityIndex() > 0.9)
   //   {
   //      EV << "CI is good enough, no need for partnership refinement" << endl;
   //      return;
   //   }

   debugOUT << "*************************************************************" << endl;
   debugOUT << "PartnerList BEFORE cleaning up uncommunicative ones:" << endl;
   debugOUT << "*************************************************************" << endl;
   m_partnerList->print2();

   // -- Clean all the partners who do not have any chunk exchange, and partner time is large enough
   //
   std::vector<IPvXAddress> disconnect_list;

   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      if (iter->second.getAverageChunkExchanged() <= 0.1)
      {
         if (simTime().dbl() - iter->second.getTimeInstanceAsPartner() > param_interval_partnershipRefinement)
         {
            //debugOUT << "This partner should be removed, since no data exchange since "
            //<< iter->second.getTimeInstanceAsPartner() << endl;

            // -- Should definitely remove the partner
            //
            //debugOUT << "A disconnect message will be sent to the partner " << iter->first << endl;

            disconnect_list.push_back(iter->first);
         }
      }
   }

   debugOUT << "remove set of " << disconnect_list.size() << " silent peers: " << endl;
   for (std::vector<IPvXAddress>::iterator iter = disconnect_list.begin(); iter != disconnect_list.end(); ++iter)
   {
      debugOUT << "this partner whose address is " << *iter << " will be deleted from the partnerlist" << endl;

      MeshPartnershipDisconnectPacket *disPkt = generatePartnershipDisconnectPacket();
      sendToDispatcher(disPkt, m_localPort, *iter, m_destPort);

      m_partnerList->deleteAddress(*iter);
      m_forwarder->removeRecord(*iter);
   }

   debugOUT << "*************************************************************" << endl;
   debugOUT << "PartnerList AFTER cleaning up uncommunicative ones:" << endl;
   debugOUT << "*************************************************************" << endl;
   m_partnerList->print2();

   // TODO: (Giang) has to find scenarios to test this behavior
   if (m_partnerList->getSize() == 0) // a lonely peer
   {
      debugOUT << "!!!Isolated peer!!!" << endl;
      rejoin();
      return;
   }

   if (m_partnerList->getSize() <= param_maxNOP)
   {
      debugOUT << "Peer has sufficient number of partners, no more refinement" << endl;
      return;
   }

   // -- Sorting the set
   Throughput_Set sorted_set;
   Throughput_Set provider_set; // :-)
   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      // -- not considering the "new" partner
      if (simTime().dbl() - iter->second.getTimeInstanceAsPartner() < param_interval_partnershipRefinement)
      {
         debugOUT << "Not considering the new partner " << iter->first << endl;
         continue;
      }

      //double max = std::max(iter->second.getAverageChunkSentPerInterval(), iter->second.getAverageChunkReceivedPerInterval());
      double max = std::max(iter->second.getAverageChunkSent(), iter->second.getAverageChunkReceived());
      sorted_set.insert(std::make_pair<IPvXAddress, double>(iter->first, max));

      provider_set.insert(std::make_pair<IPvXAddress, double>(iter->first, iter->second.getAverageChunkReceived()));
   }

   // -- Debugging
   //
   if (sorted_set.size() == 0)
      debugOUT << "sorted set EMPTY!" << endl;
   for (Throughput_Set::iterator iter = sorted_set.begin(); iter != sorted_set.end(); ++iter)
   {
      debugOUT << "- address: " << iter->first << " -- throughput: " << iter->second << endl;
   }

   std::vector<IPvXAddress> remove_set;
   IPvXAddress my_provider = provider_set.begin()->first;
   debugOUT << "my provider: " << my_provider << endl;
   short count = 1;
   for (Throughput_Set::iterator iter = sorted_set.begin(); iter != sorted_set.end(); ++iter)
   {
      if (iter->first == my_provider)
         continue;

      if (count++ < param_maxNOP)
         continue;

      remove_set.push_back(iter->first);
   }

   // -- Now remove the IP from the list of partners
   debugOUT << "remove set of " << remove_set.size() << " inactive peers: " << endl;
   for (std::vector<IPvXAddress>::iterator iter = remove_set.begin(); iter != remove_set.end(); ++iter)
   {
      debugOUT << *iter << endl;

      MeshPartnershipDisconnectPacket *disPkt = generatePartnershipDisconnectPacket();
      sendToDispatcher(disPkt, m_localPort, *iter, m_destPort);

      m_partnerList->deleteAddress(*iter);
      m_forwarder->removeRecord(*iter);
   }

}

//void DonetPeer::handleTimerTimeoutWaitingAccept()
//{
//   EV << endl;
//   EV << "------------------------- Handle timeout_waitingAccept --------------" << endl;

//   emit(sig_timeout, 1);

//   switch(m_state)
//   {
//   case MESH_JOIN_STATE_IDLE_WAITING:
//   {
//      // -- Update the state
//      EV << "State changes from MESH_JOIN_STATE_IDLE_WAITING to MESH_JOIN_STATE_IDLE" << endl;
//      m_state = MESH_JOIN_STATE_IDLE;

//      // -- Schedule for new JOIN
//      // this event, in turn, triggers findPartner()
//      scheduleAt(simTime() + uniform(0.0, 2.0), timer_join);
//      //scheduleAt(simTime(), timer_join);
//      break;
//   }
//   case MESH_JOIN_STATE_ACTIVE_WAITING:
//   {
//      // -- Process pending requests
//      // There should not be any pending request at this time
//      //        vector<PendingPartnershipRequest>::iterator iter = m_list_partnershipRequestingNode.begin();
//      //        for (; iter != m_list_partnershipRequestingNode.end(); ++iter)
//      //        {
//      //            considerAcceptPartner(*iter);
//      //        }
//      //        m_list_partnershipRequestingNode.clear();

//      EV << "State changes to MESH_JOIN_STATE_ACTIVEG" << endl;
//      m_state = MESH_JOIN_STATE_ACTIVE;
//      break;
//   }
//   case MESH_JOIN_STATE_ACTIVE:
//   {
//      //EV << "Timeout happens when the peer is in JOINED state" << endl;
//      throw cException("Timeout happens when the peer is in JOINED state");
//      break;
//   }
//   case MESH_JOIN_STATE_IDLE:
//   {
//      //EV << "Timeout happens when the peer is in IDLE state" << endl;
//      throw cException("Timeout happens when the peer is in IDLE state");
//      break;
//   }
//   default:
//   {
//      throw cException("Uncovered state");
//      break;
//   }
//   } // switch()
//}

//void DonetPeer::handleTimerReport()
//{

//}

void DonetPeer::handleTimerReportStatistic()
{
   //if (m_player->playerStarted() == true)
   if (simTime().dbl() < simulation.getWarmupPeriod().dbl())
      return;

   reportHitMiss();

   // -- Report End-to-End delays (between two sampling)
   //m_gstat->collectDeltaDelayOneOverlayHop(m_videoBuffer->getDeltaDelayOneOverlayHop());

   // -- Report overlay hop counts (between two sampling)
   //m_gstat->collectDeltaOverlayHopCount(m_videoBuffer->getDeltaOverlayHopCount());

   // -- Report number of received chunks
   //m_gstat->collectDeltaNumberOfReceivedChunk(m_videoBuffer->getDeltaNumberOfReceivedChunk());

   // -- Local statistics (for debugging)
   //long nChunk = m_videoBuffer->getNumberOfReceivedChunk();
   //if (nChunk > 0)
   //{
   //   emit(sig_e2eDelay, m_videoBuffer->getTotalDelayOneOverlayHop() / nChunk);
   //   emit(sig_overlayHopCount, m_videoBuffer->getTotalOverlayHopCount() / nChunk);
   //}

}

void DonetPeer::reportHitMiss()
{
   if (m_player->getState() == PLAYER_STATE_PLAYING)
   {
      //long m_count_chunkHit = m_player->getCountChunkHit();
      //long int delta = m_count_chunkHit - m_count_prev_chunkHit;
      long int delta = m_player->getCountChunkHit() - m_count_prev_chunkHit;
      m_count_prev_chunkHit = m_player->getCountChunkHit();
      m_gstat->increaseChunkHit((int)delta);
      //EV << "Got it: " << delta << endl;

      //long m_count_chunkMiss = m_player->getCountChunkMiss();
      //delta = m_count_chunkMiss - m_count_prev_chunkMiss;
      delta = m_player->getCountChunkMiss() - m_count_prev_chunkMiss;
      m_count_prev_chunkMiss = m_player->getCountChunkMiss();
      m_gstat->increaseChunkMiss((int)delta);
      //EV << "Got it, 2: " << delta << endl;
   }
   else
   {
      //      m_gstat->increaseChunkHit(0);
      //      m_gstat->increaseChunkMiss(0);
   }
}

void DonetPeer::handleTimerReportActive()
{
   Enter_Method("handleTimerReportActive");

   m_gstat->reportNumberOfPartner(getNodeAddress(), m_partnerList->m_map.size());
}

void DonetPeer::processPacket(cPacket *pkt)
{
   PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(pkt);
   EV << "Packet group: " << appMsg->getPacketGroup() << " --> for mesh module" << endl;
   if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
   {
      throw cException("Wrong packet type!");
   }

   MeshPeerStreamingPacket *meshMsg = check_and_cast<MeshPeerStreamingPacket *>(appMsg);
   switch (meshMsg->getPacketType())
   {
   case MESH_CHUNK_REQUEST:
   {
      processChunkRequest(pkt);
      break;
   }
   case MESH_BUFFER_MAP:
   {
      processPeerBufferMap(pkt);
      break;
   }
   case MESH_PARTNERSHIP_REQUEST:
   {
      processPartnershipRequest(pkt);
      break;
   }
   case MESH_PARTNERSHIP_ACCEPT:
   {
      processPartnershipAccept(pkt);
      break;
   }
   case MESH_PARTNERSHIP_REJECT:
   {
      processPartnershipReject(pkt);
      break;
   }
   case MESH_PARTNERSHIP_LEAVE:
   {
      processPartnershipLeave(pkt);
      break;
   }
   case MESH_PARTNERSHIP_DISCONNECT:
   {
      processPartnershipDisconnect(pkt);
      break;
   }
   default:
   {
      throw cException("Unrecognized packet types! %d", meshMsg->getPacketType());
      break;
   }
   } // switch

   delete pkt;
}

void DonetPeer::processPartnershipRequest(cPacket *pkt)
{
   EV << endl;
   EV << "-------- Process partnership Request --------------------------------" << endl;

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
      // -- Add peer directly to Partner List
      addPartner(requester.address, requester.upBW);

      EV << "Accepted pRequest from " << requester.address << endl;

      // -- Report to Active Peer Table to update the information
      EV << "Increment number of partner " << endl;
      m_memManager->incrementNPartner(getNodeAddress());

      // TODO: (Giang) should update the AP Table as well?
      //m_apTable->incrementNPartner(getNodeAddress());

      MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
      sendToDispatcher(acceptPkt, m_localPort, requester.address, requester.port);

      break;
   }
   case MESH_JOIN_STATE_IDLE:
   {
      debugOUT << "The node is currently idle --> should ignore this request" << endl;
      break;
   }
   default:
   {
      throw cException("Uncovered state, check assignment of state variable!");
      break;
   }
   } // switch()

}

void DonetPeer::processPartnershipAccept(cPacket *pkt)
{
   EV << endl;
   EV << "---------- Processing a partnership ACCEPT packet -------------------" << endl;

   // -- Extract the address of the accepter
   PendingPartnershipRequest acceptor;
   getSender(pkt, acceptor.address, acceptor.port);
   // MeshPartnershipAcceptPacket *acceptPkt = dynamic_cast<MeshPartnershipAcceptPacket *>(pkt);
   MeshPartnershipAcceptPacket *acceptPkt = check_and_cast<MeshPartnershipAcceptPacket *>(pkt);
   acceptor.upBW = acceptPkt->getUpBw();

   EV << "Acceptor: " << endl
      << "-- Address:\t\t"         << acceptor.address
      << "-- Port:\t\t"            << acceptor.port << endl
      << "-- Upload bandwidth:\t"  << acceptor.upBW << endl;

   //scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

   switch(m_state)
   {
   case MESH_JOIN_STATE_IDLE:
   {
      addPartner(acceptor.address, acceptor.upBW);

      EV << "First accept response from " << acceptor.address << endl;

      // -- Register itself to the APT
      m_apTable->addAddress(getNodeAddress());
      //        m_apTable->printActivePeerInfo(getNodeAddress());
      //        m_apTable->printActivePeerTable();

      m_memManager->addPeerAddress(getNodeAddress(), param_maxNOP);

      m_firstJoinTime = simTime().dbl();

      debugOUT << "joins at " << m_firstJoinTime << endl;

      // -- Cancel timer
      //        if (timer_timeout_waiting_accept) cancelAndDelete(timer_timeout_waiting_accept);
      //        timer_timeout_waiting_accept = NULL;
      //cancelEvent(timer_timeout_waiting_response);

      double rand_value;
      // -- Start several timers
      // -- 1. Chunk Scheduling timers
      rand_value = uniform(0.0, 1.0);
      EV << "-- Chunk scheduling will be triggered after " << rand_value << " seconds" << endl;
      scheduleAt(simTime() + rand_value, timer_chunkScheduling);

      // -- 2. Send buffer map timers
      rand_value = uniform(0.0, 0.5);
      EV << "-- Sending Buffer Map will be triggered after " << rand_value << " seconds" << endl;
      scheduleAt(simTime() + rand_value, timer_sendBufferMap);

      // -- 3. Have more partner timers
      EV << "-- Finding more partners will be triggered after "
         << param_interval_findMorePartner << " seconds" << endl;
      scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

      // -- 4. Active Player?
      EV << "-- Player will be actived now" << endl;
      debugOUT << "current state of the player " << m_player->getState() << endl;
      m_player->activate();
      //scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);

      // -- 5. Partnership refinement
      if (m_pRefinementEnabled)
      {
         //if (timer_partnershipRefinement->isScheduled()) cancelEvent(timer_partnershipRefinement);
         scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);
      }

      // -- Debug
      // m_gstat->reportMeshJoin();

      // -- Record join time (first accept response)
      m_joinTime = simTime().dbl();

      // Debuging with signals
      emit(sig_joinTime, simTime().dbl());

      EV << "State changes from IDLE_WAITING to ACTIVE" << endl;
      m_state = MESH_JOIN_STATE_ACTIVE;
      break;
   }
   case MESH_JOIN_STATE_ACTIVE:
   {
      EV << "An accept response from " << acceptor.address << endl;

      addPartner(acceptor.address, acceptor.upBW);

      m_memManager->incrementNPartner(getNodeAddress());

      // state remains
      break;
   }
   default:
   {
      throw cException("Uncovered state, check assignment of state variable!");
      break;
   }
   } // switch()

}

void DonetPeer::processPartnershipReject(cPacket *pkt)
{
   EV << endl;
   EV << "-------- Process partnership Reject ---------------------------------" << endl;

   emit(sig_pRejectRecv, 1);

   switch(m_state)
   {
   case MESH_JOIN_STATE_IDLE:
   {
      EV << "Should find another partner NOW ..." << endl;
      // state remains as idel

      scheduleAt(simTime() + uniform(0, 1.0), timer_join);
      break;
   }
   case MESH_JOIN_STATE_ACTIVE:
   {
      EV << "REJECT message arrived too late, but no problem because of timeout, will be ignored!" << endl;

      // -- State remains
      break;
   }
   default:
   {
      throw cException("Uncovered state, check assignment of state variable!");
      break;
   }
   } // switch()
}

void DonetPeer::processPartnershipDisconnect(cPacket* pkt)
{
   // -- Extract the address of the accepter
   IPvXAddress remoteAddr;
   int remotePort;
   getSender(pkt, remoteAddr, remotePort);

   debugOUT << "received a disconnect message from " << remoteAddr << endl;

   m_partnerList->deleteAddress(remoteAddr);
   m_forwarder->removeRecord(remoteAddr);

   // -- check if peer is lonely, in that case, rejoin!
   if (m_partnerList->getSize() == 0)
   {
      rejoin();
   }
}

void DonetPeer::sendBufferMap(void)
{
//   if (getNodeAddress() == IPvXAddress("192.168.0.45"))
//   {
//      std::cout << endl << "crashed node: " << getNodeAddress() << endl;
//      std::cout << "Current playback point: " << m_player->getCurrentPlaybackPoint() << endl;

//      cout << "Video buffer: "
//           << " -- start " << m_videoBuffer->getBufferStartSeqNum()
//           << " -- head " << m_videoBuffer->getHeadReceivedSeqNum()
//           << " -- end " << m_videoBuffer->getBufferEndSeqNum()
//           << endl;
//   }

   DonetBase::sendBufferMap();

//   try
//   {
//      DonetBase::sendBufferMap();
//   }
//   catch (cException e)
//   {
//      cout << "exeption @peer " << getNodeAddress() << endl;
//   }
}

void DonetPeer::processPeerBufferMap(cPacket *pkt)
{
   EV << endl;
   EV << "---------- Process received buffer map ------------------------------" << endl;

   /*
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(appMsg);
    int countOne = 0;
    for (unsigned int i = 0; i < bmPkt->getBufferMapArraySize(); i++)
    {
        countOne += bmPkt->getBufferMap(i)==true?1:0;
    }
    r_countBM.record(countOne);
    */

   IPvXAddress senderAddress = getSender(pkt);
   EV << "-- Received a buffer map from " << senderAddress << endl;
//   debugOUT << "Peer " << getNodeAddress() << " received buffer map from: " << senderAddress << endl;

   if (m_partnerList->hasAddress(senderAddress) == false)
   {
      EV << "-- Buffer Map is received from a non-partner peer!" << endl;
      return;
   }


   NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);

   // -- Cast to the BufferMap packet
   MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

   debugOUT << "-- recv BM: "
            << "  -- Start: "   << bmPkt->getBmStartSeqNum()
            << "  -- End: "     << bmPkt->getBmEndSeqNum()
            << "  -- Head: "    << bmPkt->getHeadSeqNum()
            << endl;

   // -- Copy the BufferMap content to the current record
   nbr_info->copyFrom(bmPkt);

   // -- Update the timestamp of the received BufferMap
   nbr_info->setLastRecvBmTime(simTime().dbl());

   // -- Update the range of the scheduling window
   m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;
   EV << "-- Head for the scheduling window: " << m_seqNum_schedWinHead << endl;

   // -- Debug
   ++m_nBufferMapRecv;
   // nbr_info->printRecvBm();
   // m_partnerList->printRecvBm(senderAddress);

}

void DonetPeer::addPartner(IPvXAddress remote, double upbw)
{
   //   DonetBase::addPartner(remote, upbw);
   int nChunk = (int)(param_interval_chunkScheduling*upbw/param_chunkSize/8); // per scheduling cycle
   m_partnerList->addAddress(remote, upbw, nChunk);

   m_forwarder->addRecord(remote);

   //   m_forwarder->getRecordChunk();
}

/**
 * @brief DonetPeer::findPartner
 * @return
 * false if no partner had been contacted
 * true if a partner had been contacted
 */
void DonetPeer::findPartner()
{
   Enter_Method("findPartner()");

   sendPartnershipRequest();

}

void DonetPeer::sendPartnershipRequest(void)
{
   IPvXAddress addressRandPeer = getNodeAddress();
   short count = 0;
   do
   {
      count++;
      addressRandPeer = m_memManager->getRandomPeer(getNodeAddress());
      if (count > 10)
         return;

      if (m_partnerList->have(addressRandPeer))
      {
         //debugOUT << "already have this address " << addressRandPeer << " in the partnerList, count = " << count << endl;
      }
   }
   while(m_partnerList->have(addressRandPeer));

   if (addressRandPeer == getNodeAddress())
      debugOUT << "Get the IP of itself from the APT --> This should never happen! Count = " << count << endl;

   MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
   reqPkt->setUpBw(param_upBw);
   reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

   sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);
}

void DonetPeer::updateDataExchangeRecord(double samplingInterval)
{
   Enter_Method("updateDataExchangeRecord()");
   EV << "--updateDataExchangeRecord--" << endl;

   RecordCountChunk record;

   for(std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
       iter != m_partnerList->m_map.end(); ++iter)
   {
      record = m_forwarder->getRecordChunk((IPvXAddress &)iter->first);
      double duration = simTime().dbl() - record.m_oriTime;

      //debugOUT << "simTime = " << simTime().dbl() << " -- oriTime = " << record.m_oriTime << endl;
      assert(duration >= 0);

      // -- update the data exchange since joining
      //
      if (duration == 0.0)
      {
         iter->second.setAverageChunkSent(0.0);
         iter->second.setAverageChunkReceived(0.0);
         iter->second.setAverageChunkExchanged(0.0);
      }
      else
      {
         iter->second.setAverageChunkReceived(record.m_chunkReceived / duration);
         iter->second.setAverageChunkSent(record.m_chunkSent / duration);
         iter->second.setAverageChunkExchanged((record.m_chunkSent + record.m_chunkReceived) / duration);
      }

      // -- update the data exchange per interval
      //
      long int chunkSent_diff = record.m_chunkSent - iter->second.getCountChunkSent();
      long int chunkReceived_diff = record.m_chunkReceived - iter->second.getCountChunkReceived();

      iter->second.setAverageChunkSentPerInterval(chunkSent_diff / samplingInterval);
      iter->second.setAverageChunkReceivedPerInterval(chunkReceived_diff / samplingInterval);
      iter->second.setAverageChunkExchangedPerInterval((chunkSent_diff + chunkReceived_diff) / samplingInterval);

      // -- store the latest count on sent/received chunks
      //
      iter->second.setCountChunkSent(record.m_chunkSent);
      iter->second.setCountChunkReceived(record.m_chunkReceived);

//      debugOUT << "  @Partner " << ++count << " - " << iter->first << " - as partner at " << record.m_oriTime
//               << " (or at " << iter->second.getTimeInstanceAsPartner() << "): " << endl;
//      debugOUT << "\tPrevious numChunk -- sent - received: " << iter->second.getCountPrevChunkSent()
//               << " - " << iter->second.getCountPrevChunkReceived() << endl;
//      debugOUT << "\tCurrent numChunk -- sent - received: " << iter->second.getCountChunkSent()
//               << " - " << iter->second.getCountChunkReceived() << endl;
//      debugOUT << "\tAverage since joining numChunk -- sent - received - exchanged: " << iter->second.getAverageChunkSent()
//               << " - " << iter->second.getAverageChunkReceived()
//               << " - " << iter->second.getAverageChunkExchanged() << endl;
//      debugOUT << "\tAverage per interval: numchunk -- sent - received - exchanged: "
//               << iter->second.getAverageChunkSentPerInterval()
//               << " - " << iter->second.getAverageChunkReceivedPerInterval()
//               << " - " << iter->second.getAverageChunkExchangedPerInterval() << endl;

      // -- Debug
//      EV << "Previous -- chunk sent: " << iter->second.getCountPrevChunkSent()
//         << " -- chunk received: " << iter->second.getCountPrevChunkReceived() << endl;
//      EV << "Current -- chunk sent: " << iter->second.getCountChunkSent()
//         << " -- chunk received: " << iter->second.getCountChunkReceived() << endl;
//      EV << "Average since joining -- chunk sent: " << iter->second.getAverageChunkSent()
//         << " -- chunk received: " << iter->second.getAverageChunkReceived()
//         << " -- chunk exchanged: " << iter->second.getAverageChunkExchanged() << endl;
//      EV << "Average per interval: -- chunk sent: " << iter->second.getAverageChunkSentPerInterval()
//         << " -- chunk received: " << iter->second.getAverageChunkReceivedPerInterval()
//         << " -- chunk exchanged: " << iter->second.getAverageChunkExchangedPerInterval() << endl;

   } // for
}

int DonetPeer::initializeSchedulingWindow()
{
   // Browse through all partners and find an optimal scheduling window
   SEQUENCE_NUMBER_T max_start = 0L, min_head = 0L;

   debugOUT << "Initializing with " << m_partnerList->m_map.size() << " partners" << endl;

   std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
   max_start  = iter->second.getSeqNumRecvBmStart();
   min_head = iter->second.getSeqNumRecvBmHead();
   debugOUT << "Partner " << iter->first << " with start: " << max_start << " -- head: " << min_head << endl;
   //debugOUT << "\t temp_start = " << max_start << " -- temp_head = " << min_head << endl;

   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
   {
      SEQUENCE_NUMBER_T temp_head = iter->second.getSeqNumRecvBmHead();
      SEQUENCE_NUMBER_T temp_start = iter->second.getSeqNumRecvBmStart();

      debugOUT << "Partner " << iter->first << " with start: " << temp_start << " -- head: " << temp_head << endl;
      //debugOUT << "\t temp_start = " << temp_start << " -- temp_head = " << temp_head << endl;

      if (temp_head != -1L && temp_head != 0L)
      {
         if (min_head == -1L || min_head == 0L)
         {
            min_head = (temp_head == -1L) ? min_head : temp_head;
         }
         else
         {
            min_head = (min_head > temp_head) ? temp_head : min_head;
         }
      }

      //debugOUT << "min_head = " << min_head << endl;
      max_start = (max_start < temp_start) ? temp_start : max_start;
      //debugOUT << "max_start = " << max_start << endl;
   }
   debugOUT << "------> max_start = " << max_start << " -- min_head = " << min_head <<  endl;

   //int playout_offset = (int)(m_player->getPercentBufferHigh() * m_videoBuffer->getSize() - 0.5 * m_videoStreamChunkRate);

   // -- Finding the start of the scheduling window
   //
   if (min_head > 0L)
   {
      m_sched_window.start = (max_start + min_head) / 2;

//      if (max_start + playout_offset > min_head)
//         m_sched_window.start = (max_start + min_head) / 2;
//      else
//         m_sched_window.start = max_start + playout_offset;
      m_sched_window.end   = m_sched_window.start + m_bufferMapSize_chunk - 1;
      debugOUT << "Scheduling window [start, end] = " << m_sched_window.start << " - " << m_sched_window.end << endl;

      m_videoBuffer->initializeRangeVideoBuffer(m_sched_window.start);

      cout << "@ " << simTime().dbl() << " @peer " << getNodeAddress() << " -- max_start = " << max_start
           << " -- min_head = " << min_head << " --> start = " << m_sched_window.start << endl;

      return INIT_SCHED_WIN_GOOD;
   }

   debugOUT << "max_start = " << max_start << " -- min_head = " << min_head << " --> not initialized!" << endl;
   return INIT_SCHED_WIN_BAD;
}

bool DonetPeer::shouldStartChunkScheduling(void)
{
   //if (simTime().dbl() - m_firstJoinTime >= param_waitingTime_SchedulingStart
   double elapsed_time_since_joining = simTime().dbl() - m_joinTime;
   int current_number_of_partner = m_partnerList->getSize();
   EV  << "Elapsed time since joining: " << elapsed_time_since_joining << " -- " \
       << "Current number of partners: " << current_number_of_partner << endl;

   if (elapsed_time_since_joining >= param_waitingTime_SchedulingStart
       || current_number_of_partner >= param_nNeighbor_SchedulingStart)
   {
      return true;
   }

   return false;
}

bool DonetPeer::should_be_requested(SEQUENCE_NUMBER_T seq_num)
{
   // -- Check if the chunk has been recently requested
   if (recentlyRequestedChunk(seq_num) == true)
   {
      return false;
   }

//   if (m_nChunkRequested_perSchedulingInterval > m_downloadRate_chunk)
//   {
//      return false;
//   }

   return true;
}

/*
 * This function is a _wrapper_ for the specific chunk scheduling algorithm
 */
void DonetPeer::chunkScheduling()
{
   if (m_partnerList->getSize() <= 0)
   {
      EV << "No partner just yet, exiting from chunk scheduling" << endl;
      return;
   }

   if (m_scheduling_started == false)
   {
      int ret = initializeSchedulingWindow();
      if (ret == INIT_SCHED_WIN_BAD)
      {
         EV << "Failed to initialize the scheduling window" << endl;
         return;
      }
      m_scheduling_started = true;
   }

   //updateRange();

//   if (getNodeAddress() == IPvXAddress("192.168.0.16"))
//   {
//      debugOUT << "Partners of the peer " << getNodeAddress() << " are: " << endl;

//      for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
//           iter != m_partnerList->m_map.end(); ++iter)
//      {
//         debugOUT << iter->first << " with buffer map: " << endl;
//         iter->second.printRecvBm2();
//      }
//   }

   // -------------------------------------------------------------------------
   // --- Debugging
   // -------------------------------------------------------------------------
   emit(sig_nPartner, m_partnerList->getSize());
   reportLocalStatistic();

   long int temp = m_videoBuffer->getNChunkReceived();
   emit(sig_inThroughput, (temp - m_prevNChunkReceived)/param_interval_chunkScheduling );
   m_prevNChunkReceived = temp;

   emit(sig_nBufferMapReceived, double(m_nBufferMapRecv)/(simTime().dbl() - m_firstJoinTime));

   //updateDataExchangeRecord(param_interval_chunkScheduling);

   m_videoBuffer->printStatus();

   debugOUT << "sched_win:: start " << m_sched_window.start << " -- end " << m_sched_window.end << endl;
   debugOUT << "current pb point: " << m_player->getCurrentPlaybackPoint() << endl;
   debugOUT << "video buffer:: start " << m_videoBuffer->getBufferStartSeqNum() << " -- end " << m_videoBuffer->getBufferEndSeqNum() << endl;
//   randomChunkScheduling(m_sched_window.start, m_sched_window.end);

//   if (m_player->getState() == PLAYER_STATE_PLAYING)
//      m_sched_window.start = m_player->getCurrentPlaybackPoint();
//   m_sched_window.head = m_partnerList->getMaxHeadSequenceNumber();
//   m_sched_window.start = m_sched_window.head

   donetChunkScheduling(m_sched_window.start, m_sched_window.end);

   //int adjustment = intrand(2);
   int adjustment = 0;
   int offset = m_videoStreamChunkRate - adjustment;
   // -- Move the scheduling window forward
   //
   if (m_player->getState() == PLAYER_STATE_PLAYING)
   {
      if (m_sched_window_moved == false)
      {
         if (m_player->getCurrentPlaybackPoint() - m_sched_window.start > m_player->getPercentBufferLow() * m_bufferMapSize_chunk)
         {
            m_sched_window.start += offset;
            m_sched_window.end  += offset;

//            m_videoBuffer->setBufferStartSeqNum(m_sched_window.start);
//            m_videoBuffer->setBufferEndSeqNum(m_sched_window.end);

            m_sched_window_moved = true;
         }
      }
      else
      {
         m_sched_window.start += offset;
         m_sched_window.end  += offset;

//         m_videoBuffer->setBufferStartSeqNum(m_sched_window.start);
//         m_videoBuffer->setBufferEndSeqNum(m_sched_window.end);
      }
   }
}

void DonetPeer::reportLocalStatistic(void)
{
   // -- Report ranges
//   emit(sig_minStart, m_minStart);
//   emit(sig_maxStart, m_maxStart);
//   emit(sig_minHead, m_minHead);
//   emit(sig_maxHead, m_maxHead);
//   emit(sig_currentPlaybackPoint, m_player->getCurrentPlaybackPoint());
//   emit(sig_bufferStart, m_videoBuffer->getBufferStartSeqNum());
//   emit(sig_bufferHead, m_videoBuffer->getHeadReceivedSeqNum());


   // -- Statistics from Player
   long int nHit = m_player->getCountChunkHit();
   long int nMiss = m_player->getCountChunkMiss();

   debugOUT << "Hit = " << nHit << " -- Miss = " << nMiss << endl;

   if ((nHit + nMiss) == 0)
   {
      emit (sig_localCI, 0);
   }
   else
   {
      emit(sig_localCI, (long double)nHit / (nHit + nMiss));
   }

   emit(sig_nPartner, m_partnerList->getSize());
}

// Update range for statistics
void DonetPeer::updateRange(void)
{
   // Browse through all partners and find an optimal scheduling window
   //std::map<IPvXAddress, NeighborInfo*>::iterator iter = m_partnerList->m_map.begin();
   std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
   m_minStart = iter->second.getSeqNumRecvBmStart();
   m_maxStart = iter->second.getSeqNumRecvBmStart();

   m_minHead = iter->second.getSeqNumRecvBmHead();
   m_maxHead = iter->second.getSeqNumRecvBmHead();

   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
   {
      SEQUENCE_NUMBER_T temp = iter->second.getSeqNumRecvBmHead();
      m_minHead = (m_minHead > temp) ? temp : m_minHead;
      m_maxHead = (m_maxHead < temp) ? temp : m_maxHead;

      temp = iter->second.getSeqNumRecvBmStart();
      m_minStart = (m_minStart > temp) ? temp : m_minStart;
      m_maxStart = (m_maxStart < temp) ? temp : m_maxStart;
   }
}



void DonetPeer::getAppSetting()
{
   EV << "DonetBase::getAppSetting" << endl;

   // -- Calling the function of the base
   DonetBase::getAppSetting();

   // -- Especifically for the DonetPeer
   // !!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!
   m_schedulingWindowSize = m_bufferMapSize_chunk;

}

void DonetPeer::bindToGlobalModule(void)
{
   DonetBase::bindToGlobalModule();

   // -- Churn
   cModule *temp = simulation.getModuleByPath("churnModerator");
   m_churn = check_and_cast<IChurnGenerator *>(temp);
   //if (m_churn == NULL) throw cException("m_churn == NULL is invalid");
   EV << "Binding to churnModerator is completed successfully" << endl;
}

void DonetPeer::bindToMeshModule(void)
{
   DonetBase::bindToMeshModule();

   // -- Player
   cModule *temp = getParentModule()->getModuleByRelativePath("player");
   //m_player = check_and_cast<Player *>(temp);
   m_player = check_and_cast<PlayerBase *>(temp);
   EV << "Binding to churnModerator is completed successfully" << endl;

}

// Obsolete
//void DonetPeer::startPlayer(void)
//{
//    if (shouldStartPlayer())
//    {
//        EV << "Player should start now. " << endl;
//        m_player->startPlayer();

//        emit(sig_playerStartTime, simTime().dbl());
//    }
//    else
//    {
//        EV << "Player should not start at the moment." << endl;
//        scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);
//    }

//}

// Obsolete
//bool DonetPeer::shouldStartPlayer(void)
//{
//    EV << "---------- Check whether should start the Player --------------------" << endl;
//    EV << "-- Threshold: " << m_bufferMapSize_chunk / 2 << " ";
//    if (m_videoBuffer->getNumberOfChunkFill() >= m_bufferMapSize_chunk / 2)
//    {
//        EV << "-- Enough chunks --> The Player should start now!" << endl;

//        m_video_startTime = simTime().dbl();
//        m_head_videoStart = m_videoBuffer->getHeadReceivedSeqNum();
//        m_begin_videoStart = m_videoBuffer->getBufferStartSeqNum();
//        return true;
//    }

//    EV << "-- Not enough chunks --> wait a bit more!" << endl;
//    return false;
//}

// TODO: (Giang) -- Obsolete???
//
//int DonetPeer::numberOfChunkToRequestPerCycle(void)
//{
//   int ret = 0;
//   if (m_player->playerStarted() == false)
//   {
//      // -- Chunks will be requested orderly to avoid congestion at requested peer
//      // -- Trade-off is longer startup delays

//      ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_moderate);
//   }
//   else if (inScarityState() == true)
//   {
//      // -- In "sensitive" state, since the play might soon be stalled because of no chunks in the buffer

//      ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_aggresive);
//   }
//   else
//   {
//      // -- Request chunks "freely", since there would be not so many chunks available anyway

//      ret = -1; // Unlimited
//   }

//   //EV << "Number of chunk to request per scheduling cycle: " << ret << endl;

//   return ret;
//}

bool DonetPeer::recentlyRequestedChunk(SEQUENCE_NUMBER_T seq_num)
{
   vector<SEQUENCE_NUMBER_T>::iterator it;
   it = find (m_list_requestedChunk.begin(), m_list_requestedChunk.end(), seq_num);

   if (it == m_list_requestedChunk.end())
      return false;

   return true;
}

void DonetPeer::refreshListRequestedChunk(void)
{
   int expectedSize = (int)(param_factor_requestList * m_nChunk_toRequest_perCycle);
   int nRedundantElement = m_list_requestedChunk.size() - expectedSize;
   if (nRedundantElement > 0)
   {
      EV << "-- " << nRedundantElement << " element should be deleted" << endl;
      m_list_requestedChunk.erase(m_list_requestedChunk.begin(), m_list_requestedChunk.begin()+nRedundantElement);
   }
}

// TODO: (Giang) obsolete?
//
//bool DonetPeer::inScarityState(void)
//{
//   int offset = m_videoBuffer->getHeadReceivedSeqNum() - m_player->getCurrentPlaybackPoint();

//   if (offset < (int)(m_videoStreamChunkRate * param_threshold_scarity))
//      return true;

//   return false;
//}

void DonetPeer::printListOfRequestedChunk(void)
{
   if (ev.isGUI() == false)
      return;

   EV << "Recently requested chunks: " << endl;
   int count = 1;
   vector<SEQUENCE_NUMBER_T>::iterator iter;
   for (iter = m_list_requestedChunk.begin(); iter != m_list_requestedChunk.end(); ++iter)
   {
      EV << *iter << " ";
      if (count % 10 == 0) EV << "\t";
      if (count % 30 == 0) EV << endl;
      count++;
   }
   EV << endl;
}

MeshPartnershipLeavePacket* DonetPeer::generatePartnershipRequestLeavePacket()
{
   MeshPartnershipLeavePacket *leavePkt = new MeshPartnershipLeavePacket("MESH_PEER_LEAVE");
   leavePkt->setBitLength(m_appSetting->getPacketSizePartnershipLeave());

   return leavePkt;
}

void DonetPeer::rejoin()
{
   debugOUT << "rejoin!!!!" << endl;

   cancelAllTimer();
   m_apTable->removeAddress(getNodeAddress());
   m_state = MESH_JOIN_STATE_IDLE;
   m_player->scheduleStopPlayer();

   if (timer_partnershipRefinement->isScheduled())
   {
      cout << "strange behavior " << getNodeAddress() << endl;
      cancelEvent(timer_partnershipRefinement);
   }
   // TODO: (Giang) report the state to the membership layer

   cout << "crashed node: " << getNodeAddress() << endl;
   scheduleAt(simTime() + 1+dblrand(), timer_join);
   ++m_count_rejoin;
}

void DonetPeer::checkVideoBuffer()
{
   if (m_videoBuffer->getPercentFill() < m_player->getPercentBufferLow())
   {
      debugOUT << "Buffer status not sufficient --> rejoin" << endl;
      rejoin();
   }
}

void DonetPeer::findExpectedSet(SEQUENCE_NUMBER_T currentPlaybackPoint,
                                SEQUENCE_NUMBER_T lower_bound,
                                SEQUENCE_NUMBER_T upper_bound)
{
   for (SEQUENCE_NUMBER_T seq_num = std::max(lower_bound, currentPlaybackPoint);
        seq_num <= upper_bound; ++seq_num)
   {
//      if (!should_be_requested(seq_num))
//      {
//         continue;
//      }

      if (m_videoBuffer->isInBuffer(seq_num) == false)
      {
         m_expected_set.push_back(seq_num);
      }
   }
}

void DonetPeer::printExpectedSet()
{
   cout << "Expected set: " << endl;
   for (std::vector<SEQUENCE_NUMBER_T>::iterator iter = m_expected_set.begin();
        iter != m_expected_set.end(); ++iter)
   {
      std::cout << *iter << " ";
   }
   std::cout << endl;
}

