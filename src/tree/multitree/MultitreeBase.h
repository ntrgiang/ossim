#ifndef MULTITREEBASE_H_
#define MULTITREEBASE_H_ true

#include "CommBase.h"
#include "AppCommon.h"
#include "AppSettingMultitree.h"
#include "ActivePeerTable.h"
#include "DonetStatistic.h"
#include "Forwarder.h"
#include "MultitreePartnerList.h"
#include "MultitreeStatistic.h"
#include "VideoBuffer.h"
#include "VideoBufferListener.h"

#include "TreePeerStreamingPacket_m.h"

#include <MultitreeMessages.h>

enum TreeJoinState
{
    TREE_JOIN_STATE_IDLE            = 0, // Not joined
    TREE_JOIN_STATE_IDLE_WAITING    = 1, // Not joined but requested
    TREE_JOIN_STATE_ACTIVE          = 2, // Joined/Active
    TREE_JOIN_STATE_LEAVING         = 3  // Leaving
};

typedef std::map<IPvXAddress, std::vector<int> > successorList;

struct ChildCost
{
    int stripe;
	double cost;
    IPvXAddress child;

    ChildCost(int stripe, double cost, const IPvXAddress &child) : stripe(stripe), cost(cost), child(child) {}

    bool operator < (const ChildCost& other) const
    {
        return (cost > other.cost);
    }

	bool operator == (const ChildCost& other) const
    {
        return cost == other.cost && stripe == other.stripe && child.equals(other.child);
    }
};

class MultitreeBase : public CommBase, public VideoBufferListener
{
public:
	MultitreeBase();
	virtual ~MultitreeBase();

	virtual void initialize(int stage);
	virtual void finish(void);

    virtual void handleMessage(cMessage *msg);
    virtual void processPacket(cPacket *pkt) = 0;
    virtual void handleTimerMessage(cMessage *msg) = 0;

	virtual int numInitStages() const { return 4; }

protected:
    MultitreePartnerList    *m_partnerList;
    Forwarder				*m_forwarder;
    AppSettingMultitree   	*m_appSetting;
    VideoBuffer				*m_videoBuffer;
    MultitreeStatistic 		*m_gstat;

	std::map<int, std::set<IPvXAddress> > disconnectingChildren;
	std::map<int, std::vector<IPvXAddress> > requestedChildship;

    TreeJoinState *m_state;

    int m_localPort, m_destPort;

	unsigned int numStripes;

	/*
	 * The bandwidth capacity of this node. A bandwidth capacity of 1 means,
	 * this node is capable of delivering or (!) receiving the whole stream
	 * once at a time.
	 */
	double bwCapacity;

	long *lastSeqNumber;

	int numCR;
	int numDR;
	int numCC;
	int numPNR;
	int numSI;

	bool hasBWLeft(int additionalConnections);

    void bindToGlobalModule(void);
    void bindToTreeModule(void);
    void bindToStatisticModule(void);

	void getSender(cPacket *pkt, IPvXAddress &senderAddress, int &senderPort);
	void getSender(cPacket *pkt, IPvXAddress &senderAddress);
	const IPvXAddress& getSender(const cPacket *pkt) const;

    virtual void onNewChunk(int sequenceNumber) = 0;

	void processSuccessorUpdate(cPacket *pkt);
	void processConnectRequest(cPacket *pkt);
	void removeChild(int stripe, IPvXAddress address); 

	void scheduleOptimization(void);

	void printStatus(void);

	void dropNode(int stripe, IPvXAddress address, IPvXAddress alternativeParent); 

	virtual IPvXAddress getAlternativeNode(int stripe, IPvXAddress forNode, IPvXAddress currentParent, std::vector<IPvXAddress> lastRequests) = 0;

    void handleTimerOptimization();
	cMessage *timer_optimization;

	bool isDisconnecting(int stripe, IPvXAddress child);

    int getMaxOutConnections(void);

	// Optimization functions
	double getCosts(int stripe, IPvXAddress child);
	double getGain(int stripe, IPvXAddress child);
	double getGain(int stripe, IPvXAddress child, IPvXAddress *linkToDrop);
	double getGainThreshold(void);
	double gainThreshold;

	void getCostliestChild(int &stripe, IPvXAddress &address);
	void getCheapestChild(int stripe, IPvXAddress &address, IPvXAddress skipAddress);

	virtual void optimize(void) = 0;

	double param_weightT;
	double param_weightK1;
    double param_weightK2;
	double param_weightK3;
	double param_weightK4;

    int getForwardingCosts(int stripe, IPvXAddress child); // K_forw, K_2

	virtual bool canAccept(ConnectRequest request) = 0;
	bool haveMoreChildrenInOtherStripe(unsigned int stripe);

	int getNumDisconnectChildren(void);

private:
	bool param_optimize;
	bool param_sendMissingChunks;

	void cancelAndDeleteTimer(void);

	double getBWCapacityFromChannel(void);

	void getAppSetting(void);
	void acceptConnectRequests(const std::vector<ConnectRequest> &requests, IPvXAddress address);
	void rejectConnectRequests(const std::vector<ConnectRequest> &requests, IPvXAddress address);

    virtual void scheduleSuccessorInfo(int stripe) = 0;

	void sendChunksToNewChild(int stripe, IPvXAddress address, int lastChunk);

	virtual bool isPreferredStripe(unsigned int stripe) = 0;

	double getStripeDensityCosts(unsigned int stripe); // K_sel, K_1
    double getBalanceCosts(unsigned int stripe, IPvXAddress child, IPvXAddress *linkToDrop); //K_bal, K_3
    double getDependencyCosts(IPvXAddress child); //K_4

	double param_delayOptimization;

};

#endif
