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
        sig_partnerRequest      = registerSignal("Signal_PartnerRequest");
        sig_nJoin               = registerSignal("Signal_Join");

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

    //timer_timeout_waiting_accept = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
    timer_timeout_waiting_response  = new cMessage("MESH_PEER_TIMER_WAITING_ACCEPT");
//    timer_rejoin            = new cMessage("MESH_PEER_TIMER_REJOIN");

    // Parameters of the module itself
    // param_bufferMapInterval             = par("bufferMapInterval");
    //param_chunkSchedulingInterval       = par("chunkSchedulingInterval");
    param_interval_chunkScheduling      = par("interval_chunkScheduling");
    param_interval_findMorePartner      = par("interval_findMorePartner");
//    param_interval_rejoin               = par("interval_rejoin");
    param_interval_timeout_joinReqAck   = par("interval_timeout_joinReqAck");
    param_interval_starPlayer           = par("interval_startPlayer");

    param_nNeighbor_SchedulingStart     = par("nNeighbor_SchedulingStart");
    param_waitingTime_SchedulingStart   = par("waitingTime_SchedulingStart");

    param_requestFactor_moderate        = par("requestFactor_moderate");
    param_requestFactor_aggresive       = par("requestFactor_aggressive");
    param_maxNOP                        = par("maxNOP");
    param_offsetNOP                     = par("offsetNOP");
    param_threshold_scarity             = par("threshold_scarity");

    param_minNOP = param_maxNOP - param_offsetNOP;
    param_factor_requestList            = par("factor_requestList").doubleValue();

    param_interval_waitingPartnershipResponse   = par("interval_waitingPartnershipResponse").doubleValue();

    scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

    // m_nChunk_perSchedulingInterval = param_interval_chunkScheduling * param_downBw / param_chunkSize / 8;

    m_nChunk_toRequest_perCycle = (int)(m_appSetting->getVideoStreamChunkRate() \
                    * param_interval_chunkScheduling);
                    //* param_baseValue_requestGreedyFactor);
