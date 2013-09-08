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
// OverlayTopology.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "OverlayTopology.h"
#include <boost/lexical_cast.hpp>
#include "AppSetting.h"
#include "AppSettingDonet.h"

#include "PartnerList.h"
#include "InterfaceTable.h"
#include "IPv4InterfaceData.h"
#include <string>

using namespace std;
using boost::lexical_cast;

Define_Module(OverlayTopology);

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << getFullName() << ": "
#endif

void OverlayTopology::initialize()
{

   m_debug = (hasPar("debug")) ? par("debug").boolValue() : false;

   // -- Set pointers to global modules
   cModule *temp = simulation.getModuleByPath("appSetting");
   m_appSetting = check_and_cast<AppSettingDonet *>(temp);
   EV << "Binding to AppSetting was completed!" << endl;

   param_observeTime    = par("observeTime").longValue();
   //   param_observeTimeEnd    = par("observeTimeEnd").longValue();
   //   param_observeTimeStep     = par("observeTimeStep").longValue();

   //assert(param_observeTimeEnd >= param_observeTime);

   m_observedChunk  = param_observeTime * m_appSetting->getVideoStreamChunkRate();
   /*int count = param_observeTime;
    while(count <= param_observeTimeEnd)
    {
       m_observedChunk  = count * m_appSetting->getVideoStreamChunkRate();
       m_observeChunkList.push_back(m_observedChunk);

       count += param_observeTimeStep;
    }*/

   //    for (std::vector<long>::iterator iter =  m_observeChunkList.begin();
   //         iter != m_observeChunkList.end(); ++iter)
   //    {
   //       debugOUT << *iter << " " << endl;
   //    }

   m_outFile.open(par("outputFilename").stringValue(), fstream::out);
   //m_outFile << "test" << endl;

   // -- file for the visualization of the topology
   //
   m_overlayTopologyFile.open(par("overlayTopologyFile").stringValue(), fstream::out);
   m_numPeers = par("numPeers").longValue();

   streamStripes = par("streamStripes");

   WATCH(m_observedChunk);
}

void OverlayTopology::finish() {
   topo.clear();

   //   std::list<PPEdge> edgeList = getEdges(m_observedChunk % INT_MAX);
   //   for(std::list<PPEdge>::iterator iter = edgeList.begin(); iter != edgeList.end(); ++iter)
   //   {
   //      m_outFile << iter->begin() << ";" << iter->end() << endl;
   //   }
   m_outFile.close();

   storeOverlayTopology();
}

void OverlayTopology::handleMessage(cMessage* msg) {
   throw cRuntimeError("OverlayTopology::handleMessage");
}


/**
 * Set this node as root in the respective TopologyModels
 *
 * @param root  The IP of this root
 * @param sequence The respective sequence number
 */
void OverlayTopology::setRoot(const IPvXAddress & root, const int sequence) {
   Enter_Method_Silent();

   assert(!root.isUnspecified());
   //debugOUT << "current seq = " << sequence << " - while barrier is " << m_observedChunk << endl;
   if(sequence >= m_observedChunk && topo.find(sequence) == topo.end())
   {
      //debugOUT <<" root " << root << " for sequence " << sequence << " topo.size() " << topo.size() << endl;
      topo[sequence].setRoot(root,sequence);

      //debugOUT << "topo.size(after) " << topo.size() << endl;

      if (sequence == m_observedChunk)
      {
         getSignalingOverlayEdges();
      }
   }
}


//void OverlayTopology::removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
//   Enter_Method_Silent();

//   assert(!from.isUnspecified() && !to.isUnspecified());
//   if(sequence < m_observedChunk) return;

//   assert(topo.find(sequence) != topo.end());
//   topo[sequence].removeEdge(sequence,from,to);
//}


int OverlayTopology::getNumTopologies() {

   return topo.size();
}


void OverlayTopology::addNode(const IPvXAddress& ip, const int sequence) {
   Enter_Method_Silent();

   assert(!ip.isUnspecified());
   if(sequence < m_observedChunk) return;

   //debugOUT<< "add Vertex " << ip << " for sequence " << sequence << endl;
   assert(topo.find(sequence) != topo.end());
   topo[sequence].addVertex(ip, sequence);
}

