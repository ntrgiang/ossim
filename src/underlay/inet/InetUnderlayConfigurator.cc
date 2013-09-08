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
//
// -----------------------------------------------------------------------------
// InetUnderlayConfigurator.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

/*
 * InetUnderlayConfigurator.cc
 *
 *  Created on: March 26, 2010
 *      Author: mathias
 */

#include <algorithm>
#include "IRoutingTable.h"
#include "IInterfaceTable.h"
#include "IPAddressResolver.h"
#include "InetUnderlayConfigurator.h"
#include "InterfaceEntry.h"
#include "InterfaceTable.h"
#include "IPv4InterfaceData.h"

#include <assert.h>

Define_Module(InetUnderlayConfigurator)

//#if _DEBUG
//# define //DEBUGOUT(x) if (C_DEBUG_InetUnderlayConfigurator) { std::cout << " (InetUnderlayConfigurator@" << simTime() << "): " << x << endl; };
//#else
//# define //DEBUGOUT(x)
//#endif

void InetUnderlayConfigurator::initialize(int stage) {

   if (stage == 2)
   {
      m_numTerminals = 0;
      m_numSources = 0;
      numIPNodes = 1;

      param_numSources  = (int) par("nSources").longValue();
      param_numPeers       = (int) par("nPeers").longValue();
      m_minAccessDelay = par("minAccessDelay").doubleValue();
      m_maxAccessDelay = par("maxAccessDelay").doubleValue();
      noneTransitNodesAccessRouters = par("noneTransitNodesAccessRouters").boolValue();

      // Get the router number
      cTopology topo2("topo2");
      topo2.extractByProperty("node");
      param_numRouters = topo2.getNumNodes();
      ////DEBUGOUT("discovered " << param_numRouters << " routers");

      // store all access router in a vector
      getAccessRouter(); // giang

      // -- obsolete
      // add sources and initialize them to stage 2
      //      initSources();// giang

      // extract topology into the cTopology object, then fill in
      // isIPNode, rt and ift members of nodeInfo[]
              extractTopology(m_topo, m_nodeInfo);// giang

      // assign addresses to IP nodes, and also store result in nodeInfo[].address
              assignAddresses(m_topo, m_nodeInfo);// giang

      // add default routes to hosts (nodes with a single attachment);
      // also remember result in m_nodeInfo[].usesDefaultRoute
              addDefaultRoutes(m_topo, m_nodeInfo);// giang

      // calculate shortest paths, and add corresponding static routes
              fillRoutingTables(m_topo, m_nodeInfo);// giang

      // update display string
              setDisplayString(m_topo, m_nodeInfo);// giang

      // Schedule the creation of overlay nodes
              scheduleAt(simTime(), new cMessage("CreateAllNodes")); // giang
      //scheduleAt(simTime() + 1, new cMessage("CreateNode"));

   }
   //   if (stage == 5)	checkRessources(); // checks available streaming capacity

   WATCH(param_numSources);
   WATCH(param_numPeers);
}

void InetUnderlayConfigurator::finish()
{
   FlatNetworkConfigurator::finish();
}

void InetUnderlayConfigurator::handleMessage(cMessage* msg)
{
   assert(msg->isSelfMessage());

   if(strcmp(msg->getName(),"CreateAllNodes") == 0)
   {
      EV << "Going to create all nodes ..." << endl;
      //DEBUGOUT("CreateAllNodes path");

      initSources();// giang

      //create all nodes at once
      initPeers();

      delete msg;
      msg = NULL;

   }
   else
   {
      throw cRuntimeError("msg with no use arrived");
   }
}

/**
 * Assigns default routes to all IP nodes in topo
 *
 * @param topo			The topology
 * @param nodeInfo		A vector containing information of all nodes in topology
 */
void InetUnderlayConfigurator::addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo) {

   // add default route to nodes with exactly one (non-loopback) interface
   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      cTopology::Node *node = topo.getNode(i);
      // -- skip bus types
      if (!nodeInfo[i].isIPNode)
         continue;

      IInterfaceTable *ift = nodeInfo[i].ift;
      IRoutingTable *rt = nodeInfo[i].rt;
      // -- count non-loopback interfaces
      int numIntf = 0;
      InterfaceEntry *ie = NULL;
      for (int k = 0; k < ift->getNumInterfaces(); k++)
         if (!ift->getInterface(k)->isLoopback())
         {
            ie = ift->getInterface(k);
            numIntf++;
         }

      nodeInfo[i].usesDefaultRoute = ((numIntf == 1) && (strcmp(node->getModule()->getName(),"rt") != 0) );
      if (numIntf != 1)
         continue; // only deal with nodes with one interface plus loopback

      //DEBUGOUT("  " << node->getModule()->getFullName() << "=" << nodeInfo[i].address
               //<< " has only one (non-loopback) interface, adding default route");

      // add route
      IPRoute *e = new IPRoute();
      e->setHost(IPAddress());
      e->setNetmask(IPAddress());
      e->setInterface(ie);
      e->setType(IPRoute::REMOTE);
      e->setSource(IPRoute::MANUAL);
      rt->addRoute(e);
   }
}


/**
 * Assigns default route to handovered overlayNode
 * FIXME function should take over the functionality of
 * adding the default route in
 * fillRoutingTables(cModule* terminal, IPAddress destAddr)
 *
 * @param topo			The topology
 * @param nodeInfo		A vector containing information of all nodes in topology
 * @param overlayNode	the new node
 */
