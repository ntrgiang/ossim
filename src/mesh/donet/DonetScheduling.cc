#include "DonetPeer.h"

#include <algorithm>

using namespace std;

/**
 * A very straight-forward version of the implementation --> should not expect that it is smart
 */
void DonetPeer::donetChunkScheduling(void)
{
    Enter_Method("donetChunkScheduling()");
    EV << "Donet chunk scheduling triggered! " << endl;

    int count_0 = 0;
    int count_1 = 0;
    int count_2 = 0;

    // -- Get the (current) number of partners
    int nPartner = m_partnerList->getSize();
    EV << "\t has " << nPartner << " partners" << endl;
    std::vector<SEQUENCE_NUMBER_T> dup_set[nPartner-1]; // TODO: Why nPartner-1 ???

    // -- Clear all request windows for all neighbors
    m_nChunkRequested_perSchedulingInterval = 0;
    int nNewChunkForRequest_perSchedulingCycle = 0;

    // -- For Donet scheduling
    // m_partnerList->clearAllTimeBudget();

    // -- Prepare the scheduling window
//    long upper_bound = m_seqNum_schedWinHead;
//    long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);

    long lower_bound = m_sched_window.start;
    long upper_bound = m_sched_window.end;

    // -- Get current time, for faster query time when it is used repeatedly
    // double current_time = simTime().dbl();

    // -- Calculate the available for _all_ chunk (expect redundancy, but for simplicity of implementation), for _all_ partners
    //m_partnerList->resetAllAvailableTime(m_videoBuffer->getBufferStartSeqNum(), lower_bound, m_videoBuffer->getChunkInterval());
    m_partnerList->resetAllAvailableTime(m_player->getCurrentPlaybackPoint(), lower_bound, m_videoBuffer->getChunkInterval());

    // -- Update bounds of all sendBM
    // m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->clearAllSendBm();
    m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->resetNChunkScheduled();

    // -- Finding the expected set (set of chunks which should be requested)
    std::vector<SEQUENCE_NUMBER_T> expected_set;
    for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
    {
       if (should_be_requested(seq_num) == false)
       {
          continue;
       }

       if (m_videoBuffer->isInBuffer(seq_num) == false)
       {
          expected_set.push_back(seq_num);
       }
    } // end of for

    int sizeExpectedSet = expected_set.size();
    std::vector<SEQUENCE_NUMBER_T> copied_expected_set = expected_set;

    // -- Print out the expected set
    EV << "List of " << sizeExpectedSet << " expected chunks to request: " << endl;
//    short i = 0;
//    for(std::vector<SEQUENCE_NUMBER_T>::iterator iter = expected_set.begin();
//        iter != expected_set.end(); ++iter)
//    {
//       EV << *iter << " ";
//       ++i;
//       if (i % 10 == 0) EV << endl;
//    }
//    EV << endl;

    // -- Have a copy of the expected_set, so that chunks which has been found a supplier will be deleted from this copy

    // -- Browse through the expected_set
    //for(std::vector<SEQUENCE_NUMBER_T>::iterator iter = expected_set.begin(); iter != expected_set.end(); ++iter)
    for (int i = 0; i < sizeExpectedSet; ++i)
    {
        SEQUENCE_NUMBER_T seq_num = expected_set[i];
        std::vector<IPvXAddress> holderList;
        m_partnerList->getHolderList(seq_num, holderList);
        int nHolder = holderList.size();

        //EV << "Chunk " << seq_num << " with " << nHolder << " holders" << endl;

        if (nHolder == 0)
        {
           ++count_0;
           continue;
        }

        ++nNewChunkForRequest_perSchedulingCycle;

        if (nHolder == 1)
        {
           ++count_1;

//            if (should_be_requested(seq_num) == true)
//            {
                //EV << "----> This chunk should be requested this time" << endl;

                // -- Get pointer to the respective NeighborInfo
                m_partnerList->setElementSendBm(holderList[0], seq_num, true);
//                NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(holderList[0]); // get obsolete?

                // -- Set the respective element in the SendBm to say that this chunk should be requested
                //nbr_info->setElementSendBm(expected_set[i], true);
//                nbr_info->setElementSendBm(seq_num, true); // get obsolete?

                m_list_requestedChunk.push_back(seq_num);
                ++m_nChunkRequested_perSchedulingInterval;

                // -- Get peer's upload bandwidth

                //double peerUpBw = nbr_info->getUpBw(); // get obsolete?
                double peerUpBw = m_partnerList->getUpBw(holderList[0]);

                bool ret = true;
                // -- Browse through the expected_set
                int currentSize = copied_expected_set.size();
                for (int k = 0; k < currentSize; ++k)
                {
                   // EV << "nHolder = 1; k = " << k << endl;
                   // ret = nbr_info->updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw); // get obsolete?
                   ret = m_partnerList->updateChunkAvailTime(holderList[0],
                                                             copied_expected_set[k],
                                                             (param_chunkSize*8)/peerUpBw);
                   if (ret == false)
                   {
                      EV << "updateChunkAvailTime == false" << endl;
                      break;
                   }
                } // for

                // EV << "Browse done!" << endl;

                if (ret == true)
                {
                   EV << "ret == true --> erase ... " << endl;
                   // -- Delete the chunk whose supplier had been found
                   std::vector<SEQUENCE_NUMBER_T>::iterator iter;
                   // iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), expected_set[i]);
                   iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), seq_num);
                   copied_expected_set.erase(iter);
                }

                // -- Recording results
                // m_reqChunkId.record(seq_num);

//            } // if (should_be_requested)
        }
        else
        {
            // -- Add the chunk's sequence number into a suitable dup_set
            //dup_set[nHolder-2].push_back(expected_set[i]);
            ++count_2;
            dup_set[nHolder-2].push_back(seq_num);
        }
    } // for (i)

    EV << "0: " << count_0 << " -- 1: " << count_1 << " -- >1: " << count_2
       << " -- total: " << count_0 + count_1 + count_2
       << " -- double check: " << sizeExpectedSet << endl;

   for (int n = 2; n <= nPartner; ++n)
   {
      int size_dup_set = dup_set[n-2].size();

      EV << "Number of chunks available on " << n << " partners: " << size_dup_set << endl;

      for (int k=0; k < size_dup_set; ++k)
      {
            SEQUENCE_NUMBER_T seq_num = dup_set[n-2][k];
            EV << seq_num << " ";
      }
      EV << endl;
   }

    // -- Selecting partner for chunks of different groups
    for (int n = 2; n <= nPartner; ++n)
    {
        int size_dup_set = dup_set[n-2].size();

        EV << "Number of chunks available on " << n << " partners: " << size_dup_set << endl;

        for (int k=0; k < size_dup_set; ++k)
        {
            SEQUENCE_NUMBER_T seq_num = dup_set[n-2][k];

            // TODO: optimize this piece of code with map<seq_num, vector<ipvxaddress>> inside the first for loop
            std::vector<IPvXAddress> holderList;
            m_partnerList->getHolderList(seq_num, holderList);
            int nHolder = holderList.size();

            IPvXAddress candidate1, candidate2, supplier;
            candidate1 = holderList[0];
            candidate2 = holderList[1];

            int ret = selectOneCandidate(seq_num, candidate1, candidate2, supplier);
//            int ret = 0;

            if (ret == -1)
            {
                // -- Meaning that no suitable supplier has been found
                // Consider the next chunk for finding supplier
                continue;
            }

//            if (nHolder > 2)
//            {
                for (int j = 2; j < nHolder; ++j)
                {
                    // -- User result from the previous calculation
                    candidate1 = supplier;
                    // -- Update the second candidate with another partner
                    //candidate2 = holderList[j];

                    //ret = selectOneCandidate(seq_num, supplier, candidate2, supplier);
                }
//            }

            // -- Loops through all holders should be completed at this point
            if (ret == -1)
            {
                // -- Meaning that no suitable supplier has been found
                // Consider the next chunk for finding supplier
                continue;
            }

//            if (should_be_requested(seq_num) == true)
//            {
               // -- Get pointer to the respective NeighborInfo
//               NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(supplier); // get obsolete?
               // -- Set the respective element in the SendBm to say that this chunk should be requested
//               nbr_info->setElementSendBm(seq_num, true); // get obsolete?

                m_partnerList->setElementSendBm(holderList[0], seq_num, true);

               m_list_requestedChunk.push_back(seq_num);
               ++m_nChunkRequested_perSchedulingInterval;

               // -- Get peer's upload bandwidth
//               double peerUpBw = nbr_info->getUpBw(); // get obsolete?
               double peerUpBw = m_partnerList->getUpBw(holderList[0]);

               // -- Browse through the expected_set
               int currentSize = copied_expected_set.size();
               for (int k = 0; k < currentSize; ++k)
               {
                  EV << "nHolder > 1; k = " << k << endl;
//                  nbr_info->updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw); // get obsolete?
                  m_partnerList->updateChunkAvailTime(holderList[0],
                                                      copied_expected_set[k],
                                                      (param_chunkSize*8)/peerUpBw);
               }

               // -- Delete the chunk whose supplier had been found
               // copied_expected_set.erase(expected_set[i]);
               std::vector<SEQUENCE_NUMBER_T>::iterator iter;
               iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), seq_num);
               copied_expected_set.erase(iter);

               // -- Recording results
               // m_reqChunkId.record(seq_num);

