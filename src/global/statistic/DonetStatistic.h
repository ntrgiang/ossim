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
// DonetStatistic.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "IChurnGenerator.h"
#include "NotificationBoard.h"
#include "AppCommon.h"
#include "ActivePeerTable.h"
#include "IPvXAddress.h"
#include <fstream>
#include "StatisticBase.h"
#include "StreamingStatistic.h"

#ifndef DONET_STATISTIC_H_
#define DONET_STATISTIC_H_

class ActivePeerTable;

//class GlobalStatistic : public cSimpleModule, protected INotifiable
//class DonetStatistic : public StatisticBase
class DonetStatistic : public StreamingStatistic
{
public:
    DonetStatistic();
    virtual ~DonetStatistic();

    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);

    virtual void handleMessage(cMessage *msg);
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

    virtual void finish();

    // -- For cListener (maybe!)
//    virtual void receiveSignal(cComponent *src, simsignal_t id, long l);
//    virtual void receiveSignal(cComponent *src, simsignal_t id, cObject* obj);

private:
    void handleTimerMessage(cMessage *msg);

// -- Real interfaces
public:
    void writeActivePeerTable2File(vector<IPvXAddress>);
    void writePartnerList2File(IPvXAddress node, vector<IPvXAddress> pList);
    void writePartnership2File(IPvXAddress local, IPvXAddress remote);

    void recordSizeInView(int size);
    void recordSizePartialView(int size);

    double getAverageSizeInView(void);
    double getAverageSizePartialView(void);

    void recordJoinTime(double time);

    void collectSizeIV(int size);
    void collectSizePV(int size);

    void increaseNEW(void);
    void decreaseNEW(void);
    void increaseIGN(void);
    void increaseACK(void);

    long getNEW(void);
    long getIGN(void);
    long getACK(void);

    // -- Interface to get the final average of each run
    void recordPartialViewSize(int size);

    // -- Interface to get the histogram of view sizes
    void collectPartialViewSize(int size);
    void collectInViewSize(int size);

    // -- For drawing the histogram of partial view sizes
    void collectAllPVsizes(int size);

    // -- For checking the number of new App messages created and deleted
    //inline void incrementCountAppMsg(void) { ++m_countAppMsgNew; }
    //inline void decrementCountAppMsg(void) { --; }
    inline void incrementCoundSelfMsg(void) { ++m_countSelfAppMsg; }

    // -- "Reach ratio"
    //inline void incrementCountReach(void) { ++m_countReach; }

    // -- Interface to emit signal for global statistic collection

    // -- To calculate CI or loss ratio
    //
    virtual void incrementChunkHit(const SEQUENCE_NUMBER_T &seq_num);
    virtual void incrementChunkMiss(const SEQUENCE_NUMBER_T &seq_num);
    //void reportChunkSeek(const SEQUENCE_NUMBER_T &seq_num);
    void reportRebuffering(const SEQUENCE_NUMBER_T &seq_num);

    // -- Collect stats @ finishing step
    //
    virtual void addFinalChunkHit(const long &delta);
    virtual void addFinalChunkMiss(const long &delta);
    virtual void addFinalAllChunk(const long &delta);

    void reportStall(); // should be obsolete

    void reportSkipChunk(void);
    void reportRebuffering(void);
    void reportStallDuration(double dur);
    void reportStallDuration(void); // overload the above function

    void reportRequestedChunk(const SEQUENCE_NUMBER_T &seq_num);
    void reportDuplicatedChunk(const SEQUENCE_NUMBER_T &seq_num);
    void reportLateChunk(const SEQUENCE_NUMBER_T &seq_num);
    void reportInrangeChunk(const SEQUENCE_NUMBER_T &seq_num);

    void reportMeshJoin();

    void reportNumberOfPartner(int nPartner);
    void reportNumberOfPartner(const IPvXAddress &addr, const int &nPartner);
    void reportNumberOfJoin(int val);

    //
    // Currently used --->
    //
    void reportSystemSize(void);

    // -- Function to produce signals to collect stats about CI
    void collectCI(void);
    void collectSkipChunk(void);
    void collectStallDuration(void);
    void collectRebuffering(void);

    void reportDelays(void);

    // -- To calculate Continuity Index
    //
    void increaseChunkHit(const int &delta);
    void increaseChunkMiss(const int &delta);

    // -- To calculate Delay
    void collectDeltaDelayOneOverlayHop(const double &);
    void collectDeltaOverlayHopCount(const long &);
    void collectDeltaNumberOfReceivedChunk(const long &);
    // <--- Currently used

    void collectPlaybackPoint(SEQUENCE_NUMBER_T seq);

private:
    void printActivePeerList(void);

private:
    NotificationBoard *nb; // cached pointer
    ActivePeerTable *m_apTable; // To get the number of active node for averaging recorded results

    map<IPvXAddress, int> m_peerList;

    // -- Parameters
    double param_interval_reportCI;
    double param_interval_reportSystemSize;

    // -- Messages
    cMessage *timer_reportCI;
    cMessage *timer_reportSystemSize; // to tell instantiated number of active peers in the system

    cOutVector m_joinTime;

    ofstream m_outFile;

    // -- Counting subscription messages
    long m_count_NEW;
    long m_count_ACK;
    long m_count_IGN;

    // -- For drawing the histogram of partial view sizes
    cOutVector m_allFinalPVsizes;

    // -- For checking the number of new App messages created and deleted
    //long m_countAppMsgNew;
    long m_countSelfAppMsg;

    // -- Calculate the the "reach ratio"
    //long m_countReach;

    // -- Temporary variables
    long m_count_chunkHit;
    long m_count_chunkMiss;
    long m_count_allChunk;

    long m_count_chunkHit_prev;
    long m_count_allChunk_prev;

    // -- Regarding statistics from all Players
    long m_count_skipChunk;
    long m_count_rebuffering;
    long double m_count_stallDuration;
    long m_count_stallDuration_chunk;

    // -- Calculate delays (end-to-end, overlay hop count, underlay hop count
    long double   m_totalEndToEndDelay;
    long long     m_totalNumberOfReceivedChunk;
    long long     m_totalOverlayHopCount;

    // ---------------------------- Signals ------------------------------------
    simsignal_t sig_dummy_chunkHit;

    simsignal_t sig_chunkHit;
    simsignal_t sig_chunkMiss;
    simsignal_t sig_allChunk;

    simsignal_t sig_ci;
    simsignal_t sig_loss;

    simsignal_t sig_stall; // should be obsolete

    simsignal_t sig_skipChunk;
    simsignal_t sig_rebuffering;
    simsignal_t sig_stallDuration;
    simsignal_t sig_ci_delta;
    simsignal_t sig_systemSize;

    // -- Delays
    simsignal_t sig_DelayOneOverlayHop;
    simsignal_t sig_overlayHopCount;

    // should be obsolete
    simsignal_t sig_meshJoin;

    simsignal_t sig_nPartner;
    simsignal_t sig_nJoin;

    simsignal_t sig_requestedChunk, sig_receivedChunk, sig_duplicatedChunk, sig_lateChunk, sig_inrangeChunk;

    // -- Playback lags
    simsignal_t sig_playback;
};

#endif /* DONET_STATISTIC_H_ */
