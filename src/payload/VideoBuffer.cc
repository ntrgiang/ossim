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
// VideoBuffer.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "VideoBuffer.h"
//#include "simutil.h"
#include "AppSettingDonet.h"
#include "DonetStatistic.h"

#define _VERSION_1 1
#define _VERSION_2 2
#define _VERSION_3 3
#define BUFFER_IMPL_VERSION _VERSION_3

using namespace std;

Define_Module(VideoBuffer)

VideoBuffer::VideoBuffer() {}

VideoBuffer::~VideoBuffer() {}

void VideoBuffer::initialize(int stage)
{
    if (stage == 0)
    {
        signal_seqNum_receivedChunk = registerSignal("Signal_RecevedChunk");
        signal_lateChunk            = registerSignal("Signal_Latechunk");
        signal_inrangeChunk         = registerSignal("Signal_InrangeChunk");
        signal_duplicatedChunk      = registerSignal("Signal_DuplicatedChunk");
    }

    if (stage != 3)
    {
        return;
    }

    cModule *temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet *m_appSetting = check_and_cast<AppSettingDonet *>(temp);

    temp = simulation.getModuleByPath("globalStatistic");
    //m_gstat = check_and_cast<GlobalStatistic *>(temp);
    m_gstat = check_and_cast<DonetStatistic *>(temp);

    m_bufferSize_chunk  = m_appSetting->getBufferMapSizeChunk();
    m_chunkInterval     = m_appSetting->getIntervalNewChunk();

    // -- Adjust the size of the videoBuffer to the suitable value
    m_streamBuffer.resize(m_bufferSize_chunk);

    // -- Initialize every element of the array to NULL
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        elem.m_chunk = NULL;
    }

    m_bufferStart_seqNum = m_bufferEnd_seqNum = m_head_received_seqNum = 0L;

    m_time_firstChunk = -1.0;
    m_nChunkReceived = 0L;

    WATCH(m_bufferSize_chunk);
    WATCH(m_chunkInterval);
}

void VideoBuffer::handleMessage(cMessage *)
{
    throw cException("VideoBuffer doesn't process messages!");
}

void VideoBuffer::initializeRangeVideoBuffer(SEQUENCE_NUMBER_T seq)
{
   m_bufferStart_seqNum = seq;
   m_head_received_seqNum = seq;
   m_bufferEnd_seqNum = m_bufferStart_seqNum + m_bufferSize_chunk - 1;
}

/*
 * Currently used by the Forwarder
 *
 */
