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
// CoolstreamingPartner.cc
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

#include "CoolstreamingPartner.h"

CoolstreamingPartner::CoolstreamingPartner(int substreams)
{
    mSubstreams = substreams;
    hasSequence = new int[substreams];
    mIsChild     = new bool[substreams];
    mIsParent    = new bool[substreams];

    for (int i = 0; i < substreams; i++)
    {
        hasSequence[i] = 0;
        mIsChild[i]     = false;
        mIsParent[i]    = false;
    }

    deleted = false;
}

CoolstreamingPartner::~CoolstreamingPartner()
{
    // TODO Auto-generated destructor stub
}

void CoolstreamingPartner::updateFromMessage(CoolstreamingBufferMapPacket* pkt)
{
    for (unsigned int i = 0; i < pkt->getSequenceNumbersArraySize(); i++)
    {
        hasSequence[i] = pkt->getSequenceNumbers(i);
        EV << "sequence number: " << hasSequence[i] << endl;
    }
    for (unsigned int i = 0; i < pkt->getSubscribeArraySize(); i++)
    {
        mIsChild[i] = pkt->getSubscribe(i);
        EV << "is child: " << mIsChild[i] << endl;
    }
    updateLastSeen();

    if (deleted)
        //MessageBoxA(0,"DELETED","CoolstreamingPartner::updateFromMessag",0);
        throw cException("DELETED CoolstreamingPartner::updateFromMessag");
}

int CoolstreamingPartner::getLatestSequence(int substream)
{
    if (deleted)
            //MessageBoxA(0,"DELETED","CoolstreamingPartner::getLatestSequence",0);
            throw cException("DELETED CoolstreamingPartner::getLatestSequence");

    return hasSequence[substream];
}

void CoolstreamingPartner::setParent(int substream, bool val)
{
    if (deleted)
        //MessageBoxA(0,"DELETED","CoolstreamingPartner::setParent",0);
        throw cException("DELETED CoolstreamingPartner::setParent");
    mIsParent[substream] = val;
}


void CoolstreamingPartner::updateLastSeen()
{
    lastSeen = simTime();
}

void CoolstreamingPartner::setLastSeen(simtime_t seen)
{
    lastSeen = seen;
}
