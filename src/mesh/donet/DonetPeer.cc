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

//#include "InterfaceTableAccess.h"
//#include "IPv4InterfaceData.h"
//#include "IChurnGenerator.h"
//#include "MeshPeerStreamingPacket_m.h"
//#include <cmessage.h>

#include "DonetPeer.h"
#include "DpControlInfo_m.h"
#include <algorithm>

using namespace std;

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

        return;
    }
    if (stage != 3)
        return;

    bindToMeshModule();
    bindToGlobalModule();
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

    //timer_timeout_waiting_accept = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
    timer_timeout_waiting_response  = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
//    timer_rejoin            = new cMessage("MESH_PEER_TIMER_REJOIN");
    timer_partnershipRefinement = new cMessage("MESH_PEER_TIMER_PARTNERSHIP_REFINEMENT");

    timer_partnerListCleanup = new cMessage("MESH_PEER_TIMER_PARTNERLIST_CLEANUP");

    // Parameters of the module itself
    // param_bufferMapInterval             = par("bufferMapInterval");
    // param_chunkSchedulingInterval       = par("chunkSchedulingInterval");
    //    param_interval_rejoin               = par("interval_rejoin");

    param_interval_chunkScheduling        = par("interval_chunkScheduling");
    param_interval_findMorePartner        = par("interval_findMorePartner");
    param_interval_timeout_joinReqAck     = par("interval_timeout_joinReqAck");
    param_interval_starPlayer             = par("interval_startPlayer");
    param_interval_partnershipRefinement  = par("interval_partnershipRefinement");
    param_interval_partnerlistCleanup     = par("interval_partnerlistCleanup");
    param_interval_reportStatistic        = par("interval_reportStatistic");

    if (!param_interval_reportActive)
       param_interval_reportActive = par("interval_reportActive");

    param_threshold_idleDuration_buffermap = par("threshold_idleDuration_buffermap");

    param_nNeighbor_SchedulingStart     = par("nNeighbor_SchedulingStart");
    param_waitingTime_SchedulingStart   = par("waitingTime_SchedulingStart");

    param_requestFactor_moderate        = par("requestFactor_moderate");
    param_requestFactor_aggresive       = par("requestFactor_aggressive");
    param_maxNOP                        = par("maxNOP");
    param_offsetNOP                     = par("offsetNOP");
    param_threshold_scarity             = par("threshold_scarity");

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
    // -- Init for activity logging:
//    string path = "..//results//";
//    string filename = path + getNodeAddress().str();
//    m_activityLog.open(filename.c_str(), fstream::out);

//    WATCH(filename.c_str());

    // -- State variable for join process
    m_nRequestSent = m_nRejectSent = 0;

    // -- For scheduling
    m_seqNum_schedWinHead = 0L;

    m_prevNChunkReceived = 0L;

    // -------------------------------------------------------------------------
    // -- Signals
    // -------------------------------------------------------------------------
    sig_nJoin               = registerSignal("Signal_Join");

    sig_newchunkForRequest    = registerSignal("Signal_nNewChunkForRequestPerCycle");
    sig_nChunkRequested       = registerSignal("Signal_nChunkRequested");

    // -- Unimportant signals:
    //signal_nPartner         = registerSignal("Signal_nPartner");

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
    WATCH(param_interval_partnerlistCleanup);
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

        // -- Debug
    //m_gstat->reportNumberOfPartner(m_partnerList->getSize());

    //reportStatus();
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

    if (timer_timeout_waiting_response != NULL)
    {
       delete cancelEvent(timer_timeout_waiting_response);
       timer_timeout_waiting_response       = NULL;
    }

    if (timer_partnershipRefinement != NULL)
    {
       delete cancelEvent(timer_partnershipRefinement);
       timer_partnershipRefinement = NULL;
    }

    if (timer_partnerListCleanup != NULL)
    {
       delete cancelEvent(timer_partnerListCleanup);
       timer_partnerListCleanup = NULL;
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
//    if (timer_reportStatistic != NULL) cancelAndDelete(timer_reportStatistic);

}

void DonetPeer::cancelAllTimer()
{
   cancelEvent(timer_sendBufferMap);
   cancelEvent(timer_chunkScheduling);
   cancelEvent(timer_findMorePartner);
   cancelEvent(timer_partnershipRefinement);
   cancelEvent(timer_partnerListCleanup);
   cancelEvent(timer_reportStatistic);
   cancelEvent(timer_reportActive);
}

