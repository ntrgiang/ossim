#include "CooperativeModule.h"
//#include "NodeTable.h"
//#include "AbstractCollector.h"
//#include "CSVCollector.h"
//#include "Bootstrap.h"
//#include <boost/lexical_cast.hpp>
//#include "HierarchyOrder.h"
//#include "BootMessage_m.h"
//#include "ShutdownMessage_m.h"
//#include "Coordinate.h"
//#include "NodeDescriptor.h"

using namespace std;
//using boost::lexical_cast;

void CooperativeModule::initialize() {

//	logCollector = NULL;
//	nodeTableCache = NULL;
	hostCache = NULL;
	applicationCache = NULL;
	subscribeMessages();

}

void CooperativeModule::finish() {
	statistics.clear();
	events.clear();
}

//NodeTable* CooperativeModule::nodeTable() const {

//	if (nodeTableCache == NULL)
//		nodeTableCache = NodeTableAccess().get();

//	return nodeTableCache;

//}

void CooperativeModule::clearCooperativeModuleCache() {

	// this has to be called, if the nodeTable, application moved from
	// one host to another
	// e.g. an application moves from rt[2] to rt[3], it has a new host
	// so the hostCachePointer has to be updated
//	logCollector = NULL;
//	nodeTableCache = NULL;
	applicationCache = NULL;
	hostCache = NULL;

}

cModule* CooperativeModule::parent(const char* name) const {

	cModule* module = const_cast<CooperativeModule*> (this); // we promise not to change this here

	while ((module != NULL) && (!module->isName(name)))
		module = module->getParentModule();

	return module;

}

cModule* CooperativeModule::network() const {
	cModule* module = const_cast<CooperativeModule*> (this); // we promise not to change this here

	while ((module != NULL) && !module->getModuleType()->isNetwork())
		module = module->getParentModule();

	return module;
}

cModule* CooperativeModule::host() const {

	if (hostCache == NULL) {

		hostCache = parent("cli");
		if (hostCache == NULL)
			hostCache = parent("srv");
		if (hostCache == NULL)
			hostCache = parent("rt");
	}

	return hostCache;

}

cModule* CooperativeModule::application() const {

	if (applicationCache == NULL)
		applicationCache = parent("udpApp");

	return applicationCache;

}

cModule* CooperativeModule::getModule(const char* name) const {

	return application()->getModuleByRelativePath(name);

}

Configuration* CooperativeModule::getConfig() const {

	return check_and_cast<Configuration*> (network()->getSubmodule("configuration"));

}

//Bootstrap* CooperativeModule::bootstrap() const {

//	return check_and_cast<Bootstrap*> (network()->getSubmodule("bootstrap"));

//}

//AbstractCollector* CooperativeModule::collector(std::string& name) {

//	cModule *collector = collectors[name];

//	if (collector == NULL) {

//		cModule* stats = network()->getSubmodule("statistics");
//		cModule* loc = stats->getSubmodule("localisation");
//		const char* nameChar = name.c_str();
//		if (collector == NULL) collector = stats->getSubmodule("coordinates")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = loc->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = loc->getSubmodule("localisation_pathLength")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = stats->getSubmodule("signaling")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = stats->getSubmodule("streaming")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = stats->getSubmodule("attacker_stat")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = stats->getSubmodule("ars")->getModuleByRelativePath(nameChar);
//		if (collector == NULL) collector = stats->getSubmodule("mobsStatistics")->getModuleByRelativePath(nameChar);
//        if (collector == NULL) collector = stats->getSubmodule("gossipStatistics")->getModuleByRelativePath(nameChar);
//		if (collector == NULL)
//			throw cRuntimeError(std::string("Collector not found: "	+ name).c_str());

//		collectors[name] = check_and_cast<AbstractCollector*> (collector);
//		assert(collectors[name] != NULL);
//	}

//	return collectors[name];

//}

