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

#include "NewscastBase.h"

#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"

#include "DpControlInfo_m.h"
#include "NewscastPacket_m.h"

#include "NewscastAgentInterface.h"

NewscastBase::NewscastBase() {
    ownValue = NULL;
    counter = 0;
}

NewscastBase::~NewscastBase() {
    EV << "~NewscastBase()" << endl;
    if (timer_JoinNetwork)   delete cancelEvent(timer_JoinNetwork);
    if (timer_ExchangeCache) delete cancelEvent(timer_ExchangeCache);

    if (m_cache) delete m_cache;
    if (ownValue) delete ownValue;

    localAgents.clear(); // only empty the list ... dont delete listeners!
}

void NewscastBase::initialize(int stage) {

    if (stage != 4) return;

    // get the name for the agent from the parent module
    ownName = getParentModule()->getParentModule()->getFullName();

    bindToGlobalModule(); // connect to global modules
    findNodeAddress(); // find and store own address

    // create a new cache
    m_cache = new NewscastCache((int)par("cacheSize"));

    // create the messages used for timer actions
    timer_ExchangeCache = new cMessage("Newscast: ExchangeCacheTimer");
    timer_JoinNetwork   = new cMessage("Newscast: JoinNetworkTimer");

    // schedule a timer to join the netwrok
    scheduleAt(simTime() + m_churn->getArrivalTime() , timer_JoinNetwork);
}

void NewscastBase::handleMessage(cMessage *msg){
    EV << "[NewscastBase] handleMessage" << endl;

    if (msg == timer_JoinNetwork){

        EV << "[NewscastBase] Join Network Timer!" << endl;
        m_apTable->addPeerAddress(m_localAddress);
        scheduleAt(simTime(), timer_ExchangeCache);

    }else if (msg == timer_ExchangeCache){  // intervall for cache-exchange reached ...

        EV << "[NewscastBase] Timer!" << endl;

        if (checkBootstrapNeeded()) // do we need a bootstrap?
            doBootstrap();
        else{ // do a regular exchange
            doCacheExchange();
        }

        counter++;
        if (counter < 50)
            scheduleAt(simTime() +10, timer_ExchangeCache); // schedule the next exchange ...
    }else{

        handlePacket(PK(msg));

    }
}

void NewscastBase::handlePacket(cPacket* pkt){

    // -- Extract the IP and port of the responder
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());

    // cast the "unknown" packet to our own packet-class
    NewscastPacket* np = (NewscastPacket*) pkt;

    switch (np->getPacketType()){
        case NEWSCAST_REQUEST:
            EV << "GOT REQUEST" << endl;
            // send our cache as a reply
            sendCacheExchangeReply(controlInfo->getSrcAddr());

            // calculate something?
            receivedCache(controlInfo->getSrcAddr(), &np->getCache());

            // merge the received cache
            m_cache->merge(&np->getCache());
            EV << "myCache: " << ownName << endl;
            m_cache->printCache();
            break;
        case NEWSCAST_REPLY:
            EV << "GOT REPLY" << endl;

            // calculate something?
            receivedCache(controlInfo->getSrcAddr(), &np->getCache());

            // merge the received cache
            m_cache->merge(&np->getCache());
            EV << "myCache: " << ownName << endl;
            m_cache->printCache();
            break;
        default:
            EV << "[NewscastBase] GOT UNKNOWN PACKET!" << endl;
            break;
    }

    delete pkt; // QUESTION: do i have to do this?
}

void NewscastBase::sendCacheExchangeRequest(IPvXAddress addr) {

    // create new request packet
    NewscastRequestPacket* pkt = new NewscastRequestPacket();

    // update own cache entry
    updateOwnCache();

    // add a duplicate of our cache to the packet
    pkt->setCache( *((NewscastCache*) m_cache->dup()) );
    pkt->addByteLength(m_cache->getEstimatedSize());

    //m_cache->printCache();

    // send the packet to the destination
    sendPacketTo(pkt, addr);
}

void NewscastBase::sendCacheExchangeReply(IPvXAddress addr) {

    // create new reply packet
    NewscastReplyPacket*  pkt = new NewscastReplyPacket();

    // update own cache entry
    updateOwnCache();

    // add a duplicate of our cache to the packet
    pkt->setCache( *((NewscastCache*) m_cache->dup()) );
    pkt->addByteLength(m_cache->getEstimatedSize());

    // send the packet to the destination
    sendPacketTo(pkt, addr);
}

void NewscastBase::sendPacketTo(cPacket* pkt, IPvXAddress addr){

    // generate a control info and set the sender and receiver
    DpControlInfo *ctrl = new DpControlInfo();
        ctrl->setSrcAddr(m_localAddress);
        ctrl->setSrcPort(m_localPort);
        ctrl->setDestAddr(addr);
        ctrl->setDestPort(m_destPort);

    // add the info to the packet
    pkt->setControlInfo(ctrl);

    // send the packet
    send(pkt, "dpOut");
}