void DonetPeer::handleTimerMessage(cMessage *msg)
{
    Enter_Method("handleTimerMessage()");

    if (msg == timer_sendBufferMap)
    {
        sendBufferMap();
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
        //findMorePartner();
        handleTimerFindMorePartner();
        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
    }
    else if (msg == timer_partnershipRefinement)
    {
       handleTimerPartnershipRefinement();
       scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);
    }
    else if (msg == timer_partnerListCleanup)
    {
       handleTimerPartnerlistCleanup();
       scheduleAt(simTime() + param_interval_partnerlistCleanup, timer_partnerListCleanup);
    }
    else if (msg == timer_timeout_waiting_response)
    {
        handleTimerTimeoutWaitingAccept();
    }
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
        //join();
        handleTimerJoin();

        // -- Schedule for a rejoin (if any)
        // scheduleAt(simTime() + param_interval_rejoin, timer_join);
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

    //m_gstat->reportNumberOfJoin(1);
    emit(sig_nJoin, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE:
    {
        if (findPartner() == true)
        {
            // -- State changes to waiting mode, since a request has been sent
            scheduleAt(simTime() + param_interval_waitingPartnershipResponse, timer_timeout_waiting_response);

            EV << "State changes to waiting mode" << endl;
            m_state = MESH_JOIN_STATE_IDLE_WAITING;
        }
        else
        {
           // State remains ACTIVE
        }

        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        throw cException("Join action cannot be done in JOINED_WAITING state");
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        //EV << "Join action cannot be done in JOINED state" << endl;
        throw cException("Join action cannot be done in JOINED state");
        break;
    }
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        //EV << "Join action cannot be done in IDLE_WAITING state" << endl;
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

void DonetPeer::handleTimerFindMorePartner(void)
{
    switch(m_state)
    {
    case MESH_JOIN_STATE_ACTIVE:
    {
        bool ret = findPartner();

        if (ret == true)
        {
           // -- State changes to waiting mode, since a request has been sent
           m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        }
        else
        {
           // State remains ACTIVE
        }

        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Do NOTHING, here!

        EV << "State remains as MESH_JOIN_STATE_ACTIVE_WAITING" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        //return;
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        EV << "Cannot find new partner when state is IDLE" << endl;
        break;
    }
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        EV << "Cannot start finding new partner when state is IDLE_WAITING" << endl;
        break;
    }
    default:
    {
        throw cException("Not in an expected state");
        break;
    }
    } // switch()

    //scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
}

void DonetPeer::handleTimerPartnershipRefinement()
{
   Enter_Method("handleTimerPartnershipRefinement");

   EV << endl << "**************************************************************" << endl;
   EV << "Partnership refinement !!!" << endl;

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

   // --------------------------------------------------------------------------
   // --- Find the min throughput
   // --------------------------------------------------------------------------
   IPvXAddress address_minThroughput = IPvXAddress("10.0.0.9");
   IPvXAddress address_min_inThroughput = IPvXAddress("10.0.0.9");
   double minThroughput = (double)INT_MAX;
   double min_inThroughput = (double)INT_MAX;
   //double time_minThroughput = 0.0;
   // -- Find the mininum value of throughputs
   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      EV << "Address " << iter->first
         << " -- average chunk sent: " << iter->second.getAverageChunkSent()
         << " -- average chunk received: " << iter->second.getAverageChunkReceived()
         << " -- average chunk exchanged: " << iter->second.getAverageChunkExchanged()
         << endl;
      if (minThroughput > iter->second.getAverageChunkExchanged())
      {
         minThroughput = iter->second.getAverageChunkExchanged();
         address_minThroughput = iter->first;
      }

      if (min_inThroughput > iter->second.getAverageChunkReceived())
      {
         min_inThroughput = iter->second.getAverageChunkReceived();
         address_min_inThroughput = iter->first;
      }
   }

   EV << "Partner " << address_minThroughput << endl
      << "\t with min throughput " << minThroughput << endl;
//      << "\t from time " << time_minThroughput << endl;

   EV << "Partner " << address_min_inThroughput
      << " -- with min_inThroughput " << min_inThroughput << endl;

//   if (minThroughput > m_videoStreamChunkRate * 0.1)
//   {
//      EV << "Min throughput = " << minThroughput << " -- but not too bad!" << endl;
//      return;
//   }

   if (address_minThroughput == IPvXAddress("10.0.0.9"))
   {
      EV << "Something wrong, partnershipSize = " << m_partnerList->getSize() << endl;
   }

   // -------------------------- Testing ---------------------------------------


