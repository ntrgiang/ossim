#include <omnetpp.h>

#include <algorithm>
#include <limits>

#include "MultitreePartnerList.h"
#include "AppSettingMultitree.h"

Define_Module(MultitreePartnerList)

MultitreePartnerList::MultitreePartnerList(){}
MultitreePartnerList::~MultitreePartnerList(){}

void MultitreePartnerList::initialize(int stage)
{
    if (stage == 3)
    {
		cModule *temp = simulation.getModuleByPath("appSetting");
		AppSettingMultitree *m_appSetting = check_and_cast<AppSettingMultitree *>(temp);

		numStripes = m_appSetting->getNumStripes();

		parents = new IPvXAddress[numStripes];

		for (size_t i = 0; i < numStripes; i++)
		{
			mChildren.push_back(std::map<IPvXAddress, std::vector<int> >());
			vChildren.push_back(std::set<IPvXAddress>());
		}
    }
}

void MultitreePartnerList::finish()
{
	delete[] parents;
}

void MultitreePartnerList::handleMessage(cMessage *)
{
    throw cException("PartnerList does not process messages!");
}

void MultitreePartnerList::clear(void)
{
	delete[] parents;
	parents = new IPvXAddress[numStripes];

	mChildren.clear();
	vChildren.clear();
	for (size_t i = 0; i < numStripes; i++)
	{
		mChildren.push_back(std::map<IPvXAddress, std::vector<int> >());
		vChildren.push_back(std::set<IPvXAddress>());
	}
}

bool MultitreePartnerList::hasChildren(unsigned int stripe)
{
	return mChildren[stripe].size() > 0;
}

bool MultitreePartnerList::hasChildren(void)
{
	for (size_t i = 0; i < numStripes; i++)
		if(hasChildren(i))
			return true;

	return false;
}

bool MultitreePartnerList::hasChild(IPvXAddress address)
{
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasChild(i, address))
			return true;
	}
	return false;
}

bool MultitreePartnerList::hasChild(unsigned int stripe, IPvXAddress address)
{
	return vChildren[stripe].find(address) != vChildren[stripe].end();
}

void MultitreePartnerList::addChild(unsigned int stripe, IPvXAddress address, int successors)
{
	mChildren[stripe].insert( std::pair<IPvXAddress, std::vector<int> >(address, std::vector<int>()) );

	for (size_t i = 0; i < numStripes; i++)
	{
		mChildren[stripe][address].push_back(0);
	}

	mChildren[stripe][address][stripe] = successors;

	vChildren[stripe].insert(address);
	numOutgoingChanged = true;
}

IPvXAddress MultitreePartnerList::getChildWithLeastChildren(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	IPvXAddress child;
	const std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];

   //int minActiveTrees = INT_MAX; // Giang commented
	int minSucc = INT_MAX;

	for (std::map<IPvXAddress, std::vector<int> >::const_iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
	{
		bool skipNode = skipNodes.find(it->first) != skipNodes.end();
		if(!skipNode)
		{
         //int numActiveTrees = getNumActiveTrees(it->first); // Giang commented
			int a = intrand(2);

			if(//numActiveTrees <= minActiveTrees
				(it->second[stripe] < minSucc || (it->second[stripe] == minSucc && a == 0))
			  )
			{
				//EV << it->second[stripe] << " vs. " << minSucc << " (" << a << ")" << endl;
				minSucc = it->second[stripe];
            //minActiveTrees = numActiveTrees; // Giang commented
				child = it->first;
				//EV << "take: " << child << endl;
			}
		}
	}
	return child;
}

/**
 * Returns the child that has the least (but at least one) successor. It is also possible to specify
 * some nodes that should not be chosen here.
 */
IPvXAddress MultitreePartnerList::getLaziestForwardingChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	IPvXAddress child;
	const std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];
	int minSucc = INT_MAX;
	int minActiveTrees = INT_MAX;

	for (std::map<IPvXAddress, std::vector<int> >::const_iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
	{
		bool skipNode = skipNodes.find(it->first) != skipNodes.end();
		if(!skipNode)
		{
			int curActiveTrees = getNumActiveTrees(it->first);

			if( (it->second[stripe] > 0 && it->second[stripe] < minSucc)
				|| (it->second[stripe] == minSucc && curActiveTrees < minActiveTrees)
				|| (it->second[stripe] == minSucc && curActiveTrees == minActiveTrees && (int)intrand(2) == 0)
			  )
			{
				minSucc = it->second[stripe];
				minActiveTrees = curActiveTrees;
				child = it->first;
			}
		}
	}
	return child;
}

