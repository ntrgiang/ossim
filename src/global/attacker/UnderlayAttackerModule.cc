//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "UnderlayAttackerModule.h"
#include "IPAddressResolver.h"
#include "OverlayTopology.h"
#include "RoutingTable.h"
#include "InterfaceTable.h"
#include "IPv4InterfaceData.h"

#include "TopologyModel.h"
#include <boost/algorithm/string.hpp>

Define_Module(UnderlayAttackerModule);

#if _DEBUG
# define DEBUGOUT(x) if (C_DEBUG_UnderlayAttacker) std::cout << "UnderlayAttacker: (UA@" << simTime() << "): " << x << endl
#else
# define DEBUGOUT(x)
#endif

void UnderlayAttackerModule::initialize(int stage) {

	if (stage != 3) return;

	//fetch parameters
	active = par("active");
	if (!active) return;
	attackLength = par("attackLength");
	startAttack = par("startAttack");

	victimNumber = par("victimNumber");
    simulateAttack = par("simulateAttack");
    attackPauseRatio = par("attackPauseRatio");
    truncateDamagePercentage = 0;
	performDamageTest = par("performDamageTest");
    truncateDamageMax = par("truncateDamageMax").doubleValue();
    truncateDamageStep = par("truncateDamageStep").doubleValue();

    // FIXME we need a pointer to OverlayTopology here
	oT = NULL;
	firstAttack = true;
	hueCounter = 0;
	weightedrouterUsage = 0;
	usedRouterCounter = 0;
    usedLinks = 0;

	//schedule timer to start attack
	start_timer = new cMessage("StartAttack");
	stop_timer = NULL;
	scheduleAt(par("simTimeLimit").doubleValue() - 3, start_timer);
	scheduleAt(par("simTimeLimit").doubleValue() - 2, new cMessage("CollectResults"));

    //FIXME equivalents for mesh streamign?
    /*start("uo_subtreeCollateralDamage_router");
	start("uo_subtreeCollateralDamage_links");
	start("uo_globalCollateralDamage_router");
    start("uo_globalCollateralDamage_links");*/
}

void UnderlayAttackerModule::handleMessage(cMessage* msg) {

	if(msg == start_timer) {

		startUnderlayAttacker();
		start_timer = NULL;

	} else if(msg == stop_timer) {

        performAttack(victimNumber,0);
		assert(!start_timer->isScheduled());
		start_timer = new cMessage();
		scheduleAt(simTime() + attackLength/attackPauseRatio , start_timer);
		stop_timer = NULL;

	} else if(strcmp(msg->getName(), "CollectResults") == 0) {
		collectResults();
	}

	delete msg;
	msg = NULL;
}

