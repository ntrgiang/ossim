#include "Configuration.h"
#include "IPAddressResolver.h"
//#include "CoordinateLoader.h"
//#include "p2pstreaming.h"
//#include "Stream.h"
//#include "ResourceDescriptor.h"

#include <assert.h>

Define_Module(Configuration);

void Configuration::initialize(int stage) {

#if _DEBUG
	if (C_DEBUG_Configuration) {
		std::cout << "Configuration init stage " << stage << endl;
	}
#endif

	if (stage == 0) {
#if _DEBUG
		if (C_DEBUG_Configuration) {
			//	WATCH(coordinateSpae);
			WATCH(streamName);
			WATCH(streamStripes);
			WATCH(streamPacketSize);
			WATCH(streamPacketFrequency);
			WATCH(streamStorePosition);
			WATCH(maxClients);
			WATCH(clients);
			WATCH(maxUnstableClients);
			WATCH(unstableClients);
		}
#endif

		int seed = par("randomSeed");
		if (seed > 0)
			srand(seed);
		else if (seed < 0)
			srand(time(NULL));

		coordinateSpace.dimensions = par("coordDimensions");
		coordinateSpace.min = par("coordMin");
		coordinateSpace.max = par("coordMax");
		coordinateSpace.type = par("coordType").stringValue();

		streamName = "CC437";
		streamStripes = par("streamStripes");
		streamPacketSize = par("streamPacketSize");
		streamPacketFrequency = par("streamPacketFrequency");

		maxClients = par("clients");
		unstableFraction = par("unstable");

		clients = 0;
		maxUnstableClients = 0;
		unstableClients = 0;

		streamID = 0;

	} else if (stage == 2) {
//		streamStorePosition = CoordinateLoaderAccess().get()->getRandom();
//		assert(streamStorePosition.getMetric());
	} else if(stage == 5){
		std::stringstream s;
		s << "srv[" << par("coordinator").longValue() << "]";
		coordinator = IPAddressResolver().resolve(s.str().c_str());

		assert(!coordinator.isUnspecified());
	}
}

int Configuration::groupSize() {
	Enter_Method_Silent();
	return par("clients");
}

int Configuration::simulationRun() {
	Enter_Method_Silent();
    //return simulation.runNumber();
	return 0;
	throw cRuntimeError("Configuration::simulationRun() missing");

}

int Configuration::getRouters() {
	Enter_Method_Silent();
	return (int) par("rts");
}

int Configuration::getHosts() {
	Enter_Method_Silent();
	int rts = par("rts");
	int n_r = par("hosts");
	return rts + n_r;
}

uint32 Configuration::getNetworkAddress() {
	Enter_Method_Silent();
	return IPAddress(par("networkAddress").stringValue()).getInt();
}

//Stream* Configuration::stream() {
//	Enter_Method_Silent();
//	return new Stream(streamStripes, streamPacketSize, streamPacketFrequency, streamID++);
//}

double Configuration::getStreamPacketFrequency() {
	return streamPacketFrequency;
}

int Configuration::getStreamStripes(){
	return streamStripes;
}

int Configuration::getStreamPacketSize() {
	return streamPacketSize;
}

//ResourceDescriptor Configuration::resource() {
//	Enter_Method_Silent();

//	ResourceDescriptor resource;
//	resource.setName(streamName.c_str());
//	assert(streamStorePosition.getMetric());
//	resource.setPosition(streamStorePosition);

//	return resource;
//}

//const CoordinateSpace& Configuration::space() const {
//	return coordinateSpace;
//}

const IPvXAddress& Configuration::rendezvous() {
	Enter_Method_Silent();

	if (rendezvousAddress.isUnspecified()) {
		rendezvousAddress = IPAddressResolver().resolve("srv[0]");
	}

	return rendezvousAddress;
}

bool Configuration::join() {
	Enter_Method_Silent();

	bool canJoin = (clients < maxClients);
	if (canJoin) {

		clients++;
		maxUnstableClients = (int) floor(unstableFraction * (double) clients);
	}

	return canJoin;
}

bool Configuration::unstable(bool leave) {
	Enter_Method_Silent();

	if (!leave) {

#if _DEBUG
		if (C_DEBUG_Configuration) {
			std::cout << "Rejoin: " << unstableClients << endl;
		}
#endif
		unstableClients--;
		return true;
	}

	bool canLeave = (unstableClients < maxUnstableClients);
	if (canLeave)
		unstableClients++;

#if _DEBUG
	if (C_DEBUG_Configuration) {
		std::cout << "Drop: " << unstableClients << endl;
	}
#endif
	return canLeave;
}

bool Configuration::canDrop() {
	Enter_Method_Silent();

	return (unstableClients < maxUnstableClients);
}

void Configuration::doDrop() {
	Enter_Method_Silent();

	if (!canDrop())
		throw cRuntimeError("Not allowed to drop");

#if _DEBUG
	if (C_DEBUG_Configuration) {
		std::cout << "Drop: " << unstableClients << endl;
	}
#endif
	unstableClients++;
}

void Configuration::rejoin() {
	Enter_Method_Silent();

#if _DEBUG
	if (C_DEBUG_Configuration) {
		std::cout << "Rejoin: " << unstableClients << endl;
	}
#endif
	unstableClients--;
}

bool Configuration::addFatherNode(IPvXAddress ip, int stripe) {
	Enter_Method_Silent();

	if (fathers.find(ip) != fathers.end()) {
		if (fathers[ip].find(stripe) == fathers[ip].end())
			return false;
		else {
			fathers[ip].insert(stripe);
			return true;
		}
	} else {
		std::set<int> tmp;
		tmp.insert(stripe);
		fathers[ip] = tmp;
		return true;
	}

}

void Configuration::delFatherNode(IPvXAddress ip, int stripe) {
	Enter_Method_Silent();

	if (fathers.find(ip) != fathers.end()) {
		if (fathers[ip].find(stripe) != fathers[ip].end()) {

			if (fathers[ip].size() == 1)
				fathers.erase(ip);
			else
				fathers[ip].erase(stripe);

		} else
			throw cRuntimeError(
					"Configuration::delFatherNode, empty entry 2");

	} else
		throw cRuntimeError("Configuration::delFatherNode, empty entry 1");
}

IPvXAddress Configuration::getCoordinator(){
	assert(!coordinator.isUnspecified());
	return coordinator;
}
