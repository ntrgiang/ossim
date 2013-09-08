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
// OverlayTopology.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#ifndef OVERLAYTOPOLOGY_H
#define OVERLAYTOPOLOGY_H

#include "TopologyModel.h"
#include <fstream>

class AppSetting;
class AppSettingDonet;


class OverlayTopology : public cSimpleModule
{
   typedef std::map<int, TopologyModel>::iterator Iterator;
   typedef std::map<int, TopologyModel>           TopologyMap;
   typedef std::list<PPEdge>                       PPEdgeList;

protected:
   bool m_debug;
   TopologyMap        topo;            ///< the topology data structure < streamID, Topology >
   int streamStripes;

   AppSettingDonet* m_appSetting;
   long m_observedChunk;
   int param_observeTime;
   int param_observeTimeEnd;
   int param_observeTimeStep;
   std::vector<long> m_observeChunkList;

public:
   void initialize();
   //virtual int numInitStages() const { return 5; }
   void finish();
   void handleMessage(cMessage* msg);

   void setRoot(const IPvXAddress& root, const int sequence);
   void addNode(const IPvXAddress& ip, const int sequence);  ///< for hierarchy per sequence
//   void removeNode(const IPvXAddress& ip, const int sequence);

   void addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to); ///< for hierarchy per sequence
//   void removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to);

   int           getNumVerticesSequence(const int sequence);
   int           getNumTopologies();

   // topology per packet
   TopologyModel      getTopology();
   TopologyModel      getTopology(const int sequence);              ///< for hierarchy per sequence
   TopologyModel&     getTopologyRef();
   TopologyModel&     getTopologyRef(const int sequence);              ///< for hierarchy per sequence
   TopologyModel      getMostRecentTopology();
   TopologyModel&     getMostRecentTopologyRef();
   PPEdgeList         getEdges(const int sequence);

   void  setJoinTime(const int sequence, IPvXAddress node, double joinTime);
   void  setJoinTime(IPvXAddress node, double joinTime);

   int getMostRecentSeq();
   int getMaxRecentSeq();

   int attackRecursive(const int num);
   int attackRecursive2(const int num);
   int attackRecursive(const int sequence, const int num);

   // -- Giang
   TopologyModel getTopologySet();
   std::vector<long> getObserveChunkList(void);
   inline long getObservedChunk() { return m_observedChunk; }
   void collectEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to);
   //TopologyModel getTopology(int sequence);
   void writeEdgesToFile(void);
   void writeEdgesToFile(const char* filename);

   void storeOverlayTopology(void);
   void getSignalingOverlayEdges(void);
   IPvXAddress getIpAddress(cModule* module);

protected:
   // -- result recording
   std::ofstream m_outFile;

   // -- overlay visualization
   //
   std::ofstream m_overlayTopologyFile;
   int m_numPeers;
   //std::map<IPvXAddress, IPvXAddress> m_edges;
   typedef std::pair<IPvXAddress, IPvXAddress> Edge_Type;
   std::vector<Edge_Type> m_edges;

};

#endif // OVERLAYTOPOLOGY_H
