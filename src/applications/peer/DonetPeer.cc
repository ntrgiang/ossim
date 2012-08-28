#include "InterfaceTableAccess.h"
#include "IPv4InterfaceData.h"

#include "DonetPeer.h"
#include "IChurnGenerator.h"
#include "DpControlInfo_m.h"
#include "MeshPeerStreamingPacket_m.h"

#include <assert.h>
#include <algorithm>

Define_Module(DonetPeer);

DonetPeer::DonetPeer() {}
DonetPeer::~DonetPeer() {}

void DonetPeer::initialize(int stage)
{
    if (stage == 0)
    {
        sig_chunkRequestSeqNum = registerSignal("chunkRequestSeqNum");
        sig_partnerRequest      = registerSignal("Signal_PartnerRequest");

        return;
    }
    if (stage != 3)
        return;

    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    // -------------------------------------------------------------------------
    // --------------------------- State variables------------------------------
    // -------------------------------------------------------------------------
    m_joined = m_scheduling_started = false;

    // -------------------------------------------------------------------------
    // -------------------------------- Timers ---------------------------------
    // -------------------------------------------------------------------------
    // -- One-time timers
    timer_getJoinTime       = new cMessage("MESH_PEER_TIMER_GET_JOIN_TIME");
    timer_join              = new cMessage("MESH_PEER_TIMER_JOIN");

    // -- Repeated timers
    // timer_keepAlive         = new cMessage("MESH_PEER_TIMER_KEEP_ALIVE");
    timer_chunkScheduling   = new cMessage("MESH_PEER_TIMER_CHUNK_SCHEDULING");
    timer_sendBufferMap     = new cMessage("MESH_PEER_TIMER_SEND_BUFFERMAP");
    timer_findMorePartner   = new cMessage("MESH_PEER_TIMER_FIND_MORE_PARTNER");
    timer_startPlayer       = new cMessage("MESH_PEER_TIMER_START_PLAYER");

    // Parameters of the module itself
    param_bufferMapInterval             = par("bufferMapInterval");
    param_chunkSchedulingInterval       = par("chunkSchedulingInterval");
    param_interval_chunkScheduling      = par("interval_chunkScheduling");
    param_interval_findMorePartner      = par("interval_findMorePartner");
    param_interval_starPlayer           = par("interval_startPlayer");

    param_nNeighbor_SchedulingStart     = par("nNeighbor_SchedulingStart");
    param_waitingTime_SchedulingStart   = par("waitingTime_SchedulingStart");
    param_numberOfPartner               = par("numberOfPartner");
//    param_minNOP                        = par("minNOP");
    param_maxNOP                        = par("maxNOP");
    param_offsetNOP                     = par("offsetNOP");

    param_minNOP = param_maxNOP - param_offsetNOP;

    scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

//    m_bmPacket = generateBufferMapPacket();

    // --------- Debug ------------
    m_joinTime = -1.0;
    m_monitoredAddress = IPvXAddress("192.168.0.11");
    // -- Init for activity logging:
//    string path = "..//results//";
//    string filename = path + getNodeAddress().str();
//    m_activityLog.open(filename.c_str(), fstream::out);

//    WATCH(filename.c_str());

    // -- State variable for join process
    m_nRequestSent = m_nRejectSent = 0;

    // -- Unimportant signals:
    signal_nPartner = registerSignal("Signal_nPartner");

    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_bufferMapInterval);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);
    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_BufferMapPacketSize_bit);
    WATCH(param_upBw);
    WATCH(param_chunkSchedulingInterval);

    WATCH(param_numberOfPartner);
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
    // Cancel all scheduled events
    if (timer_getJoinTime)      delete cancelEvent(timer_getJoinTime);
    if (timer_join)             delete cancelEvent(timer_join);
    if (timer_sendBufferMap)    delete cancelEvent(timer_sendBufferMap);
    if (timer_chunkScheduling)  delete cancelEvent(timer_chunkScheduling);
    if (timer_findMorePartner)  delete cancelEvent(timer_findMorePartner);
    if (timer_startPlayer)      delete cancelEvent(timer_startPlayer);

    ///// No impacts
