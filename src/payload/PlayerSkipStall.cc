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
// Player.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "PlayerSkipStall.h"

Define_Module(PlayerSkipStall)

PlayerSkipStall::PlayerSkipStall() {
    // TODO Auto-generated constructor stub
}

PlayerSkipStall::~PlayerSkipStall()
{
    if (timer_nextChunk != NULL) { delete cancelEvent(timer_nextChunk); timer_nextChunk = NULL; }
    if (timer_playerStart) cancelAndDelete(timer_playerStart);
}

void PlayerSkipStall::initialize(int stage)
{
    if (stage == 0)
    {
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
    //    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
//    if (!m_videoBuffer) throw cException("Null pointer for the VideoBuffer module");

    temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingDonet *>(temp);
    //    m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
//    if (!m_videoBuffer) throw cException("Null pointer for the AppSetting module");

    temp = simulation.getModuleByPath("globalStatistic");
    m_stat = check_and_cast<StreamingStatistic *>(temp);
    //    m_stat = dynamic_cast<GlobalStatistic *>(temp);
//    if (!m_stat) throw cException("Null pointer for the GlobalStatistic module");

    timer_nextChunk     = new cMessage("PLAYER_TIMER_NEXT_CHUNK");
    timer_playerStart   = new cMessage("PLAYER_TIMER_START");

    // -- Reading parameters from module itself
    param_interval_recheckVideoBuffer = par("interval_recheckVideoBuffer");

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
    m_countChunkHit = 0L;
    m_countChunkMiss = 0L;

    // -------------------------------------------------------------------------
    // Signals
    // -------------------------------------------------------------------------
    sig_timePlayerStart = registerSignal("Signal_timePlayerStart");

    // -- Schedule the first event for the first chunk
//    scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newChunk);

    WATCH(m_videoBuffer);
    WATCH(m_appSetting);
    WATCH(m_chunkSize);
    WATCH(m_interval_newChunk);
}

void PlayerSkipStall::activate(void)
{
    Enter_Method("activate()");

    EV << "Player activated" << endl;
    if (m_state != PLAYER_STATE_IDLE)
        throw cException("Wrong Player state %d while expecting %d", m_state, PLAYER_STATE_IDLE);

    m_state = PLAYER_STATE_BUFFERING;
    scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);
}

void PlayerSkipStall::finish()
{

}

void PlayerSkipStall::handleMessage(cMessage *msg)
{
    Enter_Method("handleMessage");

    if (!msg->isSelfMessage())
    {
        throw cException("This module does NOT process external messages!");
        return;
    }

    handleTimerMessage(msg);
}

