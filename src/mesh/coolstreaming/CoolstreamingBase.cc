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
// CoolstreamingBase.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief base class for source and peer of newscast
// @ingroup mesh
// @ingroup coolstreaming

#include "CoolstreamingBase.h"
#include "DpControlInfo_m.h"

CoolstreamingBase::CoolstreamingBase() {}

CoolstreamingBase::~CoolstreamingBase()
{
    if (timer_CheckPartners) delete cancelEvent(timer_CheckPartners);

    if (debugOutput)
        m_outFileDebug.close();
}

//void CoolstreamingBase::initialize(int stage)
//{

//}

//void CoolstreamingBase::finish()
//{

//}

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

void CoolstreamingBase::initBase()
{
    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    findNodeAddress();

    // read parameters
    param_SubstreamCount = par("substreamCount");
    param_BufferMapInterval = par("BufferMapIntervall");
    param_PartnerTimeout = param_BufferMapInterval * 5;
        // partnership
        param_maxNOP = par("maxNOP");
        if (param_maxNOP < 1) param_maxNOP = 1;
        param_CheckPartnersInterval = par("CheckPartnersIntervall").doubleValue();

    // add listener to video buffer
    m_videoBuffer->addListener(this);

    // bind to Gossip protocol
    // this module is required to retrieve random peers for partnership requests
    cModule *temp = getParentModule()->getModuleByRelativePath("gossipProtocol");
    //m_Gossiper = check_and_cast<GossipProtocol *>(temp);
    m_memManager = check_and_cast<MembershipBase *>(temp);
    EV << "Binding to GossipProtocol is completed successfully" << endl;

    // create messages
    timer_CheckPartners = new cMessage("COOLSTREAMING_TIMER_CHECK_PARTNERS");
    timer_BufferMap     = new cMessage("COOLSTREAMING_TIMER_SEND_BUFFER_MAP");

    // schedule timers
//    scheduleAt(simTime() + param_CheckPartnersInterval, timer_CheckPartners);
    scheduleAt(simTime() + param_BufferMapInterval, timer_BufferMap);

    // DEBUG
    debugOutput = par("debugOutput").boolValue();
    if (debugOutput)
    {
        std::string filename = m_localAddress.str();
        EV << "DEBUG_OPEN_FILE: " << filename << endl;
        m_outFileDebug.open(filename.c_str(), std::fstream::out);
        m_outFileDebug << simTime().str() << " INIT " << endl;
    }

}

void CoolstreamingBase::processPacket(cPacket *pkt)
{
    CoolstreamingPacket* csp = dynamic_cast<CoolstreamingPacket*>(pkt);//check_and_cast<CoolstreamingPacket*>(pkt);
    if (csp == NULL) return;

    switch (csp->getPacketType()){
        case COOLSTREAMING_PARTNERSHIP_ACCEPT:
        case COOLSTREAMING_PARTNERSHIP_REQUEST:
        case COOLSTREAMING_PARTNERSHIP_REVOKE:
        {
            processPartnershipPacket(csp);
            break;
        }
        case COOLSTREAMING_BUFFER_MAP:
        {
            processBufferMapPacket(check_and_cast<CoolstreamingBufferMapPacket*>(pkt) );
            break;
        }
        default:
        {
            throw cException("Unrecognized packet type!");
        }
    }
}

void CoolstreamingBase::processPartnershipPacket(CoolstreamingPacket *pkt)
{
    EV << "Got PartnershipPacket" << endl;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    //if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REQUEST)
    switch (pkt->getPacketType())
    {
    case COOLSTREAMING_PARTNERSHIP_REQUEST:
    {
        if ((partners.size() < param_maxNOP) || (isPartner(src)))
        {    // we can accept more partners -> send an accept
            CoolstreamingPartner* partner = addPartner(src);
            CoolstreamingPartnershipAcceptPacket* resp;
            resp = new CoolstreamingPartnershipAcceptPacket("CS_PARTNER_ACCEPT");
            sendToDispatcher(resp, m_localPort, src, m_destPort);
            sendBufferMap(partner);
        }
        break;
    }
    //if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_ACCEPT)
    case COOLSTREAMING_PARTNERSHIP_ACCEPT:
    {
        if (partners.size() < param_maxNOP )
        {    // we can accept more partners -> send an accept
            addPartner(src);
        }
        else
        {  // send a revoke ... if we have enough partners now ...
            CoolstreamingPartnershipRevokePacket* resp;
            resp  = new CoolstreamingPartnershipRevokePacket("CS_PARTNER_REVOKE");
            sendToDispatcher(resp, m_localPort, src, m_destPort);
        }
        break;
    }
    //if (pkt->getPacketType() == COOLSTREAMING_PARTNERSHIP_REVOKE)
    case COOLSTREAMING_PARTNERSHIP_REVOKE:
    {
        if (debugOutput)
            m_outFileDebug << simTime().str() << " Got Revoke Partnership: " << src.str() << endl;
        removePartner(src);
        break;
    }
    default:
    {
       throw cException("Packet type %d not known!", pkt->getPacketType());
    }
   } // end of switch
}