void UnderlayAttackerModule::startUnderlayAttacker() {

	attackedNodes = 0;
	collectNetworkData();
	collectLinkData();
	DEBUGOUT("start underlay attack" );

    //FIXME get pointer to OverlayTopology
    if(!oT) oT = NULL;
    TopologyModel topo = oT->getTopology();
	// first obtain the router vulnerability, since it is required for
	// DamageDependend - damage calculation
	calculateRouterVulnerability();
	int numNodes = topo.getNumRootSuccessors();

	if(performDamageTest) {

		for (int i = 1 ; i < 11 ; i++ ) {
			cOutVector vector;
			cOutVector damage;
            cOutVector damageDistribution;
			std::stringstream test;
			test << "remaining_successors_victims_" << i;
			std::string bla;
			test >> bla;
			vector.setName(bla.c_str());
			std::stringstream damageString;
			damageString << "underlay_greedy_damage_" << i;
			std::string bla2;
			damageString >> bla2;
			damage.setName(bla2.c_str());
            std::stringstream damageDistrString;
            damageDistrString << "underlay_damage_distribution";
            std::string bla3;
            damageDistrString >> bla3;
            damageDistribution.setName(bla3.c_str());

            double j = 0;
            std::map<double,int> damageDistrMap;
            while (j <= truncateDamageMax) {
                truncateDamagePercentage = j;
                performAttack(i,0.5,topo);
				double remainingNodes = topo.getNumRootSuccessors();
                double relDamage = ((double) numNodes - remainingNodes) / (double)numNodes;
                vector.recordWithTimestamp( ((double)j), remainingNodes);
                damage.recordWithTimestamp(((double)j), relDamage);
                if(j == 0) {
                    if(damageDistrMap.find(relDamage) == damageDistrMap.end())
                        damageDistrMap[relDamage] = 1;
                    else
                        damageDistrMap[relDamage]++;
                }
				// restore local topology copy
                topo = oT->getTopology();
                j += truncateDamageStep;
			}

            // write damageDistribution
            std::map<double,int>::iterator it;
            for(it=damageDistrMap.begin(); it != damageDistrMap.end();it++)
                damageDistribution.recordWithTimestamp(it->first,it->second);
		}
	}

	DEBUGOUT(topo.getRoot());
	DEBUGOUT("Nodes:"  << topo.countSuccessors(IPAddressResolver().resolve("srv[0]")));
	//printGatesHistogramm();
    //printUsageHistogramm();

	//calculateStripeOverlap();
	//useAttackerBandwidth(attackerPacketrate);
	
	//DEBUGOUT("Remaining Nodes:"  << topo.countSuccessors(IPAddressResolver().resolve("srv[0]")));
	//schedule timer to stop attack
	assert(!stop_timer || !stop_timer->isScheduled());
	stop_timer = new cMessage();
	scheduleAt(simTime() + attackLength, stop_timer);

} // startUnderlayAttacker()


void UnderlayAttackerModule::performAttack(unsigned int number, float errorRatio) {

    TopologyModel topo = oT->getTopology();
    performAttack(number, errorRatio, topo);
}
/**
 * Peforms an attack on Links, Routers or Channels
 *
 * @param	number		number of entities to be attacked
 * @param	errorRatio	error ratio for the chosen entities (0..1)
 */
void UnderlayAttackerModule::performAttack(unsigned int number, float errorRatio, TopologyModel& topo) {

	if (number < 1) return;


    //build vector with Routers to sort them by usagecount
    std::vector<std::pair<double, int> > dummyVector;
    std::multimap<double, int>::reverse_iterator it;

    // create vector of routers to be attacked
    double counter = 1;
    double size =routerCollateralDamage.size();
    for (it = routerCollateralDamage.rbegin() ; it != routerCollateralDamage.rend() ; it++) {
        DEBUGOUT(it->second  <<" - " << it->first << " - " << routerUsage[it->second]);
        if( (counter / size) >= truncateDamagePercentage ) {
            DEBUGOUT(" -> include");
            dummyVector.push_back(std::pair<double, int>(it->first, it->second));
        }
        counter++;
    }
    std::sort(dummyVector.begin(),dummyVector.end());

    //attack "number" Routers
    //if number of routers is smaller than routers to be attacked take this
    unsigned int maxValue = 0;
    if(!dummyVector.empty())
        maxValue = dummyVector.size() >= number ? number : dummyVector.size()-1;
    for (unsigned int j = 0 ; j < maxValue; j++ ) {
        DEBUGOUT("Attacking Router : " << dummyVector.back().second << " with damage " << dummyVector.back().first );
        attackRouter(topo,dummyVector.back().second , errorRatio);
        dummyVector.pop_back();
    }

} // END performAttack(enum AttackType at, unsigned int number, float errorRatio)

/**
 * Iterates over all Clients and the Server, to get the routes from,
 * collects Channel, Link and Routerusage and produces fancy topology picture
 */