//    emit(signal_nPartner, m_partnerList->size());

    /*
    cancelAndDelete(timer_getJoinTime);
    cancelAndDelete(timer_join);
    cancelAndDelete(timer_keepAlive);
    cancelAndDelete(timer_chunkScheduling);
    */

    // -- Debug
    m_gstat->reportNumberOfPartner(m_partnerList->getSize());

//    m_activityLog.close();

    Partnership p;
        p.address = getNodeAddress();
        p.arrivalTime = m_arrivalTime;
        p.joinTime = m_joinTime;
        p.nPartner = m_partnerList->getSize();
        p.video_startTime = m_video_startTime;
        p.head_videoStart = m_head_videoStart;
        p.begin_videoStart = m_begin_videoStart;
        p.threshold_videoStart = m_threshold_videoStart;
    m_meshOverlayObserver->writeToFile(p);
}

void DonetPeer::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_sendBufferMap)
    {
        sendBufferMap();
        scheduleAt(simTime() + param_bufferMapInterval, timer_sendBufferMap);
    }
    else if (msg == timer_chunkScheduling)
    {
        chunkScheduling();
        scheduleAt(simTime() + param_chunkSchedulingInterval, timer_chunkScheduling);
    }
    else if (msg == timer_findMorePartner)
    {
        findMorePartner();
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
        join();
    }

}

void DonetPeer::processPacket(cPacket *pkt)
{
    PeerStreamingPacket *appMsg = dynamic_cast<PeerStreamingPacket *>(pkt);
    // EV << "appMsg->getMsgType() =" << appMsg->getMsgType() << endl;
    if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
    {
        //EV << "Wrong packet type!" << endl;
        throw cException("Wrong packet type!");
        return;
    }

    MeshPeerStreamingPacket *meshMsg = dynamic_cast<MeshPeerStreamingPacket *>(appMsg);
    switch (meshMsg->getPacketType())
    {
    case MESH_PARTNERSHIP_REQUEST:
    {
        processPartnershipRequest(pkt);
        break;
    }
    case MESH_PARTNERSHIP_ACCEPT:
    {
        processAcceptResponse(pkt);
        break;
    }
    case MESH_BUFFER_MAP:
    {
        processPeerBufferMap(pkt);
//        processPeerBufferMap2(pkt);
        break;
    }
    case MESH_CHUNK_REQUEST:
    {
        processChunkRequest(pkt);
        break;
    }
    case MESH_PARTNERSHIP_REJECT:
    {
        processRejectResponse(pkt);
        break;
    }
    default:
    {
        throw cException("Unrecognized packet types! %d", meshMsg->getPacketType());
        break;
    }
    } // switch

    // EV << "going to delete packet ..." << endl;
    delete pkt;
}

void DonetPeer::processAcceptResponse(cPacket *pkt)
{
    // -- Extract the address of the accepter
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress acceptorAddress = controlInfo->getSrcAddr();
    EV << "Accepter's address: " << acceptorAddress << endl;

    // -- Extract the upBw of the accepter peer
    MeshPartnershipAcceptPacket *acceptPkt = dynamic_cast<MeshPartnershipAcceptPacket *>(pkt);
    double upBw_remotePeer = acceptPkt->getUpBw();
    EV << "Accepter's upload BW: " << upBw_remotePeer << endl;

    // 2. Store the address into its partner list
    m_partnerList->addAddress(acceptorAddress, upBw_remotePeer);

//    m_activityLog << "--- processAcceptResponse --- " << endl;
//    m_activityLog << "\tGot an ACCEPT from: " << acceptorAddress << endl;
//    m_activityLog << "\tCurrent partnership size: " << m_partnerList->getSize() << endl;

    m_nRequestSent = -1;
    m_nRejectSent = 0;

    ///// The following actions will be done
    // TODO-Giang: state variable m_joined could be replaced by checking the size of the partnershiplist!!!
    if (m_joined == false)
    {
        // -- Register myself to the ActivePeerList
        IPvXAddress myAddress = getNodeAddress();
        EV << "My own address: " << myAddress << endl;

        m_apTable->addPeerAddress(myAddress);
        m_apTable->printActivePeerTable();

        m_joined = true;
        // m_firstJoinTime = simTime().dbl();

        // -- Start several timers
        // -- 1. Chunk Scheduling timers
        scheduleAt(simTime() + param_chunkSchedulingInterval, timer_chunkScheduling);

        // -- 2. Send buffer map timers
        scheduleAt(simTime() + param_bufferMapInterval, timer_sendBufferMap);

        // -- 3. Have more partner timers
        scheduleAt(simTime() + param_interval_findMorePartner, timer_findMorePartner);

        // -- 4. Start Player?
        scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);

        // -- Debug
        m_gstat->reportMeshJoin();

        // -- Record join time (first accept response)
        m_joinTime = simTime().dbl();

    }

    //delete pkt;
}

