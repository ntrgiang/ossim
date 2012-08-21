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

    inline void setTimeBudget(double time) { m_timeBudget = time; }
    inline double getTimeBudget(void) { return m_timeBudget; }

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
    // double _lastKeepAliveTime;
    double m_lastRecvBmTime;
    int m_bufferSize;

    // -- Peers information
    double m_upBw;

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

    // -- Available time to transmit a specific chunk
    std::vector<double> m_availTime;
    // -- The sequence number of the first element of the list
    SEQUENCE_NUMBER_T m_winStart;
};


#endif /* NEIGHBORINFO_H_ */