void VideoBuffer::insertPacket(VideoChunkPacket *packet)
{
    Enter_Method("insertPacket()");

    EV << "---------- Insert a packet into video buffer ------------------------" << endl;

    long seq_num = packet->getSeqNumber();
    EV << "-- Sequence number of chunk: " << seq_num << " --> element: " << seq_num % m_bufferSize_chunk << endl;
    EV << "\t-- Buffer_start:\t" << m_bufferStart_seqNum << endl;
    EV << "\t-- Buffer_end:\t" << m_bufferEnd_seqNum << endl;

    // ------------- Out of range ------------------
    if (seq_num < m_bufferStart_seqNum)
    {
        EV << "The chunk arrived too late, should be discarded." << endl;
        emit(signal_lateChunk, seq_num);
//        m_gstat->reportLateChunk(seq_num);

        delete packet; packet = NULL;
        return;
    }
    emit(signal_inrangeChunk, seq_num);
//    m_gstat->reportInrangeChunk(seq_num);

    // ------------- Duplication ------------------
    if (isInBuffer(seq_num) == true)
    {
        EV << "The chunk already existed in the buffer, should be discarded." << endl;
        emit(signal_duplicatedChunk, seq_num);
//        m_gstat->reportDuplicatedChunk(seq_num);
        delete packet; packet = NULL;
        return;
    }

    // ------------- "Valid" packets ------------------
    // Stats
    emit(signal_seqNum_receivedChunk, seq_num);

    STREAM_BUFFER_ELEMENT_T & elem = m_streamBuffer[seq_num % m_bufferSize_chunk];

    if (elem.m_chunk != NULL)
    {
        EV << "-- Existing chunk needs to be deleted: " << seq_num % m_bufferSize_chunk << endl;
        delete elem.m_chunk;
        elem.m_chunk = NULL;

        if (__VIDEOBUFFER_CROSSCHECK__) --m_nActiveElement;
    }

    EV << "-- New chunk was attached to the Buffer" << endl;
    elem.m_chunk = packet;

    if (__VIDEOBUFFER_CROSSCHECK__) ++m_nActiveElement;
/*
#if (BUFFER_IMPL_VERSION == _VERSION_1)
    if (seq_num > m_id_bufferEnd)
    {
        // because of modification in class implementation
        m_id_bufferEnd = seq_num;
        m_id_bufferStart = std::max(0L, m_id_bufferEnd - m_bufferSize + 1);
    }
#elif (BUFFER_IMPL_VERSION == _VERSION_2)
    if (seq_num > m_bufferEnd_seqNum)
    {
        m_bufferEnd_seqNum = seq_num;
        m_bufferStart_seqNum = std::max(0L, m_bufferEnd_seqNum - m_bufferSize + 1);
    }
#elif (BUFFER_IMPL_VERSION == _VERSION_3)
    if (seq_num > m_head_received_seqNum)
    {
        EV << "-- Update the range of the Video Buffer:" << endl;

        m_head_received_seqNum = seq_num;
        m_bufferStart_seqNum = std::max(0L, m_head_received_seqNum - m_bufferSize_chunk + 1);
        m_bufferEnd_seqNum = m_bufferStart_seqNum + m_bufferSize_chunk - 1;
        EV << "  -- start:\t"   << m_bufferStart_seqNum     << endl;
        EV << "  -- end:\t"     << m_bufferEnd_seqNum       << endl;
        EV << "  -- head:\t"    << m_head_received_seqNum   << endl;
    }
#endif
*/

    // ------------- Adjust the window ------------------
    if (seq_num > m_head_received_seqNum)
    {
        EV << "-- Update the range of the Video Buffer:" << endl;

        m_head_received_seqNum = seq_num;
        //m_bufferStart_seqNum = std::max(0L, m_head_received_seqNum - m_bufferSize_chunk + 1);
        m_bufferStart_seqNum = std::max(m_bufferStart_seqNum, m_head_received_seqNum - m_bufferSize_chunk + 1);
        m_bufferEnd_seqNum = m_bufferStart_seqNum + m_bufferSize_chunk - 1;

        EV << "  -- start:\t"   << m_bufferStart_seqNum     << endl;
        EV << "  -- end:\t"     << m_bufferEnd_seqNum       << endl;
        EV << "  -- head:\t"    << m_head_received_seqNum   << endl;
    }

    // -- To know the status
    ++m_nChunkReceived;
    if (m_time_firstChunk < 0)
    {
        // This received chunk is the first one received so far
        m_time_firstChunk = simTime().dbl();
    }
    else
    {
        double delta_time = simTime().dbl() - m_time_firstChunk;
        if (delta_time > 0)
            EV << "Average received chunk rate: " << (double)m_nChunkReceived / delta_time << endl;
        else
            throw cException("delta_time is invalid");
    }

// listening support ->
    std::vector<VideoBufferListener*>::iterator it;
    for(it = mListeners.begin(); it != mListeners.end(); it++){
        (*it)->onNewChunk(seq_num);
    }
// <- listening support

}