IPvXAddress MultitreePartnerList::getBusiestLazyChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	// Laziest forwarding child, but with the most active trees
	IPvXAddress child;
	const std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];
	int minSucc = INT_MAX;
	int maxActiveTrees = INT_MIN;

	for (std::map<IPvXAddress, std::vector<int> >::const_iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
	{
		bool skipNode = skipNodes.find(it->first) != skipNodes.end();
		if(!skipNode)
		{
			int curActiveTrees = getNumActiveTrees(it->first);
			//EV << it->first << " has " << it->second[stripe] << " succ (min: " << minSucc 
			//	<< ") and " << curActiveTrees << " trees (max: " << maxActiveTrees << ")" << endl;

			if( 
				(it->second[stripe] > 0 && it->second[stripe] < minSucc)
				|| (it->second[stripe] > 0 && it->second[stripe] == minSucc && curActiveTrees > maxActiveTrees)
				|| (it->second[stripe] > 0 && it->second[stripe] == minSucc && curActiveTrees == maxActiveTrees && intrand(2) == 0)
				)
			{
				minSucc = it->second[stripe];
				maxActiveTrees = curActiveTrees;
				child = it->first;
			}
		}
	}
	return child;
}

IPvXAddress MultitreePartnerList::getRandomChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	IPvXAddress child;
	const std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];

	if(curChildren.size() > 0)
	{
		bool allInSkipNodes = true;
		for (std::map<IPvXAddress, std::vector<int> >::const_iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
		{
			if(skipNodes.find(it->first) == skipNodes.end())
			{
				allInSkipNodes = false;
				//return it->first;
			}
		}

		if(!allInSkipNodes)
		{
			do {
				std::map<IPvXAddress, std::vector<int> >::const_iterator it = curChildren.begin();
				std::advance(it, intrand(curChildren.size()));
				child = (IPvXAddress)it->first;
			} while(skipNodes.find(child) != skipNodes.end());
		}
	}
	return child;
}

IPvXAddress MultitreePartnerList::getChildWithMostChildren(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	int maxSucc = INT_MIN;
	int maxActiveTrees = INT_MIN;
	IPvXAddress busiestChild;
	std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];

	for (std::map<IPvXAddress, std::vector<int> >::iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
	{
		bool skipNode = skipNodes.find(it->first) != skipNodes.end();
		if(!skipNode)
		{
			int numActiveTrees = getNumActiveTrees(it->first);

			if(it->second[stripe] > maxSucc 
				|| (it->second[stripe] == maxSucc && maxActiveTrees < numActiveTrees)
				|| (it->second[stripe] == maxSucc && maxActiveTrees == numActiveTrees && intrand(2) == 0)
				)
			{
				maxSucc = it->second[stripe];
				busiestChild = it->first;
				maxActiveTrees = numActiveTrees;
			}
		}
	}
	return busiestChild;
}

/*
 * Tries to find the "best" "lazy" child in 2 steps:
 *    1.) Find the node with least successors, that has at least 1 successor
 *    2.) If 1. didn't yield a result, find the child with least successors
 */
IPvXAddress MultitreePartnerList::getBestLazyChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes)
{
	IPvXAddress child = getLaziestForwardingChild(stripe, skipNodes);

	if(child.isUnspecified())
	{
		child = getChildWithLeastChildren(stripe, skipNodes);
	}

	return child;
}

std::set<IPvXAddress> &MultitreePartnerList::getChildren(unsigned int stripe)
{
	return vChildren[stripe];
}

std::set<IPvXAddress> MultitreePartnerList::getChildren()
{
	std::set<IPvXAddress> distinctiveChildren;
	for (size_t i = 0; i < numStripes; ++i)
	{
		std::set<IPvXAddress> curChildren = vChildren[i];

		for(std::set<IPvXAddress>::iterator it = curChildren.begin(); it != curChildren.end(); ++it)
		{
			distinctiveChildren.insert((IPvXAddress)*it);
		}
	}
	return distinctiveChildren;
}

/** 
 * Return all children (incl. number of successors) of the given stripe
*/
std::map<IPvXAddress, std::vector<int> > MultitreePartnerList::getChildrenWithCount(unsigned int stripe)
{
	return mChildren[stripe];
}

void MultitreePartnerList::removeChild(unsigned int stripe, IPvXAddress address)
{
	mChildren[stripe].erase(address);
	vChildren[stripe].erase(vChildren[stripe].find(address));
	numOutgoingChanged = true;
}

void MultitreePartnerList::removeChild(IPvXAddress address)
{
	for (size_t i = 0; i < numStripes; i++)
	{
		removeChild(i, address);
	}
}



