#ifndef TOPOLOGYMODEL_H
#define TOPOLOGYMODEL_H

#include "PPDatatypes.h"
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

typedef boost::shared_ptr<PPIPvXAddressSet>	IPSetSmartPtr;

class TopologyModel {
protected:
    typedef std::map<IPvXAddress, PPIPvXAddressSet> Vertexes;
    typedef std::map<std::string, Vertexes>         Graph;
    typedef PPIPvXAddressIntMap                     Centrality;
    typedef PPIPvXAddressDoubleMap                  Nodetime;
    typedef std::map<IPvXAddress, unsigned int>     LoadMap;    ///< number of extra incoming packets

private:
    Graph            graph;

protected:
    Centrality       centrality;
    Centrality       inEdgesCtr;       ///< cache for number of incoming edges
    LoadMap          load;
    Nodetime         age;
    PPIPvXAddressSet nodesServiceLost; ///< nodes that are already out of service
    PPIPvXAddressSet roots;
    PPStringSet      stripes;
    bool             calculated;
    int              minRequiredStripes;
    int              numNodes;
    int              numStripes;

    IPSetSmartPtr getGraphAdjacents(const std::string& stripe, const IPvXAddress& ip) const;

public:
    TopologyModel();
    ~TopologyModel();

    int         calculate(const IPvXAddress& vertex, std::string& stripe);      ///< calculates the successor number in one stripe for vertex
    int         calculateSource(const IPvXAddress& vertex, std::string& stripe);
    int         calculate(const IPvXAddress& vertex);                           ///< calculate the overall successor number for vertex
    virtual void calculate();                                                    ///< Resets centrality and triggers centrality calculation

    IPvXAddress getRoot()                         const;
    void        setRoot(const IPvXAddress& root);
    void        removeRoot(IPvXAddress root);

    int         countChildren(const IPvXAddress& vertex)                             const;
    int         countChildren(const IPvXAddress& vertex, const std::string& stripe)  const;
    int         countChildrenMaxLevel(const IPvXAddress& vertex, const int maxLevel) const;
    int         countSuccessors(const IPvXAddress& vertex);
    int         countSuccessors(const IPvXAddress& vertex, std::string stripe);
    int         countSuccessors(std::string stripe);
    int         countNotConnected();

    int         numInboundEdges(const IPvXAddress& vertex) const;

    void	 	addVertex(const IPvXAddress vertex);
    void        addVertex(const std::string& stripe, const IPvXAddress vertex);
    void        removeVertex(const IPvXAddress& vertex);
    virtual int removeVertexRecursive(const IPvXAddress& vertex);
    int         removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex);
    int         removeVertexRecursive(const std::string& stripe, const IPvXAddress &vertex, bool loss);
    bool        hasVertex(const IPvXAddress& vertex);

    void        removeInboundEdges(const IPvXAddress to);
    void        addEdge(const IPvXAddress from, const IPvXAddress to);
    void        addEdge(const std::string& stripe, const IPvXAddress from, const IPvXAddress to);
    void        removeEdge(const IPvXAddress& from, const IPvXAddress& to);
    void        removeEdge(const std::string& stripe, const IPvXAddress& from, const IPvXAddress& to);

    bool        hasEdge(const std::string& stripe, const IPvXAddress& from, const IPvXAddress& to) const;
    bool        empty()                                                                            const;

    inline int  numberSources()                             const {return roots.size();}
    IPvXAddress getRandomVertex()                           const;
    IPvXAddress getCentralVertex();
    int         numberVertexes()                            const;
    int         numberVertexes(const std::string& stripeId) const;
    int         numberSourceVertexes();
    int         getNumRootSuccessors();

    // Returns the child with the most successors
    IPvXAddress      getBestChild(const IPvXAddress& vertex)                           const;
    IPvXAddress      getBestChild(const IPvXAddress& vertex, std::string stripe);
    PPIPvXAddressSet getChildren(const IPvXAddress& vertex, const std::string& stripe) const;
    PPIPvXAddressSet getChildren(const IPvXAddress& vertex)                            const;
    PPIPvXAddressSet getParents(const IPvXAddress& vertex)                             const;
    inline PPStringSet      getStripes()                                                      const {return stripes;}
    inline int              getTotalNodeNumber()                                              const {return numNodes;}

    IPvXAddress getPredecessor(const IPvXAddress& ip, const std::string& stripe)     const;
    IPvXAddress getPredecessorInclSource(const IPvXAddress& ip, std::string& stripe) const;

    double      getAge(const IPvXAddress& ip) const;
    void        setAge(const IPvXAddress ip, double nodeAge);

    /**
     *
     *
     *
     */
    int getMaximumLevel(const IPvXAddress& address, std::string& stripe);

    /**
     *
     *
     *
     */
    const PPStringIntMap getMaximumTreeDepth();

    int         getNumSeenStripes() const;
    void        resetLoss();
    void        setNumRequiredStripes(int num);
    void        setNumStripes(int num);
    void        setStripes(PPStringSet stripeSet);

    // Head-related functions
    PPIPvXAddressSet     getHeads();
    double      getHeadDiversity();
    bool        isHead(const IPvXAddress vertex);
    double      getHeadNum()                                          const;
    IPvXAddress getHead(std::string stripe, const IPvXAddress& succ);
    bool        getNodeStability(const IPvXAddress& address);
    double      getSmallestHeadStability();

    void        insertTopology(TopologyModel modelToInsert);
    PPEdgeList  getEdges();
    PPEdgeList  getEdges(std::string stripe);
    PPIPvXAddressSet getVertexes();
};

#endif // TOPOLOGYMODEL_H
