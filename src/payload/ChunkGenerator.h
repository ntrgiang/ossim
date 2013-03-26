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
// ChunkGenerator.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef CHUNKGENERATOR_H_
#define CHUNKGENERATOR_H_

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"

class ChunkGenerator : public cSimpleModule {
public:
    ChunkGenerator();
    virtual ~ChunkGenerator();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

protected:
    //VideoChunkPacket *generateNewVideoChunk(SEQUENCE_NUMBER_T seq_num);

protected:
    cMessage *timer_newChunk;
    SEQUENCE_NUMBER_T m_id_newChunk;

    // -- Module (secondary) parameters
    double m_interval_newChunk;
    int m_size_chunkPacket;

    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    AppSettingDonet *m_appSetting;

    // -- Signal for data collection
    simsignal_t sig_chunkSeqNum;

    // -- Objects for aggregated data
    cTimestampedValue m_tsValue;

};

#endif /* CHUNKGENERATOR_H_ */
