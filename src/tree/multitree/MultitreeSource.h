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
// MultitreeSource.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Julian Wulfheide;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

#ifndef MULTITREESOURCE_H_
#define MULTITREESOURCE_H_ true

#include "MultitreeBase.h"

class MultitreeSource : public MultitreeBase
{
public:
	MultitreeSource();
	virtual ~MultitreeSource();

protected:
    virtual void initialize(int stage);
    virtual void finish(void);

private:
	virtual IPvXAddress getAlternativeNode(int stripe, IPvXAddress forNode, IPvXAddress currentParent, std::vector<IPvXAddress> lastRequests);

	virtual void optimize(void);

    virtual void processPacket(cPacket *pkt);
    void handleTimerMessage(cMessage *msg);

    void onNewChunk(int sequenceNumber);

	void processDisconnectRequest(cPacket *pkt);

	virtual void scheduleSuccessorInfo(int stripe);

	virtual bool isPreferredStripe(unsigned int stripe);
	
    void bindToGlobalModule(void);
    void bindToTreeModule(void);

	void cancelAllTimer(void);
	void cancelAndDeleteTimer(void);

	virtual bool canAccept(ConnectRequest request);

};

#endif
