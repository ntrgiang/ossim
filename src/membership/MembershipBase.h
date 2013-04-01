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
// MembershipBase.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef MEMBERSHIPBASE_H_
#define MEMBERSHIPBASE_H_

#include "CommBase.h"

class MembershipBase : public CommBase {
public:
    MembershipBase();
    virtual ~MembershipBase();

    //virtual void initialize() = 0;

public:
    //virtual IPvXAddress getARandPeer() = 0;

    /**
     * @brief getARandPeer
     * @param address
     * @return a random address of existing active peers
     * This returned address should be different from the address in the parameter list
     */
    virtual IPvXAddress getRandomPeer(IPvXAddress address) = 0;

    /**
     * @brief addSourceAddress
     * @param address
     * @param maxNOP
     * This function is triggered by the source peer
     * This function should:
     * - Inform the Active Peer Table to add the peer's address
     * - Initialize the membership management overlay
     */
    virtual void addSourceAddress(const IPvXAddress &address, int maxNOP=0) = 0;

    /**
     * @brief addPeerAddress
     * @param address
     * @param maxNOP
     * This function is triggered when the new peer successfully joined the mesh overlay
     * This function should:
     * - Inform the Active Peer Table to add the peer's address
     * - Perform protocol-specific actions to update the membership management overlay
     */
    virtual void addPeerAddress(const IPvXAddress &address, int maxNOP=0) = 0;

    /**
     * @brief deletePeerAddress
     * @param address
     * @return
     *
     */
    virtual bool deletePeerAddress(const IPvXAddress &address) = 0;

    virtual void incrementNPartner(const IPvXAddress &addr) = 0;
    virtual void decrementNPartner(const IPvXAddress &addr) = 0;

    virtual int getActivePeerNumber(void) { return 0; }

//protected:
//    virtual void writeToMcache(void) = 0;
};

#endif /* MEMBERSHIPBASE_H_ */
