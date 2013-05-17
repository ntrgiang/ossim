#include "TopologyModel.h"
#include <iomanip>
#include <queue>


#if _DEBUG
# define DEBUGOUT(x) if (C_DEBUG_TopologyModel) { std::cout << "(TM  @" << "0x" << std::setbase(16) << ((long)this) << "): " << std::setbase(10) << x << endl;; };
#else
# define DEBUGOUT(x)
#endif


TopologyModel::TopologyModel() {
    calculated = true;
    numStripes = 0;
    numNodes = 0;
    this->minRequiredStripes = 1;
    // FIXME: 	this is a hack to prevent that the data structure
    // 		has to be changed to cover mesh-based streaming
    stripes.insert("0-0");
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
    return calculate(vertex, stripe);
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
void TopologyModel::addVertex(const IPvXAddress vertex) {

    addVertex("0-0", vertex);
}


void TopologyModel::addVertex(const std::string& stripe, const IPvXAddress vertex) {

    DEBUGOUT("addVertex " << vertex);
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
int TopologyModel::removeVertexRecursive(const IPvXAddress& vertex) {

    bool loss = (minRequiredStripes > 0);
    DEBUGOUT("removeVertexRecursive " << vertex << ", loss=" << (loss ? "true" : "false"));

    int affected = 0;
    if (loss)
        affected = nodesServiceLost.size();

    centrality.erase(vertex);
    inEdgesCtr.erase(vertex);
    numNodes--;

    int succ = countSuccessors(vertex);

    // For each stripe, remove all edges in and out of this node
    foreach(std::string stripe, stripes)
        affected += removeVertexRecursive(stripe, vertex, loss);

    calculated = false;
    DEBUGOUT(" * affected = " << affected << " and original successors " << succ);
    assert(affected == succ);

    if (loss)
        return nodesServiceLost.size() - affected;
    else
        return affected;
}


int TopologyModel::removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex) {
    return removeVertexRecursive(stripe, vertex, false);
}

int TopologyModel::removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex, bool loss) {

    DEBUGOUT(" * remove vertex " << vertex << " recursively in stripe: " << stripe);
    int affected = 0;
    // Remove all incident edges
    // This is related to the inbound links.
    int numIncomingEdges = 0;
    Vertexes::iterator it = graph[stripe].begin();
    while (it != graph[stripe].end()) {
        int old = it->second.size();
        it->second.erase(vertex);
        numIncomingEdges += old - it->second.size();
        it++;
    }
    assert(numIncomingEdges <= this->numStripes);

    // Remove all adjacent edges
    // These are tracked. An acyclic graph is expected.
    if(graph[stripe].find(vertex) != graph[stripe].end()) {

        DEBUGOUT("node " << vertex << " found in stripe " << stripe);
        PPIPvXAddressSet ads = graph[stripe][vertex];
        graph[stripe].erase(vertex);
        affected++;

        while (ads.size() > 0) {
            IPvXAddress node = *ads.begin();
            assert(roots.find(node) == roots.end());
            DEBUGOUT("   * child: " << node);
            ads.erase(node);
            assert(graph[stripe].find(node) != graph[stripe].end());
            PPIPvXAddressSet newads = graph[stripe][node]; // hardcopy here
            // NOTICE: drop all adjacents of node in stripe here, so they won't be parsed twice
            graph[stripe][node].clear();
            graph[stripe].erase(node);

            PPIPvXAddressSet::iterator it = newads.begin();
            while (it != newads.end()) {
                inEdgesCtr[*it]--;
                DEBUGOUT(" \\ removeVertexRecursive " << node << "->" << *it << " in " << stripe
                                    << " decreases #inbound[to]=" << inEdgesCtr[*it] << " >= 0");
                it++;
            }

            ads.insert(newads.begin(), newads.end());
            if (loss) {
                if (inEdgesCtr[node] < minRequiredStripes)
                    nodesServiceLost.insert(node);
            } else
                affected++;
        }
    } else {
        DEBUGOUT("  \\ Node was not found");
    }

    return affected;
}


void TopologyModel::removeVertex(const IPvXAddress &vertex) {
    DEBUGOUT("removeVertex " << vertex);

    centrality.erase(vertex);
    inEdgesCtr.erase(vertex);
    numNodes--;

    // For each stripe, remove all edges in and out of this node
    foreach(std::string stripe, stripes) {
        // Remove all incident edges
        Vertexes::iterator it = graph[stripe].begin();
        while (it != graph[stripe].end()) {
            it->second.erase(vertex);
            it++;
        }

        // Remove all adjacent edges
        std::set<IPvXAddress>::iterator itset;
        for (itset = graph[stripe][vertex].begin(); itset != graph[stripe][vertex].end(); itset++) {
            inEdgesCtr[*itset]--;
            DEBUGOUT(" \\ removeVertex " << vertex << "->" << *itset << " in " << stripe << " decreases #inbound[to]=" << inEdgesCtr[*itset] << " >= 0");
        }
        graph[stripe].erase(vertex);
    }

    //FIXME age currently not multi-stream capable
    age.erase(vertex);

    calculated = false;
}


