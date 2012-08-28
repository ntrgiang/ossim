//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "BufferMap.h"
#include <assert.h>

/*
BufferMap::BufferMap(int size)
: m_id_bmStart(-1), m_id_bmEnd(-1)
{
    // TODO Auto-generated constructor stub

    assert(size>0);

    // Creating enough number of elements for the buffer
     m_map.resize(size);

    // change the code so that it is "compatible" to Eclipse's suggestion
//    m_map.resize((unsigned long int)size, false);

    for(int i = 0; i < size; i++)
    {
        m_map[i] = false;
    }

    m_bmSize = size;

    m_mapArray = new bool[size];
    for(int i = 0; i < size; i++)
    {
        m_mapArray[i] = false;
    }
}
*/

BufferMap::BufferMap(int size)
// : m_id_bmStart(-1), m_id_bmEnd(-1)
: m_bmStart_seqNum(-1), m_bmEnd_seqNum(-1)
{
    // TODO Auto-generated constructor stub

    // assert(size>0);
    if (size <= 0) throw cException("Buffer map size has to be a positive number!");

    m_bmSize = size;
    m_map.resize(size);

    for (int i = 0; i < size; ++i) m_map[i] = false;

    /*
    m_mapArray = new bool[size];

    for(int i = 0; i < size; i++)
    {
        m_mapArray[i] = false;
    }
    */
}

BufferMap::~BufferMap() {
    // TODO Auto-generated destructor stub

    // delete [] m_mapArray;
}

bool BufferMap::getElementByOffset(unsigned int index)
{
    // assert(index >= 0 && index < m_map.size());
    if (index >= 0 && index < m_map.size())
        return m_map[index];
    throw cException("Index value %d is invalid", index);
}

void BufferMap::setElementByOffset(int index, bool val)
{
//    m_map[index] = val;
    //m_mapArray[index] = val;
    m_map[index] = val;
}

void BufferMap::setElementBySeqNum(SEQUENCE_NUMBER_T seq_num, bool val)
{
    int offset = seq_num - m_bmStart_seqNum;
    m_map[offset] = val;
}

bool BufferMap::getElementBySeqNum(SEQUENCE_NUMBER_T seq_num)
{
    int offset = seq_num - m_bmStart_seqNum;
    return m_map[offset];
}

void BufferMap::updateBufferMapMsg(MeshBufferMapPacket *msg)
{
/*
    // TODO: check if type-casting here is a problem
    int size = m_map.size();

    msg->setBufferMapArraySize(size);
    for (int i=0; i<size; i++)
        msg->setBufferMap(i, m_map[i]);
*/
}

/*
 *
 */
void BufferMap::readBufferMapPacket(MeshBufferMapPacket *pkt)
{
    // TODO: check if type-casting here is a problem
    int size = m_map.size();

    // -- Get the value of id_start or id_end of the BufferMap Packet
    m_bmStart_seqNum    = pkt->getBmStartSeqNum();
    m_bmEnd_seqNum      = pkt->getBmEndSeqNum();
    m_head_seqNum       = pkt->getHeadSeqNum();

    // -- Browse through all element of the array in the BufferMap packet
    // -- update the BufferMap elements respectively
    for (int i=0; i<size-1; i++)
    {
        setElementByOffset(i, pkt->getBufferMap(i));
    }
}

void BufferMap::copyFromBmPacket(MeshBufferMapPacket *pkt)
{
    //TODO

}

/*
void BufferMap::copyToChunkRequestPacket(MeshChunkRequestPacket *pkt)
{
    // TODO: check if type-casting here is a problem
    int size = m_map.size();

    // -- Get the value of id_start or id_end of the BufferMap Packet
    m_bmStart_seqNum = pkt->getBmStartSeqNum();
    m_bmEnd_seqNum = pkt->getBmEndSeqNum();
    m_head_seqNum = pkt->getHead_seqNum();

    // -- Browse through all element of the array in the BufferMap packet
    // -- update the BufferMap elements respectively
    for (int i=0; i<size-1; i++)
    {
        setElementByOffset(i, pkt->getBufferMap(i));
    }
}
*/

void BufferMap::printBM()
{
    int size = m_map.size();
    for (int i=0; i<size; i++)
    {
        short bit = -1;
        bit = (m_map[i] == true)?1:0;
        EV << bit << " ";
    }
    EV << endl;
}

void BufferMap::reset()
{
    for (int i = 0; i < m_bmSize; i++)
    {
        m_map[i] = false;
        // m_mapArray[i] = false;
    }
}

bool BufferMap::isInBufferMap(SEQUENCE_NUMBER_T seq_num)
{
    long offset = seq_num - m_bmStart_seqNum;
    // assert(offset >= 0);
    if (offset < 0) throw cException("Offset value %d is invalid", offset);

    return (m_map[offset]);
}
