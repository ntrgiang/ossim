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
// VideoBuffer.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

// Debug
#define __VIDEOBUFFER_CROSSCHECK__ true
#define __VIDEO_BUFFER_DEBUG__ false

//#include <queue>
//#include "MeshPeerStreamingPacket_m.h"
//#include "StatisticBase.h"
//#include "GlobalStatistic.h"

#include "VideoChunkPacket_m.h"
#include "AppCommon.h"
#include "BufferMap.h"
#include "StreamingStatistic.h"

// listening support ->
#include "VideoBufferListener.h"
// <- listening support


typedef struct __STRM_BUF_ELEM {
    __STRM_BUF_ELEM() : m_chunk(NULL) {}
    //MeshVideoChunkPacket *m_chunk;
    VideoChunkPacket *m_chunk;
    SEQUENCE_NUMBER_T m_recved_time;
} STREAM_BUFFER_ELEMENT_T;

class VideoBuffer : public cSimpleModule
{
public:
    VideoBuffer();
    virtual ~VideoBuffer();

protected:
  virtual int numInitStages() const  {return 4;}
  virtual void initialize(int stage);
  void finish(void);

  // Raises an error.
  virtual void handleMessage(cMessage *);

public:
    /*
    inline void setID_bufferStart(SEQUENCE_NUMBER_T val) { m_id_bufferStart = val; }
    inline SEQUENCE_NUMBER_T getID_bufferStart() { return m_id_bufferStart; }
    inline void setID_bufferEnd(SEQUENCE_NUMBER_T val) { m_id_bufferEnd = val; }
    inline SEQUENCE_NUMBER_T getID_bufferEnd() { return m_id_bufferEnd; }
    */

    inline SEQUENCE_NUMBER_T getBufferStartSeqNum(void) { return m_bufferStart_seqNum; }
    inline void setBufferStartSeqNum(SEQUENCE_NUMBER_T val) { m_bufferStart_seqNum = val; }

    inline SEQUENCE_NUMBER_T getBufferEndSeqNum(void) { return m_bufferEnd_seqNum; }
    inline void setBufferEndSeqNum(SEQUENCE_NUMBER_T val) { m_bufferEnd_seqNum = val; }

    inline SEQUENCE_NUMBER_T getHeadReceivedSeqNum(void) { return m_head_received_seqNum; }
    inline void setHeadReceivedSeqNum(SEQUENCE_NUMBER_T val) { m_head_received_seqNum = val; }

    inline long int getNChunkReceived(void) { return m_nChunkReceived; }

    double getPercentFill(void);
    int getNumberOfChunkFill(void);

//    inline void setSize(int size) { m_bufferSize_chunk = size; }
    inline int getSize() { return m_bufferSize_chunk; }

    inline double getChunkInterval(void) { return m_chunkInterval; }

//    void insertPacket(MeshVideoChunkPacket *packet, SIM_TIME_T current_time);
//    void insertPacket(MeshVideoChunkPacket *packet);
    //    int getNumberFilledChunk();

    void insertPacket(VideoChunkPacket *packet);
    void insertPacketDirect(VideoChunkPacket *packet);
    VideoChunkPacket *getChunk(SEQUENCE_NUMBER_T seq_num);

    bool isInBuffer(SEQUENCE_NUMBER_T seq_num);
    bool inBuffer(SEQUENCE_NUMBER_T seq_num);
    SIM_TIME_T getReceivedTime(SEQUENCE_NUMBER_T seq_num);
    STREAM_BUFFER_ELEMENT_T & getBufferElement(SEQUENCE_NUMBER_T seq_num);
    void captureVideoBuffer(BufferMap *bm);
    void fillBufferMapPacket(MeshBufferMapPacket *bmPkt);

    bool shouldResumePlaying(SEQUENCE_NUMBER_T seq_num) const;
    int getNumberOfChunkFillAhead(SEQUENCE_NUMBER_T ref_ori);
    double getPercentFillAhead(SEQUENCE_NUMBER_T ref_ori);

    void initializeRangeVideoBuffer(SEQUENCE_NUMBER_T seq);

//    double getDeadline(SEQUENCE_NUMBER_T seq_num) const;

    // Debug
    void printStatus();

    /*
     * Get the number of elements which are attached with VideoPackets
     */
    int getNumberActiveElement(void);

private:
    // -- Pointers to external modules
//    AppSettingDonet *m_appSetting;

    std::vector<STREAM_BUFFER_ELEMENT_T> m_streamBuffer;
    // sequence numbers
    SEQUENCE_NUMBER_T m_bufferStart_seqNum; // staring sequence number of the buffer
    SEQUENCE_NUMBER_T m_bufferEnd_seqNum;   // ending sequence number of the buffer
    SEQUENCE_NUMBER_T m_head_received_seqNum; // largest sequence number of chunks received so far

    // -- Parameters
//    int param_bufferSize_second;
//    int param_videoStreamBitRate;
//    int param_chunkSize;

    //GlobalStatistic *m_gstat;
    StreamingStatistic *m_gstat;

    int m_bufferSize_chunk;

    // -- Expected time interval between chunks [seconds]
    double m_chunkInterval;

    // -- Validation / Cross-check
    int m_nActiveElement;

    // -- To know the current status
    double m_time_firstChunk;
    long m_nChunkReceived;

    // -- signals for statistical analysis
    simsignal_t signal_seqNum_receivedChunk;
    simsignal_t signal_lateChunk, signal_inrangeChunk, signal_duplicatedChunk;

// Debug
private:
    cOutVector r_index;

    // listening support ->
private:
    std::vector<VideoBufferListener*> mListeners;
public:
    void addListener(VideoBufferListener* listener);
    void removeListener(VideoBufferListener* listener);
    // <- listening support
};

#endif /* VIDEOBUFFER_H_ */