void DonetPeer::processRejectResponse(cPacket *pkt)
{
    // -- Extract the address of the accepter
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress rejectorAddress = controlInfo->getSrcAddr();
    EV << "Accepter's address: " << rejectorAddress << endl;

//    m_activityLog << "--- processRejectResponse --- " << endl;
//    m_activityLog << "\t at " << simTime().dbl() << " Got a REJECT from: " << rejectorAddress << endl;

    ++m_nRejectSent;

    if (m_nRejectSent == m_nRequestSent)
    {
//        m_activityLog << "\tAll requests were rejected --> join() again!" << endl;

        // Reset state-variables
        m_nRequestSent = -1;
        m_nRejectSent = 0;
        join();
    }

}


void DonetPeer::join()
{
    std::vector<IPvXAddress> listRandPeer = m_apTable->getNPeer(param_numberOfPartner/2 + 1); //param_numberOfPartner-2
    EV << listRandPeer.size() << " peers will be requested!" << endl;

    m_nRequestSent = listRandPeer.size();

    for (std::vector<IPvXAddress>::iterator iter = listRandPeer.begin(); iter != listRandPeer.end(); ++iter)
    {
        // -- Only send a partnership request if not enough partners
        //if (m_partnerList->getSize() < param_numberOfPartner)
        if (m_partnerList->getSize() < param_minNOP)
        {
            IPvXAddress addressRandPeer = *iter;

            MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
                reqPkt->setUpBw(param_upBw);
                reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

                sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);
        }
    }

//            m_activityLog << "--- Join ---" << endl;
//            m_activityLog << "\tat " << simTime().dbl() << " Bootstrap list: size = " << listRandPeer.size() << endl;
            for (std::vector<IPvXAddress>::iterator iter = listRandPeer.begin(); iter != listRandPeer.end(); ++iter)
            {
                IPvXAddress addressRandPeer = *iter;

//                m_activityLog << "\t" << addressRandPeer << endl;
            }
    // ------- Debug ----------
}

/**
 * Each time triggered, get a random peer from tracker
 * - check whether the peer is its partner
 * - if NOT --> send request
 * - in both case, sleep another T before triggered again
 */
void DonetPeer::findMorePartner()
{
    //if (m_partnerList->getSize() >= param_numberOfPartner)
    if (m_partnerList->getSize() >= param_minNOP)
    {
//        emit(signal_nPartner, -1);
//
//        m_activityLog << "--- Find more partner --- " << endl;
//        m_activityLog << "\tBut has enough (mininum number of) partners!" << endl;
        return;
    }

    IPvXAddress addressRandPeer;
    do
    {
         addressRandPeer = m_apTable->getARandPeer();
    } while (addressRandPeer == getNodeAddress());

    EV << "A random peer " << addressRandPeer << " will be requested!" << endl;

    if (m_partnerList->have(addressRandPeer))
    {
//        emit(signal_nPartner, -2);

//        m_activityLog << "--- Find more partner --- " << endl;
//        m_activityLog << "\tBut already had partner!" << endl;
        return;
    }

//    m_activityLog << "--- Find more partner --- " << endl;
//    m_activityLog << "\tPartner to contat: " << addressRandPeer << endl;

//    emit(signal_nPartner, m_partnerList->size());
    MeshPartnershipRequestPacket *reqPkt = new MeshPartnershipRequestPacket("MESH_PEER_JOIN_REQUEST");
        reqPkt->setUpBw(param_upBw);
        reqPkt->setBitLength(m_appSetting->getPacketSizePartnershipRequest());

    sendToDispatcher(reqPkt, m_localPort, addressRandPeer, m_destPort);
}

