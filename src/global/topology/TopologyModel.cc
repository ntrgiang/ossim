#include "TopologyModel.h"
#include <iomanip>
#include <queue>
#include <sstream>
#include <string>
#include <fstream>

#if _DEBUG
# define DEBUGOUT(x) if (C_DEBUG_TopologyModel) { std::cout << "(TM  @" << "0x" << std::setbase(16) << ((long)this) << "): " << std::setbase(10) << x << endl;; };
#else
# define DEBUGOUT(x)
#endif

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "TopologyModel: "
#endif


using namespace std;

TopologyModel::TopologyModel() {
   calculated = true;
   numStripes = 1;
   numNodes = 0;
   this->minRequiredStripes = 1;
   //stripes.insert("0-0");
   m_debug = false;
}


TopologyModel::~TopologyModel() {
}


void TopologyModel::setRoot(const IPvXAddress& root) {

   if(roots.find(root) == roots.end()) {
      DEBUGOUT("Set Root: " << root);
      roots.insert(root);
      foreach(std::string stripe, stripes) {
         graph[stripe][root]; // add empty entry in graph
      }
   }
}


void TopologyModel::setRoot(const IPvXAddress& root, const int sequence) {

   //std::cout <<" addRoot " << root << " for sequence " << sequence <<endl;
   if(roots.find(root) == roots.end()) {
      DEBUGOUT("Set Root: " << root);
      roots.insert(root);
   }

   std::string stripe = getString(sequence);

   if(graph[stripe].find(root) == graph[stripe].end()) {

      graph[stripe][root]; // add empty entry in graph
      stripes.insert(stripe);
      //debugOUT << "root " << root << " for new stripe " << sequence << endl;
   }
}



IPvXAddress TopologyModel::getRoot() const {

   if(roots.size() > 0)
      return IPvXAddress(IPAddress::UNSPECIFIED_ADDRESS);

   return *(roots.begin());
}


/**
 * this method returns the number of all direct children
 * of the given node
 */
int TopologyModel::countChildren(const IPvXAddress& vertex) const {

   PPIPvXAddressSet children; // is a set (not multiset)

   // For each stripe
   foreach(std::string stripe, stripes) {
      IPSetSmartPtr list = getGraphAdjacents(stripe, vertex);
      // Boost it up! :-)
      foreach	(const IPvXAddress& addr, *list)
         children.insert(addr);
   }

   return children.size();
}



/**
 * this method returns the number of all direct children
 * of the given node in given stripe
 */
int TopologyModel::countChildren(const IPvXAddress& vertex, const std::string& stripe) const {
   return getGraphAdjacents(stripe, vertex)->size();
}


/**
 * Counts the number of children of the node vertex than can be found within the number
 * of levels given by maxLevel.
 * If maxLevel is null, null is returned.
 */
int TopologyModel::countChildrenMaxLevel(const IPvXAddress& vertex, const int maxLevel) const {

   if (maxLevel < 1)
      return 0;

   PPIPvXAddressSet children; // all seen children
   PPIPvXAddressSet stack; // to be processed parents
   stack.insert(vertex);

   for (int i = 0; i < maxLevel; i++) {
      PPIPvXAddressSet newStack; // children to be processed in the next round

      while (!stack.empty()) {
         const IPvXAddress &currentVertex = *stack.begin();
         stack.erase(currentVertex);

         // For each stripe
         foreach(std::string stripe, stripes) {
            IPSetSmartPtr list = getGraphAdjacents(stripe, currentVertex);
            foreach(const IPvXAddress& addr, *list) {
               newStack.insert(addr);
               children.insert(addr);
            }
         }
      }

      stack = newStack;
   }

   return children.size();
}


int TopologyModel::countSuccessors(const IPvXAddress& vertex) {
   return calculate(vertex);
}


int TopologyModel::countSuccessors(const IPvXAddress& vertex, std::string stripe) {
   // CHECK IT
   if (vertex.isUnspecified())
      throw cException("vertex unspecified");

   return calculate(vertex, stripe);
}


int TopologyModel::countSuccessors() {
   std::string stripe = "0-0";
   return countSuccessors(stripe);
}


int TopologyModel::countSuccessors(const std::string stripe) {
   int succ = 0;
   foreach(IPvXAddress root, roots)
      succ += countSuccessors(root, stripe);

   return succ;
}


int TopologyModel::countNotConnected() {
   int counter = 0;
   // Iterate across all nodes
   for(Centrality::iterator it = centrality.begin(); it != centrality.end(); it++ ) {
      bool connected = false;
      foreach(std::string stripe, stripes) {
         for(Vertexes::iterator it1 = graph[stripe].begin(); it1 != graph[stripe].end();it1++) {
            if(it1->second.find(it->first) != it1->second.end()) connected |= true;
         }
      }
      if(!connected) counter++;
   }
   return counter;
}



/**
 * add a new vertex into topology
 */
void TopologyModel::addVertex(const IPvXAddress vertex, const int sequence) {

   std::string stripe = getString(sequence);
   addVertex(stripe, vertex);
}


void TopologyModel::addVertex(const std::string& stripe, const IPvXAddress vertex) {

   DEBUGOUT("addVertex " << vertex);
   //std::cout << "addVertex " << vertex << " in stripe " << stripe << endl;
   assert(stripes.find(stripe) != stripes.end());

   if(graph[stripe].find(vertex) == graph[stripe].end()){
      if(roots.count(vertex) == 0 ) {
         centrality[vertex] = 0;
         inEdgesCtr[vertex] = 0;
         age[vertex] = (double) simTime().dbl();
         numNodes++;
      }
      graph[stripe][vertex]; // add empty entry in graph
   }
}


/**
 * This method removes the node vertex and all
 * incident and adjacent edges.
 * Further, it traces the outgoing edges (stripes) and removes
 * all related edges while counting the number of affected
 * nodes.
 * This is used to ensure that two attackers in a row
 * are not treated equal to two attackers at the same level in different stripes.
 */
