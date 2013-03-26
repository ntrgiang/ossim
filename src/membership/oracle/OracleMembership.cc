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
// OracleMembership.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//


#include "OracleMembership.h"

Define_Module(OracleMembership);

OracleMembership::OracleMembership() {
    // TODO Auto-generated constructor stub

}

OracleMembership::~OracleMembership() {
    // TODO Auto-generated destructor stub
}

void OracleMembership::initialize(int stage)
{
   if (stage != 3)
      return;

   bindToGlobalModule();
}

//IPvXAddress OracleMembership::getARandPeer()
//{
//   return m_apTable->getARandPeer();
//}

IPvXAddress OracleMembership::getARandPeer(IPvXAddress address)
{
   return m_apTable->getARandPeer(address);
}

void OracleMembership::addPeerAddress(const IPvXAddress &address, int maxNOP)
{
   m_apTable->addPeerAddress(address, maxNOP);
}

void OracleMembership::addSourceAddress(const IPvXAddress &address, int maxNOP)
{
   m_apTable->addSourceAddress(address, maxNOP);
}

bool OracleMembership::deletePeerAddress(const IPvXAddress &address)
{
   return m_apTable->deletePeerAddress(address);
}

void OracleMembership::incrementNPartner(const IPvXAddress &address)
{
   m_apTable->incrementNPartner(address);
}

void OracleMembership::decrementNPartner(const IPvXAddress &address)
{
   m_apTable->decrementNPartner(address);
}
