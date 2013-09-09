#include <cenvir.h>

#include "limits.h"
#include "MultitreeBase.h"
#include "DpControlInfo_m.h"

#include <algorithm> 

MultitreeBase::MultitreeBase(){
}

MultitreeBase::~MultitreeBase(){
}

void MultitreeBase::initialize(int stage)
{
	if(stage == 3)
	{
		bindToGlobalModule();
		bindToTreeModule();
		bindToStatisticModule();

		getAppSetting();
		findNodeAddress();

		m_videoBuffer->addListener(this);

		// -------------------------------------------------------------------------
		// -------------------------------- Timers ---------------------------------
		// -------------------------------------------------------------------------
		// -- One-time timers
		
		// -- Repeated timers
		timer_optimization = new cMessage("TIMER_OPTIMIZATION");

		m_state = new TreeJoinState[numStripes];

		lastSeqNumber = new long[numStripes];
		for (size_t i = 0; i < numStripes; i++)
		{
			lastSeqNumber[i] = -1L;
		}

		//bwCapacity = getBWCapacityFromChannel();
		bwCapacity = par("bwCapacity");
		param_delayOptimization = par("delayOptimization");
		param_optimize = par("optimize");

		param_sendMissingChunks = par("sendMissingChunks");

		param_weightT = par("weightT");
		param_weightK1 = par("weightK1");
		param_weightK2 = par("weightK2");
		param_weightK3 = par("weightK3");
		param_weightK4 = par("weightK4");

		numCR = 0;
		numDR = 0;
		numCC = 0;
		numPNR = 0;
		numSI = 0;
		WATCH(numCR);
		WATCH(numDR);
		WATCH(numCC);
		WATCH(numPNR);
		WATCH(numSI);
	}
}

void MultitreeBase::finish(void)
{
	cancelAndDeleteTimer();

	m_partnerList->clear();
	delete[] m_state;
}

void MultitreeBase::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        processPacket(PK(msg));
    }
}

void MultitreeBase::handleTimerOptimization(void)
{
	optimize();
}

void MultitreeBase::getAppSetting(void)
{
	m_localPort = getLocalPort();
	m_destPort = getDestPort();

	numStripes = m_appSetting->getNumStripes();
}

void MultitreeBase::bindToGlobalModule(void)
{
    CommBase::bindToGlobalModule();

    cModule *temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingMultitree *>(temp);

	// -- Churn
    temp = simulation.getModuleByPath("churnModerator");
    m_churn = check_and_cast<IChurnGenerator *>(temp);
    EV << "Binding to churnModerator is completed successfully" << endl;
}

void MultitreeBase::bindToTreeModule(void)
{
    cModule *temp = getParentModule()->getModuleByRelativePath("forwarder");
    m_forwarder = check_and_cast<Forwarder *>(temp);

    temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);

    temp = getParentModule()->getModuleByRelativePath("partnerList");
    m_partnerList = check_and_cast<MultitreePartnerList *>(temp);
}

void MultitreeBase::bindToStatisticModule(void)
{
    cModule *temp = simulation.getModuleByPath("globalStatistic");
	m_gstat = check_and_cast<MultitreeStatistic *>(temp);
}

