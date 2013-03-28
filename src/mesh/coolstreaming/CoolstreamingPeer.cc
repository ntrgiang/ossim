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
// CoolstreamingPeer.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief CoolstreamingPeer based on CoolstreamingBase
// @ingroup mesh
// @ingroup coolstreaming

#include "CoolstreamingPeer.h"

Define_Module(CoolstreamingPeer);

CoolstreamingPeer::CoolstreamingPeer()
{
    // TODO Auto-generated constructor stub

}

CoolstreamingPeer::~CoolstreamingPeer()
{
    if (timer_CheckParents) delete cancelEvent(timer_CheckParents);
}

void CoolstreamingPeer::initialize(int stage){

    if (stage != 3)
        return;

    initBase();

    stalemateDetection = new int[param_SubstreamCount];
    for (int i = 0; i < param_SubstreamCount; i++)
    {
        stalemateDetection[i] = 0;
    }

    // read Parameters
    param_minNOP = par("minNOP");
    if (param_minNOP > param_maxNOP) param_minNOP = param_maxNOP;
    param_coolstreaming_Ts = par("coolstreaming_Ts").longValue() * param_SubstreamCount; // multiply with substream count because of the internal counting
    param_coolstreaming_Tp = par("coolstreaming_Tp").longValue() * param_SubstreamCount;
    param_coolstreaming_Ta = par("coolstreaming_Ta");

    // create messages
    timer_CheckParents  = new cMessage("COOLSTREAMING_TIMER_CHECK_PARENTS");

    // schedule timers
    scheduleAt(simTime() + param_coolstreaming_Ta, timer_CheckParents);

}

void CoolstreamingPeer::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_CheckParents)
        checkParents();

    CoolstreamingBase::handleTimerMessage(msg);
}

void CoolstreamingPeer::checkPartners()
{
    EV << "CoolstreamingPeer::checkPartners()" << endl;

    removeTimeoutedPartners();

    EV << "CoolstreamingPeer::checkPartners()" << partners.size() << " _ " << param_minNOP << endl;

    if (partners.size() < param_minNOP){ // not enough partners?
        IPvXAddress newPartner;
        // query as many as we need ... (TODO: peers can get multiply requests ...)
        int count = (param_minNOP - partners.size());
        EV << "CoolstreamingPeer::checkPartners()->Queries: " << count << endl;
        for (int i = 0 ; i < min (10, count); i++)
        {
            //newPartner = m_Gossiper->getRandomPeer(m_localAddress);
            newPartner = m_memManager->getRandomPeer(m_localAddress);
            EV << "CoolstreamingPeer::checkPartners()->target" << newPartner.str() << endl;
            if (newPartner.isUnspecified())
                break;
            //MessageBoxA(0,"Test3","Test3",0);
            if (! isPartner(newPartner))
            {
                CoolstreamingPartnershipRequestPacket* pkt = new CoolstreamingPartnershipRequestPacket();
                sendToDispatcher(pkt, m_localPort, newPartner, m_destPort);
            }
        }
        //MessageBoxA(0,"need new partner! DONE","CoolstreamingPeer::checkPartners",0);
    }
    else
    { // stalemate detection, only if we have enough partners
        bool stalemate = false;
        int newMax;
        for (int s = 0; s < param_SubstreamCount; s++)
        {
            // get the latest chunk for this substream
            newMax = 0;
            for (unsigned int i = 0; i < partners.size(); i++)
            {
                newMax = max(newMax, partners.at(i)->getLatestSequence(s));
            }

            // if there was no change we assume a stalemate
            if (newMax == stalemateDetection[s])
                stalemate = true;
            stalemateDetection[s] = newMax;
        }

        // if a stalemate was detected, drop a random partner
        if (stalemate)
        {
            // select a random partner, 3 tries to find one who is not a parent
            int random;
            for (int i = 0; i < 3; i++)
            {
                random = (int)intrand(partners.size());
                if (!partners.at(random)->isParent())
                    break;
            }

            if (debugOutput)
                m_outFileDebug << simTime().str()
                               << " Stalemate ... removing: "
                               << partners.at(random)->getAddress().str() << endl;

            // send a revoke packet
            CoolstreamingPartnershipRevokePacket* resp = new CoolstreamingPartnershipRevokePacket();
            sendToDispatcher(resp, m_localPort, partners.at(random)->getAddress(), m_destPort);

            // remove partner
            removePartner(partners.at(random)->getAddress());
        }
    }

    scheduleAt(simTime() + param_CheckPartnersInterval, timer_CheckPartners);
}