void DonetPeer::initializeSchedulingWindow()
{

}

bool DonetPeer::shouldStartChunkScheduling(void)
{
    if (simTime().dbl() - m_firstJoinTime >= param_waitingTime_SchedulingStart
            || m_partnerList->getSize() >= param_nNeighbor_SchedulingStart)
    {
        return true;
    }

    return false;
}

bool DonetPeer::should_be_requested(void)
{
    return true;
}

/*
 * Depending on _strategy_ which is applied, the schedulingWindows will be initialized accordingly
 */
//void DonetPeer::initializeSchedulingWindow(void)
//{
//    // Relative lag
//
//
//    // Absolute lag
//}

/*
 * This function is a _wrapper_ for the specific chunk scheduling algorithm
 */
void DonetPeer::chunkScheduling(void)
{
    // The logic of the following lines were moved into another function, shouldStartChunkScheduling()
    // !!!
    // -- Get the number of neighbors
    // if exceeding some threshold (default = 1) --> start scheduling
    // if (m_partnerList->getSize() < param_nNeighbor_SchedulingStart)
    //     return;

    // -- Debug
    // EV << "Start chunk scheduling" << endl;

    if (m_partnerList->getSize() <= 0)
    {
        // -- Debug
        // EV << "No partner just yet, exiting from chunk scheduling" << endl;
        return;
    }

    if (m_seqNum_schedWinHead <= 0)
    {
        // -- Debug
        // EV << "Scheduling Window is empty, exiting from chunk scheduling" << endl;
        return;
    }

    randomChunkScheduling();
    //    donetChunkScheduling();
}


void DonetPeer::randomChunkScheduling(void)
{
    // EV << "random chunk scheduling triggered! " << endl;

    // -- Clear all request windows for all neighbors
    m_partnerList->clearAllSendBm();

    // -- Prepare the scheduling window
    long upper_bound = m_seqNum_schedWinHead;
    long lower_bound = std::max(0L, m_seqNum_schedWinHead-m_bufferMapSize_chunk+1);

    // -- Update bounds of all sendBM
    m_partnerList->updateBoundSendBm(m_seqNum_schedWinHead, lower_bound, lower_bound+m_bufferMapSize_chunk-1);

    // DEBUG
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
        EV << "Scheduling window: [" << lower_bound << " - " << upper_bound << "]" << endl;
        m_partnerList->printAllSendBm();

        //EV << "My partners: ";
        //m_partnerList->print();
    #endif
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

    // -- Looking for chunk to request
    for (SEQUENCE_NUMBER_T seq_num = lower_bound; seq_num <= upper_bound; ++seq_num)
    {
        // -- Debug
        // EV << "Investigating chunk " << seq_num << ": ";
        if (m_videoBuffer->isInBuffer(seq_num) == false)
        {
            if (should_be_requested() == false)
            {
                // -- Debug
                // EV << "this chunk should not be requested this time" << endl;
                continue;
            }

            // -- Debug
            //EV << "Looking for supplier of the chunk ... ";

            // -- Get list of neighbors which has this chunk in their buffer map
            std::vector<IPvXAddress> holderList;
            m_partnerList->getHolderList(seq_num, holderList);
            int nHolder = holderList.size();

            // -- Debug
            // EV << "There are " << nHolder << " suppliers" << endl;

            // if there is at least one neighbor
            if (nHolder > 0)
            {
                // -- Debug
                EV << nHolder << " ";

                int index = 0;
                if (nHolder == 1)
                {
                    index = 0;
                }
                else
                {
                    index = (int)(rand() % holderList.size());
                } // if

                // -- Debug
                // EV << "holder for chunk " << seq_num << " is " << holderList[index] << endl;

                // -- Preparing to access record of partner to set the sendBM
                NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(holderList[index]);

                // -- Debug
                // if (!nbr_info) EV << "Null pointer for neighborInfo" << endl;
//                m_reqChunkId.record(seq_num);

                nbr_info->setElementSendBm(seq_num, true);

                // -- Emitting signals for statistics collection
                emit(sig_chunkRequestSeqNum, seq_num);

            } // if (nHolder > 0)

        } // end of if(inBuffer)
        else
        {
            // -- Debug
            // EV << "this is already in VideoBuffer" << endl;
        }
    } // end of for

    // -- Browse through the list of partners to see which one have been set the sendBm
    // -- For each of those one, prepare a suitable ChunkRequestPacket and send to that one
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter = m_partnerList->m_map.begin(); iter != m_partnerList->m_map.end(); ++iter)
    {
        if (iter->second->isSendBmModified() == true)
        {
            EV << "-------Destination of the ChunkRequestPacket " << iter->first << " :" << endl;
            iter->second->printSendBm();

            MeshChunkRequestPacket *chunkReqPkt = new MeshChunkRequestPacket("MESH_PEER_CHUNK_EQUEST");
                chunkReqPkt->setBitLength(m_appSetting->getPacketSizeChunkRequest());
                // -- Map the sendBM into ChunkRequestPacket
                iter->second->copyTo(chunkReqPkt);

            // -- Send the copy
            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
        }
    } // end of for
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
    m_partnerList->updateBoundSendBm(m_seqNum_schedWinHead, lower_bound, lower_bound+m_bufferMapSize_chunk-1);

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

            // -- Map the sendBM into ChunkRequestPacket
            iter->second->copyTo(chunkReqPkt);

            // -- Send the copy
            sendToDispatcher(chunkReqPkt, m_localPort, iter->first, m_destPort);
        }
    } // end of for
} // Donet Chunk Scheduling

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
    //assert(m_churn != NULL);
    if (m_churn == NULL) throw cException("m_churn == NULL is invalid");
}