//AbstractCollector* CooperativeModule::collector(const char* name) {
//    cModule *collector = collectors[name];
//    if (collector == NULL) {
//        cModule* stats = network()->getSubmodule("statistics");
//        cModule* loc = stats->getSubmodule("localisation");
//        if (collector == NULL) collector = stats->getSubmodule("coordinates")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = loc->getModuleByRelativePath(name);
//        if (collector == NULL) collector = loc->getSubmodule("localisation_pathLength")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("signaling")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("streaming")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("attacker_stat")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("ars")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("mobsStatistics")->getModuleByRelativePath(name);
//        if (collector == NULL) collector = stats->getSubmodule("gossipStatistics")->getModuleByRelativePath(name);
//        if (collector == NULL)
//            throw cRuntimeError(std::string("Collector not found: "	+ std::string(name)).c_str());

//        collectors[name] = check_and_cast<AbstractCollector*> (collector);
//        assert(collectors[name] != NULL);
//    }
//    return collectors[name];
//}

//CSVCollector *CooperativeModule::log() {

//	if (logCollector == NULL)
//		logCollector = check_and_cast<CSVCollector*> (network()->getModuleByRelativePath("statistics.log"));

//	if (logCollector->isLineEmpty()) {

//		logCollector->add(simTime().dbl());
//		int i = 3;
//		cModule* parent = this;
//		do {
//			parent = parent->getParentModule();
//			i--;
//		} while ((parent != NULL) && (i > 0));

//		if (parent != NULL) {
//			logCollector->add(parent->getFullName());
//			logCollector->add(nodeTable()->selfDescription());
//			logCollector->add(getParentModule()->getName());
//		}
//		logCollector->add(getName());
//	}

//	return logCollector;

//}

void CooperativeModule::start(std::string& name) {
	resetData(name);
//	collector(name)->registerModule(this);
}

void CooperativeModule::start(const char* name) {
	resetData(name);
//	collector(name)->registerModule(this);
}

void CooperativeModule::stop(const char* name) {
//	collector(name)->unregisterModule(this);
}

void CooperativeModule::stop(std::string& name) {
//	collector(name)->unregisterModule(this);
}

void CooperativeModule::stop() {

	Statistics::iterator it = statistics.begin();
	while (it != statistics.end()) {
		stop(it->first.c_str());
		it++;
	}
}

void CooperativeModule::event(std::string& name) {
	// Mark as event (and remember the first time)
	if (events.find(name) == events.end())
		events[name] = simTime();

	// Collect the event
	collect(name, 1);
}

void CooperativeModule::event(const char* name) {
	// Mark as event (and remember the first time)
	if (events.find(name) == events.end())
		events[name] = simTime();

	// Collect the event
	collect(name, 1);
}

void CooperativeModule::collect(std::string& name, double value) {
	statistics[name].collect(value);
}

void CooperativeModule::collect(const char* name, double value) {
	statistics[name].collect(value);
}

void CooperativeModule::collectAbsolute(std::string& name, double value) {
	resetData(name);
	collect(name, value);
}

void CooperativeModule::collectAbsolute(const char* name, double value) {
	resetData(name);
	collect(name, value);
}

cStdDev CooperativeModule::getData(std::string& name) {
	return statistics[name];
}

cStdDev CooperativeModule::getData(const char* name) {
	return statistics[name];
}

void CooperativeModule::resetData(const char* name) {
	statistics[name].clearResult();
}

void CooperativeModule::resetData(std::string& name) {
	statistics[name].clearResult();
}


std::string CooperativeModule::debugTime() const {
    char buffer[15];
    sprintf(buffer, "%12f", simTime().dbl());
    return std::string(buffer);
}


std::string CooperativeModule::debugAddress(IPvXAddress address) const {
    char buffer[15];
    sprintf(buffer, "%-12s", address.str().c_str());
    return std::string(buffer);
}

IPvXAddress CooperativeModule::selfAddress() const {
//    return nodeTable()->selfDescription().getAddress();
}

//Coordinate CooperativeModule::selfPosition() const {
//    return nodeTable()->selfDescription().getPosition();
//}
