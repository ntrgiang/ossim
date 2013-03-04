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
    // -- Cancel all scheduled events
    if (timer_getJoinTime)      delete cancelEvent(timer_getJoinTime);
    if (timer_join)             delete cancelEvent(timer_join);

    if (timer_isolationCheck)   delete cancelEvent(timer_isolationCheck);
    if (timer_heartbeatSending) delete cancelEvent(timer_heartbeatSending);
    if (timer_sendAppMessage)   delete cancelEvent(timer_sendAppMessage);

    // -- Delete regular messages:
    delete m_heartbeatMsg;
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
        findNodeAddress();

        m_active = false;

        // -- State
        m_state = SCAMP_STATE_IDLE;

        scheduleAt(simTime() + par("startTime").doubleValue(), timer_getJoinTime);

        // -- Gossiped Application messages
        m_messageId = 0L;

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
    // -- To report the final size of InView and PartialView
//     m_gstat->reportPvSize(m_partialView.getViewSize());
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
    }
    else if (msg == timer_heartbeatSending)
    {
        //sendHeartbeatMessage();

        scheduleAt(simTime() + param_heartbeatInterval, timer_heartbeatSending);
    }
    else if (msg == timer_sendAppMessage)
    {
        //sendGossipAppMessage();

        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }
    else if (msg == timer_getJoinTime)
    {
        double arrivalTime = m_churn->getArrivalTime();
        EV << "Expected arrival time: " << arrivalTime << endl;
        scheduleAt(simTime() + arrivalTime, timer_join);
    }
    else if (msg == timer_join)
    {
        join();

        // -- Start sending the gossiped application messages
//        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }
}

void ScampPeer::join()
{
    Enter_Method("join()");

    // long int nActivePeer = m_aptable->getNumActivePeer();
    // EV << "Number of Active Peers: " << nActivePeer << endl;

    EV << "Joining ..." << endl;

    // -- Gossip
    subscribe();

    m_state = SCAMP_STATE_JOINING;

}