/*
    std::map<IPvXAddress, DataExchange> list_exchange;
    for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
         iter != m_partnerList->m_map.end(); ++iter)
    {
       IPvXAddress address = iter->first;
       RecordCountChunk record;
       DataExchange de_total;

//       IPvXAddress bad_ip("0.0.0.2");
//       if (address == bad_ip)
//       {
//          EV << "Strange address " << address << " aloha" << endl;
//       }

       m_forwarder->getRecordChunk(address, record);
       record.print();

       de_total.m_time = record.m_oriTime;

       // -- Calculate and extract the information into another list of structures
       EV << "Partner " << address << endl
          << "\t from time " << record.m_oriTime << endl;
       if (record.m_oriTime != -1.0)
       {
          long int totalChunkExchanged = record.m_chunkReceived + record.m_chunkSent;
          double duration = simTime().dbl() - record.m_oriTime;

          long int count_chunkSent_interval = record.m_chunkSent - record.m_prev_chunkSent;
          record.m_prev_chunkSent = record.m_chunkSent;

          long int count_chunkReceived_interval = record.m_chunkReceived - record.m_prev_chunkReceived;
          record.m_prev_chunkReceived = record.m_chunkReceived;

          long int total_chunkExchanged_interval = count_chunkReceived_interval + count_chunkSent_interval;

          //long int interval_chunkSent =
          if (duration != 0.0)
          {
//             EV << "Average data exchanged with peer " << address << " : (chunks/s)" << endl;
//             EV << "\t Outgoing: " << (long double) record.m_chunkSent / duration << endl;
//             EV << "\t Incoming: " << (long double) record.m_chunkReceived / duration << endl;
//             EV << "\t Total: " << (long double)totalChunkExchanged / duration << endl;

             de_total.m_throughput = (double)totalChunkExchanged / duration;
             de_total.m_throughput_interval = (double)total_chunkExchanged_interval / duration;

             list_exchange.insert(std::pair<IPvXAddress, DataExchange>(address, de_total));
          }
          else
          {
             de_total.m_throughput = 0.0;
          }
       } // if != -1.0
       else
       {
          EV << "No data exchanged with peer " << address << " yet!" << endl;
          de_total.m_throughput = 0.0;
          de_total.m_throughput_interval = 0.0;

          list_exchange.insert(std::pair<IPvXAddress, DataExchange>(address, de_total));
       }
       EV << "\t throughput " << de_total.m_throughput << endl;
    } // for std::map<IPvXAddress, NeighborInfo>

    // -- debug
    // -- Print the extracted list
    EV << "Print list_exchange:" << endl;
    for (std::map<IPvXAddress, DataExchange>::iterator iter = list_exchange.begin();
         iter != list_exchange.end(); ++iter)
    {
       EV << " --- " << "Address: " << iter->first
          << " - Time: " << iter->second.m_time
          << " - Throughput: " << iter->second.m_throughput
          << " - Throughput per interval: " << iter->second.m_throughput_interval << endl;
    }
*/

/*
   IPvXAddress address_minThroughput = IPvXAddress("10.0.0.9");
   double minThroughput = (double)INT_MAX;
   double time_minThroughput = 0.0;
    // -- Find the mininum value of throughputs
    for (std::map<IPvXAddress, DataExchange>::iterator iter = list_exchange.begin();
         iter != list_exchange.end(); ++iter)
    {
       EV << "m_throughput = " << iter->second.m_throughput
          << " -- address = " << iter->first
          << " -- time = " << iter->second.m_time << endl;

       if (minThroughput > iter->second.m_throughput)
       {
          minThroughput         = iter->second.m_throughput;
          address_minThroughput = iter->first;
          time_minThroughput    = iter->second.m_time;

          EV << address_minThroughput << " -- " << minThroughput << endl;
       }
    } // for

    EV << "Partner " << address_minThroughput << endl
       << "\t with min throughput " << minThroughput << endl
       << "\t from time " << time_minThroughput << endl;

    if (minThroughput > m_videoStreamChunkRate * 0.1)
    {
       EV << "Min throughput = " << minThroughput << " -- but not too bad!" << endl;
       return;
    }

    if (address_minThroughput == IPvXAddress("10.0.0.9"))
    {
       EV << "Something wrong, partnershipSize = " << m_partnerList->getSize() << endl;
    }
*/

//    if (time_minThroughput != -1.0)
//    {
//       if ((simTime() - time_minThroughput) > 10.0 /*seconds*/)
//       {
//          EV << "Partner to be removed: " << address_minThroughput << " hehehe" << endl;

//          if (m_partnerList->getSize() > 1)
//          {
//             m_partnerList->deleteAddress(address_minThroughput);
//             m_forwarder->removeRecord(address_minThroughput);

