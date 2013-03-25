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
// Dispatcher.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#define PORT_GOSSIP 0
#define PORT_MESH   1
#define PORT_BUFFER 2
#define PORT_TREE   3

#define OVERLAY_C_DATA 1

#include "UDPAppBase.h"

class Dispatcher : public UDPAppBase {
public:
    Dispatcher();
    virtual ~Dispatcher();
    void handleMessage(cMessage* msg);

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);

    inline int getLocalPort(void)   { return m_localPort; }
    inline int getDestPort(void)    { return m_destPort; }

protected:
//    void forwardToGossipModule(cMessage *msg);
//    void forwardToMeshStreamingModule(cMessage *msg);
//    void forwardToTreeStreamingModule(cMessage *msg);
//    void forwardToBuffer(cMessage *msg);
//    void forwardToUdp(cMessage *msg);

    void processMsgFromOverlay(cMessage *msg);
    void processUdpPacket(cMessage *msg);
    //void sendToOverlay(cPacket *msg);

    // -- Helper functions
private:
    int getGateNumber(int packetGroup);

protected:
    int m_localPort;
    int m_destPort;

};

#endif /* DISPATCHER_H_ */