void InetUnderlayConfigurator::addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode) {
   // add default route to nodes with exactly one (non-loopback) interface
   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      /// skip bus types
      if (!nodeInfo[i].isIPNode)
         continue;
      cTopology::Node *node = topo.getNode(i);
      if (node->getModule() == overlayNode)
      {
         IInterfaceTable *ift = nodeInfo[i].ift;
         IRoutingTable *rt = nodeInfo[i].rt;
         // count non-loopback interfaces
         int numIntf = 0;
         bool same = true;
         IPAddress address;
         bool first = true;

         InterfaceEntry *ie = NULL;
         for (int k = 0; k < ift->getNumInterfaces(); k++)
         {
            if (!ift->getInterface(k)->isLoopback())
            {
               ie = ift->getInterface(k);
               numIntf++;
               if (!first && (address != ift->getInterface(k)->ipv4Data()->getIPAddress()))
               {
                  same = false;
               }
               first = false;
               address = ift->getInterface(k)->ipv4Data()->getIPAddress();
            }
         }

         if (nodeInfo[i].usesDefaultRoute)
            nodeInfo[i].usesDefaultRoute = ((numIntf == 1) or !same);

         if (numIntf != 1)
            continue; // only deal with nodes with one interface plus loopback

         //DEBUGOUT(node->getModule()->getFullName() << "=" << nodeInfo[i].address << " has only one (non-loopback) interface, adding default route)");
         // add route
         IPRoute *e = new IPRoute();
         e->setHost(IPAddress());
         e->setNetmask(IPAddress());
         e->setInterface(ie);
         e->setType(IPRoute::REMOTE);
         e->setSource(IPRoute::MANUAL);
         rt->addRoute(e);
      }
   }
}

/**
 * Assigns adresses to all IP-nodes in topo
 *
 * @param topo		The topology
 * @param nodeInfo  A vector containing information of all nodes in topology
 */
void InetUnderlayConfigurator::assignAddresses(cTopology& topo,	NodeInfoVector& nodeInfo)
{
   // assign IP addresses
   uint32 networkAddress = IPAddress(par("networkAddress").stringValue()).getInt();
   uint32 netmask = IPAddress(par("netmask").stringValue()).getInt();
   int maxNodes = (~netmask) - 1; // 0 and ffff have special meaning and cannot be used
   if (topo.getNumNodes() > maxNodes)
      error("netmask too large, not enough addresses for all %d nodes",
            topo.getNumNodes());

   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      // skip bus types and nodes that already have an IP address
      if (!nodeInfo[i].isIPNode || !nodeInfo[i].address.isUnspecified())
         continue;

      uint32 addr;
      cModule* node = topo.getNode(i)->getModule();
      if (strcmp(node->getName(), "sourceNode") == 0)
      {
         addr = networkAddress | uint32(1 + node->getIndex());
         //numIPNodes++;
      }
      else
         addr = networkAddress | uint32(++numIPNodes + param_numSources);

      //DEBUGOUT("address " << IPAddress(addr)<< " for "<< topo.getNode(i)->getModule()->getFullName());
      // +1 to reserve the x.x.x.1 for the source
      //uint32 addr = networkAddress | uint32(++numIPNodes);
      nodeInfo[i].address.set(addr);
      // find interface table and assign address to all (non-loopback) interfaces
      IInterfaceTable *ift = nodeInfo[i].ift;
      for (int k = 0; k < ift->getNumInterfaces(); k++)
      {
         InterfaceEntry *ie = ift->getInterface(k);
         if (!ie->isLoopback())
         {
            ie->ipv4Data()->setIPAddress(IPAddress(addr));
            ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS); // full address must match for local delivery
         }
      }
   }
}

/**
 * Assigns an address to a new terminal
 *
 * @param terminal	Pointer to the module of the terminal
 * @return the new IPAddress of the terminal
 */
IPAddress InetUnderlayConfigurator::assignPeerAddress(cModule* terminal)
{
   Enter_Method("assignPeerAddress");
   // assign IP addresses
   uint32 networkAddress = IPAddress(par("networkAddress").stringValue()).getInt();
   uint32 netmask = IPAddress(par("netmask").stringValue()).getInt();
   uint32 addr;

   if (strcmp(terminal->getName(), "sourceNode") == 0)
   {
      addr = networkAddress | uint32(1 + terminal->getIndex());
   }
   else
   {
      addr = networkAddress | uint32(++numIPNodes + param_numSources);
//      EV << "Peer address: " << IPvXAddress(addr).str() << endl;
   }
   int maxNodes = (~netmask) - 1; // 0 and ffff have special meaning and cannot be used
   if ((int)m_nodeInfo.size() > maxNodes)
      error("netmask too large, not enough addresses for all %d nodes");

   // find interface table
   IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(terminal);
   IPAddress address = IPAddress(addr);

   for(unsigned int i = m_nodeInfo.size();i > 0; i--)
   {
      if( m_nodeInfo[i-1].isIPNode
          &&
          ((InterfaceTable*)(m_nodeInfo[i-1].ift))->getParentModule()->getFullName() == terminal->getFullName())
      {
         m_nodeInfo[i-1].address=address;
         break;
      }
   }

   // assign address to all (non-loopback) interfaces
   for (int k = 0; k < (int)ift->getNumInterfaces(); k++)
   {
      InterfaceEntry *ie = ift->getInterface(k);
      if (!ie->isLoopback())
      {
         ie->ipv4Data()->setIPAddress(address);
         // full address must match for local delivery
         ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
      }
   }
   return(address);

}

/**
 * FIXME @Markus description required
 */