//             MeshPartnershipLeavePacket *leavePkt = new MeshPartnershipLeavePacket("MESH_PEER_LEAVE_PARTNER");
//             leavePkt->setBitLength(m_appSetting->getPacketSizePartnershipLeave());
//             sendToDispatcher(leavePkt, m_localPort, address_minThroughput, m_destPort);
//          }
//          else
//          {
//             EV << "Only one partner left --> no removal at all" << endl;
//          }
//       }
//       else
//       {
//          EV << "The idle period is not long enough" << endl;
//       }
//    }
//    else
//    {
//       EV << "First time to inquire the record for address " << address_minThroughput << endl;
//    }
}

void DonetPeer::handleTimerPartnerlistCleanup()
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

         EV << "Partner " << iter->first << " was deleted" << endl;
      }
   }

   EV << "Number of partners to be removed " << removeAddressList.size() << endl;

   // Delete the list of addresses
   for (std::vector<IPvXAddress>::iterator iter = removeAddressList.begin();
        iter != removeAddressList.end(); ++iter)
   {
      m_partnerList->deleteAddress(*iter);
   }

   if (m_partnerList->getSize() == 0)
   {
      EV << "This node has no partner (isolated) --> should rejoin the overlay" << endl;
      m_apTable->removeAddress(getNodeAddress());
      m_state = MESH_JOIN_STATE_IDLE;
      cancelAllTimer();
      m_player->scheduleStopPlayer();

      double rejoinOffset = dblrand();
      EV << "Node will rejoin after " << rejoinOffset << " seconds" << endl;
      scheduleAt(simTime() + rejoinOffset, timer_join);
   }

}

void DonetPeer::handleTimerTimeoutWaitingAccept()
{
    EV << endl;
    EV << "------------------------- Handle timeout_waitingAccept --------------" << endl;

    emit(sig_timeout, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        // -- Update the state
        EV << "State changes from MESH_JOIN_STATE_IDLE_WAITING to MESH_JOIN_STATE_IDLE" << endl;
        m_state = MESH_JOIN_STATE_IDLE;

        // -- Schedule for new JOIN
        // this event, in turn, triggers findPartner()
        scheduleAt(simTime() + uniform(0.0, 2.0), timer_join);
        //scheduleAt(simTime(), timer_join);
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Process pending requests
        // There should not be any pending request at this time
//        vector<PendingPartnershipRequest>::iterator iter = m_list_partnershipRequestingNode.begin();
//        for (; iter != m_list_partnershipRequestingNode.end(); ++iter)
//        {
//            considerAcceptPartner(*iter);
//        }
//        m_list_partnershipRequestingNode.clear();

        EV << "State changes to MESH_JOIN_STATE_ACTIVEG" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        //EV << "Timeout happens when the peer is in JOINED state" << endl;
        throw cException("Timeout happens when the peer is in JOINED state");
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        //EV << "Timeout happens when the peer is in IDLE state" << endl;
        throw cException("Timeout happens when the peer is in IDLE state");
        break;
    }
    default:
    {
        throw cException("Uncovered state");
        break;
    }
    } // switch()
}

//void DonetPeer::handleTimerReport()
//{

//}

