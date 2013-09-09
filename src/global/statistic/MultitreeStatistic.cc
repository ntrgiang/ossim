#include "MultitreeStatistic.h"

#include <algorithm>

Define_Module(MultitreeStatistic)

MultitreeStatistic::MultitreeStatistic(){}
MultitreeStatistic::~MultitreeStatistic(){}

void MultitreeStatistic::finish()
{
	char name[24];
	for (int i = 0; i < numStripes; i++)
	{
		sprintf(name, "treeHeight%d", i);
		recordScalar(name, treeHeights[i]);

		sprintf(name, "meanHopcountInTree%d", i);
		recordScalar(name, meanHopcounts[i]);

		sprintf(name, "meanOutDegree%d", i);
		recordScalar(name, meanOutDegree[i]);
	}

	delete[] oVNumTrees;
	delete[] oVMaxHopcount;
	delete[] oVMeanHopcount;
	delete[] oVOutDegree;
}

void MultitreeStatistic::initialize(int stage)
{
    if (stage == 0)
    {
        sig_minTreeHeight		  = registerSignal("Signal_Min_Tree_Height");
        sig_maxTreeHeight		  = registerSignal("Signal_Max_Tree_Height");
        sig_meanTreeHeight		  = registerSignal("Signal_Mean_Tree_Height");
        sig_medianTreeHeight	  = registerSignal("Signal_Median_Tree_Height");

        sig_chunkArrival		  = registerSignal("Signal_Chunk_Arrival");
        sig_packetLoss   		  = registerSignal("Signal_Packet_Loss");
        sig_numTrees              = registerSignal("Signal_Mean_Num_Trees");
        sig_BWUtil                = registerSignal("Signal_BW_Utilization");
        sig_connTime              = registerSignal("Signal_Connection_Time");
        sig_mean_retrys_per_stripe= registerSignal("Signal_Mean_Retrys_Per_Stripe");
        sig_mean_retrys_per_node  = registerSignal("Signal_Mean_Retrys_Per_Node");
        sig_max_retrys            = registerSignal("Signal_Max_Retrys");
        sig_total_retrys          = registerSignal("Signal_Total_Retrys");
        sig_meanOutDegree         = registerSignal("Signal_Mean_Out_Degree");
        sig_meanHopcount          = registerSignal("Signal_Mean_Hopcount");

        sig_forwardingNone        = registerSignal("Signal_Forwarding_In_None");
        sig_forwardingOne         = registerSignal("Signal_Forwarding_In_One");
        sig_forwardingMoreThanOne = registerSignal("Signal_Forwarding_In_More_Than_One");

        sig_messageCount          = registerSignal("Signal_Message_Count");
        sig_messageCountCR        = registerSignal("Signal_Message_Count_CR");
        sig_messageCountDR        = registerSignal("Signal_Message_Count_DR");
        sig_messageCountCC        = registerSignal("Signal_Message_Count_CC");
        sig_messageCountPNR       = registerSignal("Signal_Message_Count_PNR");
        sig_messageCountSI        = registerSignal("Signal_Message_Count_SI");
	}

    if (stage != 3)
        return;

	// -- Binding to Active Peer Table
    cModule *temp = simulation.getModuleByPath("activePeerTable");
    m_apTable = check_and_cast<ActivePeerTable *>(temp);
    EV << "Binding to activePeerTable is completed successfully" << endl;

    temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingMultitree *>(temp);
	numStripes = m_appSetting->getNumStripes();

	timer_reportGlobal = new cMessage("GLOBAL_STATISTIC_REPORT_GLOBAL");

	param_interval_reportGlobal = par("interval_reportGlobal");

	awakeNodes = 0;

	m_count_chunkHit = 0;
	m_count_allChunk = 0;
	m_count_chunkMiss = 0;

	messageCount = 0L;
	messageCountCR = 0L;
	messageCountDR = 0L;
	messageCountCC = 0L;
	messageCountPNR = 0L;
	messageCountSI = 0L;

	meanBWUtil = 0;
	meanConnectionTime = 0;
	meanNumTrees = 0;
	meanRetrysPerNode = 0;
	maxRetrys = 0;

	forwardingInNone = 0;
	forwardingInOne = 0;
	forwardingInMoreThanOne = 0;

	minTreeHeight = 0;
	maxTreeHeight = 0;
	meanTreeHeight = 0;
	medianTreeHeight = 0;

	oVNumTrees = new cOutVector[numStripes + 1];
	oVMaxHopcount = new cOutVector[numStripes];
	oVMeanHopcount = new cOutVector[numStripes];
	oVOutDegree = new cOutVector[numStripes];

	for (int i = 0; i < numStripes + 1; i++)
	{
		nodesForwardingInITrees.push_back(0);

		char name[24];
		sprintf(name, "nodesActiveIn%dstripes", i);
		oVNumTrees[i].setName(name);
	}

	for (int i = 0; i < numStripes; i++)
	{
		treeHeights.push_back(0);
		meanHopcounts.push_back(0);
		hopcounts.push_back(std::vector<int>());

		meanOutDegree.push_back(0);
		outDegreeSamples.push_back(std::map<IPvXAddress, int>());

		char name[24];
		sprintf(name, "maxHopcountTree%d", i);
		oVMaxHopcount[i].setName(name);

		sprintf(name, "meanHopcountTree%d", i);
		oVMeanHopcount[i].setName(name);

		sprintf(name, "outDegreeTree%d", i);
		oVOutDegree[i].setName(name);
	}

	WATCH(m_count_chunkHit);
	WATCH(m_count_chunkMiss);
	WATCH(m_count_allChunk);

	WATCH(overallOutDegree);
	WATCH(meanBWUtil);
	WATCH(meanConnectionTime);
	WATCH(meanNumTrees);
	WATCH(meanRetrysPerNode);
	WATCH(maxRetrys);
	WATCH(meanHopcount);

	WATCH(messageCount);
	WATCH(messageCountCR);
	WATCH(messageCountDR);
	WATCH(messageCountCC);
	WATCH(messageCountPNR);
	WATCH(messageCountSI);

	WATCH(forwardingInNone);
	WATCH(forwardingInOne);
	WATCH(forwardingInMoreThanOne);

	WATCH(awakeNodes);

	WATCH_VECTOR(nodesForwardingInITrees);
	WATCH_VECTOR(meanOutDegree);
	WATCH_VECTOR(treeHeights);
	WATCH_VECTOR(meanHopcounts);

	scheduleAt(simTime() + param_interval_reportGlobal, timer_reportGlobal);
}

