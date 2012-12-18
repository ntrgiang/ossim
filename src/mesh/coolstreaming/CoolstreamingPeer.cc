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

CoolstreamingPeer::CoolstreamingPeer() {
    // TODO Auto-generated constructor stub

}

CoolstreamingPeer::~CoolstreamingPeer() {
    // TODO Auto-generated destructor stub
    if (timer_CheckPartners) delete cancelEvent(timer_CheckPartners);
}

void CoolstreamingPeer::initialize(int stage){
    if (stage != 3)
        return;

    initBase();


}

void CoolstreamingPeer::handleTimerMessage(cMessage *msg){
    if (msg == timer_CheckPartners)
        checkPartners();
}

void CoolstreamingPeer::checkPartners(){

    removeTimeoutedPartners();

    if (partners.size() < param_minNOP){ // send a new request
        IPvXAddress newPartner;

        for (int i = 0 ; i < 10; i++){
            newPartner = m_Gossiper->getRandomPeer(m_localAddress);
            if (! isPartner(newPartner)){
                CoolstreamingPartnershipRequestPacket* pkt = new CoolstreamingPartnershipRequestPacket();
                sendToDispatcher(pkt, m_localPort, newPartner, m_destPort);
            }
        }
    }

    scheduleAt(simTime() + param_CheckPartnersIntervall, timer_CheckPartners);
}
