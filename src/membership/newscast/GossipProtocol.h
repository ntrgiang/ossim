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
// GossipProtocol.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief Interface class for gossip protocols
// @ingroup membership
// @ingroup newscast

#ifndef GOSSIPPROTOCOL_H_
#define GOSSIPPROTOCOL_H_

#include "IPvXAddress.h"
#include "MembershipBase.h"

//class GossipProtocol {
class GossipProtocol : public MembershipBase
{
public:
    GossipProtocol();
    virtual ~GossipProtocol();

    //virtual bool joinNetwork(IPvXAddress bootstrap = "0.0.0.0") = 0;
    //virtual void leaveNetwork();

    //virtual IPvXAddress getRandomPeer();
    virtual IPvXAddress getRandomPeer(IPvXAddress notThisAddress);
    virtual std::vector<IPvXAddress> getKnownPeers();

    // -- Old interface
    virtual void addSourceAddress(const IPvXAddress &address, int maxNOP=0) = 0;
    virtual void addPeerAddress(const IPvXAddress &address, int maxNOP=0) = 0;
    virtual bool deletePeerAddress(const IPvXAddress &address) = 0;

    virtual void incrementNPartner(const IPvXAddress &addr) = 0;
    virtual void decrementNPartner(const IPvXAddress &addr) = 0;

};

#endif /* GOSSIPPROTOCOL_H_ */
