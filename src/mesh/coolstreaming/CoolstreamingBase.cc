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

#include "CoolstreamingBase.h"
#include "DpControlInfo_m.h"

CoolstreamingBase::CoolstreamingBase() {}

CoolstreamingBase::~CoolstreamingBase() {
    if (timer_CheckPartners) delete cancelEvent(timer_CheckPartners);


    m_outFileDebug.close();
}

void CoolstreamingBase::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        processPacket(PK(msg));
        delete msg;
    }
}

void CoolstreamingBase::initBase(){
    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    findNodeAddress();

    // read parameters
    param_SubstreamCount = par("substreamCount");
    param_BufferMapIntervall = par("BufferMapIntervall");
    param_PartnerTimeout = param_BufferMapIntervall * 5;
        // partnership
        param_maxNOP = par("maxNOP");
        param_maxNOC = par("maxNOC");
        param_CheckPartnersIntervall = par("CheckPartnersIntervall").doubleValue();


    // add listener to video buffer
    m_videoBuffer->addListener(this);

    // bind to Gossip protocol
    cModule *temp = getParentModule()->getModuleByRelativePath("gossipProtocol");
    m_Gossiper = check_and_cast<GossipProtocol *>(temp);
    EV << "Binding to GossipProtocol is completed successfully" << endl;

    // create messages
    timer_CheckPartners = new cMessage("COOLSTREAMING_TIMER_CHECK_PARTNERS");
    timer_BufferMap     = new cMessage("COOLSTREAMING_TIMER_SEND_BUFFER_MAP");

    // schedule timers
    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
    scheduleAt(simTime() + param_BufferMapIntervall, timer_BufferMap);

    // DEBUG
    const char* test = m_localAddress.str().c_str();
    m_outFileDebug.open(test, std::fstream::out);
    m_outFileDebug << simTime().str() << " INIT " << endl;
    delete test;
}

void CoolstreamingBase::processPacket(cPacket *pkt){
    CoolstreamingPacket* csp = dynamic_cast<CoolstreamingPacket*>(pkt);//check_and_cast<CoolstreamingPacket*>(pkt);
    if (csp == NULL) return;

    switch (csp->getPacketType()){
        case COOLSTREAMING_PARTNERSHIP_ACCEPT:
        case COOLSTREAMING_PARTNERSHIP_REQUEST:
        case COOLSTREAMING_PARTNERSHIP_REVOKE:
            processPartnershipPacket(csp);
            break;
        case COOLSTREAMING_BUFFER_MAP:
            processBufferMapPacket( check_and_cast<CoolstreamingBufferMapPacket*>(pkt) );
            break;
    }
}

void CoolstreamingBase::processPartnershipPacket(CoolstreamingPacket *pkt){

    EV << "Got PartnershipPacket" << endl;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REQUEST){
        if ((partners.size() < param_maxNOP) || (isPartner(src))){    // we can accept more partners -> send an accept
            CoolstreamingPartner* partner = addPartner(src);
            CoolstreamingPartnershipAcceptPacket* resp = new CoolstreamingPartnershipAcceptPacket();
            sendToDispatcher(resp, m_localPort, src, m_destPort);
            sendBufferMap(partner);
        }
    }

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_ACCEPT){
        if (partners.size() < param_maxNOP ){    // we can accept more partners -> send an accept
            addPartner(src);
        }else{  // send a revoke ... if we have enough partners now ...
            CoolstreamingPartnershipRevokePacket* resp = new CoolstreamingPartnershipRevokePacket();
            sendToDispatcher(resp, m_localPort, src, m_destPort);
        }
    }

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REVOKE){
        m_outFileDebug << simTime().str() << " Got Revoke Partnership: " << src.str() << endl;
        removePartner(src);
    }
}

CoolstreamingPartner* CoolstreamingBase::addPartner(IPvXAddress addr){

    CoolstreamingPartner* p = findPartner(addr);
    if (p != NULL) return p; // already exists

    p = new CoolstreamingPartner(param_SubstreamCount);
        p->setAddress(addr);
        p->setLastSeen(simTime());
    partners.push_back(p);

    m_outFileDebug << simTime().str() << " Got new Partner: " << addr.str() << endl;

    return p;
}

