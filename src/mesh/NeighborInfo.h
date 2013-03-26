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
// NeighborInfo.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef NEIGHBORINFO_H_
#define NEIGHBORINFO_H_

#include "BufferMap.h"
#include "MeshPeerStreamingPacket_m.h"

class NeighborInfo
{
public:
    NeighborInfo(int bmSize=0);
    virtual ~NeighborInfo();

public:
    // --------- New implementation -----------
    // -- Getter & Setter
    inline SEQUENCE_NUMBER_T getSeqNumRecvBmStart(void) { return m_seqNum_recvBmStart; }
    inline SEQUENCE_NUMBER_T getSeqNumRecvBmEnd(void) { return m_seqNum_recvBmEnd; }
    inline SEQUENCE_NUMBER_T getSeqNumRecvBmHead(void) { return m_seqNum_recvBmHead; }

    inline void setSeqNumRecvBmStart(SEQUENCE_NUMBER_T seq_num) { m_seqNum_recvBmStart = seq_num; }
    inline void setSeqNumRecvBmEnd(SEQUENCE_NUMBER_T seq_num)   { m_seqNum_recvBmEnd = seq_num; }
    inline void setSeqNumRecvBmHead(SEQUENCE_NUMBER_T seq_num)  { m_seqNum_recvBmHead = seq_num; }

    inline void setSeqNumSendBmStart(SEQUENCE_NUMBER_T seq_num) { m_seqNum_sendBmStart = seq_num; }
    inline void setSeqNumSendBmEnd(SEQUENCE_NUMBER_T seq_num)   { m_seqNum_sendBmEnd = seq_num; }
    inline void setSeqNumSendBmHead(SEQUENCE_NUMBER_T seq_num)  { m_seqNum_sendBmHead = seq_num; }

    inline void setLastRecvBmTime(double timeStamp) { m_lastRecvBmTime = timeStamp; }
    inline double getLastRecvBmTime(void) { return m_lastRecvBmTime; }

    inline void setUpBw(double bw) { m_upBw = bw; }
    inline double getUpBw(void) { return m_upBw; }

    inline void setUploadRate_Chunk(int nChunk) { m_uploadRate_chunk = nChunk; }
    inline int getUploadRate_Chunk(void) { return m_uploadRate_chunk; }

    inline int getNChunkScheduled(void) { return m_nChunkScheduled; }
    inline void setNChunkScheduled(int nChunk) { m_nChunkScheduled = nChunk; }

    inline void setTimeBudget(double time) { m_timeBudget = time; }
    inline double getTimeBudget(void) { return m_timeBudget; }

    // -- "current" chunk record
    inline void setCountChunkReceived(long int n) { m_count_chunkReceived = n; }
    inline long int getCountChunkReceived(void) { return m_count_chunkReceived; }

    inline void setCountChunkSent(long int n) { m_count_chunkSent = n; }
    inline long int getCountChunkSent(void) { return m_count_chunkSent; }

    // -- "previous" chunk record
    inline void setCountPrevChunkSent(long int n) { m_count_prev_chunkSent = n; }
    inline long int getCountPrevChunkSent(void) { return m_count_prev_chunkSent; }

    inline void setCountPrevChunkReceived(long int n) { m_count_prev_chunkReceived = n; }
    inline long int getCountPrevChunkReceived(void) { return m_count_prev_chunkReceived; }

    // -- average chunk record since joining
    inline void setAverageChunkSent(double c) { m_average_chunkSent = c; }
    inline double getAverageChunkSent(void) { return m_average_chunkSent; }

    inline void setAverageChunkReceived(double c) { m_average_chunkReceived = c; }
    inline double getAverageChunkReceived(void) { return m_average_chunkReceived; }

    inline void setAverageChunkExchanged(double c) { m_average_chunkExchanged = c; }
    inline double getAverageChunkExchanged(void) { return m_average_chunkExchanged; }

    // -- average chunk record per interval
    inline void setAverageChunkSentPerInterval(double c) { m_average_chunkSent_interval = c; }
    inline double getAverageChunkSentPerInterval() { return m_average_chunkSent_interval; }

    inline void setAverageChunkReceivedPerInterval(double c) { m_average_chunkReceived_interval = c; }
    inline double getAverageChunkReceivedPerInterval() { return m_average_chunkReceived_interval; }

    inline void setAverageChunkExchangedPerInterval(double c) { m_average_chunkExchanged_interval = c; }
    inline double getAverageChunkExchangedPerInterval() { return m_average_chunkExchanged_interval; }

    // ----------------------------------------------------------------------------------------------

    bool isInRecvBufferMap(SEQUENCE_NUMBER_T seq_num);
    void copyFrom(BufferMap *bm);
    void copyFrom(MeshBufferMapPacket *bmPkt);

    void clearSendBm(void);

    void setElementSendBm(SEQUENCE_NUMBER_T seq_num, bool val);
    void copyTo(BufferMap *bm);
    void copyTo(MeshChunkRequestPacket *bmPkt);

    bool isSendBmModified(void);

    void resetVectorAvailableTime(SEQUENCE_NUMBER_T vb_start, SEQUENCE_NUMBER_T win_start, double chunkInterval);
    //void updateChunkAvailTime(SEQUENCE_NUMBER_T seq_num, double time);
    bool updateChunkAvailTime(SEQUENCE_NUMBER_T seq_num, double time);
    double getChunkAvailTime(SEQUENCE_NUMBER_T seq_num);

    // -- For debugging --
    void printRecvBm(void);
    void printSendBm(void);
    void printVectorAvailableTime(void);

// Data member
private:
    double m_lastRecvBmTime;
    int m_bufferSize;

    // -- Peers information
    double m_upBw;
    int m_uploadRate_chunk;

    // -- Buffer to store received buffer map
    std::vector<bool> m_recvBm;
    SEQUENCE_NUMBER_T m_seqNum_recvBmStart;
    SEQUENCE_NUMBER_T m_seqNum_recvBmEnd;
    SEQUENCE_NUMBER_T m_seqNum_recvBmHead;

    // -- Buffer to store map for chunk request
    std::vector<bool> m_sendBm;
    SEQUENCE_NUMBER_T m_seqNum_sendBmStart;
    SEQUENCE_NUMBER_T m_seqNum_sendBmEnd;
    SEQUENCE_NUMBER_T m_seqNum_sendBmHead;

    // -- A flag to check whether the sendBm has been modified
    // if true --> a chunk request should be sent to the partner
    // if false --> no chunk request should be sent to this partner
    bool m_sendBmModified;

    // -- For Donet Chunk scheduling
    double m_timeBudget;

    int m_nChunkScheduled;

    // -- For partnership refinement
    // Number of chunks delivered in both direction between two partners
    long int m_count_chunkSent;     // (current) number of chunks sent to that partner
    long int m_count_chunkReceived; // (current) number of chunks received from that partner

    long int m_count_prev_chunkSent;      // number of chunks sent to partner in previous sampling
    long int m_count_prev_chunkReceived;  // number of chunks received from partner in previous sampling

    // -- per unit time since joining
    double m_average_chunkSent;
    double m_average_chunkReceived;
    double m_average_chunkExchanged;

    double m_average_chunkSent_interval;        // during the previous interval
    double m_average_chunkReceived_interval;    // during the previous interval
    double m_average_chunkExchanged_interval;   // during the previous interval

    // -- Available time to transmit a specific chunk
    std::vector<double> m_availTime;

    // -- The sequence number of the first element of the list
    SEQUENCE_NUMBER_T m_winStart;
};


#endif /* NEIGHBORINFO_H_ */