void UnderlayAttackerModule::collectLinkData() {

	//clear old data
	linkUsage.clear();
	channelUsage.clear();
	routerUsage.clear();
	clientDepRouter.clear();
	clientDepChannel.clear();

	//collect new data
	outputFile.open("test.dot");
	outputFile << "digraph G {" << endl;
	int i = 0;
	usedRouterCounter = 0;
	usedLinks = 0;
    double hue = 1 / (double)(getParentModule()->getSubmodule("peerNode",0)->getVectorSize() + 1);

    // clients
    i = 0;
    while(getParentModule()->getSubmodule("peerNode",i)) {
        collectDataFromModule(getParentModule()->getSubmodule("peerNode",i));
		i++;
		hueCounter += hue;
	}

	// servers
    i = 0;
    while(getParentModule()->getSubmodule("srv",i)) {
        collectDataFromModule(getParentModule()->getSubmodule("srv",i));
        i++;
    }

	outputFile << "}" << endl;
	outputFile.close();

	std::map<std::pair<int, int>, int>::iterator it;
	for (it = linkUsage.begin(); it != linkUsage.end() ; it++) {
		usedLinks += it->second;
	}
    //usedLinks -= linkUsage.size();
    //usedRouterCounter -= routerUsage.size();
	DEBUGOUT("Used Routers :" << usedRouterCounter);
	DEBUGOUT("Used Links :" << usedLinks);
}

/**
 * collects data from the RouteTables of the given module
 * saves which link, router, channel is used how often
 * and gets the stripe datarate from server
 *
 * @ param mod	cModule * of the node
 */
void UnderlayAttackerModule::collectDataFromModule(cModule * mod) {

	if(!mod->getSubmodule("udpApp",0)) return;

    //FIXME add pointer to TracerouteTable here
    TracerouteTable* rt = NULL;
    TracerouteTable::RouteMap rm = rt->getRoutes();
	TracerouteTable::RouteMap::iterator it;

    // FIXME get self address
    IPAddress selfAddr = IPvXAddress().get4();
    DEBUGOUT(" - collectDataFromModule " << selfAddr);

    std::vector<IPAddress> vec;
	std::map<IPvXAddress,std::string> stripes;
    TopologyModel& topo = oT->getMostRecentTopologyRef();
    PPIPvXAddressSet children = topo.getChildren(selfAddr);
    foreach(IPvXAddress ip, children) {
        //we only need to cover the outbound links
        vec.push_back(ip.get4());
        stripes[ip] = "0-0";
    }
    // in mesh streaming we have one inbound link
    IPvXAddress pred = topo.getPredecessor(selfAddr);
    if(!pred.isUnspecified()) predecessors[selfAddr]["0-0"] = pred.get4();

    /**
     * FIXME-END
     */

    // Counter for usage of underlay components
	int routeCounter = 0;

	for (it = rm.begin(); it != rm.end();it++) {
		//if destination is not in linkvector continue

		bool isdest = false;
		foreach(IPAddress destaddr,vec) {
			if(destaddr == it->first.get4()) {
				isdest = true;
				break;
			}
		}
		if(!isdest) continue;

		TracerouteTable::Route route = it->second;
		linkRoutes[std::pair<IPAddress,IPAddress>(selfAddr,it->first.get4())] = route;
		// iterate over all route entrys
		// increase how often Routers, Links and Channels were used
		if(route.size() == 0) continue;
		std::stringstream test;

		//test << " Route:";
		outputFile << "edge [color=\"" << hueCounter <<",0.862,0.8549\"]" << endl;
		test << mod->getName() <<  mod->getIndex() ;

		if (route.size() == 1) {

			IPAddress addr = route[0]->addr.get4();
			clientDepRouter[addrMap[addr]].push_back(std::pair<IPvXAddress, std::string>(it->first,stripes[it->first]));
			routerUsage[addrMap[addr]]++;
			RouterCentrality[addrMap[addr]].collect(1);//!
			DEBUGOUT("Inserting Client: " << it->first << "depending on router " << addrMap[addr]);
			test << "->rt" << addrMap[addr];
            test << "->peerNode"  << cliMap[it->first.get4()] << ";";
			std::string test2;
			test >> test2;
			outputFile << test2 << endl;
			DEBUGOUT(test2);

		} else { // route.size() != 1

			for (unsigned int j = 0 ; j < route.size() - 1 ; j++) {

				// source
				IPAddress addr1 = route[j]->addr.get4();
				// and dest of this part of the route
				IPAddress addr2 = route[j+1]->addr.get4();

				// increment channel-usage
				cChannel * ch = getChannel(route[j]->addr.get4(),route[j+1]->addr.get4());
				channelUsage[ch]++;
				// link-usage
				increaseLinkUsage(addrMap[addr1],addrMap[addr2]);
				double halfroutesize = (double)(route.size() - 1)/2;

				if ( j == 0 ) {
                    DEBUGOUT("Inserting Client: " << it->first << " depending on router " << addrMap[addr1]);
					routerUsage[addrMap[addr1]]++;
					RouterCentrality[addrMap[addr1]].collect(fabs(halfroutesize-(double)(j))/halfroutesize);
					DEBUGOUT("Router: " << addr2 << " is Position: " << j << " of " << route.size()
							 << " Centrality: " << fabs(halfroutesize-(double)(j))/halfroutesize);
					clientDepRouter[addrMap[addr1]].push_back(std::pair<IPvXAddress,std::string>(it->first,stripes[it->first]));
				}

				routerUsage[addrMap[addr2]]++;
				RouterCentrality[addrMap[addr2]].collect(fabs(halfroutesize-(double)(j+1))/halfroutesize);
				DEBUGOUT("Router: " << addr2 << " is Position: " << j+1 << " of " << route.size()
						 << " Centrality: " << fabs(halfroutesize-(double)(j+1))/halfroutesize);
				DEBUGOUT("Inserting Client: " << it->first << "depending on router " << addrMap[addr2]);
				clientDepRouter[addrMap[addr2]].push_back(std::pair<IPvXAddress, std::string>(it->first,stripes[it->first]));
				clientDepChannel[ch].push_back(std::pair<IPvXAddress, std::string>(it->first,stripes[it->first]));

				test << "->rt" << addrMap[addr1];
			} // END for

			test << "->rt" << addrMap[route[route.size() - 1]->addr.get4()];
            test << "->peerNode"  << cliMap[it->first.get4()] << ";";
			std::string test2;
			test >> test2;
			outputFile << test2 << endl;
			DEBUGOUT(test2);

        } // route.size != 1
		routeCounter += route.size();
	} // END for
    DEBUGOUT(" usedRouterCounter now " << usedRouterCounter);
	usedRouterCounter +=  routeCounter;

} // void UnderlayAttackerModule::collectDataFromModule(cModule * mod)


