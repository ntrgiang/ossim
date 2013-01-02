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

NewscastBase::NewscastBase() {
    // TODO Auto-generated constructor stub
    ownValue = 0.0f;
    counter = 0;
}

NewscastBase::~NewscastBase() {
    EV << "~NewscastBase()" << endl;
    if (timer_JoinNetwork)   delete cancelEvent(timer_JoinNetwork);
    if (timer_ExchangeCache) delete cancelEvent(timer_ExchangeCache);

    if (m_cache) delete m_cache;
}

void NewscastBase::initialize(int stage) {

    if (stage != 4) return;

    /*EV << "0My name? " << getName() << endl;
    EV << "1My name? " << getParentModule()->getName() << endl;
    EV << "2My name? " << getParentModule()->getParentModule()->getName() << endl;
    EV << "2My name? " << getParentModule()->getParentModule()->getId() << endl;
    EV << "2My name? " << getParentModule()->getParentModule()->getFullName() << endl;*/

    ownName = getParentModule()->getParentModule()->getFullName();
    EV << "my name: " << ownName << " value: " << ownValue <<  endl;

    bindToGlobalModule(); // connect to global modules
    findNodeAddress(); // find and store own address

    m_cache = new NewscastCache((int)par("cacheSize"));

    timer_ExchangeCache = new cMessage("Newscast: ExchangeCacheTimer");
    timer_JoinNetwork   = new cMessage("Newscast: JoinNetworkTimer");

    scheduleAt(simTime() + m_churn->getArrivalTime() , timer_JoinNetwork);
}

void NewscastBase::handleMessage(cMessage *msg){
    EV << "[NewscastBase] handleMessage" << endl;
    if (msg == timer_JoinNetwork){

        EV << "[NewscastBase] Join Network Timer!" << endl;
        m_apTable->addPeerAddress(m_localAddress);
        scheduleAt(simTime(), timer_ExchangeCache);

    }else if (msg == timer_ExchangeCache){ // now do the algorithm ...

        EV << "[NewscastBase] Timer!" << endl;

        if (checkBootstrap()) // do we need a bootstrap?
            doBootstrap();
        else{ // do a regular exchange
            doCacheExchange();
        }

        counter++;
        if (counter < 50)
            scheduleAt(simTime() +10, timer_ExchangeCache);
    }else{

        EV << "BEFORE: " << msg->getName() << endl;
        handlePacket(PK(msg));

    }
}

void NewscastBase::handlePacket(cPacket* pkt){

    // -- Extract the IP and port of the responder
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
/*
    //EV << "START" << endl;
    if (strcmp(pkt->getName(), MESSAGE_CODE_REQUEST) == 0){ // we got a request ... send our cache and update it
        EV << "[NewscastBase] Got REQUEST!" << endl;
        EV << "[NewscastBase] REQUEST PARAS COUNT: " << pkt->getParList().size() << endl;


    }else if (strcmp(pkt->getName(), MESSAGE_CODE_REPLY) == 0){ // got a answer ... update our cache
        EV << "[NewscastBase] Got REPLY!" << endl;


    }else{ // should happen ...
*/
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

    //}
    //EV << "DONE" << endl;

    delete pkt; // QUESTION: do i have to do this?
}

void NewscastBase::sendCacheExchangeRequest(IPvXAddress addr) {
    //EV << "sendCacheExchangeRequest: 1" << endl;
    NewscastRequestPacket* pkt = new NewscastRequestPacket();

    //EV << "sendCacheExchangeRequest: 2" << endl;
    // update own cache entry
    m_cache->setEntry(ownName, m_localAddress, simTime(), ownValue);

    pkt->setCache( *((NewscastCache*) m_cache->dup()) );
    //pkt->addByteLength(m_cache->getByteLength());

    //EV << "myCache: " << ownName << endl;
    m_cache->printCache();

    //EV << "sendCacheExchangeRequest: 3" << endl;
    sendPacketTo(pkt, addr);
    //EV << "sendCacheExchangeRequest: 4" << endl;
}

void NewscastBase::sendCacheExchangeReply(IPvXAddress addr) {
    //EV << "sendCacheExchangeReply: 1" << endl;
    NewscastReplyPacket*  pkt = new NewscastReplyPacket();

    // update own cache entry
    //EV << "[before-set]my name: " << ownName << " value: " << ownValue <<  endl;
    m_cache->setEntry(ownName, m_localAddress, simTime(), ownValue);
    //EV << "[after-set]my name: " << ownName << " value: " << ownValue <<  endl;

    //EV << "sendCacheExchangeReply: 2" << endl;
    pkt->setCache( *((NewscastCache*) m_cache->dup()) );

    //EV << "sendCacheExchangeReply: 3" << endl;
    sendPacketTo(pkt, addr);
    //EV << "sendCacheExchangeReply: 4" << endl;
}

void NewscastBase::sendPacketTo(cPacket* pkt, IPvXAddress addr){
    //EV << "sendPacketTo: 1" << endl;
    DpControlInfo *ctrl = new DpControlInfo();
        ctrl->setSrcAddr(m_localAddress);
        ctrl->setSrcPort(m_localPort);
        ctrl->setDestAddr(addr);
        ctrl->setDestPort(m_destPort); /// well ... this shouldnt work ...
    //EV << "sendPacketTo: 2" << endl;
    pkt->setControlInfo(ctrl);
    //EV << "sendPacketTo: 3" << endl;

    send(pkt, "dpOut");
    //EV << "sendPacketTo: 4" << endl;
}

bool NewscastBase::checkBootstrap(){
    //EV << "checkBootstrap: 1" << endl;
    if (m_cache->getSize() > 1) // there are at least 2 entries(ours + 1 else)
        return false;

    //EV << "checkBootstrap: 2" << endl;
    if ((m_cache->getSize() > 0) && (!m_cache->getEntry(0)->getAddress().equals(m_localAddress))) // just one entry ... but isnt ours -> we are fine
        return false;

    //EV << "checkBootstrap: 3" << endl;
    return true; // we need a bootstrap ..
}

void NewscastBase::doBootstrap(){
    //EV << "doBootstrap: 1" << endl;
    if (m_apTable->getNumActivePeer() == 0) // no active peers in the network?! this shouldnt happen ...
        return;

    //EV << "doBootstrap: 2" << endl;
    IPvXAddress addr = m_apTable->getARandPeer();

    //EV << "doBootstrap: 3" << endl;
    if (addr.equals(m_localAddress) && (m_apTable->getNumActivePeer() == 1)) // we are the only entry :(
        return;

    //EV << "doBootstrap: 4" << endl;
    while(addr.equals(m_localAddress))   // loop until we find one that isnt ours ...
        addr = m_apTable->getARandPeer();

    //EV << "doBootstrap: 5" << endl;
    // query the new addr ...
    sendCacheExchangeRequest(addr);
    //EV << "doBootstrap: 6" << endl;
}

void NewscastBase::doCacheExchange(){
    // find an address ...
    IPvXAddress addr = m_cache->getRandomEntry()->getAddress();
    while(addr.equals(m_localAddress))   // loop until we find one that isnt ours ...
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


void NewscastBase::receivedCache(IPvXAddress from, NewscastCache* cache){
    NewscastCacheEntry* entry = cache->getEntry(from);

    if (entry == NULL) return;

    ownValue = (ownValue + entry->getValue()) / 2;
    EV << "estimated number of nodes: " << round(1 / ownValue) << " at " << counter << endl;
}