void DonetPeer::handleTimerReportStatistic()
{
   //if (m_player->playerStarted() == true)
   if (m_player->getState() == PLAYER_STATE_PLAYING)
   {
      //long m_count_chunkHit = m_player->getCountChunkHit();
      //long int delta = m_count_chunkHit - m_count_prev_chunkHit;
      long int delta = m_player->getCountChunkHit() - m_count_prev_chunkHit;
      m_count_prev_chunkHit = m_player->getCountChunkHit();
      m_gstat->increaseChunkHit((int)delta);
      EV << "Got it: " << delta << endl;

      //long m_count_chunkMiss = m_player->getCountChunkMiss();
      //delta = m_count_chunkMiss - m_count_prev_chunkMiss;
      delta = m_player->getCountChunkMiss() - m_count_prev_chunkMiss;
      m_count_prev_chunkMiss = m_player->getCountChunkMiss();
      m_gstat->increaseChunkMiss((int)delta);
      EV << "Got it, 2: " << delta << endl;
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
    default:
    {
        throw cException("Unrecognized packet types! %d", meshMsg->getPacketType());
        break;
    }
    } // switch

    delete pkt;
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
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
       // -- Store the address of acceptor into its partner list
       if (acceptor.address != address_responseExpected)
       {
          EV << "IP address of acceptor: " <<  acceptor.address.str().c_str() << endl;
          EV << "IP address expected: " << address_responseExpected.str().c_str() << endl;
       }
//           throw cException("ACK from %s, expected IP: %s. Strange behavior!",
//           acceptor.address.str().c_str(),
//           address_responseExpected.str().c_str());
        //m_partnerList->addAddress(acceptor.address, acceptor.upBW);
        addPartner(acceptor.address, acceptor.upBW);

        EV << "First accept response from " << acceptor.address << endl;

        // -- Register itself to the APT
        //m_apTable->addPeerAddress(getNodeAddress(), param_maxNOP);
        m_apTable->addAddress(getNodeAddress());
//        m_apTable->printActivePeerInfo(getNodeAddress());
//        m_apTable->printActivePeerTable();

        m_memManager->addPeerAddress(getNodeAddress(), param_maxNOP);

        m_firstJoinTime = simTime().dbl();

        // -- Cancel timer
//        if (timer_timeout_waiting_accept) cancelAndDelete(timer_timeout_waiting_accept);
//        timer_timeout_waiting_accept = NULL;
        cancelEvent(timer_timeout_waiting_response);

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
        m_player->activate();
        //scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);

        // -- 5. Partnership refinement
        scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);

        // -- 6. Partnership cleanup
        cancelEvent(timer_partnerListCleanup);
        scheduleAt(simTime() + param_interval_partnerlistCleanup, timer_partnerListCleanup);

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
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Store the address of acceptor into its partner list
//        if (acceptor.address != address_responseExpected)
//        {
//           // Not the ACK from expected peer
//           // State should remain
//           break;
//        }
        //m_partnerList->addAddress(acceptor.address, acceptor.upBW);

        if (m_partnerList->getSize() >= param_maxNOP)
           break;

        EV << "An accept response from " << acceptor.address << endl;

        addPartner(acceptor.address, acceptor.upBW);

        //m_apTable->incrementNPartner(getNodeAddress());
        m_memManager->incrementNPartner(getNodeAddress());

        // -- Cancel timer
//        if (timer_timeout_waiting_accept) cancelAndDelete(timer_timeout_waiting_accept);
//        timer_timeout_waiting_accept = NULL;
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from MESH_JOIN_STATE_ACTIVE_WAITING to MESH_JOIN_STATE_ACTIVE_WAITING" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        EV << "ACCEPT message arrived too late, will be ignored!" << endl;

        // -- State remains
//        m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        EV << "ACCEPT message arrived too late, will be ignored!" << endl;

        // -- State remains
//        m_state = MESH_JOIN_STATE_IDLE;
        break;
    }
    default:
    {
        throw cException("Uncovered state, check assignment of state variable!");
        break;
    }
    } // switch()

}


//void DonetPeer::processRejectResponse(cPacket *pkt)
//{
//    EV << endl;
//    EV << "---------- Process partnership reject -------------------------------" << endl;

//    // -- Extract the address of the accepter
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress rejectorAddress = controlInfo->getSrcAddr();
//    EV << "Rejector's address: " << rejectorAddress << endl;

////    m_activityLog << "--- processRejectResponse --- " << endl;
////    m_activityLog << "\t at " << simTime().dbl() << " Got a REJECT from: " << rejectorAddress << endl;

//    // TODO-Giang: has to verify this code, the logic seems not very clear
//    ++m_nRejectSent;
//    EV << "-- Number of rejected requests so far: " << m_nRejectSent << endl;

//    if (m_nRejectSent == m_nRequestSent)
//    {
////        m_activityLog << "\tAll requests were rejected --> join() again!" << endl;

//        EV << "m_nRejectSent == m_nRequestSent --> rejoin()!" << endl;

//        // Reset state-variables
//        m_nRequestSent = -1;
//        m_nRejectSent = 0;
//        join();
//    }

//    emit (sig_pRejectReceived, m_nRejectSent);

//}


