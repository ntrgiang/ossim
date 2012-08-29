#ifndef DONETPEER_H_
#define DONETPEER_H_

#define __DONET_PEER_DEBUG__ true

#include "DonetBase.h"
#include "IChurnGenerator.h"
#include "Player.h"

class DonetPeer : public DonetBase
{
public:
    DonetPeer();
    virtual ~DonetPeer();

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

    // -- Partnership
    void join();
    void findMorePartner();
    void processPartnershipRequest(cPacket *pkt);
    void processAcceptResponse(cPacket *pkt);
    void processRejectResponse(cPacket *pkt);
    void processPeerBufferMap(cPacket *pkt);
//    void processChunkRequest(cPacket *pkt);

    // -- State variables for join process
    int m_nRequestSent;
    int m_nRejectSent;

    // temp one:
    // void processPeerBufferMap2(cPacket *pkt);

    // Heartbeat <-- might not be used
    // void processKeepAliveMessage(cPacket *pkt);
    // void startSendingHeartbeat();
    // void sendKeepAliveMsgToNeighbor();
//    void acceptJoinRequest(cPacket *msg);

    /*
     * This method adds the address of the packet's source node into the local Neighbor List
     */
//    void addToNeighborList(cPacket *pkt);
//
//    bool canHaveMorePartner(void);
//
//    // BufferMap
//    void sendBufferMap(void);
//    inline int getBufferMapSize(void) { return m_bufferMapSize_chunk; }

    // Chunk scheduling
    bool should_be_requested(void);
    void initializeSchedulingWindow(void);
    bool shouldStartChunkScheduling();
    void chunkScheduling(void);
    void randomChunkScheduling(void);

    void donetChunkScheduling(void);
    int selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier);

    bool shouldStartPlayer(void);
    void startPlayer(void);

private:
    // -- Timers
    cMessage *timer_getJoinTime;
    cMessage *timer_join;
    cMessage *timer_chunkScheduling;
    cMessage *timer_findMorePartner;
    cMessage *timer_startPlayer;

    bool    param_moduleDebug;
    double  param_chunkSchedulingInterval;
    double  param_interval_chunkScheduling;     // as a potential replacement for the above
    double  param_waitingTime_SchedulingStart;
    int     param_nNeighbor_SchedulingStart;
    double  param_interval_findMorePartner;
    double  param_interval_starPlayer;

    //    int param_numberOfPartner;

    // Pointers to "global" modules
    IChurnGenerator *m_churn;

    // -- Pointer to external modules
    Player *m_player;

    // State variables
    bool m_joined;
    bool m_scheduling_started;

    int m_schedulingWindowSize;     /* in [chunks] */

    // Variables to store history
    double m_firstJoinTime;

    // variables to keep up-to-date with
    SEQUENCE_NUMBER_T m_seqNum_schedWinStart;
    SEQUENCE_NUMBER_T m_seqNum_schedWinEnd;
    SEQUENCE_NUMBER_T m_seqNum_schedWinHead;

    // -- Partnership size
    int param_minNOP;
    int param_maxNOP;
    int param_offsetNOP;

    // -- Time stampt value objects
    // cTimestampedValue

    // -- For debugging purpose about the number of partners
    IPvXAddress m_monitoredAddress;
//    double m_arrivalTime;
//    double m_joinTime;
//    double m_video_startTime;
//    SEQUENCE_NUMBER_T m_head_videoStart;
//    SEQUENCE_NUMBER_T m_begin_videoStart;
//    int m_threshold_videoStart;


    // -- Signals
    simsignal_t sig_chunkRequestSeqNum;
    simsignal_t signal_nPartner;

    simsignal_t sig_partnerRequest;
};

#endif
