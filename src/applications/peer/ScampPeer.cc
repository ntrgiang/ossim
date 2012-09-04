//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "omnetpp.h"
#include "ScampPeer.h"
#include "IChurnGenerator.h"
#include "ActivePeerTable.h"
#include "AppCommon.h"
#include "MeshPeerStreamingPacket_m.h"

//#include "UDPControlInfo_m.h"
#include "DpControlInfo_m.h"

#include "UDPPacket.h"
#include <IPAddressResolver.h>
#include "InterfaceTableAccess.h"
#include "IPv4InterfaceData.h"

#include <vector>
#include <algorithm>

Define_Module(ScampPeer);

ScampPeer::ScampPeer()
{
    // TODO Auto-generated constructor stub
}

ScampPeer::~ScampPeer() {
    // TODO Auto-generated destructor stub

}

void ScampPeer::initialize(int stage)
{
    if (stage == 4)
    {
        // Randomize()
        srand(time(NULL));

        //    timer_getJoinTime = timer_join = timer_keepAlive = timer_chunkScheduling = NULL;
        m_inView.setName("InView");
        m_partialView.setName("PartialView");

        // Timers
        timer_getJoinTime       = new cMessage("MESH_PEER_TIMER_GET_JOIN_TIME");
        timer_join              = new cMessage("MESH_PEER_TIMER_JOIN");
        timer_isolationCheck    = new cMessage("GOSSIP_ISOLATION_CHECK");
        timer_heartbeatSending  = new cMessage("GOSSIP_HEARTBEAT_SENDING");
        timer_sendAppMessage    = new cMessage("GOSSIP_SEND_APP_MESSAGE");

        // Regular messages
        m_heartbeatMsg = new GossipHeartbeatPacket("GOSSIP_HEARTBEAT");

        // -- Bind to the external modules
        bindToGlobalModule();
        bindToParentModule();

        // -- Read parameters
        readParameter();

        // -- Parameter for debugging
        if(hasPar("moduleDebug"))
            param_moduleDebug = par("moduleDebug").boolValue();
        else
            param_moduleDebug = false;

//        bindToPort(m_localPort);

        m_active = false;

        scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

        // -- Gossiped Application messages
        m_messageId = 0L;

        // ------------------------- STATISTICS ------------------------------
//        stat_sizeIV.setName("sizeIV");

        // ------------------------- WATCH ------------------------------------

        WATCH(m_localPort);
        WATCH(m_destPort);
        // WATCH(m_active);

        // -- watches to outside modules
        WATCH(m_apTable);
        WATCH(m_churn);
        WATCH(m_dispatcher);
    }

}

void ScampPeer::finish()
{
    // -- Cancel all scheduled events
    if (timer_getJoinTime)      delete cancelEvent(timer_getJoinTime);
    if (timer_join)             delete cancelEvent(timer_join);

    if (timer_isolationCheck)   delete cancelEvent(timer_isolationCheck);
    if (timer_heartbeatSending) delete cancelEvent(timer_heartbeatSending);
    if (timer_sendAppMessage)   delete cancelEvent(timer_sendAppMessage);

    // -- Delete regular messages:
    delete m_heartbeatMsg;

    // -- To report the final size of InView and PartialView
//    m_gstat->collectSizeIV(m_inView.getViewSize());
//    m_gstat->collectSizePV(m_partialView.getViewSize());

//    m_gstat->recordPartialViewSize(m_partialView.getViewSize());

    // -- for the histogram of the partialView sizes
//    m_gstat->collectPartialViewSize(m_partialView.getViewSize());
//    m_gstat->collectInViewSize(m_inView.getViewSize());

//    EV << "------------------------------ at " << simTime() << ": " << getNodeAddress() << endl;
//    EV << "Partial View (with size: " << m_partialView.getViewSize() << "): " << endl;
//    m_partialView.print();
//    EV << "InView: (with size: " << m_inView.getViewSize() << "): " << endl;
//    m_inView.print();

//    m_gstat->collectAllPVsizes(m_partialView.getViewSize());
}

void ScampPeer::handleTimerMessage(cMessage *msg)
{
//    Enter_Method("handleTimerMessage(msg)");

    if (msg == timer_isolationCheck)
    {
        // -- re-subscribe to the gossip overlay
        // subscribe();
        checkIsolation();

        // -- Schedule for the next isolation check
        scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);

        // -- Statistic for module validation
//        stat_sizeIV.record(m_inView.getViewSize());

        // -- Global statistic collection
//        m_gstat->recordSizeInView(m_inView.getViewSize());
//        m_gstat->recordSizePartialView(m_partialView.getViewSize());

    }
    else if (msg == timer_heartbeatSending)
    {
        //sendHeartbeatMessage();

        scheduleAt(simTime() + param_heartbeatInterval, timer_heartbeatSending);
    }
    else if (msg == timer_sendAppMessage)
    {
        sendGossipAppMessage();

        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }
    else if (msg == timer_getJoinTime)
    {
        #if (__DONET_PEER_DEBUG__)
            EV << "in get_arrival_time" << endl;
        #endif

        double joinTime = m_churn->getArrivalTime();
//        double sessionDuration = m_churn->getDepartTime()
//        double leaveTime = joinTime +
        scheduleAt(simTime() + joinTime, timer_join);
//        scheduleAt()
    }
    else if (msg == timer_join)
    {
        join();
//        m_gstat->recordJoinTime(simTime().dbl());

        // -- Start sending the gossiped application messages
        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }
}


//void ScampPeer::handleExternalMessage(cMessage *pkt) {}

/*
void ScampPeer::processPacket(cPacket *pkt)
{
    Enter_Method("processPacket(pkt)");

    // -- Get the address of the source node of the Packet
    //UDPControlInfo *controlInfo = check_and_cast<UDPControlInfo *>(pkt->getControlInfo());
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//    IPvXAddress sourceAddress = controlInfo->getSrcAddr();
//
//    PeerStreamingPacket *appPkt = check_and_cast<PeerStreamingPacket *>(pkt);

    processGossipPacket(pkt);

    // EV << "going to delete packet ..." << endl;
    // delete pkt;
}
*/

/*
void ScampPeer::startSendingHeartbeat()
{
    Enter_Method("startSendingHeartbeat()");

    std::vector<IPvXAddress> updateList;
    updateList = m_partnerList->getAddressList();

    std::vector<IPvXAddress>::iterator it;
    for (it=updateList.begin(); it != updateList.end(); ++it)
    {
        cPacket *heartbeatPkt = new cPacket("MESH_PEER_KEEP_ALIVE");
        sendToUDP(heartbeatPkt, localPort, *it, destPort);
    }
}

*/

void ScampPeer::join()
{
    Enter_Method("join()");

    // long int nActivePeer = m_aptable->getNumActivePeer();
    // EV << "Number of Active Peers: " << nActivePeer << endl;

    // -- Initial PartialView consists of only itself
    // m_partialView.addContact(getNodeAddress(), m_localPort);

    // -- Gossip
    subscribe();

    // -- Streaming Overlay
    /*
    for (int i=0; i<nActivePeer; ++i)
    {
        IPvXAddress addressRandPeer = m_aptable->getARandPeer();

        // -- Partner establishment
        MeshPartnershipPacket *pkt_join_req = new MeshPartnershipPacket("MESH_PEER_JOIN");
        pkt_join_req->setPacketType(MESH_PARTNERSHIP);
        pkt_join_req->setCommand(CMD_MESH_PARTNERSHIP_REQUEST);

        sendToUDP(pkt_join_req, localPort, addressRandPeer, destPort);

    } // end of for(i)
    */
}