void InetUnderlayConfigurator::assignNewAddress(IPAddress oldAddress, int interfaceNbr)
{
   Enter_Method_Silent();
   std::string pppInterface;
   if (interfaceNbr == 0)
      pppInterface = "ppp0";
   else
      pppInterface = "ppp1";
   numIPNodes++;
   jumps.insert(std::make_pair(simTime(), oldAddress));
   // assign IP addresses
   uint32 networkAddress = IPAddress(par("networkAddress").stringValue()).getInt();
   uint32 netmask = IPAddress(par("netmask").stringValue()).getInt();

   int maxNodes = (~netmask) - 1; // 0 and ffff have special meaning and cannot be used
   if (m_topo.getNumNodes() > maxNodes)
      error("netmask too large, not enough addresses for all %d nodes",
            m_topo.getNumNodes());

   for (int i = 0; i < m_topo.getNumNodes(); i++)
   {
      // skip bus types
      if (!m_nodeInfo[i].isIPNode)
         continue;

      if (oldAddress == m_nodeInfo[i].address)
      {
         uint32 addr = networkAddress | uint32(numIPNodes + param_numSources);
         m_nodeInfo[i].address.set(addr);

         // find interface table and assign address to all (non-loopback) interfaces
         IInterfaceTable *ift = m_nodeInfo[i].ift;
         for (int k = 0; k < ift->getNumInterfaces(); k++)
         {
            InterfaceEntry *ie = ift->getInterface(k);
            if ((!ie->isLoopback()) && (ie->getName() == pppInterface))
            {
               ie->ipv4Data()->setIPAddress(IPAddress(addr));

               // full address must match for local delivery
               ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
            }
         }
      }
   }
}


/**
 * Connects a terminal to a randomly selected access router
 *
 * @param terminal		The terminal that has to be connected
 * @param conn
 */
void InetUnderlayConfigurator::connect(cModule* terminal, bool conn)
{
   cModule * router;
   if (!noneTransitNodesAccessRouters)
   {
      // - Determining a random access router
      int rand = intrand(m_accessRouterList.size());
      router = m_accessRouterList[rand];
   }
   else
   {
      // - Determining a random access router by chances through number of gates of non-transit nodes
      double rand2 = dblrand();
      for (unsigned int i = 0; i < m_accessRoutersWeighted.size() ; i++ )
      {
         if (m_accessRoutersWeighted[i].second >= rand2)
         {
            router = m_accessRoutersWeighted[i].first;
            break;
         }
      }
   }

   // - get moduleType for creation of new dynamic modules
   cModuleType *moduleTypePPP = cModuleType::get("inet.linklayer.ppp.PPPInterface");
   // - look for an unconnected ppp-module in the router
   int i = 0;
   while (router->findSubmodule("ppp", i) != -1)
   {
      i++;
   }

   cGate* routerOutGate = NULL;
   cGate* routerInGate = NULL;
   if (router->gateSize("pppg$o") < (i + 1))
   {
      routerOutGate = router->getOrCreateFirstUnconnectedGate("pppg", 'o', false, true);
      routerInGate = router->getOrCreateFirstUnconnectedGate("pppg", 'i', false, true);
   }
   else
   {
      routerOutGate = router->gate("pppg$o", i);
      routerInGate = router->gate("pppg$i", i);
   }

   assert(routerOutGate != NULL && routerInGate != NULL);

   //- create new channels for the new connection
   cDatarateChannel *channelIn = cDatarateChannel::create("channel");
   cDatarateChannel *channelOut = cDatarateChannel::create("channel");
   // -- set the data rate
   channelIn->setDatarate(400000000);
   channelOut->setDatarate(400000000);
   channelIn->setDefaultOwner(terminal);
   channelOut->setDefaultOwner(terminal);
   // -- set the delay
   double delay = (double) intuniform((int)m_minAccessDelay * 10, (int)m_maxAccessDelay * 10) / ((double) 10000);
   channelIn->setDelay(delay);
   channelOut->setDelay(delay);

   // - new ppp-module has to be built
   if (router->getSubmodule("networkLayer")->gateSize("ifIn") < (i + 1))
   {
      router->getSubmodule("networkLayer")->setGateSize("ifIn", i + 1);
      router->getSubmodule("networkLayer")->setGateSize("ifOut", i + 1);
      router->getSubmodule("networkLayer")->getSubmodule("ip")->setGateSize("queueIn", i + 1);
      router->getSubmodule("networkLayer")->getSubmodule("arp")->setGateSize("nicOut", i + 1);
      router->getSubmodule("networkLayer")->gate("ifIn", i)->connectTo(
               router->getSubmodule("networkLayer") ->getSubmodule("ip")
               ->gate("queueIn", i), cIdealChannel::create("channel"), false);
   }
   router->getSubmodule("networkLayer")->getSubmodule("arp")->gate("nicOut", i) ->connectTo(
            router->getSubmodule("networkLayer") ->gate("ifOut", i), cIdealChannel::create("channel"), false);

   // - create new ppp-module and statistic module
   cModule* moduleRouterPPP = moduleTypePPP->create("ppp", router, i, i);
   moduleRouterPPP->changeParentTo(router);
   moduleRouterPPP->finalizeParameters();

   // - connect all gates
   // -- in between terminal and router
   routerOutGate->connectTo(terminal->gate("pppg$i", 0), channelIn, false);
   terminal->gate("pppg$o", 0)->connectTo(routerInGate, channelOut, false);
   // -- initialize the channels
   if(conn)
   {
      channelIn->callInitialize();
      channelOut->callInitialize();
   }

   // -- at router side
   moduleRouterPPP->gate("phys$o")->connectTo(router->gate("pppg$o", i));
   router->gate("pppg$i", i)->connectTo(moduleRouterPPP->gate("phys$i"));
   router->getSubmodule("networkLayer")->gate("ifOut", i)->connectTo(moduleRouterPPP->gate("netwIn"));
   moduleRouterPPP->gate("netwOut")->connectTo(router->getSubmodule("networkLayer")->gate("ifIn", i));

   //no formal used ppp-module was found, finalize the new ppp-module
   moduleRouterPPP->buildInside();
   moduleRouterPPP->setDisplayString("i=block/ifcard");
   moduleRouterPPP->scheduleStart(simTime());
   moduleRouterPPP->callInitialize();

   //set the IPv4Data for the new InterfaceEntry
   InterfaceTable *routerInterfaceTable = check_and_cast<InterfaceTable*> (router->getSubmodule("interfaceTable"));
   std::string tocut = moduleRouterPPP->getFullName();
   tocut = "ppp" + tocut.substr(tocut.find('[', 0) + 1, tocut.length() - 5);
       InterfaceEntry *interfaceEntry = routerInterfaceTable->getInterfaceByName(tocut.c_str());
       interfaceEntry = routerInterfaceTable->getInterfaceByName(tocut.c_str());
       interfaceEntry->setIPv4Data(routerInterfaceTable->getInterface(1)->ipv4Data());
}