void MultitreeBase::processConnectRequest(cPacket *pkt)
{
	bool allIdle = true;
	for (size_t i = 0; i < numStripes; i++)
	{
		if(m_state[i] != TREE_JOIN_STATE_IDLE)
		{
			allIdle = false;
			break;
		}
	}
	if(allIdle)
		return;

	IPvXAddress senderAddress;
    getSender(pkt, senderAddress);

	TreeConnectRequestPacket *treePkt = check_and_cast<TreeConnectRequestPacket *>(pkt);
	std::vector<ConnectRequest> requests = treePkt->getRequests();
	std::random_shuffle(requests.begin(), requests.end(), intrand);

	std::vector<ConnectRequest> accept;
	std::vector<ConnectRequest> reject;

	bool doOptimize = false;

	bool onlyPreferredStripes = true;
	// 2 runs: 1 for the preferred stripes, 1 for the remaining
	
	for (int i = 0; i < 2; i++)
	{
		for(std::vector<ConnectRequest>::const_iterator it = requests.begin(); it != requests.end(); ++it)
		{
			const ConnectRequest request = (ConnectRequest)*it;
			int stripe = request.stripe;

			if( (onlyPreferredStripes && !isPreferredStripe(stripe))
					|| (!onlyPreferredStripes && isPreferredStripe(stripe)) )
				continue;

			std::set<IPvXAddress> &myChildren = m_partnerList->getChildren(stripe);

			std::set<IPvXAddress>::iterator itCurParent = 
				std::find(myChildren.begin(), myChildren.end(), request.currentParent);

			int numSucc = request.numSuccessors[stripe];

			if(m_state[stripe] == TREE_JOIN_STATE_LEAVING)
			{
				EV << "Received ConnectRequest (stripe " << stripe << ") while leaving. Rejecting..." << endl;
				reject.push_back(request);
			}
			else if( (m_state[stripe] == TREE_JOIN_STATE_IDLE && m_partnerList->getParent(stripe).isUnspecified()) )
			{
				EV << "Received ConnectRequest (stripe " << stripe
					<< ") in (not yet or not anymore) unconnected stripe. Rejecting..." << endl;
				reject.push_back(request);
			}
			else if( m_partnerList->hasParent(stripe, senderAddress) )
			{
				EV << "Received ConnectRequest from parent " << senderAddress << " for stripe " << stripe 
					<< ". Rejecting..." << endl;
				reject.push_back(request);
			}
			else if( m_partnerList->hasChild(stripe, senderAddress) )
			{
				EV << "Received ConnectRequest from child (" << senderAddress << ", stripe " << stripe
					<< "). Tell him he can stay..." << endl;
				accept.push_back(request);

				// This is most likely a "failed" optimization-drop. That means I already updated
				// the number of successors of the parent I dropped the node to. That should be
				// reverted.

				std::set<IPvXAddress>::iterator itChild = disconnectingChildren[stripe].find(senderAddress);
				if(itChild != disconnectingChildren[stripe].end())
					disconnectingChildren[stripe].erase(disconnectingChildren[stripe].find(senderAddress));

				if(request.lastRequests.size() > 0)
				{
					IPvXAddress droppedTo = (IPvXAddress)*request.lastRequests.begin();
					int succDroppedTo = m_partnerList->getNumChildsSuccessors(stripe, droppedTo);
					int newSucc = succDroppedTo - 1 - numSucc;
					if(newSucc < 0)
						// The childs successor might be bigger, if it accepted a new child while
						// being dropped
						newSucc = 0;
					m_partnerList->updateNumChildsSuccessors(stripe, droppedTo, newSucc);
				}
				else
				{
					// This happens when I give an invalid alternative parent. The node will
					// reconnect to me (wihthout having tried other peers, hence lastRequests.size()
					// == 0). I cannot "recreate" the numbers of successors now, because I don't
					// know which node I dropped to.
				}

			}
			else if( !requestedChildship.empty() && senderAddress.equals(requestedChildship[stripe].back()) )
			{
				EV << "Received ConnectRequest from a node (" << senderAddress<< ") that I requested "
					<< "childship from for stripe " << stripe << ". Rejecting..." << endl;
				reject.push_back(request);
			}
			else if(itCurParent != myChildren.end())
			{

				EV << "Received ConnectRequest from a childs child (" << senderAddress<< ", stripe "
					<< stripe << ")." << endl;

				// One of my children is the nodes parent -> this must be a PNR or the parent wants
				// to leave

				accept.push_back(request);
				m_partnerList->addChild(stripe, senderAddress, numSucc);

				if(!onlyPreferredStripes)
				{
					doOptimize = true;
				}

				//EV << "New child has " << numSucc << " succ." << endl;

				// The nodes old parent is one of my children. So I can already
				// update my partnerlist (that child now has a successors less)
				IPvXAddress oldParent = (IPvXAddress)*itCurParent;
				int currentSucc = m_partnerList->getNumChildsSuccessors(stripe, (IPvXAddress)*itCurParent);
				int newSucc = currentSucc - (1 + numSucc);
				if(newSucc < 0)
					// The child node probably accepted a child, but the SuccessorsInfo didn't came
					// all the way up to me.
					newSucc = 0;
				m_partnerList->updateNumChildsSuccessors(stripe, oldParent, newSucc);

			}
			else if(hasBWLeft(1))
			{

				if(canAccept(request))
				{
					accept.push_back(request);
					m_partnerList->addChild(stripe, senderAddress, numSucc);

					if(!onlyPreferredStripes)
					{
						doOptimize = true;
					}

					scheduleSuccessorInfo(stripe);
				}
				else
				{
					reject.push_back(request);
				}

			}
			else
			{
				EV << "Received ConnectRequest from " << senderAddress << " (stripe " << stripe
				<< ") but have no bandwidth left. Rejecting..." << endl;

				reject.push_back(request);
				doOptimize  = true;
			}
		}

		onlyPreferredStripes = false;
	}

	if(!accept.empty())
	{
		acceptConnectRequests(accept, senderAddress);
		printStatus();
	}
	if(!reject.empty())
	{
		rejectConnectRequests(reject, senderAddress);
	}

	if(doOptimize)
	{
		scheduleOptimization();
	}
}

