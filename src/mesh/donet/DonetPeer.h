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
// DonetPeer.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef DONETPEER_H_
#define DONETPEER_H_

#define __DONET_PEER_DEBUG__ true

#include "DonetBase.h"
#include "IChurnGenerator.h"
#include "PlayerBase.h"
#include <queue> // for std::queue
//#include "Player.h"

#define INIT_SCHED_WIN_GOOD   1
#define INIT_SCHED_WIN_BAD    0

/*
 * Suppose to replace:
 * - m_seqNum_schedWinHead
 * - m_seqNum_schedWinEnd
 */
struct SchedulingWindow
{
   SEQUENCE_NUMBER_T end, start;
};

struct DataExchange
{
   double m_time;
   double m_throughput; // overall throughput since partnership establishment
   double m_throughput_interval; // throughput between the two most recent sampling
};

typedef std::map<SEQUENCE_NUMBER_T, double> AllTimeBudget_t;
typedef std::map<IPvXAddress, AllTimeBudget_t> AllPartnerTimeBudget_t;

class DonetPeer : public DonetBase
{
public:
    DonetPeer();
    virtual ~DonetPeer();

    void gracefulLeave(void);

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

// Helper functions
private:
    virtual void processPacket(cPacket *pkt);
    void handleTimerMessage(cMessage *msg);

    void bindToGlobalModule(void);
    void bindToMeshModule(void);

    void getAppSetting();
//    void readChannelRate(void);

    void cancelAndDeleteAllTimer();
    void cancelAllTimer();

    // *************************************************************************
    // *************************************************************************
    // -- Partnership
    void handleTimerGetJoinTime(void);
    void handleTimerJoin(void);
    void handleTimerLeave();
    void handleTimerFindMorePartner(void);
    //void handleTimerTimeoutWaitingAccept();
    void handleTimerPartnershipRefinement(void);
    //void handleTimerPartnerlistCleanup(void);

    void findPartner(); // New interface for the FSM
    void sendPartnershipRequest(void);
    void processPartnershipRequest(cPacket *pkt);
    void processPartnershipAccept(cPacket *pkt);
    void processPartnershipReject(cPacket *pkt);
    void processPartnershipDisconnect(cPacket* pkt);
    void addPartner(IPvXAddress remote, double bw);

    void processTimeoutJoinRequestAccept(cMessage *msg);

    void updateDataExchangeRecord(double samplingInterval);

    void handleTimerReportStatistic();
    void handleTimerReportActive();

    // !!! obsolete !!!
//    void join();
//    void findMorePartner();
    //void processRejectResponse(cPacket *pkt);
    // *************************************************************************
    // *************************************************************************

    void sendBufferMap();
    void processPeerBufferMap(cPacket *pkt);
//    void processChunkRequest(cPacket *pkt);

    // -- State variables for join process
    int m_nRequestSent;
    int m_nRejectSent;
    bool m_timerDeleted;

    // Chunk scheduling
    // bool should_be_requested(void);
    bool should_be_requested(SEQUENCE_NUMBER_T seq_num);
    //void initializeSchedulingWindow(void);
    int initializeSchedulingWindow(void);
    void initializeSchedulingWindow2(void);
    bool shouldStartChunkScheduling();
    void chunkScheduling(void);
    void randomChunkScheduling(SEQUENCE_NUMBER_T lower_bound, SEQUENCE_NUMBER_T upper_bound);
    void donetChunkScheduling(SEQUENCE_NUMBER_T lower_bound, SEQUENCE_NUMBER_T upper_bound);
    void RF_RandomChunkScheduling(SEQUENCE_NUMBER_T lower_bound, SEQUENCE_NUMBER_T upper_bound);
    int selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier, AllPartnerTimeBudget_t &);
    void findExpectedSet(SEQUENCE_NUMBER_T, SEQUENCE_NUMBER_T, SEQUENCE_NUMBER_T);
    void printExpectedSet(void);

    void updateRange(void);
    void reportLocalStatistic(void);
    void reportHitMiss();

    //int numberOfChunkToRequestPerCycle(void); // TODO: (Giang) obsolete?
    double currentRequestGreedyFactor(void);
    bool recentlyRequestedChunk(SEQUENCE_NUMBER_T seq_num);
    void refreshListRequestedChunk(void);
    void printListOfRequestedChunk(void);
    //bool inScarityState(void); // TODO: (Giang) obsolete?

    MeshPartnershipLeavePacket* generatePartnershipRequestLeavePacket();
    void rejoin(void);
    void checkVideoBuffer(void);
//    bool shouldStartPlayer(void);
//    void startPlayer(void);

private:
// -----------------------------------------------------------------------------
//                               Timers
// -----------------------------------------------------------------------------
    cMessage *timer_getJoinTime;
    cMessage *timer_join;
    cMessage *timer_leave;
    cMessage *timer_chunkScheduling;
    cMessage *timer_findMorePartner;
    cMessage *timer_startPlayer;
    cMessage *timer_timeout_joinReqAccept;
    cMessage *timer_reportStatistic;
    cMessage *timer_reportActive;

    //cMessage *timer_timeout_waiting_accept;
    //cMessage *timer_timeout_waiting_ack;
    cMessage *timer_timeout_waiting_response;
    cMessage *timer_partnershipRefinement;
    cMessage *timer_checkVideoBuffer;