bool MultitreePartnerList::hasParent(unsigned int stripe, IPvXAddress address)
{
	return parents[stripe].equals(address);
}

bool MultitreePartnerList::hasParent(IPvXAddress address)
{
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasParent(i, address))
			return true;
	}
	return false;
}

void MultitreePartnerList::addParent(unsigned int stripe, IPvXAddress address){
	parents[stripe] = address;
}

void MultitreePartnerList::addParent(IPvXAddress address){
	for (size_t i = 0; i < numStripes; i++)
	{
		addParent(i, address);
	}
}

IPvXAddress MultitreePartnerList::getParent(unsigned int stripe)
{
	return parents[stripe];
}

void MultitreePartnerList::removeParent(unsigned int stripe)
{
	IPvXAddress address;
	parents[stripe] = address;
}

std::vector<int> MultitreePartnerList::removeParent(IPvXAddress address)
{
	std::vector<int> affectedStripes;

	for (size_t i = 0; i < numStripes; i++)
	{
		if(address.equals(parents[i]))
		{
			IPvXAddress address;
			parents[i] = address;

			affectedStripes.push_back(i);
		}
	}
	return affectedStripes;
}

int MultitreePartnerList::getNumOutgoingConnections(void)
{
	if(numOutgoingChanged)
	{
		int sum = 0;
		for (size_t i = 0; i < numStripes; i++)
		{
    	    sum = sum + getNumOutgoingConnections(i);
		}
		outgoingConnections = sum;
		numOutgoingChanged = false;
		return sum;
	}
	else
	{
		return outgoingConnections;
	}
}

int MultitreePartnerList::getNumOutgoingConnections(unsigned int stripe)
{
	return vChildren[stripe].size();
}

int MultitreePartnerList::getNumSuccessors(unsigned int stripe)
{
	int sum = 0;
	std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[stripe];
	for (std::map<IPvXAddress, std::vector<int> >::iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
	{
		sum = sum + 1 + it->second[stripe];
	}
	return sum;
}

int MultitreePartnerList::getNumChildsSuccessors(unsigned int stripe, IPvXAddress address)
{
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasChild(i, address))
			return mChildren[i][address][stripe];
	}
	return -1;
}

bool MultitreePartnerList::updateNumChildsSuccessors(unsigned int stripe, IPvXAddress address, int numSuccessors)
{
	bool changed = false;
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasChild(i, address))
		{
			mChildren[i][address][stripe] = numSuccessors;
			changed = true;
		}
	}
	return changed;
}

void MultitreePartnerList::printPartnerList(void)
{
	if(ev.isDisabled())
		return;

	for (size_t i = 0; i < numStripes; i++)
	{
		EV << "*********** Stripe " << i << " **********" << endl;
		EV << "Parent  : " << parents[i] << " (" << getNumOutgoingConnections(i) << " children, "
			<< getNumSuccessors(i) << " successors)" << endl;
		EV << "Children: ";

		std::map<IPvXAddress, std::vector<int> > curChildren = mChildren[i];
		for (std::map<IPvXAddress, std::vector<int> >::iterator it = curChildren.begin() ; it != curChildren.end(); ++it)
		{
			EV << it->first.str() << " (" << it->second[i] << " successors [";

			for (size_t i = 0; i < numStripes; i++)
			{
				EV << it->second[i] << ",";
			}

			EV << "]), ";
		}
		EV << endl;
	}
}

int MultitreePartnerList::getNumActiveTrees(void)
{
	int numActiveTrees = 0;
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasChildren(i))
			numActiveTrees++;
	}
	return numActiveTrees;
}

bool MultitreePartnerList::nodeForwardingInOtherStripe(unsigned int stripe, IPvXAddress node)
{
	for (size_t i = 0; i < numStripes; i++)
	{
		if(i == stripe)
			continue;

		if(getNumChildsSuccessors(i, node) > 0)
			return true;
	}
	return false;
}

bool MultitreePartnerList::nodeHasMoreChildrenInOtherStripe(unsigned int stripe, IPvXAddress node)
{
	int numSucc = getNumChildsSuccessors(stripe, node);
	for (size_t i = 0; i < numStripes; i++)
	{
		if(i == stripe)
			continue;

		if(getNumChildsSuccessors(i, node) > numSucc)
			return true;
	}
	return false;
}

int MultitreePartnerList::getNumActiveTrees(IPvXAddress node)
{
	int activeTrees = 0;
	for (size_t i = 0; i < numStripes; i++)
	{
		if(hasParent(i, node) || getNumChildsSuccessors(i, node) > 0)
		{
			activeTrees++;
		}
	}
	return activeTrees;
}