int TopologyModel::removeVertexRecursive(const IPvXAddress& vertex)
{
   debugOUT << "removeVertexRecursive(vertex)::" << endl;

   bool flag_loss = (numNodes == 1);
   debugOUT << "in removeVertexRecursive" << ", loss = " << (flag_loss ? "true" : "false") << endl;

   int affected = 0;

   centrality.erase(vertex);
   //inEdgesCtr.erase(vertex);
   numNodes--;

   debugOUT << "total number of stripes: " << stripes.size() << endl;
   // -- For each stripe, remove all edges in and out of this node
   //
   foreach(std::string stripe, stripes)
   {
      int impact = removeVertexRecursive(stripe, vertex, flag_loss);
      affected += impact;
      debugOUT << "Damage for stripe " << stripe << ": " << impact << endl;
   }

   debugOUT << "affected = " << affected << endl;
   calculated = false;

   return affected;
}

// -- a modified version for Giang
//
void TopologyModel::removeVertexRecursive2(const IPvXAddress& vertex)
{
   debugOUT << "removeVertexRecursive(vertex)::" << endl;

   centrality.erase(vertex);
   //inEdgesCtr.erase(vertex);
   numNodes--;

   debugOUT << "total number of stripes: " << stripes.size() << endl;
   // -- For each stripe, remove all edges in and out of this node
   //
   foreach(std::string stripe, stripes)
   {
      debugOUT << "Removing vertex " << vertex << " in stripe " << stripe << endl;
      removeVertexRecursive2(stripe, vertex);
   }

   calculated = false;
}


int TopologyModel::removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex) {
   return removeVertexRecursive(stripe, vertex, false);
}

int TopologyModel::removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex, bool loss)
{
   debugOUT << "removeVertexRecursive3(stripe, vertex, loss)::" << endl;

   debugOUT << " * remove vertex " << vertex
            << " recursively in stripe: " << stripe
            << " with loss = " << (loss?"true":"false") << endl;

   debugOUT << "Current inEdgesCtr of " << vertex << " is " << inEdgesCtr[vertex] << endl;
   int affected = 0;

   // -- Remove all incident edges
   // -- This is related to the inbound links.
   //
   //   int numIncomingEdges = 0;
   //   Vertexes::iterator it = graph[stripe].begin();
   //   while (it != graph[stripe].end())
   //   {
   //      //debugOUT << "Investigating vertex " << it->first << endl;
   //      int old = it->second.size();
   //      //debugOUT << "\t Number of children of " << it->first << " is: " << old << endl;
   //      it->second.erase(vertex);
   //      //debugOUT << "\t Number of children of " << it->first
   //      //         << " after deleting " << vertex
   //      //         << " is: " << it->second.size() << endl;
   //      numIncomingEdges += old - it->second.size();
   //      //debugOUT << "\t vertex " << it->first
   //      //         << " with old = " << old
   //      //         << " -- numIncomingEdges = " << numIncomingEdges << endl;
   //      it++;
   //   }
   //   debugOUT << "numIncomingEdges to " << vertex << " = " << numIncomingEdges << endl;
   //   debugOUT << "this->numStripes = " << this->numStripes << endl;
   //   assert(numIncomingEdges <= this->numStripes);

   // -- Remove all adjacent edges
   // -- These are tracked. An acyclic graph is expected.
   //
   if(graph[stripe].find(vertex) != graph[stripe].end())
   {
      debugOUT << "node " << vertex << " is found in stripe " << stripe << endl;

      // -- Find all children of ``vertex''
      //
      PPIPvXAddressSet ads = graph[stripe][vertex];

      // -- Debug
      debugOUT << "All children of node " << vertex << ":" << endl;
      for (PPIPvXAddressSet::iterator iter = ads.begin(); iter != ads.end(); ++iter)
         debugOUT << "\t Child " << *iter << endl;

      // -- Delete node "vertex" on the graph, and increment number of affected nodes
      //
      graph[stripe].erase(vertex);
      //affected++;
      inEdgesCtr[vertex]--; // Giang
      debugOUT << " \\ removeVertexRecursive root" << "->" << vertex << " in " << stripe
               << " decreases #inbound[to]= " << inEdgesCtr[vertex] << " >= 0" << endl;

      if (inEdgesCtr[vertex] < minRequiredStripes)
      {
         debugOUT << "inedgectr of node " << vertex
                  << " = " << inEdgesCtr[vertex]
                  << " -- minRequiredStripes = " << minRequiredStripes << endl;
         debugOUT << "inEdgesCtr less than minNode " << vertex << " with service lost" << endl;
         nodesServiceLost.insert(vertex);
         inEdgesCtr.erase(vertex);
      }

      debugOUT << "Removed " << vertex << " affted = " << affected << endl;

      IPvXAddress old_parent = vertex;
      // -- Browse through the children of vertex
      //
      while (ads.size() > 0)
      {
         IPvXAddress node = *ads.begin();
         debugOUT << "Investigating child node " << node << endl;
         assert(roots.find(node) == roots.end()); // make sure the the node is not the root node :)

         // -- Remove the vertex "node" so that it won't be investigated again
         //
         ads.erase(node);
         assert(graph[stripe].find(node) != graph[stripe].end());

         // -- Get the list of children of vertex "node"
         //
         PPIPvXAddressSet newads = graph[stripe][node]; // hardcopy here

         debugOUT << "-- Children of node " << node << endl;
         for (PPIPvXAddressSet::iterator iter = newads.begin(); iter != newads.end(); ++iter)
            debugOUT << "\t Child " << *iter << endl;

         // -- NOTICE: drop all adjacents of node in stripe here, so they won't be parsed twice
         //
         graph[stripe][node].clear();
         graph[stripe].erase(node);

         inEdgesCtr[node]--; // Giang
         debugOUT << " \\ removeVertexRecursive " << old_parent << "->" << node << " in " << stripe
                  << " decreases #inbound[to]= " << inEdgesCtr[node] << " >= 0" << endl;

         if (inEdgesCtr[node] < minRequiredStripes)
         {
            debugOUT << "inedgectr of node " << node
                     << " = " << inEdgesCtr[node]
                     << " -- minRequiredStripes = " << minRequiredStripes << endl;
            debugOUT << "inEdgesCtr less than minNode " << node << " with service lost" << endl;
            nodesServiceLost.insert(node);
            inEdgesCtr.erase(node);
         }

         // -- kind of recursive (delete the cur_node, and browse through its children instead)
         //
         ads.insert(newads.begin(), newads.end());
         old_parent = node;

      } // while
   }
   else
   {
      DEBUGOUT("  \\ Node was not found");
   }

   debugOUT << "Impacts after removing node " << vertex
            << " in stripe " << stripe
            << "\t -- affected: " << affected
            << "\t -- nodesServiceLoss is " << nodesServiceLost.size() << endl;

   return affected;
}

