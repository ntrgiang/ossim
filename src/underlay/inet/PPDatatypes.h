/**
  * \author skehr
  * This header defines global datatypes
  *
  *  \addtogroup datatypes Global Datatypes
  * @{
  */
#ifndef PPDATATYPES_H
#define PPDATATYPES_H

#include <list>
#include "IPvXAddress.h"

//#include "StreamMessage_m.h"
//using forward class definitions here to avoid inclusion of headers and
//unnecessary dependencies in other files
//#include "PPEdge.h"

class StreamMessage;
class ResourceDescriptor;
class NeighborInfo;

typedef std::list<int>                          PPIntList;
typedef std::list<IPvXAddress>                  PPIPvXAddressList;
//typedef std::list<PPEdge>                       PPEdgeList;
typedef std::set<int>                           PPIntSet;
typedef std::set<IPvXAddress>                   PPIPvXAddressSet;
typedef std::set<NeighborInfo>                  PPNeighborInfoSet;
typedef std::set<std::string>                   PPStringSet;
typedef std::set<unsigned int>                  PPUIntSet;
typedef std::map<int, StreamMessage*>           PPMessageBuffer;
typedef std::map<int, int>                      PPIntIntMap;
typedef std::map<int, IPvXAddress>              PPIntIPvXAddressMap;
typedef std::map<IPvXAddress, cModule*>         PPIPvXAddressCModuleMap;
typedef std::map<IPvXAddress, simtime_t>        PPIPvXAddressSimtimeMap;
typedef std::map<IPvXAddress, double>           PPIPvXAddressDoubleMap;
typedef std::map<IPvXAddress, int>              PPIPvXAddressIntMap;
typedef std::map<IPvXAddress, unsigned long>    PPIPvXAddressULongMap;
typedef std::map<std::string, simtime_t>        PPStringSimtimeMap;
typedef std::map<std::string, int>              PPStringIntMap;
typedef std::multimap<int, int>                 PPIntIntMultimap;
typedef std::pair<IPvXAddress, int>             PPIPvXAddressIntPair;
typedef std::pair<IPvXAddress, std::string>     PPIPvXAddressStringPair;
typedef std::pair<IPvXAddress, IPvXAddress>     PPIPvXAddressPair;
typedef std::pair<int, int>                     PPIntIntPair;
typedef std::pair<ResourceDescriptor,bool>      PPResourceDescriptorBoolPair;
typedef std::vector<double>                     PPDoubleVector;
typedef std::vector<int>                        PPIntVector;
typedef std::vector<IPvXAddress>                PPIPvXAddressVector;
typedef std::vector<std::string>                PPStringVector;

typedef std::map<PPIPvXAddressPair, simtime_t>             PPIPvXAddressPairSimtimeMap;
typedef std::map<std::string,PPResourceDescriptorBoolPair> PPStringResDescriptorPairMap;
typedef PPStringResDescriptorPairMap::iterator             PPStringResMapIterator;


/**
  * module specific typedefs
  */
typedef PPUIntSet                               PPBufferMap;
#endif // PPDATATYPES_H

/** @} */
