
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
// DonetScheduling.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "DonetPeer.h"
#include <algorithm>
#include <assert.h>

using namespace std;

//#define 	ev   (*cSimulation::getActiveEnvir())
#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "::" << getFullName() << " @ " << simTime().dbl() << ": "
//#define debugOUT (!m_debug) ? ev : ev << "::" << getFullName() << " @ " << simTime().dbl() << ": "
#endif

#define anIP "192.168.0.2"

/**
 * A very straight-forward version of the implementation --> should not expect that it is smart
 */
//void DonetPeer::donetChunkScheduling(void)
void DonetPeer::donetChunkScheduling(SEQUENCE_NUMBER_T lower_bound, SEQUENCE_NUMBER_T upper_bound)
{
   Enter_Method("donetChunkScheduling()");

   //if (getNodeAddress() == IPvXAddress(anIP)) debugOUT << "Donet scheduling" << endl;

   int count_0 = 0;
   int count_1 = 0;
   int count_2 = 0;

   // -- Get the (current) number of partners
   int nPartner = m_partnerList->getSize();
   EV << "\t has " << nPartner << " partners" << endl;
   std::vector<SEQUENCE_NUMBER_T> dup_set[nPartner-1]; // TODO: Why nPartner-1 ???

   // -- testing
   std::map<int, std::vector<SEQUENCE_NUMBER_T> > dupSet;

   // -- Reset
   //
   m_nChunkRequested_perSchedulingInterval = 0;
   int nNewChunkForRequest_perSchedulingCycle = 0;

   // -- Calculate the available time for _all_ chunk, for _all_ partners
   // (expect redundancy, but for simplicity of implementation),
   m_partnerList->resetAllAvailableTime(m_player->getCurrentPlaybackPoint(),
                                        lower_bound,
                                        m_videoBuffer->getChunkInterval());

   // -- Update bounds of all sendBM
   m_partnerList->clearAllSendBm();
   m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);
   m_partnerList->resetNChunkScheduled();

   // -------------------------------------------------------------------------
   // -- Finding the expected set (set of chunks which should be requested)
   // -------------------------------------------------------------------------
   std::vector<SEQUENCE_NUMBER_T> expected_set;
   for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
   {
      if (should_be_requested(seq_num) == false)
      {
         //EV << "----> This chunk should be requested this time" << endl;
         continue;
      }

      if (m_videoBuffer->isInBuffer(seq_num) == false)
      {
         expected_set.push_back(seq_num);
      }
   } // end of for

   int sizeExpectedSet = expected_set.size();
   std::vector<SEQUENCE_NUMBER_T> copied_expected_set = expected_set;

//   debugOUT << "List of " << sizeExpectedSet << " expected chunks to request: " << endl;

//   if (getNodeAddress() == IPvXAddress(anIP))
//   {
//      short i = 0;
//      for(std::vector<SEQUENCE_NUMBER_T>::iterator iter = expected_set.begin();
//          iter != expected_set.end(); ++iter)
//      {
//         std::cout << *iter << " ";
//         ++i;
//         if (i % 10 == 0) std::cout << endl;
//      }
//      std::cout << endl;
//   }

   // -- Have a copy of the expected_set, so that chunks which has been found a supplier will be deleted from this copy

   //std::map<SEQUENCE_NUMBER_T, std::vector<IPvXAddress> > holder;
   // -------------------------------------------------------------------------
   // -- Browse through the expected_set
   // -------------------------------------------------------------------------
   for (int i = 0; i < sizeExpectedSet; ++i)
   {
      SEQUENCE_NUMBER_T seq_num = expected_set[i];
      IpAddresses_t holderList;
      m_partnerList->getHolderList(seq_num, holderList);

      int numHolders = holderList.size();
      assert(numHolders > 0);
      if (numHolders == 1)
         m_rareSet[seq_num] = holderList[0];
      else if (numHolders > 1)
      {
         DupSet_t::iterator it= m_dupSet.find(numHolders);
         if (it == m_dupSet.end())
         {
            ChunkProviders_t chunkHolderEntry;
            chunkHolderEntry.insert(std::pair<SEQUENCE_NUMBER_T, IpAddresses_t>(seq_num, holderList));
            DupSet_t dupSetEntry;
            dupSetEntry.insert(std::pair<int, ChunkProviders_t>(numHolders, chunkHolderEntry));
         }
         else
         {
            it->second.insert(std::pair<SEQUENCE_NUMBER_T, IpAddresses_t>(seq_num, holderList));
         }
      }

      if (numHolders == 0)
      {
         ++count_0;
         continue;
      }

      //++nNewChunkForRequest_perSchedulingCycle;

      if (numHolders == 1)
      {
         ++count_1;

         // -- Get pointer to the respective NeighborInfo

         // -- Set the respective element in the SendBm to say that this chunk should be requested
         m_partnerList->setElementSendBm(holderList[0], seq_num, true);

         m_list_requestedChunk.push_back(seq_num);
         ++m_nChunkRequested_perSchedulingInterval;

         // -- Get peer's upload bandwidth

         bool ret = true;
         // -- Browse through the expected_set
         int currentSize = copied_expected_set.size();
         for (int k = 0; k < currentSize; ++k)
         {
            ret = m_partnerList->updateChunkAvailTime(holderList[0],
                                                      copied_expected_set[k],
                                                      (param_chunkSize*8)/m_partnerList->getUpBw(holderList[0]));
            if (ret == false) break;
         } // for

         if (ret == true)
         {
            //EV << "ret == true --> erase ... " << endl;
            // -- Delete the chunk whose supplier had been found
            std::vector<SEQUENCE_NUMBER_T>::iterator iter;

            iter = std::find(copied_expected_set.begin(), copied_expected_set.end(), seq_num);
            copied_expected_set.erase(iter);
         }

         // -- Recording results
         // m_reqChunkId.record(seq_num);
      }
      else // nHolder > 1
      {
         // -- Add the chunk's sequence number into a suitable dup_set
         ++count_2;
         dup_set[numHolders-2].push_back(seq_num);

         std::map<int, std::vector<SEQUENCE_NUMBER_T> >::iterator iter = dupSet.find(numHolders);
         if (iter == dupSet.end())
         {
            std::vector<SEQUENCE_NUMBER_T> tempList;
            tempList.push_back(seq_num);
            dupSet.insert(std::pair<int, std::vector<SEQUENCE_NUMBER_T> >(numHolders, tempList));
         }
         else
         {
            iter->second.push_back(seq_num);
         }
      }
   } // for (i)

