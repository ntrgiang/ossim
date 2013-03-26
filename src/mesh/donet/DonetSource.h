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
// DonetSource.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef __DONETSOURCE_H__
#define __DONETSOURCE_H__

#include "DonetBase.h"
#include "AppCommon.h"

// TODO-Giang if the peer doesn't respond during an interval, remove it from list
class DonetSource : public DonetBase
{
public:
    DonetSource();
	virtual ~DonetSource();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void processPacket(cPacket *pkt);
    void handleTimerMessage(cMessage *msg);
    void handleTimerPartnerlistCleanup(void);

private:
   void processPeerBufferMap(cPacket *pkt);

	// -- Partnership Management
//	void processPartnershipRequest(cPacket *pkt);
	void acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress, double bw);

	//bool canHaveMorePartner(void);

private:
    // -- Timer
    cMessage *timer_startVideo;
    cMessage *timer_newChunk;

    int totalBytesUploaded;
    int totalBytesDownloaded;

    // -- Signals
    simsignal_t sig_nPartner;

};

#endif

