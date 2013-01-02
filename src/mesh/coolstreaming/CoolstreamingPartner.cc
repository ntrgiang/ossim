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

#include "CoolstreamingPartner.h"

CoolstreamingPartner::CoolstreamingPartner(int substreams) {
    mSubstreams = substreams;
    hasSequence = new int[substreams];
    mIsChild     = new bool[substreams];
    mIsParent    = new bool[substreams];

    for (int i = 0; i < substreams; i++){
        hasSequence[i] = 0;
        mIsChild[i]     = false;
        mIsParent[i]    = false;
    }
}

CoolstreamingPartner::~CoolstreamingPartner() {
    // TODO Auto-generated destructor stub
}

void CoolstreamingPartner::updateFromMessage(CoolstreamingBufferMapPacket* pkt){
    for (unsigned int i = 0; i < pkt->getSequenceNumbersArraySize(); i++)
        hasSequence[i] = pkt->getSequenceNumbers(i);
    for (unsigned int i = 0; i < pkt->getSubscribeArraySize(); i++)
        mIsChild[i] = pkt->getSubscribe(i);
    updateLastSeen();
}
