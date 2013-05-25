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
#ifndef HANDLETRACEROUTE_H_
#define HANDLETRACEROUTE_H_
/*
 * module for doing traceroutes and saving the routes to routeTable
 */

#include <omnetpp.h>
#include "PPDatatypes.h"

class TracerouteHandler: public cSimpleModule {

protected:
	bool active;
	cMessage* test_timer;
	IPvXAddress reference;

public:
	TracerouteHandler();
	virtual ~TracerouteHandler();

//        typedef PPIPvXAddressVector TracerouteVector;
        PPIPvXAddressVector tracerouteStarted;

	void initialize(int stage);
	void finish();
	void handleMessage(cMessage* msg);
	void doHandleTraceroute(IPvXAddress addr);
};

#endif /* HANDLETRACEROUTE_H_ */
