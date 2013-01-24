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

#include "IPvXAddress.h"
#include "CoolstreamingPacket_m.h"
#include "VideoChunkPacket_m.h"

#ifndef COOLSTREAMINGPARTNER_H_
#define COOLSTREAMINGPARTNER_H_

class CoolstreamingPartner {
public:
    CoolstreamingPartner(int substreams);
    virtual ~CoolstreamingPartner();

    void updateFromMessage(CoolstreamingBufferMapPacket* pkt);

    void updateLastSeen();

    bool isChild(int substream){
        if (deleted)
            MessageBoxA(0,"DELETED","CoolstreamingPartner::isChild",0);
        return mIsChild[substream];
    }
    bool isParent(int substream){
        if (deleted)
            MessageBoxA(0,"DELETED","CoolstreamingPartner::isParent",0);
        return mIsParent[substream];
    }

    void setParent(int substream, bool val);

    int getLatestSequence(int substream);

    IPvXAddress getAddress(){if (deleted)
        MessageBoxA(0,"DELETED","CoolstreamingPartner::getAddress",0);return addr;}
    void setAddress(IPvXAddress address){addr = address;}
    simtime_t getLastSeen(){if (deleted)
        MessageBoxA(0,"DELETED","CoolstreamingPartner::getLastSeen",0);return lastSeen;}
    void setLastSeen(simtime_t seen);

    bool deleted;
private:
    int         mSubstreams;

    bool*       mIsParent;
    bool*       mIsChild;

    IPvXAddress addr;
    int*        hasSequence;
    simtime_t   lastSeen;
};

#endif /* COOLSTREAMINGPARTNER_H_ */