void DonetPeer::bindToMeshModule(void)
{
    DonetBase::bindToMeshModule();

    // -- Player
    cModule *temp = getParentModule()->getModuleByRelativePath("player");
    m_player = dynamic_cast<Player *>(temp);
    if (!m_player)
        throw cException("Null pointer to Player module!");

//    assert(m_churn != NULL);
}

void DonetPeer::processPartnershipRequest(cPacket *pkt)
{
    // -- Get the identifier (IP:port) of the requester
    IPvXAddress requesterAddress;
    int requesterPort;
    getSender(pkt, requesterAddress, requesterPort);
    EV << "Requester: " << requesterAddress << ":" << requesterPort << endl; // Debug

//    m_activityLog << "--- Partnership Request --- " << endl;
//    m_activityLog << "\tGot a REQUEST from: " << requesterAddress << endl;
//    m_activityLog << "\tCurrent partnership size: " << m_partnerList->getSize() << endl;


    //if (canHaveMorePartner())
    if (m_partnerList->getSize() < param_maxNOP)
    {
        // -- Debug
        emit(sig_partnerRequest, m_partnerList->getSize());
//        emit(sig_partnerRequest, param_numberOfPartner);

        // 1.1. Get the upBw of the remote peer
        MeshPartnershipRequestPacket *memPkt = dynamic_cast<MeshPartnershipRequestPacket *>(pkt);
        double upBw_remotePeer = memPkt->getUpBw();
        EV << "up load bandwidth: " << upBw_remotePeer << endl;

        m_partnerList->addAddress(requesterAddress, upBw_remotePeer);

        Link link;
            link.timeStamp = simTime();
            link.linkType = 1;
            link.root = getNodeAddress();
            link.head = requesterAddress;
//        m_meshOverlayObserver->writeToFile(link);

        // m_partnerList->print(); // Debug:
        // TODO: insert code to set the packet size here!
//        MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
//            acceptPkt->setUpBw(param_upBw);
//            acceptPkt->setBitLength(m_appSetting->getPacketSizePartnershipAccept());

        MeshPartnershipAcceptPacket *acceptPkt = generatePartnershipRequestAcceptPacket();
        sendToDispatcher(acceptPkt, m_localPort, requesterAddress, requesterPort);

//        m_activityLog << "\tAccept it " << m_partnerList->getSize() << endl;
    }
    else // cannot accept any more partner
    {
        // -- Debug
        emit(sig_partnerRequest, 0);

        // -- Create a Partnership message and send it to the remote peer
        // TODO: insert code to set the packet size here!
//        MeshPartnershipRejectPacket *rejPkt = new MeshPartnershipRejectPacket("MESH_PARTNERSHIP_REJECT");
//            rejPkt->setBitLength(m_appSetting->getPacketSizePartnershipReject());
        MeshPartnershipRejectPacket *rejectPkt = generatePartnershipRequestRejectPacket();
        sendToDispatcher(rejectPkt, m_localPort, requesterAddress, requesterPort);

//        m_activityLog << "\tReject it " << m_partnerList->getSize() << endl;
    }
}


