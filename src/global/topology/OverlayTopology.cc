#include "OverlayTopology.h"
#include <boost/lexical_cast.hpp>
#include <arpa/inet.h>

using namespace std;
using boost::lexical_cast;

Define_Module(OverlayTopology);


void OverlayTopology::initialize() {

}

void OverlayTopology::finish() {
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

    assert(topo.find(sequence) != topo.end());
    if(topo.find(sequence) != topo.end())
        topo[sequence].setRoot(root);

}


void OverlayTopology::removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].removeEdge(from,to);
}


int OverlayTopology::getNumTopologies() {

	return topo.size();
}


void OverlayTopology::addNode(const IPvXAddress& ip, const int sequence) {
    Enter_Method_Silent();

    topo[sequence].addVertex(ip);
}

void OverlayTopology::removeNode(const IPvXAddress& ip, const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].removeVertex(ip);
}


/**
  * \param add the edge
 */
void OverlayTopology::addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].addEdge(from, to);
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

    assert(topo.find(sequence) != topo.end());
    return topo[sequence].getEdges();
}


TopologyModel OverlayTopology::getSequenceTopology(const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    return topo[sequence];
}

TopologyModel& OverlayTopology::getSequenceTopologyRef(const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    return topo[sequence];
}
