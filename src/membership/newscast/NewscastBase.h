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
// NewscastBase.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief Base class of the newscast implementation
// @ingroup membership
// @ingroup newscast

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
// Gossip Interface
#include "GossipProtocolWithUserData.h"
//stats
#include "NewscastStatistic.h"

#include "Dispatcher.h"


//class NewscastBase : public cSimpleModule, public GossipProtocolWithUserData {
class NewscastBase : public GossipProtocolWithUserData
{
public:
    NewscastBase();
    virtual ~NewscastBase();

    virtual void handleMessage(cMessage* msg);

    virtual void finish();

public:
    // Interface: GossipProtocolWithUserData -->
    virtual void setOwnData(GossipUserData* data);
    virtual GossipUserData* getPeerData(IPvXAddress addr);
    // <-- Interface: GossipProtocolWithUserData

    // Interface: GossipProtocol -->
    bool joinNetwork(IPvXAddress bootstrap = "0.0.0.0");
    //void leaveNetwork();

    // -- obsolete?
    // @brief returns a random Peer address from the local cache
    //IPvXAddress getRandomPeer();

    // @brief returns a random Peer address from the local cache exluding a specified address
    IPvXAddress getRandomPeer(IPvXAddress notThisAddress);
    // @brief returns a list of all peer addresses from the local cache
    std::vector<IPvXAddress> getKnownPeers();
    // <-- Interface: GossipProtocol

    // -- old interface
    void addSourceAddress(const IPvXAddress &address, int maxNOP=0) {};
    void addPeerAddress(const IPvXAddress &address, int maxNOP=0) {};
    bool deletePeerAddress(const IPvXAddress &address) { return false; };

    void incrementNPartner(const IPvXAddress &addr) {};
    void decrementNPartner(const IPvXAddress &addr) {};


protected:
    // seems like the multiple stages are needed for the ip interface :/
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);

    void bindToGlobalModule(void);
    IChurnGenerator *m_churn;
    ActivePeerTable *m_apTable;
    Dispatcher *m_dispatcher;
    NewscastStatistic* m_statistics;

    // -- For communicating via UDP
    int m_localPort, m_destPort;
    IPvXAddress m_localAddress;
    // @brief discovers our own Address
    void findNodeAddress(void);
    IPvXAddress getNodeAddress(void);
    // end global modules

    // Messages for timers
    cMessage* timer_ExchangeCache;
    cMessage* timer_JoinNetwork;

private:
    // local objects
    NewscastCache m_cache;
    // @brief name of the local Agent
    std::string m_ownName;
    // @brief data of the local Agent
    GossipUserData* m_ownValue;
    // @brief indicates if this peer has joined the network
    bool m_Active;

    void sendCacheExchangeRequest(IPvXAddress addr);
    void sendCacheExchangeReply(IPvXAddress addr);
    // @brief checks if we are connected to peers
    bool checkBootstrapNeeded();
    // @brief initiates a cache exchange for bootstrapping. If hint is a valid address that peer will be used.
    void doBootstrap(IPvXAddress hint = "0.0.0.0");
    void doCacheExchange();

public:
    // @brief adds an agent to this peer
    void addAgent(NewscastAgentInterface* agent);
    // @brief removes an agent from this peer
    void removeAgent(NewscastAgentInterface* agent);
private:
    typedef std::list<NewscastAgentInterface*> AgentList;
    // @brief list of attached agents
    mutable AgentList localAgents;

    void sendPacketTo(cPacket* pkt, IPvXAddress addr);
    void handlePacket(cPacket* pkt);

    // @brief adds our own data and the data of all attached agents to our cache
    void updateOwnCache();
    // @brief get called whenever we receive a cache and informs all local agents
    void receivedCache(IPvXAddress from, NewscastCache* cache);
};

#endif /* NEWSCASTBASE_H_ */
