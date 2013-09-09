#include "MultitreeSource.h"

#include <algorithm> 

Define_Module(MultitreeSource)

MultitreeSource::MultitreeSource(){}
MultitreeSource::~MultitreeSource(){}

void MultitreeSource::initialize(int stage)
{
	MultitreeBase::initialize(stage);

	if(stage != 3)
		return;
	if(stage == 2)
	{
		bindToGlobalModule();
		bindToTreeModule();
		bindToStatisticModule();
	}

	if(stage == 3)
	{
		for (size_t i = 0; i < numStripes; i++)
		{
			m_state[i] = TREE_JOIN_STATE_ACTIVE;
		}

		m_apTable->addAddress(getNodeAddress());

		// -------------------------------------------------------------------------
		// -------------------------------- Timers ---------------------------------
		// -------------------------------------------------------------------------
		// -- One-time timers


		// -- Repeated timers

		if(bwCapacity < 1)
			throw cException("The source has a bandwidth capacity < 1. This won't work.");

	}
}

void MultitreeSource::finish(void)
{
	char name[24];
	for (unsigned int i = 0; i < numStripes; i++)
	{
		sprintf(name, "numChildren%d", i);
		recordScalar(name, m_partnerList->getNumOutgoingConnections(i));
	}

	recordScalar("distinctiveChildren", m_partnerList->getChildren().size());

	MultitreeBase::finish();

	cancelAndDeleteTimer();
}

void MultitreeSource::processPacket(cPacket *pkt)
{
	PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(pkt);
    if (appMsg->getPacketGroup() != PACKET_GROUP_TREE_OVERLAY)
    {
        throw cException("MultitreeSource::processPacket: received a wrong packet. Wrong packet type!");
    }

    TreePeerStreamingPacket *treeMsg = check_and_cast<TreePeerStreamingPacket *>(appMsg);
    switch (treeMsg->getPacketType())
    {
	case TREE_CONNECT_REQUEST:
    {
		processConnectRequest(treeMsg);
		break;
    }
	case TREE_DISCONNECT_REQUEST:
	{
		processDisconnectRequest(treeMsg);
		break;
	}
	case TREE_SUCCESSOR_INFO:
	{
		processSuccessorUpdate(treeMsg);
		break;
	}
    default:
    {
        throw cException("MultitreeSource::processPacket: Unrecognized packet types! %d", treeMsg->getPacketType());
        break;
    }
    }

    delete pkt;
}

void MultitreeSource::processDisconnectRequest(cPacket* pkt)
{
	TreeDisconnectRequestPacket *treePkt = check_and_cast<TreeDisconnectRequestPacket *>(pkt);

	IPvXAddress address;
	getSender(pkt, address);

	std::vector<DisconnectRequest> requests = treePkt->getRequests();

	for(std::vector<DisconnectRequest>::iterator it = requests.begin(); it != requests.end(); ++it)
	{
		int stripe = ((DisconnectRequest)*it).stripe;
		removeChild(stripe, address);
	}
}
 

void MultitreeSource::handleTimerMessage(cMessage *msg)
{
	if (msg == timer_optimization)
	{
        handleTimerOptimization();
	}
}

void MultitreeSource::bindToGlobalModule(void)
{
	MultitreeBase::bindToGlobalModule();
}

void MultitreeSource::bindToTreeModule(void)
{
	MultitreeBase::bindToTreeModule();
}

void MultitreeSource::cancelAndDeleteTimer(void)
{
}

void MultitreeSource::cancelAllTimer()
{
}


void MultitreeSource::scheduleSuccessorInfo(int stripe)
{ 
	// Do nothing because a source has no parents...
}

bool MultitreeSource::isPreferredStripe(unsigned int stripe)
{
	return true;
}

void MultitreeSource::onNewChunk(int sequenceNumber)
{
	Enter_Method("onNewChunk");

	VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(sequenceNumber);
	VideoStripePacket *stripePkt = check_and_cast<VideoStripePacket *>(chunkPkt);

	int stripe = stripePkt->getStripe();
	int hopcount = stripePkt->getHopCount();
	lastSeqNumber[stripe] = stripePkt->getSeqNumber();

	stripePkt->setHopCount(++hopcount);

	std::set<IPvXAddress> &children = m_partnerList->getChildren(stripe);

	for(std::set<IPvXAddress>::iterator it = children.begin(); it != children.end(); ++it)
	{
		sendToDispatcher(stripePkt->dup(), m_localPort, (IPvXAddress)*it, m_destPort);
	}
}