//void OverlayTopology::removeNode(const IPvXAddress& ip, const int sequence) {
//   Enter_Method_Silent();

//   assert(!ip.isUnspecified());
//   if(sequence < m_observedChunk) return;

//   assert(topo.find(sequence) != topo.end());
//   topo[sequence].removeVertex(ip);
//}

/**
  * \param add the edge
 */
void OverlayTopology::addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
   Enter_Method_Silent();

   if(sequence < m_observedChunk) return;

   assert(!from.isUnspecified() && !to.isUnspecified());
   assert(topo.find(sequence) != topo.end());
   topo[sequence].addEdge(sequence, from, to);
   //debugOUT<< "add Edge from " << from << " to " << to << " for sequence " << sequence << endl;
}


int OverlayTopology::getNumVerticesSequence(const int sequence) {
   Enter_Method_Silent();

   if (topo.count(sequence) == 1) {
      return topo[sequence].numberVertexes();
   } else {
      return 0;
   }
}


void OverlayTopology::setJoinTime(const int sequence, IPvXAddress node, double joinTime) {
   Enter_Method_Silent();

   assert(topo.find(sequence) != topo.end());
   topo[sequence].setAge(node, joinTime);
}


PPEdgeList OverlayTopology::getEdges(const int sequence) {
   Enter_Method_Silent();

   debugOUT << "topo has: " << topo.size() << " topologyModel" << endl;
   assert(topo.find(sequence) != topo.end());
   return topo[sequence].getEdges(sequence);
}


TopologyModel OverlayTopology::getMostRecentTopology()
{
   debugOUT << "getMostRecentTopology::" << endl;
   int topoMax = getMaxRecentSeq();
   debugOUT << "topoMax = " << topoMax << endl;
   debugOUT << "streamStripes = " << streamStripes << endl;

   if(streamStripes == 1)
   {
      debugOUT << "There is only one stripe" << endl;
      return getTopology(topoMax);
   }

   debugOUT << "There are more than one stripe" << endl;
   TopologyModel globalTopology = TopologyModel();
   for(int i = 0; i < streamStripes; i++) {
      int topoNum = topoMax - i;
      assert(topoNum >= 0);
      globalTopology.insertTopology(getTopology(topoNum));
   }

   return globalTopology;
}


TopologyModel OverlayTopology::getTopology()
{
   return getMostRecentTopology();
}

/*
 * Get a number of consecutive topologies
 */
TopologyModel OverlayTopology::getTopologySet()
{
   debugOUT << "getTopologySet::" << endl;

   int seqTopoMax = getMaxRecentSeq();
   TopologyModel topoMax = getTopology(seqTopoMax);

   debugOUT << "seqTopoMax = " << seqTopoMax << endl;
   debugOUT << "streamStripes = " << streamStripes << endl;
   debugOUT << "Number of vertex of topoMax " << topoMax.numberVertexes() << endl;

   assert((unsigned int)streamStripes < topo.size());

   TopologyModel outputTopology = TopologyModel();
   for (int i = 0; i < streamStripes; ++i)
   {
      int stripeNum = i + (int)m_observedChunk;
      debugOUT << "\t Considering snapshot " << stripeNum << endl;
      TopologyModel tempTopo = getTopology(stripeNum);
      debugOUT << "\t Number of vertexes of the tempTopo: " << tempTopo.numberVertexes() << endl;
      outputTopology.insertTopology2(tempTopo);
   }

   return outputTopology;
}

TopologyModel & OverlayTopology::getTopologyRef() {
   int topoNum = getMaxRecentSeq();
   return getTopologyRef(topoNum);
}

TopologyModel& OverlayTopology::getMostRecentTopologyRef(){
   int topoNum = getMaxRecentSeq();
   return getTopologyRef(topoNum);
}