void MultitreeBase::rejectConnectRequests(const std::vector<ConnectRequest> &requests, IPvXAddress address)
{
	int numReqStripes = requests.size();

	TreeDisconnectRequestPacket *pkt = new TreeDisconnectRequestPacket("TREE_DISCONNECT_REQUEST");

	for (int i = 0; i < numReqStripes; i++)
	{
		ConnectRequest cRequest = requests[i];
		int stripe = cRequest.stripe;
		IPvXAddress currentParent = cRequest.currentParent;
		std::vector<IPvXAddress> lastRequests = cRequest.lastRequests;

		IPvXAddress alternativeParent = getAlternativeNode(stripe, address, currentParent, lastRequests);

		DisconnectRequest dRequest;
		dRequest.stripe = stripe;
		dRequest.alternativeParent = alternativeParent;

		EV << "Alternative parent for " << address << " (stripe " << stripe << ") is " << alternativeParent << endl;

		pkt->getRequests().push_back(dRequest);

		// Update successors of the alternative parent
		if(!alternativeParent.equals(m_partnerList->getParent(stripe)))
		{
			int newSucc = m_partnerList->getNumChildsSuccessors(stripe, alternativeParent) + 1;
			EV << "UPDATE " << alternativeParent << " to " << newSucc << endl;
			m_partnerList->updateNumChildsSuccessors(stripe, alternativeParent, newSucc);
		}

	}

	numDR++;
	m_gstat->reportMessageDR();
	sendToDispatcher(pkt, m_localPort, address, m_destPort);
}

