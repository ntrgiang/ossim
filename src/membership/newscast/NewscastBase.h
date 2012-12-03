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

#ifndef NEWSCASTBASE_H_
#define NEWSCASTBASE_H_

#include "csimplemodule.h"
#include "INETDefs.h"

// Includes for Global modules
#include "ActivePeerTable.h"
#include "IChurnGenerator.h"

// Cache
#include "NewscastCache.h"
// Agents
#include "NewscastAgentInterface.h"

#include "Dispatcher.h"
class NewscastBase : public cSimpleModule {
public:
    NewscastBase();
    virtual ~NewscastBase();

    virtual void handleMessage(cMessage* msg);

    // TODO: TEMP
    int counter;

protected:
    // seems like the multiple stages are needed for the ip interface :/
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);

    // global modules -> check later if rly needed/change
    void bindToGlobalModule(void);
    IChurnGenerator *m_churn;
    ActivePeerTable *m_apTable;

    Dispatcher *m_dispatcher;
    // -- For communicating via UDP
    int m_localPort, m_destPort;
    IPvXAddress m_localAddress;
    void findNodeAddress(void);
    IPvXAddress getNodeAddress(void);
    // end global modules

    // Messages for timers
    cMessage* timer_ExchangeCache;
    cMessage* timer_JoinNetwork;

    // local objects
    NewscastCache* m_cache;
    std::string ownName;    // name of the local agent
    cObject* ownValue;      // value of the local agent

    void sendCacheExchangeRequest(IPvXAddress addr);
    void sendCacheExchangeReply(IPvXAddress addr);
    bool checkBootstrapNeeded();
    void doBootstrap();
    void doCacheExchange();

private:

    //listeners -> "agents"
    typedef std::list<NewscastAgentInterface*> AgentList;
    mutable AgentList localAgents;
    void addAgent(NewscastAgentInterface* agent);
    void removeAgent(NewscastAgentInterface* agent);

    void sendPacketTo(cPacket* pkt, IPvXAddress addr);
    void handlePacket(cPacket* pkt);

    void updateOwnCache();
    void receivedCache(IPvXAddress from, NewscastCache* cache);
#define MESSAGE_CODE_REQUEST "NEWSCAST_REQUEST"
#define MESSAGE_CODE_REPLY   "NEWSCAST_REPLY"
    //static const char* MESSAGE_CODE_REQUEST = "NEWSCAST_REQUEST";
    //static const char* MESSAGE_CODE_REPLY   = "NEWSCAST_REPLY";
};

#endif /* NEWSCASTBASE_H_ */