/**
 * Increases the LinkUsage Counter for Link between Router 1 and Router 2
 *
 * @param	r1	Index of Router 1
 * @param	r2 	Index of Router 2
 */
void UnderlayAttackerModule::increaseLinkUsage(int r1, int r2) {
	if(r1 < r2) {
		linkUsage[std::pair<int,int>(r1,r2)]++;
	} else {
		linkUsage[std::pair<int,int>(r2,r1)]++;
	}
}

/**
 * Gets the pointer to the Channel between router 2 and 1
 *
 * @param	Start	IPAddress of Router 1
 * @param 	End	IPAddress of Router 2
 *
 * @return	cChannel * from Router Start to End
 */
cChannel * UnderlayAttackerModule::getChannel(IPAddress End, IPAddress Start) {
	return connectingChannels[std::pair<int,int>(addrMap[End],addrMap[Start])];
}

/**
 * Creates a map with Router IPs and their corresponding Index in the Routervector
 * and the Map of Routers and their connecting Links
 */
void UnderlayAttackerModule::collectNetworkData() {

	int index=0;
	// address map for routers
	while(getParentModule()->getSubmodule("rt",index)) {
		RoutingTable * rt = check_and_cast<RoutingTable*>(getParentModule()->getSubmodule("rt",index)->getSubmodule("routingTable"));
		IPAddress addr = rt->getRouterId();
		addrMap.insert(std::pair<IPAddress, int>(addr,index));
		index++;
	}

	index=0;
	// address map for clients
    while(getParentModule()->getSubmodule("peerNode",index)) {
        //FIXME we need to obtain ip of node here
        InterfaceTable* interface = check_and_cast<InterfaceTable*>(getParentModule()->getSubmodule("peerNode",index)->getSubmodule("interfaceTable"));
        IPAddress addr = interface->getFirstLoopbackInterface()->ipv4Data()->getIPAddress();
		cliMap.insert(std::pair<IPAddress, int>(addr,index));
		index++;
	}

	index=0;
	// map for Channels
	while(getParentModule()->getSubmodule("rt",index)) {

		int gc = 0; //gatecounter
		cModule * rt = check_and_cast<cModule*>(getParentModule()->getSubmodule("rt",index));

		// iterate over all gates
		for (cModule::GateIterator git(rt); !git.end(); git++) {

			cGate *gate = git();
			cGate* nextGate = gate->getNextGate();
			// filter out gates to the inside, else fill in Link map, info must contain different arrow ( --> , <-- )
			if(		strcmp(gate->getNextGate()->getOwnerModule()->getName(),"rt") == 0
					&& strcmp(gate->getOwnerModule()->getName(), nextGate->getOwnerModule()->getName()) == 0) {
				if(gate->getOwnerModule()->getIndex() == nextGate->getOwnerModule()->getIndex())
					throw cRuntimeError(" index error");
				// filter out channels to clients
				RouterNumbers b(gate->getNextGate()->getOwnerModule()->getIndex(),index);
				connectingChannels.insert(std::pair<RouterNumbers, cChannel *>(b,gate->getChannel()));
				channelPacketRate.push_back(std::pair<double,cChannel *>(dynamic_cast<cDatarateChannel *>(gate->getChannel())->getDatarate(),gate->getChannel()));
			}
			gc++;
		}
		routerDegree.push_back(std::pair<int,int>(gc,index));
		index++;
	}
}


