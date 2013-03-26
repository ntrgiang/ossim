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
// Forwarder.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef FORWARDER_H_
#define FORWARDER_H_

#include "CommBase.h"
#include "VideoBuffer.h"
#include "Dispatcher.h"

//struct RecordCountChunk
//{
//   long int m_chunkSent;
//   long int m_chunkReceived;

//   long int m_prev_chunkSent;
//   long int m_prev_chunkReceived;
////   long int m_chunkExchanged;
//   double m_oriTime; // Time when the record was created for the first time
//};

class RecordCountChunk
{
public:
   void print(void);

private:
   long int m_chunkSent;
   long int m_chunkReceived;

   long int m_prev_chunkSent;
   long int m_prev_chunkReceived;
//   long int m_chunkExchanged;
   double m_oriTime; // Time when the record was created for the first time

   friend class Forwarder;
   friend class DonetPeer;
};

class Forwarder : public CommBase {
public:
   Forwarder();
   virtual ~Forwarder();

   virtual void handleMessage(cMessage* msg);
   virtual int numInitStages() const { return 4; }
   virtual void initialize(int stage);
   virtual void finish();

   inline long int getCountTotalChunkIncoming(void) { return m_count_totalChunk_incoming; }

   void sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort);
   void getRecordChunk(IPvXAddress &addr, RecordCountChunk&);
   const RecordCountChunk & getRecordChunk(IPvXAddress &addr);

   void updateSentChunkRecord(IPvXAddress &destAddress);
   void updateReceivedChunkRecord(IPvXAddress &senderAddress);

   void addRecord(const IPvXAddress & address);
   void removeRecord(const IPvXAddress & address);

   void printRecord(void);

private:
    // -- Utility functions
    // int getLocalPort();

protected:
    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    Dispatcher *m_dispatcher;

    std::map<IPvXAddress, RecordCountChunk> m_record_countChunk;
    long int m_count_totalChunk_incoming; // from all partners
};

#endif /* FORWARDER_H_ */
