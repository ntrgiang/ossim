#include "AttackerModule.h"
#include "OverlayTopology.h"
#include "TopologyModel.h"

using namespace std;

Define_Module(AttackerModule);

/**
 * initialise the AttackerModule in Stage 3.
 */
void AttackerModule::initialize(int stage) {

    if (stage != 3) return;

    //FIXME get pointer to OverlayTopology-module
    oT = NULL;

    // the global attacker parameters
	startAttack = par("start");
	stopAttack = par("stop");
	assert(startAttack < stopAttack);
    interval = par("interval");

    // Percentage of nodes to attack
    percentage = par("percentage");
    numAttack = par("numAttack");

	// the statistical parameters
	statname_damage_global = "streaming_importance_global";

	// init statistics
	initStatistics();

	// schedule attacks+statistics
    scheduleAt(simTime() + startAttack, new cMessage("GLOBAL_SCHED_ID"));
	updateDisplay();

}

TopologyModel AttackerModule::getTopo(const int sequence) {

    return oT->getTopology(sequence);
}


void AttackerModule::finish() {

	// *** Statistics ***
	char buffer[128];
	// - write damage to sca
	sprintf(buffer, "%s - %s", "attacker.damage", "mean");
	recordScalar(buffer, attackerDamage.getMean());
	sprintf(buffer, "%s - %s", "attacker.damage", "min");
	recordScalar(buffer, attackerDamage.getMin());
	sprintf(buffer, "%s - %s", "attacker.damage", "max");
	recordScalar(buffer, attackerDamage.getMax());
}

void AttackerModule::handleMessage(cMessage* msg) {

	if (!msg->isSelfMessage()) {
		delete msg;
		msg = NULL;
		throw cRuntimeError("AttackerModule cannot handle non-loopback messages");

    }

    attackGlobal();

    if (simTime() + interval <= stopAttack) {
        scheduleAt(simTime() + interval, msg);
        msg = NULL;
        return;

    } else {
        delete msg;
        msg = NULL;
    }

    assert(msg == NULL);
}


void AttackerModule::attackGlobal() {

    // put stats here
    int damage = oT->attackRecursive(numAttack);
    //FIXME recording the damage
    attackerDamage.collect((double) damage);
    attackerDamageTime.record((double) damage);
}




/**
 * starts statistics
 */
void AttackerModule::initStatistics() {
	nodesOverallVec.setName("total_number_nodes");
}

/**
 * omnetpp standard method
 *
 * There are two kinds of messages to come in:
 *   - loopback: ATTACK_MSG_ID: triggers the global attacker
 *   - loopback: AGENT_MSG_ID: triggers the attack agent
 */
void AttackerModule::updateDisplay() {
#if _DEBUG
	char buffer[32];
        sprintf(buffer, "%d getNode(s)", (int)modules.size());

	cDisplayString display = getDisplayString();
	// to show the current amount of nodes
	display.setTagArg("t", 0, buffer);
	display.setTagArg("t", 1, "t");
	display.setTagArg("t", 2, "darkgray");
	// and also the online status
	setDisplayString(display.str());
#endif
}

