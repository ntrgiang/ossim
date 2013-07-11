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
// DummyMembership.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef DUMMYMEMBERSHIP_H_
#define DUMMYMEMBERSHIP_H_

#include "MembershipBase.h"

struct ActivePeerItem
{
   int m_maxNOP;
   int m_current_nPartner;
   double m_joinTime;
};

class DummyMembership : public MembershipBase {
public:
   DummyMembership();
   virtual ~DummyMembership();

   virtual int numInitStages() const  {return 4;}
   virtual void initialize(int stage);

public:
   //IPvXAddress getARandPeer();
   virtual IPvXAddress getRandomPeer(IPvXAddress address);

   void addPeerAddress(const IPvXAddress &address, int maxNOP=0);
   void addSourceAddress(const IPvXAddress &address, int maxNOP=0);

   bool deletePeerAddress(const IPvXAddress &address);

   void incrementNPartner(const IPvXAddress &addr);
   void decrementNPartner(const IPvXAddress &addr);

   int getActivePeerNumber(void);

private:
   bool m_debug;
   static map<IPvXAddress, ActivePeerItem> m_activePeerList;
   static vector<IPvXAddress> m_tempList;

};

#endif /* DUMMYMEMBERSHIP_H_ */
