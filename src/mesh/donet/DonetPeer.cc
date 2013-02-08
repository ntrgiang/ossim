#include "InterfaceTableAccess.h"
#include "IPv4InterfaceData.h"

#include "DonetPeer.h"
#include "IChurnGenerator.h"
#include "DpControlInfo_m.h"
#include "MeshPeerStreamingPacket_m.h"

#include <algorithm>
#include <cmessage.h>

using namespace std;

Define_Module(DonetPeer);

DonetPeer::DonetPeer() {}
DonetPeer::~DonetPeer()
{
        cancelAndDeleteAllTimer();
}

void DonetPeer::initialize(int stage)
{
    if (stage == 0)
    {
        sig_chunkRequestSeqNum  = registerSignal("Signal_chunkRequest");
        //sig_partnerRequest      = registerSignal("Signal_PartnerRequest");
        sig_nJoin               = registerSignal("Signal_Join");

        flag_rangeUpdated = false;

        // -- Reset all range
        m_minStart = -1L;
        m_maxStart = -1L;
        m_minHead = -1L;
        m_maxHead = -1L;

//        // -- signals for ranges & current playback point
//        sig_minStart = registerSignal("Signal_MinStart");
//        sig_maxStart = registerSignal("Signal_MaxStart");
//        sig_minHead  = registerSignal("Signal_MinHead");
//        sig_maxHead  = registerSignal("Signal_MaxHead");
//        sig_currentPlaybackPoint = registerSignal("Signal_CurrentPlaybackPoint");

//        sig_localCI = registerSignal("Signal_LocalCI");

        return;
    }
    if (stage != 3)
        return;

    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    findNodeAddress();

    // -------------------------------------------------------------------------
    // --------------------------- State variables------------------------------
    // -------------------------------------------------------------------------
//    m_state_joined = false;
    m_state = MESH_JOIN_STATE_IDLE;
    m_scheduling_started = false;
    // -- Join status
//    m_joinState = MESH_STATE_JOIN_IDLE;

    // -------------------------------------------------------------------------
    // -------------------------------- Timers ---------------------------------
    // -------------------------------------------------------------------------
    // -- One-time timers
    timer_getJoinTime       = new cMessage("MESH_PEER_TIMER_GET_JOIN_TIME");
    timer_join              = new cMessage("MESH_PEER_TIMER_JOIN");

    // -- Repeated timers
    timer_chunkScheduling   = new cMessage("MESH_PEER_TIMER_CHUNK_SCHEDULING");
    timer_sendBufferMap     = new cMessage("MESH_PEER_TIMER_SEND_BUFFERMAP");
    timer_findMorePartner   = new cMessage("MESH_PEER_TIMER_FIND_MORE_PARTNER");
    timer_startPlayer       = new cMessage("MESH_PEER_TIMER_START_PLAYER");
    timer_sendReport        = new cMessage("MESH_PEER_TIMER_SEND_REPORT");

    //timer_timeout_waiting_accept = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
    timer_timeout_waiting_response  = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
//    timer_rejoin            = new cMessage("MESH_PEER_TIMER_REJOIN");
    timer_partnershipRefinement = new cMessage("MESH_PEER_TIMER_PARTNERSHIP_REFINEMENT");

    timer_partnerListCleanup = new cMessage("MESH_PEER_TIMER_PARTNERLIST_CLEANUP");


    // Parameters of the module itself
    // param_bufferMapInterval             = par("bufferMapInterval");
    //param_chunkSchedulingInterval       = par("chunkSchedulingInterval");
    //    param_interval_rejoin               = par("interval_rejoin");

    param_interval_chunkScheduling      = par("interval_chunkScheduling");
    param_interval_findMorePartner      = par("interval_findMorePartner");
    param_interval_timeout_joinReqAck   = par("interval_timeout_joinReqAck");
    param_interval_starPlayer           = par("interval_startPlayer");
    param_interval_partnershipRefinement = par("interval_partnershipRefinement");
    param_interval_partnerlistCleanup   = par("interval_partnerlistCleanup");

    param_nNeighbor_SchedulingStart     = par("nNeighbor_SchedulingStart");
    param_waitingTime_SchedulingStart   = par("waitingTime_SchedulingStart");

    param_requestFactor_moderate        = par("requestFactor_moderate");
    param_requestFactor_aggresive       = par("requestFactor_aggressive");
    param_maxNOP                        = par("maxNOP");
    param_offsetNOP                     = par("offsetNOP");
    param_threshold_scarity             = par("threshold_scarity");
    param_threshold_idleDuration_buffermap = par("threshold_idleDuration_buffermap");

    param_minNOP = param_maxNOP - param_offsetNOP;
    param_factor_requestList            = par("factor_requestList").doubleValue();

    m_downloadRate_chunk = (int)((param_downBw / (param_chunkSize * 8)) * param_interval_chunkScheduling);

    param_interval_waitingPartnershipResponse   = par("interval_waitingPartnershipResponse").doubleValue();

    scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

    // m_nChunk_perSchedulingInterval = param_interval_chunkScheduling * param_downBw / param_chunkSize / 8;

    m_nChunk_toRequest_perCycle = (int)(m_appSetting->getVideoStreamChunkRate() \
                    * param_interval_chunkScheduling);
                    //* param_baseValue_requestGreedyFactor);
//    m_bmPacket = generateBufferMapPacket();

    // -- Schedule for writing to log file
    //scheduleAt(getSimTimeLimit() - uniform(0.05, 0.95), timer_sendReport);

    // --------- Debug ------------
    m_joinTime = -1.0;
    // -- Init for activity logging:
//    string path = "..//results//";
//    string filename = path + getNodeAddress().str();
//    m_activityLog.open(filename.c_str(), fstream::out);

//    WATCH(filename.c_str());

    // -- State variable for join process
    m_nRequestSent = m_nRejectSent = 0;

    // -- For scheduling
    m_seqNum_schedWinHead = 0L;

    m_prevNChunkReceived = 0L;

    sig_newchunkForRequest    = registerSignal("Signal_nNewChunkForRequestPerCycle");
    sig_nChunkRequested       = registerSignal("Signal_nChunkRequested");

    // -- Unimportant signals:
    //signal_nPartner         = registerSignal("Signal_nPartner");

    sig_nPartner            = registerSignal("Signal_nPartner");
    sig_joinTime            = registerSignal("Signal_joinTime");
//    sig_playerStartTime     = registerSignal("Signal_playerStartTime");

       sig_pRequestSent        = registerSignal("Signal_pRequestSent");
       sig_pRejectRecv     = registerSignal("Signal_pRejectReceived");

    // Number of requests SENT & RECV
//    sig_pRequestSent = registerSignal("Signal_pRequestSent");
    sig_pRequestRecv = registerSignal("Signal_pRequestRecv");

    // Number of rejects SENT & RECV
//    sig_pRejectRecv = registerSignal("Signal_pRejectRecv");
//    sig_pRejectSent = registerSignal("Signal_pRejectSent");

//    sig_pRequestRecv_whileWaiting = registerSignal("Signal_pRequestRecv_whileWaiting");

//    sig_timeout = registerSignal("Signal_timeout");

    // -- signals for ranges & current playback point
    sig_minStart = registerSignal("Signal_MinStart");
    sig_maxStart = registerSignal("Signal_MaxStart");
    sig_minHead  = registerSignal("Signal_MinHead");
    sig_maxHead  = registerSignal("Signal_MaxHead");
    sig_currentPlaybackPoint = registerSignal("Signal_CurrentPlaybackPoint");
    sig_bufferStart = registerSignal("Signal_BufferStart");
    sig_bufferHead = registerSignal("Signal_BufferHead");

    sig_localCI = registerSignal("Signal_LocalCI");
    sig_myci = registerSignal("Signal_myCI");

    sig_inThroughput = registerSignal("Signal_InThroughput");

    sig_nBufferMapReceived = registerSignal("Signal_nBufferMapReceived");

    // -- Debugging variables
    m_arrivalTime = -1.0;
    m_joinTime = -1.0;
    m_video_startTime = -1.0;
    m_head_videoStart = -1L;
    m_begin_videoStart = -1L;
    m_threshold_videoStart = m_bufferMapSize_chunk/2;
    m_nChunkRequestReceived = 0L;
    m_nChunkSent = 0L;
    m_nBufferMapRecv = 0L;

    // -------------------------------------------------------------------------
    // ------------------- Initialize local variables --------------------------
    // -------------------------------------------------------------------------
    m_seqNum_schedWinStart = 0L;
    m_seqNum_schedWinEnd = 0L;
    m_seqNum_schedWinHead = 0L;

    // -------------------------------------------------------------------------
    // --------------------------- WATCH ---------------------------------------
    // -------------------------------------------------------------------------

    WATCH(m_localAddress);
    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_interval_bufferMap);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);
    WATCH(param_upBw);
    WATCH(param_downBw);
    WATCH(param_interval_chunkScheduling);
    WATCH(param_downBw);
    WATCH(param_minNOP);
    WATCH(param_maxNOP);
    WATCH(param_offsetNOP);
    WATCH(param_interval_partnerlistCleanup);
    WATCH(param_threshold_idleDuration_buffermap);

    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_BufferMapPacketSize_bit);
    WATCH(m_nChunk_toRequest_perCycle);
    WATCH(m_downloadRate_chunk);
    //WATCH(m_nChunk_perSchedulingInterval);

    WATCH(m_state);
    WATCH(m_appSetting);
    WATCH(m_apTable);
    WATCH(m_churn);
    WATCH(m_partnerList);
    WATCH(m_videoBuffer);
}

