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
// PlayerSimpleSkip.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "PlayerSimpleSkip.h"

Define_Module(PlayerSimpleSkip)

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "::" << getFullName() << ": "
#endif

PlayerSimpleSkip::PlayerSimpleSkip() {
   // TODO Auto-generated constructor stub
}

PlayerSimpleSkip::~PlayerSimpleSkip()
{
   if (timer_nextChunk != NULL)
   {
      delete cancelEvent(timer_nextChunk);
      timer_nextChunk = NULL;
   }

   if (timer_playerStart)
   {
      cancelAndDelete(timer_playerStart);
   }

   if (timer_playerStop)
   {
      cancelAndDelete(timer_playerStop);
   }

   //if (timer_reportStatistic) cancelAndDelete(timer_reportStatistic);
}

void PlayerSimpleSkip::initialize(int stage)
{
   if (stage == 0)
   {
      m_debug = (hasPar("debug")) ? par("debug").boolValue() : false;

      sig_chunkHit            = registerSignal("Signal_ChunkHit");
      sig_chunkMiss           = registerSignal("Signal_ChunkMiss");
      sig_chunkSeek           = registerSignal("Signal_ChunkSeek");
      sig_rebuffering_local   = registerSignal("rebuffering_Local");
      sig_rebuffering         = registerSignal("Signal_Rebuffering");
      sig_stall               = registerSignal("Signal_Stall");
      return;
   }

   if (stage != 3)
      return;

   // -- pointing to the Video Buffer
   cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
   m_videoBuffer = check_and_cast<VideoBuffer *>(temp);

   temp = simulation.getModuleByPath("appSetting");
   m_appSetting = check_and_cast<AppSettingDonet *>(temp);

   temp = simulation.getModuleByPath("globalStatistic");
   //m_stat = check_and_cast<StatisticBase *>(temp);
   //m_stat = check_and_cast<StreamingStatistic *>(temp);

   timer_nextChunk     = new cMessage("PLAYER_TIMER_NEXT_CHUNK");
   timer_playerStart   = new cMessage("PLAYER_TIMER_START");
   timer_playerStop    = new cMessage("PLAYER_TIMER_STOP");
   //timer_reportStatistic = new cMessage("PLAYER_TIMER_REPORT_STATISTIC");

   // -- Reading parameters from module itself
   param_interval_recheckVideoBuffer = par("interval_recheckVideoBuffer");
   param_interval_reportStatistic = par("interval_reportStatistic");

   // -- for the FSM
   param_percent_buffer_low = par("percent_buffer_low").doubleValue();
   param_percent_buffer_high = par("percent_buffer_high").doubleValue();
   param_max_skipped_chunk = (int) par("max_skipped_chunk").doubleValue();
   param_interval_probe_playerStart = par("interval_probe_playerStart").doubleValue();
   m_state = PLAYER_STATE_IDLE;
   m_skip = 0;

   m_interval_newChunk = m_appSetting->getIntervalNewChunk();
   m_chunkSize  = m_appSetting->getChunkSize();

   // -- State variables
   m_playerStarted = false;
   m_id_nextChunk = -1L;

   // -- Continuity Index
   m_count_chunkHit = 0L;
   m_count_chunkMiss = 0L;

   m_count_prev_chunkHit = 0L;
   m_count_prev_chunkMiss = 0L;

   // -- Schedule the first event for the first chunk
   //    scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newChunk);

   // -------------------------------------------------------------------------
   // Signals
   // -------------------------------------------------------------------------
   sig_timePlayerStart = registerSignal("Signal_timePlayerStart");

   WATCH(m_videoBuffer);
   WATCH(m_appSetting);
   WATCH(m_chunkSize);
   WATCH(m_interval_newChunk);

}

void PlayerSimpleSkip::activate(void)
{
   Enter_Method("activate()");

   EV << "Player (Simple skip) activated!" << endl;

   if (m_state != PLAYER_STATE_IDLE)
   {
      throw cException("Wrong Player state %d (PLAYER_STATE_BUFFERING) while expecting %d (PLAYER_STATE_IDLE)",
                       m_state, PLAYER_STATE_IDLE);
   }

   m_state = PLAYER_STATE_BUFFERING;
   cancelEvent(timer_playerStart);
   scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);

   debugOUT << "activated" << endl;
}

void PlayerSimpleSkip::finish()
{

}

void PlayerSimpleSkip::handleMessage(cMessage *msg)
{
   Enter_Method("handleTimerMessage()");

   if (!msg->isSelfMessage())
   {
      throw cException("This module does NOT process external messages!");
      return;
   }

   handleTimerMessage(msg);
}

