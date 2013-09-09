#include "IChurnGenerator.h"
#include "NotificationBoard.h"
#include "AppCommon.h"
#include "ActivePeerTable.h"
#include "AppSettingMultitree.h"
#include "IPvXAddress.h"
#include <fstream>
#include "StatisticBase.h"
#include "StreamingStatistic.h"

#ifndef MULTITREE_STATISTIC_H_
#define MULTITREE_STATISTIC_H_

class ActivePeerTable;

class MultitreeStatistic : public StreamingStatistic
{
public:
    MultitreeStatistic();
    virtual ~MultitreeStatistic();

    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);

	void increaseChunkHit(const int &delta);
	void increaseChunkMiss(const int &delta);

	virtual void receiveChangeNotification(int category, const cPolymorphic *details);

	void reportAwakeNode(void);
	void reportNodeLeft(void);

	void reportMessageCR(void);
	void reportMessageDR(void);
	void reportMessageCC(void);
	void reportMessagePNR(void);
	void reportMessageSI(void);

	void reportChunkArrival(int stripe, int hopcount);

	void gatherPreferredStripe(const IPvXAddress node, int stripe);
	void gatherBWUtilization(const IPvXAddress node, int curNumConn, int maxNumConn);
	void gatherNumTreesForwarding(const IPvXAddress node, int numTrees);
	void gatherOutDegree(const IPvXAddress node, int stripe, int degree);
	void gatherConnectionTime(int stripe, double time);
	void gatherRetrys(int retrys);

private:
	cMessage *timer_reportGlobal;
	double param_interval_reportGlobal;

	void handleTimerMessage(cMessage *msg);

	void reportOutDegree();
	void reportBWUtilization();
	void reportPacketLoss();
	void reportNumTreesForwarding();
	void reportConnectionTime();
	void reportRetrys();
	void reportHopcounts();

	ActivePeerTable			*m_apTable;
    AppSettingMultitree   	*m_appSetting;

	simsignal_t sig_maxTreeHeight;
	simsignal_t sig_minTreeHeight;
	simsignal_t sig_meanTreeHeight;
	simsignal_t sig_medianTreeHeight;

	simsignal_t sig_messageCount;
	simsignal_t sig_messageCountCR;
	simsignal_t sig_messageCountDR;
	simsignal_t sig_messageCountCC;
	simsignal_t sig_messageCountPNR;
	simsignal_t sig_messageCountSI;

	simsignal_t sig_forwardingNone;
	simsignal_t sig_forwardingOne;
	simsignal_t sig_forwardingMoreThanOne;

	simsignal_t sig_BWUtil;
	simsignal_t sig_packetLoss;
	simsignal_t sig_chunkArrival;
	simsignal_t sig_numTrees;
	simsignal_t sig_connTime;
	simsignal_t sig_max_retrys;
	simsignal_t sig_total_retrys;
	simsignal_t sig_mean_retrys_per_node;
	simsignal_t sig_mean_retrys_per_stripe;
	simsignal_t sig_meanOutDegree;
	simsignal_t sig_meanHopcount;

	std::vector<std::map<IPvXAddress, int> > outDegreeSamples;
	std::vector<double> meanOutDegree;

	std::map<IPvXAddress, int> preferredStripes;
	std::map<IPvXAddress, int> currentBWUtilization;
	std::map<IPvXAddress, int> maxBWUtilization;
	std::map<IPvXAddress, int> totalNumTreesForwarding;
	std::vector<double> connectionTimes;
	std::vector<int> retrys;
	std::vector<int> nodesForwardingInITrees;

	std::vector<int> treeHeights;
	std::vector<double> meanHopcounts;
	std::vector<std::vector<int> > hopcounts;
	double meanHopcount;
	cOutVector *oVMaxHopcount;
	cOutVector *oVMeanHopcount;

	cOutVector *oVNumTrees;
	cOutVector *oVOutDegree;

	int numStripes;

	int joinedNodes;
	int awakeNodes;

	double forwardingInNone;
	double forwardingInOne;
	double forwardingInMoreThanOne;

	unsigned long m_count_chunkHit;
	unsigned long m_count_chunkMiss;
	unsigned long m_count_allChunk;

	unsigned long messageCount;
	unsigned long messageCountCR;
	unsigned long messageCountDR;
	unsigned long messageCountCC;
	unsigned long messageCountPNR;
	unsigned long messageCountSI;

	int maxRetrys;
	unsigned long totalRetrys;
	double meanRetrysPerNode;
	double meanRetrysPerStripe;
	double overallOutDegree;
	double meanBWUtil;
	double meanConnectionTime;
	double meanNumTrees;

	int minTreeHeight;
	int maxTreeHeight;
	double meanTreeHeight;
	int medianTreeHeight;

};

#endif /* MULTITREE_STATISTIC_H_ */
