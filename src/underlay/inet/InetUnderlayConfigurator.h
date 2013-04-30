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
// InetUnderlayConfigurator.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

/*
 * InetUnderlayConfigurator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: mtrapp,mathias
 */

#ifndef INETUNDERLAYCONFIGURATOR_H_
#define INETUNDERLAYCONFIGURATOR_H_

#include "INETDefs.h"
#include "IPAddress.h"
#include <map>
#include "../../../inet/src/networklayer/autorouting/FlatNetworkConfigurator.h"

class IInterfaceTable;
class IRoutingTable;


class InetUnderlayConfigurator: public FlatNetworkConfigurator
{
public: // OMNeT++ stuff
   virtual int numInitStages() const { return 3; }
   void initialize(int stage);
   void handleMessage(cMessage* msg);
   virtual void finish();

public:
   void addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo, cModule *jumpedNode);
   void addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo);
   void addDefaultRoutes();
   void assignNewAddress(IPAddress oldAddress, int interfaceNbr);
   void connect(cModule* terminal,bool conn);
   void deleteOutdatedRoutes();
   void drop1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp);
   void fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo, cModule *jumpedNode);
   void fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo);
   void fillRoutingTables(cModule* terminal, IPAddress destAddr);
   void getSecondAddress(IPAddress oldAddress, int interfaceNbr);
   void reAdd1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp);
   bool updateRoutes(cModule *jumpedNode, bool multihoming, cModule *modulePPP);

protected:
   IPAddress assignPeerAddress(cModule* terminal);
   void assignAddresses(cTopology& topo, NodeInfoVector& nodeInfo);
   void initSources();
   void initPeers();
   cModule* createNode(bool conn);
   cModule* createSourceNode(bool conn);
   cModule* createPeerNode(bool conn);
   void getAccessRouter();
   // TODO
   // removeOverlayNode(cModule* terminal);

   // not used!
   //   void kfournisseur();
   //   void checkRessources();

private:
   std::vector<cModule*> m_accessRouterList; // WATCHOUT!!! vector of pointers
   std::vector<std::pair<cModule*,double> > m_accessRoutersWeighted;

   // Access delay bounds for clients
   double m_minAccessDelay;
   double m_maxAccessDelay;

protected:
   cTopology m_topo;
   NodeInfoVector m_nodeInfo; // will be of size topo.nodes[]
   std::map<SimTime, IPAddress> jumps;
   std::set<cModule*> m_nodeList;
   int numIPNodes;

   int param_numSources;
   int param_numPeers;
   int param_numRouters;

   int m_numTerminals;
   int m_numSources;
   IInterfaceTable *oldIT;
   bool noneTransitNodesAccessRouters;
};

#endif /* INETUNDERLAYCONFIGURATOR_H_ */