void MultitreeBase::acceptConnectRequests(const std::vector<ConnectRequest> &requests, IPvXAddress address)
{
	int numReqStripes = requests.size();

	TreeConnectConfirmPacket *pkt = new TreeConnectConfirmPacket("TREE_CONECT_CONFIRM");

	EV << "Accepting ConnectRequest for stripe(s) ";
	for (int i = 0; i < numReqStripes; i++)
	{
		ConnectRequest request = requests[i];
		int stripe = request.stripe;
		IPvXAddress currentParent = request.currentParent;
		std::vector<IPvXAddress> lastRequests = request.lastRequests;
		IPvXAddress alternativeParent = getAlternativeNode(stripe, address, currentParent, lastRequests);

		for (size_t j = 0; j < numStripes; j++)
		{
			m_partnerList->updateNumChildsSuccessors(j, address, request.numSuccessors[j]);
		}

		ConnectConfirm confirm;
		confirm.stripe = stripe;
		confirm.alternativeParent = alternativeParent;

		pkt->getConfirms().push_back(confirm);

		EV << stripe << ", ";
	}
	EV << " of node " << address << endl;

	numCC++;
	m_gstat->reportMessageCC();
    sendToDispatcher(pkt, m_localPort, address, m_destPort);

	for (int i = 0; i < numReqStripes; i++)
	{
		ConnectRequest request = requests[i];

		int stripe = request.stripe;

		if(param_sendMissingChunks)
		{
			EV << "Sending missing chunks to new child." << endl;
			int lastChunk = request.lastReceivedChunk;
			sendChunksToNewChild(stripe, address, lastChunk);
		}
	}

	gainThreshold = getGainThreshold();
}

void MultitreeBase::processSuccessorUpdate(cPacket *pkt)
{
    TreeSuccessorInfoPacket *treePkt = check_and_cast<TreeSuccessorInfoPacket *>(pkt);

	std::vector<SuccessorInfo> updates = treePkt->getUpdates();

	IPvXAddress address;
	getSender(pkt, address);

	bool changes = false;

	for(std::vector<SuccessorInfo>::iterator it = updates.begin(); it != updates.end(); ++it)
	{
		SuccessorInfo update = (SuccessorInfo)*it;
		int stripe = update.stripe;

		int oldSucc = m_partnerList->getNumChildsSuccessors(stripe, address);

		if(oldSucc != -1)
		{
			int newSucc = update.numSuccessors;
			if(newSucc != oldSucc)
			{
				changes = true;
				m_partnerList->updateNumChildsSuccessors(stripe, address, newSucc);

				if(m_partnerList->hasChild(stripe, address))
				{
					scheduleSuccessorInfo(stripe);
					scheduleOptimization();
				}
			}
		}
	}

	if(changes)
	{
		printStatus();
		// Optimize when a node detects "major changes" in the topology below
		//scheduleOptimization();
	}
}

void MultitreeBase::removeChild(int stripe, IPvXAddress address)
{
	EV << "Removing child: " << address << " (stripe: " << stripe << ")" << endl;
	m_partnerList->removeChild(stripe, address);

	gainThreshold = getGainThreshold();

	std::set<IPvXAddress> &curDisconnectingChildren = disconnectingChildren[stripe];
	if(curDisconnectingChildren.find(address) != curDisconnectingChildren.end())
		curDisconnectingChildren.erase(curDisconnectingChildren.find(address));

	//scheduleOptimization();
	//scheduleSuccessorInfo(stripe);

	printStatus();
}

void MultitreeBase::getSender(cPacket *pkt, IPvXAddress &senderAddress, int &senderPort)
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
	senderAddress   = controlInfo->getSrcAddr();
	senderPort      = controlInfo->getSrcPort();
}

void MultitreeBase::getSender(cPacket *pkt, IPvXAddress &senderAddress)
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        senderAddress   = controlInfo->getSrcAddr();
}

const IPvXAddress& MultitreeBase::getSender(const cPacket *pkt) const
{
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
        return controlInfo->getSrcAddr();
}

bool MultitreeBase::hasBWLeft(int additionalConnections)
{
    int outConnections = m_partnerList->getNumOutgoingConnections() - getNumDisconnectChildren();
	//EV << "BW: current: " << outConnections << ", req: " << additionalConnections << ", max: " << getMaxOutConnections() << endl;
	return (outConnections + additionalConnections) <= getMaxOutConnections();
}

void MultitreeBase::scheduleOptimization(void)
{
	if(timer_optimization->isScheduled() || !param_optimize)
		return;

	EV << "Scheduling optimization to: " << simTime() + param_delayOptimization << endl;
    scheduleAt(simTime() + param_delayOptimization, timer_optimization);
}