void DonetPeer::processPeerBufferMap(cPacket *pkt)
{
    // EV << "received BufferMap packet! --------------------" << endl;
    // -- Module validation
    // r_countBM.record(1);
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

    // Get the record respective to the address of the partner
    if (nbr_info == NULL)
    {
        EV << "Buffer Map is received from a non-partner peer!" << endl;
        return;
    }

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);

    // -- Copy the BufferMap content to the current record
    nbr_info->copyFrom(bmPkt);

    // -- Update the timestamp of the received BufferMap
    nbr_info->setLastRecvBmTime(simTime().dbl());

    // -- Update the range of the scheduling window
    m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;

    //////////////////////////////////////////////////////////////////////////////////////////////// DEBUG_START /////////////////////////
    #if(__DONET_PEER_DEBUG__)
        EV << "A BufferMap received from: ";
//                                << " -- Start: " << bmPkt->getBmStartSeqNum()
//                                << " -- End: " << bmPkt->getBmEndSeqNum()
//                                << " -- Head: " << bmPkt->getHeadSeqNum()
//                                << " -- BM: ";
        m_partnerList->printRecvBm(senderAddress);
        EV << endl;
    #endif
    //////////////////////////////////////////////////////////////////////////////////////////////// DEBUG_END /////////////////////////

}

void DonetPeer::startPlayer(void)
{
    if (shouldStartPlayer())
    {
        m_player->startPlayer();
        if (timer_startPlayer)
            cancelEvent(timer_startPlayer);
    }
    else
    {
        scheduleAt(simTime() + param_interval_starPlayer, timer_startPlayer);
    }

}

bool DonetPeer::shouldStartPlayer(void)
{
    SEQUENCE_NUMBER_T head = m_videoBuffer->getHeadReceivedSeqNum();
    SEQUENCE_NUMBER_T begin = m_videoBuffer->getBufferStartSeqNum();
    int offset = head - begin;
    if (offset < 0) throw cException("Non-sense values of head and begin");

    // A simple logic to start the player
    // More sophisticated one could be included as nested if to "ensure" better playback quality
    if (offset >= m_bufferMapSize_chunk / 2)
    {
        m_video_startTime = simTime().dbl();
        m_head_videoStart = head;
        m_begin_videoStart = begin;
        m_threshold_videoStart = m_bufferMapSize_chunk / 2;
        return true;
    }
    return false;
}
/*
void DonetPeer::processChunkRequest(cPacket *pkt)
{
    IPvXAddress senderAddress;
    int senderPort;
    getSender(pkt, senderAddress, senderPort);
    // EV << "A chunk request packet has just been received from " << sourceAddress << " ! prepare to print packet's content" << endl;
    // EV << "----------------------------------------------------------" << endl;
    MeshChunkRequestPacket *reqPkt = check_and_cast<MeshChunkRequestPacket *>(pkt);

    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
        EV << "Chunk request received from " << senderAddress << ": " << endl;
        EV << "Start: " << reqPkt->getSeqNumMapStart()
            << " -- End: " << reqPkt->getSeqNumMapEnd()
            << " -- Head: " << reqPkt->getSeqNumMapHead()
            << " -- ";
        for (int i=0; i < m_bufferMapSize_chunk; ++i)
        {
            EV << reqPkt->getRequestMap(i);
        }
        EV << endl;
    #endif
    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

    // -- TODO: Need reply to chunk request here
    // -- Find the id of the requested chunk
    SEQUENCE_NUMBER_T start = reqPkt->getSeqNumMapStart();
    for (int offset=0; offset < m_bufferMapSize_chunk; ++offset)
    {
        if (reqPkt->getRequestMap(offset) == true)
        {
            SEQUENCE_NUMBER_T seqNum_requestedChunk = offset + start;

            // -- Look up to see if the requested chunk is available in the Video Buffer
            if (m_videoBuffer->isInBuffer(seqNum_requestedChunk) ==  true)
            {
                // -- If YES, duplicate the chunk and send to the requesting peer

                //MeshVideoChunkPacket *copyPkt = m_videoBuffer->getChunk(seqNum_requestedChunk)->dup();
                m_forwarder->sendChunk(seqNum_requestedChunk, senderAddress, senderPort);

                //sendToDispatcher(copyPkt, m_localPort, senderAddress, m_destPort);
            }
        }
    }
}
*/