void MultitreeStatistic::reportMessageCR(void)
{
	messageCount++;
	messageCountCR++;
}
void MultitreeStatistic::reportMessageDR(void)
{
	messageCount++;
	messageCountDR++;
}
void MultitreeStatistic::reportMessageCC(void)
{
	messageCount++;
	messageCountCC++;
}
void MultitreeStatistic::reportMessagePNR(void)
{
	messageCount++;
	messageCountPNR++;
}
void MultitreeStatistic::reportMessageSI(void)
{
	messageCount++;
	messageCountSI++;
}

void MultitreeStatistic::gatherOutDegree(const IPvXAddress node, int stripe, int degree)
{
	outDegreeSamples[stripe][node] = degree;
}

void MultitreeStatistic::gatherPreferredStripe(const IPvXAddress node, int stripe)
{
	preferredStripes[node] = stripe;
}

void MultitreeStatistic::gatherRetrys(int numRetrys)
{
	retrys.push_back(numRetrys);
}

void MultitreeStatistic::reportRetrys()
{
	int total = 0;
	for(std::vector<int>::iterator it = retrys.begin(); it != retrys.end(); ++it)
	{
		int numRetrys = (int)*it;
		total += numRetrys;

		if(numRetrys > maxRetrys)
			maxRetrys = numRetrys;
	}

	meanRetrysPerNode = (double)total / (double)m_apTable->getNumActivePeer();
	meanRetrysPerStripe = (double)total / (double)numStripes;

	emit(sig_mean_retrys_per_node, meanRetrysPerNode);
	emit(sig_mean_retrys_per_stripe, meanRetrysPerStripe);
	emit(sig_total_retrys, total);
	emit(sig_max_retrys, maxRetrys);
}