void DonetPeer::finish()
{
        cancelAndDeleteAllTimer();

        // -- Debug
    m_gstat->reportNumberOfPartner(m_partnerList->getSize());

    //reportStatus();
}

void DonetPeer::cancelAndDeleteAllTimer()
{
    if (timer_getJoinTime != NULL)
    {
       delete cancelEvent(timer_getJoinTime);
       timer_getJoinTime       = NULL;
    }

    if (timer_join != NULL)
    {
       delete cancelEvent(timer_join);
       timer_join              = NULL;
    }

    if (timer_sendBufferMap != NULL)
    {
       delete cancelEvent(timer_sendBufferMap);
       timer_sendBufferMap     = NULL;
    }

    if (timer_chunkScheduling != NULL)
    {
       delete cancelEvent(timer_chunkScheduling);
       timer_chunkScheduling   = NULL;
    }

    if (timer_findMorePartner != NULL)
    {
       delete cancelEvent(timer_findMorePartner);
       timer_findMorePartner   = NULL;
    }

    if (timer_startPlayer != NULL)
    {
       delete cancelEvent(timer_startPlayer);
       timer_startPlayer       = NULL;
    }

    if (timer_sendReport != NULL)
    {
       delete cancelEvent(timer_sendReport);
       timer_sendReport       = NULL;
    }

    if (timer_timeout_waiting_response != NULL)
    {
       delete cancelEvent(timer_timeout_waiting_response);
       timer_timeout_waiting_response       = NULL;
    }

    if (timer_partnershipRefinement != NULL)
    {
       delete cancelEvent(timer_partnershipRefinement);
       timer_partnershipRefinement = NULL;
    }

    if (timer_partnerListCleanup != NULL)
    {
       delete cancelEvent(timer_partnerListCleanup);
       timer_partnerListCleanup = NULL;
    }

}

void DonetPeer::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_sendBufferMap)
    {
        sendBufferMap();
        scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);
    }
    else if (msg == timer_chunkScheduling)
    {
        chunkScheduling();
        scheduleAt(simTime() + param_interval_chunkScheduling, timer_chunkScheduling);
    }
    else if (msg == timer_findMorePartner)
    {
        //findMorePartner();
        handleTimerFindMorePartner();
        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
    }
    else if (msg == timer_partnershipRefinement)
    {
       handleTimerPartnershipRefinement();
       scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);
    }
    else if (msg == timer_partnerListCleanup)
    {
       handleTimerPartnerlistCleanup();
       scheduleAt(simTime() + param_interval_partnerlistCleanup, timer_partnerListCleanup);
    }
    else if (msg == timer_timeout_waiting_response)
    {
        handleTimerTimeoutWaitingAccept();
    }
    else if (msg == timer_startPlayer)
    {
        startPlayer();

    }
    else if (msg == timer_getJoinTime)
    {
        m_arrivalTime = m_churn->getArrivalTime();
        EV << "Scheduled arrival time: " << simTime().dbl() + m_arrivalTime << endl;
        scheduleAt(simTime() + m_arrivalTime, timer_join);
    }
    else if (msg == timer_join)
    {
        //join();
        handleTimerJoin();

        // -- Schedule for a rejoin (if any)
        // scheduleAt(simTime() + param_interval_rejoin, timer_join);
    }
    else if (msg == timer_sendReport)
    {
       handleTimerReport();
    }
}

