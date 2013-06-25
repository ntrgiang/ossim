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
// Traceroute.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#ifndef __INET_TRACEROUTE_H_
#define __INET_TRACEROUTE_H_

#include <omnetpp.h>
#include "TracerouteTable.h"
#include "TracerouteHandler.h"

class TracerouteHandler;

/**
 * TODO - Generated class
 */
class Traceroute : public cSimpleModule
{
public:
   virtual ~Traceroute();

protected:

   TracerouteTable::RouteMap traces;

   virtual void initialize(int stage);
   virtual void handleMessage(cMessage *msg);
   virtual void processPacket(cPacket *pkt);
   virtual void handleTimerMessage(cMessage *msg);

   /*
     * sends ICMPEchoRequest for traceroute
     */
   virtual void sendICMPMessage(const IPAddress& dest, int ttl);

   /*
     * Send route to hold it in RouteTable and delete it from trace map
     */
   virtual void finishTrace(const IPAddress& dest);

   int port;
   IPvXAddress m_curDestAddr; // current destination address

   cMessage* test_timer;

   cMessage* timer_triggerSend;

   TracerouteHandler* m_trHandler;
   TracerouteTable* m_trTable;

public:
   TracerouteTable::RouteMap getTraces();
   void restartTraceroute(TracerouteTable::RouteMap traceMap);

   // -- to be triggered from the PeerApp module
   void triggerSendTracerouteMessage(IPvXAddress addr);
   void sendTracerouteMessage(void);
};

#endif
