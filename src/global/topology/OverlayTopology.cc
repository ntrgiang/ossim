#include "OverlayTopology.h"
#include <boost/lexical_cast.hpp>
#include <arpa/inet.h>

using namespace std;
using boost::lexical_cast;

Define_Module(OverlayTopology);


void OverlayTopology::initialize(int stage) {

}

void OverlayTopology::finish() {
	//close socket if necessary
	if(socketExport) close(fd_client_socket);
}

void OverlayTopology::handleMessage(cMessage* msg) {
	throw cRuntimeError("OverlayTopology::handleMessage");
}

/**
 * Opens a socket to the VisualAlm-Program
 */
void OverlayTopology::openClientSocket() {

	struct sockaddr_in addr;

	fd_client_socket = socket(PF_INET,SOCK_STREAM,0);
	if(fd_client_socket == -1)
		throw cRuntimeError("could not create socket");

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(((int)par("sendPort")));
	addr.sin_family = AF_INET;

	if (connect(fd_client_socket, (struct sockaddr*) &addr, sizeof(addr)) == -1)
		throw cRuntimeError("could not bind to socket via connect()");
}


/**
 * Sends a string via socket
 *
 * @param str	The string to send
 */
void OverlayTopology::sendSocket(std::string str) {

	if(!socketExport)
		return;

	//std::cout<<str<<endl;

	str = str + "\r\n";
	int bytes;
	//create char array out of string
	char data[str.size()];
	for(unsigned int i=0; i <str.size(); i++)
		data[i] = str[i];

	// send the packet
	bytes = ::send(fd_client_socket, data, strlen(data), 0);
	if (bytes == -1){
		throw cRuntimeError("nothing send");
	}

	if(socketExport) sleep(0.25);
}

/**
 * Set this node as root in the respective TopologyModels
 *
 * @param root  The IP of this root
 * @param stream	The respective stream
 */
void OverlayTopology::setRoot(const IPvXAddress & root, Stream* stream) {
	if(topo.find(stream->getId()) != topo.end())
		topo[stream->getId()].setRoot(root);

	foreach(Stripe* stripe, stream->getStripes()) {
		sendSocket("addSource "  + stripe->getId() + " " + root.str() );
	}
}


void OverlayTopology::addNode(const IPvXAddress &ip, Stream *stream) {
	Enter_Method_Silent();
	addNode(ip, stream, false);
}

void OverlayTopology::addNode(const IPvXAddress& ip, Stream* stream, bool isAlmRouter) {
    Enter_Method_Silent();
	assert(topo.find(stream->getId()) != topo.end());
    if(topo.find(stream->getId()) != topo.end()){
		topo[stream->getId()].addVertex(stream,ip);


		foreach(Stripe* stripe, stream->getStripes()) {
			sendSocket("addNode " + stripe->getId() + " " + ip.str());
		}
    }
}


void OverlayTopology::removeNode(const IPvXAddress& ip, Stream* stream) {
    Enter_Method_Silent();
	assert(topo.find(stream->getId()) != topo.end());
	topo[stream->getId()].removeVertex(stream,ip);
	foreach(Stripe* stripe, stream->getStripes())
		sendSocket("removeNode " + stripe->getId()  + " " + ip.str());
}


void OverlayTopology::addEdge(Stripe* stripe, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();
    topo[stripe->getStream()->getId()].addEdge(stripe->getId(),from,to);
    sendSocket("addEdge " + stripe->getId() + " " + from.str() + " " + to.str());
}


void OverlayTopology::removeEdge(Stripe* stripe, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();
	topo[stripe->getStream()->getId()].removeEdge(stripe->getId(),from,to);
	sendSocket("removeEdge " + stripe->getId() + " " + from.str() + " " + to.str());
}


TopologyModel OverlayTopology::getTopology(const int streamNum) {
    Enter_Method_Silent();
    assert(topo.find(streamNum) != topo.end());
	return topo[streamNum];
}


TopologyModel& OverlayTopology::getTopologyReference(const int streamNum) {
    Enter_Method_Silent();
    assert(topo.find(streamNum) != topo.end());
	return topo[streamNum];
}


TopologyModel OverlayTopology::getTopology(const Stream* stream) {
    Enter_Method_Silent();
    assert(topo.find(stream->getId()) != topo.end());
	return topo[stream->getId()];
}


TopologyModel& OverlayTopology::getTopologyReference(Stream* stream) {
    Enter_Method_Silent();
    assert(topo.find(stream->getId()) != topo.end());
    return topo[stream->getId()];
}


TopologyModel* OverlayTopology::getTopologyPointer(Stream* stream) {
    Enter_Method_Silent();
    assert(topo.find(stream->getId()) != topo.end());
    return &topo[stream->getId()];
}


int OverlayTopology::getNumVertices(const int streamNum) {
	assert(topo.find(streamNum) != topo.end());
	return topo[streamNum].numberVertexes();
}