void DonetPeer::processPacket(cPacket *pkt)
{
    PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(pkt);
    EV << "Packet group: " << appMsg->getPacketGroup() << " --> for mesh module" << endl;
    if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
    {
        throw cException("Wrong packet type!");
    }

    MeshPeerStreamingPacket *meshMsg = check_and_cast<MeshPeerStreamingPacket *>(appMsg);
    switch (meshMsg->getPacketType())
    {
    case MESH_CHUNK_REQUEST:
    {
        processChunkRequest(pkt);
        break;
    }
    case MESH_BUFFER_MAP:
    {
        processPeerBufferMap(pkt);
        break;
    }
    case MESH_PARTNERSHIP_REQUEST:
    {
        processPartnershipRequest(pkt);
        break;
    }
    case MESH_PARTNERSHIP_ACCEPT:
    {
        processPartnershipAccept(pkt);
        break;
    }
    case MESH_PARTNERSHIP_REJECT:
    {
        processPartnershipReject(pkt);
        break;
    }
    default:
    {
        throw cException("Unrecognized packet types! %d", meshMsg->getPacketType());
        break;
    }
    } // switch

    delete pkt;
}

void DonetPeer::processPartnershipAccept(cPacket *pkt)
{
    EV << endl;
    EV << "---------- Processing a partnership ACCEPT packet -------------------" << endl;

    // -- Extract the address of the accepter
    PendingPartnershipRequest acceptor;
    getSender(pkt, acceptor.address, acceptor.port);
    // MeshPartnershipAcceptPacket *acceptPkt = dynamic_cast<MeshPartnershipAcceptPacket *>(pkt);
    MeshPartnershipAcceptPacket *acceptPkt = check_and_cast<MeshPartnershipAcceptPacket *>(pkt);
        acceptor.upBW = acceptPkt->getUpBw();

    EV << "Acceptor: " << endl
       << "-- Address:\t\t"         << acceptor.address
       << "-- Port:\t\t"            << acceptor.port << endl
       << "-- Upload bandwidth:\t"  << acceptor.upBW << endl;

//scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
       // -- Store the address of acceptor into its partner list
       if (acceptor.address != address_responseExpected)
       {
          EV << "IP address of acceptor: " <<  acceptor.address.str().c_str() << endl;
          EV << "IP address expected: " << address_responseExpected.str().c_str() << endl;
       }
//           throw cException("ACK from %s, expected IP: %s. Strange behavior!",
//           acceptor.address.str().c_str(),
//           address_responseExpected.str().c_str());
        //m_partnerList->addAddress(acceptor.address, acceptor.upBW);
        addPartner(acceptor.address, acceptor.upBW);

        EV << "First accept response from " << acceptor.address << endl;

        // -- Register itself to the APT
        m_apTable->addPeerAddress(getNodeAddress(), param_maxNOP);
        m_apTable->printActivePeerInfo(getNodeAddress());
        m_apTable->printActivePeerTable();

        m_firstJoinTime = simTime().dbl();

        // -- Cancel timer
//        if (timer_timeout_waiting_accept) cancelAndDelete(timer_timeout_waiting_accept);
//        timer_timeout_waiting_accept = NULL;
        cancelEvent(timer_timeout_waiting_response);

        double rand_value;
        // -- Start several timers
        // -- 1. Chunk Scheduling timers
        rand_value = uniform(0.0, 1.0);
        EV << "-- Chunk scheduling will be triggered after " << rand_value << " seconds" << endl;
        scheduleAt(simTime() + rand_value, timer_chunkScheduling);

        // -- 2. Send buffer map timers
        rand_value = uniform(0.0, 0.5);
        EV << "-- Sending Buffer Map will be triggered after " << rand_value << " seconds" << endl;
        scheduleAt(simTime() + rand_value, timer_sendBufferMap);

        // -- 3. Have more partner timers
        EV << "-- Finding more partners will be triggered after "
           << param_interval_findMorePartner << " seconds" << endl;
        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

        // -- 4. Start Player?
        EV << "-- Player will be tried to start after " << param_interval_starPlayer << " seconds" << endl;
        // -- Activate the Player
        m_player->activate();
        //scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);

        // -- 5. Partnership refinement
        scheduleAt(simTime() + param_interval_partnershipRefinement, timer_partnershipRefinement);

        // -- 6. Partnership cleanup
        scheduleAt(simTime() + param_interval_partnerlistCleanup, timer_partnerListCleanup);

        // -- Debug
        // m_gstat->reportMeshJoin();

        // -- Record join time (first accept response)
        m_joinTime = simTime().dbl();

        // Debuging with signals
        emit(sig_joinTime, simTime().dbl());

        EV << "State changes from IDLE_WAITING to ACTIVE" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Store the address of acceptor into its partner list
//        if (acceptor.address != address_responseExpected)
//        {
//           // Not the ACK from expected peer
//           // State should remain
//           break;
//        }
        //m_partnerList->addAddress(acceptor.address, acceptor.upBW);

        if (m_partnerList->getSize() >= param_maxNOP)
           break;

        EV << "An accept response from " << acceptor.address << endl;

        addPartner(acceptor.address, acceptor.upBW);

        m_apTable->incrementNPartner(getNodeAddress());

        // -- Cancel timer
//        if (timer_timeout_waiting_accept) cancelAndDelete(timer_timeout_waiting_accept);
//        timer_timeout_waiting_accept = NULL;
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from MESH_JOIN_STATE_ACTIVE_WAITING to MESH_JOIN_STATE_ACTIVE_WAITING" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        EV << "ACCEPT message arrived too late, will be ignored!" << endl;

        // -- State remains
//        m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        EV << "ACCEPT message arrived too late, will be ignored!" << endl;

        // -- State remains
//        m_state = MESH_JOIN_STATE_IDLE;
        break;
    }
    default:
    {
        throw cException("Uncovered state, check assignment of state variable!");
        break;
    }
    } // switch()

}

void DonetPeer::addPartner(IPvXAddress remote, double upbw)
{
//   DonetBase::addPartner(remote, upbw);
   int nChunk = (int)(param_interval_chunkScheduling*upbw/param_chunkSize/8); // per scheduling cycle
   m_partnerList->addAddress(remote, upbw, nChunk);
}


//void DonetPeer::processRejectResponse(cPacket *pkt)
//{
//    EV << endl;
//    EV << "---------- Process partnership reject -------------------------------" << endl;

//    // -- Extract the address of the accepter
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress rejectorAddress = controlInfo->getSrcAddr();
//    EV << "Rejector's address: " << rejectorAddress << endl;

////    m_activityLog << "--- processRejectResponse --- " << endl;
////    m_activityLog << "\t at " << simTime().dbl() << " Got a REJECT from: " << rejectorAddress << endl;

//    // TODO-Giang: has to verify this code, the logic seems not very clear
//    ++m_nRejectSent;
//    EV << "-- Number of rejected requests so far: " << m_nRejectSent << endl;