/*
void DonetPeer::acceptJoinRequest(cPacket *pkt)
{
    Enter_Method("acceptJoinRequest");

    // 1. Get the IP-address of the peer
//    UDPPacket *pkt = check_and_cast<UDPPacket *>(msg);
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress sourceAddress = controlInfo->getSrcAddr();
//    int srcPort = controlInfo->getSrcPort();

    // 2. Store the address into its neighbor list
    m_partnerList->addAddress(sourceAddress);

    // 3. Check if this is the first neighbor, if YES --> trigger the Heartbeat module (to periodically start sending the hearbeat)
    if (m_partnerList->getSizem_partnerList() == 1)
    {
//        startSendingHeartbeat();
    }
    delete pkt;
}
*/
/*
void DonetPeer::acceptJoinRequest(cPacket *pkt)
{
    Enter_Method("acceptJoinRequest(pkt)");

    // 1. Get the IP-address of the peer
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress reqPeerAddress = controlInfo->getSrcAddr();

    // 1.1. Get the upBw of the remote peer
    MeshPartnershipPacket *meshPkt = dynamic_cast<MeshPartnershipPacket *>(pkt);
    double upBw_remotePeer = meshPkt->getUpBw();

    // 2. Store the address into its neighbor list
    // m_partnerList->addAddress(reqPeerAddress);
    m_partnerList->addAddress(reqPeerAddress, upBw_remotePeer);

    // Debug:
    m_partnerList->print();

    // 3. Send the explicit ACCEPT request
    MeshPartnershipPacket *acceptPkt = new MeshPartnershipPacket("MESH_PEER_JOIN");
        acceptPkt->setPacketType(MESH_PARTNERSHIP);
        acceptPkt->setCommand(CMD_MESH_PARTNERSHIP_ACCEPT);
        acceptPkt->setUpBw(param_upBw);

    sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);

    // 4. Check if this is the first neighbor, if YES --> trigger the Heartbeat module (to periodically start sending the hearbeat)
    if (m_partnerList->getSize() == 1)
    {
//        startSendingHeartbeat();
    }

    //delete pkt;
}
*/

// -- Moved to Base class
//
//void DonetPeer::addToNeighborList(cPacket *pkt)
//{
//    Enter_Method("addToNeighborList(pkt)");
//
//    // 1. Get the IP-address of the peer
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress reqPeerAddress = controlInfo->getSrcAddr();
//
//    // 2. Store the address into its neighbor list
//    m_partnerList->addAddress(reqPeerAddress);
//}

/*
void DonetPeer::startSendingHeartbeat()
{
    Enter_Method("startSendingHeartbeat()");

    std::vector<IPvXAddress> updateList;
    updateList = m_partnerList->getAddressList();

    std::vector<IPvXAddress>::iterator it;
    for (it=updateList.begin(); it != updateList.end(); ++it)
    {
        cPacket *heartbeatPkt = new cPacket("MESH_PEER_KEEP_ALIVE");
        sendToDispatcher(heartbeatPkt, localPort, *it, destPort);
    }
}
*/