/**
 * Creates all sources at once
 */
void InetUnderlayConfigurator::initSources()
{
   Enter_Method("initSources");

//   for (int i = 0; i < param_numSources; i++)
//   {
////      createNode(true);
//      createSourceNode(true);
//   }

   //createSourceNode(true);

   for (int i = 0; i < param_numSources; i++)
   {
      cModule* terminal = createSourceNode(false);

      extractTopology(m_topo,m_nodeInfo);
      IPAddress addr = assignPeerAddress(terminal);
      EV << "Created a source node : " << terminal->getFullName()
         << "with address: " << addr
         << endl;
      fillRoutingTables(terminal, addr);
      for(int i = 2; i <= numInitStages(); i++)
         terminal->callInitialize(i);
   }
   setDisplayString(m_topo, m_nodeInfo);
}


/**
 * Testing method:
 * Creates all terminals
 */
void InetUnderlayConfigurator::initPeers()
{
   Enter_Method("initPeers");
   EV << "Adding " << param_numPeers << " peer nodes ... " << endl;

   for (int i = 0; i < param_numPeers; i++)
   {
      cModule* terminal = createPeerNode(false);

      extractTopology(m_topo,m_nodeInfo);
      IPAddress addr = assignPeerAddress(terminal);
      EV << "Created end node : " << terminal->getFullName()
         << "with address: " << addr
         << endl;
      fillRoutingTables(terminal, addr);
      for(int i = 2; i <= numInitStages(); i++)
         terminal->callInitialize(i);
   }
   setDisplayString(m_topo, m_nodeInfo);
}

// obsolete ----------------->
/**
 * Creates a new terminal
 *
 * @param conn	true if channel should be initialized during establishing connection
 * @return  a pointer to the new terminal
 */
/*
cModule* InetUnderlayConfigurator::createNode(bool conn)
{
   Enter_Method("createNode()");

   // derive overlay node from ned
//   std::string nameStr = "so.mesh.donet.DonetNode";
//   cModuleType* moduleType = cModuleType::get(nameStr.c_str());
//   assert(moduleType);

   std::string displayString;
   cModule* terminal = NULL;

   if (numSources < param_numSources)
   {
      //DEBUGOUT(" Creating source node");

      // -- derive overlay node from ned
      std::string nameStr = "so.mesh.donet.DonetSourceNode";
      cModuleType* moduleType = cModuleType::get(nameStr.c_str());
      assert(moduleType);

//      EV << "moduleType info: " << endl
//         << "  classname: " << moduleType->getClassName() << endl
//         << "  full name: " << moduleType->getFullName() << endl
//         << "  full path: " << moduleType->getFullPath().c_str()
//         << endl;

      terminal = moduleType->create("sourceNode", getParentModule(), param_numSources, numSources);
      displayString = "i=device/server;tt=The Source node - Providing content into the network";
      ////DEBUGOUT(" Creating source ...");
      EV << "Creating source ..." << endl;

      numSources++;
   }
   else
   {
      // -- derive overlay node from ned
      std::string nameStr = "so.mesh.donet.DonetPeerNode";
      cModuleType* moduleType = cModuleType::get(nameStr.c_str());
      assert(moduleType);

      terminal = moduleType->create("peerNode", getParentModule(), param_numPeers, m_numTerminals);
      displayString = "i=device/wifilaptop_l;i2=block/circle_s";
      // //DEBUGOUT(" Creating terminal node");
      EV << "Creating terminal node..." << endl;
      m_numTerminals++;
   }

   assert(terminal);
   EV << "Class name of terminal: " << terminal->getClassName() << endl;
   terminal->setGateSize("pppg", 1);
   terminal->finalizeParameters();
   terminal->setDisplayString(displayString.c_str());
   terminal->buildInside(); // FIXME
   terminal->scheduleStart(simTime());

   nodeList.insert(terminal);
   // Giang
   // -- Connect the terminal to a random access router
      connect(terminal,conn);

//// initialize the terminals until stage 1 since we are already in stage 2

   terminal->callInitialize(0);
   terminal->callInitialize(1);

   return terminal;
}
*/
// <--------------