void MultitreeBase::getCheapestChild(int stripe, IPvXAddress &address, IPvXAddress skipAddress)
{
    IPvXAddress maxAddress;
	double maxGain = INT_MIN;
	int minActiveTrees = INT_MAX;

	std::set<IPvXAddress> children = m_partnerList->getChildren(stripe);

	for(std::set<IPvXAddress>::iterator it = children.begin() ; it != children.end(); ++it)
	{
		IPvXAddress curAddress = (IPvXAddress)*it;

		if(//m_partnerList->hasParent(curAddress)
				curAddress.equals(skipAddress) 
				|| disconnectingChildren[stripe].find(curAddress) != disconnectingChildren[stripe].end()
				|| m_partnerList->nodeHasMoreChildrenInOtherStripe(stripe, curAddress)
			)
		{
			continue;
		}

		double curGain = getGain(stripe, curAddress);
		int curActiveTrees = m_partnerList->getNumActiveTrees(curAddress);

		//EV << "checking: " << curAddress << ", gain: " << curGain << endl;

		if(maxGain < curGain 
				|| (maxGain == curGain && minActiveTrees < curActiveTrees)
				|| (maxGain == curGain && minActiveTrees == curActiveTrees && intrand(2) == 0)
			)
		{
			maxGain = curGain;
			maxAddress = curAddress;
			minActiveTrees = curActiveTrees;
		}
	}

	if(maxAddress.isUnspecified())
	{
		for(std::set<IPvXAddress>::iterator it = children.begin() ; it != children.end(); ++it)
		{
			IPvXAddress curAddress = (IPvXAddress)*it;

			if( curAddress.equals(skipAddress) 
					|| disconnectingChildren[stripe].find(curAddress) != disconnectingChildren[stripe].end()
				)
			{
				continue;
			}

			double curGain = getGain(stripe, curAddress);
			int curActiveTrees = m_partnerList->getNumActiveTrees(curAddress);

			//EV << "checking: " << curAddress << ", gain: " << curGain << endl;

			if(maxGain < curGain 
					|| (maxGain == curGain && minActiveTrees < curActiveTrees)
					|| (maxGain == curGain && minActiveTrees == curActiveTrees && intrand(2) == 0)
				)
			{
				maxGain = curGain;
				maxAddress = curAddress;
				minActiveTrees = curActiveTrees;
			}
		}

	}

	address = maxAddress;
}

double MultitreeBase::getGain(int stripe, IPvXAddress child)
{
	//EV << "*************** GAIN *******************" << endl;
	//EV << " Child: " << child << ": ";
	//EV << "K3: " << getBalanceCosts(childList, stripe, child);
	//EV << ", K2: " << getForwardingCosts(stripe, child);
	//EV << ", Total: " << (param_weightK3 * getBalanceCosts(childList, stripe, child))
	//   	- (param_weightK2 * getForwardingCosts(stripe, child));

	// K_3 - K_2
	return (getBalanceCosts(stripe, child, NULL))
		- (getForwardingCosts(stripe, child));
}

double MultitreeBase::getGain(int stripe, IPvXAddress child, IPvXAddress *linkToDrop)
{
	//EV << "*************** GAIN *******************" << endl;
	//EV << " Child: " << child << ": ";
	//EV << "K3: " << getBalanceCosts(stripe, child, linkToDrop);
	//EV << ", K2: " << getForwardingCosts(stripe, child);
	//EV << ", Total: " << (param_weightK3 * getBalanceCosts(stripe, child, linkToDrop))
	//   	- (param_weightK2 * getForwardingCosts(stripe, child));

	// K_3 - K_2
	return (getBalanceCosts(stripe, child, linkToDrop))
		- (getForwardingCosts(stripe, child));
}