CoolstreamingPartner* CoolstreamingBase::addPartner(IPvXAddress addr)
{
    CoolstreamingPartner* p = findPartner(addr);
    if (p != NULL) return p; // already exists

    p = new CoolstreamingPartner(param_SubstreamCount);
        p->setAddress(addr);
        p->setLastSeen(simTime());
    partners.push_back(p);

    if (debugOutput)
        m_outFileDebug << simTime().str() << " Got new Partner: " << addr.str() << endl;

    return p;
}

CoolstreamingPartner* CoolstreamingBase::findPartner(IPvXAddress addr)
{
    for (unsigned int i = 0; i < partners.size(); i++)
        if (partners.at(i)->getAddress().equals(addr))
            return partners.at(i);
    return NULL;
}

CoolstreamingPartner* CoolstreamingBase::getParent(int substream)
{
    for (unsigned int i = 0; i < partners.size(); i++)
            if (partners.at(i)->isParent(substream))
                return partners.at(i);
        return NULL;
}

bool CoolstreamingBase::isPartner(IPvXAddress addr)
{
    return (findPartner(addr) != NULL);
}

void CoolstreamingBase::removePartner(IPvXAddress addr)
{
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++)
    {
        if ( (*it)->getAddress().equals(addr) )
        {
            (*it)->deleted = true;

            if (debugOutput)
                m_outFileDebug << simTime().str() << " Remove Partner: " << addr.str() << endl;

            delete *it;
            partners.erase(it);

            return;
        }
    }
}

void CoolstreamingBase::removeTimeoutedPartners()
{
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++)
    {
        if ( (*it)->getLastSeen() + param_PartnerTimeout < simTime())
        { // Timed out
            if (debugOutput)
                m_outFileDebug << simTime().str()
                               << " Partner timed out: " << (*it)->getAddress().str() << endl;
            EV << " partner timedout: " << (*it)->getAddress().str()
               << " @ " << m_localAddress.str() << endl;

            (*it)->deleted = true;
            delete *it;
            partners.erase(it);

            return;
        }
    }
}

void CoolstreamingBase::checkPartners()
{
    removeTimeoutedPartners();

    scheduleAt(simTime() + param_CheckPartnersInterval, timer_CheckPartners);
}


void CoolstreamingBase::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_BufferMap)
    {
        EV << "CoolstreamingBase::timer_BufferMap" << m_localAddress.str() << endl;
        if (debugOutput)
            m_outFileDebug << simTime().str() << " CoolstreamingBase::timer_BufferMap" << endl;
        for (int i = 0; i < param_SubstreamCount; i++)
        {
            CoolstreamingPartner* parent = getParent(i);
            if (parent != NULL)
            {
                EV << "[" << i << "] " << getLatestSequenceNumber(i)
                   << " from " << parent->getAddress().str() << endl;
                if (debugOutput)
                    m_outFileDebug << simTime().str()
                                   << " [" << i << "] " << getLatestSequenceNumber(i)
                                   << " (" << getLatestSequenceNumberB(i) << ") from "
                                   << parent->getAddress().str() << endl;
            }
            else
            {
                EV << "[" << i << "] "
                   << getLatestSequenceNumber(i) << " NO PARENT" << endl;
                if (debugOutput)
                    m_outFileDebug << simTime().str()
                                   << " [" << i << "] " << getLatestSequenceNumber(i)
                                   << " (" << getLatestSequenceNumberB(i) << ") NO PARENT" << endl;
            }
        }

        m_videoBuffer->printStatus();

        if (debugOutput)
            m_outFileDebug << simTime().str() << " Partners: ";

        std::vector<CoolstreamingPartner*>::iterator it;
        for (it = partners.begin(); it != partners.end(); it++)
        {
            if (debugOutput)
                m_outFileDebug << (*it)->getAddress().str() << ", ";
            sendBufferMap( (*it) );
        }

        if (debugOutput)
            m_outFileDebug << endl;

        scheduleAt(simTime() + param_BufferMapInterval, timer_BufferMap);
    }
    else  if (msg == timer_CheckPartners)
    {
        checkPartners();
    }
}

void CoolstreamingBase::sendBufferMap(CoolstreamingPartner* dest)
{
    CoolstreamingBufferMapPacket* pkt = new CoolstreamingBufferMapPacket("CS_BUFFER_MAP");

    pkt->setSubscribeArraySize(param_SubstreamCount);
    pkt->setSequenceNumbersArraySize(param_SubstreamCount);

    // set if we want to receive a certain substream
    for (int i = 0; i < param_SubstreamCount; i++)
    {
        pkt->setSubscribe(i, dest->isParent(i));
        pkt->setSequenceNumbers(i, getLatestSequenceNumber(i));
    }

    sendToDispatcher(pkt, m_localPort, dest->getAddress(), m_destPort);
}