cModule* InetUnderlayConfigurator::createSourceNode(bool conn)
{
   Enter_Method("createNode()");

   std::string displayString;
   cModule* terminal = NULL;

   if (m_numSources < param_numSources)
   {
      //DEBUGOUT(" Creating source node");

      // -- derive overlay node from ned
      //std::string nameStr = "so.mesh.donet.DonetSourceNode";
      std::string nameStr = "so.mesh.donet.DonetSourceNodeTR";
      cModuleType* moduleType = cModuleType::get(nameStr.c_str());
      assert(moduleType);

//      EV << "moduleType info: " << endl
//         << "  classname: " << moduleType->getClassName() << endl
//         << "  full name: " << moduleType->getFullName() << endl
//         << "  full path: " << moduleType->getFullPath().c_str()
//         << endl;

      terminal = moduleType->create("sourceNode", getParentModule(), param_numSources, m_numSources);
      displayString = "i=device/server;tt=The Source node - Providing content into the network";
      ////DEBUGOUT(" Creating source ...");
      EV << "Creating source ..." << endl;

      m_numSources++;
   }

   assert(terminal);
   EV << "Class name of terminal: " << terminal->getClassName() << endl;
   terminal->setGateSize("pppg", 1);
   terminal->finalizeParameters();
   terminal->setDisplayString(displayString.c_str());
   terminal->buildInside(); // FIXME
   terminal->scheduleStart(simTime());

   m_nodeList.insert(terminal);
   // Giang
   // -- Connect the terminal to a random access router
      connect(terminal,conn);
   // -- initialize the terminals until stage 1 since we are already in stage 2
   terminal->callInitialize(0);
   terminal->callInitialize(1);

   return terminal;
}

cModule* InetUnderlayConfigurator::createPeerNode(bool conn)
{
   Enter_Method("createNode()");

   std::string displayString;
   cModule* terminal = NULL;

   if (m_numTerminals < param_numPeers)
   {
      // -- derive overlay node from ned
      //std::string nameStr = "so.mesh.donet.DonetPeerNode";
      std::string nameStr = "so.mesh.donet.DonetPeerNodeTR";
      cModuleType* moduleType = cModuleType::get(nameStr.c_str());
      assert(moduleType);

      terminal = moduleType->create("peerNode", getParentModule(), param_numPeers, m_numTerminals);
      displayString = "i=device/wifilaptop_l;i2=block/circle_s";
      EV << "Creating terminal node..." << endl;
      m_numTerminals++;
   }

   assert(terminal);
   EV << "Class name of terminal: " << terminal->getClassName() << endl;
   terminal->setGateSize("pppg", 1);
   terminal->finalizeParameters();
   terminal->setDisplayString(displayString.c_str());
   terminal->buildInside(); // FIXME
   terminal->scheduleStart(simTime());

   m_nodeList.insert(terminal);
   // Giang
   // -- Connect the terminal to a random access router
      connect(terminal,conn);
   // -- initialize the terminals until stage 1 since we are already in stage 2
   terminal->callInitialize(0);
   terminal->callInitialize(1);

   return terminal;
}

/**
 * FIXME @Markus description required
 */
void InetUnderlayConfigurator::deleteOutdatedRoutes()
{
   std::map<SimTime, IPAddress>::iterator iter;
   std::map<SimTime, IPAddress> delList;
   iter = jumps.begin();
   while (iter != jumps.end())
   {
      if ((simTime() - (iter->first)) > 500)
      {
         //DEBUGOUT("saved Jump Time " << iter->first << " for Node " << iter->second << " --> delete Route in Routers");
         for (int j = 0; j < (int)m_topo.getNumNodes(); j++)
         {
            if (!m_nodeInfo[j].isIPNode) continue;
            cTopology::Node *atNode = m_topo.getNode(j);
            if (atNode->getNumPaths() == 0) continue; // not connected
            if (m_nodeInfo[j].usesDefaultRoute) continue; // already added default route here
            IRoutingTable *rt = m_nodeInfo[j].rt;
            if (rt->findRoute(iter->second, IPAddress(), IPAddress()) != NULL)
               rt->deleteRoute(rt->findRoute(iter->second, IPAddress(), IPAddress()));
         }
         delList.insert(*iter);
      }
      iter++;
   }

   for(iter = delList.begin(); iter != delList.end(); iter++)
      jumps.erase(iter->first);
   delList.clear();
}


/**
 * FIXME @Markus description required
 */
void InetUnderlayConfigurator::drop1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp)
{
   throw cRuntimeError("not required anymore");

   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      cModule *mod = topo.getNode(i)->getModule();
      if (overlayNode == mod)
      {
         oldIT = nodeInfo[i].ift;
         InterfaceTable *tempIT = new InterfaceTable(); //temp= new InterfaceTable();
         tempIT->addInterface(oldIT->getInterface(0), ppp);
         tempIT->addInterface(oldIT->getInterface(2), ppp);
         nodeInfo[i].ift = tempIT;
      }
   }
}


/**
 * Fills RoutingTables of handovered node
 * (currently called from updateRoutes only)
 *
 * @param topo			The topology
 * @param nodeInfo		A vector containing information of all nodes in topology
 * @param overlayNode	The node that handovered
 */
