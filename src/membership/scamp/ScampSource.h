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
// ScampSource.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef __GOSSIP_VIDEO_SOURCE_H__
#define __GOSSIP_VIDEO_SOURCE_H__

#include "ScampBase.h"

// TODO if the peer don't respond during an interval, remove it from list
class ScampSource : public ScampBase
{
public:
//	friend class PeerConnectionThread;
    ScampSource();
	virtual ~ScampSource();

protected:
//	virtual void initialize();
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);
    virtual void finish();

// Helper functions
private:
	//! Print a debug message to the passed ostream, which defaults to clog.
	//   void printDebugMsg(std::string s);
//	virtual void processPacket(cPacket *pkt);
	void handleTimerMessage(cMessage *msg);

// Member data
private:

    SEQUENCE_NUMBER_T m_id_newChunk;

};

#endif