IPvXAddress MultitreeSource::getAlternativeNode(int stripe, IPvXAddress forNode, IPvXAddress currentParent, std::vector<IPvXAddress> lastRequests)
{
	//EV << "Searching alternative parent for " << forNode << ", currentParent="
	//	<< currentParent << ", alread tried to connect to " << lastRequests.size()
	//	<< " nodes." << endl;

	std::set<IPvXAddress> skipNodes;
	skipNodes.insert(forNode);
	skipNodes.insert(currentParent);
	for(std::vector<IPvXAddress>::iterator it = lastRequests.begin(); it != lastRequests.end(); ++it)
	{
		skipNodes.insert( (IPvXAddress)*it );
	}

	//IPvXAddress address = m_partnerList->getChildWithMostChildren(stripe, skipNodes);
	IPvXAddress address = m_partnerList->getBestLazyChild(stripe, skipNodes);
	//IPvXAddress address = m_partnerList->getChildWithLeastChildren(stripe, skipNodes);
	
	while(m_partnerList->nodeHasMoreChildrenInOtherStripe(stripe, address) && !address.isUnspecified())
	{
		skipNodes.insert(address);
		//address = m_partnerList->getChildWithMostChildren(stripe, skipNodes);
		address = m_partnerList->getBestLazyChild(stripe, skipNodes);
	}

	if(address.isUnspecified())
	{
		skipNodes.clear();
		skipNodes.insert(forNode);
		skipNodes.insert(currentParent);
		if(!lastRequests.empty())
			skipNodes.insert( lastRequests.back() );

		//address = m_partnerList->getRandomChild(stripe, skipNodes);
		address = m_partnerList->getChildWithMostChildren(stripe, skipNodes);
		//address = m_partnerList->getBestLazyChild(stripe, skipNodes);
		//address = m_partnerList->getChildWithLeastChildren(stripe, skipNodes);
	}

	if(address.isUnspecified())
	{
		if(!lastRequests.empty())
			skipNodes.erase(skipNodes.find( lastRequests.back() ));

		address = m_partnerList->getRandomChild(stripe, skipNodes);
	}

	if(address.isUnspecified())
	{
		address = getNodeAddress();
	}

	return address;
}