void InetUnderlayConfigurator::fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode) {

   // fill in routing tables with static routes
   for (int i = 0; i < topo.getNumNodes(); i++)
   {

      cTopology::Node *destNode = topo.getNode(i);

      // calculate shortest paths from everywhere towards destNode
      topo.calculateUnweightedSingleShortestPathsTo(destNode);

      if (destNode->getModule() == overlayNode)
      {
         // skip bus types
         if (!nodeInfo[i].isIPNode)
            continue;

         IPAddress destAddr = nodeInfo[i].address;
         std::string destModName = destNode->getModule()->getFullName();

         // add route (with host=destNode) to every routing table in the network
         // (excepting nodes with only one interface -- there we'll set up a default route)
         for (int j = 0; j < topo.getNumNodes(); j++)
         {
            if (i == j)
               continue;
            if (!nodeInfo[j].isIPNode)
               continue;

            cTopology::Node *atNode = topo.getNode(j);
            if (atNode->getNumPaths() == 0)
               continue; // not connected
            if ((nodeInfo[j].usesDefaultRoute))
               continue; // already added default route here


            IPAddress atAddr = nodeInfo[j].address;

            IInterfaceTable *ift = nodeInfo[j].ift;

            int outputGateId = atNode->getPath(0)->getLocalGate()->getId();
            InterfaceEntry *ie = ift->getInterfaceByNodeOutputGateId(outputGateId);

            if (!ie)
               error("%s has no interface for output gate id %d", ift->getFullPath().c_str(), outputGateId);

            //DEBUGOUT("Adding Route from: "<<atNode->getModule()->getFullName() << "=" << IPAddress(atAddr) << " towards " << destModName << "=" << IPAddress(destAddr) << " interface " << ie->getName());

            //EV << "  from " << atNode->getModule()->getFullName() << "=" << IPAddress(atAddr);
            //EV << " towards " << destModName << "=" << IPAddress(destAddr) << " interface " << ie->getName() << endl;

            // add route
            IRoutingTable *rt = nodeInfo[j].rt;
            IPRoute *e = new IPRoute();
            e->setHost(destAddr);
            e->setNetmask(IPAddress(255,255,255,255)); // full match needed
            e->setInterface(ie);
            e->setType(IPRoute::DIRECT);
            e->setSource(IPRoute::MANUAL);
            //e->getMetric() = 1;
            rt->addRoute(e);
         }
      }
   }
}

/**
 * - Fills in routing tables with static routes
 * - Required for inserting nodes during simulation time
 * - Sets default routes at new nodes and adds routes to
 *	 all routers -> FIXME has to be moved to addDefaultRoutes
 *
 * @param terminal	a new terminal that has to be added
 * @param destAddr	the IPAddress of the new terminal
 */
void InetUnderlayConfigurator::fillRoutingTables(cModule* terminal, IPAddress destAddr)
{
   cTopology::Node *terminalNode = m_topo.getNodeFor(terminal);
   //DEBUGOUT(" adding default route and fill routing table in node "<<terminalNode->getModule()->getFullName());

   /*
    * Add default route at terminal
    */
   std::string destModName = terminalNode->getModule()->getFullName();
   EV << "destModName: " << destModName.c_str() << endl;

   IInterfaceTable* ift_own = IPAddressResolver().interfaceTableOf(terminal);
   assert(ift_own);
   EV << "ift_own has " << ift_own->getNumInterfaces() << " interfaces " << endl;

   IRoutingTable *rt_terminal =  IPAddressResolver().routingTableOf(terminal);
   // count non-loopback interfaces
   int numIntf = 0;
   InterfaceEntry *ie_terminal = NULL;
   for (int k = 0; k < ift_own->getNumInterfaces(); k++)
   {
      if (!ift_own->getInterface(k)->isLoopback())
      {
         ie_terminal = ift_own->getInterface(k);
         numIntf++;
      }
   }

   assert(ie_terminal);

   // add default route
   if(numIntf == 1)
   {
      IPRoute *e_default = new IPRoute();
      e_default->setHost(IPAddress());
      e_default->setNetmask(IPAddress());
      e_default->setInterface(ie_terminal);
      e_default->setType(IPRoute::REMOTE);
      e_default->setSource(IPRoute::MANUAL);
      rt_terminal->addRoute(e_default);
   }

   /*
    * Add route with host=terminal to every routing table in the network
    * (excepting nodes with only one interface -- there we already set up a default route)
    */

   // calculate shortest paths from everywhere towards terminalNode

   m_topo.calculateUnweightedSingleShortestPathsTo(terminalNode);
   for (unsigned int j = 0; j < m_nodeInfo.size(); j++)
   {
      if (!m_nodeInfo[j].isIPNode)
         continue;
      cTopology::Node *atNode = m_topo.getNode(j);
      if (atNode == terminalNode)
      {
         m_nodeInfo[j].usesDefaultRoute = (numIntf == 1);
         continue;
      }
      else if (atNode->getNumPaths() == 0)
      {
         continue; // not connected
      }
      else if (strcmp(atNode->getModule()->getName(),"rt") != 0 && m_nodeInfo[j].usesDefaultRoute)
      {
         continue; // already added default route here
      }

//      IPAddress atAddr = m_nodeInfo[j].address;
      IInterfaceTable *ift = m_nodeInfo[j].ift;
      int outputGateId = atNode->getPath(0)->getLocalGate()->getId();
      InterfaceEntry *ie = ift->getInterfaceByNodeOutputGateId(outputGateId);
      if (!ie)
         error("%s has no interface for output gate id %d", ift->getFullPath().c_str(), outputGateId);

      //EV << "  from " << atNode->getModule()->getFullName() << "=" <<IPAddress(atAddr);
      //EV << " towards " << destModName << "=" << destAddr << " interface " << ie->getName() << endl;

      // add route from dest to new terminal
      IRoutingTable *rt = m_nodeInfo[j].rt;
      IPRoute *e = new IPRoute();
      e->setHost(IPAddress(destAddr));
      e->setNetmask(IPAddress(255,255,255,255)); // full match needed
      e->setInterface(ie);
      e->setType(IPRoute::DIRECT);
      e->setSource(IPRoute::MANUAL);
      rt->addRoute(e);
   }

}