void MultitreeStatistic::reportAwakeNode(void)
{
	awakeNodes++;
}

void MultitreeStatistic::reportNodeLeft(void)
{
	joinedNodes--;
	awakeNodes--;
}

void MultitreeStatistic::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        throw cException("MultitreeStatistic doesn't process messages!");
    }
}

void MultitreeStatistic::handleTimerMessage(cMessage *msg)
{
	if(msg == timer_reportGlobal)
	{
		joinedNodes = m_apTable->getNumActivePeer();

		reportBWUtilization();
		reportPacketLoss();
		reportNumTreesForwarding();
		reportConnectionTime();
		reportRetrys();
		reportOutDegree();
		reportHopcounts();


		emit(sig_messageCount, messageCount);
		emit(sig_messageCountCR, messageCountCR);
		emit(sig_messageCountDR, messageCountDR);
		emit(sig_messageCountCC, messageCountCC);
		emit(sig_messageCountPNR, messageCountPNR);
		emit(sig_messageCountSI, messageCountSI);

		scheduleAt(simTime() + param_interval_reportGlobal, timer_reportGlobal);
	}
}

void MultitreeStatistic::reportBWUtilization()
{
	if(currentBWUtilization.size() > 0)
	{
		int totalCurNumCon = 0;
		int totalMaxNumCon = 0;

		for (std::map<IPvXAddress, int>::iterator it = currentBWUtilization.begin() ; it != currentBWUtilization.end(); ++it)
		{
			totalCurNumCon += it->second;
			totalMaxNumCon += maxBWUtilization[it->first];
		}

		meanBWUtil = (double)totalCurNumCon / (double)totalMaxNumCon;
		emit(sig_BWUtil, meanBWUtil);
	}
}

void MultitreeStatistic::gatherConnectionTime(int stripe, double time)
{
	connectionTimes.push_back(time);
}

void MultitreeStatistic::reportConnectionTime()
{
	if(connectionTimes.size() > 0)
	{
		double sum = 0;
		for(std::vector<double>::iterator it = connectionTimes.begin(); it != connectionTimes.end(); ++it)
		{
			sum += (double)*it;
		}
		meanConnectionTime = sum / (double)connectionTimes.size();
		emit(sig_connTime, meanConnectionTime);
	}
}

void MultitreeStatistic::reportOutDegree()
{
	int overallDegree = 0;

	for (int i = 0; i < numStripes; i++)
	{
		meanOutDegree[i] = 0;
		int sum = 0;
		int numNonZeros = 0;

		for (std::map<IPvXAddress, int>::iterator it = outDegreeSamples[i].begin() ; it != outDegreeSamples[i].end(); ++it)
		{
			int degree = it->second;
			if(degree > 0)
			{
				overallDegree += degree;
				sum += degree;
				numNonZeros++;
			}
		}

		if(sum > 0)
		{
			meanOutDegree[i] = (double)sum / (double)numNonZeros;
			oVOutDegree[i].record(meanOutDegree[i]);
		}
	}

	overallOutDegree = (double)overallDegree / (double)outDegreeSamples[0].size();
	if(overallOutDegree > 0)
		emit(sig_meanOutDegree, overallOutDegree);
}

void MultitreeStatistic::gatherBWUtilization(const IPvXAddress node, int curNumConn, int maxNumConn)
{
	currentBWUtilization[node] = curNumConn;
	maxBWUtilization[node] = maxNumConn;
}

