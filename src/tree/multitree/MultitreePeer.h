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
// MultitreePeer.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Julian Wulfheide;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

#ifndef MULTITREEPEER_H_
#define MULTITREEPEER_H_ true

#include "MultitreeBase.h"
#include "PlayerBase.h"

class MultitreePeer : public MultitreeBase
{
public:
	MultitreePeer();
	virtual ~MultitreePeer();

protected:
    virtual void initialize(int stage);
    virtual void finish(void);

private:
	IPvXAddress *fallbackParent;
	int *stat_retrys;
	std::vector<simtime_t> beginConnecting;

	simsignal_t sig_numTrees;
	simsignal_t sig_outDegree;

	long firstSequenceNumber;

	bool *numSuccChanged;

	double param_retryLeave;
	double param_intervalReportStats;
	double param_intervalReconnect;
	double param_delaySuccessorInfo;
	double param_delayRetryConnect;

	virtual IPvXAddress getAlternativeNode(int stripe, IPvXAddress forNode, IPvXAddress currentParent, std::vector<IPvXAddress> lastRequests);

    virtual void processPacket(cPacket *pkt);
    void handleTimerMessage(cMessage *msg);

    void onNewChunk(int sequenceNumber);

    void startPlayer(void);

	void processConnectConfirm(cPacket* pkt);
	void processDisconnectRequest(cPacket *pkt);
	void processPassNodeRequest(cPacket *pkt);

	virtual void optimize(void);
	void leave(void);

	void disconnectFromParent(int stripe, IPvXAddress alternativeParent);

	virtual bool isPreferredStripe(unsigned int stripe);
	int getStripeToOptimize(void);

	void connectVia(IPvXAddress address, const std::vector<int> &stripes);

    void bindToGlobalModule(void);
    void bindToTreeModule(void);

	virtual void scheduleSuccessorInfo(int);

	void handleTimerJoin(void);
	void handleTimerLeave(void);
	void handleTimerSuccessorInfo(void);
	void handleTimerReportStatistic(void);
	void handleTimerConnect(void);

	void cancelAllTimer(void);
	void cancelAndDeleteTimer(void);

	int getGreatestReceivedSeqNumber(void);

    PlayerBase *m_player;

	cMessage *timer_getJoinTime;
	cMessage *timer_join;
	cMessage *timer_leave;
	cMessage *timer_successorInfo;
	cMessage *timer_reportStatistic;
	cMessage *timer_connect;

	long m_count_prev_chunkMiss;
	long m_count_prev_chunkHit;

	std::map<IPvXAddress, std::vector<int> > connectTo;


	virtual bool canAccept(ConnectRequest request);
};
#endif