/**
 * Fills routing tables of all nodes in the topology
 * (used in initialization phase of this module)
 *
 * @param topo		The topology
 * @param nodeInfo	A vector containing information of all nodes in topology
 */
void InetUnderlayConfigurator::fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo)
{
   // fill in routing tables with static routes
   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      cTopology::Node *destNode = topo.getNode(i);
      //DEBUGOUT(" fill routing table in node "<<destNode->getModule()->getFullName());

      // skip bus types
      if (!nodeInfo[i].isIPNode)
         continue;

      IPAddress destAddr = nodeInfo[i].address;
      std::string destModName = destNode->getModule()->getFullName();

      // calculate shortest paths from everywhere towards destNode
      topo.calculateUnweightedSingleShortestPathsTo(destNode);

      // add route (with host=destNode) to every routing table in the network
      // (excepting nodes with only one interface -- there we'll set up a default route)
      for (int j = 0; j < topo.getNumNodes(); j++)
      {
         if (i == j)
            continue;
         if (!nodeInfo[j].isIPNode)
            continue;

         cTopology::Node *atNode = topo.getNode(j);
         if (atNode->getNumPaths() == 0)
            continue; // not connected
         if (nodeInfo[j].usesDefaultRoute)
            continue; // already added default route here

         IPAddress atAddr = nodeInfo[j].address;

         IInterfaceTable *ift = nodeInfo[j].ift;

         int outputGateId = atNode->getPath(0)->getLocalGate()->getId();
         InterfaceEntry *ie = ift->getInterfaceByNodeOutputGateId(outputGateId);
         if (!ie)
            error("%s has no interface for output gate id %d", ift->getFullPath().c_str(), outputGateId);

         //EV << "  from " << atNode->getModule()->getFullName() << "=" <<IPAddress(atAddr);
         //EV << " towards " << destModName << "=" << IPAddress(destAddr) << " interface " << ie->getName() << endl;
         //DEBUGOUT("  from " << atNode->getModule()->getFullName() << "=" <<IPAddress(atAddr) << " towards " << destModName << "=" << destAddr << " interface " << ie->getName());

         // add route
         IRoutingTable *rt = nodeInfo[j].rt;
         IPRoute *e = new IPRoute();
         e->setHost(destAddr);
         e->setNetmask(IPAddress(255,255,255,255)); // full match needed
         e->setInterface(ie);
         e->setType(IPRoute::DIRECT);
         e->setSource(IPRoute::MANUAL);
         //e->getMetric() = 1;
         rt->addRoute(e);
      }
   }
}


/**
 * Store all router with less than two connections to
 * other routers in  vector accessRouter
 */
void InetUnderlayConfigurator::getAccessRouter()
{
   bool routerLine = (strcmp(this->getParentModule()->getFullName(), "RouterLine") == 0);
   int size = getParentModule()->getSubmodule("rt",0)->getVectorSize();
   //DEBUGOUT("Checking NED-File for access routers with a total number of "<<size<<" routers");
   double sum = 0.0;
   for (int i = 0; i < size; i++)
   {
      if (getParentModule()->findSubmodule("rt"), i)
      {
         cModule* router = getParentModule()->getSubmodule("rt", i);
         //DEBUGOUT("1.router->gateSize(out): rt["<<i<<"] :"<<router->gateSize("pppg$o"));

         // the ned file should not contain unconnected routers
         assert(router->gateSize("pppg$o") != 0);

         if (router->gateSize("pppg$o") == 1 || routerLine)
            m_accessRouterList.push_back(router);

         if (!(bool)router->par("isTransit"))
         {
            sum += 1/(double)router->gateSize("pppg$o");
            m_accessRoutersWeighted.push_back(std::pair<cModule*,double>(router,sum));
         }

      }
   }

   for (unsigned int i = 0 ; i < m_accessRoutersWeighted.size() ; i++ )
   {
      double interval = m_accessRoutersWeighted[i].second/sum;
      m_accessRoutersWeighted[i] = std::pair<cModule*,double>(m_accessRoutersWeighted[i].first,interval);
   }

   if(m_accessRouterList.size() == 0)
   {
      throw cRuntimeError("accessRouter == 0");
   }

   //DEBUGOUT("number of access routers: "<<accessRouter.size());
}


/**
 * FIXME @Markus description required
 */