//    m_bmPacket = generateBufferMapPacket();

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

    // -- Unimportant signals:
    //signal_nPartner         = registerSignal("Signal_nPartner");
    sig_newchunkForRequest  = registerSignal("Signal_nNewChunkForRequestPerCycle");
    sig_nPartner            = registerSignal("Signal_nPartner");

    sig_joinTime            = registerSignal("Signal_joinTime");
    sig_playerStartTime     = registerSignal("Signal_playerStartTime");

    sig_pRequestSent        = registerSignal("Signal_pRequestSent");
    sig_pRejectReceived     = registerSignal("Signal_pRejectReceived");

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
    // --------------------------- WATCH ---------------------------------------
    // -------------------------------------------------------------------------

    WATCH(m_localAddress);
    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_interval_bufferMap);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);
    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_BufferMapPacketSize_bit);
    WATCH(param_upBw);
    WATCH(param_downBw);
    WATCH(param_interval_chunkScheduling);
    WATCH(m_nChunk_toRequest_perCycle);
    //WATCH(m_nChunk_perSchedulingInterval);

    WATCH(m_state);

    WATCH(param_minNOP);
    WATCH(param_maxNOP);
    WATCH(param_offsetNOP);

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
    if (timer_getJoinTime != NULL)      { delete cancelEvent(timer_getJoinTime);        timer_getJoinTime       = NULL; }
    if (timer_join != NULL)             { delete cancelEvent(timer_join);               timer_join              = NULL; }
    if (timer_sendBufferMap != NULL)    { delete cancelEvent(timer_sendBufferMap);      timer_sendBufferMap     = NULL; }
    if (timer_chunkScheduling != NULL)  { delete cancelEvent(timer_chunkScheduling);    timer_chunkScheduling   = NULL; }
    if (timer_findMorePartner != NULL)  { delete cancelEvent(timer_findMorePartner);    timer_findMorePartner   = NULL; }
    if (timer_startPlayer != NULL)      { delete cancelEvent(timer_startPlayer);        timer_startPlayer       = NULL; }
    if (timer_timeout_waiting_response != NULL)
    {
        delete cancelEvent(timer_timeout_waiting_response);
        timer_timeout_waiting_response       = NULL;
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
    else if (msg == timer_startPlayer)
    {
        startPlayer();

    }
    else if (msg == timer_getJoinTime)
    {
        m_arrivalTime = m_churn->getArrivalTime();
        scheduleAt(simTime() + m_arrivalTime, timer_join);
    }
    else if (msg == timer_join)
    {
        //join();
        handleTimerJoin();

        // -- Schedule for a rejoin (if any)
        // scheduleAt(simTime() + param_interval_rejoin, timer_join);
    }
    else if (msg == timer_timeout_waiting_response)
    {
        handleTimerTimeoutWaitingAccept();
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

//void DonetPeer::processAcceptResponse(cPacket *pkt)
//{
//    EV << endl;
//    EV << "---------- Process Accept response ----------------------------------" << endl;

//    // -- Extract the address of the accepter
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress acceptorAddress = controlInfo->getSrcAddr();
//    EV << "-- Acceptor: " << acceptorAddress << endl;

//    // -- Extract the upBw of the accepter peer
//    MeshPartnershipAcceptPacket *acceptPkt = dynamic_cast<MeshPartnershipAcceptPacket *>(pkt);
//    double upBw_remotePeer = acceptPkt->getUpBw();
//    EV << "-- BW of acceptor: " << upBw_remotePeer << endl;

//    // 2. Store the address into its partner list
//    m_partnerList->addAddress(acceptorAddress, upBw_remotePeer);

////    m_activityLog << "--- processAcceptResponse --- " << endl;
////    m_activityLog << "\tGot an ACCEPT from: " << acceptorAddress << endl;
////    m_activityLog << "\tCurrent partnership size: " << m_partnerList->getSize() << endl;

//    m_nRequestSent = -1;
//    m_nRejectSent = 0;

//    ///// The following actions will be done
//    // TODO-Giang: state variable m_state_joined could be replaced by checking the size of the partnershiplist!!!
//    if (m_state_joined == false)
//    {
//        EV << "-- State will be changed from _IDLE_ to _JOINED_" << endl;

//        // -- Register myself to the ActivePeerList
//        IPvXAddress myAddress = getNodeAddress();
//        EV << "-- My own address: " << myAddress << endl;

//        EV << "Register node's own address to ActivePeerTable" << endl;
//        m_apTable->addPeerAddress(myAddress);
//        m_apTable->printActivePeerTable();

//        m_state_joined = true;

//        // -- Start several timers
//        // -- 1. Chunk Scheduling timers
//        double rand_value = uniform(0.0, 1.0);
//        scheduleAt(simTime() + rand_value, timer_chunkScheduling);
//        EV << "-- Chunk scheduling starts after " << rand_value << " seconds" << endl;

//        // -- 2. Send buffer map timers
//        rand_value = uniform(0.0, 0.5);
//        scheduleAt(simTime() + rand_value, timer_sendBufferMap);
//        EV << "-- Schedule for sending Buffer Map after " << rand_value << " seconds" << endl;

//        // -- 3. Have more partner timers
//        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);
//        EV << "-- Finding more partners starts after " << param_interval_findMorePartner << " seconds" << endl;

//        // -- 4. Start Player?
//        scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);
//        EV << "-- Player will be tried to start after " << param_interval_starPlayer << " seconds" << endl;

//        // -- Debug
//        // m_gstat->reportMeshJoin();

//        // -- Record join time (first accept response)
//        m_joinTime = simTime().dbl();

//        // Debuging with signals
//        emit(sig_joinTime, simTime().dbl());

//    }
//}

void DonetPeer::processPartnershipAccept(cPacket *pkt)
{
    EV << endl;
    EV << "---------- Processing a partnership ACCEPT packet -------------------" << endl;

    // -- Extract the address of the accepter
    PendingPartnershipRequest acceptor;
    MeshPartnershipAcceptPacket *acceptPkt = dynamic_cast<MeshPartnershipAcceptPacket *>(pkt);
    //DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        getSender(pkt, acceptor.address, acceptor.port);
        acceptor.upBW = acceptPkt->getUpBw();

    EV << "Acceptor: " << endl
       << "-- Address:\t\t"         << acceptor.address
       << "-- Port:\t\t"            << acceptor.port << endl
       << "-- Upload bandwidth:\t"  << acceptor.upBW << endl;

    switch(m_state)
    {
    case MESH_JOIN_STATE_IDLE_WAITING:
    {
        // -- Store the address of acceptor into its partner list
        m_partnerList->addAddress(acceptor.address, acceptor.upBW);

        // -- Register itself to the APT
        m_apTable->addPeerAddress(getNodeAddress(), param_maxNOP);
        m_apTable->printActivePeerInfo(getNodeAddress());
        m_apTable->printActivePeerTable();

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
            EV << "-- Finding more partners will be triggered after " << param_interval_findMorePartner << " seconds" << endl;
        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

        // -- 4. Start Player?
            EV << "-- Player will be tried to start after " << param_interval_starPlayer << " seconds" << endl;
            // -- Activate the Player
            m_player->activate();
        //scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);

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
        m_partnerList->addAddress(acceptor.address, acceptor.upBW);
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

    if (m_partnerList->getSize() >= param_minNOP)
    {
        EV << "Minimum number of partners has been reach --> stop finding more partner for the moment." << endl;
        return false;
    }

    IPvXAddress addressRandPeer;
    do
    {
        addressRandPeer = m_apTable->getARandPeer();
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
        findPartner();
        scheduleAt(simTime() + param_interval_waitingPartnershipResponse, timer_timeout_waiting_response);

        EV << "State changes to waiting mode" << endl;
        m_state = MESH_JOIN_STATE_IDLE_WAITING;
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

        // -- State changes to waiting mode
        if (ret == true)
            m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
        else
           // State remains ACTIVE

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

void DonetPeer::initializeSchedulingWindow()
{
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
void DonetPeer::chunkScheduling(void)
{
    if (m_partnerList->getSize() <= 0)
    {
        EV << "No partner just yet, exiting from chunk scheduling" << endl;
        return;
    }

    if (m_seqNum_schedWinHead <= 0)
    {
        EV << "Scheduling Window is empty, exiting from chunk scheduling" << endl;
        return;
    }

    emit(sig_nPartner, m_partnerList->getSize());

    randomChunkScheduling();
    //    donetChunkScheduling();
}


void DonetPeer::randomChunkScheduling(void)
{
    EV << endl;
    EV << "---------- Random chunk scheduling ----------------------------------" << endl;
    //srand(time(NULL));

    // -- Clear state variables
    m_nChunkRequested_perSchedulingInterval = 0;

    // -- Clear all request windows for all neighbors
    m_partnerList->clearAllSendBm();

    // -- Prepare the scheduling window
    long upper_bound = m_seqNum_schedWinHead;
    long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);

    // -- Update bounds of all sendBM
    //m_partnerList->updateBoundSendBm(m_seqNum_schedWinHead, lower_bound, lower_bound+m_bufferMapSize_chunk-1);
    m_partnerList->updateBoundSendBm(lower_bound, lower_bound+m_bufferMapSize_chunk-1);

    EV << "-- Scheduling window: [" << lower_bound << " - " << upper_bound << "]" << endl;
    EV << "-- This node has " << m_partnerList->getSize() << " partners" << endl;
    EV << "-- Initial state of the request Bm:" << endl;
    m_partnerList->printAllSendBm();

    int nNewChunkForRequest_perSchedulingCycle = 0;

    EV << "----- Browsing through the chunks within the scheduling windows-----" << endl;
    // -- Looking for chunk to request
    for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
    {
        EV << "-- Investigating chunk " << seq_num << ": ";
        if (m_videoBuffer->isInBuffer(seq_num) == false)
        {
            EV << "-- This chunk is NOT in the Buffer" << endl;
            // -- Debug -- Counting
            ++nNewChunkForRequest_perSchedulingCycle;

            if (should_be_requested(seq_num) == false)
            {
                EV << "----> This chunk should NOT be requested this time" << endl;
                continue;
            }

            // -- Get list of neighbors which has this chunk in their buffer map
            std::vector<IPvXAddress> holderList;
            m_partnerList->getHolderList(seq_num, holderList);
            int nHolder = holderList.size();

            // EV << "-- There are " << nHolder << " suppliers" << endl;

            // if there is at least one neighbor
            if (nHolder > 0)
            {
                int index = -1;
                if (nHolder == 1)
                {
                    EV << "  -- There is only one holder of the chunk." << " -- ";
                    index = 0;
                }
                else
                {
                    //index = (int)(rand() % holderList.size());
                    index = (int)intrand(holderList.size());
                    EV << "  -- There are " << nHolder << " holders, random index: " << index << " -- ";
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
                EV << "  -- There is no holder" << endl;
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
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        if (iter->second->isSendBmModified() == true)
        {
            EV << "Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
            iter->second->printSendBm();

            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket("MESH_PEER_CHUNK_REQUEST");
                chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
                // -- Map the sendBM into ChunkRequestPacket
                iter->second->copyTo(chunkReqPkt);

            // -- Send the copy
            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
        }
    } // end of for

    // -- Now refreshing the m_list_requestedChunk
    refreshListRequestedChunk();

    EV << endl;
} // Random Chunk Scheduling


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
            NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(holderList[0]);
            // -- Set the respective element in the SendBm to say that this chunk should be requested
            nbr_info->setElementSendBm(expected_set[i], true);

            // -- Get peer's upload bandwidth
            double peerUpBw = nbr_info->getUpBw();

            // -- Browse through the expected_set
            int currentSize = copied_expected_set.size();
            for (int k = 0; k < currentSize; ++k)
            {
                nbr_info->updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw);
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
            NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(supplier);
            // -- Set the respective element in the SendBm to say that this chunk should be requested
            nbr_info->setElementSendBm(seq_num, true);

            // -- Get peer's upload bandwidth
            double peerUpBw = nbr_info->getUpBw();

            // -- Browse through the expected_set
            int currentSize = copied_expected_set.size();
            for (int k = 0; k < currentSize; ++k)
            {
                nbr_info->updateChunkAvailTime(copied_expected_set[k], (param_chunkSize*8)/peerUpBw);
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
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        if (iter->second->isSendBmModified() == true)
        {
            EV << "-------Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
            iter->second->printSendBm();

            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket;
                chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
                // -- Map the sendBM into ChunkRequestPacket
                iter->second->copyTo(chunkReqPkt);

            // -- Send the copy
            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
        }
    } // end of for
} // Donet Chunk Scheduling

/**
 * Used for Donet chunk scheduling
 */
int DonetPeer::selectOneCandidate(SEQUENCE_NUMBER_T seq_num, IPvXAddress candidate1, IPvXAddress candidate2, IPvXAddress &supplier)
{

    int ret = 0;
    // -- Get pointer to the respective NeighborInfo
    NeighborInfo *info_candidate1 = m_partnerList->getNeighborInfo(candidate1);
    NeighborInfo *info_candidate2 = m_partnerList->getNeighborInfo(candidate2);

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
    m_player = check_and_cast<Player *>(temp);
    EV << "Binding to churnModerator is completed successfully" << endl;

}

//void DonetPeer::processPartnershipRequest(cPacket *pkt)
//{
//    EV << endl;
//    EV << "-------- Process partnership Request --------------------------------" << endl;

//    // -- Get the identifier (IP:port) of the requester
//    IPvXAddress address_requesteringNode;
//    int port_requestingNode;
//    getSender(pkt, address_requesteringNode, port_requestingNode);
//    EV << "Requester: " << address_requesteringNode << ":" << port_requestingNode << endl; // Debug

//    switch(m_state)
//    {
//    case MESH_JOIN_STATE_ACTIVE_WAITING:
//    {
//        // TODO-Giang: the logic of this join process has flaws, try to fix it ASAP
//        if (canAcceptMorePartner())
//        {
//            EV << "-- Can accept this request" << endl;
//            // -- Debug
//            emit(sig_partnerRequest, m_partnerList->getSize());

//            // 1.1. Get the upBw of the remote peer
//            MeshPartnershipRequestPacket *memPkt = check_and_cast<MeshPartnershipRequestPacket *>(pkt);
//                m_address_candidate = address_requesteringNode;
//                m_upBw_candidate    = memPkt->getUpBw();
//            EV << "-- Candidate for partnership: " << m_address_candidate
//               << "with upload bandwidth: " << m_upBw_candidate << endl;

//            //m_partnerList->addAddress(requesterAddress, upBw_remotePeer);

//            //m_partnerList->print(); // Debug

//            MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
//            sendToDispatcher(acceptPkt, m_localPort, address_requesteringNode, port_requestingNode);

//            m_state = MESH_JOIN_STATE_ACTIVE_WAITING;
//        }
//        else
//        {
//            EV << "-- Enough partners --> cannot accept this request." << endl;
//            emit(sig_partnerRequest, 0);

//            // -- Create a Partnership message and send it to the remote peer
//            MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
//            sendToDispatcher(rejectPkt, m_localPort, address_requesteringNode, port_requestingNode);
//        }

//        break;
//    }
//    case MESH_JOIN_STATE_ACTIVE_WAITING:
//    {
//        m_list_joinRequestingNode.push_back(address_requesteringNode);
//        m_state = MESH_JOIN_STATE_ACTIVE_WAITING; // state remains
//        break;
//    }
//    case MESH_JOIN_STATE_IDLE:
//    case MESH_JOIN_STATE_IDLE_WAITING:
//    {
//        throw cException("Unexpected event: A join request could not be sent to an idel node");
//        break;
//    }
//    default:
//    {
//        throw cException("Uncovered state");
//        break;
//    }
//    } // switch()

//}


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
    NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);

    EV << "-- Received a buffer map from " << senderAddress << endl;

    // Get the record respective to the address of the partner
    if (nbr_info == NULL)
    {
        EV << "-- Buffer Map is received from a non-partner peer!" << endl;
        return;
    }

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

    EV << "-- Buffer Map information: " << endl;
    EV << "  -- Start:\t"   << bmPkt->getBmStartSeqNum()    << endl;
    EV << "  -- End:\t"     << bmPkt->getBmEndSeqNum()      << endl;
    EV << "  -- Head:\t"    << bmPkt->getHeadSeqNum()       << endl;

    // -- Copy the BufferMap content to the current record
    nbr_info->copyFrom(bmPkt);

    // -- Update the timestamp of the received BufferMap
    nbr_info->setLastRecvBmTime(simTime().dbl());

    // -- Update the range of the scheduling window
    m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;
    EV << "-- New head for the scheduling window: " << m_seqNum_schedWinHead << endl;

    // -- Debug
    ++m_nBufferMapRecv;
    nbr_info->printRecvBm();
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
        vector<PendingPartnershipRequest>::iterator iter = m_list_partnershipRequestingNode.begin();
        for (; iter != m_list_partnershipRequestingNode.end(); ++iter)
        {
            considerAcceptPartner(*iter);
        }
        m_list_partnershipRequestingNode.clear();

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