// -----------------------------------------------------------------------------
//                               Parameters
// -----------------------------------------------------------------------------
    bool    m_debug_scheduling;
    double  param_chunkSchedulingInterval;
    double  param_interval_chunkScheduling;     // as a potential replacement for the above
    double  param_waitingTime_SchedulingStart;
    int     param_nNeighbor_SchedulingStart;

    double  param_interval_findMorePartner;
    double  param_interval_starPlayer;
    double  param_interval_rejoin;
    double  param_interval_timeout_joinReqAck;
    double  param_interval_waitingPartnershipResponse;
    double  param_interval_partnershipRefinement;
    //double  param_interval_partnerlistCleanup;
    double  param_interval_reportStatistic;
    static double  param_interval_reportActive;

    //double  param_baseValue_requestGreedyFactor;
    //double  param_aggressiveValue_requestGreedyFactor
    //double  param_requestFactor_moderate;
    //double  param_requestFactor_aggresive;
    double  param_factor_requestList;
    //double  param_threshold_scarity;
    double  param_threshold_idleDuration_buffermap;

    // -- Partnership size
    int param_minNOP;
    int param_offsetNOP;

    // Address of the host whose response is expected
    // TODO: (Giang) obsoleted?
//    IPvXAddress address_responseExpected;

    // -- To store list of random peers got from APT
    //vector<IPvXAddress> m_list_randPeer;

// -----------------------------------------------------------------------------
//               Pointers to external modules
// -----------------------------------------------------------------------------
    // -- Pointers to "global" modules
    IChurnGenerator *m_churn;

    // -- Pointer to external modules
    PlayerBase *m_player;
//    StreamingStatistic *m_gstat;


    // -- Features
    //
    bool m_pRefinementEnabled;

    // State variables
    bool m_scheduling_started;
    bool m_sched_window_moved;

    // Variables to store history
    double m_firstJoinTime;

    // variables to keep up-to-date with
    SEQUENCE_NUMBER_T m_seqNum_schedWinStart;
    SEQUENCE_NUMBER_T m_seqNum_schedWinEnd;
    SEQUENCE_NUMBER_T m_seqNum_schedWinHead;

    // -- Scheduling
    int m_nChunkRequested_perSchedulingInterval;
    //int m_nChunk_perSchedulingInterval;
    int m_nChunk_toRequest_perCycle;
    int m_schedulingWindowSize;     /* in [chunks] */
    SchedulingWindow m_sched_window;

    typedef std::vector<IPvXAddress> IpAddresses_t;
    typedef std::map<SEQUENCE_NUMBER_T, IpAddresses_t> ChunkProviders_t;
    typedef std::map<int, ChunkProviders_t> DupSet_t;

    DupSet_t m_dupSet;
    std::map<SEQUENCE_NUMBER_T, IPvXAddress> m_rareSet;
    std::vector<SEQUENCE_NUMBER_T> m_expected_set;

    // -- Easy version with a vector
    vector<SEQUENCE_NUMBER_T> m_list_requestedChunk;
    std::queue<int> m_numRequestedChunks;

    // Ranges of received buffer maps
    SEQUENCE_NUMBER_T m_minStart, m_maxStart, m_minHead, m_maxHead;
    bool flag_rangeUpdated;

    bool flag_partnershipRefinement;

    // -- The number of chunks received previously at the Video Buffer
    long int m_prevNChunkReceived;
    int m_downloadRate_chunk;

    // -- For reporting statistics
    long int m_count_prev_chunkHit, m_count_prev_chunkMiss;
    int m_count_rejoin;

    // --------------------------- Optimization --------------------------------
    //std::vector<IPvXAddress> m_blacklist;

    // -------------------------------- Signals --------------------------------

    // -- For ranges of received buffer maps, and current playback point
       simsignal_t sig_currentPlaybackPoint;
       simsignal_t sig_minStart;
       simsignal_t sig_maxStart;
       simsignal_t sig_minHead;
       simsignal_t sig_maxHead;
       simsignal_t sig_bufferStart;
       simsignal_t sig_bufferHead;

       simsignal_t sig_schedWin_start;
       simsignal_t sig_schedWin_end;

       simsignal_t sig_localCI;

    // -- Chunks
       simsignal_t sig_chunkRequestSeqNum;
       simsignal_t sig_newchunkForRequest;
       simsignal_t sig_nChunkRequested;

    //simsignal_t sig_partnerRequest;

    // -- Partnership
      // Number of partners
      simsignal_t sig_nPartner;

      // Number of requests SENT & RECV
      simsignal_t sig_pRequestSent;

      // Number of rejects SENT & RECV
      simsignal_t sig_pRejectRecv;

    simsignal_t sig_nJoin;

    simsignal_t sig_joinTime;
    simsignal_t sig_playerStartTime;

    // -- Throughputs
    simsignal_t sig_inThroughput; // chunks / second


    // Accounting
    simsignal_t sig_timeout;

    simsignal_t sig_nBufferMapReceived;

    // -- Delays measurement
    simsignal_t sig_e2eDelay;
    simsignal_t sig_overlayHopCount;

};

#endif // DONETPEER_H_
