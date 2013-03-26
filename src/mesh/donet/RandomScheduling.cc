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
// RandomScheduling.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "DonetPeer.h"

using namespace std;

void DonetPeer::randomChunkScheduling(void)
{
    EV << endl;
    EV << "---------- Random chunk scheduling ----------------------------------" << endl;

    // -- Clear state variablesm
    m_nChunkRequested_perSchedulingInterval = 0;

    // -- Clear all request windows for all neighbors
    m_partnerList->clearAllSendBm();

    // -- Prepare the scheduling window
//    long upper_bound = m_seqNum_schedWinHead;
//    long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);

    // -- New scheduling windows
    long lower_bound = m_sched_window.start;
    long upper_bound = m_sched_window.end;

    // -- New update scheme which takes into account the initialized scheduling window
//    long lower_bound = std::max(m_seqNum_schedWinStart, m_sched_window.start);
//    long upper_bound = lower_bound + m_bufferMapSize_chunk - 1;
    //long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);


    // -- Update bounds of all sendBM
    //m_partnerList->updateBoundSendBm(m_seqNum_schedWinHead, lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->resetNChunkScheduled();

    EV << "-- Scheduling window: [" << lower_bound << " - " << upper_bound << "]" << endl;
    EV << "-- This node has " << m_partnerList->getSize() << " partners" << endl;
    EV << "-- Initial state of the request Bm:" << endl;
    m_partnerList->printAllSendBm();

    int nNewChunkForRequest_perSchedulingCycle = 0;

    EV << "----- Browsing through the chunks within the scheduling windows-----" << endl;

    printListOfRequestedChunk();

    // -- Looking for chunk to request
    for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
    {
        EV << "-- Investigating chunk " << seq_num << ": ";
        if (m_videoBuffer->isInBuffer(seq_num) == false)
        {
            EV << "-- This chunk is NOT in the Buffer" << endl;
            // -- Debug -- Counting

            if (should_be_requested(seq_num) == false)
            {
                EV << "----> This chunk should NOT be requested this time" << endl;
                continue;
            }

            // -- Get list of neighbors which has this chunk in their buffer map
            std::vector<IPvXAddress> holderList;
            m_partnerList->getHolderList(seq_num, holderList);
            int nHolder = holderList.size();

            // if there is at least one neighbor
            if (nHolder > 0)
            {
                ++nNewChunkForRequest_perSchedulingCycle;

                int index = -1;
                if (nHolder == 1)
                {
                    EV << "  -- There is only one holder of the chunk." << " -- ";
                    index = 0;
                }
                else
                {
                    index = (int)intrand(holderList.size());
                    //EV << "  -- There are " << nHolder << " holders, random index: " << index << " -- ";
                } // if

                EV << "  -- Holder for chunk " << seq_num << " is " << holderList[index] << endl;

                // -- Preparing to access record of partner to set the sendBM
                NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(holderList[index]);

                // -- Debug
                // if (!nbr_info) EV << "Null pointer for neighborInfo" << endl;
//                m_reqChunkId.record(seq_num);

                nbr_info->setElementSendBm(seq_num, true);

                m_list_requestedChunk.push_back(seq_num);
                ++m_nChunkRequested_perSchedulingInterval;

                // -- Emit signal for statistics collection
                emit(sig_chunkRequestSeqNum, seq_num);
                m_gstat->reportRequestedChunk(seq_num);

            } // if (nHolder > 0)
            else
            {
//                EV << "  -- There is no holder" << endl;
            }

        } // end of if(inBuffer)
        else
        {
            EV << "-- this is already in VideoBuffer" << endl;
        }

        EV << "-----" << endl;

    } // end of for

    emit(sig_newchunkForRequest, nNewChunkForRequest_perSchedulingCycle);

    // -- Browse through the list of partners to see which one have been set the sendBm
    // -- For each of those one, prepare a suitable ChunkRequestPacket and send to that one

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        if (iter->second.isSendBmModified() == true)
        {
            EV << "Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
            iter->second.printSendBm();

            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket("MESH_PEER_CHUNK_REQUEST");
                chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
                // -- Map the sendBM into ChunkRequestPacket
                iter->second.copyTo(chunkReqPkt);

            // -- Send the copy
            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
        }
    } // end of for

    // -- Now refreshing the m_list_requestedChunk
    refreshListRequestedChunk();

    // -- Move the scheduling window forward
    if (m_player->getState() == PLAYER_STATE_PLAYING)
    {
       m_sched_window.start += m_videoStreamChunkRate;
       m_sched_window.end  += m_videoStreamChunkRate;
    }

    // -- Report statistics
    emit(sig_nChunkRequested, m_nChunkRequested_perSchedulingInterval);

    EV << endl;
}
// Random Chunk Scheduling