CoolstreamingPartner* CoolstreamingBase::findPartner(IPvXAddress addr){
    for (unsigned int i = 0; i < partners.size(); i++)
        if (partners.at(i)->getAddress().equals(addr))
            return partners.at(i);
    return NULL;
}
CoolstreamingPartner* CoolstreamingBase::getParent(int substream){
    for (unsigned int i = 0; i < partners.size(); i++)
            if (partners.at(i)->isParent(substream))
                return partners.at(i);
        return NULL;
}
bool CoolstreamingBase::isPartner(IPvXAddress addr){
    return (findPartner(addr) != NULL);
}

void CoolstreamingBase::removePartner(IPvXAddress addr){
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++){
        if ( (*it)->getAddress().equals(addr) ){
            (*it)->deleted = true;

            m_outFileDebug << simTime().str() << " Remove Partner: " << addr.str() << endl;

            delete *it;
            partners.erase(it);

            return;
        }
    }
}

void CoolstreamingBase::removeTimeoutedPartners(){
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++){
        if ( (*it)->getLastSeen() + param_PartnerTimeout < simTime()){ // Timed out
            MessageBoxA(0,"Timed out!","CoolstreamingBase::removeTimeoutedPartners",0);
            //delete *it;
            (*it)->deleted = true;

            m_outFileDebug << simTime().str() << " Partner timed out: " << (*it)->getAddress().str() << endl;

            delete *it;
            partners.erase(it);

            //partners.erase(it);
            MessageBoxA(0,"Timed out! 2","CoolstreamingBase::removeTimeoutedPartners",0);
            return;
        }
    }
}

void CoolstreamingBase::checkPartners(){
    removeTimeoutedPartners();

    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
}


void CoolstreamingBase::handleTimerMessage(cMessage *msg){

    if (msg == timer_BufferMap){
        EV << "CoolstreamingBase::timer_BufferMap" << m_localAddress.str() << endl;
        m_outFileDebug << simTime().str() << " CoolstreamingBase::timer_BufferMap" << endl;
        for (int i = 0; i < param_SubstreamCount; i++){
            CoolstreamingPartner* parent = getParent(i);
            if (parent != NULL){
                EV << "[" << i << "] " << getLatestSequenceNumber(i) << " from " << parent->getAddress().str() << endl;
                m_outFileDebug << simTime().str() << " [" << i << "] " << getLatestSequenceNumber(i) << " (" << getLatestSequenceNumberB(i) << ") from " << parent->getAddress().str() << endl;
            }
            else{
                EV << "[" << i << "] " << getLatestSequenceNumber(i) << " NO PARENT" << endl;
                m_outFileDebug << simTime().str() << " [" << i << "] " << getLatestSequenceNumber(i) << " (" << getLatestSequenceNumberB(i) << ") NO PARENT" << endl;
            }
        }

        m_videoBuffer->printStatus();

        std::vector<CoolstreamingPartner*>::iterator it;
        for (it = partners.begin(); it != partners.end(); it++){
            sendBufferMap( (*it) );
        }

        scheduleAt(simTime() + param_BufferMapIntervall, timer_BufferMap);
    }

    if (msg == timer_CheckPartners)
        checkPartners();
}

void CoolstreamingBase::sendBufferMap(CoolstreamingPartner* dest){
    CoolstreamingBufferMapPacket* pkt = new CoolstreamingBufferMapPacket();

    pkt->setSubscribeArraySize(param_SubstreamCount);
    pkt->setSequenceNumbersArraySize(param_SubstreamCount);

    // set if we want to receive a certain substream
    for (int i = 0; i < param_SubstreamCount; i++){
        pkt->setSubscribe(i, dest->isParent(i));
        pkt->setSequenceNumbers(i, getLatestSequenceNumber(i));
    }

    sendToDispatcher(pkt, m_localPort, dest->getAddress(), m_destPort);
}

int CoolstreamingBase::getLatestSequenceNumber(int substream){
    /*int lastKnown = m_videoBuffer->getBufferStartSeqNum() + ( substream - (m_videoBuffer->getBufferStartSeqNum() % param_SubstreamCount));
    for(int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getBufferEndSeqNum(); i++){
        if ((i % param_SubstreamCount) == substream){
            if ( m_videoBuffer->isInBuffer(i) )
                lastKnown = i;
            else
                break;
        }
    }
    return lastKnown;*/

    int ok = 0;
    int last = m_videoBuffer->getBufferEndSeqNum() + ( substream - (m_videoBuffer->getBufferEndSeqNum() % param_SubstreamCount));

    for ( int i = last; i > m_videoBuffer->getBufferStartSeqNum(); i -= param_SubstreamCount)
        if (m_videoBuffer->isInBuffer(i)){
            ok++;
            if (ok == 5 ) return  (i + 4 * param_SubstreamCount);
        }else
            ok = 0;

    return m_videoBuffer->getBufferStartSeqNum() + ( substream - (m_videoBuffer->getBufferStartSeqNum() % param_SubstreamCount));
}

