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

#ifndef UNDERLAYATTACKER_H_
#define UNDERLAYATTACKER_H_

#include <fstream>
#include <omnetpp.h>
#include "TracerouteTable.h"
#include "PPDatatypes.h"

class OverlayTopology;
class TopologyModel;

class UnderlayAttackerModule: public cSimpleModule {
	
protected:
	//some variables
	enum AttackType {CrucialRouters, CrucialLinks, CrucialChannels, RandomRouters, RandomLinks, RandomChannels, WeakestChannels, BiggestNodeDegree, OverlayConnectionDependend, DamageDependend};	
	AttackType attackType;
	
	bool active;
	int attackLength;
	int startAttack;
	int victimNumber;
	int attackedNodes;
	
	//parameters
	bool firstAttack;
	bool simulateAttack;
	double perStripePacketrate;
	unsigned int packetSize;
	double attackerPacketrate;
	double attackPauseRatio;
	double truncateDamagePercentage;
    double truncateDamageMax;
    double truncateDamageStep;
	bool performDamageTest;
	
	std::ofstream outputFile;
	
	OverlayTopology* oT;
	
	double hueCounter;
	std::set<IPAddress> alreadyUnderAttack;
	// the datatypes to store information collected from network in it
    typedef PPIntIntPair RouterNumbers;
    typedef std::list<PPIPvXAddressStringPair> ChannelList;
	// | int | list (IPvXAddress, std::string) |
        typedef std::map<int, std::list<PPIPvXAddressStringPair> > ClientsDependingOnRouter;
	// | cChannel* | list(IPvXAddress, std::string) |
	typedef std::map<cChannel *, ChannelList > ClientsDependingOnChannel;
	
	//which clients depend on which network part
	ClientsDependingOnRouter clientDepRouter;
	ClientsDependingOnChannel clientDepChannel;

	//channels between two routers
	std::map<RouterNumbers, cChannel *> connectingChannels;
	
	//mapping of routers to their index
	std::map<IPAddress, int> addrMap;
	
	//mapping of clients to their index
	std::map<IPAddress, int> cliMap;
	
	// mapping of Router Connections to index
	std::vector<std::pair<int, int> > routerDegree;
	
	//packetrate of each channel
	std::vector<std::pair<double, cChannel *> > channelPacketRate;
	
	//for usage statistic of channels, links and routers
	void increaseLinkUsage(int r1, int r2);
	// | Router-ID | Router-ID | #Usage |
	std::map<std::pair<int, int>, int> linkUsage;
	std::map<cChannel *, int> channelUsage;
	// | Router-ID | #Usage |
	std::map<int, int> routerUsage;
	// | Damage | Router-ID |
    std::multimap<double, int> routerCollateralDamage;
	std::map<int, cStdDev> RouterCentrality;

	//predecessor
	std::map<IPAddress,std::map<std::string, IPAddress> > predecessors;
	
	//routes from used links
	std::map<std::pair<IPAddress, IPAddress>,TracerouteTable::Route> linkRoutes;
	
	//statistics
	int usedRouterCounter;
	double weightedrouterUsage;
	int usedLinks;
	std::map<std::pair<int,int>,cStdDev> stripeCorrelationRouters;
	std::map<std::pair<int,int>,cStdDev> stripeCorrelationRoutersWeighted;
	std::map<std::pair<int,int>,cStdDev> stripeCorrelationLinks;
	std::map<std::pair<int,int>,cStdDev> stripeCorrelationLinksWeighted;
	//messages to start and stop attacks
	cMessage * start_timer;
	cMessage * stop_timer;

protected:
	double calculateRouterVulnerability();
	double calculateRouterVulnerability2();
	double calculateLinkVulnerability(); // GCD
	double calculateLinkVulnerability2(); // GSCD
	double gatherDamage(TopologyModel& topo, std::map<std::string,int>& ref, bool global);
	void startUnderlayAttacker();

	//attack stuff
    void performAttack(unsigned int number = 1, float errorRatio = 1);
    void performAttack(unsigned int number, float errorRatio, TopologyModel& topo);

	void attackChannel(TopologyModel& topo, cChannel * ch, float per);
	int attackChannelOnce(TopologyModel& topo,cChannel* ch, ChannelList& combChannel, float per);
	void attackLink(TopologyModel& topo, int r1, int r2, float packetErrorRate);
	int attackLinkOnce(TopologyModel& topo, int r1, int r2, float packetErrorRate);
	void attackRouter(TopologyModel& topo, int router, float packetErrorRate);
	int attackRouterOnce(TopologyModel& top, int router);
	std::set<int>attackRateSet;
	bool predecessorIsAttacked(IPvXAddress address, std::string stripe);
	
	//collect data
	void collectNetworkData();
	void collectLinkData();
	void collectDataFromModule(cModule* mod);
	void collectResults();
	void printGatesHistogramm();
	void printUsageHistogramm();
	
	//useful functions
	bool compareDirection(std::string& str1, std::string& str2, int index);
	cChannel* getChannel(IPAddress addr1, IPAddress addr2);
	IPAddress getClientIP(int number);
	IPAddress getRouterIP(int number);
	int faculty(int i);

public:
	//standard methods
	void handleMessage(cMessage * msg);
	void initialize(int stage);
	void finish();	
	virtual int numInitStages() const {
		return 5;
	}
};

#endif /* UNDERLAYATTACKER_H_ */