// -- a modified version for Giang
//
void TopologyModel::removeVertexRecursive2(const std::string& stripe, const IPvXAddress &vertex)
{
   debugOUT << "removeVertexRecursive3(stripe, vertex, loss)::" << endl;

   debugOUT << " * remove vertex " << vertex
            << " recursively in stripe: " << stripe << endl;

   debugOUT << "Current inEdgesCtr of " << vertex << " is " << inEdgesCtr[vertex] << endl;

   // -- Remove all incident edges
   // -- This is related to the inbound links.
   //
   //   int numIncomingEdges = 0;
   //   Vertexes::iterator it = graph[stripe].begin();
   //   while (it != graph[stripe].end())
   //   {
   //      //debugOUT << "Investigating vertex " << it->first << endl;
   //      int old = it->second.size();
   //      //debugOUT << "\t Number of children of " << it->first << " is: " << old << endl;
   //      it->second.erase(vertex);
   //      //debugOUT << "\t Number of children of " << it->first
   //      //         << " after deleting " << vertex
   //      //         << " is: " << it->second.size() << endl;
   //      numIncomingEdges += old - it->second.size();
   //      //debugOUT << "\t vertex " << it->first
   //      //         << " with old = " << old
   //      //         << " -- numIncomingEdges = " << numIncomingEdges << endl;
   //      it++;
   //   }
   //   debugOUT << "numIncomingEdges to " << vertex << " = " << numIncomingEdges << endl;
   //   debugOUT << "this->numStripes = " << this->numStripes << endl;
   //   assert(numIncomingEdges <= this->numStripes);

   // -- Remove all adjacent edges
   // -- These are tracked. An acyclic graph is expected.
   //
   if(graph[stripe].find(vertex) != graph[stripe].end())
   {
      debugOUT << "node " << vertex << " is found in stripe " << stripe << endl;

      // -- Find all children of ``vertex''
      //
      PPIPvXAddressSet ads = graph[stripe][vertex];

      // -- Debug
      debugOUT << "All children of node " << vertex << ":" << endl;
      for (PPIPvXAddressSet::iterator iter = ads.begin(); iter != ads.end(); ++iter)
         debugOUT << "\t Child " << *iter << endl;

      // -- Delete node "vertex" on the graph, and increment number of affected nodes
      //
      graph[stripe].erase(vertex);
      //affected++;
      inEdgesCtr[vertex]--; // Giang
      debugOUT << " \\ removeVertexRecursive root" << "->" << vertex << " in " << stripe
               << " decreases #inbound[to]= " << inEdgesCtr[vertex] << " >= 0" << endl;

      if (inEdgesCtr[vertex] < minRequiredStripes)
      {
         debugOUT << "inedgectr of node " << vertex
                  << " = " << inEdgesCtr[vertex]
                  << " -- minRequiredStripes = " << minRequiredStripes << endl;
         debugOUT << "inEdgesCtr less than minNode " << vertex << " with service lost" << endl;
         nodesServiceLost.insert(vertex);
         inEdgesCtr.erase(vertex);
      }

      IPvXAddress old_parent = vertex;
      // -- Browse through the children of vertex
      //
      while (ads.size() > 0)
      {
         IPvXAddress node = *ads.begin();
         debugOUT << "Investigating child node " << node << endl;
         assert(roots.find(node) == roots.end()); // make sure the the node is not the root node :)

         // -- Remove the vertex "node" so that it won't be investigated again
         //
         ads.erase(node);
         assert(graph[stripe].find(node) != graph[stripe].end());

         // -- Get the list of children of vertex "node"
         //
         PPIPvXAddressSet newads = graph[stripe][node]; // hardcopy here

         debugOUT << "-- Children of node " << node << endl;
         for (PPIPvXAddressSet::iterator iter = newads.begin(); iter != newads.end(); ++iter)
            debugOUT << "\t Child " << *iter << endl;

         // -- NOTICE: drop all adjacents of node in stripe here, so they won't be parsed twice
         //
         graph[stripe][node].clear();
         graph[stripe].erase(node);

         inEdgesCtr[node]--; // Giang
         debugOUT << " \\ removeVertexRecursive " << old_parent << "->" << node << " in " << stripe
                  << " decreases #inbound[to]= " << inEdgesCtr[node] << " >= 0" << endl;

         if (inEdgesCtr[node] < minRequiredStripes)
         {
            debugOUT << "inedgectr of node " << node
                     << " = " << inEdgesCtr[node]
                     << " -- minRequiredStripes = " << minRequiredStripes << endl;
            debugOUT << "inEdgesCtr less than minNode " << node << " with service lost" << endl;
            nodesServiceLost.insert(node);
            inEdgesCtr.erase(node);
         }

         // -- kind of recursive (delete the cur_node, and browse through its children instead)
         //
         ads.insert(newads.begin(), newads.end());
         old_parent = node;

      } // while
   }
   else
   {
      DEBUGOUT("  \\ Node was not found");
   }

   debugOUT << "Impacts after removing node " << vertex
            << " in stripe " << stripe
            << "\t -- nodesServiceLoss is " << nodesServiceLost.size() << endl;
}


//void TopologyModel::removeVertex(const IPvXAddress &vertex) {
//   DEBUGOUT("removeVertex " << vertex);

//   centrality.erase(vertex);
//   inEdgesCtr.erase(vertex);
//   numNodes--;

//   // For each stripe, remove all edges in and out of this node
//   foreach(std::string stripe, stripes) {
//      // Remove all incident edges
//      Vertexes::iterator it = graph[stripe].begin();
//      while (it != graph[stripe].end()) {
//         it->second.erase(vertex);
//         it++;
//      }

//      // Remove all adjacent edges
//      std::set<IPvXAddress>::iterator itset;
//      for (itset = graph[stripe][vertex].begin(); itset != graph[stripe][vertex].end(); itset++) {
//         inEdgesCtr[*itset]--;
//         DEBUGOUT(" \\ removeVertex " << vertex << "->" << *itset << " in " << stripe << " decreases #inbound[to]=" << inEdgesCtr[*itset] << " >= 0");
//      }
//      graph[stripe].erase(vertex);
//   }