TopologyModel OverlayTopology::getTopology(const int sequence)
{
   Enter_Method_Silent();
   debugOUT << "getTopology::" << endl;

   debugOUT << "sequence: " << sequence << endl;
   if (topo.find(sequence) == topo.end())
   {
      debugOUT << "no topology found!" << endl;
      debugOUT<< " topology " << sequence << " not found " <<endl;
   }
   else
   {
      debugOUT << "topology is present!" << endl;
      debugOUT << "number of collected topologies: " << topo.size() << endl;
   }
   assert(topo.find(sequence) != topo.end());
   return topo[sequence];
}

TopologyModel& OverlayTopology::getTopologyRef(const int sequence)
{
   Enter_Method_Silent();

   assert(topo.find(sequence) != topo.end());
   return topo[sequence];
}


int OverlayTopology::attackRecursive(const int numAttacks)
{
   Enter_Method("attackRecursive");

   debugOUT << "attackRecursive():: " << numAttacks << " times" << endl;

   return attackRecursive2(numAttacks);
}


int OverlayTopology::getMostRecentSeq() {

   int seq = -1;
   for(Iterator it = topo.begin(); it != topo.end(); it++) {
      if(seq < it->first) seq = it->first;
   }
   assert(seq >= 0);
   return seq;
}

// returns sequence number of topology
// with maximum number of nodes
int OverlayTopology::getMaxRecentSeq()
{
   debugOUT << "getMaxRecentSeq()::" << endl;
   int seq = -1;
   int max = -1;
   debugOUT << "\t the number of collected topologies: " << topo.size() << endl;
   for(Iterator it = topo.begin(); it != topo.end(); it++)
   {
      int num = it->second.numberVertexes();
      if(max < num || (max == num && seq < it->first))
      {
         seq = it->first;
         max = num;
      }
   }
   assert(seq >= 0);
   debugOUT << "\t max Sequence = " << seq << endl;
   return seq;
}


int OverlayTopology::attackRecursive(const int sequence, const int num)
{
   Enter_Method("attackRecursive(sequence,num");
   debugOUT << "attackRecursive2(sequence, num)::" << endl;

   // -- Get the set of topologies
   //
   TopologyModel topoM = getTopologySet();

   // -- Calculate the centralities for all the nodes
   //
   topoM.calculate();

   // -- Output the topologies into .dot files for visualization
   //
   topoM.writeTopologyToDotFile("//home//giang//Downloads//topology");

   // -- Debugging
   //
//   debugOUT << "number of stripes after getTopologySet(): " << topoM.getNumSeenStripes() << endl;
//   debugOUT << "\t topoM is empty: " << topoM.empty() << endl;
//   debugOUT << "List of stripes:" << endl;
//   PPStringSet stripeSet = topoM.getStripes();
//   for (PPStringSet::iterator iter = stripeSet.begin(); iter != stripeSet.end(); ++iter)
//   {
//      debugOUT << "Edges for stripe " << *iter << endl;
//      PPEdgeList eList = topoM.getEdges(*iter);
//      for (PPEdgeList::iterator iter = eList.begin(); iter != eList.end(); ++iter)
//      {
//         debugOUT << iter->begin() << " - " << iter->end() << endl;
//      }
//   }
//   topoM.printCentralityList();
//   topoM.printIncomingEdgeList();

   int damage = 0;
   for(int i = 0; i < num; i++)
   {
      damage += topoM.removeCentralVertex();
   }

   damage += topoM.getTotalNodesServiceLost();
   debugOUT << "Total nodes with service loss: " << topoM.getTotalNodesServiceLost() << endl;
   debugOUT << "\t damage of attack recursive = " << damage << endl;
   return damage;
}

