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

#include "CoolstreamingPeer.h"

Define_Module(CoolstreamingPeer);

CoolstreamingPeer::CoolstreamingPeer() {
    // TODO Auto-generated constructor stub

}

CoolstreamingPeer::~CoolstreamingPeer() {
    if (timer_CheckParents) delete cancelEvent(timer_CheckParents);
}

void CoolstreamingPeer::initialize(int stage){
    if (stage != 3)
        return;

    initBase();

    // read Parameters
    param_minNOP = par("minNOP");
    param_coolstreaming_Ts = par("coolstreaming_Ts");
    param_coolstreaming_Tp = par("coolstreaming_Tp");
    param_coolstreaming_Ta = par("coolstreaming_Ta");

    // create messages
    timer_CheckParents  = new cMessage("COOLSTREAMING_TIMER_CHECK_PARENTS");

    // schedule timers
    scheduleAt(simTime() + param_coolstreaming_Ta, timer_CheckParents);

}

void CoolstreamingPeer::handleTimerMessage(cMessage *msg){
    if (msg == timer_CheckParents)
        checkParents();

    CoolstreamingBase::handleTimerMessage(msg);
}

void CoolstreamingPeer::checkPartners(){

    EV << "CoolstreamingPeer::checkPartners()" << endl;
    //MessageBoxA(0,"Test","Test",0);
    //MessageBoxA(0,"checkPartners","CoolstreamingPeer::checkPartners",0);
    removeTimeoutedPartners();
//    MessageBoxA(0,"Test1","Test1",0);
    EV << "CoolstreamingPeer::checkPartners()" << partners.size() << " _ " << param_minNOP << endl;
    //MessageBoxA(0,"checkPartners2","CoolstreamingPeer::checkPartners",0);

    if (partners.size() < param_minNOP){ // not enough partners?
        //MessageBoxA(0,"need new partner!","CoolstreamingPeer::checkPartners",0);
        IPvXAddress newPartner;
//        MessageBoxA(0,"Test2", "Test2",0);
        // query as many as we need ... (TODO: peers can get multiply requests ...)
        int count = (param_minNOP - partners.size());
        EV << "CoolstreamingPeer::checkPartners()->Queries: " << count << endl;
        for (int i = 0 ; i < min (10, count); i++){
            newPartner = m_Gossiper->getRandomPeer(m_localAddress);
            EV << "CoolstreamingPeer::checkPartners()->target" << newPartner.str() << endl;
            if (newPartner.isUnspecified())
                break;
            //MessageBoxA(0,"Test3","Test3",0);
            if (! isPartner(newPartner)){
                CoolstreamingPartnershipRequestPacket* pkt = new CoolstreamingPartnershipRequestPacket();
                sendToDispatcher(pkt, m_localPort, newPartner, m_destPort);
            }
        }
        //MessageBoxA(0,"need new partner! DONE","CoolstreamingPeer::checkPartners",0);
    }

    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
}

void CoolstreamingPeer::checkParents(){
    CoolstreamingPartner* parent;

    EV << "CoolstreamingPeer::checkParents() for " << m_localAddress.str() << endl;

    std::set<CoolstreamingPartner*> sendMaps;

    for (int Si = 0; Si < param_SubstreamCount; Si++){  // check for each substream
        parent = getParent(Si);
        EV << "CoolstreamingPeer::checkParents()->" << Si << " _ " << ((parent == NULL)?"NULL":parent->getAddress().str()) << endl;
        // does the current partner mets the condition?
        if ( (parent == NULL) || (!satisfiesInequalitys(parent, Si)) ){
            std::vector<CoolstreamingPartner*> possiblePartners;
            std::vector<CoolstreamingPartner*>::iterator it;

            m_outFileDebug << simTime().str() << " Parent for substream  " << (Si) << " NOT GOOD!" << endl;
            EV << "CoolstreamingPeer::checkParents()->Parent NOT GOOD :("  << endl;
            // look for possible partners satisfying the inequalities
            for (it = partners.begin(); it != partners.end(); it++)
                if ( satisfiesInequalitys(*it, Si) )
                        possiblePartners.push_back(*it);

            EV << "CoolstreamingPeer::checkParents()->Possible Parents: " << possiblePartners.size() << endl;
            if ( (possiblePartners.size() == 0) && ( parent == NULL ) )  // it seems like no one is good but we have no parent so select one at random
                for (it = partners.begin(); it != partners.end(); it++)
                    possiblePartners.push_back(*it);

            EV << "CoolstreamingPeer::checkParents()->Possible Parents-2: " << possiblePartners.size() << endl;
            if (possiblePartners.size() > 0){ // there is atleast 1 possible new partner for this substream
                // inform the old parent
                if (parent != NULL){
                    parent->setParent(Si, false);
                    //sendBufferMap(parent);
                    sendMaps.insert(parent);
                }

                // select a random new parent and inform him
                int index = (int)intrand(possiblePartners.size());
                possiblePartners.at(index)->setParent(Si, true);

                EV << "CoolstreamingPeer::checkParents()->setStart: " << possiblePartners.at(index)->getLatestSequence(Si) << endl;
                if (m_videoBuffer->getBufferStartSeqNum() > possiblePartners.at(index)->getLatestSequence(Si))
                    m_videoBuffer->setBufferStartSeqNum(possiblePartners.at(index)->getLatestSequence(Si));

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

    scheduleAt(simTime() + param_coolstreaming_Ta, timer_CheckParents);
}

bool CoolstreamingPeer::satisfiesInequalitys(CoolstreamingPartner* partner, int substream){
    EV << "CoolstreamingPeer::satisfiesInequalitys() me, partner " << m_localAddress.str() << partner->getAddress().str() << endl;
    // inequality 1
    EV << "CoolstreamingPeer::satisfiesInequalitys() 1: " << getLatestSequenceNumber(substream) << " _ " << partner->getLatestSequence(substream) << " ? " << param_coolstreaming_Ts << endl;
    if ( getLatestSequenceNumber(substream) - partner->getLatestSequence(substream) >= param_coolstreaming_Ts)
        return false;


    int inequalTwo = 0;
    std::vector<CoolstreamingPartner*>::iterator it;
    for (it = partners.begin(); it != partners.end(); it++){
        inequalTwo = max (inequalTwo, (*it)->getLatestSequence(substream));
    }

    EV << "CoolstreamingPeer::satisfiesInequalitys() 2: " << inequalTwo << " _ " << partner->getLatestSequence(substream) << " ? " << param_coolstreaming_Tp << endl;
    if (inequalTwo - partner->getLatestSequence(substream) >= param_coolstreaming_Tp)
        return false;

    // substream parent is ok
    return true;
}
