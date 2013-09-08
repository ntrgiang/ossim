//
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// AttackerModule.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "AttackerModule.h"
#include "OverlayTopology.h"
#include "TopologyModel.h"
#include "DonetPeer.h"
#include "InterfaceTable.h"
#include "IPv4InterfaceData.h"

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "::" << getFullName() << ": "
#endif

Define_Module(AttackerModule);

AttackerModule::~AttackerModule()
{
   if (timer_attack) cancelAndDelete(timer_attack);
   if (timer_reviewTopo) cancelAndDelete(timer_reviewTopo);
}

/**
 * initialise the AttackerModule in Stage 5
 */
void AttackerModule::initialize(int stage)
{
   if (stage == 0)
   {
      sig_damage = registerSignal("Signal_Damage");

      if (hasPar("debug"))
         m_debug = par("debug").boolValue();
      else
         m_debug = false;

   }

   if (stage != 4) return;

   m_active = par("active").boolValue();
   if (!m_active) return;

   // -- Binding to the global module Topology Observer
   cModule *temp = simulation.getModuleByPath("topoObserver");
   oT = check_and_cast<OverlayTopology*>(temp);

   timer_attack = new cMessage("ATTACKER_MODULE_TRIGGER");
   timer_reviewTopo = new cMessage("ATTACKER_REVIEW_TOPO");

   // the global attacker parameters
   startAttack = par("start").doubleValue();
   stopAttack = par("stop").doubleValue();
   assert(startAttack < stopAttack);
   interval = par("interval").doubleValue();

   // Percentage of nodes to attack
   //percentage = par("percentage").doubleValue(); // not used so far
   numAttack = par("numAttack").longValue();

   // the statistical parameters
   statname_damage_global = "streaming_importance_global";

   // init statistics
   initStatistics();

   // schedule attacks+statistics
   //scheduleAt(simTime() + startAttack, new cMessage("GLOBAL_SCHED_ID"));

   debugOUT << "schedule attack!!!" << " -- will start in " << startAttack << " seconds" << endl;
   scheduleAt(simTime() + startAttack, timer_attack);

   scheduleAt(par("simTimeLimit").doubleValue() - 30, timer_reviewTopo);

   //updateDisplay();

   WATCH(numAttack);

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
   Enter_Method("handleMessage");

   EV << "handle messages" << endl;

   if (!msg->isSelfMessage()) {
      delete msg;
      msg = NULL;
      throw cRuntimeError("AttackerModule cannot handle non-loopback messages");

   }

   if (msg == timer_attack)
   {
      attackGlobal();

      //nodeShutdown();

      //debugOUT << "topo size after shuting down nodes: " << oT->getNumTopologies() << endl;
   }
   else if (msg == timer_reviewTopo)
   {
      debugOUT << "topo size at reviewing: " << oT->getNumTopologies() << endl;

      oT->writeEdgesToFile("/tmp/edgeList_after.dat");
   }
   // Attacking once for the moment
//   if (simTime() + interval <= stopAttack) {
//      scheduleAt(simTime() + interval, timer_attack);
//      msg = NULL;
//      return;

//   }

// <--- should be deleted
//   else {
//      delete msg;
//      msg = NULL;
//   }

//   assert(msg == NULL);
}


void AttackerModule::attackGlobal()
{
   Enter_Method("attackGlobal()");
   debugOUT << "Attack Global! Calculating potential damage ..." << endl;

   // -- Attack on the Overlay Topology recursively with "numAttack" times
   //
   int damage = oT->attackRecursive2(numAttack);
   debugOUT << "damage of attackGlobal() = " << damage << endl;

   // -- Statistics
   //
   emit(sig_damage, damage);
   attackerDamage.collect((double) damage);
   attackerDamageTime.record((double) damage);
}

void AttackerModule::nodeShutdown()
{
   debugOUT << "Node shutdown!" << endl;

   int sequence = oT->getMaxRecentSeq();
   TopologyModel topoM = oT->getTopology(sequence);

   for (int i = 0; i < numAttack; ++i)
   {
      IPvXAddress vertex = topoM.getCentralVertex();

      // -- Has to figure out which node has this IP address
      int j = 0;
      while(getParentModule()->getSubmodule("peerNode",j))
      {
         cModule* module = getParentModule()->getSubmodule("peerNode",j);
         InterfaceTable* interface = check_and_cast<InterfaceTable*>(module->getSubmodule("interfaceTable"));
         EV << "Number of interfaces: " << interface->getNumInterfaces() << endl;

         IPvXAddress nodeAddr;
         if (interface->getNumInterfaces() > 1)
         {
            nodeAddr = IPvXAddress(interface->getInterface(interface->getNumInterfaces()-1)->ipv4Data()->getIPAddress());
         }
         else
         {
            nodeAddr = IPvXAddress(interface->getInterface(0)->ipv4Data()->getIPAddress());
         }

         if (vertex == nodeAddr)
         {
            DonetPeer* peer = check_and_cast<DonetPeer*>(module->getSubmodule("peerApp",0)->getSubmodule("mesh",0));
            assert(peer);

            debugOUT << "node to be shutdown gracefully: " << vertex << endl;
            peer->gracefulLeave();
         }

         j++;
      } // while

      // -- removed the identified node, to prepare for the next removal
      topoM.removeVertexRecursive(vertex);

   } // for

   // put stats here
//   int damage = oT->attackRecursive(numAttack);

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

