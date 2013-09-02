
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
#include <iomanip> // setw()

using namespace std;

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "@" << simTime().dbl() << " DonetScheduling:: "
#endif

#define anIP "192.168.0.2"

/**
 * A very straight-forward version of the implementation --> should not expect that it is smart
 */
void DonetPeer::donetChunkScheduling(SEQUENCE_NUMBER_T lower_bound, SEQUENCE_NUMBER_T upper_bound)
{
   Enter_Method("donetChunkScheduling()");
   debugOUT << "Donet chunk scheduling" << endl;

   // -- Reset
   //
   m_expected_set.clear();
   m_dupSet.clear();
   m_rareSet.clear();

   m_nChunkRequested_perSchedulingInterval = 0;
   //int nNewChunkForRequest_perSchedulingCycle = 0;

   // -- Calculate the available time for _all_ chunk, for _all_ partners
   // (expect redundancy, but for simplicity of implementation),
   //m_partnerList->resetAllAvailableTime(m_player->getCurrentPlaybackPoint(),
   //                                     new_lower_bound,
   //                                     m_videoBuffer->getChunkInterval());

   SEQUENCE_NUMBER_T currentPlaybackPoint = m_player->getCurrentPlaybackPoint();
   //m_partnerList->resetAllAvailableTime2(lower_bound, m_videoBuffer->getChunkInterval()); // obsolete

   // -- Update bounds of all sendBM
   m_partnerList->clearAllSendBm();
   m_partnerList->updateBoundSendBm(lower_bound, lower_bound + m_bufferMapSize_chunk - 1);
   //m_partnerList->updateBoundSendBm(lower_bound, upper_bound);
   m_partnerList->resetNChunkScheduled();

   // -------------------------------------------------------------------------
   // -- Finding the expected set (set of chunks which should be requested)
   // -------------------------------------------------------------------------
//   debugOUT << "lower bound: " << lower_bound << " -- upper bound: " << upper_bound << endl;

   findExpectedSet(currentPlaybackPoint, lower_bound, upper_bound);

   debugOUT << "Number of cycles " << m_numRequestedChunks.size() << endl;
   //printListOfRequestedChunk();

   debugOUT << "expected set size: " << m_expected_set.size() << endl;
   //printExpectedSet();

   AllTimeBudget_t chunkAvailableTime;
   for (std::vector<SEQUENCE_NUMBER_T>::iterator iter = m_expected_set.begin(); iter != m_expected_set.end(); ++iter)
   {
      // All chunks in this list are further behind the playback point
      double availableTime = (*iter - currentPlaybackPoint) / m_videoStreamChunkRate;
      chunkAvailableTime.insert(std::pair<SEQUENCE_NUMBER_T, double>(*iter, availableTime));
   }

   // --- Debug
   //   debugOUT << "Time budget (compared to PB point " << currentPlaybackPoint
   //            << ", video stream " << m_videoStreamChunkRate << " (chunk/s)): " << endl;
   //   for (AllTimeBudget_t::iterator iter = chunkAvailableTime.begin(); iter != chunkAvailableTime.end(); ++iter)
   //   {
   //      debugOUT << "*** chunk " << iter->first << " -- time " << iter->second << " seconds" << endl;
   //   }

   AllPartnerTimeBudget_t allTimeBudget;

   std::vector<IPvXAddress> allPartner = m_partnerList->getAddressList();
   assert(allPartner.size() > 0);

   for(std::vector<IPvXAddress>::iterator iter = allPartner.begin(); iter != allPartner.end(); ++iter)
   {
      allTimeBudget.insert(std::pair<IPvXAddress, AllTimeBudget_t>(*iter, chunkAvailableTime));
   }

   // --- Debug
//      debugOUT << "All time budget: " << endl;
//      for (AllPartnerTimeBudget_t::iterator iter = allTimeBudget.begin(); iter != allTimeBudget.end(); ++iter)
//      {
//         debugOUT << "\t Of Partner " << iter->first << endl;
//         for (AllTimeBudget_t::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
//         {
//            debugOUT << "\t*** chunk " << it->first << " -- time " << it->second << " seconds" << endl;
//         }
//      }

   // -- Have a copy of the expected_set, so that chunks which has been found a supplier will be deleted from this copy

   // -------------------------------------------------------------------------
   // -- Browse through the expected_set
   // -------------------------------------------------------------------------
   int sizeExpectedSet = m_expected_set.size();
   for (int i = 0; i < sizeExpectedSet; ++i)
   {
      SEQUENCE_NUMBER_T seq_num = m_expected_set[i];
      IpAddresses_t holderList;
      m_partnerList->getHolderList(seq_num, holderList);

      int numHolders = holderList.size();
      assert(numHolders >= 0);

      //debugOUT << "chunk " << seq_num << " -- " << numHolders << " holders" << endl;
      if (numHolders == 0)
      {
         continue;
      }
      else if (numHolders == 1)
      {
         m_rareSet[seq_num] = holderList[0];
      }
      else
      {
         DupSet_t::iterator it= m_dupSet.find(numHolders);
         if (it == m_dupSet.end())
         {
            ChunkProviders_t chunkHolderEntry;
            chunkHolderEntry.insert(std::pair<SEQUENCE_NUMBER_T, IpAddresses_t>(seq_num, holderList));
            m_dupSet.insert(std::pair<int, ChunkProviders_t>(numHolders, chunkHolderEntry));
         }
         else
         {
            it->second.insert(std::pair<SEQUENCE_NUMBER_T, IpAddresses_t>(seq_num, holderList));
         }
      }
   } // for (i)

   // --- DEBUG: Browse through the created map
      debugOUT << "Rare set:" << endl;
      if (m_rareSet.size() == 0)
      {
         debugOUT << "rare set has no element" << endl;
      }
      else
      {
//         for (std::map<SEQUENCE_NUMBER_T, IPvXAddress>::iterator iter = m_rareSet.begin(); iter != m_rareSet.end(); ++iter)
//         {
//            //debugOUT << "chunk " << iter->first << " -- provider " << iter->second << endl;
//            //cout << iter->first << " ";
//         }
//         cout << endl;
      }
         debugOUT << "dupset has " << m_dupSet.size() << " types of chunks:" << endl;
         for(DupSet_t::iterator iter = m_dupSet.begin(); iter != m_dupSet.end(); ++iter)
         {
            debugOUT << "Chunks with " << iter->first << " providers:" << endl;
            for(ChunkProviders_t::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
            {
               debugOUT << "\tChunk " << it->first << " with provider(s): " << endl;
               for (IpAddresses_t::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
               {
                  debugOUT << "\t\t " << *it2 << endl;
               }
            }
         }

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

// -----------------------------------------------------------------------------
//                               Chunk request
// -----------------------------------------------------------------------------

   // --- Request rarest chunk first
   //
   //ChunkProviders_t rarestChunks = m_dupSet[1]; // map: (seq_num, IP)
   // --- debug
   //debugOUT << rarestChunks.size() << " rarest chunks " << endl;
   for (std::map<SEQUENCE_NUMBER_T, IPvXAddress>::iterator iter = m_rareSet.begin(); iter != m_rareSet.end(); ++iter)
   {
      SEQUENCE_NUMBER_T seq_num = iter->first;

      if (seq_num > lower_bound + m_bufferMapSize_chunk - 1) continue;

      IPvXAddress provider = iter->second;
      double time_to_send_Chunk = (double)param_chunkSize * 8 / m_partnerList->getUpBw(provider);

//      debugOUT << "\t *** Chunk " << iter->first << " --- provider " << provider << endl;

      // -- Check time budget
//      debugOUT << "time budget for chunk " << seq_num << ": " << allTimeBudget[provider][seq_num]
//                  << " -- time to send chunk: " << time_to_send_Chunk << endl;

      if (allTimeBudget[provider][seq_num] < time_to_send_Chunk)
      {
         debugOUT << "provider does NOT have enough available time" << endl;
         continue;
      }

      // -- Set the respective element in the SendBm to say that this chunk should be requested
      m_partnerList->setElementSendBm(provider, seq_num, true);
      m_list_requestedChunk.push_back(seq_num);
      ++m_nChunkRequested_perSchedulingInterval;

      // --- Debug
      //      debugOUT << "Time budget before adjustment: " << endl;
      //      for (AllTimeBudget_t::iterator it = allTimeBudget[provider].begin(); it != allTimeBudget[provider].end(); ++it)
      //      {
      //         cout << "chunk " << it->first << " -- time " << setprecision(6) << it->second << " (s)" << endl;
      //      }

      for (AllTimeBudget_t::iterator it = allTimeBudget[provider].find(seq_num); it != allTimeBudget[provider].end(); ++it)
      {
         it->second -= time_to_send_Chunk;
      }

      // --- Debug
      //      debugOUT << "Time budget after adjustment: " << endl;
      //      for (AllTimeBudget_t::iterator it = allTimeBudget[provider].begin(); it != allTimeBudget[provider].end(); ++it)
      //      {
      //         cout << "chunk " << it->first << " -- time " << setprecision(6) << it->second << " (s)" << endl;
      //      }

   } // for each rarest chunk

   // -- Sending chunk request packets
   //
   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
        iter != m_partnerList->m_map.end(); ++iter)
   {
      if (iter->second.isSendBmModified() == false)
         continue;

      MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket;
         chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
         // -- Map the sendBM into ChunkRequestPacket
         iter->second.copyTo(chunkReqPkt);
         sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);

         debugOUT << "chunk request sent!" << endl;
   }

   // -- Prepare for the next requests
   //
   m_partnerList->clearAllSendBm();
   m_partnerList->updateBoundSendBm(lower_bound, lower_bound + m_bufferMapSize_chunk - 1);

   // -- Request chunks with more than one providers
   //
   for(DupSet_t::iterator iter = m_dupSet.begin(); iter != m_dupSet.end(); ++iter)
   {
      if (iter->first == 1) continue;

      // -------------
      // numHoders > 1
      // -------------

      int nHolder = iter->first;
      for(ChunkProviders_t::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
      {
         //assert(it->second.size() == 2);

         SEQUENCE_NUMBER_T seq_num = it->first;

         if (seq_num > lower_bound + m_bufferMapSize_chunk - 1) continue;

         IPvXAddress candidate1, candidate2, provider;

         candidate1 = it->second[0];
         candidate2 = it->second[1];

         int ret = selectOneCandidate(seq_num, candidate1, candidate2, provider, allTimeBudget);
         //if (ret == -1) continue;

         for (int j = 2; j < nHolder; ++j)
         {
            candidate1 = provider;
            candidate2 = it->second[j];

            ret = selectOneCandidate(seq_num, candidate1, candidate2, provider, allTimeBudget);
         }
         if (ret == -1) continue;

         m_partnerList->setElementSendBm(provider, seq_num, true);
         m_list_requestedChunk.push_back(seq_num);
         ++m_nChunkRequested_perSchedulingInterval;

         // -- Update the time budget for other chunks
         //
         double time_to_send_Chunk = (double)param_chunkSize * 8 / m_partnerList->getUpBw(provider);
         for (AllTimeBudget_t::iterator it2 = allTimeBudget[provider].find(seq_num); it2 != allTimeBudget[provider].end(); ++it2)
         {
            //debugOUT << "available time of chunk " << it2->first << endl;
            //debugOUT << "\t before update: " << it2->second << endl;
            it2->second -= time_to_send_Chunk;
            //debugOUT << "\t after update: " << it2->second << endl;
            //debugOUT << "Using another way to get result: " << m_partnerList->getChunkAvailTime(provider, seq_num) << endl;

         }
      } // for each chunk
   } // for each group

   //emit(sig_newchunkForRequest, nNewChunkForRequest_perSchedulingCycle);

   // -- Browse through the list of partners to see which one have been set the sendBm
   // -- For each of those one, prepare a suitable ChunkRequestPacket and send to that one
   // -- Chunk request
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
   {
      if (iter->second.isSendBmModified() == true)
      {
         //iter->second.printSendBm();

         MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket;
         chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
         // -- Map the sendBM into ChunkRequestPacket
         iter->second.copyTo(chunkReqPkt);

         // -- Send the copy
         sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
      }
   } // end of for

   m_numRequestedChunks.push(m_nChunkRequested_perSchedulingInterval);
   // -- Now refreshing the m_list_requestedChunk
   refreshListRequestedChunk();

//   if (upper_bound > lower_bound + m_bufferMapSize_chunk - 1)
//   {
//      cout << "@ " << simTime().dbl() << " " << getNodeAddress() << endl
//           << "\t -- request range " << lower_bound << " - " << lower_bound + m_bufferMapSize_chunk - 1 << endl
//           << "\t -- head received " << m_videoBuffer->getHeadReceivedSeqNum() << endl
//           << "\t -- max head " << upper_bound << endl
//           << "\t -- numReqChunks " << m_nChunkRequested_perSchedulingInterval << endl
//           << "\t -- player:: missed " << m_player->getCountChunkMiss()
//           << endl;
//   }

   // -- Report statistics
   //emit(sig_nChunkRequested, m_nChunkRequested_perSchedulingInterval);
   //emit(sig_schedWin_start, m_sched_window.start);
   //emit(sig_schedWin_end, m_sched_window.end);

//   if (getNodeAddress() == IPvXAddress("192.168.2.162"))
//   {
//       std::cout << "\t Number of chunks to request: " << m_expected_set.size() << endl;
//       std::cout << "\t Number of requested chunks: " << m_nChunkRequested_perSchedulingInterval << endl;
//   }

} // Donet Chunk Scheduling

/**
 * Used for Donet chunk scheduling
 */
int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier, AllPartnerTimeBudget_t &allTimeBudget)
{
   int ret = 0;

    //double availableTime1 = m_partnerList->getChunkAvailTime(candidate1, seq_num); // obsolete
    //double availableTime2 = m_partnerList->getChunkAvailTime(candidate2, seq_num); // obsolete
   double availableTime1 = allTimeBudget[candidate1][seq_num];
   double availableTime2 = allTimeBudget[candidate2][seq_num];

   double bw1 = m_partnerList->getUpBw(candidate1);
   double bw2 = m_partnerList->getUpBw(candidate2);

   double chunkSendingTime1 = ((double)param_chunkSize*8) / bw1;
   double chunkSendingTime2 = ((double)param_chunkSize*8) / bw2;

   //std::cout << endl;
   debugOUT << "--------------------- seq_num = " << seq_num << endl
      << "\t -- candidate1 = " << candidate1 << " with available time " << availableTime1
      << "\t -- candidate2 = " << candidate2 << " with available time " << availableTime2
      << endl;

   debugOUT << "\t available time of candidate1: " << setprecision(8) << availableTime1 << endl;
   debugOUT << "\t Time to send the chunk with candidate1: " << chunkSendingTime1 << endl;
   debugOUT << "\t chunk size (Bytes): " << param_chunkSize << endl;
   debugOUT << "\t upBW: " << bw1 << endl;
   if (availableTime1 > chunkSendingTime1)
   {
      debugOUT << "\t --> candidate1 is qualified" << endl;

      debugOUT << "\t available time of candidate2: " << setprecision(8) << availableTime2 << endl;
      debugOUT << "\t Time to send the chunk with candidate2: " << chunkSendingTime2 << endl;
      debugOUT << "\t chunk size (Bytes): " << param_chunkSize << endl;
      debugOUT << "\t upBW: " << bw2 << endl;
      if (availableTime2 > chunkSendingTime2)
      {
         debugOUT << "--> candidate2 is ALSO qualified" << endl;

         debugOUT << "UpBW of candidate1: " << bw1 << endl;
         debugOUT << "UpBW of candidate2: " << bw2 << endl;
         if (bw1 > bw2)
         {
            debugOUT << "Candidate1 is better!" << endl;
            supplier = candidate1;
         }
         else if (bw1 < bw2)
         {
            debugOUT << "Candidate2 is better!" << endl;
            supplier = candidate2;
         }
         else
         {
            supplier = (intrand(10) % 2 == 0) ? candidate2 : candidate1;
            debugOUT << "Both candidates are equally good, select a any of them: " << supplier << endl;
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
//      debugOUT << "--> candidate1 is NOT qualified" << endl;
      if (availableTime2 > chunkSendingTime2)
      {
//         debugOUT << "--> candidate2 is qualified --> it will be selected" << endl;
         supplier = candidate2;
      }
      else
      {
//         debugOUT << "Both candidates were /fully/ booked with requested chunks!" << endl;
         ret = -1;
         // -- Just a /Padding code/
         supplier = candidate1;
      }
   }

   return ret;
}