void DonetPeer::processPartnershipReject(cPacket *pkt)
{
    EV << endl;
    EV << "-------- Process partnership Reject ---------------------------------" << endl;

    emit(sig_pRejectRecv, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        EV << "Should find another partner NOW ..." << endl;
        // -- Cancel timer
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from IDLE_WAITING to IDLE" << endl;
        m_state = MESH_JOIN_STATE_IDLE;

        scheduleAt(simTime() + uniform(0, 2.0), timer_join);
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        EV << "Try to contact another peer, later on ..." << endl;
        // -- Cancel timer
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from MESH_JOIN_STATE_ACTIVE_WAITING to MESH_JOIN_STATE_ACTIVE" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        EV << "REJECT message arrived too late, but no problem because of timeout, will be ignored!" << endl;

        // -- State remains
        break;
    }
    case MESH_JOIN_STATE_IDLE:
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


    // -- Extract the address of the accepter
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress rejectorAddress = controlInfo->getSrcAddr();
//    EV << "Rejector's address: " << rejectorAddress << endl;

    //emit (sig_pRejectReceived, m_nRejectSent);
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

    if (m_partnerList->hasAddress(senderAddress) == false)
    {
       EV << "-- Buffer Map is received from a non-partner peer!" << endl;
       return;
    }


    NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

    EV << "-- Buffer Map information: " << endl;
    EV << "  -- Start:\t"   << bmPkt->getBmStartSeqNum()    << endl;
    EV << "  -- End:\t"     << bmPkt->getBmEndSeqNum()      << endl;
    EV << "  -- Head:\t"    << bmPkt->getHeadSeqNum()       << endl;

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
bool DonetPeer::findPartner()
{
   Enter_Method("findPartner()");

   // Init
   address_responseExpected = IPvXAddress("0.0.0.0");

    if (m_partnerList->getSize() >= param_minNOP)
    {
        EV << "Minimum number of partners has been reach --> stop finding more partner for the moment." << endl;
        return false;
    }

    bool ret = sendPartnershipRequest();
    return ret;

//    return true;
}

bool DonetPeer::sendPartnershipRequest(void)
{
    IPvXAddress addressRandPeer = getNodeAddress();
    short count = 0;
    do
    {
       count++;
       addressRandPeer = m_memManager->getRandomPeer(getNodeAddress());
       if (count > 10)
          return false;
    }
    while(m_partnerList->have(addressRandPeer));

    if (m_partnerList->have(addressRandPeer))
    {
        EV << "The peer has already been node's partner. No more request will be sent out." << endl;
        return false;
    }

    EV << "Peer " << addressRandPeer << " will be requested!" << endl;

    MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
        reqPkt->setUpBw(param_upBw);
        reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

    sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);
    address_responseExpected = addressRandPeer;

    emit(sig_pRequestSent, 1);
    return true;
}


//void DonetPeer::updateDataExchangeRecord(void)
void DonetPeer::updateDataExchangeRecord(double samplingInterval)
{
   Enter_Method("updateDataExchangeRecord()");

   EV << "--updateDataExchangeRecord--" << endl;

   RecordCountChunk record;

   for(std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
       iter != m_partnerList->m_map.end(); ++iter)
   {
//      IPvXAddress addr = iter->first;
//      long int currentCountReceived, currentCountSent;
//      m_forwarder->getRecordChunk(addr, currentCountReceived, currentCountSent);

//      // Update the received chunk count
//      long int prevCount = iter->second->getCountChunkReceived();
//      iter->second->setCountChunkReceived(currentCountReceived-prevCount);

//      // Update the sent chunk count
//      prevCount = iter->second->getCountChunkSent();
//      iter->second->setCountChunkSent(currentCountSent-prevCount);

      IPvXAddress addr = iter->first;
      //m_forwarder->getRecordChunk(addr, record);
      record = m_forwarder->getRecordChunk(addr);
      double duration = simTime().dbl() - record.m_oriTime;

      if (duration > 0)
      {
         // -- update the data exchange since joining
         iter->second.setAverageChunkReceived(record.m_chunkReceived / duration);
         iter->second.setAverageChunkSent(record.m_chunkSent / duration);
         iter->second.setAverageChunkExchanged((record.m_chunkSent + record.m_chunkReceived) / duration);

         // -- update the data exchange per interval
         long int chunkSent_diff = record.m_chunkSent - iter->second.getCountPrevChunkSent();
         long int chunkReceived_diff = record.m_chunkReceived - iter->second.getCountPrevChunkReceived();

         iter->second.setAverageChunkSentPerInterval(chunkSent_diff / samplingInterval);
         iter->second.setAverageChunkReceivedPerInterval(chunkReceived_diff / samplingInterval);
         iter->second.setAverageChunkExchangedPerInterval((chunkSent_diff + chunkReceived_diff) / samplingInterval);

         // -- Move value the to store it
         iter->second.setCountPrevChunkSent(iter->second.getCountChunkSent());
         iter->second.setCountPrevChunkReceived(iter->second.getCountChunkReceived());

         iter->second.setCountChunkSent(record.m_chunkSent);
         iter->second.setCountChunkReceived(record.m_chunkReceived);

         // -- Debug
         EV << "Previous -- chunk sent: " << iter->second.getCountPrevChunkSent()
               << " -- chunk received: " << iter->second.getCountPrevChunkReceived() << endl;
         EV << "Current -- chunk sent: " << iter->second.getCountChunkSent()
               << " -- chunk received: " << iter->second.getCountChunkReceived() << endl;
         EV << "Average since joining -- chunk sent: " << iter->second.getAverageChunkSent()
               << " -- chunk received: " << iter->second.getAverageChunkReceived()
               << " -- chunk exchanged: " << iter->second.getAverageChunkExchanged() << endl;
         EV << "Average per interval: -- chunk sent: " << iter->second.getAverageChunkSentPerInterval()
               << " -- chunk received: " << iter->second.getAverageChunkReceivedPerInterval()
               << " -- chunk exchanged: " << iter->second.getAverageChunkExchangedPerInterval() << endl;
      }
      else if (duration == 0)
      {
         EV << "First record to be stored" << endl;
      }
      else
      {
         throw cException("Interval has to be positive");
      }


   } // for
}

