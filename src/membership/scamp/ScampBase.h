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

#ifndef SCAMP_BASE_H_
#define SCAMP_BASE_H_

#define SCAMP_STATE_IDLE      0
#define SCAMP_STATE_JOINING   1
#define SCAMP_STATE_JOINED    2
#define SCAMP_STATE_LEAVE     3

#include "CommBase.h"

#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"

//#include "ActivePeerTable.h"
//#include "IChurnGenerator.h"
//#include "GlobalStatistic.h"

#include "MembershipBase.h"
#include "AppCommon.h"
#include "Contact.h"
#include "MessageLogger.h"
#include "Dispatcher.h"
#include "GossipMembershipPacket_m.h"
#include "ScampStatistic.h"

//class ScampBase : public CommBase
class ScampBase : public MembershipBase
{
public:
    ScampBase();
    virtual ~ScampBase();

    virtual void handleMessage(cMessage* msg);

protected:
    virtual void finish();

    // Define functions in Base class
public:
    //IPvXAddress getARandPeer();
    IPvXAddress getARandPeer(IPvXAddress address);

    void addPeerAddress(const IPvXAddress &address, int maxNOP=0);
    void addSourceAddress(const IPvXAddress &address, int maxNOP=0);

    bool deletePeerAddress(const IPvXAddress &address);

    void incrementNPartner(const IPvXAddress &addr);
    void decrementNPartner(const IPvXAddress &addr);

protected:
//    virtual void processPacket(cPacket *pkt) = 0;
    virtual void handleTimerMessage(cMessage *msg) = 0;

    void readParameter(void);
    void bindToGlobalModule(void);
    void bindToParentModule(void);
    void bindToStatisticModule(void);

    // -- Helper functions
//    int getNodeAddress(IPvXAddress &address);
//    IPvXAddress getNodeAddress(void);

    // -- Gossip related functions
    void processGossipPacket(cPacket *pkt);
    void handleNewSubscription(cPacket *pkt);
    void handleForwardedSubscription(cPacket *pkt);
    void subscribe(void);
    void unsubscribe(void);
    void resubscribe(void);

    bool isIsolated(void);
    void checkIsolation(void);

    // -- Heartbeat
    void handleHeartbeat(cPacket *pkt);

    void handleAckPacket(cPacket *pkt);

    // -- Gossiped Application Message
    void sendGossipAppMessage();
    void handleAppPacket(cPacket *pkt);

    double getSimDuration(void);

//    void sendToDispatcher(cPacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort);
//    void printPacket(cPacket *pkt);

protected:
    // -- For communicating via UDP
    int m_localPort, m_destPort;

    // SCAMP protocl parameters
    int m_c;

    bool m_active;

    // Parameters
    double param_isoCheckInterval;
    double param_heartbeatInterval;
    double param_appMessageInterval;

    // -- Timers
    cMessage *timer_isolationCheck;
    cMessage *timer_heartbeatSending;
    cMessage *timer_sendAppMessage;

    //cMessage *timer_reportPvSize;
    Dispatcher *m_dispatcher;

    // -- Global module
    ScampStatistic *m_gstat;

    // -- Internal modules
    MessageLogger m_msgLogger;
    ContactView m_inView;
    ContactView m_partialView;

    // -- Heartbeat message
    GossipHeartbeatPacket *m_heartbeatMsg;

    // -- Id for each gossiped Application message
    long m_messageId;

    double m_simDuration;

    int m_state;

};

#endif /* SCAMP_BASE_H_ */
