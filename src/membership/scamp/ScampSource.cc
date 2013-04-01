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
// ScampSource.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include <algorithm>
#include <cstring>
#include <cstdio>
#include "ScampSource.h"

Define_Module(ScampSource)

ScampSource::ScampSource()
{
}

ScampSource::~ScampSource()
{
    // -- Cancel and delete timer messages
    if (timer_isolationCheck)   delete cancelEvent(timer_isolationCheck);
    if (timer_sendAppMessage)   delete cancelEvent(timer_sendAppMessage);
    if (timer_reportPvSize) cancelAndDelete(timer_reportPvSize);
}

void ScampSource::initialize(int stage)
{
    if (stage == 4)
    {
        // -- Set names for the InView & PartialView lists
        m_inView.setName("InView");
        m_partialView.setName("PartialView");

        // -- Timers
        timer_isolationCheck    = new cMessage("GOSSIP_ISOLATION_CHECK");
        timer_sendAppMessage    = new cMessage("GOSSIP_SEND_APP_MESSAGE");
        timer_reportPvSize      = new cMessage("TIMER_REPORT_PVSIZE");

        //timer_reportPvSize      = new cMessage("GOSSIP_REPORT_PV_SIZE");

        // Regular messages
//        m_heartbeatMsg = new GossipHeartbeatPacket("GOSSIP_HEARTBEAT");

        // -- Binding to external modules
        bindToParentModule();
        bindToGlobalModule();
        bindToStatisticModule();

        // -- Reading values for parameters
        readParameter();
        findNodeAddress();

        param_time_reportPvSize = par("time_reportPvSize");
        scheduleAt(simTime() + param_time_reportPvSize, timer_reportPvSize);

        m_simDuration = getSimDuration();

        // -- Add the IP address of the video source to the Active Peer Table
        // -- (so that other peers could have at least one address in the list to contact to)
        m_apTable->addAddress(getNodeAddress());

        // -- Declare itself that it is active now
        m_active = true;

        // -- State
        m_state = SCAMP_STATE_JOINED;

        // -- Set up timer to check isolation
        scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);
//        scheduleAt(m_simDuration - 0.01, timer_reportPvSize);
        //scheduleAt(SimTime() + param_appMessageInterval, timer_sendAppMessage);

        // -- Signals
        sig_pvSize = registerSignal("Signal_pvSize");

        // m_apTable->printActivePeerTable();

        // ---------------------------------------------------------------------
        // --- WATCH
        // ---------------------------------------------------------------------
        WATCH(m_localPort);
        WATCH(m_apTable);
        WATCH(m_simDuration);
        WATCH(m_c);
    }
}

void ScampSource::finish()
{
    // -- To report the final size of InView and PartialView
    // m_gstat->reportPvSize(m_partialView.getViewSize());
}

/**
 * ----------------
 * Helper functions
 * ----------------
 */

void ScampSource::handleTimerMessage(cMessage *msg)
{
//    Enter_Method("handleTimerMessage(msg)");
    if (msg == timer_isolationCheck)
    {
        // -- re-subscribe to the gossip overlay
        // subscribe();
        //checkIsolation();

        // -- Schedule for the next isolation check
        //scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);
    }
    else if (msg == timer_sendAppMessage)
    {
//        sendGossipAppMessage();
//        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }
    else if (msg == timer_reportPvSize)
    {
       m_gstat->reportPvSize(m_partialView.getViewSize());
       emit(sig_pvSize, m_partialView.getViewSize());
    }
//    else if (msg == timer_reportPvSize)
//    {
//        EV << "report pv size: " << m_partialView.getViewSize() << endl;
//        m_gstat->reportPvSize(m_partialView.getViewSize());
//    }

}

/*
 * This function DELETEs the message
 */
/*
void ScampSource::processPacket(cPacket *pkt)
{
    Enter_Method("processPacket(pkt)");

    // -- Get the address of the source node of the Packet
    // UDPControlInfo *controlInfo = check_and_cast<UDPControlInfo *>(pkt->getControlInfo());
    // IPvXAddress sourceAddress = controlInfo->getSrcAddr();
    // int senderPort = controlInfo->getSrcPort();

    PeerStreamingPacket *appPkt = dynamic_cast<PeerStreamingPacket *>(pkt);
    assert(appPkt != NULL);

    EV << "Packet of Overlay: " << appPkt->getPacketGroup() << endl;

    switch (appPkt->getPacketGroup())
    {
        case PACKET_GROUP_GOSSIP_OVERLAY:
        {
            // GossipMembershipPacket *gossipPkt = dynamic_cast<GossipMembershipPacket *>(appPkt);
            // processGossipPacket(gossipPkt);
            processGossipPacket(pkt);
            break;
        }
        case PACKET_GROUP_MESH_OVERLAY:
        {
            break;
        }
        case PACKET_GROUP_TREE_OVERLAY:
        {
            break;
        }
        default:
        {
            EV << "Something wrong with the message, strange type!" << endl;
            break;
        }
    }; // switch

    delete pkt;
}
*/

//void GossipVideoSource::processGossipPacket(GossipMembershipPacket *pkt)

// -----------------------------------------------------------------------------
//void GossipVideoSource::handleNewSubscription(GossipSubscriptionPacket *pkt)
//{
//
//}