/**
 * Compares parts of two strings that are
 * separated by space characters
 *
 * @param str1  first string
 * @param str2  second string
 * @param index the index to compare both strings
 * @return	bool true if the respective parts of
 *			the strings	are equal
 */
bool UnderlayAttackerModule::compareDirection(std::string& str1, std::string& str2, int index) {

	std::vector<std::string> strs1;
	std::vector<std::string> strs2;
	boost::split(strs1, str1, boost::is_any_of(" "));
	boost::split(strs2, str2, boost::is_any_of(" "));
	return (strs1[index] == strs2[index]);
}


/**
 *	Attacks the Channel with DoS
 *
 *	@param ch		Pointer to the Channel
 *	@param per		Packet Error Rate
 */
void UnderlayAttackerModule::attackChannel(TopologyModel& topo, cChannel * ch, float per) {

    std::pair<IPvXAddress, std::string > entry;
    foreach (entry, clientDepChannel[ch]) {
        DEBUGOUT("Removing edge from " << IPvXAddress(predecessors[entry.first.get4()][entry.second])<< " to "
                 << entry.first );
        topo.removeVertexRecursive(entry.second, entry.first);
    }
}

/**
 *	Attacks the Channel with DoS
 *
 *	@param ch		Pointer to the Channel
 *	@param per		Packet Error Rate
 */
int UnderlayAttackerModule::attackChannelOnce(TopologyModel& topo, cChannel* ch, ChannelList& combChannel, float per) {

	DEBUGOUT("Attacking Link with id: " << ch->getConnectionId());
	int damage = 0;
	int best = 0;
	IPvXAddress bestAddr;
	std::pair<IPvXAddress, std::string > entry;
	foreach (entry, combChannel) {

	  int succ = topo.countSuccessors(entry.first, entry.second);
	  if(best < succ) {
		  best = topo.countSuccessors(entry.first, entry.second);
		  bestAddr = entry.first;
	  }

	}
	foreach (entry, combChannel) {
	  damage += topo.removeVertexRecursive(entry.second, entry.first);
	}

	if(!bestAddr.isUnspecified()) {
		DEBUGOUT("Removing edge from " <<  IPvXAddress(predecessors[entry.first.get4()][entry.second])
				 << " to " << entry.first);
		// we don't observe the damage of the best subtree,
		// so subtract it
		damage -= best;
	}

	return damage;
}


