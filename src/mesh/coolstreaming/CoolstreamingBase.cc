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

CoolstreamingBase::~CoolstreamingBase() {}

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
    timer_BufferMap     = new cMessage("COOLSTREAMING_SEND_BUFFER_MAP");

    // schedule timers
    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
    scheduleAt(simTime() + param_BufferMapIntervall, timer_BufferMap);
}

void CoolstreamingBase::processPacket(cPacket *pkt){
    CoolstreamingPacket* csp = check_and_cast<CoolstreamingPacket*>(pkt);

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
    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REQUEST){
        if ((partners.size() < param_maxNOP) && (!isPartner(src))){    // we can accept more partners -> send an accept
            addPartner(src);
            CoolstreamingPartnershipAcceptPacket* resp = new CoolstreamingPartnershipAcceptPacket();
            sendToDispatcher(resp, m_localPort, src, m_destPort);
        }
    }

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_ACCEPT){
        if (partners.size() < param_maxNOP ){    // we can accept more partners -> send an accept
            if (isPartner(src))
                return;
            addPartner(src);
        }else{  // send a revoke ... if we have enough partners now ...
            CoolstreamingPartnershipRevokePacket* resp = new CoolstreamingPartnershipRevokePacket();
            sendToDispatcher(resp, m_localPort, src, m_destPort);
        }
    }

    if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REVOKE)
        removePartner(src);
}

void CoolstreamingBase::addPartner(IPvXAddress addr){
    CoolstreamingPartner* p = new CoolstreamingPartner(param_SubstreamCount);
        p->addr = addr;
        p->lastSeen = simTime();
    partners.push_back(p);
}

void CoolstreamingBase::removePartner(IPvXAddress addr){
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++){
        if ( (*it)->addr.equals(addr)){ // Timedout
            delete *it;
            partners.erase(it);
        }
    }
}

CoolstreamingPartner* CoolstreamingBase::findPartner(IPvXAddress addr){
    for (unsigned int i = 0; i < partners.size(); i++)
        if (partners.at(i)->addr.equals(addr))
            return partners.at(i);
    return NULL;
}
bool CoolstreamingBase::isPartner(IPvXAddress addr){
    return (findPartner(addr) != NULL);
}

void CoolstreamingBase::removeTimeoutedPartners(){
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++){
        if ( (*it)->lastSeen + param_PartnerTimeout < simTime()){ // Timedout
            delete *it;
            partners.erase(it);
        }
    }
}

void CoolstreamingBase::checkPartners(){
    removeTimeoutedPartners();

    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
}


void CoolstreamingBase::handleTimerMessage(cMessage *msg){

    if (msg == timer_sendBufferMap){
        std::vector<CoolstreamingPartner*>::iterator it;
        for (it = partners.begin(); it != partners.end(); it++){
            sendBufferMap( (*it) );
        }
    }
}

void CoolstreamingBase::sendBufferMap(CoolstreamingPartner* dest){
    CoolstreamingBufferMapPacket* pkt = new CoolstreamingBufferMapPacket();

    pkt->setSubscribeArraySize(param_SubstreamCount);
    pkt->setSequenceNumbersArraySize(param_SubstreamCount);

    // set if we want to receive a certain substream
    for (int i = 0; i < param_SubstreamCount; i++){
        pkt->setSubscribe(i, dest->isParent(i));
    }

    // set our sequence numbers
    bool valid[param_SubstreamCount];   // if we have k, k+2 and not k+1 we want to set it to k not k+2
    for (int i = 0; i < param_SubstreamCount; i++){
        valid[i] = true;
        pkt->setSequenceNumbers(i, 0);
    }

    for(int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getBufferEndSeqNum(); i++){
        if ( (m_videoBuffer->isInBuffer(i)) && (valid[i % param_SubstreamCount]) )
            pkt->setSequenceNumbers( i % param_SubstreamCount, i);
        else
            valid[i % param_SubstreamCount] = false;
    }
}

void CoolstreamingBase::processBufferMapPacket(CoolstreamingBufferMapPacket* pkt){
    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();
    CoolstreamingPartner* partner = findPartner(src);

    if (partner == NULL){
        EV << "received BUFFER-MAP from NON-PARTNER!" << endl;
        return;
    }

    // if he is a new subscriber
    for (unsigned int i = 0; i < pkt->getSubscribeArraySize(); i++)
        if ((partner->isChild(i) != pkt->getSubscribe(i)) && (pkt->getSubscribe(i))) // new subscriber
            for (int k = pkt->getSequenceNumbers(i); k < m_videoBuffer->getBufferEndSeqNum(); k += param_SubstreamCount)    // send him the "missing" packets from the substream(s)
                sendChunk(partner, k);

    partner->updateFromMessage(pkt);
}

void CoolstreamingBase::sendChunk(CoolstreamingPartner* dest, int number){
    if (m_videoBuffer->isInBuffer(number)) // check if we have the chunk
        return;

    m_forwarder->sendChunk(number, dest->addr, m_destPort);

    /*VideoChunkPacket* pkt = m_videoBuffer->getChunk(number)->dup();
    pkt->setPacketGroup(PACKET_GROUP_MESH_OVERLAY); // HACK to receive the packet :/
    sendToDispatcher(pkt, m_localPort, dest->addr, m_destPort);*/
}

void CoolstreamingBase::onNewChunk(int sequenceNumber){

    int substream = sequenceNumber % param_SubstreamCount;

    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it < partners.end(); it++){
        if ( (*it)->isChild(substream) )
            sendChunk( (*it), sequenceNumber );
    }
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
