#include "DonetPeer.h"

#include <algorithm>

using namespace std;

/**
 * A very straight-forward version of the implementation --> should not expect that it is smart
 */
void DonetPeer::donetChunkScheduling(void)
{
    Enter_Method("donetChunkScheduling()");

    // -- Get the (current) number of partners
    int nPartner = m_partnerList->getSize();
    std::vector<SEQUENCE_NUMBER_T> dup_set[nPartner-1];

    // EV << "random chunk scheduling triggered! " << endl;

    // -- Clear all request windows for all neighbors
    m_partnerList->clearAllSendBm();

    // -- For Donet scheduling
    // m_partnerList->clearAllTimeBudget();

    // -- Prepare the scheduling window
    long upper_bound = m_seqNum_schedWinHead;
    long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);

    // -- Get current time, for faster query time when it is used repeatedly
    // double current_time = simTime().dbl();

    // -- Calculate the available for _all_ chunk (expect redundancy, but for simplicity of implementation), for _all_ partners
    m_partnerList->resetAllAvailableTime(m_videoBuffer->getBufferStartSeqNum(), lower_bound, m_videoBuffer->getChunkInterval());

    // -- Update bounds of all sendBM
    //m_partnerList->updateBoundSendBm(m_seqNum_schedWinHead, lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);

    // -- Finding the expected set
    std::vector<SEQUENCE_NUMBER_T> expected_set;
    for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
    {
        if (m_videoBuffer->isInBuffer(seq_num) == false)
        {
            expected_set.push_back(seq_num);
        }
    } // end of for

    int sizeExpectedSet = expected_set.size();
    std::vector<SEQUENCE_NUMBER_T> copied_expected_set;

    // -- Have a copy of the expected_set, so that chunks which has been found a supplier will be deleted from this copy
    copied_expected_set = expected_set;

    // -- Browse through the expected_set
    //for(std::vector<SEQUENCE_NUMBER_T>::iterator iter = expected_set.begin(); iter != expected_set.end(); ++iter)
    for (int i = 0; i < sizeExpectedSet; ++i)
    {
        SEQUENCE_NUMBER_T seq_num = expected_set[i];
        std::vector<IPvXAddress> holderList;
        m_partnerList->getHolderList(seq_num, holderList);
        int nHolder = holderList.size();

        if (nHolder == 1)
        {
            // -- Get pointer to the respective NeighborInfo
            NeighborInfo nbr_info = m_partnerList->getNeighborInfo(holderList[0]);
            // -- Set the respective element in the SendBm to say that this chunk should be requested
            nbr_info.setElementSendBm(expected_set[i], true);

            // -- Get peer's upload bandwidth
            double peerUpBw = nbr_info.getUpBw();

            // -- Browse through the expected_set
            int currentSize = copied_expected_set.size();
            for (int k = 0; k < currentSize; ++k)
            {
                nbr_info.updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw);
            }
            // -- Delete the chunk whose supplier had been found
            std::vector<SEQUENCE_NUMBER_T>::iterator iter;
            iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), expected_set[i]);
            copied_expected_set.erase(iter);

            // -- Recording results
//            m_reqChunkId.record(seq_num);
        }
        else
        {
            // -- Add the chunk's sequence number into a suitable dup_set
            dup_set[nHolder-2].push_back(expected_set[i]);
        }
    } // for (i)

    // -- Selecting partner for chunks of different groups
    for (int n = 2; n < nPartner; ++n)
    {
        int size_dup_set = dup_set[n-2].size();

        for (int k=0; k < size_dup_set; ++k)
        {
            SEQUENCE_NUMBER_T seq_num = dup_set[n-2][k];

            std::vector<IPvXAddress> holderList;
            m_partnerList->getHolderList(seq_num, holderList);
            int nHolder = holderList.size();

            IPvXAddress candidate1, candidate2, supplier;
            candidate1 = holderList[0];
            candidate2 = holderList[1];

            int ret = selectOneCandidate(seq_num, candidate1, candidate2, supplier);

            if (nHolder > 2)
            {
                for (int j = 2; j < nHolder; ++j)
                {
                    // -- User result from the previous calculation
                    candidate1 = supplier;
                    // -- Update the second candidate with another partner
                    candidate2 = holderList[j];

                    ret = selectOneCandidate(seq_num, supplier, candidate2, supplier);
                }
            }

            // -- Loops through all holders should be completed at this point
            if (ret == -1)
            {
                // -- Meaning that no suitable supplier has been found
                // Consider the next chunk for finding supplier
                continue;
            }

            // -- Get pointer to the respective NeighborInfo
            NeighborInfo nbr_info = m_partnerList->getNeighborInfo(supplier);
            // -- Set the respective element in the SendBm to say that this chunk should be requested
            nbr_info.setElementSendBm(seq_num, true);

            // -- Get peer's upload bandwidth
            double peerUpBw = nbr_info.getUpBw();

            // -- Browse through the expected_set
            int currentSize = copied_expected_set.size();
            for (int k = 0; k < currentSize; ++k)
            {
                nbr_info.updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw);
            }
            // -- Delete the chunk whose supplier had been found
            // copied_expected_set.erase(expected_set[i]);
            std::vector<SEQUENCE_NUMBER_T>::iterator iter;
            iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), seq_num);
            copied_expected_set.erase(iter);

            // -- Recording results
//            m_reqChunkId.record(seq_num);

        } // for (k) -- Browse through all dup_set
    } // for (n) -- Browse through all partners

    // DEBUG
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
        EV << "Scheduling window: [" << lower_bound << " - " << upper_bound << "]" << endl;
        m_partnerList->printAllSendBm();

        //EV << "My partners: ";
        //m_partnerList->print();
    #endif
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

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

} // Donet Chunk Scheduling