void InetUnderlayConfigurator::getSecondAddress(IPAddress oldAddress, int interfaceNbr)
{
   numIPNodes++;
   std::string pppInterface;
   if (interfaceNbr == 0)
      pppInterface = "ppp0";
   else
      pppInterface = "ppp1";

   jumps.insert(std::make_pair(simTime(), oldAddress));
   // assign IP addresses
   uint32 networkAddress =	IPAddress(par("networkAddress").stringValue()).getInt();
   uint32 netmask = IPAddress(par("netmask").stringValue()).getInt();

   int maxNodes = (~netmask) - 1; // 0 and ffff have special meaning and cannot be used
   if (m_topo.getNumNodes() > maxNodes)
      error("netmask too large, not enough addresses for all %d nodes", m_topo.getNumNodes());

   for (int i = 0; i < m_topo.getNumNodes(); i++)
   {
      // skip bus types
      if (!m_nodeInfo[i].isIPNode)
         continue;

      if (oldAddress == m_nodeInfo[i].address)
      {
         uint32 addr = networkAddress | uint32(numIPNodes);
         m_nodeInfo[i].address.set(addr);

         // find interface table and assign address to all (non-loopback) interfaces
         IInterfaceTable *ift = m_nodeInfo[i].ift;
         for (int k = 0; k < ift->getNumInterfaces(); k++)
         {
            InterfaceEntry *ie = ift->getInterface(k);
            if ((!ie->isLoopback()) && (ie->getName() == pppInterface))
            {
               ie->ipv4Data()->setIPAddress(IPAddress(addr));

               // full address must match for local delivery
               ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
            }
         }
      }
   }
}

/**
 * FIXME @Markus description required
 */
void InetUnderlayConfigurator::reAdd1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp)
{
   throw cRuntimeError("not required anymore");

   for (int i = 0; i < topo.getNumNodes(); i++)
   {
      cModule *mod = topo.getNode(i)->getModule();
      if (overlayNode == mod)
      {
         nodeInfo[i].ift = oldIT;
      }
   }
}



/**
 * Called from the outside to update routes for nodes after a handover
 *
 * @param overlayNode	The handovered node
 * @param multihoming	True if multihoming is applied
 * @param oldRouter		A pointer to the former router of overlayNode
 */
bool InetUnderlayConfigurator::updateRoutes(cModule *overlayNode, bool multihoming, cModule *oldrouter)
{
   Enter_Method_Silent();
   //DEBUGOUT("Updating Routes");
   if (!multihoming)
   {
      // extract topology into the cTopology object, then fill in
      // isIPNode, rt and ift members of nodeInfo[]
      extractTopology(m_topo, m_nodeInfo);

      // add default routes to hosts (nodes with a single attachment);
      // also remember result in m_nodeInfo[].usesDefaultRoute
      addDefaultRoutes(m_topo, m_nodeInfo, overlayNode);

      // calculate shortest paths, and add corresponding static routes
      fillRoutingTables(m_topo, m_nodeInfo, overlayNode);

      deleteOutdatedRoutes();

      // update display string
      setDisplayString(m_topo, m_nodeInfo);
   }
   else
   {
      // extract topology into the cTopology object, then fill in
      // isIPNode, rt and ift members of m_nodeInfo[]

      extractTopology(m_topo, m_nodeInfo);

      // assign addresses to IP nodes, and also store result in m_nodeInfo[].address
      //assignAddresses(topo, m_nodeInfo);
      for (int i=0; i<m_topo.getNodeFor(oldrouter)->getNumOutLinks();i++)
      {
         if (m_topo.getNodeFor(oldrouter)->getLinkOut(i)->getRemoteNode() == m_topo.getNodeFor(overlayNode))
         {
            //DEBUGOUT("Disable Link");
            m_topo.getNodeFor(oldrouter)->getLinkOut(i)->disable();
         }
      }

      for (int i=0; i<m_topo.getNodeFor(overlayNode)->getNumOutLinks();i++)
      {
         if (m_topo.getNodeFor(overlayNode)->getLinkOut(i)->getRemoteNode() == m_topo.getNodeFor(oldrouter))
         {
            //DEBUGOUT("Disable Link");
            m_topo.getNodeFor(overlayNode)->getLinkOut(i)->disable();
         }
      }

      // add default routes to hosts (nodes with a single attachment);
      // also remember result in m_nodeInfo[].usesDefaultRoute
      addDefaultRoutes(m_topo, m_nodeInfo, overlayNode);

      // calculate shortest paths, and add corresponding static routes
      fillRoutingTables(m_topo, m_nodeInfo, overlayNode);

      deleteOutdatedRoutes();
      // update display string
      setDisplayString(m_topo, m_nodeInfo);
   }
   return true;
}


/**
 * Initializes the Kfournisseur-Module for solving k-center.
 * This does not depend on the actual alm router positions but on the (potential alm-) router positions.
 */
/*
void InetUnderlayConfigurator::kfournisseur() {

       ARSPositionCollector* arsPC = ARSPositionCollectorAccess().get();
       ARS_RouterManager* routerManager = ARS_RouterManagerAccess().get();
       assert(arsPC);
       assert(routerManager);

       arsPC->configure(accessRouter, nodeList, &topo, routerManager->getNumALMRouter());
}
*/

/*
void InetUnderlayConfigurator::checkRessources(){
   cModule* network = this;
   while ((network != NULL) && !network->getModuleType()->isNetwork())
      network = network->getParentModule();

   //	GlobalStreamTable* gT = GlobalStreamTableAccess().get();
   cTopology networkTopology;
   networkTopology.extractByProperty( "node" );
   //	int sumRessources = 0;
   for(int srv = 0; srv < networkTopology.getNumNodes(); srv++){
      cTopology::Node* node = networkTopology.getNode( srv );
      if( !( node->getModule()->isName( "sourceNode" ) ) ) continue;
      //		cModule* nodeModule = node->getModule();
      //      UserInterface* ui = check_and_cast<UserInterface*>( nodeModule->getModuleByRelativePath("udpApp[0].application") );
      //		sumRessources += ui->par("numParallelStreams").longValue();
   }
   //	if( sumRessources < gT->par( "numStreams" ).longValue() ) throw cRuntimeError( "There are less ressources than streams" );
}
*/