/**
 *	Attacks the Link by a given amount of DoS PER,
 *	i.e. attack the 2 corresponding channels
 *
 *	@param r1		index of Router 1
 *	@param r2		index of Router 2
 *	@param per	DoS error Rate
 */
void UnderlayAttackerModule::attackLink(TopologyModel& topo, int r1, int r2, float packetErrorRate) {

	attackChannel(topo,connectingChannels[std::pair<int,int>(r1,r2)],packetErrorRate);
	attackChannel(topo, connectingChannels[std::pair<int,int>(r2,r1)],packetErrorRate);

}

/**
 *	Attacks the Link by a given amount of DoS PER,
 *	i.e. attack the 2 corresponding channels
 *
 *	@param r1		index of Router 1
 *	@param r2		index of Router 2
 *	@param per	DoS error Rate
 */
int UnderlayAttackerModule::attackLinkOnce(TopologyModel& topo, int r1, int r2, float packetErrorRate) {

	cChannel* ch1 = connectingChannels[std::pair<int,int>(r1,r2)];
	cChannel* ch2 = connectingChannels[std::pair<int,int>(r2,r1)];
	ChannelList comb = clientDepChannel[ch1];
	comb.merge(clientDepChannel[ch2]);

	return attackChannelOnce(topo,ch1,comb,packetErrorRate);
}

/**
 * Returns if one of the predecessors in the Stripe is already attacked
 * 
 * @param address	Address of the currently observed node
 * @param stripe	current stripe number
 */
bool UnderlayAttackerModule::predecessorIsAttacked(IPvXAddress address, std::string stripe) {
	bool attacked = false;
	IPAddress addr = address.get4();
	while (predecessors[addr].find(stripe) != predecessors[addr].end()) {
		if (alreadyUnderAttack.find(addr) != alreadyUnderAttack.end()) {
			attacked = true;
			break;
		}
		addr = predecessors[addr][stripe];
	}
	return attacked;
}

/**
 *	attacks given Router, i.e. set the packet error ratio of all channels to given value
 *
 *	@param router	index of the router
 *	@param packetErrorRate	packet error rate
 */
void UnderlayAttackerModule::attackRouter(TopologyModel& topo,int router, float packetErrorRate) {

		std::pair<IPvXAddress, std::string > entry;
		std::map<int,int> attackedClients;
		DEBUGOUT("Attacking Router: rt[" << router << "]");

		// obtain damage for all occurrences of the router "router" across all stripes and subtrees
		foreach (entry, clientDepRouter[router]) {

			DEBUGOUT("Removing edge from " <<  IPvXAddress(predecessors[entry.first.get4()][entry.second])
					 << " to " << entry.first);

			// removing edge recursives
			topo.removeVertexRecursive(entry.second, entry.first);

			// store client that is  attacked by packetErrorRate
			if( packetErrorRate != 0.0) {
				if(attackedClients.find(cliMap[entry.first.get4()]) == attackedClients.end())
					attackedClients[cliMap[entry.first.get4()]] = 1;
				else
					attackedClients[cliMap[entry.first.get4()]]++;
			}

		} // END foreach

}


/**
 * Calculates the number of nodes in the largest subtree incident to the given router
 *
 * @param topo		the topology to operate on
 * @param router	the router id
 */
int UnderlayAttackerModule::attackRouterOnce(TopologyModel& topo, int router) {

    DEBUGOUT("Attacking Router: rt[" << router << "]");
	int damage = 0;
	int best = 0;
    IPvXAddress bestAddr;
	//if(clientDepRouter.find(router) == clientDepRouter.end() || clientDepRouter[router].size() < 2)
	  //return 0;

	// obtain maximum damage of all occurrences of the router across all stripes and subtrees
	 std::pair<IPvXAddress, std::string > entry;
    foreach (entry, clientDepRouter[router]) {

		int succ = topo.countSuccessors(entry.first, entry.second);
		if(best < succ) {
			best = topo.countSuccessors(entry.first, entry.second);
            bestAddr = entry.first;
        }
    } // END foreach

	// and now attack the topology
	foreach (entry, clientDepRouter[router]) {
		damage += topo.removeVertexRecursive(entry.second, entry.first);
	}

    if(!bestAddr.isUnspecified()) {
        DEBUGOUT("Removing edge from " <<  IPvXAddress(predecessors[entry.first.get4()][entry.second])
                 << " to " << entry.first);
		// we don't observe the damage of the best subtree,
		// so subtract it
		damage -= best;
    }

	return damage;
}