void VideoBuffer::insertPacketDirect(VideoChunkPacket *packet)
{
    Enter_Method("insertPacket()");

    EV << "---------- Insert a packet into video buffer ------------------------" << endl;

    long seq_num = packet->getSeqNumber();
    EV << "-- Sequence number of chunk: " << seq_num << " --> element: " << seq_num % m_bufferSize_chunk << endl;
    EV << "\t-- Buffer_start:\t" << m_bufferStart_seqNum << endl;
    EV << "\t-- Buffer_end:\t" << m_bufferEnd_seqNum << endl;

    STREAM_BUFFER_ELEMENT_T & elem = m_streamBuffer[seq_num % m_bufferSize_chunk];

    if (elem.m_chunk != NULL)
    {
        EV << "-- Existing chunk needs to be deleted: " << seq_num % m_bufferSize_chunk << endl;
        delete elem.m_chunk;
        elem.m_chunk = NULL;

        if (__VIDEOBUFFER_CROSSCHECK__) --m_nActiveElement;
    }

    EV << "-- New chunk was attached to the Buffer" << endl;
    elem.m_chunk = packet;

    if (__VIDEOBUFFER_CROSSCHECK__) ++m_nActiveElement;

    if (seq_num > m_head_received_seqNum)
    {
        EV << "-- Update the range of the Video Buffer:" << endl;

        m_head_received_seqNum = seq_num;
        m_bufferStart_seqNum = std::max(0L, m_head_received_seqNum - m_bufferSize_chunk + 1);
        m_bufferEnd_seqNum = m_bufferStart_seqNum + m_bufferSize_chunk - 1;
        EV << "  -- start:\t"   << m_bufferStart_seqNum     << endl;
        EV << "  -- end:\t"     << m_bufferEnd_seqNum       << endl;
        EV << "  -- head:\t"    << m_head_received_seqNum   << endl;
    }

//    // -- To know the status
//    ++m_nChunkReceived;
//    if (m_time_firstChunk < 0)
//    {
//        // This received chunk is the first one received so far
//        m_time_firstChunk = simTime().dbl();
//    }
//    else
//    {
//        double delta_time = simTime().dbl() - m_time_firstChunk;
//        if (delta_time > 0)
//            EV << "Average received chunk rate: " << (double)m_nChunkReceived / delta_time << endl;
//        else
//            throw cException("delta_time is invalid");
//    }

// listening support ->
    std::vector<VideoBufferListener*>::iterator it;
    for(it = mListeners.begin(); it != mListeners.end(); it++){
        (*it)->onNewChunk(seq_num);
    }
// <- listening support

}

// TODO: verification!!!
bool VideoBuffer::isInBuffer(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("isInBuffer()");

    // Browse through the queue?
    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_streamBuffer.size()];
    /*
     * Return TRUE only if the elem points to a packet AND the seq_num of the packet is correct
     */
    return (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num);
}

bool VideoBuffer::inBuffer(SEQUENCE_NUMBER_T seq_num)
{
    // Browse through the queue?
    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_streamBuffer.size()];
    /*
     * Return TRUE only if the elem points to a packet AND the seq_num of the packet is correct
     */
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
        return true;

    return false;
}

// TODO: verification!!!
SIM_TIME_T VideoBuffer::getReceivedTime(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getReceivedTime()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_recved_time;
    }
    return (SIM_TIME_T)-1;
}

// TODO: verification!!!
/*
MeshVideoChunkPacket * VideoBuffer::getChunk(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getChunk()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_chunk;
    }
    return NULL;
}
*/

VideoChunkPacket * VideoBuffer::getChunk(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getChunk()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_chunk;
    }
    return NULL;
}

// TODO: verification!!!
STREAM_BUFFER_ELEMENT_T & VideoBuffer::getBufferElement(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getBufferElement()");

    return m_streamBuffer[seq_num % m_bufferSize_chunk];

}

double VideoBuffer::getPercentFill()
{
    int count_fill = getNumberOfChunkFill();
    return (double)count_fill / m_bufferSize_chunk;
}

int VideoBuffer::getNumberOfChunkFill()
{
    // !!! Asuming that the ref_ori is a valid value between seq_num_start & seq_num_end
    int count_fill = 0;
    for (SEQUENCE_NUMBER_T seq_num = m_bufferStart_seqNum; seq_num <= m_bufferEnd_seqNum; ++seq_num)
    {
        if (inBuffer(seq_num))
            ++count_fill;
    }

    return count_fill;
}

int VideoBuffer::getNumberOfChunkFillAhead(SEQUENCE_NUMBER_T ref_ori)
{
    // !!! Asuming that the ref_ori is a valid value between seq_num_start & seq_num_end
    int count_fill = 0;
    for (SEQUENCE_NUMBER_T seq_num = ref_ori; seq_num <= m_bufferEnd_seqNum; ++seq_num)
    {
        if (inBuffer(seq_num))
            ++count_fill;
    }

    return count_fill;
}

double VideoBuffer::getPercentFillAhead(SEQUENCE_NUMBER_T ref_ori)
{
    int count_fill_ahead = getNumberOfChunkFillAhead(ref_ori);
    int count_total_chunk_ahead = m_bufferEnd_seqNum - ref_ori;

    EV << "count_fill_ahead: " << count_fill_ahead << endl;
    EV << "count_total_chunk_ahead: " << count_total_chunk_ahead << endl;

    return (double)count_fill_ahead / (double)count_total_chunk_ahead;
}

