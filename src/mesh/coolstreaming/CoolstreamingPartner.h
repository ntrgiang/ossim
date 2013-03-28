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
// CoolstreamingPartner.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


// @author Thorsten Jacobi
// @brief class for containing information of partners
// like address, lastSeen, sequence numbers, child- and parentship
// @ingroup mesh
// @ingroup coolstreaming

#include "IPvXAddress.h"
#include "CoolstreamingPacket_m.h"
#include "VideoChunkPacket_m.h"

#ifndef COOLSTREAMINGPARTNER_H_
#define COOLSTREAMINGPARTNER_H_

class CoolstreamingPartner
{
public:
    CoolstreamingPartner(int substreams);
    virtual ~CoolstreamingPartner();

    // @brief updates the data of this partner with the information of the buffermap
    void updateFromMessage(CoolstreamingBufferMapPacket* pkt);

    // @brief resets the last seen time to the current timestamp
    void updateLastSeen();
    // @brief sets last seen to a given timestamp
    void setLastSeen(simtime_t seen);

    // @brief returns if this partner is a child for the given substream
    bool isChild(int substream)
    {
        if (deleted)
            //MessageBoxA(0,"DELETED","CoolstreamingPartner::isChild",0);
            throw cException("DELETED CoolstreamingPartner::isChild");
        return mIsChild[substream];
    }

    // @brief returns if this partner is the parent of any substream
    bool isParent()
    {
        if (deleted)
            //MessageBoxA(0,"DELETED","CoolstreamingPartner::isParent",0);
            throw cException("DELETED CoolstreamingPartner::isParent");
        for (int i = 0; i < mSubstreams; i++)
            if (mIsParent[i])
                return true;
        return false;
    }

    // @brief returns if this partner is the parent of the given substream
    bool isParent(int substream)
    {
        if (deleted)
            //MessageBoxA(0,"DELETED","CoolstreamingPartner::isParent",0);
            throw cException("DELETED CoolstreamingPartner::isParent");
        return mIsParent[substream];
    }

    // @brief sets if this partner is a parent for the given substream
    void setParent(int substream, bool val);

    // @brief returns the current sequence number of the partner for a given substream
    int getLatestSequence(int substream);

    IPvXAddress getAddress(){if (deleted)
        //MessageBoxA(0,"DELETED","CoolstreamingPartner::getAddress",0);
        throw cException("DELETED CoolstreamingPartner::getAddress");
        return addr;}
    void setAddress(IPvXAddress address){addr = address;}
    simtime_t getLastSeen(){if (deleted)
        //MessageBoxA(0,"DELETED","CoolstreamingPartner::getLastSeen",0);
        throw cException("DELETED CoolstreamingPartner::getLastSeen");
        return lastSeen;}

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
