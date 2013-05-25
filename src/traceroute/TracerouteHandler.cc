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

#include <omnetpp.h>
#include "TracerouteHandler.h"
//#include "TracerouteTable.h"

#include "StartTraceroute_m.h"
#include "RouteMsg_m.h"
#include "UDPControlInfo_m.h"
#include "IPAddressResolver.h"
#include "IPvXAddress.h"


Define_Module(TracerouteHandler);
void TracerouteHandler::finish() {

}

void TracerouteHandler::initialize(int stage){

	if (stage == 0) {
		active = par("active");

        //FIXME Pointer to TracerouteTable required
        TracerouteTable *routetable = NULL;
		routetable->setActive(active);
		reference = IPAddressResolver().resolve(par("referencePoint"));
		routetable->setReferencePeer(reference);
		if(active)
			scheduleAt(simTime() + 2, new cMessage("ReferencePoint"));
	}
}

void TracerouteHandler::handleMessage(cMessage* msg){

	if(!active){
		//std::cout<< "(TH): message rcvd and deleted since not active" <<endl;
		delete msg;
		msg = NULL;
		return;
	}

	if(msg->isSelfMessage() && strcmp(msg->getName(), "ReferencePoint") == 0) {

			doHandleTraceroute(reference);

	//received StartTraceroute message from Nodecontrol sent due to connect message
	} else if (dynamic_cast<StartTraceroute *>(msg)) {
		
		doHandleTraceroute(dynamic_cast<StartTraceroute *>(msg)->getDest());

	//save route and delete Address from Started-Traceroute-Vector
	} else if (dynamic_cast<RouteMsg *>(msg)) {

		RouteMsg *msg2 = dynamic_cast<RouteMsg *>(msg);

        // FIXME get pointer to traceroutetable!
        TracerouteTable *routetable = NULL;
        //event("numberOfRoutes");
		routetable->setRoute(msg2->getRoute(),msg2->getDest());
		if (tracerouteStarted.size() > 0) {
            for (int i = 0 ; i < (int)tracerouteStarted.size(); i++) {
				if (tracerouteStarted[i] == msg2->getDest()){
					tracerouteStarted.erase(tracerouteStarted.begin() + i);
					break;
				}
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
void TracerouteHandler::doHandleTraceroute(IPvXAddress addr) {
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

    // FIXME get pointer to TracerouteTable
    TracerouteTable *routetable = NULL;
	if(routetable->getRouteSize2(addr) > 0) {
		return;
	}

	StartTraceroute *msg = new StartTraceroute();
	msg->setDest(addr);
	UDPControlInfo *ctrl = new UDPControlInfo();
	//FIXME: get own address for message?
	//NodeDescriptor self = nodeTable()->selfDescription();
	//ctrl->setSrcAddr(self.getAddress());
	ctrl->setDestAddr(IPvXAddress(IPAddress(0,0,0,0)));
	ctrl->setDestPort(30000);
	ctrl->setSockId(30000);
	msg->setControlInfo(ctrl);
	tracerouteStarted.push_back(addr);
	send(msg,"to_ctrl");
}

TracerouteHandler::TracerouteHandler() {
}

TracerouteHandler::~TracerouteHandler() {
}