void VideoBuffer::captureVideoBuffer(BufferMap *bm)
{
    Enter_Method("captureVideoBuffer()");

    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for (iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++iter)
    {
        // if the element stores no chunk
        if (iter->m_chunk == NULL)
            continue;

        // if the id of the packet at that element is too "old"
        // if (iter->m_chunk->getSeqNumber() < m_id_bufferStart)
        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
            continue;

        // int idx = iter->m_chunk->getSeqNumber() - m_id_bufferStart;
        int idx = iter->m_chunk->getSeqNumber() - m_bufferStart_seqNum;
        bm->setElementByOffset(idx, true); // TODO: find out the problem of this line
    }
}

void VideoBuffer::printStatus()
{
    if (ev.isGUI() == false)
        return;
/*
    // Version 1
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    EV << "[" << m_bufferStart_seqNum << ", " << m_bufferEnd_seqNum << "] --- ";
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        short bit = -1;
        bit = (elem.m_chunk != NULL)?1:0;
        EV << bit << " ";
    }
    EV << std::endl;
*/

/*
    // Version 2
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    int k = 1;
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        short bit = -1;
        bit = (elem.m_chunk != NULL)?1:0;
        EV << bit << " ";

        // -- For better presenting the bit array
        if (k % 10 == 0) EV << "  ";
        if (k % 100 == 0) EV << endl;
        k++;
    } // end of for
    EV << endl;
*/

    // Version 3
    EV << endl;
    EV << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    EV << "Video Buffer:" << endl;
    EV << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    EV << "-- Start:\t" << m_bufferStart_seqNum     << endl;
    EV << "-- End:\t"   << m_bufferEnd_seqNum       << endl;
    EV << "-- Head:\t"  << m_head_received_seqNum   << endl;

    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    iter = m_streamBuffer.begin();
    int k = 1;
    for(int i = 0; i <  m_bufferSize_chunk; i++)
    {
       long int chunkSeq = i + m_bufferStart_seqNum;
       iter = m_streamBuffer.begin() + (chunkSeq % m_bufferSize_chunk);
        //STREAM_BUFFER_ELEMENT_T &elem = *iter;
        short bit = -1;
        bit = (iter->m_chunk != NULL) ? 1 : 0;
        EV << bit << " ";

        // -- For better presenting the bit array
        if (k % 10 == 0) EV << "  ";
        if (k % 100 == 0) EV << endl;
        k++;
    } // end of for
    EV << endl;

}

