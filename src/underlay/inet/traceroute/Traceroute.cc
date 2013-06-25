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
// Traceroute.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "Traceroute.h"
#include "IPControlInfo.h"

#include "StartTraceroute_m.h"
#include "IPDatagram.h"
#include "RouteMsg_m.h"
#include "UDPPacket.h"
#include <cassert>

Define_Module(Traceroute);

Traceroute::~Traceroute()
{
   if(timer_triggerSend) cancelAndDelete(timer_triggerSend);
}

void Traceroute::initialize(int stage)
{
   if (stage == 0)
   {
      timer_triggerSend = new cMessage("TRACEROUTE_TIMER_SEND");

      cModule* temp = getParentModule()->getModuleByRelativePath("trHandler");
      m_trHandler = check_and_cast<TracerouteHandler*>(temp);
      EV << "Binding to tracerouteHandler was successful!" << endl;

      temp = getParentModule()->getModuleByRelativePath("trTable");
      m_trTable = check_and_cast<TracerouteTable*>(temp);
      EV << "Binding to tracerouteTable was successful!" << endl;

   }
}

void Traceroute::handleMessage(cMessage *msg)
{
   Enter_Method("handleMessage");

   if (msg->isSelfMessage())
   {
      handleTimerMessage(msg);
   }
   else
   {
      processPacket(PK(msg));
   }
}

void Traceroute::handleTimerMessage(cMessage* msg)
{
   Enter_Method("handleTimerMessage");

   if (msg == timer_triggerSend)
   {
      sendTracerouteMessage();
   }
}

void Traceroute::processPacket(cPacket *msg)
{
   Enter_Method("processPacket");

   // TODO - timeouts??
   if(dynamic_cast<StartTraceroute *>(msg))
   {
      //start new traceroute to given destination address
      StartTraceroute * startmsg = check_and_cast<StartTraceroute *>(msg);
      //port = startmsg->getPort();
      sendICMPMessage(startmsg->getDest().get4(),1);
      //std::cout <<" start a traceroute to " << startmsg->getDest() << endl;
   }
   else if(dynamic_cast<ICMPMessage *>(msg))
   {
      ICMPMessage *icmpmsg = check_and_cast<ICMPMessage *>(msg);
      // get Control information and original message
      IPControlInfo *ctrl = check_and_cast<IPControlInfo*>(icmpmsg->removeControlInfo());

      if(icmpmsg->getType() == ICMP_TIME_EXCEEDED)
      {
         IPDatagram *origMsg = check_and_cast<IPDatagram *>(icmpmsg->decapsulate());
         IPvXAddress dest = IPvXAddress(origMsg->getDestAddress());

         if(traces.find(dest) == traces.end()) {
             std::cout<<" problems with trace to " << dest << endl;
         }
         assert(traces.find(dest) != traces.end());

         // which hop is it?
         int hopnumber = traces[dest].size() + 2;

         // insert next hop at the end of the map
         TracerouteTable::RouteInfo * entry = new TracerouteTable::RouteInfo();
         entry->hop = hopnumber-1;
         entry->addr = IPvXAddress(ctrl->getSrcAddr());
         traces[dest].push_back(entry);

         //std::cout<< "(TR): icmp answer arrived from " << ctrl->getSrcAddr()
         //<< " to dest " << dest << ", so incrementing hop count to "<< hopnumber << endl;

         // send message for next hop
         sendICMPMessage(origMsg->getDestAddress(),hopnumber);
         delete ctrl;
         delete origMsg;

      }
      else if(strcmp(icmpmsg->getName(),"Traceroute") == 0 && traces.find(ctrl->getSrcAddr()) != traces.end())
      {
         // insert next hop at the end of the map
         TracerouteTable::RouteInfo * entry = new TracerouteTable::RouteInfo();
         entry->hop = traces[ctrl->getSrcAddr()].size() + 1;
         entry->addr = IPvXAddress(ctrl->getSrcAddr());
         traces[ctrl->getSrcAddr()].push_back(entry);

         finishTrace(ctrl->getSrcAddr());

      }
      //std::cout << icmpmsg->getType() << endl;
   }

   delete msg;
   msg = NULL;
}

/**
 * Finishes the Traceroute and sends a Message containing the route to the tracerouteHandler
 *
 * @ param	dest	IPAddress of the Traceroute Destination
 */
void Traceroute::finishTrace(const IPAddress& dest)
{
   Enter_Method("finishTrace");

   EV << " finish trace to dest " << dest << " with route-size " << traces[IPvXAddress(dest)].size() << endl;

   EV << " we are here " << endl;
   IPvXAddress destX = IPvXAddress(dest);

   if(traces.find(destX) != traces.end()) {
      RouteMsg * msg = new RouteMsg("RouteMsg");
      msg->setDest(IPvXAddress(dest));
      msg->setRoute(traces[IPvXAddress(dest)]);
      m_trHandler->processRouteMessage(msg);
      traces[destX].clear();
      traces.erase(destX);
   }
}

/**
 * Sends an ICMPMessage to Destination , given the TTL for the packet
 *
 * @param	dest	Destination for the ICMPPackets
 * @param	ttl	the TTL for returning the Hop with number TTL for the traceroute
 */

void Traceroute::sendICMPMessage(const IPAddress& dest, int ttl)
{
   Enter_Method("sendICMPMessage");

   IPvXAddress destX = IPvXAddress(dest);
   traces[destX];

   IPControlInfo *controlInfo = new IPControlInfo();
   controlInfo->setDestAddr(dest);
   controlInfo->setTimeToLive(ttl);
   ICMPMessage *request = new ICMPMessage("Traceroute");
   request->setControlInfo(controlInfo);
   send(request, "to_icmp");
}


TracerouteTable::RouteMap Traceroute::getTraces()
{
   Enter_Method_Silent();

   return traces;
}

/**
 * Function is called to restart traceroute from the outside
 */
void Traceroute::restartTraceroute(TracerouteTable::RouteMap traceMap)
{
   Enter_Method_Silent();

   TracerouteTable::RouteMap::iterator it;
   for(it = traceMap.begin(); it != traceMap.end(); it++)
   {
      it->second.clear();
      std::cout << " resend ICMP message to " << it->first << " after movement " << endl;
      sendICMPMessage(it->first.get4(),1);
   }
}

void Traceroute::triggerSendTracerouteMessage(IPvXAddress addr)
{
   Enter_Method("triggerSendTracerouteMessage");

   if (m_trTable->inTable(addr) || traces.find(addr) != traces.end()) return;

   //std::cout << this->getFullPath() <<" send traceroute to " << addr << endl;

   m_curDestAddr = addr;
   sendICMPMessage(m_curDestAddr.get4(),1);

   //scheduleAt(simTime() + 0.0, timer_triggerSend);
}

void Traceroute::sendTracerouteMessage()
{
   Enter_Method("triggerSendTracerouteMessage");

   sendICMPMessage(m_curDestAddr.get4(),1);
}