double MultitreeBase::getCosts(int stripe, IPvXAddress child)
{
	//EV << child << ": ";
	//EV << "K1: " << getStripeDensityCosts(stripe);
	//EV << ", K2: " << getForwardingCosts(stripe, child);
	//EV << ", K3: " << getBalanceCosts(stripe, child, NULL);
	//EV << ", K4: " << getDependencyCosts(child);
	//EV << ", Total: " << param_weightK1 * getStripeDensityCosts(stripe)
	//	+ param_weightK2 * getForwardingCosts(stripe, child)
	//	+ param_weightK3 * getBalanceCosts(stripe, child, NULL)
	//	+ param_weightK4 * getDependencyCosts(child) << endl;
	//EV << "****************************************" << endl;

	// K_1 + 2 * K_2 + 3 * K_3 + 4 * K_4
	return param_weightK1 * getStripeDensityCosts(stripe)
		+ param_weightK2 * getForwardingCosts(stripe, child)
		+ param_weightK3 * getBalanceCosts(stripe, child, NULL)
		+ param_weightK4 * getDependencyCosts(child);
}

double MultitreeBase::getStripeDensityCosts(unsigned int stripe) // K_sel ,K_1
{
	int fanout = m_partnerList->getNumOutgoingConnections(stripe) - disconnectingChildren[stripe].size();
	double outCapacity = (bwCapacity * numStripes);
	//EV << "K1: " << fanout << " " << outCapacity << endl;
	return 1 - (fanout / outCapacity);
}

int MultitreeBase::getForwardingCosts(int stripe, IPvXAddress child) // K_forw, K_2
{
	if(m_partnerList->nodeHasMoreChildrenInOtherStripe(stripe, child))
		return 1;
	return 0;
}

double MultitreeBase::getBalanceCosts(unsigned int stripe, IPvXAddress child, IPvXAddress *linkToDrop) // K_bal, K_3
{
    double numChildren = m_partnerList->getNumOutgoingConnections(stripe) - disconnectingChildren[stripe].size();

	if(linkToDrop != NULL)
		numChildren--;

	double numSucc = m_partnerList->getNumSuccessors(stripe);

	//if(numChildren == numSucc)
	//	// None of my children has children themself
	//	return 1;

    int childsSuccessors = m_partnerList->getNumChildsSuccessors(stripe, child);

	if(linkToDrop != NULL)
		childsSuccessors += 1 + m_partnerList->getNumChildsSuccessors(stripe, *linkToDrop);

	//EV << "fanout: " << numChildren << " mySucc: " << numSucc << " childsSucc: " << childsSuccessors << endl; 

    double meanNumSucc = (numSucc / numChildren) - 1.0;
	if(meanNumSucc == 0)
		return 0;

    return  (meanNumSucc - childsSuccessors) / meanNumSucc;
}

double MultitreeBase::getDependencyCosts(IPvXAddress child) // K_4
{
	int numConnections = 0;
	for (size_t i = 0; i < numStripes; i++)
	{
		if(m_partnerList->hasChild(i, child) && disconnectingChildren[i].find(child) == disconnectingChildren[i].end() )
		{
			numConnections++;
		}
	}
    return (double)numConnections / (double)numStripes;
}

double MultitreeBase::getGainThreshold(void)
{
	double t = param_weightT;

    int outDegree = 0;
    for (size_t i = 0; i < numStripes; i++)
    {
		int out = m_partnerList->getNumOutgoingConnections(i);
		int disconnecting = disconnectingChildren[i].size();

		outDegree += out;
		outDegree -= disconnecting;
    }
    double b = (double)outDegree / (double)(bwCapacity * numStripes);

	//EV << "deg: " << outDegree << " max: " << (bwCapacity * numStripes) << " b: " << b << endl;
	//EV << "t: " <<  (1 - pow(b, pow(2 * t, 3)) * (1 - pow(t, 3))) + pow(t, 3) << endl;

	if(b >= 1)
		return INT_MIN;

	return (1.0 - pow(b, pow(2.0 * t, 3.0)) * (1.0 - pow(t, 3.0))) + pow(t, 3.0);
}