void TopologyModel::addEdge(const IPvXAddress from, const IPvXAddress to) {

    addEdge("0-0", from, to);
}

void TopologyModel::addEdge(const std::string& stripe, const IPvXAddress from, const IPvXAddress to) {

    DEBUGOUT("addEdge " << from << "->" << to << " in " << stripe);

    if(roots.count(to) == 1){
        DEBUGOUT("tried to insert an edge which is inbound for root");
        throw cRuntimeError("there are no inbound edges into the root");
    }

    if (stripes.find(stripe) == stripes.end()) {
        DEBUGOUT("stripe unknown");
		    assert(stripes.find(stripe) != stripes.end());
        return;
		}

    if(graph[stripe].find(from) == graph[stripe].end()) {
        DEBUGOUT("Edge-Start-Vertex " << from << " not found");
        //throw cRuntimeError("Edge-Start-Vertex not found");
        return;
    }
    if(graph[stripe].find(to) == graph[stripe].end()) {
        DEBUGOUT("Edge-End-Vertex " << to << " not found");
        //throw cRuntimeError("Edge-End-Vertex not found");
        return;
    }

    if (graph[stripe][from].count(to) > 0) {
        // edge already known
        DEBUGOUT(" \\ addEdge "<< from << "->" << to << " in " << stripe << " is duplicate");
        return;
    }

    assert(graph[stripe][from].find(to) == graph[stripe][from].end());
    graph[stripe][from].insert(to);
    inEdgesCtr[to]++;
    DEBUGOUT(" \\ addEdge "<<from <<"->" << to << " in " << stripe << " increases #inbound[to]=" << inEdgesCtr[to] << " <= " << numStripes);

    assert(inEdgesCtr[to] <= numStripes);
    calculated = false;
}


void TopologyModel::removeEdge(const IPvXAddress& from, const IPvXAddress& to) {

    removeEdge("0-0",from, to);
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
        successors += calculate(vertex, stripe);

    return successors;
}


/**
 * Calculates the total successor number for a node in a single stripe
 */
int TopologyModel::calculate(const IPvXAddress& vertex, std::string& stripe) {

    if (vertex.isUnspecified()) throw cRuntimeError("In TopologyModel::calculate(): unspecified node");
    //DEBUGOUT("calculate "<< vertex << " in stripe " << stripe);
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
void TopologyModel::calculate() {

    // Reset centrality
    Centrality::iterator it = centrality.begin();
    while (it != centrality.end()) {
        it->second = 0;
        it++;
    }

    // Recursively calculate the centrality for each stripe
    foreach(std::string stripe, stripes) {
        foreach(IPvXAddress root, roots){
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


IPvXAddress TopologyModel::getCentralVertex() {

    if (centrality.size() == 0)
        return IPvXAddress();

    if (!calculated)
        calculate();

    // Search list for maximum centrality
    int max = 0;
    IPvXAddress vertex;
    Centrality::const_iterator it = centrality.begin();
    while (it != centrality.end()) {

        if ((roots.find(it->first) == roots.end()) && (it->second >= max)) {

            max = it->second;
            vertex = it->first;
        }

        it++;
    }

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


/**
 * Returns a predecessor within the topology
 *
 * @params	the observed node ip, and the stripe
 * @return 	the predecessor, or an unspecified IPvXAddress if not found
 * 			or the predecessor == root
 */

IPvXAddress TopologyModel::getPredecessor(const IPvXAddress& ip, const std::string& stripe) const {

    std::string preferredStripe = stripe;
    /*if (stripe.find("-1") != std::string::npos) {
  throw cRuntimeError("TopologyModel::getPredecessor");
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
        for (PPIPvXAddressSet::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            if ((*it2) == ip && roots.count(it->first) == 0) return (it->first);
        }
        it++;
    }
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
    nodesServiceLost.clear();
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
    foreach(const IPvXAddress& addr, children){
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


/**
 * add modelToInsert to this topology
 */
void TopologyModel::insertTopology(TopologyModel modelToInsert) {

    //std::cout<<" globalTopology: " << getTotalNodeNumber() << " - inserting topology of size " << modelToInsert.getTotalNodeNumber()<<endl;
    if(modelToInsert.empty()) return;

    setStripes(modelToInsert.getStripes());
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

PPEdgeList TopologyModel::getEdges() {

    return getEdges("0-0");
}

PPEdgeList TopologyModel::getEdges(std::string stripe) {
    PPEdgeList edges;
    edges.clear();
    if (graph.count(stripe) == 0) return edges;
    foreach (Vertexes::value_type node, graph[stripe]) {
        foreach (PPIPvXAddressSet::value_type adjacent, node.second) {
            edges.push_back(PPEdge(node.first, adjacent, age[node.first], age[adjacent]));
        }
    }
    return edges;
}


PPIPvXAddressSet TopologyModel::getVertexes() {
    PPIPvXAddressSet vertexes;
    foreach (const Centrality::value_type& central, centrality)
        vertexes.insert(central.first);
    return vertexes;
}
