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
// UnderlayAttackerModule.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "UnderlayAttackerModule.h"
#include "IPAddressResolver.h"
#include "OverlayTopology.h"
#include "RoutingTable.h"
#include "InterfaceTable.h"
#include "IPv4InterfaceData.h"

#include <InterfaceTableAccess.h>

#include "TopologyModel.h"
#include <boost/algorithm/string.hpp>

Define_Module(UnderlayAttackerModule);

#if _DEBUG
# define DEBUGOUT(x) if (C_DEBUG_UnderlayAttacker) std::cout << "UnderlayAttacker: (UA@" << simTime() << "): " << x << endl
#else
# define DEBUGOUT(x)
#endif

void UnderlayAttackerModule::initialize(int stage)
{
   if (stage == 0)
   {
      sig_damage = registerSignal("Signal_Damage");

      sig_remaining_nodes              = registerSignal("Signal_remaining_nodes");;
      sig_used_Links                   = registerSignal("Signal_used_Links");;
      sig_used_Routers                 = registerSignal("Signal_used_Routers");;
      sig_weighted_router_usage_gates  = registerSignal("Signal_weighted_router_usage_gates");;
      sig_average_router_usage         = registerSignal("Signal_average_router_usage");;
   }

   if (stage != 3) return;

   //fetch parameters
   active = par("active");
   if (!active) return;

   startAttack = par("startAttack");
   victimNumber = par("victimNumber");
   truncateDamagePercentage = 0;
   performDamageTest = par("performDamageTest");
   truncateDamageMax = par("truncateDamageMax").doubleValue();
   truncateDamageStep = par("truncateDamageStep").doubleValue();

   // -- Binding to the global module Topology Observer
   cModule *temp = simulation.getModuleByPath("topoObserver");
   oT = check_and_cast<OverlayTopology*>(temp);

   firstAttack = true;
   hueCounter = 0;
   weightedrouterUsage = 0;
   usedRouterCounter = 0;
   usedLinks = 0;
   numRouters = 0;

   //schedule timer to start attack
   start_timer = new cMessage("StartAttack");
   scheduleAt(par("simTimeLimit").doubleValue() - 3, start_timer);
   scheduleAt(par("simTimeLimit").doubleValue() - 2, new cMessage("CollectResults"));
}

void UnderlayAttackerModule::handleMessage(cMessage* msg)
{
   if(msg == start_timer)
   {
      startUnderlayAttacker();
      start_timer = NULL;

   }
   else if(strcmp(msg->getName(), "CollectResults") == 0)
   {
      collectResults();
   }

   delete msg;
   msg = NULL;
}

