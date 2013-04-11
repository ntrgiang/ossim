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
// DummyMembership.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "DummyMembership.h"

Define_Module(DummyMembership)

map<IPvXAddress, ActivePeerItem> DummyMembership::m_activePeerList;
vector<IPvXAddress> DummyMembership::m_tempList;

DummyMembership::DummyMembership() {
    // TODO Auto-generated constructor stub

}

DummyMembership::~DummyMembership() {
    // TODO Auto-generated destructor stub
}

void DummyMembership::initialize(int stage)
{
   if (stage != 3)
      return;
}

//IPvXAddress DummyMembership::getARandPeer()
//{
//   return m_apTable->getARandPeer();
//}

IPvXAddress DummyMembership::getRandomPeer(IPvXAddress address)
{
   Enter_Method("getARandPeer()");

   IPvXAddress ret_address;

   if (m_activePeerList.size() == 1)
   {
      ret_address = m_activePeerList.begin()->first;
      return ret_address;
   }

   // -- Clear ALL content of the the tempList
   m_tempList.clear();

   for (map<IPvXAddress, ActivePeerItem>::iterator iter = m_activePeerList.begin();
        iter != m_activePeerList.end(); ++iter)
   {
      if (iter->first == address)
         continue;

      for (int i = iter->second.m_current_nPartner; i < iter->second.m_maxNOP; ++i)
      {
         m_tempList.push_back(iter->first);
         EV << "Address: " << iter->first << endl;
      }
   }

   int size = m_tempList.size();

   if (size <= 0)
   {
      //throw cException("Wrong size of the tempList %d", size);

      // Hacking !!! (return the address of the source
      return m_activePeerList.begin()->first;
   }

   int aRandomIndex = (int)intrand(size);

   return m_tempList[aRandomIndex];
}

void DummyMembership::addPeerAddress(const IPvXAddress &address, int maxNOP)
{
   ActivePeerItem item;
      item.m_maxNOP = maxNOP;
      item.m_current_nPartner = 1; // peer can only be "in" if it has one partner
      item.m_joinTime = simTime().dbl();

   m_activePeerList.insert(pair<IPvXAddress, ActivePeerItem>(address, item));
}

void DummyMembership::addSourceAddress(const IPvXAddress &address, int maxNOP)
{
   Enter_Method("addSourceAddress()");

   EV << "Source address: " << address << " -- maxNOP: " << maxNOP << endl;

   ActivePeerItem item;
      item.m_maxNOP = maxNOP;
      item.m_current_nPartner = 0;
      item.m_joinTime = simTime().dbl();

   m_activePeerList.insert(pair<IPvXAddress, ActivePeerItem>(address, item));
}

bool DummyMembership::deletePeerAddress(const IPvXAddress &address)
{
   Enter_Method("deletePeerAddress()");
   m_activePeerList.erase(address);
   return false;
}

void DummyMembership::incrementNPartner(const IPvXAddress &address)
{
   //m_apTable->incrementNPartner(address);
}

void DummyMembership::decrementNPartner(const IPvXAddress &address)
{
   //m_apTable->decrementNPartner(address);
}

int DummyMembership::getActivePeerNumber(void)
{
   return m_activePeerList.size();
//   return -9;
}