//   EV << " -- Summary: " << endl
//      << " -- 0 partner(s): " << count_0 << endl
//      << " -- 1 partner(s): " << count_1 << endl
//      << " -- >1 partner(s): " << count_2 << endl
//      << " -- total: " << count_0 + count_1 + count_2 << endl
//      << " -- double check: " << sizeExpectedSet << endl;

   // --------------------------------------------------------------------------
   // -- Browse through the new map for debug purpose
   // --------------------------------------------------------------------------
//   if (getNodeAddress() == IPvXAddress(anIP))
//   {
//      debugOUT << "::Number of chunks which have >1 providers:: " << dupSet.size() << endl;
//      if (dupSet.size() > 0)
//      {
//         for (std::map<int, std::vector<SEQUENCE_NUMBER_T> >::iterator iter = dupSet.begin();
//              iter != dupSet.end(); ++iter)
//         {
//            debugOUT << "List of chunks whose have " << iter->first << " partners: " << endl;
//            for (std::vector<SEQUENCE_NUMBER_T>::iterator it = iter->second.begin();
//                 it != iter->second.end(); ++it)
//            {
//               std::cout << *it << " ";
//            }
//            std::cout << endl;
//         }
//      }
//   }

   // -- Selecting partner for chunks of different groups
   for (std::map<int, std::vector<SEQUENCE_NUMBER_T> >::iterator iter = dupSet.begin();
        iter != dupSet.end(); ++iter)
   {
      for (std::vector<SEQUENCE_NUMBER_T>::iterator it = iter->second.begin();
           it != iter->second.end(); ++it)
      {
         //SEQUENCE_NUMBER_T seq_num = dup_set[n-2][k];
         SEQUENCE_NUMBER_T seq_num = *it;
         EV << "Finding all holders for chunk " << seq_num << endl;

         // TODO: optimize this piece of code with map<seq_num, vector<ipvxaddress>> inside the first for loop
         std::vector<IPvXAddress> holderList;
         IPvXAddress candidate1, candidate2, supplier;
         int nHolder_check = 0;
         //m_partnerList->getHolderList(seq_num, holderList);

         for (std::map<IPvXAddress, NeighborInfo>::iterator i = m_partnerList->m_map.begin();
              i != m_partnerList->m_map.end(); ++i)
         {
            if (i->second.getLastRecvBmTime() != -1)
            {
               //EV << "  -- At peer " << iter->first << ": ";
               if (i->second.isInRecvBufferMap(seq_num))
               {
                  if (i->second.getNChunkScheduled() < i->second.getUploadRate_Chunk())
                  {
                     EV << "\tPartner " << i->first << " holds this chunk" << endl;
                     holderList.push_back(i->first);
                     ++nHolder_check;
                     // EV << "\tPartner " << iter->first << " HAS the chunk " << seq_num << endl;
                  }
               }
               else
               {
                  // EV << "\tPartner " << iter->first << " does NOT have chunk " << seq_num << endl;
               }
            }
            else
            {
               // EV << "\tBufferMap from " << iter->first << " is too old!" << endl;
            }
         } // for -- browse through partnerList

         int nHolder = holderList.size();
         int ret = 0;

         if (nHolder_check == 0)
         {
            // -- Inconsistence with the result found before,
            // since some partners has already been assigned enough chunk that it could send
            continue;
         }
         else if (nHolder == 1)
         {
            supplier = holderList[0];
            ret = 0;
         }
         else // nHolder >= 2
         {
            candidate1 = holderList[0];
            candidate2 = holderList[1];

            assert(!candidate1.isUnspecified());
            assert(!candidate2.isUnspecified());

            ret = selectOneCandidate(seq_num, candidate1, candidate2, supplier);

            for (int j = 2; j < nHolder; ++j)
            {
               // -- User result from the previous calculation
               candidate1 = supplier;

               // -- Update the second candidate with another partner
               candidate2 = holderList[j];

               ret = selectOneCandidate(seq_num, candidate1, candidate2, supplier);
            }
         } // if (nHolder)

         // -- Loops through all holders should be completed at this point
         if (ret == -1)
         {
            // -- Meaning that no suitable supplier has been found
            // Consider the next chunk for finding supplier
            continue;
         }

         // -- Set the respective element in the SendBm to say that this chunk should be requested
         m_partnerList->setElementSendBm(supplier, seq_num, true);

         m_list_requestedChunk.push_back(seq_num);
         ++m_nChunkRequested_perSchedulingInterval;

         // -- Get peer's upload bandwidth
         double peerUpBw = m_partnerList->getUpBw(supplier);

         // -- Browse through the expected_set
         int currentSize = copied_expected_set.size();
         for (int k = 0; k < currentSize; ++k)
         {
            //EV << "nHolder > 1; k = " << k << endl;
            m_partnerList->updateChunkAvailTime(supplier,
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

   // -- Chunk request
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
   {
      if (iter->second.isSendBmModified() == true)
      {
         EV << "-------Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
         //iter->second.printSendBm();

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

   // TOTO-Giang: Fix the moving of the buffer / sched. window
   // - to outside of the scheduling module
   // - other patterns of moving

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
int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier)
{
   int ret = 0;

   std::cout << endl;
   debugOUT << "--------------------- seq_num = " << seq_num
      << " -- candidate1 = " << candidate1
      << " -- candidate2 = " << candidate2
      << endl;

   debugOUT << "\t available time of candidate1: " << m_partnerList->getChunkAvailTime(candidate1, seq_num) << endl;
   debugOUT << "\t Time to send the chunk with candidate1: " << (param_chunkSize*8)/m_partnerList->getUpBw(candidate1) << endl;
   debugOUT << "\t chunk size (Bytes): " << param_chunkSize << endl;
   debugOUT << "\t upBW: " << m_partnerList->getUpBw(candidate1) << endl;
   if (m_partnerList->getChunkAvailTime(candidate1, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate1))
   {
      debugOUT << "--> candidate1 is qualified" << endl;

      debugOUT << "\t available time of candidate2: " << m_partnerList->getChunkAvailTime(candidate2, seq_num) << endl;
      debugOUT << "\t Time to send the chunk with candidate2: " << (param_chunkSize*8)/m_partnerList->getUpBw(candidate2) << endl;
      debugOUT << "\t chunk size (Bytes): " << param_chunkSize << endl;
      debugOUT << "\t upBW: " << m_partnerList->getUpBw(candidate2) << endl;
      if (m_partnerList->getChunkAvailTime(candidate2, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate2))
      {
         debugOUT << "--> candidate2 is ALSO qualified" << endl;

         debugOUT << "UpBW of candidate1: " << m_partnerList->getUpBw(candidate1) << endl;
         debugOUT << "UpBW of candidate2: " << m_partnerList->getUpBw(candidate2) << endl;
         if (m_partnerList->getUpBw(candidate1) > m_partnerList->getUpBw(candidate2))
         {
            debugOUT << "Candidate1 is better!" << endl;
            supplier = candidate1;
         }
         else if (m_partnerList->getUpBw(candidate1) < m_partnerList->getUpBw(candidate2))
         {
            debugOUT << "Candidate2 is better!" << endl;
            supplier = candidate2;
         }
         else
         {
            debugOUT << "Both candidates are equally good, select a any of them" << endl;
            if (intrand(10) % 2 == 0)
               supplier = candidate2;
            else
               supplier = candidate1;
         }
      }
      else
      {
         debugOUT << "--> candidate1 is qualified" << endl;
         supplier = candidate1;
      }
   }
   else
   {
      debugOUT << "--> candidate1 is NOT qualified" << endl;
      if (m_partnerList->getChunkAvailTime(candidate2, seq_num) > (param_chunkSize*8)/m_partnerList->getUpBw(candidate2))
      {
         debugOUT << "--> candidate2 is qualified --> it will be selected" << endl;
         supplier = candidate2;
      }
      else
      {
         debugOUT << "Both candidates were /fully/ booked with requested chunks!" << endl;
         ret = -1;
         // -- Just a /Padding code/
         supplier = candidate1;
      }
   }

   return ret;
}