void MultitreeBase::dropNode(int stripe, IPvXAddress address, IPvXAddress alternativeParent)
{
	if(isDisconnecting(stripe, address))
		// Only drop if not already disconnecting
		return;

	TreeDisconnectRequestPacket *pkt = new TreeDisconnectRequestPacket("TREE_DISCONNECT_REQUEST");

	EV << "Sending DisconnectRequests to " << address << " (stripe: " << stripe 
		<< "), alternativeParent=" << alternativeParent << endl;

	DisconnectRequest request;
	request.stripe = stripe;
	request.alternativeParent = alternativeParent;

	pkt->getRequests().push_back(request);

	if(!m_partnerList->hasParent(stripe, address))
		// If that node is no parent, mark it as disconnecting child
		disconnectingChildren[stripe].insert(address);

	gainThreshold = getGainThreshold();
	numDR++;
	m_gstat->reportMessageDR();
	sendToDispatcher(pkt, m_localPort, address, m_destPort);
}

void MultitreeBase::printStatus(void)
{
	if(!ev.isDisabled())
	{
		EV << "*******************************" << endl;
		EV << getNodeAddress() << " (" << m_partnerList->getNumActiveTrees() << " active stripes)" << endl;
		m_partnerList->printPartnerList();
		EV << "*******************************" << endl;
	}
}


double MultitreeBase::getBWCapacityFromChannel(void)
{
	double rate;
    cModule* nodeModule = getParentModule();

    int gateSize = nodeModule->gateSize("pppg$o");

    for (int i = 0; i < gateSize; i++)
    {
        cGate* currentGate = nodeModule->gate("pppg$o", i);
        if(currentGate->isConnected())
        {
            rate = 0.9 * (check_and_cast<cDatarateChannel *>(currentGate->getChannel())->getDatarate());
        }
    }

	double capacity = (rate / m_appSetting->getVideoStreamBitRate());
	EV << "Detected bandwidth capacity of " << capacity << endl;
	return capacity;
}

void MultitreeBase::cancelAndDeleteTimer(void)
{
	if(timer_optimization != NULL)
	{
		delete cancelEvent(timer_optimization);
		timer_optimization = NULL;
	}
}

int MultitreeBase::getMaxOutConnections()
{
	return numStripes * (bwCapacity);
}

void MultitreeBase::sendChunksToNewChild(int stripe, IPvXAddress address, int lastChunk)
{
	//EV << "Childs last chunk was: " <<  lastChunk << endl;
	if(lastChunk == -1)
		return;

	for (int i = lastChunk + 1; i <= lastSeqNumber[stripe]; i++)
	{
		if(m_videoBuffer->isInBuffer(i))
		{
			VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(i);
			VideoStripePacket *stripePkt = check_and_cast<VideoStripePacket *>(chunkPkt);

			if(stripePkt->getStripe() == stripe)
			{
				//EV << "Sending chunk: " << i << endl;
				sendToDispatcher(stripePkt->dup(), m_localPort, address, m_destPort);
			}
		}
	}
}

bool MultitreeBase::isDisconnecting(int stripe, IPvXAddress child)
{
	return disconnectingChildren[stripe].find(child) != disconnectingChildren[stripe].end();
}

int MultitreeBase::getNumDisconnectChildren(void)
{
	int sum = 0;
	for (size_t i = 0; i < numStripes; i++)
	{
		sum += disconnectingChildren[i].size();
	}
	return sum;
}

bool MultitreeBase::haveMoreChildrenInOtherStripe(unsigned int stripe)
{
	int numChildren = m_partnerList->getNumOutgoingConnections(stripe) - disconnectingChildren[stripe].size();
	for (size_t i = 0; i < numStripes; i++)
	{
		if(i == stripe)
			continue;

		int curNumChildren = m_partnerList->getNumOutgoingConnections(i) - disconnectingChildren[i].size();

		if(curNumChildren > numChildren)
			return true;
	}
	return false;
}