//   age.erase(vertex);

//   calculated = false;
//}

int TopologyModel::removeCentralVertex()
{
   debugOUT << "removeCentralVertex::" << endl;

   IPvXAddress vertex = getCentralVertex();
   debugOUT << "Central Vertex: " << vertex << endl;

   int damage = removeVertexRecursive(vertex);

   debugOUT << "Cummulative damage after removing the central vertex = " << damage << endl;
   return damage;
}

void TopologyModel::removeCentralVertex2()
{
   debugOUT << "removeCentralVertex::" << endl;

   // -- Step 1: Find a central vertex
   //
   IPvXAddress vertex = getCentralVertex();
   debugOUT << "Central Vertex: " << vertex << endl;

   // -- Step 2: Remove the central vertex found in step 1
   //
   removeVertexRecursive2(vertex);

}

void TopologyModel::addEdge(const int sequence, const IPvXAddress from, const IPvXAddress to)
{
   debugOUT <<"add edge for sequence " << sequence << " from " << from << " to " <<  to << endl;
   std::string stripe = getString(sequence);
   addEdge(stripe, from, to);
}

void TopologyModel::addEdge(const std::string& stripe, const IPvXAddress from, const IPvXAddress to)
{
   debugOUT << "add edge for stripe " << stripe << ": " << from << " -> " << to << " in " << endl;

   //foreach(std::string s, stripes) std::cout<< " stripe " << s << endl;
   assert(stripes.find(stripe) != stripes.end());

   if(roots.count(to) == 1){
      DEBUGOUT("tried to insert an edge which is inbound for root");
      throw cRuntimeError("there are no inbound edges into the root");
   }


   if(graph[stripe].find(from) == graph[stripe].end()) {
      DEBUGOUT("Edge-Start-Vertex " << from << " not found");
      //std::cout << "Edge-Start-Vertex " << from << " not found" << endl;
      return;
   }
   if(graph[stripe].find(to) == graph[stripe].end()) {
      DEBUGOUT("Edge-End-Vertex " << to << " not found");
      //std::cout << "Edge-End-Vertex " << to << " not found" << endl;
      //throw cRuntimeError("Edge-End-Vertex not found");
      return;
   }

   if (graph[stripe][from].count(to) > 0) {
      // edge already known
      DEBUGOUT(" \\ addEdge "<< from << "->" << to << " in " << stripe << " is duplicate");
      //std::cout << " \\ addEdge "<< from << "->" << to << " in " << stripe << " is duplicate" << endl;
      return;
   }

   assert(graph[stripe][from].find(to) == graph[stripe][from].end());
   graph[stripe][from].insert(to);
   inEdgesCtr[to]++;
   debugOUT << "inEdgesCtr of node " << to << " was increased by one to " << inEdgesCtr[to] << endl;

   assert(inEdgesCtr[to] <= numStripes); /// <--- disabled by Giang
   calculated = false;
}


void TopologyModel::removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {

   std::string stripe = getString(sequence);
   removeEdge(stripe,from, to);
}

int TopologyModel::removeEdgesRecursive(const IPvXAddress& from, const IPvXAddress& to) {

   int damage = 0;
   foreach(std::string stripe, stripes) {
      if(hasEdge(stripe, from, to))
         damage += removeVertexRecursive(stripe,to);
   }

   return damage;
}

void TopologyModel::removeEdge(const std::string& stripe, const IPvXAddress& from, const IPvXAddress& to) {

   DEBUGOUT("removeEdge " << from << "->" << to << " in " << stripe);

   if(roots.count(to) == 1) {
      DEBUGOUT("tried to remove an edge which is inbound for root");
      std::stringstream s;
      s << "In TopologyModel::removeEdge(): There are no inbound edges into root " << to;
      throw cRuntimeError(s.str().c_str());
   }

   if (graph[stripe].find(from) == graph[stripe].end() || (graph[stripe].find(from) != graph[stripe].end()
                                                           && graph[stripe][from].find(to) == graph[stripe][from].end())) {
      // required by LinkTable
      DEBUGOUT(" \\ removeEdge "<<from << "->" << to << " in " << stripe << " does not exist, inEdgesCtr: "<<inEdgesCtr[to]);
      return;
   }

   // edge already known
   graph[stripe][from].erase(to);
   inEdgesCtr[to]--;
   DEBUGOUT(" \\ removeEdge "<<from<<"->" << to << " in " << stripe << " decreases #inbound[to]=" << inEdgesCtr[to] << " >= 0");
   assert(inEdgesCtr[to] >= 0);
   calculated = false;
}


/**
 * Calculates the total successor number for a node in all stripes
 */
int TopologyModel::calculate(const IPvXAddress& vertex) {
   int successors = 0;
   foreach(std::string stripe, stripes)
   {
      if((!vertex.isUnspecified()))
         successors += calculate(vertex, stripe);
   }

   return successors;
}


/**
 * Calculates the total successor number for a node in a single stripe
 */
int TopologyModel::calculate(const IPvXAddress& vertex, std::string& stripe)
{
   if (vertex.isUnspecified()) throw cException("In TopologyModel::calculate(): unspecified node");
   //debugOUT << "calculate "<< vertex << " in stripe " << stripe << endl;

   if(graph[stripe].find(vertex) == graph[stripe].end())
      return 0;

   PPIPvXAddressSet& adj = graph[stripe][vertex];
   int successors = 0;

   foreach(IPvXAddress successor, adj) {
      if((!successor.isUnspecified()))
         successors += calculate(successor,stripe);
   }

   if(roots.count(vertex) == 0)
      centrality[vertex] += successors + 1;

   //DEBUGOUT("calculate(stripe="<<stripe<<", "<<vertex<<") = "<<successors+1);
   return successors + 1;
}

int TopologyModel::calculateSource(const IPvXAddress& vertex, std::string& stripe) {

   assert(!vertex.isUnspecified());
   DEBUGOUT("calculate "<< vertex << " in stripe " << stripe);
   if(graph[stripe].find(vertex) == graph[stripe].end())
      return 0;

   PPIPvXAddressSet& adj = graph[stripe][vertex];
   int successors = 0;

   foreach(IPvXAddress successor, adj) {
      if((!successor.isUnspecified()))
         successors += calculate(successor,stripe);
   }

   if(roots.count(vertex) == 0)
      centrality[vertex] += successors + 1;

   //DEBUGOUT("calculate(stripe="<<stripe<<", "<<vertex<<") = "<<successors+1);
   return successors;
}