int CoolstreamingBase::getLatestSequenceNumber(int substream)
{

    int ok = 0;
    int last = m_videoBuffer->getBufferEndSeqNum() +
          ( substream - (m_videoBuffer->getBufferEndSeqNum() % param_SubstreamCount));

    int needed = 10;

    for (int i = last; i > m_videoBuffer->getBufferStartSeqNum(); i -= param_SubstreamCount)
    {
        if (m_videoBuffer->isInBuffer(i))
        {
            ok++;
            if (ok >= needed ) return  (i + (needed-1) * param_SubstreamCount);
        }
        else
        {
            ok = 0;
        }
    }
    return m_videoBuffer->getBufferStartSeqNum() +
          ( substream - (m_videoBuffer->getBufferStartSeqNum() % param_SubstreamCount));
}

int CoolstreamingBase::getLatestSequenceNumberB(int substream)
{
    int lastKnown = m_videoBuffer->getBufferStartSeqNum() +
          ( substream - (m_videoBuffer->getBufferStartSeqNum() % param_SubstreamCount));
    for(int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getBufferEndSeqNum(); i++)
    {
        if ((i % param_SubstreamCount) == substream)
        {
            if ( m_videoBuffer->isInBuffer(i) )
                lastKnown = i;
            else
                break;
        }
    }
    return lastKnown;
}

void CoolstreamingBase::processBufferMapPacket(CoolstreamingBufferMapPacket* pkt)
{
    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    if (debugOutput)
        m_outFileDebug << simTime().str() << " Got BufferMap from " << src.str() << endl;

    EV << "CoolstreamingBase::processBufferMapPacket: " << src.str() << endl;

    CoolstreamingPartner* partner = findPartner(src);
    if (partner == NULL){
        EV << "received BUFFER-MAP from NON-PARTNER!" << endl;
        return;
    }
//    EV << "CoolstreamingBase::processBufferMapPacket " << src.str() <<  endl;
    // if he is a new subscriber
    for (unsigned int i = 0; i < pkt->getSubscribeArraySize(); i++)
    {
        if ((!partner->isChild(i)) && (pkt->getSubscribe(i)))
        {   // new subscriber

            /*char* test = new char[100];
            sprintf(test, "test: %i %i %i %i",i ,pkt->getSequenceNumbers(i), m_videoBuffer->getBufferEndSeqNum(), param_SubstreamCount);
            delete test;*/

            EV << "CoolstreamingBase::processBufferMapPacket-> GotSubscribe: " << i
               << " _ " << pkt->getSequenceNumbers(i) << " _ " << getLatestSequenceNumber(i) << endl;
            if (debugOutput)
                m_outFileDebug << simTime().str()
                               << " Got subscribe: " << (i)
                               << " _ " << pkt->getSequenceNumbers(i)
                               << " from " << src.str() << endl;

            //for (int k = pkt->getSequenceNumbers(i); k < m_videoBuffer->getBufferEndSeqNum(); k += param_SubstreamCount)    // send him the "missing" packets from the substream(s)
                //if ( (k % param_SubstreamCount) == i)
                    //sendChunk(partner, k);
        }
        else if ( (partner->isChild(i)) && (!pkt->getSubscribe(i)) )
        {
            if (debugOutput)
                m_outFileDebug << simTime().str()
                               << " Got UNsubscribe: " << (i)
                               << " _ " << pkt->getSequenceNumbers(i)
                               << " from " << src.str() << endl;
        }
    }
    EV << "CoolstreamingBase::processBufferMapPacket->2" << endl;
    partner->updateFromMessage(pkt);
}

void CoolstreamingBase::sendChunk(CoolstreamingPartner* dest, int number)
{
    if (!m_videoBuffer->isInBuffer(number)) // check if we have the chunk
        return;

    m_forwarder->sendChunk(number, dest->getAddress(), m_destPort);
}

void CoolstreamingBase::onNewChunk(int sequenceNumber)
{
    EV << "CoolstreamingBase::onNewChunk: " << m_localAddress.str() << " " << sequenceNumber << endl;

    if (debugOutput)
        m_outFileDebug << simTime().str()
                       << " Got new chunk: " << (sequenceNumber % param_SubstreamCount)
                       << " _ " << sequenceNumber << endl;

    int substream = sequenceNumber % param_SubstreamCount;

    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it < partners.end(); it++)
    {
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

    param_videoStreamBitRate    = m_appSetting->getVideoStreamBitRate();
    param_chunkSize             = m_appSetting->getChunkSize();

    m_videoStreamChunkRate      = m_appSetting->getVideoStreamChunkRate();
    m_bufferMapSize_chunk       = m_appSetting->getBufferMapSizeChunk();
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
    cDatarateChannel *channel = check_and_cast<cDatarateChannel *>
          (getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());

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

void CoolstreamingBase::bindToMeshModule(void){

    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
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
