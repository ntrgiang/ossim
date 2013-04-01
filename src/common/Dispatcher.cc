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
// Dispatcher.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//

#include "Dispatcher.h"
#include "PeerStreamingPacket_m.h"
#include "DpControlInfo_m.h"
#include "UDPControlInfo_m.h"

Define_Module(Dispatcher)

Dispatcher::Dispatcher() {
    // TODO Auto-generated constructor stub
}

Dispatcher::~Dispatcher() {
    // TODO Auto-generated destructor stub
}

void Dispatcher::initialize(int stage)
{
    if (stage != 3) return;

    m_localPort = par("localPort");
    m_destPort = par("destPort");

    bindToPort(m_localPort);

    WATCH(m_localPort);
    WATCH(m_destPort);
}

void Dispatcher::handleMessage(cMessage* msg)
{

    if (msg->isSelfMessage() == true)
    {
        EV << "Dispatcher uses no timers! Some thing is wrong!" << endl;
        return;
    }

    // received from underlaying UDP layer
    if (msg->arrivedOn("udpIn"))
    {
        processUdpPacket(msg);
    }
    else // received from upper Gossip/Streaming Modules
    {
        processMsgFromOverlay(msg);
    }
}

/*void Dispatcher::forwardToGossipModule(cMessage *msg)
{
//    UDPControlInfo *controlInfo = check_and_cast<UDPControlInfo *>(msg->removeControlInfo());
//
//    PeerStreamingPacket *pkt = dynamic_cast<PeerStreamingPacket *>(msg);
//
//    sendToUDP(pkt, controlInfo->getSrcPort(), controlInfo->getDestAddr(), controlInfo->getDestPort());

}

void Dispatcher::forwardToMeshStreamingModule(cMessage *msg)
{

}

void Dispatcher::forwardToTreeStreamingModule(cMessage *msg)
{

}

void Dispatcher::forwardToBuffer(cMessage *msg)
{

}

*
 * Dispatch ControlInfo & payload (original message)
 * Change the ControlInfo into UDP ControlInfo, then attach back to the payload message
 * Then, send it to UDP, using sendToUDP()

void Dispatcher::forwardToUdp(cMessage *msg)
{
//    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(msg->removeControlInfo());
//
//    PeerStreamingPacket *pkt = dynamic_cast<PeerStreamingPacket *>(msg);
//
//    sendToUDP(pkt, controlInfo->getSrcPort(), controlInfo->getDestAddr(), controlInfo->getDestPort());
}*/


// ------------- new interface -------------
void Dispatcher::processMsgFromOverlay(cMessage *overlayData)
{
    EV << "Dispatcher received a packet from overlay" << endl;

    DpControlInfo *dpCtrl = check_and_cast<DpControlInfo *>(overlayData->removeControlInfo());

    PeerStreamingPacket *pkt = check_and_cast<PeerStreamingPacket *>(overlayData);

    EV << "-- Payload size: " << pkt->getByteLength() << "(bytes)" << endl;

    sendToUDP(pkt, dpCtrl->getSrcPort(),
            dpCtrl->getDestAddr(),
            dpCtrl->getDestPort());

    delete dpCtrl;
}

void Dispatcher::processUdpPacket(cMessage *udpMsg)
{
    UDPControlInfo *udpCtrl = check_and_cast<UDPControlInfo *>(udpMsg->removeControlInfo());

    PeerStreamingPacket *strmPkt = check_and_cast<PeerStreamingPacket *>(udpMsg);

    DpControlInfo *dpCtrl = new DpControlInfo();
        dpCtrl->setSrcAddr(udpCtrl->getSrcAddr());
        dpCtrl->setSrcPort(udpCtrl->getSrcPort());
        dpCtrl->setDestAddr(udpCtrl->getDestAddr());
        dpCtrl->setDestPort(udpCtrl->getDestPort());
    udpMsg->setControlInfo(dpCtrl);

    int packetGroup = strmPkt->getPacketGroup();

    // -- Get the size of the overlayOut array
    // int nOverlayOut = gateSize("overlayOut");

    // -- "Direct" mapping from packetGroup into gateNumber
    // (just to avoid switch which produces more computational overhead
    int gateNumber = getGateNumber(packetGroup);

    send(udpMsg, "overlayOut", gateNumber);

    delete udpCtrl;
}

int Dispatcher::getGateNumber(int packetGroup)
{
    /*
        switch(packetGroup)
        {
        case PACKET_GROUP_GOSSIP_OVERLAY:
        {
            gateNumber = 0;
            break;
        }
        case PACKET_GROUP_PAYLOAD:
        {
            gateNumber = 1;
            break;
        }
        case PACKET_GROUP_MESH_OVERLAY:
        {
            gateNumber = 2;
            break;
        }
        case PACKET_GROUP_TREE_OVERLAY:
        {
            gateNumber = 3;
            break;
        }
        default:
        {
            EV << "Wrong packetGroup! " << endl;
            break;
        }
        } // end of switch
    */
    return packetGroup;
}