//    if (m_nRejectSent == m_nRequestSent)
//    {
////        m_activityLog << "\tAll requests were rejected --> join() again!" << endl;

//        EV << "m_nRejectSent == m_nRequestSent --> rejoin()!" << endl;

//        // Reset state-variables
//        m_nRequestSent = -1;
//        m_nRejectSent = 0;
//        join();
//    }

//    emit (sig_pRejectReceived, m_nRejectSent);

//}


void DonetPeer::processPartnershipReject(cPacket *pkt)
{
    EV << endl;
    EV << "-------- Process partnership Reject ---------------------------------" << endl;

    emit(sig_pRejectRecv, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        EV << "Should find another partner NOW ..." << endl;
        // -- Cancel timer
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from IDLE_WAITING to IDLE" << endl;
        m_state = MESH_JOIN_STATE_IDLE;

        scheduleAt(simTime() + uniform(0, 2.0), timer_join);
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        EV << "Try to contact another peer, later on ..." << endl;
        // -- Cancel timer
        cancelEvent(timer_timeout_waiting_response);

        EV << "State changes from MESH_JOIN_STATE_ACTIVE_WAITING to MESH_JOIN_STATE_ACTIVE" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        EV << "REJECT message arrived too late, but no problem because of timeout, will be ignored!" << endl;

        // -- State remains
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        EV << "REJECT message arrived too late, but no problem because of timeout, will be ignored!" << endl;

        // -- State remains
        break;
    }
    default:
    {
        throw cException("Uncovered state, check assignment of state variable!");
        break;
    }
    } // switch()


    // -- Extract the address of the accepter
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress rejectorAddress = controlInfo->getSrcAddr();
//    EV << "Rejector's address: " << rejectorAddress << endl;

    //emit (sig_pRejectReceived, m_nRejectSent);
}


//void DonetPeer::join()
//{
//    EV << endl;
//    EV << "---------- Join -----------------------------------------------------" << endl;

//    if (m_state != MESH_JOIN_STATE_IDLE)
//    {
//        throw cException("Not in expected state (IDLE), something wrong!");
//    }

//    // TODO: This code is very unlikely, should be removed later on
//    // -- Only send a partnership request if not enough partners
//    if (m_partnerList->getSize() >= param_minNOP)
//    {
//        EV << "Has enough partners, should not request more" << endl;
//        return;
//    }

//    int init_number = min(param_maxNOP/2 + 1, param_minNOP);
//    m_list_randPeer = m_apTable->getNPeer(init_number);
//    if (m_list_randPeer.size() == 0)
//    {
//        throw cException("Empty list of random peers from APT");
//    }
//    else
//    {
//        EV << "Got a list of " << m_list_randPeer.size() << " peers. Stored!" << endl;
//    }

//    IPvXAddress aRandPeer = m_list_randPeer[0];

//    // -- Prepare the JOIN_REQUEST packet and then send it to the remote peer
//    MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
//        reqPkt->setUpBw(param_upBw);
//        reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());
//    sendToDispatcher(reqPkt, m_localPort, aRandPeer, m_destPort);

//    // -- Remove the peer which was requested
//    m_list_randPeer.erase(m_list_randPeer.begin());
//    EV << "Number of elements after deleting the first one: " << m_list_randPeer.size() << endl;

//    // Debuging with signals
//    // emit(sig_pRequestSent, s);

//    // -- Issue a timer for timeout to receive ACK for those requests
//    cMessage *timer_timeout_joinReqAccept = new cMessage("MESH_TIMER_TIMEOUT_JOIN_REQ_ACK");
//    scheduleAt(simTime() + param_interval_timeout_joinReqAck, timer_timeout_joinReqAccept);

//    // -- Update the joinState
//    m_state = MESH_JOIN_STATE_IDLE_WAITING;

//    // m_joinState = MESH_STATE_JOIN_WAITING_ACCEPT;

////    m_nRequestSent = listRandPeer.size();

////    int s = listRandPeer.size();

////    for (std::vector<IPvXAddress>::iterator iter = m_list_randPeer.begin(); iter != m_list_randPeer.end(); ++iter)
////    {
////        //IPvXAddress addressRandPeer = *iter;
////        EV << "-- Peer to be requested: " << *iter << endl;

////        MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
////            reqPkt->setUpBw(param_upBw);
////            reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

////        sendToDispatcher(reqPkt, m_localPort, *iter, m_destPort);

////        // Debuging with signals
////        emit(sig_pRequestSent, s);

////        // -- Store the addresses of the requested peers to the list for later query
////        m_list_joinRequestedNode.push_back(*iter);
////    }

//}

/**
 * Each time triggered, get a random peer from tracker
 * - check whether the peer is its partner
 * - if NOT --> send request
 * - in both case, sleep another T before triggered again
 */
//void DonetPeer::findMorePartner()
//{
//    if (m_partnerList->getSize() >= param_minNOP)
//    {
////        emit(signal_nPartner, -1);
////
////        m_activityLog << "--- Find more partner --- " << endl;
////        m_activityLog << "\tBut has enough (mininum number of) partners!" << endl;
//        EV << "Minimum number of partners has been reach --> stop finding more partner for the moment." << endl;
//        return;
//    }

//    IPvXAddress addressRandPeer;
//    do
//    {
//         addressRandPeer = m_apTable->getARandPeer();
//    } while (addressRandPeer == getNodeAddress());


//    if (m_partnerList->have(addressRandPeer))
//    {
////        emit(signal_nPartner, -2);

////        m_activityLog << "--- Find more partner --- " << endl;
////        m_activityLog << "\tBut already had partner!" << endl;
//        EV << "The peer has already been node's partner. No more request will be sent out." << endl;
//        return;
//    }
////    m_activityLog << "--- Find more partner --- " << endl;
////    m_activityLog << "\tPartner to contat: " << addressRandPeer << endl;

////    emit(signal_nPartner, m_partnerList->size());

//    EV << "Peer " << addressRandPeer << " will be requested!" << endl;

//    MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
//        reqPkt->setUpBw(param_upBw);
//        reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

//    sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);

//    emit(sig_pRequestSent, 1);
//}

/**
 * @brief DonetPeer::findPartner
 * @return
 * false if no partner had been contacted
 * true if a partner had been contacted
 */
bool DonetPeer::findPartner()
{
   Enter_Method("findPartner()");

   // Init
   address_responseExpected = IPvXAddress("0.0.0.0");

    if (m_partnerList->getSize() >= param_minNOP)
    {
        EV << "Minimum number of partners has been reach --> stop finding more partner for the moment." << endl;
        return false;
    }

    bool ret = sendPartnershipRequest();
    return ret;

//    return true;
}

