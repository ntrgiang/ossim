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
// ICMPMobility.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "ICMPMobility.h"
#include "IPControlInfo.h"
#include "StartTraceroute_m.h"
#include "RouteMsg_m.h"
#include "IPvXAddress.h"
#include "IPDatagram.h"

Define_Module(ICMPMobility);

void ICMPMobility::handleMessage(cMessage *msg)
{
    cGate *arrivalGate = msg->getArrivalGate();

    // process arriving ICMP message
    if (!strcmp(arrivalGate->getName(), "localIn"))
    {
    	if(dynamic_cast<StartTraceroute *>(msg)){
    		send(msg, "to_trcrt");
    	}else{
    		processICMPMessage(check_and_cast<ICMPMessage *>(msg));
    	}
        return;
    }

    // request from application
    if (!strcmp(arrivalGate->getName(), "pingIn"))
    {
        sendEchoRequest(PK(msg));
        return;
    }

    //Message from Traceroute
    if (!strcmp(arrivalGate->getName(), "from_trcrt"))
    {
       if(dynamic_cast<cPacket *>(msg))
       {
          if (dynamic_cast<RouteMsg *>(dynamic_cast<cPacket *>(msg)->getEncapsulatedPacket()))
          {
             send(msg,"sendOut");
             return;
          }else
          {
             sendEchoRequest(PK(msg));
             return;
          }
       }
    }
}

void ICMPMobility::processICMPMessage(ICMPMessage *icmpmsg)
{
	switch (icmpmsg->getType())
    {
        case ICMP_DESTINATION_UNREACHABLE:
            errorOut(icmpmsg);
            break;
        case ICMP_REDIRECT:
            errorOut(icmpmsg);
            break;
        case ICMP_PARAMETER_PROBLEM:
            errorOut(icmpmsg);
            break;
        case ICMP_TIME_EXCEEDED:
		{
			// deliver traceroute messages to traceroute module else send to error handling
			if( strcmp(icmpmsg->getEncapsulatedPacket()->getName(),"Traceroute")==0) {
              send(icmpmsg, "to_trcrt");
            }else{
              errorOut(icmpmsg);
            }
		}
            break;
        case ICMP_ECHO_REQUEST:
            processEchoRequest(icmpmsg);
            break;
        case ICMP_ECHO_REPLY:
            processEchoReply(icmpmsg);
            break;
        case ICMP_TIMESTAMP_REQUEST:
            processEchoRequest(icmpmsg);
            break;
        case ICMP_TIMESTAMP_REPLY:
            processEchoReply(icmpmsg);
            break;
        default:
            opp_error("Unknown ICMP type %d", icmpmsg->getType());
    }
}

void ICMPMobility::processEchoReply(ICMPMessage *reply)
{
    IPControlInfo *ctrl = check_and_cast<IPControlInfo*>(reply->removeControlInfo());
    cPacket *payload = reply->decapsulate();
    payload->setControlInfo(ctrl);
    delete reply;
    if(strcmp(payload->getName(),"Traceroute")==0){
    	send(payload, "to_trcrt");
    }else{
    	send(payload, "pingOut");
    }
}