int OverlayTopology::getNumVertices(const Stripe *stripe) {
	assert(topo.find(stripe->getStreamId()) != topo.end());
	return(topo[stripe->getStreamId()].numberVertexes(stripe->getId()));
}


int OverlayTopology::getNumVerticesInSimulation(){
	
	return globalTopology().numberVertexes() - globalTopology().numberSources();
}


TopologyModel OverlayTopology::globalTopology(){
	
    if(topo.size() == 1) {
        return topo.begin()->second;
    }

	TopologyModel globalTopology = TopologyModel();
	int lossMinStripes = ((int) par("lossMinStripes"));
	globalTopology.setNumRequiredStripes(lossMinStripes);

	foreach(TopologyMap::value_type topology, topo){
		globalTopology.insertTopology(topology.second);
	}

	return globalTopology;
}


int OverlayTopology::getNumTopologies() {
    if(gT->getSCGEnabled()) {
        return topo.size()-1;
    }
	return topo.size();
}


/// funtions for hierarchy
void OverlayTopology::addHierarchyTopology(const int sequence) {
    Stream* stream = new Stream(1, 1, 1,0);
    streamingHierarchy[sequence] = TopologyModel();
    streamingHierarchy[sequence].setStripes(stream);
    streamingHierarchy[sequence].setNumStripes(1);
    streamingHierarchy[sequence].setNumRequiredStripes(1);
    delete stream;
    stream = NULL;
};


/// funtions for hierarchy per time
void OverlayTopology::addHierarchyTopology() {
    if (streamingHierarchyPerTime != NULL)
        removeHierarchyPerTimeTopology();
    Stream* stream = new Stream(1, 1, 1,0);
    streamingHierarchyPerTime = new MeshTopologyModel();
    streamingHierarchyPerTime->setStripes(stream);
    streamingHierarchyPerTime->setNumStripes(1);
    streamingHierarchyPerTime->setNumRequiredStripes(1);
    delete stream;
    stream = NULL;
};


bool OverlayTopology::allNodesAddedToHierarchy(const int sequence) {
    if (streamingHierarchy.count(sequence) == 0)
        return false;
    return streamingHierarchy[sequence].numberVertexes() == bootstrap()->getNumOnlineNodes();
}


void OverlayTopology::addNode(const IPvXAddress& ip, const int sequence) {
    if (streamingHierarchy.count(sequence) == 1) {
        Stream* stream = new Stream(1, 1, 1,0);
        streamingHierarchy[sequence].addVertex(stream, ip);
        delete stream;
        stream = NULL;
    }
}


/**
  * \param addToStreamingHierarchy add the edge to the packet hierarchy, otherwise to time hierarchy. default true
  */
void OverlayTopology::addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to, const bool addToStreamingHierarchy) {
    if (addToStreamingHierarchy) {
        if (streamingHierarchy.count(sequence) == 1) {
            Stream* stream = new Stream(1, 1, 1,0);
            Stripe* stripe = new Stripe(stream, 0);
            streamingHierarchy[sequence].addEdge(stripe->getId(), from, to);
            delete stripe;
            delete stream;
            stream = NULL;
            stripe = NULL;
        }
    } else {
        streamingHierarchyPerTime->addEdge(sequence, from, to);
    }
}


int OverlayTopology::getNumVerticesOfHierarchy(const int sequence) {
    if (streamingHierarchy.count(sequence) == 1) {
        return streamingHierarchy[sequence].numberVertexes();
    } else {
        return 0;
    }
}


void OverlayTopology::setJoinTime(const int sequence, IPvXAddress node, double joinTime) {
    if (streamingHierarchy.count(sequence) == 1)
        streamingHierarchy[sequence].setAge(node, joinTime);
}


void OverlayTopology::setJoinTime(IPvXAddress node, double joinTime) {
    streamingHierarchyPerTime->setAge(node, joinTime);
}


PPEdgeList OverlayTopology::getEdges(const int sequence) {
    if (streamingHierarchy.count(sequence) == 1) {
        Stream* stream = new Stream(1, 1, 1,0);
        Stripe* stripe = new Stripe(stream, 0);
        return streamingHierarchy[sequence].getEdges(stripe->getId());
        delete stripe;
        stripe = NULL;
        delete stream;
        stream = NULL;
    }
}


TopologyModel OverlayTopology::getHierarchyPerSequenceTopology(const int sequence) {
    if (streamingHierarchy.count(sequence) == 1) {
        return streamingHierarchy[sequence];
    } else {
        return TopologyModel();
    }
}


MeshTopologyModel* OverlayTopology::getHierarchyPerTimeTopology() {
    return streamingHierarchyPerTime;
}


void OverlayTopology::removeHierarchyPerTimeTopology() {
    if (streamingHierarchyPerTime != NULL)
        delete streamingHierarchyPerTime;
    streamingHierarchyPerTime = NULL;
}