int DonetPeer::initializeSchedulingWindow()
{
   // Browse through all partners and find an optimal scheduling window
   SEQUENCE_NUMBER_T min_head = 0L, max_start = 0L;

   std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
   min_head = iter->second.getSeqNumRecvBmHead();
   max_start  = iter->second.getSeqNumRecvBmStart();

   EV << "number of partners = " << m_partnerList->m_map.size() << endl;

   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
   {
      SEQUENCE_NUMBER_T temp = 0L;
      temp = iter->second.getSeqNumRecvBmHead();
      EV << "head_tempt = " << temp << endl;
      if (min_head != 0L && temp != 0L)
         min_head=(min_head > temp) ? temp : min_head;
      else
         min_head = std::max(temp, min_head);

      temp = iter->second.getSeqNumRecvBmStart();
      EV << "start_temp = " << temp << endl;
      max_start=(max_start < temp)?temp:max_start;
   }

   EV << "min_head = " << min_head << " -- max_start = " << max_start << endl;

   //if (min_head > 0L && max_start != 0L)
   if (min_head > 0L)
   {
      m_sched_window.start = (max_start + min_head) / 2;
      m_sched_window.end   = m_sched_window.start + m_bufferMapSize_chunk - 1;
      EV << "Scheduling window [start, end] = " << m_sched_window.start << " - " << m_sched_window.end << endl;

      m_videoBuffer->initializeRangeVideoBuffer(m_sched_window.start);

      return INIT_SCHED_WIN_GOOD;
   }

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

/*
bool DonetPeer::should_be_requested(void)
{
    EV << "Number of chunks requested so far per cycle: " \
            << m_nChunkRequested_perSchedulingInterval << endl;
    if (m_nChunkRequested_perSchedulingInterval >= numberOfChunkToRequestPerCycle())
        return false;

    return true;
}
*/

bool DonetPeer::should_be_requested(SEQUENCE_NUMBER_T seq_num)
{
    // -- Check if the chunk has been recently requested
    if (recentlyRequestedChunk(seq_num) == true)
    {
        return false;
    }

    SEQUENCE_NUMBER_T current_playbackPoint = m_player->getCurrentPlaybackPoint();
    if (seq_num < current_playbackPoint)
    {
        EV << "-- Chunk " << seq_num << " is behind play-back point " << current_playbackPoint << endl;
        return false;
    }

    if (m_nChunkRequested_perSchedulingInterval > m_downloadRate_chunk)
    {
       return false;
    }

    return true;
}

/*
bool DonetPeer::should_be_requested(SEQUENCE_NUMBER_T seq_num)
{
    EV << endl;
    EV << "%%%" << endl;
    EV << "Number of chunks requested in this cycle: " \
       << m_nChunkRequested_perSchedulingInterval << endl;

    // -- Check if already requested too many chunks
//    if (m_nChunkRequested_perSchedulingInterval >= numberOfChunkToRequestPerCycle())
//        return false;

    int nChunkPerCycle = numberOfChunkToRequestPerCycle();
    EV << "numberOfChunkToRequestPerCycle(): " << nChunkPerCycle << endl;

    if (numberOfChunkToRequestPerCycle() > 0)
    {
        // -- Case of limited requests
        EV << "-- Limited number of chunks to request" << endl;
        if (m_nChunkRequested_perSchedulingInterval >= nChunkPerCycle)
        {
            EV << "-- Limit was exceeded" << endl;
            return false;
        }
    }
    else
    {
        EV << "-- Unlimited number of chunks to request" << endl;
    }

    // -- Check if the chunk has been recently requested
    if (recentlyRequestedChunk(seq_num) == true)
    {
        return false;
    }

    SEQUENCE_NUMBER_T current_playbackPoint = m_player->getCurrentPlaybackPoint();
    if (seq_num < current_playbackPoint)
    {
        EV << "-- Chunk " << seq_num << " is behind play-back point " << current_playbackPoint << endl;
        return false;
    }

    return true;
}
*/


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

//    if (m_seqNum_schedWinHead <= 0)
//    {
//        EV << "Scheduling Window is empty, exiting from chunk scheduling" << endl;
//        return;
//    }

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

    // -------------------------------------------------------------------------
    // --- Show the statistics on data exchange with partners
    // -------------------------------------------------------------------------
//    EV << "*********************************************************************" << endl;
//    std::map<IPvXAddress, NeighborInfo>::iterator iter;
//    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
//    {
//       IPvXAddress address = iter->first;
//       RecordCountChunk record;

//       m_forwarder->getRecordChunk(address, record);
//       if (record.m_oriTime != -1.0)
//       {
//          long int totalChunkExchanged = record.m_chunkReceived + record.m_chunkSent;
//          double duration = simTime().dbl() - record.m_oriTime;

//          if (duration != 0.0)
//          {
//             EV << "Average data exchanged with peer " << address << " : (chunks/s)" << endl;
//             EV << "\t Outgoing: " << (long double) record.m_chunkSent / duration << endl;
//             EV << "\t Incoming: " << (long double) record.m_chunkReceived / duration << endl;
//             EV << "\t Total: " << (long double)totalChunkExchanged / duration << endl;
//          }
//       } // if != -1.0
//       else
//       {
//          EV << "No data exchanged with peer " << address << " yet!" << endl;
//       }
//    } // for
//    EV << "*********************************************************************" << endl;

    // -- Update the range variables (for statistics collection)
    updateRange();

    EV << "Partners of " << getNodeAddress() << ": ";
    for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
         iter != m_partnerList->m_map.end(); ++iter)
    {
       EV << iter->first << " -- ";
    }
    EV << endl;

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

