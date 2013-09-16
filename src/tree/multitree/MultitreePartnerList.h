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
// MultitreePartnerList.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Julian Wulfheide;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

#ifndef MULTITREEPARTNERLIST_H_
#define MULTITREEPARTNERLIST_H_ true

#include <IPvXAddress.h>
#include <vector>

class MultitreePartnerList : public cSimpleModule
{
public:
	MultitreePartnerList();
	virtual ~MultitreePartnerList();

	virtual int numInitStages() const { return 4; }
	virtual void initialize(int stage);
	virtual void finish();
	virtual void handleMessage(cMessage *);

	void clear(void);

	int getNumOutgoingConnections(void);
	int getNumOutgoingConnections(unsigned int stripe);

	bool nodeForwardingInOtherStripe(unsigned int stripe, IPvXAddress node);
	bool nodeHasMoreChildrenInOtherStripe(unsigned int stripe, IPvXAddress node);

	int getNumChildren(void);
	int getNumSuccessors(unsigned int stripe);
    int getNumChildsSuccessors(unsigned int stripe, IPvXAddress address);

    bool updateNumChildsSuccessors(unsigned int stripe, IPvXAddress address, int numSuccessors);

	bool hasParent(IPvXAddress address);
	bool hasParent(unsigned int stripe, IPvXAddress address);
	void addParent(IPvXAddress address);
	void addParent(unsigned int stripe, IPvXAddress address);
	std::vector<int> removeParent(IPvXAddress address);
	void removeParent(unsigned int stripe);
	IPvXAddress getParent(unsigned int stripe);

	bool hasChildren(unsigned int stripe);
	bool hasChildren(void);
	bool hasChild(IPvXAddress address);
	bool hasChild(unsigned int stripe, IPvXAddress address);
	void addChild(unsigned int stripe, IPvXAddress address, int successors);
	void removeChild(IPvXAddress address);
	void removeChild(unsigned int stripe, IPvXAddress address);
	std::set<IPvXAddress> getChildren();
	std::set<IPvXAddress> &getChildren(unsigned int stripe);
	std::map<IPvXAddress, std::vector<int> > getChildrenWithCount(unsigned int stripe);

	IPvXAddress getBusiestLazyChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);

	IPvXAddress getLaziestForwardingChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);
	IPvXAddress getChildWithMostChildren(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);
	IPvXAddress getChildWithLeastChildren(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);
	IPvXAddress getRandomChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);
	IPvXAddress getBestLazyChild(unsigned int stripe, const std::set<IPvXAddress> &skipNodes);

	int getNumActiveTrees(void);
	int getNumActiveTrees(IPvXAddress child);

	void printPartnerList(void);
protected:

private:
	bool numOutgoingChanged;
	int outgoingConnections;

	unsigned int numStripes;

	IPvXAddress* parents;
	std::vector<std::map<IPvXAddress, std::vector<int> > > mChildren;
	std::vector<std::set<IPvXAddress> > vChildren;


};

#endif
