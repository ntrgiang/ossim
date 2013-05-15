#ifndef OVERLAYTOPOLOGY_H
#define OVERLAYTOPOLOGY_H

#include "TopologyModel.h"

class OverlayTopology : public cSimpleModule {
    typedef std::map<int, TopologyModel>::iterator Iterator;
    typedef std::map<int, TopologyModel>           TopologyMap;
    typedef std::list<PPEdge>                       PPEdgeList;

private:

    TopologyMap        topo;            ///< the topology data structure < streamID, Topology >


    /// Topology per sequence
    TopologyMap        streamingHierarchy;
    MeshTopologyModel* streamingHierarchyPerTime;

private:
    void openClientSocket();
    void sendSocket(std::string str);

public:
    void initialize(int);
    virtual int numInitStages() const { return 5; }
    void finish();
    void handleMessage(cMessage* msg);

    void setRoot(const IPvXAddress& root, Stream* stream);
    void addNode(const IPvXAddress& ip, Stream* stream);
    void addNode(const IPvXAddress& ip, const int sequence);  ///< for hierarchy per sequence
    void removeNode(const IPvXAddress& ip, Stream* stream);

    void addEdge(Stripe* stripe, const IPvXAddress& from, const IPvXAddress& to);
    void addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to, const bool addToStreamingHierarchy = true); ///< for hierarchy per sequence
    void removeEdge(Stripe* stripe, const IPvXAddress& from, const IPvXAddress& to);

    TopologyModel  getTopology(const int streamNum);
    TopologyModel  getTopology(const Stream* stream);
    TopologyModel& getTopologyReference(const int streamNum);
    TopologyModel& getTopologyReference(Stream* stream);
    TopologyModel* getTopologyPointer(Stream* stream);

    int           getNumVertices(const int streamNum);
    int           getNumVerticesOfHierarchy(const int sequence);
    int           getNumVertices(const Stripe* stripe);
    int           getNumVerticesInSimulation();
    TopologyModel globalTopology();
    int           getNumTopologies();

    // topology per packet
    TopologyModel      getHierarchyPerSequenceTopology(const int sequence);              ///< for hierarchy per sequence
    MeshTopologyModel* getHierarchyPerTimeTopology();                                    ///< for hierarchy per simtime
    inline void        removeHierarchyTopology(const int sequence)         { streamingHierarchy.erase(sequence); }
    PPEdgeList         getEdges(const int sequence);
    void  addHierarchyTopology(const int sequence);
    void  addHierarchyTopology();
    bool  allNodesAddedToHierarchy(const int sequence);
    void  removeHierarchyPerTimeTopology();
    void  setJoinTime(const int sequence, IPvXAddress node, double joinTime);
    void  setJoinTime(IPvXAddress node, double joinTime);
};


class OverlayTopologyAccess : public INetModuleAccess<OverlayTopology> {
  public:
    OverlayTopologyAccess() :
            INetModuleAccess<OverlayTopology>("overlayTopology") {}
};

#endif // OVERLAYTOPOLOGY_H
