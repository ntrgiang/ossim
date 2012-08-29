//#include "VideoSource.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include "UDPControlInfo_m.h"
//#include "UDPPacket.h"
//#include <IPAddressResolver.h>
//#include <csimulation.h>

#include "ScampSource.h"

#include "PeerStreamingPacket_m.h"
#include "MeshPeerStreamingPacket_m.h"
#include "GossipMembershipPacket_m.h"

Define_Module(ScampSource);

ScampSource::ScampSource()
{
}

ScampSource::~ScampSource()
{
}

void ScampSource::initialize(int stage)
//void GossipVideoSource::initialize()
{
    if (stage == 4)
    {
        // -- Set names for the InView & PartialView lists
        m_inView.setName("InView");
        m_partialView.setName("PartialView");

        // -- Timers
        timer_isolationCheck    = new cMessage("GOSSIP_ISOLATION_CHECK");
        timer_sendAppMessage    = new cMessage("GOSSIP_SEND_APP_MESSAGE");

        // Regular messages
//        m_heartbeatMsg = new GossipHeartbeatPacket("GOSSIP_HEARTBEAT");

        // -- Binding to external modules
        bindToGlobalModule();
        bindToParentModule();

        // -- Reading values for parameters
        readParameter();

//        bindToPort(m_localPort);

        // -- Add the IP address of the video source to the Active Peer Table
        // -- (so that other peers could have at least one address in the list to contact to)
        m_apTable->addPeerAddress(getNodeAddress());

        // -- Declare itself that it is active now
        m_active = true;

        // -- Set up timer to check isolation
        scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);
        scheduleAt(SimTime() + param_appMessageInterval, timer_sendAppMessage);

        // m_apTable->printActivePeerTable();

        WATCH(m_localPort);
        WATCH(m_apTable);
    }
}

void ScampSource::finish()
{
    // -- Cancel and delete timer messages
    if (timer_isolationCheck)   delete cancelEvent(timer_isolationCheck);
    if (timer_sendAppMessage)   delete cancelEvent(timer_sendAppMessage);

    // -- To report the final size of InView and PartialView
//    m_gstat->collectSizeIV(m_inView.getViewSize());
//    m_gstat->collectSizePV(m_partialView.getViewSize());

//    m_gstat->recordPartialViewSize(m_partialView.getViewSize());
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
        checkIsolation();

        // -- Schedule for the next isolation check
        scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);
    }
    else if (msg == timer_sendAppMessage)
    {
        sendGossipAppMessage();
        scheduleAt(simTime() + param_appMessageInterval, timer_sendAppMessage);
    }

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