int CoolstreamingBase::getLatestSequenceNumberB(int substream){
    int lastKnown = m_videoBuffer->getBufferStartSeqNum() + ( substream - (m_videoBuffer->getBufferStartSeqNum() % param_SubstreamCount));
    for(int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getBufferEndSeqNum(); i++){
        if ((i % param_SubstreamCount) == substream){
            if ( m_videoBuffer->isInBuffer(i) )
                lastKnown = i;
            else
                break;
        }
    }
    return lastKnown;
}

void CoolstreamingBase::processBufferMapPacket(CoolstreamingBufferMapPacket* pkt){
    //MessageBoxA(0,"processBufferMapPacket_","processBufferMapPacket_",0);
    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    EV << "CoolstreamingBase::processBufferMapPacket: " << src.str() << endl;

    //MessageBoxA(0,"processBufferMapPacket_0","processBufferMapPacket_",0);
    CoolstreamingPartner* partner = findPartner(src);
    //MessageBoxA(0,"processBufferMapPacket_1","processBufferMapPacket_",0);
    if (partner == NULL){
        EV << "received BUFFER-MAP from NON-PARTNER!" << endl;
        return;
    }
    //MessageBoxA(0,"processBufferMapPacket_2","processBufferMapPacket_",0);
    EV << "CoolstreamingBase::processBufferMapPacket " << src.str() <<  endl;
    // if he is a new subscriber
    for (unsigned int i = 0; i < pkt->getSubscribeArraySize(); i++)
        if ((!partner->isChild(i)) && (pkt->getSubscribe(i))) {// new subscriber

            /*char* test = new char[100];
            sprintf(test, "test: %i %i %i %i",i ,pkt->getSequenceNumbers(i), m_videoBuffer->getBufferEndSeqNum(), param_SubstreamCount);
            MessageBoxA(0,test,"processBufferMapPacket_3",0);
            delete test;*/

            EV << "CoolstreamingBase::processBufferMapPacket-> GotSubscribe: " << i << " _ " << pkt->getSequenceNumbers(i) << " _ " << getLatestSequenceNumber(i) << endl;
            m_outFileDebug << simTime().str() << " Got subscribe: " << (i) << " _ " << pkt->getSequenceNumbers(i) << " from " << src.str() << endl;

            //for (int k = pkt->getSequenceNumbers(i); k < m_videoBuffer->getBufferEndSeqNum(); k += param_SubstreamCount)    // send him the "missing" packets from the substream(s)
                //if ( (k % param_SubstreamCount) == i)
                    //sendChunk(partner, k);
        }else if ( (partner->isChild(i)) && (!pkt->getSubscribe(i)) ){
            m_outFileDebug << simTime().str() << " Got UNsubscribe: " << (i) << " _ " << pkt->getSequenceNumbers(i) << " from " << src.str() << endl;
        }
    //MessageBoxA(0,"processBufferMapPacket_4","processBufferMapPacket_",0);
    EV << "CoolstreamingBase::processBufferMapPacket->2" << endl;
    partner->updateFromMessage(pkt);
    //MessageBoxA(0,"processBufferMapPacket_5","processBufferMapPacket_",0);
}

void CoolstreamingBase::sendChunk(CoolstreamingPartner* dest, int number){
    //EV << "CoolstreamingBase::sendChunk(" << dest->addr.str() << " _ " << number << endl;

    /*char* test = new char[100];
    sprintf(test, "sendChunk: %i", number);
    MessageBoxA(0,test,"sendChunk",0);
    delete test;*/

    if (!m_videoBuffer->isInBuffer(number)) // check if we have the chunk
        return;

    m_forwarder->sendChunk(number, dest->getAddress(), m_destPort);

    //m_outFileDebug << simTime().str() << " Sending chunk: " << (number % param_SubstreamCount) << " _ " << number << " to " << dest->getAddress().str() << endl;

    /*VideoChunkPacket* pkt = m_videoBuffer->getChunk(number)->dup();
    pkt->setPacketGroup(PACKET_GROUP_MESH_OVERLAY); // HACK to receive the packet :/
    sendToDispatcher(pkt, m_localPort, dest->addr, m_destPort);*/
}