void MultitreeStatistic::reportNumTreesForwarding()
{
	//EV << totalNumTreesForwarding.size() << " nodes have reported their number of active trees." << endl;
	int num[numStripes + 1];
	for (int i = 0; i < numStripes + 1; i++)
	{
		num[i] = 0;
	}

	if(totalNumTreesForwarding.size() > 0)
	{
		int totalTrees = 0;
		forwardingInNone = 0;
		forwardingInOne = 0;
		forwardingInMoreThanOne = 0;

		for (std::map<IPvXAddress, int>::iterator it = totalNumTreesForwarding.begin() ; it != totalNumTreesForwarding.end(); ++it)
		{
			totalTrees += it->second;
			num[it->second]++;
		}

		meanNumTrees = (double)totalTrees / (double)totalNumTreesForwarding.size();

		for (int i = 0; i < numStripes + 1; i++)
		{
			nodesForwardingInITrees[i] = num[i];
			oVNumTrees[i].record(num[i]);

			if(i > 1)
				forwardingInMoreThanOne += num[i];
		}

		forwardingInMoreThanOne /= (double)totalNumTreesForwarding.size();
		forwardingInOne = num[1] / (double)totalNumTreesForwarding.size();
		forwardingInNone = num[0] / (double)totalNumTreesForwarding.size();
		
		emit(sig_forwardingNone, forwardingInNone);
		emit(sig_forwardingOne, forwardingInOne);
		emit(sig_forwardingMoreThanOne, forwardingInMoreThanOne);

		emit(sig_numTrees, meanNumTrees);
	}

}

void MultitreeStatistic::gatherNumTreesForwarding(const IPvXAddress node, int numTrees)
{
	totalNumTreesForwarding[node] = numTrees;
}

void MultitreeStatistic::reportPacketLoss()
{
	emit(sig_packetLoss, (double)m_count_chunkMiss / (double)m_count_allChunk);
}

void MultitreeStatistic::reportChunkArrival(int stripe, int hopcount)
{
	hopcounts[stripe].push_back(hopcount);

    emit(sig_chunkArrival, hopcount);
}

void MultitreeStatistic::increaseChunkHit(const int &delta)
{  
	m_count_chunkHit += delta;
	m_count_allChunk += delta;
}

void MultitreeStatistic::increaseChunkMiss(const int &delta)
{
	m_count_chunkMiss += delta;
	m_count_allChunk += delta;
}

void MultitreeStatistic::receiveChangeNotification(int category, const cPolymorphic *details)
{
	return;
}

void MultitreeStatistic::reportHopcounts()
{
	int total = 0;
	int totalSamples = 0;
	int sumTreeHeights = 0;

	for (int i = 0; i < numStripes; ++i)
	{
		int max = 0;
		int totalPerTree = 0;

		for(std::vector<int>::iterator it = hopcounts[i].begin(); it != hopcounts[i].end(); ++it)
		{
			int currentHopcount = (int)*it;
			if(currentHopcount > max)
				max = currentHopcount;

			totalPerTree += currentHopcount;
			totalSamples++;
		}

		total += totalPerTree;

		if(max > 0)
		{
			oVMaxHopcount[i].record(max);
			treeHeights[i] = max;
		}

		double mean = (double)totalPerTree / (double)hopcounts[i].size();
		if(mean > 0)
		{
			oVMeanHopcount[i].record(mean);
			meanHopcounts[i] = mean;
		}

		sumTreeHeights += treeHeights[i];

		hopcounts[i].clear();
	}

	// Sort the tree heights in order to get the median
	int sortedTreeHeights[numStripes];
	std::copy(treeHeights.begin(), treeHeights.end(), sortedTreeHeights);
	std::sort(sortedTreeHeights, sortedTreeHeights + numStripes);

	minTreeHeight = *std::min_element(treeHeights.begin(), treeHeights.end());
	maxTreeHeight = *std::max_element(treeHeights.begin(), treeHeights.end());
	meanTreeHeight = (double)sumTreeHeights / (double)numStripes;
	if(numStripes % 2 == 0)
		medianTreeHeight = sortedTreeHeights[numStripes / 2];
	else
		medianTreeHeight = sortedTreeHeights[(numStripes + 1) / 2];

	emit(sig_minTreeHeight, minTreeHeight);
	emit(sig_maxTreeHeight, maxTreeHeight);
	emit(sig_meanTreeHeight, meanTreeHeight);
	emit(sig_medianTreeHeight, medianTreeHeight);

	for (int i = 0; i < numStripes; ++i)
	{
	}

	if(totalSamples > 0 && total > 0)
	{
		meanHopcount = total / (double)totalSamples;
		emit(sig_meanHopcount, meanHopcount);
	}
}