/**
 * Resets and triggers centrality calculation
 */
void TopologyModel::calculate()
{
   debugOUT << "calculate::" << endl;

   // Reset centrality
   Centrality::iterator it = centrality.begin();
   while (it != centrality.end())
   {
      it->second = 0;
      it++;
   }

   // Recursively calculate the centrality for each stripe
   foreach(std::string stripe, stripes)
   {
      foreach(IPvXAddress root, roots)
      {
         if (root.isUnspecified())
            throw cException("root unspecified");

         calculate(root, stripe);
      }
   }
   calculated = true;

}


IPvXAddress TopologyModel::getRandomVertex() const {

   if (centrality.size() == 0)
      return IPvXAddress();

   int index = intrand(centrality.size());

   Centrality::const_iterator it = centrality.begin();
   while ((index > 0) && (it != centrality.end())) {
      it++;
      index--;
   };

   return it->first;
}


/**
 * Get most relevant vertex that is not a root node
 */
IPvXAddress TopologyModel::getCentralVertex()
{
   if (centrality.size() == 0)
   {
      debugOUT << "no more vertex in the list of centrality" << endl;
      return IPvXAddress();
   }

   if (!calculated)
   {
      debugOUT << "centrality has NOT been calculated" << endl;
      calculate();
   }

   // Search list for maximum centrality
   int max = 0;
   IPvXAddress vertex;
   Centrality::const_iterator it = centrality.begin();
   while (it != centrality.end())
   {
      if ((roots.find(it->first) == roots.end()) && (it->second >= max))
      {
         max = it->second;
         vertex = it->first;
      }

      it++;
   }

   debugOUT << "vertex " << vertex << " with max centrality " << max << endl;
   return vertex;
}

/**
 * Returns the total node number
 */
int TopologyModel::numberVertexes() const {
   return centrality.size();
}

int TopologyModel::numberSourceVertexes() {

   int connectedNodes = 0;
   foreach(IPvXAddress root, roots) {
      int successors = 0;
      foreach(std::string stripe, stripes)
         successors += calculateSource(root, stripe) ;
      connectedNodes += successors;

   }

   return connectedNodes;
}

/**
 * Returns the total node number per stripe
 *
 * @param stripe
 * @return int
 */
int TopologyModel::numberVertexes(const std::string& stripeId) const {
   assert(graph.find(stripeId) != graph.end());
   return graph.find(stripeId)->second.size();
}


void TopologyModel::removeInboundEdges(const IPvXAddress to) {
   DEBUGOUT("removeInboundEdges (" << to << ")");
   // For each stripe, remove all edges into this node
   foreach(std::string stripe, stripes) {
      // Remove all incident edges
      Vertexes::iterator it = graph[stripe].begin();
      while (it != graph[stripe].end()) {

         it->second.erase(to);
         it++;
      }
   }

   inEdgesCtr[to] = 0;

}

IPvXAddress TopologyModel::getPredecessor(const IPvXAddress& ip, const int sequence) const {

   std::string stripe = getString(sequence);
   return getPredecessor(ip, stripe);
}


/**
 * Returns a predecessor within the topology
 *
 * @params	the observed node ip, and the stripe
 * @return 	the predecessor, or an unspecified IPvXAddress if not found
 * 			or the predecessor == root
 */

IPvXAddress TopologyModel::getPredecessor(const IPvXAddress& ip, const std::string& stripe) const {

   std::string preferredStripe = stripe;

   Graph::const_iterator it0 = graph.find(preferredStripe);
   if (it0 == graph.end()) {
      //std::cout << " pred for ip " << ip << " in stripe " << stripe << " not found " << endl;
      //std::cout << " available stripes: ";
      //foreach(std::string stripe, stripes) std::cout<< stripe <<", ";
      //std::cout<<endl;
      assert(stripes.find(stripe) == stripes.end());

      return IPvXAddress();
   }

   Vertexes::const_iterator it = it0->second.begin();
   while (it != it0->second.end()) {
      for (PPIPvXAddressSet::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
         //if ((*it2) == ip && roots.count(it->first) == 0) return (it->first);
         if ((*it2) == ip) return (it->first);
      }
      it++;
   }

   //std::cout << " pred for ip " << ip << " in stripe " << stripe << " not found " << endl;
   //std::cout << " available stripes: ";
   //foreach(std::string stripe, stripes) std::cout<< stripe <<", ";
   //std::cout<<endl;
   return IPvXAddress();
}


/**
 * Returns a predecessor within the topology
 *
 * @params	the observed node ip, and the stripe stripe
 * @return 	the predecessor (including the root),
 * 			or an unspecified IPvXAddress if not found
 *
 */
IPvXAddress TopologyModel::getPredecessorInclSource(const IPvXAddress& ip, std::string& stripe) const {

   std::string preferredStripe = stripe;
   /*if (stripe.find("-1") != std::string::npos) {
  std::set<std::string>::iterator it = stripes.begin();
  int stripeNum = intuniform(1,numStripes-1);
  while(stripeNum != 0 && it != stripes.end()) {
   stripeNum--;
   it++;
  }
  preferredStripe = *it;
 }*/

   Graph::const_iterator it0 = graph.find(preferredStripe);
   if (it0 == graph.end())
      return IPvXAddress();

   Vertexes::const_iterator it = it0->second.begin();
   while (it != it0->second.end()) {
      for (PPIPvXAddressSet::const_iterator it2 = it->second.begin(); it2!= it->second.end(); it2++) {
         if ((*it2) == ip)
            return (it->first);
         /*if ((*it2) == ip && it->first != root)
    return (it->first);*/
      }
      it++;
   }

   return IPvXAddress();

}


/**
 * @return maximum stripe number used in topology
 */
int TopologyModel::getNumSeenStripes() const {
   return this->numStripes;
}


/**
 * @return age of vertex
 */
double TopologyModel::getAge(const IPvXAddress& ip) const {
   return age.find(ip)->second;
}
void TopologyModel::setAge(const IPvXAddress ip, double nodeAge) {
   if (age.count(ip) == 1)
      age[ip] = nodeAge;
}

void TopologyModel::setNumStripes(int num) {
   numStripes = +num;
}


