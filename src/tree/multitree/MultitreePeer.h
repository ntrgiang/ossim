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
