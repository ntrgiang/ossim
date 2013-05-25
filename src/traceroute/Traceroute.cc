//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Traceroute.h"
#include "IPControlInfo.h"

#include "StartTraceroute_m.h"
#include "IPDatagram.h"
#include "RouteMsg_m.h"
#include "UDPPacket.h"
#include <cassert>

Define_Module(Traceroute);

void Traceroute::initialize(int stage){
}

void Traceroute::handleMessage(cMessage *msg){
    /*
     * TODO - timeouts
     */

	if(dynamic_cast<StartTraceroute *>(msg)){

		//start new traceroute to given destination address
		StartTraceroute * startmsg = check_and_cast<StartTraceroute *>(msg);		
		//port = startmsg->getPort();
		sendICMPMessage(startmsg->getDest().get4(),1);
		//std::cout <<" start a traceroute to " << startmsg->getDest() << endl;

	}else if(dynamic_cast<ICMPMessage *>(msg)){

		ICMPMessage *icmpmsg = check_and_cast<ICMPMessage *>(msg);
		// get Control information and original message
		IPControlInfo *ctrl = check_and_cast<IPControlInfo*>(icmpmsg->removeControlInfo());

		if(icmpmsg->getType() == ICMP_TIME_EXCEEDED){

			IPDatagram *origMsg = check_and_cast<IPDatagram *>(icmpmsg->decapsulate());
			IPvXAddress dest = IPvXAddress(origMsg->getDestAddress());

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

		} else if(strcmp(icmpmsg->getName(),"Traceroute") == 0 && traces.find(ctrl->getSrcAddr()) != traces.end()){

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
void Traceroute::finishTrace(const IPAddress& dest){

	//std::cout<< " finish trace to dest " << dest << " with route-size " << traces[IPvXAddress(dest)].size() << endl;

	//std::cout<< " we are here " << endl;
    IPvXAddress destX = IPvXAddress(dest);

    if(getParentModule()->getParentModule()->getSubmodule("peerApp",0)
       && traces.find(destX) != traces.end()) {

        RouteMsg * msg = new RouteMsg("RouteMsg");
        msg->setDest(IPvXAddress(dest));
        msg->setRoute(traces[IPvXAddress(dest)]);

        UDPPacket * packet = new UDPPacket("UDPPacket");
        IPControlInfo *controlInfo = new IPControlInfo();
        //FIXME: get own address for message?
        //controlInfo->setDestAddr(nodeTable()->selfDescription().getAddress().get4());
        packet->encapsulate(msg);
        packet->setControlInfo(controlInfo);

        port = getParentModule()->getParentModule()->getSubmodule("peerApp", 0)->getSubmodule("monitoring")->getSubmodule("control")->par("port");
        packet->setDestinationPort(port);
        send(packet,"to_icmp");
	} else {
        std::cout<< " finishTrace to " << dest << " but no peerApp anymore" <<endl;
	}

    if(traces.find(destX) != traces.end()) {
        traces[destX].clear();
        traces.erase(destX);
    }

	/*if(traces.find(destX) != traces.end()) {
		TracerouteTable::Route routeMap = traces.find(destX)->second;
		foreach(TracerouteTable::RouteInfo* info, routeMap) delete info;
	}*/

}

/**
 * Sends an ICMPMessage to Destination , given the TTL for the packet
 *
 * @param	dest	Destination for the ICMPPackets
 * @param	ttl	the TTL for returning the Hop with number TTL for the traceroute
 */

void Traceroute::sendICMPMessage(const IPAddress& dest, int ttl){

    IPvXAddress destX = IPvXAddress(dest);
    traces[destX];

    IPControlInfo *controlInfo = new IPControlInfo();
    controlInfo->setDestAddr(dest);
    controlInfo->setTimeToLive(ttl);
    ICMPMessage *request = new ICMPMessage("Traceroute");
    request->setControlInfo(controlInfo);
    send(request, "to_icmp");
}


TracerouteTable::RouteMap Traceroute::getTraces() {
    Enter_Method_Silent();
    return traces;
}

/**
 * Function is called to restart traceroute from the outside
 */
void Traceroute::restartTraceroute(TracerouteTable::RouteMap traceMap) {
    Enter_Method_Silent();
    TracerouteTable::RouteMap::iterator it;
    for(it = traceMap.begin(); it != traceMap.end(); it++) {
        it->second.clear();
		std::cout << " resend ICMP message to " << it->first << " after movement " << endl;
        sendICMPMessage(it->first.get4(),1);
    }
}
