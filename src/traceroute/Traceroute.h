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

#ifndef __INET_TRACEROUTE_H_
#define __INET_TRACEROUTE_H_

#include <omnetpp.h>
#include "TracerouteTable.h"

/**
 * TODO - Generated class
 */
class Traceroute : public cSimpleModule
{
  protected:

	TracerouteTable::RouteMap traces;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    /*
     * sends ICMPEchoRequest for traceroute
     */
    virtual void sendICMPMessage(const IPAddress& dest, int ttl);

    /*
     * Send route to hold it in RouteTable and delete it from trace map
     */
    virtual void finishTrace(const IPAddress& dest);

    int port;
    cMessage* test_timer;

  public:
    TracerouteTable::RouteMap getTraces();
    void restartTraceroute(TracerouteTable::RouteMap traceMap);
};

#endif
