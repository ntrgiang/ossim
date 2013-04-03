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
// PlayerBase.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"
//#include "GlobalStatistic.h"

#define PLAYER_STATE_IDLE       0
#define PLAYER_STATE_BUFFERING  1
#define PLAYER_STATE_PLAYING    2
#define PLAYER_STATE_STALLED    3

//#define EVENT_CHUNK_NOT_IN_BUFFER   0
//#define EVENT_CHUNK_IN_BUFFER       1

class PlayerBase : public cSimpleModule {
public:
    PlayerBase() {}
    virtual ~PlayerBase() {}

protected:
    virtual void handleMessage(cMessage *msg) = 0;

public:
    virtual void activate(void) = 0;
    virtual void scheduleStopPlayer(void) = 0;

    virtual SEQUENCE_NUMBER_T getCurrentPlaybackPoint(void) = 0;
    virtual bool playerStarted(void) = 0;

    // virtual int getPlayerState(void) = 0;
    virtual double getContinuityIndex(void) = 0;

    inline long int getCountChunkHit(void) { return m_count_chunkHit; }
    inline long int getCountChunkMiss(void) { return m_count_chunkMiss; }

    inline int getState(void) { return m_state; }

protected:
    virtual void startPlayer(void) = 0;
    virtual void stopPlayer(void) = 0;

protected:
    long int m_count_chunkHit, m_count_chunkMiss;
    long int m_count_prev_chunkHit, m_count_prev_chunkMiss;

    double param_interval_reportStatistic;

    //cMessage *timer_reportStatistic;

    // -- State variable
    bool m_playerStarted;
    int m_state;
};

#endif /* PLAYERBASE_H_ */