void TopologyModel::setStripes(std::set<std::string> stripeSet) {
   foreach(std::string str, stripeSet) {
      DEBUGOUT("TopologyModel: adding stripe "<<str );
      stripes.insert(str);
   }
}


void TopologyModel::setNumRequiredStripes(int num) {
   minRequiredStripes = num;
}


void TopologyModel::resetLoss() {
   debugOUT << "loss #0" << endl;
   nodesServiceLost.clear();
}

bool TopologyModel::hasEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) const {
   std::string stripe = getString(sequence);
   return hasEdge(stripe, from, to);
}

bool TopologyModel::hasEdge(const std::string& stripe, const IPvXAddress& from, const IPvXAddress& to) const {

   IPSetSmartPtr adjacents = getGraphAdjacents(stripe, from);
   foreach(const IPvXAddress& addr, *adjacents) {
      if (addr == to) return true;
   }
   return false;

}


int TopologyModel::numInboundEdges(const IPvXAddress& vertex) const {

   if (roots.count(vertex) == 1)
      return 0;
   if (inEdgesCtr.count(vertex) < 1)
      throw cRuntimeError("no such node");
   return inEdgesCtr.find(vertex)->second;

}


IPSetSmartPtr TopologyModel::getGraphAdjacents(const std::string& stripe, const IPvXAddress& ip) const {

   // equals graph[stripe][vertex].size() but runs in const
   if (graph.find(stripe) == graph.end())
      return IPSetSmartPtr(new PPIPvXAddressSet());

   const Vertexes & v = graph.find(stripe)->second;
   if (v.find(ip) == v.end())
      return IPSetSmartPtr(new PPIPvXAddressSet());

   return IPSetSmartPtr(new PPIPvXAddressSet(v.find(ip)->second));

}

const PPStringIntMap TopologyModel::getMaximumTreeDepth() {

   PPStringIntMap depthPerStripe;

   // Recursively calculate the centrality for each stripe
   int currentDepth;
   foreach(std::string stripe, stripes) {
      // initialize known maximum depth
      depthPerStripe[stripe] = 0;
      currentDepth = 0;
      // collect the maximum depth
      foreach(IPvXAddress root, roots){
         currentDepth = getMaximumLevel(root, stripe);
         if(currentDepth > depthPerStripe[stripe]) {
            depthPerStripe[stripe] = currentDepth;
         }
      }
   }

   return depthPerStripe;
}


int TopologyModel::getMaximumLevel(const IPvXAddress& address, std::string& stripe) {

   // check if this Node is within the given stripe
   if(graph[stripe].find(address) == graph[stripe].end())
      return 0;

   // get successors within stripe
   PPIPvXAddressSet& adj = graph[stripe][address];

   int currentDepth = 0;
   int maxDepth = 0;
   foreach(IPvXAddress successor, adj) {
      if((!successor.isUnspecified())) {
         currentDepth = getMaximumLevel(successor,stripe);

         if(currentDepth > maxDepth) {
            maxDepth = currentDepth;
         }
      }
   }
   return maxDepth+1;
}


PPIPvXAddressSet TopologyModel::getChildren(const IPvXAddress& vertex) const {

   PPIPvXAddressSet children; // is a set (not multiset)
   // For each stripe
   foreach(std::string stripe, stripes) {
      IPSetSmartPtr list = getGraphAdjacents(stripe, vertex);
      foreach	(const IPvXAddress& addr, *list)
         children.insert(addr);
   }

   return children;
}


PPIPvXAddressSet TopologyModel::getChildren(const IPvXAddress& vertex, const std::string& stripe) const {

   PPIPvXAddressSet children; // is a set (not multiset)
   IPSetSmartPtr list = getGraphAdjacents(stripe,vertex);
   foreach (const IPvXAddress& addr, *list)
      children.insert(addr);

   return children;
}


PPIPvXAddressSet TopologyModel::getParents(const IPvXAddress& vertex) const {

   PPIPvXAddressSet parents; // is a set (not multiset)
   // For each stripe
   foreach(std::string stripe, stripes) {
      const IPvXAddress& addr = getPredecessor(vertex, stripe);
      if (!addr.isUnspecified())
         parents.insert(addr);
   }
   return parents;
}


/**
 * Returns the child with the most successors
 */
IPvXAddress TopologyModel::getBestChild(const IPvXAddress& father) const {

   PPIPvXAddressSet children = getChildren(father);
   IPvXAddress target;
   int counter = -1;
   foreach(const IPvXAddress& addr, children){
      int succ = countChildren(addr);
      if(succ > counter) {
         target = addr;
         counter = succ;
      }
   }
   return target;
}


/**
 * Returns the child with the most successors in one stripe
 */
IPvXAddress TopologyModel::getBestChild(const IPvXAddress& father, std::string stripe) {

   PPIPvXAddressSet children = getChildren(father,stripe);
   IPvXAddress target;
   int counter = -1;
   foreach(const IPvXAddress& addr, children)
   {
      if (addr.isUnspecified())
         throw cException("addr unspecified");

      int succ = calculate(addr,stripe);
      if(succ > counter) {
         target = addr;
         counter = succ;
      }
   }
   return target;
}


bool TopologyModel::empty() const {
   return(numNodes == 0);
}


void TopologyModel::removeRoot(IPvXAddress root){

   roots.erase(root);
   if(roots.size() == 0)
      throw cRuntimeError("TopologyModel::removeRoot(): No more sources left in stream.");

}

bool TopologyModel::hasVertex(const IPvXAddress& vertex) {

   foreach(std::string stripe, stripes) {
      if(graph[stripe].find(vertex) != graph[stripe].end()) {
         DEBUGOUT("graph for stripe "<< stripe << " contains vertex "<< vertex);
         return true;
      }
   }
   return false;
}


/**
 *  Arranges the heads of this topology (direct successors of roots)
 * in a set
 *
 * @return	set<IPvXAddress> a set of the head nodes of this topology
 */
std::set<IPvXAddress> TopologyModel::getHeads() {

   std::set<IPvXAddress> heads;
   // Iterate across graph
   for(Graph::iterator it = graph.begin(); it != graph.end(); it++) {
      Vertexes& v = it->second;
      // Check for root nodes
      foreach(IPvXAddress root, roots) {

         if(v.find(root) != v.end()) {
            PPIPvXAddressSet& ipList = v[root];
            //std::cout<<" checking root "<< root<< " with fanout of " << ipList.size()<< " in stripe " << it->first<<endl;
            // put all head nodes in list
            foreach(IPvXAddress addr, ipList) {
               heads.insert(addr);
            }
         }
      }
   }

   return heads;
}


