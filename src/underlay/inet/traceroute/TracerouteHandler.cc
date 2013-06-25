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
// TracerouteHandler.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include <omnetpp.h>
#include "TracerouteHandler.h"

#include "StartTraceroute_m.h"
#include "RouteMsg_m.h"
#include "IPAddressResolver.h"
#include "IPvXAddress.h"

Define_Module(TracerouteHandler);

TracerouteHandler::TracerouteHandler() {
}

TracerouteHandler::~TracerouteHandler()
{
   if (timer_triggerTracerouteRefPoint != NULL)
      cancelAndDelete(timer_triggerTracerouteRefPoint);
}

void TracerouteHandler::finish() {

}

void TracerouteHandler::initialize(int stage)
{
   if (stage == 0)
   {
      active = par("active");

      // -- Binding to the TracerouteTable module
      cModule* temp = getParentModule()->getModuleByRelativePath("trTable");
      m_trTable = check_and_cast<TracerouteTable*>(temp);
      EV << "Binding to the TracerouteTable was successful!" << endl;

      m_trTable->setActive(active);
      reference = IPAddressResolver().resolve(par("referencePoint"));
      m_trTable->setReferencePeer(reference);

      timer_triggerTracerouteRefPoint = new cMessage("ReferencePoint");

   }

   cModule* temp = getParentModule()->getModuleByRelativePath("traceroute");
   m_traceroute = check_and_cast<Traceroute*>(temp);

   WATCH(active);
   WATCH(reference);

   WATCH(m_trTable);
   WATCH(m_traceroute);
}

void TracerouteHandler::handleMessage(cMessage* msg)
{
   if (msg->isSelfMessage())
   {
      handleTimerMessage(msg);
   }
   else
   {
      processPacket(PK(msg));
   }
}

void TracerouteHandler::handleTimerMessage(cMessage *msg)
{
   if(!active)
   {
      return;
   }

   if (msg == timer_triggerTracerouteRefPoint)
   {
      doHandleTraceroute(reference);
   }
}

void TracerouteHandler::processPacket(cPacket* msg)
{
   if(!active)
   {
      delete msg;
      msg = NULL;

      return;
   }

   if (dynamic_cast<StartTraceroute *>(msg))
   {
      doHandleTraceroute(dynamic_cast<StartTraceroute *>(msg)->getDest());

      //save route and delete Address from Started-Traceroute-Vector
   }
   else if (dynamic_cast<RouteMsg *>(msg))
   {
      processRouteMessage(dynamic_cast<RouteMsg *>(msg));
//      RouteMsg *msg2 = dynamic_cast<RouteMsg *>(msg);

//      // FIXME get pointer to traceroutetable!
//      TracerouteTable *routetable = NULL;
//      //event("numberOfRoutes");
//      routetable->setRoute(msg2->getRoute(),msg2->getDest());
//      if (tracerouteStarted.size() > 0) {
//         for (int i = 0 ; i < (int)tracerouteStarted.size(); i++) {
//            if (tracerouteStarted[i] == msg2->getDest()){
//               tracerouteStarted.erase(tracerouteStarted.begin() + i);
//               break;
//            }
//         }
//      }
   }

   delete msg;
   msg = NULL;
}

void TracerouteHandler::processRouteMessage(RouteMsg* msg)
{
   Enter_Method("processRouteMessage");

   RouteMsg *msg2 = dynamic_cast<RouteMsg *>(msg);
   m_trTable->setRoute(msg2->getRoute(),msg2->getDest());

   if (tracerouteStarted.size() > 0)
   {
      for (int i = 0 ; i < (int)tracerouteStarted.size(); i++)
      {
         if (tracerouteStarted[i] == msg2->getDest())
         {
            tracerouteStarted.erase(tracerouteStarted.begin() + i);
            break;
         }
      }
   }
   delete msg;
   msg = NULL;
}
/**
 * Starts Traceroute if not already started or was discovered before
 *
 * @param addr	Destination for Traceroute
 */
void TracerouteHandler::doHandleTraceroute(IPvXAddress addr)
{
   Enter_Method_Silent();

   if(!active) return;
   //if traceroute already started -> return
   if (tracerouteStarted.size() > 0){
      for (int i = 0 ; i < (int)tracerouteStarted.size(); i++) {
         if (tracerouteStarted[i] == addr) {
            return;
         }
      }
   }

   //if route already discovered return
   if(m_trTable->getRouteSize2(addr) > 0) {
      return;
   }

   // -- Trigger StartTraceroute
   //
   m_traceroute->triggerSendTracerouteMessage(addr);

}

void TracerouteHandler::triggerTracerouteRefPoint()
{
   scheduleAt(simTime() + 0.0, timer_triggerTracerouteRefPoint);
}