int OverlayTopology::attackRecursive2(const int numAttacks)
{
   Enter_Method("attackRecursive(numAttacks");
   debugOUT << "attackRecursive2(numAttacks)::" << endl;

   // -- Get the set of topologies
   //
   TopologyModel topoM = getTopologySet();

   // -- Calculate the centralities for all the nodes
   //
   topoM.calculate();

   // -- Output the topologies into .dot files for visualization
   //
   topoM.writeTopologyToDotFile("//home//giang//Downloads//topology");

   // -- Debugging
   //
//   debugOUT << "number of stripes after getTopologySet(): " << topoM.getNumSeenStripes() << endl;
//   debugOUT << "\t topoM is empty: " << topoM.empty() << endl;
//   debugOUT << "List of stripes:" << endl;
//   PPStringSet stripeSet = topoM.getStripes();
//   for (PPStringSet::iterator iter = stripeSet.begin(); iter != stripeSet.end(); ++iter)
//   {
//      debugOUT << "Edges for stripe " << *iter << endl;
//      PPEdgeList eList = topoM.getEdges(*iter);
//      for (PPEdgeList::iterator iter = eList.begin(); iter != eList.end(); ++iter)
//      {
//         debugOUT << iter->begin() << " - " << iter->end() << endl;
//      }
//   }
//   topoM.printCentralityList();
//   topoM.printIncomingEdgeList();

   for(int i = 0; i < numAttacks; i++)
   {
      topoM.removeCentralVertex2();
   }

   int damage = topoM.getTotalNodesServiceLost();
   debugOUT << "Total nodes with service loss: " << topoM.getTotalNodesServiceLost() << endl;
   debugOUT << "\t damage of attack recursive = " << damage << endl;
   return damage;
}


int OverlayTopology::attackRecursiveTopoSet(const int num)
{
   Enter_Method("attackRecursive(sequence,num");
   debugOUT << "attackRecursive on set of topology::" << endl;

   TopologyModel topoSet = getTopologySet();
   PPStringSet stripeSet = topoSet.getStripes();
   int numStripes = stripeSet.size();
   debugOUT << "\t topoM is " << (topoSet.empty() ? "" : " NOT ") << "empty" << endl;
   debugOUT << "\t number of removed nodes = " << num << endl;
   debugOUT << "\t number of topologies: " << numStripes << endl;

   for (PPStringSet::iterator iter = stripeSet.begin(); iter != stripeSet.end(); ++iter)
   {
      debugOUT << "stripe " << *iter << endl;
      //TopologyModel tempTopo = topoM[*iter];
   }

   int damage = 0;
   for(int i = 0; i < num; i++)
   {
      damage += topoSet.removeCentralVertex();
   }

   damage += topoSet.getTotalNodesServiceLost();
   debugOUT << "Total nodes with service loss: " << topoSet.getTotalNodesServiceLost() << endl;
   debugOUT << "\t damage of attack recursive for TopoSet = " << damage << endl;
   return damage;
}

std::vector<long> OverlayTopology::getObserveChunkList(void)
{
   return m_observeChunkList;
}

void OverlayTopology::collectEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to)
{
   //   if (topo.hasNode())
   addEdge(sequence, from, to);
}

void OverlayTopology::writeEdgesToFile(void)
{
   debugOUT << "::writeEdgesToFile()" << endl;

   TopologyModel topo = getTopology();
   PPStringSet stripeSet = topo.getStripes();

   assert(stripeSet.size() > 0);
   PPEdgeList eList = topo.getEdges(*stripeSet.begin());

   //   for (PPStringSet::iterator iter = stripeSet.begin(); iter != stripeSet.end(); ++iter)
   //   {
   //      PPEdgeList eList = topo.getEdges(*iter);
   //   }

   m_outFile << "Source;Target" << endl;
   debugOUT << "this topo has " << eList.size() << " edges" << endl;
   for(std::list<PPEdge>::iterator iter = eList.begin(); iter != eList.end(); ++iter)
   {
      m_outFile << iter->begin() << ";" << iter->end() << endl;
   }
}

void OverlayTopology::writeEdgesToFile(const char* filename)
{
   debugOUT << "::writeEdgesToFile()" << endl;

   std::ofstream outFile(filename, fstream::out);

   int seq = getMostRecentSeq();
   debugOUT << "current number of topo: " << topo.size() << endl;
   debugOUT << "most recent sequence: " << seq << endl;

   TopologyModel topo = getTopology(seq-50);
   PPStringSet stripeSet = topo.getStripes();

   assert(stripeSet.size() > 0);
   PPEdgeList eList = topo.getEdges(*stripeSet.begin());

   outFile << "Source;Target" << endl;
   debugOUT << "this topo has " << eList.size() << " edges" << endl;
   for(std::list<PPEdge>::iterator iter = eList.begin(); iter != eList.end(); ++iter)
   {
      outFile << iter->begin() << ";" << iter->end() << endl;
   }

   outFile.close();
}