/**
 * 	gets Client IP by given index
 *
 * 	@param	number	Index for the client module
 * 	@return	IPAddress of the Client
 */
IPAddress UnderlayAttackerModule::getClientIP(int number) {
	std::map<IPAddress, int>::iterator it;
	for (it = cliMap.begin();it != cliMap.end();it++) {
		if (it->second == number) return it->first;
	}
	return IPAddress();
}

/**
 * 	gets Router IP by given index
 *
 * 	@param	number	Index for the client module
 * 	@return	IPAddress of the Client
 */
IPAddress UnderlayAttackerModule::getRouterIP(int number) {
	std::map<IPAddress, int>::iterator it;
	for (it = addrMap.begin();it != addrMap.end();it++) {
		if (it->second == number) return it->first;
	}
	return IPAddress();
}


/**
 * Collects necessary results shortly before
 * the end of simulation
 */
void UnderlayAttackerModule::collectResults() {

	if (active) {

        TopologyModel topo = oT->getTopologyRef();
		recordScalar("remaining_nodes" , topo.countSuccessors(IPAddressResolver().resolve("srv[0]")));
		recordScalar("used_Links", usedLinks);
		recordScalar("used_Routers", usedRouterCounter);
		recordScalar("weighted_router_usage_gates", weightedrouterUsage);
		recordScalar("average_router_usage", ((double)usedRouterCounter/(double)addrMap.size()) );
        //recordScalar("vulnerabilityLinks" , calculateLinkVulnerability());
        //recordScalar("vulnerabilityLinks2" , calculateLinkVulnerability2());
        //recordScalar("vulnerabilityRouters" , calculateRouterVulnerability());
        //recordScalar("vulnerabilityRouters2" , calculateRouterVulnerability2());

	} // ! active
}


/**
 * Finishes the Module, records some Statistics
 */
void UnderlayAttackerModule::finish() {

}


/**
 * Calculate the Overlay Damage for attacking single routerss
 *
 * @return double		The arising damage by attacking this router
 */
double UnderlayAttackerModule::calculateRouterVulnerability() {

    const bool fullDamage = true;
    const bool partialDamage = false;
    assert((fullDamage && !partialDamage) || (!fullDamage && partialDamage));

	//collect the damage
	double overallDamage = 0.0;
    TopologyModel topo = oT->getTopology();

	int topoSize = topo.getNumRootSuccessors();
	//entry for the route-usage (| Router-ID| #Usage|)
	std::pair<int, int> entry;
    double counter = 0;
    // Iterate across all routers and calculate the damage
	foreach(entry, routerUsage) {

        if(entry.second == 0) continue;
		attackRouter(topo,entry.first,0.0);
        double damage = topoSize - topo.getNumRootSuccessors();

       // collect("uo_globalCollateralDamage",damage);

        routerCollateralDamage.insert(std::pair<double,int>(damage,entry.first));
		DEBUGOUT("Router: " << entry.first << " Damage " << damage);
		overallDamage += damage;
        topo = oT->getTopology();
        counter++;
    } //end foreach router

    return overallDamage / counter;
}


/**
 * Calculate the Vulnerability Metric: Global Subtree Collateral Damage for Routers
 *
 * @return double		The arising damage by attacking this router
 */
