#ifndef COOPERATIVEMODULE_H_
#define COOPERATIVEMODULE_H_

#include "Configuration.h"

//class NodeTable;
//class Bootstrap;
//class AbstractCollector;
//class CSVCollector;
//class EventCollector;
//class ShutdownMessage;
//class BootMessage;

#define DATA  0
#define EVENT 1
#define RATIO 2

class CooperativeModule: public cSimpleModule {

	friend class AbstractCollector;
	friend class AverageCollector;
	friend class LastCollector;
	friend class TotalCollector;
	friend class FlowCollector;
	friend class MeanCollector;

protected:
	typedef std::map<std::string, cStdDev> Statistics;
//        typedef PPStringSimtimeMap Events;
	Statistics statistics;
        PPStringSimtimeMap events;

//	typedef std::map<std::string, AbstractCollector*> AbCollectors;
//	AbCollectors collectors;

//	CSVCollector *logCollector;

	cModule* parent(const char* name) const;
	cModule* network() const;
	cModule* host() const;
	cModule* application() const;
	cModule* getModule(const char* name) const;
	Configuration* getConfig() const;
//	Bootstrap* bootstrap() const;

//	AbstractCollector* collector(std::string& name);
//	AbstractCollector* collector(const char* name);
	cStdDev getData(const char* name);
	cStdDev getData(std::string& name);
	void resetData(const char* name);
	void resetData(std::string& name);

	// nodeTableCache, hostCache, applicationCache has
	// been shown to gain approx. 4% runtime
//	mutable NodeTable* nodeTableCache;
   mutable cModule* hostCache;
	mutable cModule* applicationCache;

protected:
        virtual void subscribeMessages() {}
    IPvXAddress selfAddress()  const;
//    Coordinate  selfPosition() const;
public:
	void initialize();
	void finish();

//	NodeTable* nodeTable() const;
	void clearCooperativeModuleCache();

	void start(const char* name);
	void start(std::string& name);
	void stop(std::string& name);
	void stop(const char* name);
	void stop();

//	CSVCollector* log();
	void event(std::string& name);
	void event(const char* name);
	void collect(const char* name, double value);
	void collect(std::string& name, double value);
	void collectAbsolute(const char* name, double value);
	void collectAbsolute(std::string& name, double value);
    std::string debugTime() const;                          ///< returns present simtime with fixed length as string
    std::string debugAddress(IPvXAddress) const;            ///< returns address with a fixed length, by adding whitespaces
};

#endif /*COOPERATIVEMODULE_H_*/