void MultitreeSource::optimize(void)
{
	std::vector<ChildCost> childCosts;

	printStatus();

	// nodes from who I should not request children
	std::map<int, std::set<IPvXAddress> > skipNodes;

	for (size_t i = 0; i < numStripes; i++)
	{
		const std::set<IPvXAddress> &curChildren = m_partnerList->getChildren(i);

		for(std::set<IPvXAddress>::iterator it = curChildren.begin(); it != curChildren.end(); ++it)
		{
			childCosts.push_back(ChildCost(i, getCosts(i, (IPvXAddress)*it), (IPvXAddress)*it));
		}
	}

	sort(childCosts.begin(), childCosts.end());

	unsigned int lookedAt = 0;
	bool gain = true;
	//for(std::vector<ChildCost>::iterator it = childCosts.begin(); it != childCosts.end(); ++it)
	while(gain || lookedAt <= childCosts.size())
	{
		gain = false;

		if(childCosts.size() < 1)
			break;

		for(std::vector<ChildCost>::iterator it = childCosts.begin(); it != childCosts.end(); ++it)
		{
			double newCost = getCosts(it->stripe, it->child);
			it->cost = newCost;
		}
		sort(childCosts.begin(), childCosts.end());

		for(std::vector<ChildCost>::iterator it = childCosts.begin(); it != childCosts.end(); ++it)
		{
			EV << it->child << " (stripe " << it->stripe << ") " << it->cost << endl;
		}

		ChildCost linkToDrop = childCosts[0];
		IPvXAddress child = linkToDrop.child;
		int stripe = linkToDrop.stripe;

		lookedAt++;

		unsigned int asd = 1;
		while((
					m_partnerList->getNumOutgoingConnections(stripe) <= 1 || 
					haveMoreChildrenInOtherStripe(stripe))
				&& asd < childCosts.size())
		{
			linkToDrop = childCosts[asd++];
			child = linkToDrop.child;
			stripe = linkToDrop.stripe;
		}

		if(m_partnerList->getNumOutgoingConnections(stripe) <= 1 || haveMoreChildrenInOtherStripe(stripe))
			break;

		IPvXAddress alternativeParent;	
		getCheapestChild(stripe, alternativeParent, child);

		EV << "COSTLIEST CHILD: " << child << " (stripe " << stripe << ")" << endl;
		EV << "CHEAPEST CHILD: " << alternativeParent << endl;
		if(!child.isUnspecified() && !alternativeParent.isUnspecified())
		{
			double gainIf = getGain(stripe, alternativeParent, &child);
			EV << "GAIN: " << gainIf << endl;
			gainThreshold = getGainThreshold();
			EV << "THRESHOLD: " << gainThreshold << endl;

			if(gainIf >= gainThreshold)
			{
				// Drop costliest to cheapest
				dropNode(stripe, child, alternativeParent);

				skipNodes[stripe].insert(child);
				if(m_partnerList->getNumChildsSuccessors(stripe, alternativeParent) == 0)
					skipNodes[stripe].insert(alternativeParent);

				gain = true;

				int succParent = m_partnerList->getNumChildsSuccessors(stripe, alternativeParent);
				int succDrop = m_partnerList->getNumChildsSuccessors(stripe, child);
				m_partnerList->updateNumChildsSuccessors(stripe, alternativeParent, succParent + 1 + succDrop);

				gainThreshold = getGainThreshold();
			}
		}

		EV << "--------------------------------------------" << endl;

		childCosts.erase(std::find(childCosts.begin(), childCosts.end(), linkToDrop));

	}

	int remainingBW = getMaxOutConnections() - (m_partnerList->getNumOutgoingConnections() - getNumDisconnectChildren());

	EV << "Currently have " << m_partnerList->getNumOutgoingConnections() <<
		" outgoing connections. Max: " << getMaxOutConnections() << " remaining: " << remainingBW << endl;

	// <node, <stripe, remainingBW> >
	std::map<IPvXAddress, std::map<int ,int> > requestNodes;
	std::set<int> treesWithNoMoreChildren;
	int stripe = 0;
	while(remainingBW > 0 && treesWithNoMoreChildren.size() < numStripes)
	{
		// Get stripe with least children...
		int minIndex = 0;
		int minChildren = INT_MAX;
		for (size_t i = 0; i < numStripes; i++)
		{
			if(treesWithNoMoreChildren.find(i) != treesWithNoMoreChildren.end())
				continue;

			int currentNumChildren = m_partnerList->getNumOutgoingConnections(i) - disconnectingChildren[i].size();
			if(currentNumChildren < minChildren
				|| (currentNumChildren == minChildren && intrand(2) == 0)
				)
			{
				minChildren = currentNumChildren;
				minIndex = i;
			}
		}
		stripe = minIndex;

		IPvXAddress child = m_partnerList->getChildWithMostChildren(stripe, skipNodes[stripe]);
		int childsSucc = m_partnerList->getNumChildsSuccessors(stripe, child);

		//EV << "stripe: " << stripe << " child: " << child << " succ: " << childsSucc << endl;

		if(child.isUnspecified() || childsSucc <= 0)
		{
			treesWithNoMoreChildren.insert(stripe);
			continue;
		}

		// ... and request a node from that child

		TreePassNodeRequestPacket *reqPkt = new TreePassNodeRequestPacket("TREE_PASS_NODE_REQUEST");

		PassNodeRequest request;
		request.stripe = stripe;
		request.threshold = gainThreshold;
		request.dependencyFactor = (double)(m_partnerList->getNumSuccessors(stripe) / 
					((double)m_partnerList->getNumOutgoingConnections(stripe) )) - 1;

		EV << "depFactor: " << request.dependencyFactor << endl;

		if(remainingBW >= childsSucc)
			request.remainingBW = childsSucc;
		else
			request.remainingBW = remainingBW;
		remainingBW -= request.remainingBW;

		reqPkt->getRequests().push_back(request);

		EV << "Request " << request.remainingBW << " from " << child << ", stripe " << stripe << endl;

		numPNR++;
		m_gstat->reportMessagePNR();
		sendToDispatcher(reqPkt, m_localPort, child, m_localPort);

		skipNodes[stripe].insert(child);
	}
}

bool MultitreeSource::canAccept(ConnectRequest request)
{
	//return true;

	unsigned int stripe = request.stripe;
	bool moreSuccInOther = true;
	
	int myNumSucc = m_partnerList->getNumSuccessors(stripe);
	
	for (size_t i = 0; i < numStripes; i++)
	{
	  if(i == stripe)
	    continue;
	
	  if(myNumSucc > m_partnerList->getNumSuccessors(i))
	  {
	    moreSuccInOther = false;
	  }
	
	}
	
	bool triedAllChildren = true;
	std::vector<IPvXAddress> lastRequests = request.lastRequests;
	std::set<IPvXAddress> &myChildren = m_partnerList->getChildren(stripe);
	for(std::set<IPvXAddress>::iterator it = myChildren.begin(); it != myChildren.end(); ++it)
	{
	  if( std::find(lastRequests.begin(), lastRequests.end(), (IPvXAddress)*it) == lastRequests.end() )
	  {
	    triedAllChildren = false;
	  }
	}
	
	  // lastRequests.size == 0 and no current parent means this is
	// probably due to a PassNodeRequest
	return (triedAllChildren && moreSuccInOther)
	  || (!request.currentParent.isUnspecified() && request.lastRequests.size() == 0);
}