/**
 * Calculates head diversity
 *
 * @return int	Ratio of Number of distinct direct successors of root-nodes
 *						and their total direct successor number
 */
double TopologyModel::getHeadDiversity() {

   std::set<IPvXAddress> heads;
   int counter = 0;

   // Iterate across graph
   for(Graph::iterator it = graph.begin(); it != graph.end(); it++) {
      Vertexes& v = it->second;
      // Check for root nodes
      foreach(IPvXAddress root, roots) {
         if(v.find(root) != v.end()) {
            PPIPvXAddressSet& ipList = v[root];
            // put all head nodes in list
            foreach(IPvXAddress addr, ipList) {
               counter++;
               heads.insert(addr);
            }
         }
      }
   }

   if(counter == 0)
      return 0;
   else {
      return ((double)heads.size() / (double)counter);
   }
}


/**
 * Returns true if given node is a direct successor of
 * a source node
 *
 * @param vertex	Potential head node
 *
 * @return bool		true if given node is head node
 */
bool TopologyModel::isHead(const IPvXAddress vertex) {

   std::set<IPvXAddress> heads = getHeads();
   return (heads.find(vertex) != heads.end());
}


/**
 * Returns the number of heads
 * (direct successors of root nodes)
 */
double TopologyModel::getHeadNum() const {
   int headCount = 0;
   // Iterate across graph
   for(Graph::const_iterator it = graph.begin(); it != graph.end(); it++) {
      const Vertexes& v = it->second;
      // Check for root nodes
      foreach(IPvXAddress root, roots) {
         if(v.find(root) != v.end()) {
            const PPIPvXAddressSet& ipList = v.find(root)->second;
            headCount += ipList.size();
         }
      }
   }
   return (double)headCount;
}


/**
 * Returns the head node of the given node in the given stripe
 *
 * @param stripe
 * @param successor
 *
 * @return Reference to head
 */
IPvXAddress TopologyModel::getHead(std::string stripe, const IPvXAddress& address) {

   DEBUGOUT("getHead for address "<< address <<" in stripe " << stripe);
   assert(!address.isUnspecified());
   IPvXAddress head = IPvXAddress();
   if(roots.find(address) == roots.end()) {
      IPvXAddress pred = getPredecessor(address,stripe);
      DEBUGOUT(" - node " << pred);
      while (!pred.isUnspecified() && roots.find(pred) == roots.end()) {
         head = pred;
         pred = getPredecessor(pred,stripe);
         DEBUGOUT(" - node " << pred);
      }
   }
   DEBUGOUT("  - got head " << head);
   return head;
}


/**
 * Check a node if its head nodes (need not be direct predecessors)
 * have more successors than itself in stripes it forwards
 *
 * @param address		The observed node
 *
 * @return bool			True if condition is met
 */
bool TopologyModel::getNodeStability(const IPvXAddress& address) {

   bool stable = true;
   int succ = countSuccessors(address);
   foreach(std::string stripe, stripes) {
      if(	graph.find(stripe) != graph.end()
            && graph[stripe].find(address) != graph[stripe].end()
            && countSuccessors(address, stripe) > 0) {
         IPvXAddress head = getHead(stripe, address);
         if(!head.isUnspecified()) stable &= (succ < countSuccessors(head));
      }
   }
   return stable;
}

/**
 * Calculates the Smallest-Head-Stability-Requirement across all nodes
 * (node must not have more successors across all stripes than its head nodes in stripes it forwards)
 *
 * @return Fraction of nodes satisfying the requirement
 */
double TopologyModel::getSmallestHeadStability() {

   if(centrality.size() == 0) return 1;

   int stableNodes = 0;
   for(Centrality::iterator it = centrality.begin(); it != centrality.end(); it++) {
      stableNodes += (int) getNodeStability(it->first);
   }
   double stable = (((double)stableNodes) / ((double)centrality.size()));
   return stable;
}

int TopologyModel::getNumRootSuccessors() {

   int counter = 0;
   foreach(IPvXAddress addr, roots) {
      int count = countSuccessors(addr);
      counter += count;
   }

   return counter;
}

PPEdgeList TopologyModel::getEdges(const int sequence) {

   std::string stripe = getString(sequence);
   return getEdges(stripe);
}

PPEdgeList TopologyModel::getEdges(std::string stripe) {
   PPEdgeList edges;
   edges.clear();
   if (graph.count(stripe) == 0)
   {
      EV << "no stripe!!!" << endl;
      return edges;
   }
   else
   {
      EV << "there is one stripe!!!" << endl;
   }

   foreach (Vertexes::value_type node, graph[stripe])
   {
      EV << "node " << node.first << endl;
      foreach (PPIPvXAddressSet::value_type adjacent, node.second)
      {
         EV << "\tadjacent: " << adjacent << endl;
         edges.push_back(PPEdge(node.first, adjacent, age[node.first], age[adjacent]));
      }
   }

   // -- Debugging:
   EV << "print edge list with size " << edges.size() << ": " << endl;
   for (PPEdgeList::iterator iter = edges.begin(); iter != edges.end(); ++iter)
   {
      EV << iter->begin() << " - " << iter->end() << endl;
   }

   return edges;
}


PPIPvXAddressSet TopologyModel::getVertexes() {
   PPIPvXAddressSet vertexes;
   foreach (const Centrality::value_type& central, centrality)
      vertexes.insert(central.first);
   return vertexes;
}

/**
 * add modelToInsert to this topology
 */
