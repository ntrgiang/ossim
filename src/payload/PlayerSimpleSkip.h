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
// PlayerSimpleSkip.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef PLAYERSIMPLESKIP_H_
#define PLAYERSIMPLESKIP_H_

#include "PlayerBase.h"

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"
//#include "GlobalStatistic.h"
//#include "DonetStatistic.h"
#include "StreamingStatistic.h"

//#define PLAYER_STATE_IDLE       0
//#define PLAYER_STATE_BUFFERING  1
//#define PLAYER_STATE_PLAYING    2
//#define PLAYER_STATE_STALLED    3

#define EVENT_CHUNK_NOT_IN_BUFFER   0
#define EVENT_CHUNK_IN_BUFFER       1

class PlayerSimpleSkip : public PlayerBase
{
public:
    PlayerSimpleSkip();
    virtual ~PlayerSimpleSkip();

protected:
    int numInitStages() const { return 4; }
    void initialize(int stage);
    void finish();

    void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

public:
    void activate(void);
    void scheduleStopPlayer(void);

    SEQUENCE_NUMBER_T getCurrentPlaybackPoint(void);
    bool playerStarted(void);

    double getContinuityIndex(void);

private:
    void startPlayer(void);
    void stopPlayer(void);
    bool shouldResumePlaying(SEQUENCE_NUMBER_T seq_num);

protected:
    // Pointer to other modules

    // -- Message
    cMessage *timer_nextChunk;
    cMessage *timer_playerStart;
    cMessage *timer_playerStop;

    SEQUENCE_NUMBER_T m_id_nextChunk;

    // -- Module (secondary) parameters
    double param_interval_recheckVideoBuffer;
    double  m_interval_newChunk;
    int     m_chunkSize; // Bytes

    // -- Some new parameters for the Finite State Machine
    double  param_percent_buffer_low, param_percent_buffer_high;
    int     param_max_skipped_chunk;
    double  param_interval_probe_playerStart;

    // Obsolete -- since moved up the PlayerBase class
    // -- Statistics locally
    //long int m_countChunkHit, m_countChunkMiss;[

    int m_skip;

    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    AppSettingDonet *m_appSetting;
    //GlobalStatistic *m_stat;
    //StatisticBase *m_stat;
    //StreamingStatistic *m_stat;

    // -- Signal for data collection
    simsignal_t sig_chunkHit;
    simsignal_t sig_chunkMiss;
    simsignal_t sig_chunkSeek;

    simsignal_t sig_rebuffering_local;
    simsignal_t sig_rebuffering;
    simsignal_t sig_stall;

    simsignal_t sig_timePlayerStart;
};

#endif /* PLAYER_H_ */