// might not be used in the future
/*
void DonetPeer::processPeerBufferMap(IPvXAddress senderAddress, MeshPeerStreamingPacket *appMsg)
{
    // EV << "received BufferMap packet! --------------------" << endl;
    // -- Module validation
    // r_countBM.record(1);

    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(appMsg);
    int countOne = 0;
    for (unsigned int i = 0; i < bmPkt->getBufferMapArraySize(); i++)
    {
        countOne += bmPkt->getBufferMap(i)==true?1:0;
    }
    r_countBM.record(countOne);


    // Get the record respective to the address of the partner
    NeighborInfo *nbr_info = m_partnerList->getNeighborInfo(senderAddress);
    if (nbr_info == NULL)
    {
        EV << "Buffer Map is received from a non-partner peer!" << endl;
        return;
    }

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(appMsg);

    // -- Copy the BufferMap content to the current record
    nbr_info->copyFrom(bmPkt);

    // -- Update the timestamp of the received BufferMap
    nbr_info->setLastRecvBmTime(simTime().dbl());

    // -- Update the range of the scheduling window
    m_seqNum_schedWinHead = (bmPkt->getHeadSeqNum() > m_seqNum_schedWinHead)?
            bmPkt->getHeadSeqNum():m_seqNum_schedWinHead;

    //////////////////////////////////////////////////////////////////////////////////////////////// DEBUG_START /////////////////////////
    #if(__DONET_PEER_DEBUG__)
        EV << "A BufferMap received from: ";
//                                << " -- Start: " << bmPkt->getBmStartSeqNum()
//                                << " -- End: " << bmPkt->getBmEndSeqNum()
//                                << " -- Head: " << bmPkt->getHeadSeqNum()
//                                << " -- BM: ";
        m_partnerList->printRecvBm(senderAddress);
        EV << endl;
    #endif
    //////////////////////////////////////////////////////////////////////////////////////////////// DEBUG_END /////////////////////////
}
*/

/*
void DonetPeer::processPeerBufferMap2(cPacket *pkt)
{
    IPvXAddress senderAddress = getSender(pkt);

    // -- Cast to the BufferMap packet
    MeshBufferMapPacket *bmPkt = check_and_cast<MeshBufferMapPacket *>(pkt);
    int bufferSize = bmPkt->getBufferMapArraySize();

    EV << "Start: " << bmPkt->getBmStartSeqNum()
            << " -- End: " << bmPkt->getBmEndSeqNum()
            << " -- Head: " << bmPkt->getHeadSeqNum()
            << " -- Size: " << bufferSize
            << ": ";

    // -- Browse through all element of the array in the BufferMap packet
    for (int i=0; i < m_bufferMapSize_chunk; ++i)
    {
        short k = (bmPkt->getBufferMap(i)==true)?1:0;
        EV << k << " ";
    }

    EV << endl;
}
*/


/*
void DonetPeer::readChannelRate(void)
{
    cDatarateChannel *channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 1)->getTransmissionChannel());
    param_upBw = channel->getDatarate();

    // -- problem unresolved!!!
//    channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
//            ->getTransmissionChannel());
//    param_downBw = channel->getDatarate();
}
*/

// might not be used in the future
/*
void DonetPeer::processPartnershipRequest(MeshPeerStreamingPacket *meshMsg)
{
    // -- Get the IP-address of the requester
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(meshMsg->getControlInfo());
    IPvXAddress reqPeerAddress = controlInfo->getSrcAddr();
    m_destPort = controlInfo->getSrcPort();
    EV << "Sender: " << reqPeerAddress << ":" << m_destPort << endl;

    if (canHaveMorePartner() == true)
    {
        // 1.1. Get the upBw of the remote peer
        MeshPartnershipRequestPacket *memPkt = dynamic_cast<MeshPartnershipRequestPacket *>(meshMsg);
        double upBw_remotePeer = memPkt->getUpBw();
        EV << "up load bandwidth: " << upBw_remotePeer << endl;

        m_partnerList->addAddress(reqPeerAddress, upBw_remotePeer);

        // m_partnerList->print(); // Debug:
        MeshPartnershipAcceptPacket *acceptPkt = new MeshPartnershipAcceptPacket("MESH_PEER_JOIN_ACCEPT");
            acceptPkt->setUpBw(param_upBw);

        sendToDispatcher(acceptPkt, m_localPort, reqPeerAddress, m_destPort);
    }
    else // cannot accept any more partner
    {
        // -- Create a Partnership message and send it to the remote peer
        MeshPartnershipRejectPacket *rejPkt = new MeshPartnershipRejectPacket("MESH_PARTNERSHIP_REJECT");
        sendToDispatcher(rejPkt, m_localPort, reqPeerAddress, m_destPort);
    }
}
*/