void TopologyModel::insertTopology(TopologyModel modelToInsert)
{
   debugOUT << "insertTopology" << endl;

   //std::cout<<" globalTopology: " << getTotalNodeNumber() << " - inserting topology of size " << modelToInsert.getTotalNodeNumber()<<endl;
   if(modelToInsert.empty()) return;

   setStripes(modelToInsert.getStripes());
   numStripes = stripes.size();
   setNumStripes(modelToInsert.getNumSeenStripes());

   // Set root nodes
   foreach(std::set<IPvXAddress>::value_type ip, modelToInsert.roots)
      this->setRoot(ip);

   // Iterate across all stripes of graph
   foreach(TopologyModel::Graph::value_type stripe, modelToInsert.graph) {
      std::string stripeId = stripe.first;

      DEBUGOUT("inserting elements of stripe " <<stripeId << "  to global topology");
      // first append all nodes
      foreach(TopologyModel::Vertexes::value_type edge, stripe.second) {
         IPvXAddress node = edge.first;
         this->addVertex(stripeId,node);
      }

      // add edges
      foreach(TopologyModel::Vertexes::value_type edge, stripe.second) {
         PPIPvXAddressSet toNodeList = edge.second;
         IPvXAddress from = edge.first;
         foreach(IPvXAddress to, toNodeList) this->addEdge(stripeId, from, to);
      }

   }

   //std::cout<<"    -> globalTopology know with " << getTotalNodeNumber()<< " nodes " << endl;
   calculate();
}

void TopologyModel::insertTopology2(TopologyModel modelToInsert)
{
   debugOUT << "insertTopology" << endl;

   if(modelToInsert.empty()) return;

   // -- Stripes
   //
   setStripes(modelToInsert.getStripes());
   numStripes = stripes.size();

   debugOUT << "modelToInsert.getNumSeenStripes = " << modelToInsert.getNumSeenStripes() << endl;
   debugOUT << "numStripes after insertion " << numStripes << endl;

   debugOUT << "inEdges before merging:" << endl;
   for (Centrality::iterator iter = this->inEdgesCtr.begin(); iter != this->inEdgesCtr.end(); ++iter)
   {
      debugOUT << "\t vertex " << iter->first << " -- " << iter->second << endl;
   }

   Centrality copyInEdges = inEdgesCtr;

   debugOUT << "inEdges of model to be inserted:" << endl;
   Centrality temp_inEdges = modelToInsert.getInEdgesCountList();
   for (Centrality::iterator iter = temp_inEdges.begin(); iter != temp_inEdges.end(); ++iter)
   {
      debugOUT << "\t vertex " << iter->first << " -- " << iter->second << endl;
   }

   // Set root nodes
   foreach(std::set<IPvXAddress>::value_type ip, modelToInsert.roots)
      this->setRoot(ip);

   // Iterate across all stripes of graph
   foreach(TopologyModel::Graph::value_type stripe, modelToInsert.graph)
   {
      std::string stripeId = stripe.first;

      DEBUGOUT("inserting elements of stripe " <<stripeId << "  to global topology");
      // first append all nodes
      foreach(TopologyModel::Vertexes::value_type edge, stripe.second) {
         IPvXAddress node = edge.first;
         this->addVertex(stripeId,node);
      }

      // add edges
      foreach(TopologyModel::Vertexes::value_type edge, stripe.second) {
         PPIPvXAddressSet toNodeList = edge.second;
         IPvXAddress from = edge.first;
         foreach(IPvXAddress to, toNodeList) this->addEdge(stripeId, from, to);
      }
   }

   // -- Do the trick, since addVertex will reset the values of inEdgesCtr[]
   //
   for (Centrality::iterator iter = copyInEdges.begin(); iter != copyInEdges.end(); ++iter)
   {
      if (this->inEdgesCtr.find(iter->first) == inEdgesCtr.end())
      {
         this->inEdgesCtr[iter->first] = 1;
      }
      else
      {
         this->inEdgesCtr[iter->first] += iter->second;
      }
   }

   debugOUT << "inEdges after merging:" << endl;
   for (Centrality::iterator iter = this->inEdgesCtr.begin(); iter != this->inEdgesCtr.end(); ++iter)
   {
      debugOUT << "\t vertex " << iter->first << " -- " << iter->second << endl;
   }
}

std::string TopologyModel::getString(const int sequence) const {

   int temp = sequence;
   // string conversion
   std::ostringstream convert;
   convert << temp;
   return convert.str();
}

void TopologyModel::printCentralityList()
{
   debugOUT << "printCentrality::" << endl;

   // -- Debug
   debugOUT << "Print centrality list" << endl;
   for (Centrality::iterator iter = centrality.begin(); iter != centrality.end(); ++iter)
   {
      debugOUT << "vertex " << iter->first << " -- its centrality " << iter->second << endl;
   }
}

void TopologyModel::printIncomingEdgeList()
{
   debugOUT << "printIncomingEdgeList::" << endl;

   for (Centrality::iterator iter = inEdgesCtr.begin(); iter != inEdgesCtr.end(); ++iter)
   {
      debugOUT << "\t vertex " << iter->first << " -- its number of incoming edges " << iter->second << endl;
   }
}

PPIPvXAddressIntMap TopologyModel::getInEdgesCountList()
{
   return inEdgesCtr;
}

void TopologyModel::writeTopologyToDotFile(std::string folder)
{
   debugOUT << "writeTopologyToDotFile::" << endl;

   for (std::set<std::string>::iterator iter = stripes.begin(); iter != stripes.end(); ++iter)
   {
      std::string cur_stripe = *iter;
      debugOUT << "Parsing stripe " << cur_stripe << endl;
      std::string filename = folder + "//" + cur_stripe + ".dot";
      std::ofstream dotFile(filename.c_str(), fstream::out);
      debugOUT << "Output file for stripe " << cur_stripe << " is " << filename << endl;

      // -- Get list of vertexSet corresponding to each stripe
      //
      dotFile << "digraph graphname {" << endl;

      // -- Browse through the vertexes
      //
      for (Vertexes::iterator it = graph[cur_stripe].begin(); it != graph[cur_stripe].end(); ++it)
      {
         IPvXAddress cur_parent = it->first;
         debugOUT << "Parent node: " << cur_parent << endl;

         std::set<IPvXAddress> childrenSet = it->second;
         for (std::set<IPvXAddress>::iterator child_iter = childrenSet.begin();
              child_iter != childrenSet.end(); ++child_iter)
         {
            IPvXAddress cur_child = *child_iter;
            dotFile << cur_parent.str().erase(0, 10) << " -> "
                    << cur_child.str().erase(0, 10) << ";" << endl;
         }


      }
      dotFile << "}" << endl;

      dotFile.close();
   }

   debugOUT << "storeOverlayTopology" << endl;

   // -- Write to file
   //


   //   m_overlayTopologyFile.close();
}
