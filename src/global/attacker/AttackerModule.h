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
// AttackerModule.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#ifndef ATTACKER_H
#define ATTACKER_H

#include <string>
#include <omnetpp.h>
#include "OverlayTopology.h"

//class OverlayTopology;
//class TopologyModel;

class AttackerModule : public cSimpleModule
{
public:
   virtual ~AttackerModule();

protected:
   // *************** general purpose ***************
   void initStatistics();
   void updateDisplay();

public:

   // *************** OMNETPP+INET *****************
   void initialize(int stage);
   virtual int numInitStages() const {	return 5; }
   void finish();
   void handleMessage(cMessage* msg);

   // **************** topology change *************
   TopologyModel getTopo(const int sequence);

   // ****** Attacker ****
   void attackGlobal();
   void nodeShutdown();

   int attackRecursive(const int sequence, const int num);

protected:
   bool m_debug;
   bool m_useEV;

   cMessage* timer_attack;
   cMessage* timer_reviewTopo;

   // ********** general configuration ***********
   // double rejoin;
   std::string damage;
   std::string estimate;

   // ********** global attacker ****************
   bool m_active;
   double startAttack, stopAttack, interval;
   //double percentage;
   OverlayTopology* oT;
   int numAttack;
   int param_numPeers;

   // ************** statistics ****************
   std::string statname_damage_global;
   cStdDev attackerDamage;
   cOutVector attackerDamageTime;
   cOutVector nodesOverallVec;

   simsignal_t sig_damage;
   simsignal_t sig_damage_ratio;

};

#endif // ATTACKER_H
