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
// IPtr.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

/*
 *  Created on: Aug 4, 2009
 *      Author: mtrapp
 */

#include "IPtr.h"
#include "StartTraceroute_m.h"
#include "RouteMsg_m.h"
#include <assert.h>

Define_Module(IPtr);

#if _DEBUG
# define DEBUGOUT(x) std::cout << " (IP@" << simTime() << "): " << x << endl
#else
# define DEBUGOUT(x)
#endif

void IPtr::handleMessageFromHL(cPacket *msg)
{
   // if no interface exists, do not send datagram
   if (ift->getNumInterfaces() == 0)
   {
      EV << "No interfaces exist, dropping packet\n";
      delete msg;
      return;
   }

   //send start traceroute message to ICMP Module
   if(dynamic_cast<StartTraceroute *>(msg->getEncapsulatedPacket()))
   {
      send(msg->decapsulate(),"transportOut",2);

   }
   else if(dynamic_cast<RouteMsg *>(msg->getEncapsulatedPacket()))
   {
      send(msg,"transportOut",1);
   }
   else
   {
      // encapsulate and send
      InterfaceEntry *destIE; // will be filled in by encapsulate()
      IPDatagram *datagram = IP::encapsulate(msg, destIE);
      assert(datagram);

      if (datagram!=NULL)
      {
         // route packet
         if (!datagram->getDestAddress().isMulticast())
            routePacket(datagram, destIE, true);
         else
            routeMulticastPacket(datagram, destIE, NULL);
      }
   }

}

IPDatagram *IPtr::encapsulate(cPacket *transportPacket, InterfaceEntry *&destIE, IPControlInfo *controlInfo)
{
   IPDatagram *datagram = createIPDatagram(transportPacket->getName());
   datagram->setByteLength(IP_HEADER_BYTES);
   datagram->encapsulate(transportPacket);

   // set source and destination address
   IPAddress dest = controlInfo->getDestAddr();
   datagram->setDestAddress(dest);

   // IP_MULTICAST_IF option, but allow interface selection for unicast packets as well
   destIE = ift->getInterfaceById(controlInfo->getInterfaceId());

   IPAddress src = controlInfo->getSrcAddr();

   // when source address was given, use it; otherwise it'll get the address
   // of the outgoing interface after routing
   if (!src.isUnspecified())
   {
      // if interface parameter does not match existing interface, do not send datagram
      if (rt->getInterfaceByAddress(src)==NULL)
      {
         // opp_error("Wrong source address %s in (%s)%s: no interface with such address",
         //         src.str().c_str(), transportPacket->getClassName(), transportPacket->getFullName());
         //delete datagram;
         //delete transportPacket;
      }

      datagram->setSrcAddress(src);
   }

   // set other fields
   datagram->setDiffServCodePoint(controlInfo->getDiffServCodePoint());

   datagram->setIdentification(curFragmentId++);
   datagram->setMoreFragments(false);
   datagram->setDontFragment (controlInfo->getDontFragment());
   datagram->setFragmentOffset(0);

   short ttl;
   if (controlInfo->getTimeToLive() > 0)
      ttl = controlInfo->getTimeToLive();
   else if (datagram->getDestAddress().isLinkLocalMulticast())
      ttl = 1;
   else if (datagram->getDestAddress().isMulticast())
      ttl = defaultMCTimeToLive;
   else
      ttl = defaultTimeToLive;

   datagram->setTimeToLive(ttl);
   datagram->setTransportProtocol(controlInfo->getProtocol());

   // setting IP options is currently not supported

   return datagram;
}