//            } // if (should_be_requested)

        } // for (k) -- Browse through all dup_set
    } // for (n) -- Browse through all partners

   emit(sig_newchunkForRequest, nNewChunkForRequest_perSchedulingCycle);

    // -- Debug
    // EV << "Scheduling window: [" << lower_bound << " - " << upper_bound << "]" << endl;
    // m_partnerList->printAllSendBm();

    //EV << "My partners: ";
    //m_partnerList->print();

    // -- Browse through the list of partners to see which one have been set the sendBm
    // -- For each of those one, prepare a suitable ChunkRequestPacket and send to that one
// obsolete
//    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
//    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
//    {
//        if (iter->second->isSendBmModified() == true)
//        {
//            EV << "-------Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
//            iter->second->printSendBm();

//            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket;
//                chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
//                // -- Map the sendBM into ChunkRequestPacket
//                iter->second->copyTo(chunkReqPkt);

//            // -- Send the copy
//            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
//        }
//    } // end of for

    // -- Chunk request
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        if (iter->second.isSendBmModified() == true)
        {
            EV << "-------Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
            iter->second.printSendBm();

            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket;
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
    if (m_player->getPlayerState() == PLAYER_STATE_PLAYING)
    {
       m_sched_window.start += m_videoStreamChunkRate;
       m_sched_window.end  += m_videoStreamChunkRate;
    }

    EV << "m_sched_window.start = " << m_sched_window.start << endl;
    EV << "m_sched_window.end = " << m_sched_window.end << endl;

    // -- Report statistics
    emit(sig_nChunkRequested, m_nChunkRequested_perSchedulingInterval);
    emit(sig_schedWin_start, m_sched_window.start);
    emit(sig_schedWin_end, m_sched_window.end);


} // Donet Chunk Scheduling