bool NewscastBase::checkBootstrapNeeded(){

    // if there are more than one entry in the cache we are fine
    if (m_cache->getSize() > 1)
        return false;

    // if there is one entry in the cache which isnt ours we are fine too
    if ((m_cache->getSize() > 0) && (!m_cache->getEntry(0)->getAddress().equals(m_localAddress))) // just one entry ... but isnt ours -> we are fine
        return false;

    // we need a bootstrap ...
    return true;
}

void NewscastBase::doBootstrap(){

    // check if we can get a peer from the table
    if (m_apTable->getNumActivePeer() == 0) // no active peers in the network?! this shouldnt happen ...
        return;

    // get a random address
    IPvXAddress addr = m_apTable->getARandPeer();

    // if this address is ours and there is only one entry we cant do anything :(
    if (addr.equals(m_localAddress) && (m_apTable->getNumActivePeer() == 1))
        return;

    // getting random addresses until we find one that isnt ours
    while(addr.equals(m_localAddress))
        addr = m_apTable->getARandPeer();

    // send a cache-exchange-request to the address
    sendCacheExchangeRequest(addr);
}

void NewscastBase::doCacheExchange(){
    // we have no entry in the cache :(
    if (m_cache->getSize() == 0)
        return;

    // find a random address from our cache ...
    IPvXAddress addr = m_cache->getRandomEntry()->getAddress();

    // if this address is ours and there is only one entry we cant do anything :(
    if (addr.equals(m_localAddress) && (m_cache->getSize() == 1))
        return;

    // loop until we find one that isnt ours ...
    while(addr.equals(m_localAddress))
        addr = m_cache->getRandomEntry()->getAddress();

    // send a request
    sendCacheExchangeRequest(addr);
}


// code from CommBase
void NewscastBase::bindToGlobalModule(void)
{
    // -- Active Peer Table
    cModule *temp = simulation.getModuleByPath("activePeerTable");
    m_apTable = check_and_cast<ActivePeerTable *>(temp);
    //if (m_apTable == NULL) throw cException("NULL pointer to module activePeerTable");
    EV << "Binding to activePeerTable is completed successfully" << endl;

    // -- Churn
    temp = simulation.getModuleByPath("churnModerator");
    m_churn = check_and_cast<IChurnGenerator *>(temp);
    EV << "Binding to Churn is completed successfully" << endl;
}

void NewscastBase::findNodeAddress(void)
{
    IInterfaceTable *inet_ift;
    inet_ift = InterfaceTableAccess().get();

    EV << "Number of interfaces: " << inet_ift->getNumInterfaces() << endl;
    if (inet_ift->getNumInterfaces() < 2) throw cException("Less than 2 interfaces");

    m_localAddress = (IPvXAddress)inet_ift->getInterface(1)->ipv4Data()->getIPAddress();
    EV << "Node's own address is: " << m_localAddress << endl;

    // -- Dispatcher
    cModule *temp = getParentModule()->getModuleByRelativePath("dispatcher");
    m_dispatcher = check_and_cast<Dispatcher *>(temp);
    if (m_dispatcher == NULL) throw cException("m_dispatcher == NULL is invalid");

    m_localPort = m_dispatcher->getLocalPort();
    m_destPort  = m_dispatcher->getDestPort();

}

IPvXAddress NewscastBase::getNodeAddress(void)
{
    return m_localAddress;
}

// end code from CommBase

/*
 * called whenever a cache from a peer has been received
 */
void NewscastBase::receivedCache(IPvXAddress from, NewscastCache* cache){
    AgentList::iterator it;
    for (it = localAgents.begin(); it != localAgents.end(); it++){
        (*it)->onCacheReceived(from, cache);
    }
}

/*
 * inserts the entries of all local agents in our cache
 */
void NewscastBase::updateOwnCache(){
    if (ownName.size() > 0)
        m_cache->setEntry(ownName, m_localAddress, simTime(), ownValue);

    // query local Agents
    AgentList::iterator it;
    for (it = localAgents.begin(); it != localAgents.end(); it++){
        if ((*it)->getAgentName().size() > 0)
            m_cache->setEntry((*it)->getAgentName(), m_localAddress, simTime(), (*it)->getOwnData());
    }
}


/*
 * adds an agent to this instance
 */
void NewscastBase::addAgent(NewscastAgentInterface* agent){
    removeAgent(agent);
    localAgents.push_back(agent);
}
/*
 * removes an agent from this instance
 */
void NewscastBase::removeAgent(NewscastAgentInterface* agent){
    AgentList::iterator it;
    for (it = localAgents.begin(); it != localAgents.end(); it++){
        if (*it == agent)
            localAgents.erase(it);
    }
}