bool DonetPeer::sendPartnershipRequest(void)
{
    IPvXAddress addressRandPeer;
    int count = 0;
    do
    {
         count++;
//        addressRandPeer = m_apTable->getARandPeer();
         addressRandPeer = m_apTable->getARandPeer(getNodeAddress());
        if (count > 10)
           return false;
    }
    while (addressRandPeer == getNodeAddress());

    if (m_partnerList->have(addressRandPeer))
    {
        EV << "The peer has already been node's partner. No more request will be sent out." << endl;
        return false;
    }

    EV << "Peer " << addressRandPeer << " will be requested!" << endl;

    MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
        reqPkt->setUpBw(param_upBw);
        reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

    sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);
    address_responseExpected = addressRandPeer;

    emit(sig_pRequestSent, 1);
    return true;
}

void DonetPeer::handleTimerJoin(void)
{
   Enter_Method("handleTimerJoin()");

    EV << endl;
    EV << "----------------------- Handle timer JOIN ---------------------------" << endl;

    m_gstat->reportNumberOfJoin(1);
    emit(sig_nJoin, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE:
    {
        if (findPartner() == true)
        {
            // -- State changes to waiting mode, since a request has been sent
            scheduleAt(simTime() + param_interval_waitingPartnershipResponse, timer_timeout_waiting_response);

            EV << "State changes to waiting mode" << endl;
            m_state = MESH_JOIN_STATE_IDLE_WAITING;
        }
        else
        {
           // State remains ACTIVE
        }

        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        throw cException("Join action cannot be done in JOINED_WAITING state");
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        //EV << "Join action cannot be done in JOINED state" << endl;
        throw cException("Join action cannot be done in JOINED state");
        break;
    }
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        //EV << "Join action cannot be done in IDLE_WAITING state" << endl;
        throw cException("Join action cannot be done in JOINED state");
        break;
    }
    default:
    {
        throw cException("Not in an expected state");
        break;
    }
    } // switch()
}


void DonetPeer::handleTimerFindMorePartner(void)
{
    switch(m_state)
    {
    case MESH_JOIN_STATE_ACTIVE:
    {
        bool ret = findPartner();

        if (ret == true)
        {
           // -- State changes to waiting mode, since a request has been sent
           m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        }
        else
        {
           // State remains ACTIVE
        }

        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Do NOTHING, here!

        EV << "State remains as MESH_JOIN_STATE_ACTIVE_WAITING" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        //return;
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        EV << "Cannot find new partner when state is IDLE" << endl;
        break;
    }
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        EV << "Cannot start finding new partner when state is IDLE_WAITING" << endl;
        break;
    }
    default:
    {
        throw cException("Not in an expected state");
        break;
    }
    } // switch()

    //scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
}

void DonetPeer::handleTimerPartnershipRefinement()
{
//   bool ret = sendPartnershipRequest();

//   if (ret == false)
//   {
//      flag_partnershipRefinement = false;
//   }

//   flag_partnershipRefinement = true;
}

void DonetPeer::handleTimerPartnerlistCleanup()
{
// obsolete
//   std::map<IPvXAddress, NeighborInfo*>::iterator iter;
//   for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
//   {
//      //NeighborInfo *nbr_info = iter->second
//      double timeStamp = iter->second->getLastRecvBmTime();
//      if (simTime().dbl() - timeStamp > param_threshold_idleDuration_buffermap)
//      {
//         delete iter->second;
//         m_partnerList->m_map.erase(iter);
//      }
//   }

   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
   {
      //NeighborInfo *nbr_info = iter->second
      double timeStamp = iter->second.getLastRecvBmTime();
      if (simTime().dbl() - timeStamp > param_threshold_idleDuration_buffermap)
      {
         //delete iter->second;
         m_partnerList->m_map.erase(iter);
         break; // delete only one address at a time
      }
   }

}