void PlayerSkipStall::handleTimerMessage(cMessage *msg)
{
    Enter_Method("handleTimerMessage");

    if (msg == timer_playerStart)
    {
        switch (m_state)
        {
        case PLAYER_STATE_BUFFERING:
        {
            if (m_videoBuffer->getPercentFill() < param_percent_buffer_high)
            {
               EV << "*********************************************************" << endl;
               EV << "Buffer filled not enough! Should not start the player now!" << endl;

                // Probe the status of the buffer again
                scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);
            }
            else
            {
                // -- Change state to PLAYING
                m_state = PLAYER_STATE_PLAYING;

                EV << "*********************************************************" << endl;
                EV << "Player starts now" << endl;
                EV << "*********************************************************" << endl;

                // Signal
                emit(sig_timePlayerStart, simTime().dbl());

                if (m_id_nextChunk <= m_videoBuffer->getBufferStartSeqNum())
                    m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();
                else if (m_id_nextChunk > m_videoBuffer->getBufferEndSeqNum())
                    throw cException("Expected sequence number %ld is out of range [%ld, %ld]",
                                     m_id_nextChunk,
                                     m_videoBuffer->getBufferStartSeqNum(),
                                     m_videoBuffer->getBufferEndSeqNum());

                scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
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
            switch (m_state)
            {
            case PLAYER_STATE_PLAYING: // Chunk HIT
            {
                ++m_id_nextChunk;
                scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

                ++m_countChunkHit;

                // -- State remains

                // -- Statistics collection
                emit(sig_chunkHit, m_id_nextChunk);
                emit(sig_chunkSeek, m_id_nextChunk);
//                m_stat->reportChunkHit(m_id_nextChunk);
//                m_stat->reportChunkSeek(m_id_nextChunk);

                break;
            }
            case PLAYER_STATE_STALLED:
            {
                if (m_id_nextChunk < m_videoBuffer->getBufferStartSeqNum() ||
                        m_id_nextChunk > m_videoBuffer->getBufferEndSeqNum())
                throw cException("Expected sequence number %ld is out of range [%ld, %ld]",
                                 m_id_nextChunk,
                                 m_videoBuffer->getBufferStartSeqNum(),
                                 m_videoBuffer->getBufferEndSeqNum());

                if (shouldResumePlaying(m_id_nextChunk))
                {
                    ++m_id_nextChunk;
                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

                    m_state = PLAYER_STATE_PLAYING;
                }
                else
                {
                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
                    //-- State remains
                }
                break;
            }
            default:
            {
                throw cException("Unexpected state %d", m_state);
            }
            }
        }
        else // expected chunk is NOT in buffer
        {
            switch (m_state)
            {
            case PLAYER_STATE_PLAYING: // Chunk MISS
            {
                if (m_skip < param_max_skipped_chunk)
                {
                    ++m_skip;
                    ++m_id_nextChunk;
                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
                    //-- State remains

                    // -- Statistics collection
                    emit(sig_chunkMiss, m_id_nextChunk);
                    emit(sig_chunkSeek, m_id_nextChunk);
//                    m_stat->reportChunkMiss(m_id_nextChunk);
//                    m_stat->reportChunkSeek(m_id_nextChunk);

                    // on-going stuff
//                    m_stat->reportSkipChunk();
                }
                else // Cannot skip any more chunk
                {
                    // -- Reset the counter for skips
                    m_skip = 0;
                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);

                    m_state = PLAYER_STATE_STALLED;

                    // -- Statistics collection
                    emit(sig_stall, 1);
//                    m_stat->reportStall();

                    // -- on-going stuff
//                    m_stat->reportStallDuration();
                }

                ++m_countChunkMiss;
                break;
            }
            case PLAYER_STATE_STALLED:
            {
                if (m_videoBuffer->getPercentFill() >= param_percent_buffer_low)
                {
                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
                    // -- State remains

                    // -- on-going stuff
//                    m_stat->reportStallDuration();
                }
                else // Cannot stall the video any more
                {

                    //scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
                   scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);

                    m_state = PLAYER_STATE_BUFFERING;

                    // -- Statistics collection
                    emit(sig_rebuffering, 1);
//                    m_stat->reportRebuffering();

                    // -- on-going stuff
//                    m_stat->reportRebuffering();
                }
                break;
            }
            default:
            {
                throw cException("Unexpected state %d", m_state);
            }
            }
        }
    } // not in Buffer

//    if (msg == timer_nextChunk)
//    {
//        scheduleAt(simTime() + m_interval_newChunk, timer_nextChunk);

//        emit(sig_chunkSeek, m_id_nextChunk);
//        m_stat->reportChunkSeek(m_id_nextChunk);

//        if (m_videoBuffer->isInBuffer(m_id_nextChunk))
//        {
//            EV << "\tChunk " << m_id_nextChunk << " is in the buffer." << endl;

//            emit(sig_chunkHit, m_id_nextChunk);
//            m_stat->reportChunkHit(m_id_nextChunk);
//        }
//        else
//        {
//            EV << "\tChunk " << m_id_nextChunk << " is NOT in the buffer." << endl;

//            emit(sig_chunkMiss, m_id_nextChunk);
//            m_stat->reportChunkMiss(m_id_nextChunk);

//            if (m_id_nextChunk >= m_videoBuffer->getBufferEndSeqNum())
//            {
//                EV << "\t\tNo more chunk in the buffer to play out --> rebuffering!" << endl;
//                cancelEvent(timer_nextChunk);
//                scheduleAt(simTime() + param_interval_recheckVideoBuffer, timer_nextChunk);

//                // -- recording re-buffering events
//                emit(sig_rebuffering_local, m_id_nextChunk);
//                m_stat->reportRebuffering(m_id_nextChunk);
//            }
//        }

//        ++m_id_nextChunk;

//    }
//    else
//    {
//        throw cException("Wrong timer at Player module!");
//    }
}

void PlayerSkipStall::startPlayer()
{
    Enter_Method("startPlayer");
    scheduleAt(simTime(), timer_nextChunk);

    EV << "Player starts with chunk " << m_id_nextChunk << endl;

    m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();

    m_playerStarted = true;

}

SEQUENCE_NUMBER_T PlayerSkipStall::getCurrentPlaybackPoint(void)
{
    return m_id_nextChunk;
}

bool PlayerSkipStall::playerStarted(void)
{
    return m_playerStarted;
}

bool PlayerSkipStall::shouldResumePlaying(SEQUENCE_NUMBER_T seq_num)
{
    // !!! Asuming that the seq_num is a valid value within the range [id_start, id_end]

    if (m_videoBuffer->getPercentFillAhead(seq_num) >= 0.5)
        return true;

    return false;
}