void OverlayTopology::getSignalingOverlayEdges(void)
{
   Enter_Method("getSignalingOverlayEdges");
   debugOUT << "getSignalingOverlayEdges" << endl;
   debugOUT << "hello :)" << endl;
   // -- Browse through all the nodes to extract the partner list
   //
   // Source node
   cModule* sourceNodeModule = getParentModule()->getSubmodule("sourceNode",0);
   assert(sourceNodeModule);

   PartnerList* partnerList = check_and_cast<PartnerList*>(sourceNodeModule-> \
                                                           getModuleByRelativePath("peerApp")-> \
                                                           getModuleByRelativePath("partnerList"));

   IPvXAddress sourceAddress = getIpAddress(sourceNodeModule);

   std::vector<IPvXAddress> addressList = partnerList->getPartnerAddresses();

   debugOUT << "source address: " << sourceAddress << endl;
   debugOUT << "edges relating to the source node:" << endl;
   for (std::vector<IPvXAddress>::iterator iter = addressList.begin();
        iter != addressList.end(); ++iter)
   {
      debugOUT << "edge " << sourceAddress << " -- " << *iter << endl;
      //m_edges.insert(std::pair<IPvXAddress, IPvXAddress>(sourceAddress, *iter));
      m_edges.push_back(std::pair<IPvXAddress, IPvXAddress>(sourceAddress, *iter));
   }

   // Peer nodes
   for (int i = 0; i < m_numPeers; ++i)
   {
      cModule* peerNodeModule = getParentModule()->getSubmodule("peerNode",i);

      partnerList = check_and_cast<PartnerList*>(peerNodeModule-> \
                                                 getModuleByRelativePath("peerApp")-> \
                                                 getModuleByRelativePath("partnerList"));

      IPvXAddress peerAddress = getIpAddress(peerNodeModule);
      debugOUT << "Connections of peer " << peerAddress << endl;
      addressList = partnerList->getPartnerAddresses();
      for (std::vector<IPvXAddress>::iterator iter = addressList.begin(); iter != addressList.end(); ++iter)
      {
         bool pair_existed = false;
         for(std::vector<Edge_Type>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
         {
            if (peerAddress == it->second && *iter == it->first)
            {
               pair_existed = true;
               break;
            }
         }
         if (pair_existed)
         {
            debugOUT << "edge " << *iter << " -- " << peerAddress << " existed" << endl;
         }
         else
         {
            debugOUT << "edge " << peerAddress << " -- " << *iter << endl;
            m_edges.push_back(std::pair<IPvXAddress, IPvXAddress>(peerAddress, *iter));
         }
      } // for each address in the partnerList
   }

}

void OverlayTopology::storeOverlayTopology(void)
{
   debugOUT << "storeOverlayTopology" << endl;

   // -- Write to file
   //
   m_overlayTopologyFile << " graph graphname {" << endl;
   for (std::vector<Edge_Type>::iterator iter = m_edges.begin();
        iter != m_edges.end(); ++iter)
   {
      m_overlayTopologyFile << iter->first.str().erase(0, 10) << " -- "
                            << iter->second.str().erase(0, 10) << ";" << endl;
   }
   m_overlayTopologyFile << "}" << endl;

   m_overlayTopologyFile.close();
}

IPvXAddress OverlayTopology::getIpAddress(cModule* module)
{
   InterfaceTable* interface = check_and_cast<InterfaceTable*>(module->getSubmodule("interfaceTable"));
   //debugOUT << "Number of interfaces: " << interface->getNumInterfaces() << endl;

   IPvXAddress address;
   if (interface->getNumInterfaces() > 1)
   {
      address = IPvXAddress(interface->getInterface(interface->getNumInterfaces()-1)->ipv4Data()->getIPAddress());
   }
   else
   {
      address = IPvXAddress(interface->getInterface(0)->ipv4Data()->getIPAddress());
   }

   return address;
}
