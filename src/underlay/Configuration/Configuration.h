#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//#include "Coordinate.h"
#include "PPDatatypes.h"

#include <string>

class Stream;
class ResourceDescriptor;

class CoordinateSpace {
public:
   int dimensions;
   double min, max;
   std::string type;
};

class Configuration: public cSimpleModule
{
protected:
	CoordinateSpace coordinateSpace;

        PPIPvXAddressVector * alleHosts;
	int hosts;
	int testHosts;
	int testRouters;
	uint32 testAddress;
	uint32 networkAddress;
	std::string streamName;
	int streamStripes;
	int streamPacketSize;
	double streamPacketFrequency;
//	Coordinate streamStorePosition;
	IPvXAddress rendezvousAddress;
	int maxClients, clients;
	int maxUnstableClients, unstableClients;
	double unstableFraction;
	int streamID;
	IPvXAddress coordinator;
public:

    virtual int numInitStages() const {return 6;}
	void initialize(int stage);

	int groupSize();
	int simulationRun();
//	Stream* stream();
	int getStreamStripes();
	int getStreamPacketSize();
	double getStreamPacketFrequency();
//	ResourceDescriptor resource();
//	const CoordinateSpace& space() const;
	const IPvXAddress& rendezvous();
	bool join();

	// Depricated
	bool unstable(bool leave);

	bool canDrop();
	void doDrop();
	void rejoin();

	//		HostList* allHosts();
	int getHosts();
	int getRouters();
	uint32 getNetworkAddress();
	IPvXAddress getCoordinator();
public:
	bool addFatherNode(IPvXAddress ip, int stripe);
	void delFatherNode(IPvXAddress ip, int stripe);

	// Data structures needed by attackers attempting to identify the source
	typedef std::set<int> CountInStripe;
	typedef std::map<IPvXAddress, CountInStripe> KnownFathers;
	KnownFathers fathers;

};

#include "INetModuleAccess.h"

/**
 * Gives access to Configuration.
 */
class ConfigurationAccess: public INetModuleAccess<Configuration> {

public:
	ConfigurationAccess() :
		INetModuleAccess<Configuration> ("configuration") {
	}
};

#endif /*CONFIGURATION_H_*/