void DonetPeer::updateDataExchangeRecord(void)
{
   RecordCountChunk record;
   //std::map<IPvXAddress, NeighborInfo *>::iterator iter;
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   for(iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
   {
//      IPvXAddress addr = iter->first;
//      long int currentCountReceived, currentCountSent;
//      m_forwarder->getRecordChunk(addr, currentCountReceived, currentCountSent);

//      // Update the received chunk count
//      long int prevCount = iter->second->getCountChunkReceived();
//      iter->second->setCountChunkReceived(currentCountReceived-prevCount);

//      // Update the sent chunk count
//      prevCount = iter->second->getCountChunkSent();
//      iter->second->setCountChunkSent(currentCountSent-prevCount);

      IPvXAddress addr = iter->first;
      m_forwarder->getRecordChunk(addr, record);
      double interval = simTime().dbl() - record.m_oriTime;

      if (interval > 0)
      {
         iter->second.setAverageChunkReceived(record.m_chunkReceived / interval);
         iter->second.setAverageChunkSent(record.m_chunkSent / interval);
      }
      else
      {
         throw cException("Interval has to be positive");
      }
   }
}

//void DonetPeer::initializeSchedulingWindow()
//{
//   // Browse through all partners and find an optimal scheduling window
//   SEQUENCE_NUMBER_T min_head, max_end;
//   std::map<IPvXAddress, NeighborInfo*>::iterator iter = m_partnerList->m_map.begin();
//   min_head = iter->second->getSeqNumRecvBmHead();
//   max_end  = iter->second->getSeqNumRecvBmEnd();

//   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
//   {
//      SEQUENCE_NUMBER_T temp = iter->second->getSeqNumRecvBmHead();
//      min_head=(min_head > temp)?temp:min_head;

//      temp = iter->second->getSeqNumRecvBmEnd();
//      max_end=(max_end < temp)?temp:max_end;
//   }

//   m_sched_window.end = (max_end + min_head) / 2;
//   m_sched_window.head = m_sched_window.end + m_bufferMapSize_chunk - 1;
//}

int DonetPeer::initializeSchedulingWindow()
{
   // Browse through all partners and find an optimal scheduling window
   SEQUENCE_NUMBER_T min_head = 0L, max_start = 0L;

// obsolete
//   std::map<IPvXAddress, NeighborInfo*>::iterator iter = m_partnerList->m_map.begin();
//   min_head = iter->second->getSeqNumRecvBmHead();
//   max_start  = iter->second->getSeqNumRecvBmStart();

//   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
//   {
//      SEQUENCE_NUMBER_T temp = 0L;
//      temp = iter->second->getSeqNumRecvBmHead();
//      if (min_head != 0L && temp != 0L)
//         min_head=(min_head > temp) ? temp : min_head;
//      else
//         min_head = std::max(temp, min_head);

//      temp = iter->second->getSeqNumRecvBmStart();
//      max_start=(max_start < temp)?temp:max_start;
//   }

   std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
   min_head = iter->second.getSeqNumRecvBmHead();
   max_start  = iter->second.getSeqNumRecvBmStart();

   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
   {
      SEQUENCE_NUMBER_T temp = 0L;
      temp = iter->second.getSeqNumRecvBmHead();
      if (min_head != 0L && temp != 0L)
         min_head=(min_head > temp) ? temp : min_head;
      else
         min_head = std::max(temp, min_head);

      temp = iter->second.getSeqNumRecvBmStart();
      max_start=(max_start < temp)?temp:max_start;
   }


   EV << "min_head = " << min_head << " -- max_start = " << max_start << endl;

   //if (min_head > 0L && max_start != 0L)
   if (min_head > 0L)
   {
      m_sched_window.start = (max_start + min_head) / 2;
      m_sched_window.end   = m_sched_window.end + m_bufferMapSize_chunk - 1;
      EV << "Scheduling window [start, end] = " << m_sched_window.start << " - " << m_sched_window.end << endl;

      m_videoBuffer->initializeRangeVideoBuffer(m_sched_window.start);

      return INIT_SCHED_WIN_GOOD;
   }

   return INIT_SCHED_WIN_BAD;
}

bool DonetPeer::shouldStartChunkScheduling(void)
{
    //if (simTime().dbl() - m_firstJoinTime >= param_waitingTime_SchedulingStart
    double elapsed_time_since_joining = simTime().dbl() - m_joinTime;
    int current_number_of_partner = m_partnerList->getSize();
    EV  << "Elapsed time since joining: " << elapsed_time_since_joining << " -- " \
        << "Current number of partners: " << current_number_of_partner << endl;

    if (elapsed_time_since_joining >= param_waitingTime_SchedulingStart
            || current_number_of_partner >= param_nNeighbor_SchedulingStart)
    {
        return true;
    }

    return false;
}

/*
bool DonetPeer::should_be_requested(void)
{
    EV << "Number of chunks requested so far per cycle: " \
            << m_nChunkRequested_perSchedulingInterval << endl;
    if (m_nChunkRequested_perSchedulingInterval >= numberOfChunkToRequestPerCycle())
        return false;

    return true;
}
*/

bool DonetPeer::should_be_requested(SEQUENCE_NUMBER_T seq_num)
{
    // -- Check if the chunk has been recently requested
//    if (recentlyRequestedChunk(seq_num) == true)
//    {
//        return false;
//    }

    SEQUENCE_NUMBER_T current_playbackPoint = m_player->getCurrentPlaybackPoint();
    if (seq_num < current_playbackPoint)
    {
        EV << "-- Chunk " << seq_num << " is behind play-back point " << current_playbackPoint << endl;
        return false;
    }

    if (m_nChunkRequested_perSchedulingInterval > m_downloadRate_chunk)
    {
       return false;
    }

    return true;
}

/*
bool DonetPeer::should_be_requested(SEQUENCE_NUMBER_T seq_num)
{
    EV << endl;
    EV << "%%%" << endl;
    EV << "Number of chunks requested in this cycle: " \
       << m_nChunkRequested_perSchedulingInterval << endl;

    // -- Check if already requested too many chunks
//    if (m_nChunkRequested_perSchedulingInterval >= numberOfChunkToRequestPerCycle())
//        return false;

    int nChunkPerCycle = numberOfChunkToRequestPerCycle();
    EV << "numberOfChunkToRequestPerCycle(): " << nChunkPerCycle << endl;

    if (numberOfChunkToRequestPerCycle() > 0)
    {
        // -- Case of limited requests
        EV << "-- Limited number of chunks to request" << endl;
        if (m_nChunkRequested_perSchedulingInterval >= nChunkPerCycle)
        {
            EV << "-- Limit was exceeded" << endl;
            return false;
        }
    }
    else
    {
        EV << "-- Unlimited number of chunks to request" << endl;
    }

    // -- Check if the chunk has been recently requested
    if (recentlyRequestedChunk(seq_num) == true)
    {
        return false;
    }

    SEQUENCE_NUMBER_T current_playbackPoint = m_player->getCurrentPlaybackPoint();
    if (seq_num < current_playbackPoint)
    {
        EV << "-- Chunk " << seq_num << " is behind play-back point " << current_playbackPoint << endl;
        return false;
    }

    return true;
}
*/

/*
 * Depending on _strategy_ which is applied, the schedulingWindows will be initialized accordingly
 */
//void DonetPeer::initializeSchedulingWindow(void)
//{
//    // Relative lag
//
//    // Absolute lag
//}

/*
 * This function is a _wrapper_ for the specific chunk scheduling algorithm
 */
void DonetPeer::chunkScheduling()
{
    if (m_partnerList->getSize() <= 0)
    {
        EV << "No partner just yet, exiting from chunk scheduling" << endl;
        return;
    }

//    if (m_seqNum_schedWinHead <= 0)
//    {
//        EV << "Scheduling Window is empty, exiting from chunk scheduling" << endl;
//        return;
//    }

    if (m_scheduling_started == false)
    {
       int ret = initializeSchedulingWindow();
       if (ret == INIT_SCHED_WIN_BAD)
       {
          EV << "Failed to initialize the scheduling window" << endl;
          return;
       }
       m_scheduling_started = true;
    }


    // -- Update the range variables
    updateRange();

    // -------------------------------------------------------------------------
    // --- Debugging
    // -------------------------------------------------------------------------
    emit(sig_nPartner, m_partnerList->getSize());
    reportLocalStatistic();

    long int temp = m_videoBuffer->getNChunkReceived();
    emit(sig_inThroughput, (temp - m_prevNChunkReceived)/param_interval_chunkScheduling );
    m_prevNChunkReceived = temp;

    emit(sig_nBufferMapReceived, double(m_nBufferMapRecv)/(simTime().dbl() - m_firstJoinTime));

    updateDataExchangeRecord();

    m_videoBuffer->printStatus();

    randomChunkScheduling();
//        donetChunkScheduling();
}

void DonetPeer::reportLocalStatistic(void)
{
    // -- Report ranges
    emit(sig_minStart, m_minStart);
    emit(sig_maxStart, m_maxStart);
    emit(sig_minHead, m_minHead);
    emit(sig_maxHead, m_maxHead);
    emit(sig_currentPlaybackPoint, m_player->getCurrentPlaybackPoint());
    emit(sig_bufferStart, m_videoBuffer->getBufferStartSeqNum());
    emit(sig_bufferHead, m_videoBuffer->getHeadReceivedSeqNum());


    // -- Statistics from Player
    long int nHit = m_player->getCountChunkHit();
    long int nMiss = m_player->getCountChunkMiss();

    if ((nHit + nMiss) == 0)
    {
       emit (sig_localCI, 0);
    }
    else
    {
       emit(sig_localCI, (long double)nHit / (nHit + nMiss));
//       emit(sig_localCI, (long double)nHit);
    }
}

void DonetPeer::updateRange(void)
{
   // Browse through all partners and find an optimal scheduling window
   //std::map<IPvXAddress, NeighborInfo*>::iterator iter = m_partnerList->m_map.begin();
   std::map<IPvXAddress, NeighborInfo>::iterator iter = m_partnerList->m_map.begin();
   m_minStart = iter->second.getSeqNumRecvBmStart();
   m_maxStart = iter->second.getSeqNumRecvBmStart();

   m_minHead = iter->second.getSeqNumRecvBmHead();
   m_maxHead = iter->second.getSeqNumRecvBmHead();

   for (++iter; iter != m_partnerList->m_map.end(); ++iter)
   {
      SEQUENCE_NUMBER_T temp = iter->second.getSeqNumRecvBmHead();
      m_minHead = (m_minHead > temp) ? temp : m_minHead;
      m_maxHead = (m_maxHead < temp) ? temp : m_maxHead;

      temp = iter->second.getSeqNumRecvBmStart();
      m_minStart = (m_minStart > temp) ? temp : m_minStart;
      m_maxStart = (m_maxStart < temp) ? temp : m_maxStart;
   }
}

/**
 * Used for Donet chunk scheduling
 */
int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier)
{

    int ret = 0;
    // -- Get pointer to the respective NeighborInfo

    // obsolete
    // NeighborInfo *info_candidate1 = m_partnerList->getNeighborInfo(candidate1);
    // NeighborInfo *info_candidate2 = m_partnerList->getNeighborInfo(candidate2);

//    if (info_candidate1->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate1->getUpBw())
//    {
//        if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
//        {
//            if (info_candidate1->getUpBw() > info_candidate2->getUpBw())
//            {
//                supplier = candidate1;
//            }
//            else
//            {
//                supplier = candidate2;
//            }
//        }
//        else
//        {
//            supplier = candidate1;
//        }
//    }
//    else
//    {
//        if (info_candidate2->getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2->getUpBw())
//        {
//            supplier = candidate2;
//        }
//        else
//        {
//            ret = -1;
//            // -- Just a /Padding code/
//            supplier = candidate1;
//        }
//    }

    NeighborInfo info_candidate1 = m_partnerList->getNeighborInfo(candidate1);
    NeighborInfo info_candidate2 = m_partnerList->getNeighborInfo(candidate2);

    if (info_candidate1.getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate1.getUpBw())
    {
        if (info_candidate2.getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2.getUpBw())
        {
            if (info_candidate1.getUpBw() > info_candidate2.getUpBw())
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
        if (info_candidate2.getChunkAvailTime(seq_num) > (param_chunkSize*8)/info_candidate2.getUpBw())
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

void DonetPeer::getAppSetting()
{
    EV << "DonetBase::getAppSetting" << endl;

    // -- Calling the function of the base
    DonetBase::getAppSetting();

    // -- Especifically for the DonetPeer
    // !!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!
    m_schedulingWindowSize = m_bufferMapSize_chunk;

}

void DonetPeer::bindToGlobalModule(void)
{
    DonetBase::bindToGlobalModule();

    // -- Churn
    cModule *temp = simulation.getModuleByPath("churnModerator");
    m_churn = check_and_cast<IChurnGenerator *>(temp);
    //if (m_churn == NULL) throw cException("m_churn == NULL is invalid");
    EV << "Binding to churnModerator is completed successfully" << endl;
}

void DonetPeer::bindToMeshModule(void)
{
    DonetBase::bindToMeshModule();

    // -- Player
    cModule *temp = getParentModule()->getModuleByRelativePath("player");
    //m_player = check_and_cast<Player *>(temp);
    m_player = check_and_cast<PlayerBase *>(temp);
    EV << "Binding to churnModerator is completed successfully" << endl;

}

void DonetPeer::processPeerBufferMap(cPacket *pkt)
{
    EV << endl;
    EV << "---------- Process received buffer map ------------------------------" << endl;

    /*
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(appMsg);
    int countOne = 0;
    for (unsigned int i = 0; i < bmPkt->getBufferMapArraySize(); i++)
    {
        countOne += bmPkt->getBufferMap(i)==true?1:0;
    }
    r_countBM.record(countOne);
    */

    IPvXAddress senderAddress = getSender(pkt);
    // obsolete
    //NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);
//    EV << "-- Received a buffer map from " << senderAddress << endl;
    // Get the record respective to the address of the partner
//    if (nbr_info == NULL)
//    {
//        EV << "-- Buffer Map is received from a non-partner peer!" << endl;
//        return;
//    }

    if (m_partnerList->hasAddress(senderAddress) == false)
    {
       EV << "-- Buffer Map is received from a non-partner peer!" << endl;
       return;
    }
    NeighborInfo nbr_info = m_partnerList->getNeighborInfo(senderAddress);

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

    EV << "-- Buffer Map information: " << endl;
    EV << "  -- Start:\t"   << bmPkt->getBmStartSeqNum()    << endl;
    EV << "  -- End:\t"     << bmPkt->getBmEndSeqNum()      << endl;
    EV << "  -- Head:\t"    << bmPkt->getHeadSeqNum()       << endl;

    // -- Copy the BufferMap content to the current record
    nbr_info.copyFrom(bmPkt);

    // -- Update the timestamp of the received BufferMap
    nbr_info.setLastRecvBmTime(simTime().dbl());

    // -- Update the range of the scheduling window
    m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;
    EV << "-- Head for the scheduling window: " << m_seqNum_schedWinHead << endl;

    // -- Debug
    ++m_nBufferMapRecv;
    nbr_info.printRecvBm();
    // m_partnerList->printRecvBm(senderAddress);

}

void DonetPeer::startPlayer(void)
{
    if (shouldStartPlayer())
    {
        EV << "Player should start now. " << endl;
        m_player->startPlayer();

        emit(sig_playerStartTime, simTime().dbl());
    }
    else
    {
        EV << "Player should not start at the moment." << endl;
        scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);
    }

}

bool DonetPeer::shouldStartPlayer(void)
{
    EV << "---------- Check whether should start the Player --------------------" << endl;
    EV << "-- Threshold: " << m_bufferMapSize_chunk / 2 << " ";
    if (m_videoBuffer->getNumberOfChunkFill() >= m_bufferMapSize_chunk / 2)
    {
        EV << "-- Enough chunks --> The Player should start now!" << endl;

        m_video_startTime = simTime().dbl();
        m_head_videoStart = m_videoBuffer->getHeadReceivedSeqNum();
        m_begin_videoStart = m_videoBuffer->getBufferStartSeqNum();
        return true;
    }

    EV << "-- Not enough chunks --> wait a bit more!" << endl;
    return false;
}

/*
int DonetPeer::numberOfChunkToRequestPerCycle(void)
{
    int ret = (int)(m_nChunk_toRequest_perCycle * param_baseValue_requestGreedyFactor);
    EV << "Number of chunk to request per scheduling cycle: " << ret << endl;

    return ret;
}
*/

int DonetPeer::numberOfChunkToRequestPerCycle(void)
{
    int ret = 0;
    if (m_player->playerStarted() == false)
    {
        // -- Chunks will be requested orderly to avoid congestion at requested peer
        // -- Trade-off is longer startup delays

        ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_moderate);
    }
    else if (inScarityState() == true)
    {
        // -- In "sensitive" state, since the play might soon be stalled because of no chunks in the buffer

        ret = (int)(m_nChunk_toRequest_perCycle * param_requestFactor_aggresive);
    }
    else
    {
        // -- Request chunks "freely", since there would be not so many chunks available anyway

        ret = -1; // Unlimited
    }

    //EV << "Number of chunk to request per scheduling cycle: " << ret << endl;

    return ret;
}

/*
double DonetPeer::currentRequestGreedyFactor(void)
{
    return param_baseValue_requestGreedyFactor;
}
*/

bool DonetPeer::recentlyRequestedChunk(SEQUENCE_NUMBER_T seq_num)
{
    EV << endl;
    EV << "%%%" << endl;
    vector<SEQUENCE_NUMBER_T>::iterator it;
    it = find (m_list_requestedChunk.begin(), m_list_requestedChunk.end(), seq_num);

    // -- Debug
    printListOfRequestedChunk();

    if (it == m_list_requestedChunk.end())
    {
        EV << "Chunk " << seq_num << " has NOT been requested recently" << endl;
        return false;
    }
    else
    {
        EV << "Chunk " << seq_num << " has been requested recently" << endl;
    }

    return true;
}

void DonetPeer::refreshListRequestedChunk(void)
{
    int expectedSize = (int)(param_factor_requestList * m_nChunk_toRequest_perCycle);
    int nRedundantElement = m_list_requestedChunk.size() - expectedSize;
    if (nRedundantElement > 0)
    {
        EV << "-- " << nRedundantElement << " element should be deleted" << endl;
        m_list_requestedChunk.erase(m_list_requestedChunk.begin(), m_list_requestedChunk.begin()+nRedundantElement);
    }
}

bool DonetPeer::inScarityState(void)
{
    int offset = m_videoBuffer->getHeadReceivedSeqNum() - m_player->getCurrentPlaybackPoint();

    if (offset < (int)(m_videoStreamChunkRate * param_threshold_scarity))
        return true;

    return false;
}

void DonetPeer::printListOfRequestedChunk(void)
{
    if (ev.isGUI() == false)
        return;

    EV << "Recently requested chunks: " << endl;
    int count = 1;
    vector<SEQUENCE_NUMBER_T>::iterator iter;
    for (iter = m_list_requestedChunk.begin(); iter != m_list_requestedChunk.end(); ++iter)
    {
        EV << *iter << " ";
        if (count % 10 == 0) EV << "\t";
        if (count % 30 == 0) EV << endl;
        count++;
    }
    EV << endl;
}

//void DonetPeer::processTimeoutJoinRequestAccept(cMessage *msg)
//{
//    Enter_Method("processTimeoutJoinRequestAccept");
//    EV << "Time is over to wait for all Accept packets" << endl;

//    switch(m_state)
//    {
//    case MESH_JOIN_STATE_IDLE_WAITING:
//    {
//        // -- Clear list of requested nodes
//        m_list_joinRequestedNode.clear();

//        // -- Cancel event, delete timer message
//        cancelAndDelete(msg); msg = NULL;

//        // -- Process list of requesting nodes
//        break;
//    }
//    case MESH_STATE_JOIN_ALL_ACCEPT_RECEIVED:
//    {
//        // -- Double check the list of requested node (it should be empty now)
//        if (m_list_joinRequestedNode.size() != 0)
//            throw cException("Mismatch between state and list_joinRequestedNode");

//        // -- Cancel event, delete timer message
//        cancelAndDelete(msg); msg = NULL;
//        break;
//    }
////    case MESH_STATE_JOIN_IDLE:
////    case MESH_STATE_JOIN_WAITING_ACCEPT_ACK:
//    default:
//    {
//        throw cException("At a wrong state");
//    }
//    }
//}


void DonetPeer::handleTimerTimeoutWaitingAccept()
{
    EV << endl;
    EV << "------------------------- Handle timeout_waitingAccept --------------" << endl;

    emit(sig_timeout, 1);

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        // -- Update the state
        EV << "State changes from MESH_JOIN_STATE_IDLE_WAITING to MESH_JOIN_STATE_IDLE" << endl;
        m_state = MESH_JOIN_STATE_IDLE;

        // -- Schedule for new JOIN
        // this event, in turn, triggers findPartner()
        scheduleAt(simTime() + uniform(0.0, 2.0), timer_join);
        //scheduleAt(simTime(), timer_join);
        break;
    }
    case MESH_JOIN_STATE_ACTIVE_WAITING:
    {
        // -- Process pending requests
        // There should not be any pending request at this time
//        vector<PendingPartnershipRequest>::iterator iter = m_list_partnershipRequestingNode.begin();
//        for (; iter != m_list_partnershipRequestingNode.end(); ++iter)
//        {
//            considerAcceptPartner(*iter);
//        }
//        m_list_partnershipRequestingNode.clear();

        EV << "State changes to MESH_JOIN_STATE_ACTIVEG" << endl;
        m_state = MESH_JOIN_STATE_ACTIVE;
        break;
    }
    case MESH_JOIN_STATE_ACTIVE:
    {
        //EV << "Timeout happens when the peer is in JOINED state" << endl;
        throw cException("Timeout happens when the peer is in JOINED state");
        break;
    }
    case MESH_JOIN_STATE_IDLE:
    {
        //EV << "Timeout happens when the peer is in IDLE state" << endl;
        throw cException("Timeout happens when the peer is in IDLE state");
        break;
    }
    default:
    {
        throw cException("Uncovered state");
        break;
    }
    } // switch()
}
