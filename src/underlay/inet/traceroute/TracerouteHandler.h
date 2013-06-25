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
// TracerouteHandler.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#ifndef TRACEROUTEHANDLER_H_
#define TRACEROUTEHANDLER_H_
/*
 * module for doing traceroutes and saving the routes to routeTable
 */

#include <omnetpp.h>
#include "PPDatatypes.h"
#include "TracerouteTable.h"
#include "Traceroute.h"
#include "RouteMsg_m.h"

class Traceroute;

class TracerouteHandler: public cSimpleModule
{
protected:
   bool active;
   cMessage* test_timer;
   IPvXAddress reference;

   TracerouteTable* m_trTable;
   Traceroute* m_traceroute;
   cMessage* timer_triggerTracerouteRefPoint;

public:
   TracerouteHandler();
   virtual ~TracerouteHandler();

   PPIPvXAddressVector tracerouteStarted;

   void initialize(int stage);
   void finish();
   void handleMessage(cMessage* msg);

   void handleTimerMessage(cMessage* msg);
   void processPacket(cPacket* pkt);

   void doHandleTraceroute(IPvXAddress addr);

   // FIXME-Giang -- ugly/duplicated conversion of msg
   void processRouteMessage(RouteMsg*);

   void triggerTracerouteRefPoint();

};

#endif // TRACEROUTEHANDLER_H_