void CoolstreamingPeer::checkParents()
{
    CoolstreamingPartner* parent;

    EV << "CoolstreamingPeer::checkParents() for " << m_localAddress.str() << endl;

    std::set<CoolstreamingPartner*> sendMaps;

    for (int Si = 0; Si < param_SubstreamCount; Si++)
    {  // check for each substream
        parent = getParent(Si);
        EV << "CoolstreamingPeer::checkParents()->" << Si << " _ " << ((parent == NULL)?"NULL":parent->getAddress().str()) << endl;
        // does the current partner mets the condition?
        if ( (parent == NULL) || (!satisfiesInequalitys(parent, Si)) )
        {
            std::vector<CoolstreamingPartner*> possiblePartners;
            std::vector<CoolstreamingPartner*>::iterator it;

            if (debugOutput)
                m_outFileDebug << simTime().str()
                               << " Parent " << ((parent == NULL)? " NULL ": parent->getAddress().str())
                               << " for substream " << (Si) << " NOT GOOD!" << endl;
            EV << "CoolstreamingPeer::checkParents()->Parent NOT GOOD :("  << endl;
            // look for possible partners satisfying the inequalities
            for (it = partners.begin(); it != partners.end(); it++)
                if ( satisfiesInequalityTwo(*it, Si) )
                        possiblePartners.push_back(*it);

            EV << "CoolstreamingPeer::checkParents()->Possible Parents: " << possiblePartners.size() << endl;
            if ( (possiblePartners.size() == 0) && ( parent == NULL ) )  // it seems like no one is good but we have no parent so select one at random
                for (it = partners.begin(); it != partners.end(); it++)
                    possiblePartners.push_back(*it);

            EV << "CoolstreamingPeer::checkParents()->Possible Parents-2: "
               << possiblePartners.size() << endl;
            if (possiblePartners.size() > 0)
            { // there is atleast 1 possible new partner for this substream
                // inform the old parent
                if (parent != NULL)
                {
                    parent->setParent(Si, false);
                    //sendBufferMap(parent);
                    sendMaps.insert(parent);
                }

                // select a random new parent and inform him
                int index;
                for (int i = 0; i < 3; i++)
                {    // try 3 times to find a partner who is not our child ...
                    index = (int)intrand(possiblePartners.size());
                    if (!possiblePartners.at(index)->isChild(Si))
                        break;
                }
                possiblePartners.at(index)->setParent(Si, true);

                EV << "CoolstreamingPeer::checkParents()->setStart: " << possiblePartners.at(index)->getLatestSequence(Si) << endl;
                if (m_videoBuffer->getBufferStartSeqNum() > possiblePartners.at(index)->getLatestSequence(Si))
                    m_videoBuffer->setBufferStartSeqNum(possiblePartners.at(index)->getLatestSequence(Si));

                if (debugOutput)
                    m_outFileDebug << simTime().str() << " Set new parent: " << (Si) << " _ " << m_videoBuffer->getBufferStartSeqNum() << " to " << possiblePartners.at(index)->getAddress().str() << endl;

                //sendBufferMap(possiblePartners.at(index));
                sendMaps.insert(possiblePartners.at(index));
            }
        }
    }

    EV << "CoolstreamingPeer::checkParents() sending Maps: " << sendMaps.size() << endl;
    std::set<CoolstreamingPartner*>::iterator it;
    for (it = sendMaps.begin(); it != sendMaps.end(); it++)
        sendBufferMap(*it);
//    sendMaps.clear();

    scheduleAt(simTime() + param_coolstreaming_Ta + (int)intrand(1), timer_CheckParents);
}

bool CoolstreamingPeer::satisfiesInequalitys(CoolstreamingPartner* partner, int substream)
{
    EV << "CoolstreamingPeer::satisfiesInequalitys() me, partner "
       << m_localAddress.str() << partner->getAddress().str() << endl;
    // inequality 1
    EV << "CoolstreamingPeer::satisfiesInequalitys() 1: "
       << getLatestSequenceNumber(substream) << " _ "
       << partner->getLatestSequence(substream) << " ? "
       << param_coolstreaming_Ts << endl;
    if ( partner->getLatestSequence(substream) - getLatestSequenceNumber(substream) >= param_coolstreaming_Ts)
        return false;


    int inequalTwo = 0;
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++)
    {
        inequalTwo = max (inequalTwo, (*it)->getLatestSequence(substream));
    }

    EV << "CoolstreamingPeer::satisfiesInequalitys() 2: "
       << inequalTwo << " _ " << partner->getLatestSequence(substream)
       << " ? " << param_coolstreaming_Tp << endl;
    if (inequalTwo - partner->getLatestSequence(substream) >= param_coolstreaming_Tp)
        return false;

    // substream parent is ok
    return true;
}

bool CoolstreamingPeer::satisfiesInequalityTwo(CoolstreamingPartner* partner, int substream)
{
    int inequalTwo = 0;
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++)
    {
        inequalTwo = max (inequalTwo, (*it)->getLatestSequence(substream));
    }

    if (inequalTwo - partner->getLatestSequence(substream) >= param_coolstreaming_Tp)
        return false;

    // substream parent is ok
    return true;
}