double UnderlayAttackerModule::calculateRouterVulnerability2() {

	//collect the damage
	double overallDamage = 0;
    TopologyModel topo = oT->getTopology();

	// obtain reference value
	std::map<std::string,int> ref;
    ref["0-0"] = topo.countSuccessors();

	const double numStripes = (double) ref.size();
	const double numNodes = (double) ref.begin()->second;
	//entry for the route-usage (| Router-ID| #Usage|)
	std::pair<int, int> entry;

    double counter = 0;
	// Iterate across all routers and calculate the damage
	foreach(entry, routerUsage) {
        // only used routers
        if(entry.second == 0) continue;
		double damage = ((double) attackRouterOnce(topo, entry.first)) / (numNodes * numStripes);
		overallDamage += damage;

        //collect("uo_subtreeCollateralDamage_router",damage);

        topo = oT->getTopology();
        counter++;
	} //end foreach router

    // overallDamage / divided by total number of used router
    return ( overallDamage / counter );
}


/**
 * Calculate the Vulnerability Metric for Links: Global Collateral Damage for Links
 *
 * @return double		The arising damage by attacking this link
 */
double UnderlayAttackerModule::calculateLinkVulnerability() {

	//collect the damage
	double overallDamage = 0.0;
    TopologyModel topo = oT->getTopology();
	//get reference value
	std::map<std::string,int> ref;
    ref["0-0"] = topo.countSuccessors();

	//entry for link-usage
	std::pair<std::pair<int,int>, int> entry;
	foreach(entry, linkUsage) {
		attackLink(topo,entry.first.first , entry.first.second,0.0);
		double damage = gatherDamage(topo, ref,true);

    //	collect("uo_globalCollateralDamage_links",damage);

        overallDamage += damage;
        topo = oT->getTopology();
	}//end foreach link

	return overallDamage/(double)linkUsage.size();
}


/**
 * Calculate the Vulnerability Metric for Links: Global Collateral Damage for Links
 *
 * @return double		The arising damage by attacking this link
 */
double UnderlayAttackerModule::calculateLinkVulnerability2() {

	//collect the damage
	double overallDamage = 0.0;
    TopologyModel topo = oT->getTopology();
	//get reference value
	std::map<std::string,int> ref;
    ref["0-0"] = topo.countSuccessors();

	const double numStripes = (double) ref.size();
	const double numNodes = (double) ref.begin()->second;
	//entry for link-usage
	std::pair<std::pair<int,int>, int> entry;
	foreach(entry, linkUsage) {
		double damage = ((double) attackLinkOnce(topo,entry.first.first,entry.first.second,0.0))
										/ (numNodes * numStripes);
    //	collect("uo_subtreeCollateralDamage_links",damage);

		overallDamage += damage;
        topo = oT->getTopology();
	}//end foreach link

	return overallDamage / (double) linkUsage.size();
}


/**
  * collect the attack damage of the before applied attack 
  * 
  * @param ref			the reference node count collected before the attack
  * @param global	get global damage or local (faculty or not)
  *
  * @return double	the actual damage of the attacked router
  */
double UnderlayAttackerModule::gatherDamage(TopologyModel& topo, std::map<std::string,int>& ref, bool global) {

    std::multimap<int,std::string> damageMap;
	//iterate over all stripes and collect the caused damage

    int damage = ref["0-0"] - topo.countSuccessors();
    assert(damage >= 0);
    //insert damage into map
    damageMap.insert(std::pair<int,std::string>(damage,"0-0"));

    //start with biggest entry
    std::multimap<int,std::string>::reverse_iterator it;
	int damageAkkumulator = 0;
	int counter = 1;

	//multiply the damage by the faculty of its rank in the damage list
	for(it = damageMap.rbegin(); it != damageMap.rend() ; it++,counter++)
		damageAkkumulator += (counter - 1) * it->first;

	const int numStripes = ref.size();
	const int numNodes = ref.begin()->second;
	return (double) damageAkkumulator / (double)((double) numNodes * ((double) numStripes - 1));
}


/**
 * Calcutates faculty of int i
 */
int UnderlayAttackerModule::faculty(int i) {
	return i<1 ? 1 : faculty(i-1) * i;
}