void PlayerSimpleSkip::handleTimerMessage(cMessage *msg)
{
   Enter_Method("handleTimerMessage()");

   EV << "Handle Timer Message" << endl;

   if (msg == timer_playerStart)
   {
      EV << "Timer_playerStart" << endl;

      switch (m_state)
      {
      case PLAYER_STATE_BUFFERING:
      {
         if (m_videoBuffer->getPercentFill() < param_percent_buffer_high)
         {
            EV << "Buffer filled not enough, should wait more!" << endl;

            // Probe the status of the buffer again
            scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);
         }
         else
         {
            // -- Change state to PLAYING
            m_state = PLAYER_STATE_PLAYING;
            m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();

            m_videoBuffer->printRange();

            debugOUT << "Player starts from chunk " << m_id_nextChunk << endl;

            //            if (m_id_nextChunk <= m_videoBuffer->getBufferStartSeqNum())
            //               m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();
            //            else if (m_id_nextChunk > m_videoBuffer->getBufferEndSeqNum())
            //               throw cException("Expected sequence number %ld is out of range [%ld, %ld]",
            //                                m_id_nextChunk,
            //                                m_videoBuffer->getBufferStartSeqNum(),
            //                                m_videoBuffer->getBufferEndSeqNum());

            emit(sig_timePlayerStart, simTime().dbl());

            scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

            // -- Schedule to report chunkHit, chunkMiss
            //scheduleAt(simTime() + param_interval_reportStatistic, timer_reportStatistic);
         }
         break;
      }
      default:
      {
         throw cException("Wrong state %d while expecting %d", m_state, PLAYER_STATE_BUFFERING);
      }
      }
   }
   else if (msg == timer_nextChunk)
   {
      if (m_videoBuffer->inBuffer(m_id_nextChunk))
      {
         ++m_id_nextChunk;
         scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

         ++m_count_chunkHit;
      }
      else // expected chunk is NOT in buffer
      {
         ++m_id_nextChunk;
         scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

         ++m_count_chunkMiss;
      } // else ~ chunk not in buffer

   }
   else if (msg == timer_playerStop)
   {
      stopPlayer();
   }
}

void PlayerSimpleSkip::startPlayer()
{
   Enter_Method("startPlayer");

   scheduleAt(simTime(), timer_nextChunk);

   EV << "Player starts with chunk " << m_id_nextChunk << endl;

   m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();

   m_playerStarted = true;

}

void PlayerSimpleSkip::scheduleStopPlayer(void)
{
   Enter_Method("scheduleStopPlayer");

   //double random_period = dblrand();
   //EV << "Player will be stopped after " << random_period << " seconds" << endl;
   //scheduleAt(simTime() + random_period, timer_playerStop);

   EV << "Player will be stopped NOW" << endl;
   scheduleAt(simTime() + 0.0, timer_playerStop);
}

void PlayerSimpleSkip::stopPlayer(void)
{
   Enter_Method("stopPlayer");

   cancelEvent(timer_nextChunk);
   cancelEvent(timer_playerStart);

   m_state = PLAYER_STATE_IDLE;
   m_playerStarted = false;
}

SEQUENCE_NUMBER_T PlayerSimpleSkip::getCurrentPlaybackPoint(void)
{
   return m_id_nextChunk;
}

double PlayerSimpleSkip::getContinuityIndex(void)
{
   long int total_chunkSeek = m_count_chunkHit + m_count_chunkMiss;
   if (total_chunkSeek != 0)
      return m_count_chunkHit / total_chunkSeek;

   return 0.0;
}

double PlayerSimpleSkip::getPercentBufferLow(void)
{
   return param_percent_buffer_low;
}

bool PlayerSimpleSkip::playerStarted(void)
{
   return m_playerStarted;
}

bool PlayerSimpleSkip::shouldResumePlaying(SEQUENCE_NUMBER_T seq_num)
{
   // !!! Asuming that the seq_num is a valid value within the range [id_start, id_end]

   if (m_videoBuffer->getPercentFillAhead(seq_num) >= 0.5)
      return true;

   return false;
}

//double PlayerSimpleSkip::getLocalContinuityIndex(void)
//{
//   long int total_chunkSeek = m_count_chunkHit + m_count_chunkMiss;
//   if (total_chunkSeek != 0)
//      return m_count_chunkHit / total_chunkSeek;

//   return 0.0;
//}