/**
 * Used for Donet chunk scheduling
 */
/*
int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier)
{
    int ret = 0;

    // -- Get pointer to the respective NeighborInfo
    NeighborInfo *info_candidate1 = m_partnerList->getNeighborInfo(candidate1);
    if (info_candidate1 == NULL)
       throw cException("null pointer");

    NeighborInfo *info_candidate2 = m_partnerList->getNeighborInfo(candidate2);
    if (info_candidate1 == NULL)
       throw cException("null pointer");

    if (info_candidate1->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate1->getUpBw())
    {
        if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
        {
            if (info_candidate1->getUpBw() > info_candidate2->getUpBw())
            {
                supplier = candidate1;
            }
            else
            {
                supplier = candidate2;
            }
        }
        else
        {
            supplier = candidate1;
        }
    }
    else
    {
        if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
        {
            supplier = candidate2;
        }
        else
        {
            ret = -1;
            // -- Just a /Padding code/
            supplier = candidate1;
        }
    }

    return ret;
}
*/

int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier)
{
    int ret = 0;

    // -- Get pointer to the respective NeighborInfo
//    NeighborInfo *info_candidate1 = m_partnerList->getNeighborInfo(candidate1);
//    if (info_candidate1 == NULL)
//       throw cException("null pointer");

//    NeighborInfo *info_candidate2 = m_partnerList->getNeighborInfo(candidate2);
//    if (info_candidate1 == NULL)
//       throw cException("null pointer");

//    if (info_candidate1->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate1->getUpBw())
    if (m_partnerList->getChunkAvailTime(candidate1, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate1))
    {
//        if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
        if (m_partnerList->getChunkAvailTime(candidate2, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate2))
        {
            //if (info_candidate1->getUpBw() > info_candidate2->getUpBw())
            if (m_partnerList->getUpBw(candidate1) > m_partnerList->getUpBw(candidate2))
            {
                supplier = candidate1;
            }
            else
            {
                supplier = candidate2;
            }
        }
        else
        {
            supplier = candidate1;
        }
    }
    else
    {
        //if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
        if (m_partnerList->getChunkAvailTime(candidate2, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate2))
        {
            supplier = candidate2;
        }
        else
        {
            ret = -1;
            // -- Just a /Padding code/
            supplier = candidate1;
        }
    }

    return ret;
}

