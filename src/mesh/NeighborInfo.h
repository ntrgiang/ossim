/*
 * NeighborInfo.h
 *
 *  Created on: May 3, 2012
 *      Author: giang
 */

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

    inline void setCountChunkReceived(long int n) { m_count_chunkReceived = n; }
    inline long int getCountChunkReceived(void) { return m_count_chunkReceived; }

    inline void setCountChunkSent(long int n) { m_count_chunkSent = n; }
    inline long int getCountChunkSent(void) { return m_count_chunkSent; }

    inline void setAverageChunkSent(double c) { m_average_chunkSent = c; }
    inline double getAverageChunkSent(void) { return m_average_chunkSent; }

    inline void setAverageChunkReceived(double c) { m_average_chunkReceived = c; }
    inline double getAverageChunkReceived(void) { return m_average_chunkReceived; }

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
    void updateChunkAvailTime(SEQUENCE_NUMBER_T seq_num, double time);
    double getChunkAvailTime(SEQUENCE_NUMBER_T seq_num);

    // -- For debugging --
    void printRecvBm(void);
    void printSendBm(void);

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
    long int m_count_chunkSent;     // sent to that partner
    long int m_count_chunkReceived; // received from that partner
    double m_average_chunkSent;     // per unit time
    double m_average_chunkReceived;  // per unit time

    // -- Available time to transmit a specific chunk
    std::vector<double> m_availTime;
    // -- The sequence number of the first element of the list
    SEQUENCE_NUMBER_T m_winStart;
};


#endif /* NEIGHBORINFO_H_ */
