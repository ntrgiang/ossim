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
// BufferMap.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef BUFFERMAP_H_
#define BUFFERMAP_H_

#include <vector>
#include "AppCommon.h"
#include "MeshPeerStreamingPacket_m.h"

class BufferMap {
public:
    BufferMap(int size=0);
    virtual ~BufferMap();

public:
//    inline void setID_bmStart(SEQUENCE_NUMBER_T val) { m_id_bmStart = val; }
//    inline SEQUENCE_NUMBER_T getID_bmStart() { return m_id_bmStart; }
//    inline void setID_bmEnd(SEQUENCE_NUMBER_T val) { m_id_bmEnd = val; }
//    inline SEQUENCE_NUMBER_T getID_bmEnd() { return m_id_bmEnd; }

    inline void setBmStartSeqNum(SEQUENCE_NUMBER_T val) { m_bmStart_seqNum = val; }
    inline void setBmEndSeqNum(SEQUENCE_NUMBER_T val) { m_bmEnd_seqNum = val; }
    inline void setHeadSeqNum(SEQUENCE_NUMBER_T val) { m_head_seqNum = val; }
    inline void setSize(int size) { m_map.resize(size); }

    inline SEQUENCE_NUMBER_T getBmStartSeqNum(void) { return m_bmStart_seqNum; }
    inline SEQUENCE_NUMBER_T getBmEndSeqNum(void) { return m_bmEnd_seqNum; }
    inline SEQUENCE_NUMBER_T getHeadSeqNum(void) { return m_head_seqNum; }
    /*
    // not sure to have this kind of public interface
    */

    inline int getSize() { return m_map.size(); }

    void setElementByOffset(int index, bool val);
    bool getElementByOffset(unsigned int index);

    void setElementBySeqNum(SEQUENCE_NUMBER_T seq_num, bool val);
    bool getElementBySeqNum(SEQUENCE_NUMBER_T seq_num);

    void updateBufferMapMsg(MeshBufferMapPacket *msg);
    void readBufferMapPacket(MeshBufferMapPacket *msg);

    void copyFromBmPacket(MeshBufferMapPacket *pkt);
    //void copyToChunkRequestPacket(MeshChunkRequestPacket *pkt);

    bool isInBufferMap(SEQUENCE_NUMBER_T seq_num);

    /*
     * Set the value of all elements in m_map to false
     */
    void reset();

    // For debugging purpose
    void printBM();

private:
    std::vector<bool> m_map;
    // bool *m_mapArray;
//    SEQUENCE_NUMBER_T m_id_bmStart;
//    SEQUENCE_NUMBER_T m_id_bmEnd;

    // Bounds of the Buffer Map
    SEQUENCE_NUMBER_T m_bmStart_seqNum;
    SEQUENCE_NUMBER_T m_bmEnd_seqNum;
    SEQUENCE_NUMBER_T m_head_seqNum;

    int m_bmSize;
};

#endif /* BUFFERMAP_H_ */