//    randomChunkScheduling();
        donetChunkScheduling();
}

void DonetPeer::reportLocalStatistic(void)
{
    // -- Report ranges
    emit(sig_minStart, m_minStart);
    emit(sig_maxStart, m_maxStart);
    emit(sig_minHead, m_minHead);
    emit(sig_maxHead, m_maxHead);
    emit(sig_currentPlaybackPoint, m_player->getCurrentPlaybackPoint());
    emit(sig_bufferStart, m_videoBuffer->getBufferStartSeqNum());
    emit(sig_bufferHead, m_videoBuffer->getHeadReceivedSeqNum());


    // -- Statistics from Player
    long int nHit = m_player->getCountChunkHit();
    long int nMiss = m_player->getCountChunkMiss();

    if ((nHit + nMiss) == 0)
    {
       emit (sig_localCI, 0);
    }
    else
    {
       emit(sig_localCI, (long double)nHit / (nHit + nMiss));
    }
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

int DonetPeer::numberOfChunkToRequestPerCycle(void)
{
    int ret = 0;
    if (m_player->playerStarted() == false)
    {
        // -- Chunks will be requested orderly to avoid congestion at requested peer
        // -- Trade-off is longer startup delays

        ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_moderate);
    }
    else if (inScarityState() == true)
    {
        // -- In "sensitive" state, since the play might soon be stalled because of no chunks in the buffer

        ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_aggresive);
    }
    else
    {
        // -- Request chunks "freely", since there would be not so many chunks available anyway

        ret = -1; // Unlimited
    }

    //EV << "Number of chunk to request per scheduling cycle: " << ret << endl;

    return ret;
}

bool DonetPeer::recentlyRequestedChunk(SEQUENCE_NUMBER_T seq_num)
{
//    EV << endl;
//    EV << "%%%" << endl;
    vector<SEQUENCE_NUMBER_T>::iterator it;
    it = find (m_list_requestedChunk.begin(), m_list_requestedChunk.end(), seq_num);

    if (it == m_list_requestedChunk.end())
    {
//        EV << "Chunk " << seq_num << " has NOT been requested recently" << endl;
        return false;
    }
    else
    {
//        EV << "Chunk " << seq_num << " has been requested recently" << endl;
    }

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

bool DonetPeer::inScarityState(void)
{
    int offset = m_videoBuffer->getHeadReceivedSeqNum() - m_player->getCurrentPlaybackPoint();

    if (offset < (int)(m_videoStreamChunkRate * param_threshold_scarity))
        return true;

    return false;
}

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
