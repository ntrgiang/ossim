#ifndef ATTACKER_H
#define ATTACKER_H

#include <string>
#include <omnetpp.h>

class OverlayTopology;
class TopologyModel;

class AttackerModule : public cSimpleModule {

protected:

	// ********** general configuration ***********
	double rejoin;
	std::string damage;
	std::string estimate;

	// ********** global attacker ****************
	double startAttack, stopAttack, interval, percentage;
    OverlayTopology* oT;
    int numAttack;

	// ************** statistics ****************
	std::string statname_damage_global;
	cStdDev attackerDamage;
    cOutVector attackerDamageTime;
	cOutVector nodesOverallVec;

protected:

	// *************** general purpose ***************
	void initStatistics();
	void updateDisplay();

public:

	// *************** OMNETPP+INET *****************
	void initialize(int stage);
	virtual int numInitStages() const {
		return 5;
	}
	void finish();
	void handleMessage(cMessage* msg);

	// **************** topology change *************
    TopologyModel getTopo(const int sequence);

    // ****** Attacker ****
    void attackGlobal();
    int attackRecursive(const int sequence, const int num);

};

#endif