void UnderlayAttackerModule::startUnderlayAttacker()
{
   attackedNodes = 0;

   collectNetworkData();
   collectLinkData();

   DEBUGOUT("start underlay attack" );

   assert(oT != NULL);

   TopologyModel topo = oT->getTopology();
   // first obtain the router vulnerability, since it is required for
   // DamageDependend - damage calculation
   calculateRouterVulnerability();
   int numNodes = topo.getNumRootSuccessors();

   //std::cout <<" Start underlay Attack with " << numNodes << " nodes " <<endl;

   if(performDamageTest)
   {

      for (int i = 1 ; i < 11 ; i++ )
      {
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
         while (j <= truncateDamageMax)
         {
            truncateDamagePercentage = j;
            //std::cout << " *** Perform Attack with i,j " << i << "," << j << endl;
            performAttack(i,0.5,topo);
            double remainingNodes = topo.getNumRootSuccessors();
            double relDamage = ((double) numNodes - remainingNodes) / (double)numNodes;
            vector.recordWithTimestamp( ((double)j), remainingNodes);
            damage.recordWithTimestamp(((double)j), relDamage);

            // -- added by Giang
            emit(sig_damage, relDamage);

            if(j == 0)
            {
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
         for(it=damageDistrMap.begin(); it != damageDistrMap.end();it++) {
             //std::cout << " record with timestamp " << it->first << " and " << it->second << endl;
             damageDistribution.recordWithTimestamp(it->first,it->second);
         }
      } // for
   } // if

   DEBUGOUT(topo.getRoot());
   DEBUGOUT("Nodes:"  << topo.countSuccessors(IPAddressResolver().resolve("srv[0]")));

   // FIXME-Giang
   recordScalar("vulnerabilityRouters2" , calculateRouterVulnerability2());

} // startUnderlayAttacker()


void UnderlayAttackerModule::performAttack(unsigned int number, float errorRatio)
{
   TopologyModel topo = oT->getTopology();
   performAttack(number, errorRatio, topo);
}
/**
 * Peforms an attack on Links, Routers or Channels
 *
 * @param	number		number of entities to be attacked
 * @param	errorRatio	error ratio for the chosen entities (0..1)
 */
void UnderlayAttackerModule::performAttack(unsigned int number, float errorRatio, TopologyModel& topo)
{
   if (number < 1) return;

   //build vector with Routers to sort them by usagecount
   std::vector<std::pair<double, int> > dummyVector;
   std::multimap<double, int>::reverse_iterator it;

   // create vector of routers to be attacked
   double counter = 1;
   double size =routerCollateralDamage.size();
   for (it = routerCollateralDamage.rbegin() ; it != routerCollateralDamage.rend() ; it++)
   {
      DEBUGOUT(it->second  <<" - " << it->first << " - " << routerUsage[it->second]);
      if( (counter / size) >= truncateDamagePercentage )
      {
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
   for (unsigned int j = 0 ; j < maxValue; j++ )
   {
      DEBUGOUT("Attacking Router : " << dummyVector.back().second << " with damage " << dummyVector.back().first );
     // std::cout << "Attacking Router : " << dummyVector.back().second << " with damage " << dummyVector.back().first << endl;
      attackRouter(topo,dummyVector.back().second , errorRatio);
      dummyVector.pop_back();
   }

} // END performAttack(enum AttackType at, unsigned int number, float errorRatio)

/**
 * Iterates over all Clients and the Server, to get the routes from,
 * collects Channel, Link and Routerusage and produces fancy topology picture
 */
void UnderlayAttackerModule::collectLinkData()
{
   Enter_Method("collectLinkData");

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

   // -- clients
   i = 0;
   while(getParentModule()->getSubmodule("peerNode",i)) {
      collectDataFromModule(getParentModule()->getSubmodule("peerNode",i));
      i++;
      hueCounter += hue;
   }

   // TODO-Giang:
   // Generalize to support an array of video source nodes
   if (getParentModule()->getSubmodule("sourceNode",0)) {
      collectDataFromModule(getParentModule()->getSubmodule("sourceNode",0));
   }


   outputFile << "}" << endl;
   outputFile.close();

   std::map<std::pair<int, int>, int>::iterator it;
   for (it = linkUsage.begin(); it != linkUsage.end() ; it++)
   {
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
void UnderlayAttackerModule::collectDataFromModule(cModule * mod)
{
   Enter_Method("collectDataFromModule");

   cModule* temp = mod->getModuleByRelativePath("networkLayer")->getModuleByRelativePath("trTable");
   TracerouteTable* rt = check_and_cast<TracerouteTable*>(temp);
   TracerouteTable::RouteMap rm = rt->getRoutes();
   TracerouteTable::RouteMap::iterator it;

   InterfaceTable* interface = check_and_cast<InterfaceTable*>(mod->getSubmodule("interfaceTable"));
   EV << "Number of interfaces: " << interface->getNumInterfaces() << endl;

   IPvXAddress selfAddr;
   if (interface->getNumInterfaces() > 1)
   {
      selfAddr = IPvXAddress(interface->getInterface(interface->getNumInterfaces()-1)->ipv4Data()->getIPAddress());
   }
   else
   {
      selfAddr = IPvXAddress(interface->getInterface(0)->ipv4Data()->getIPAddress());
   }


   EV << "self address: " << selfAddr << endl;
   DEBUGOUT(" - collectDataFromModule " << selfAddr);
   std::cout << "- collectDataFromModule " << selfAddr << endl;
   assert(!rm.empty());
   assert(!selfAddr.isUnspecified());

   std::vector<IPAddress> vec;
   std::map<IPvXAddress, PPStringSet> stripes;
   TopologyModel topo = oT->getTopology();

   PPStringSet topoStripes = topo.getStripes();
   foreach(std::string stripe, topoStripes) {

        //std::cout <<" collecting date for stripe " << stripe << endl;
        PPIPvXAddressSet children = topo.getChildren(selfAddr,stripe);
        foreach(IPvXAddress ip, children) {
            //we only need to cover the outbound links
            //std::cout <<" taking into accoung child " << ip << " in stripe " << stripe << endl;
            vec.push_back(ip.get4());
            stripes[ip].insert(stripe);
        }

        IPvXAddress pred = topo.getPredecessor(selfAddr,stripe);
        if(!pred.isUnspecified()) {
            //std::cout << " insert into predecessor map " << selfAddr << " <- " << pred << endl;
            predecessors[selfAddr][stripe] = pred;
        } /*else{
            std::cout << " ARGH, for node " << selfAddr << " predecessor is unspecified" << endl;
        }*/

   }

   // Counter for usage of underlay components
   int routeCounter = 0;

   for (it = rm.begin(); it != rm.end();it++) {

      //if destination is not in linkvector continue
      // DEBUG
      //rt->printRoute(it->second);

      bool isdest = false;
      foreach(IPAddress destaddr,vec) {

        if(destaddr == it->first.get4()) {
            isdest = true;
            break;
         }
      }
      if(!isdest) continue;
     // std::cout << "  - check destination " << it->first << endl;

      TracerouteTable::Route route = it->second;
      linkRoutes[std::pair<IPAddress,IPAddress>(selfAddr.get4(),it->first.get4())] = route;
      // iterate over all route entrys
      // increase how often Routers, Links and Channels were used
      if(route.size() == 0) continue;
      std::stringstream test;

      //test << " Route:";
      outputFile << "edge [color=\"" << hueCounter <<",0.862,0.8549\"]" << endl;
      test << mod->getName() <<  mod->getIndex() ;

      if (route.size() == 1) {

         IPAddress addr = route[0]->addr.get4();

         foreach(std::string s, stripes[it->first]) {
            clientDepRouter[addrMap[addr]].push_back(std::pair<IPvXAddress, std::string>(it->first,s));
            routerUsage[addrMap[addr]]++;
            //std::cout << "Inserting Client: " << it->first << " depending on router " << addrMap[addr] << " in stripe " << s << endl;
         }


      } else { // route.size() != 1

          for (unsigned int j = 0 ; j < route.size() - 2 ; j++) {

            // source
            IPAddress addr1 = route[j]->addr.get4();
            // and dest of this part of the route
            IPAddress addr2 = route[j+1]->addr.get4();

            // link-usage
            increaseLinkUsage(addrMap[addr1],addrMap[addr2]);

            if ( j == 0 ) {

               foreach(std::string s, stripes[it->first]) {
                  clientDepRouter[addrMap[addr1]].push_back(std::pair<IPvXAddress,std::string>(it->first,s));
                  routerUsage[addrMap[addr1]]++;
                  //std::cout << "Inserting Client: " << it->first << " depending on router " << addrMap[addr1] << " in stripe " << s <<endl;
               }

            }

            foreach(std::string s, stripes[it->first]) {
               clientDepRouter[addrMap[addr2]].push_back(std::pair<IPvXAddress, std::string>(it->first,s));
               routerUsage[addrMap[addr2]]++;
               //std::cout << "Router: " << addr2 << " is Position: " << j+1 << " of " << route.size() << " depending on " << addrMap[addr2] << " in stripe " << s <<endl;
            }

         } // END for

      } // route.size != 1
      //foreach(std::string s, stripes[it->first])
      assert(!stripes[it->first].empty());
      routeCounter += route.size() * stripes[it->first].size();
   } // END for
   DEBUGOUT(" usedRouterCounter now " << usedRouterCounter);
   usedRouterCounter +=  routeCounter;

   assert((int)addrMap.size() == numRouters);

} // void UnderlayAttackerModule::collectDataFromModule(cModule * mod)


/**
 * Increases the LinkUsage Counter for Link between Router 1 and Router 2
 *
 * @param	r1	Index of Router 1
 * @param	r2 	Index of Router 2
 */
void UnderlayAttackerModule::increaseLinkUsage(int r1, int r2)
{
   if(r1 < r2)
   {
      linkUsage[std::pair<int,int>(r1,r2)]++;
   }
   else
   {
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
cChannel * UnderlayAttackerModule::getChannel(IPAddress End, IPAddress Start)
{
   return connectingChannels[std::pair<int,int>(addrMap[End],addrMap[Start])];
}

/**
 * Creates a map with Router IPs and their corresponding Index in the Routervector
 * and the Map of Routers and their connecting Links
 */
void UnderlayAttackerModule::collectNetworkData()
{
   int index=0;
   // address map for routers
   while(getParentModule()->getSubmodule("rt",index))
   {
      RoutingTable * rt = check_and_cast<RoutingTable*>(getParentModule()->getSubmodule("rt",index)->getSubmodule("routingTable"));
      IPAddress addr = rt->getRouterId();
      EV << "router id: " << addr << endl;
      addrMap.insert(std::pair<IPAddress, int>(addr,index));
      index++;
      numRouters++;
   }

   index=0;
   // address map for clients
   while(getParentModule()->getSubmodule("peerNode",index))
   {
      IPAddress addr;
      InterfaceTable* interface = check_and_cast<InterfaceTable*>(getParentModule()->getSubmodule("peerNode",index)->getSubmodule("interfaceTable"));
      EV << "Number of interfaces: " << interface->getNumInterfaces() << endl;

      if (interface->getNumInterfaces() > 1)
      {
         addr = interface->getInterface(interface->getNumInterfaces()-1)->ipv4Data()->getIPAddress();
      }
      else
      {
         addr = interface->getInterface(0)->ipv4Data()->getIPAddress();
      }
      assert(!addr.isUnspecified());
      EV << "address of node " << index << ": " << addr << endl;

      cliMap.insert(std::pair<IPAddress, int>(addr,index));
      index++;
   }

   index=0;
   // map for Channels
   while(getParentModule()->getSubmodule("rt",index))
   {
      int gc = 0; //gatecounter
      cModule * rt = check_and_cast<cModule*>(getParentModule()->getSubmodule("rt",index));

      // iterate over all gates
      for (cModule::GateIterator git(rt); !git.end(); git++)
      {
         cGate *gate = git();
         cGate* nextGate = gate->getNextGate();
         // filter out gates to the inside, else fill in Link map, info must contain different arrow ( --> , <-- )
         if(		strcmp(gate->getNextGate()->getOwnerModule()->getName(),"rt") == 0
                  && strcmp(gate->getOwnerModule()->getName(), nextGate->getOwnerModule()->getName()) == 0)
         {
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
   } // while
   assert((int)addrMap.size() == numRouters);
}


/**
 *	attacks given Router, i.e. set the packet error ratio of all channels to given value
 *
 *	@param router	index of the router
 *	@param packetErrorRate	packet error rate
 */
void UnderlayAttackerModule::attackRouter(TopologyModel& topo,int router, float packetErrorRate)
{
   std::pair<IPvXAddress, std::string > entry;
   std::map<int,int> attackedClients;
   DEBUGOUT("Attacking Router: rt[" << router << "]");
   //std::cout << "Attacking Router: rt[" << router << "]" << endl;


   // obtain damage for all occurrences of the router "router" across all stripes and subtrees
   foreach (entry, clientDepRouter[router]) {

      assert(predecessors.find(entry.first) != predecessors.end());
      //std::cout << "Removing edge from " <<  IPvXAddress(predecessors[entry.first][entry.second]) << " to " << entry.first << endl;

      assert(!IPvXAddress(predecessors[entry.first][entry.second]).isUnspecified());

      // removing edge recursives
      //topo.removeEdgesRecursive(IPvXAddress(predecessors[entry.first][entry.second]), entry.first);
      topo.removeVertexRecursive(entry.second, entry.first);

      // store client that is  attacked by packetErrorRate
      if( packetErrorRate != 0.0)
      {
         if(attackedClients.find(cliMap[entry.first.get4()]) == attackedClients.end())
            attackedClients[cliMap[entry.first.get4()]] = 1;
         else
            attackedClients[cliMap[entry.first.get4()]]++;
      }
   } // END foreach
}


/**
 * Collects necessary results shortly before
 * the end of simulation
 */
void UnderlayAttackerModule::collectResults()
{
   if (active)
   {
      TopologyModel topo = oT->getTopologyRef();
      emit(sig_remaining_nodes, topo.countSuccessors(IPvXAddress("192.168.0.1")));
      emit(sig_used_Links, usedLinks);
      emit(sig_used_Routers, usedRouterCounter);
      emit(sig_weighted_router_usage_gates, weightedrouterUsage);
      emit(sig_average_router_usage, ((double)usedRouterCounter/(double)numRouters));
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
double UnderlayAttackerModule::calculateRouterVulnerability()
{
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
      //std::cout<< " Router: " << entry.first << " Damage " << damage << endl;
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
    PPStringSet stripes = topo.getStripes();
    foreach(std::string stripe, stripes) {
        ref[stripe] = topo.countSuccessors(stripe);
    }

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
        topo = oT->getTopology();
        counter++;
    } //end foreach router

    //std::cout << " gscd = " << ( overallDamage / counter ) << endl;
    // overallDamage / divided by total number of used router
    return ( overallDamage / counter );
}


/**
 * Calculates the number of nodes in the largest subtree incident to the given router
 *
 * @param topo		the topology to operate on
 * @param router	the router id
 */
int UnderlayAttackerModule::attackRouterOnce(TopologyModel& topo, int router) {

    //std::cout << "Attacking Router: rt[" << router << "]" <<endl;
    int damage = 0;
    int best = 0;
    IPvXAddress bestAddr;
    //if(clientDepRouter.find(router) == clientDepRouter.end() || clientDepRouter[router].size() < 2)
      //return 0;

    // obtain maximum damage of all occurrences of the router across all stripes and subtrees
    std::pair<IPvXAddress, std::string > entry;
    foreach (entry, clientDepRouter[router]) {

        int succ = topo.countSuccessors(entry.first, entry.second);
        //std::cout <<"  - damage is " << succ << endl;
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
        DEBUGOUT("Removing edge from " <<  IPvXAddress(predecessors[entry.first][entry.second])
                 << " to " << entry.first);
        // we don't observe the damage of the best subtree,
        // so subtract it
        damage -= best;
        //std::cout <<" = gscd damage for router is " << damage << endl;
    }

    return damage;
}