void CoolstreamingBase::onNewChunk(int sequenceNumber){

    EV << "CoolstreamingBase::onNewChunk: " << m_localAddress.str() << " " << sequenceNumber << endl;

    //m_outFileDebug << simTime().str() << " Got new chunk: " << (sequenceNumber % param_SubstreamCount) << " _ " << sequenceNumber << endl;

    int substream = sequenceNumber % param_SubstreamCount;

    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it < partners.end(); it++){
        if ( (*it)->isChild(substream) )
            sendChunk( (*it), sequenceNumber );
    }

    if ( getParent(substream) != NULL )
        getParent(substream)->updateLastSeen();
}































void CoolstreamingBase::getAppSetting(void)
{
    EV << "CoolstreamingBase::getAppSetting" << endl;

    // -- AppSetting
    m_localPort = getLocalPort();
    m_destPort = getDestPort();

    param_interval_bufferMap     = m_appSetting->getBufferMapInterval();

    param_videoStreamBitRate    = m_appSetting->getVideoStreamBitRate();
    param_chunkSize             = m_appSetting->getChunkSize();
    param_bufferMapSize_second  = m_appSetting->getBufferMapSizeSecond();

    m_videoStreamChunkRate      = m_appSetting->getVideoStreamChunkRate();
    m_bufferMapSize_chunk       = m_appSetting->getBufferMapSizeChunk();
    m_BufferMapPacketSize_bit   = m_appSetting->getPacketSizeBufferMap() * 8;
//    m_BufferMapPacketSize_bit   = m_appSetting->getBufferMapPacketSizeBit();
}

void CoolstreamingBase::readChannelRate(void)
{
//    cDatarateChannel *channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());
//    param_upBw = channel->getDatarate();

    // -- problem unresolved!!!
//    channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
//            ->getTransmissionChannel());
//    param_downBw = channel->getDatarate();

    // -- Trying new functions
    param_upBw = getUploadBw();
    param_downBw = getDownloadBw();
}

double CoolstreamingBase::getUploadBw()
{
    cDatarateChannel *channel = check_and_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());

    return channel->getDatarate();
}

double CoolstreamingBase::getDownloadBw()
{
    double rate;
    cModule* nodeModule = getParentModule()->getParentModule();

    int gateSize = nodeModule->gateSize("pppg$i");
    for (int i=0; i<gateSize; i++)
    {
        cGate* currentGate = nodeModule->gate("pppg$i",i);
        if (currentGate->isConnected())
        {
            rate = check_and_cast<cDatarateChannel *>(currentGate->getPreviousGate()->getChannel())->getDatarate();
//                capacity += check_and_cast<cDatarateChannel *>
//                    (currentGate->getPreviousGate()->getChannel())->getDatarate()
//                    - uniform(0,800000);
        }
    }
    //channel = dynamic_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$i", 0)->getChannel());
                //->getTransmissionChannel());

    return rate;
}

void CoolstreamingBase::bindToMeshModule(void)
{
    cModule *temp = getParentModule()->getModuleByRelativePath("partnerList");
    m_partnerList = check_and_cast<PartnerList *>(temp);
    EV << "Binding to PartnerList is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
    EV << "Binding to VideoBuffer is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("forwarder");
    m_forwarder = check_and_cast<Forwarder *>(temp);
    EV << "Binding to Forwarder is completed successfully" << endl;

}

void CoolstreamingBase::bindToGlobalModule(void)
{
    // -- Recall the same function at the base class
    CommBase::bindToGlobalModule();

    // -- Some thing new to the function
    cModule *temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingDonet *>(temp);
    EV << "Binding to AppSettingDonet is completed successfully" << endl;

    temp = simulation.getModuleByPath("meshObserver");
    m_meshOverlayObserver = check_and_cast<MeshOverlayObserver *>(temp);
    EV << "Binding to MeshOverlayObserver is completed successfully" << endl;

    temp = simulation.getModuleByPath("logger");
    m_logger = check_and_cast<Logger *>(temp);
    EV << "Binding to Logger is completed successfully" << endl;

}