void VideoBuffer::fillBufferMapPacket(MeshBufferMapPacket *bmPkt)
{
    EV << endl;
    EV << "%%%" << endl;
    EV << "Copy set bits from Buffer Map into BM packet: " << endl;

    // bmPkt->setBufferMapArraySize(m_bufferSize_chunk);
    // bmPkt->setIdStart(m_id_bufferStart);
    // bmPkt->setIdEnd(m_id_bufferEnd);
    // -- Synchronize the ranges of the two buffer maps
    bmPkt->setBmStartSeqNum(m_bufferStart_seqNum);
    bmPkt->setBmEndSeqNum(m_bufferEnd_seqNum);
    bmPkt->setHeadSeqNum(m_head_received_seqNum);

    EV << "-- Buffer Map info: " << endl;
    EV << "  -- m_bufferSize_chunk =\t"        << m_bufferSize_chunk       << endl;
    EV << "  -- Start:\t"   << m_bufferStart_seqNum     << endl;
    EV << "  -- End:\t"     << m_bufferEnd_seqNum       << endl;
    EV << "  -- Head:\t"    << m_head_received_seqNum   << endl;

/*
    // -- Initialize all of the element of the BM to false
    for (int i = 0; i < m_bufferSize_chunk; i++)
    {
        bmPkt->setBufferMap(i, false);
    }

    // this block has problem?
    int offset = 0;
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for (iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++iter)
    {
        EV << "Slot with offset = " << offset << " :: ";

        // if the element stores no chunk
        if (iter->m_chunk == NULL)
        {
            EV << "No chunk!" << endl;
            continue;
        }

        // if the id of the packet at that element is too "old"
        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
        {
            EV << "Chunk is out-dated!" << endl;
            continue;
        }

        EV << "with seq_num " << iter->m_chunk->getSeqNumber() << " is set to true!" << endl;
        bmPkt->setBufferMap(offset, true);
        offset++;
    }
*/
/*
    // -- New version of the code to optimize the speed
    int offset = -1;
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for (iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++iter)
    {
        ++offset;
        //EV << "BM slot with offset = " << offset << " :: ";

        // if the element stores no chunk
        if (iter->m_chunk == NULL)
        {
            //EV << "No chunk! --> set to false" << endl;
            bmPkt->setBufferMap(offset, false);
            continue;
        }

        // if the id of the packet at that element is too "old"
        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
        {
            //EV << "Chunk is out-dated! --> set to false" << endl;
            bmPkt->setBufferMap(offset, false);
            continue;
        }

        //EV << "has chunk with seq_num " << iter->m_chunk->getSeqNumber() << " in range --> is set to true!" << endl;
        bmPkt->setBufferMap(offset, true);
    } // end of for
*/
    // Even more optimized codes

    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    iter = m_streamBuffer.begin();
    int fillRange = m_head_received_seqNum - m_bufferStart_seqNum + 1;

    for (int i = 0; i < fillRange; i++)
    {
       long int videoChunk_seq = i + m_bufferStart_seqNum;
       iter = m_streamBuffer.begin() + (videoChunk_seq % m_bufferSize_chunk);

       if (iter->m_chunk == NULL)
       {
           //EV << "No chunk! --> set to false" << endl;
           bmPkt->setBufferMap(i, false);
           continue;
       }

        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
        {
            //EV << "Chunk is out-dated! --> set to false" << endl;
            bmPkt->setBufferMap(i, false);
            continue;
        }

       if (iter->m_chunk->getSeqNumber() != videoChunk_seq)
       {
          throw cException("Expected chunk %ld, but actual chunk %ld",
                           videoChunk_seq,
                           iter->m_chunk->getSeqNumber());
          // EV << "Chunk is out-dated! --> set to false" << endl;
          // bmPkt->setBufferMap(offset, false);
       }

       bmPkt->setBufferMap(i, true);


    } // for

    for (int i = fillRange; i < m_bufferSize_chunk; i++)
    {
       bmPkt->setBufferMap(i, false);
    }
//    for (int i = m_bufferStart_seqNum; i <= m_head_received_seqNum; i++)
//    {
//       //if (m_streamBuffer[i % m_bufferSize_chunk])
//       iter = m_streamBuffer.begin() + (i % m_bufferSize_chunk);
//       if (iter->m_chunk == NULL)
//       {
//           //EV << "No chunk! --> set to false" << endl;
//           bmPkt->setBufferMap(offset, false);
//       }


//    }
}

void VideoBuffer::finish(void)
{
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;

    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        if (elem.m_chunk != NULL)
        {
            delete elem.m_chunk;
            elem.m_chunk = NULL;

            if (__VIDEOBUFFER_CROSSCHECK__) --m_nActiveElement;

            // EV << "one element deleted! --- " << endl;
        }
    }

    #if(__VIDEO_BUFFER_DEBUG__)
        EV << "VideoBuffer@finish() :: Number of undeleted objects: " << m_nActiveElement << endl;
    #endif
}

int VideoBuffer::getNumberActiveElement(void)
{
    return m_nActiveElement;
}

//int VideoBuffer::getNumberFilledChunk()
//{
//    int count = 0;
//    for (SEQUENCE_NUMBER_T i = m_bufferStart_seqNum; i <= m_bufferEnd_seqNum; ++i)
//    {
//        if (inBuffer(i))
//            ++count;
//    }
//    EV << "Number of chunks in the video buffer: " << count << endl;
//    return count;
//}




// listening support ->
void VideoBuffer::addListener(VideoBufferListener* listener){
    mListeners.push_back(listener);
}
void VideoBuffer::removeListener(VideoBufferListener* listener){
    std::vector<VideoBufferListener*>::iterator it;
    for(it = mListeners.begin(); it != mListeners.end(); it++){
        if ( (*it) == listener )
            mListeners.erase(it);
    }
}
// <- listening support
