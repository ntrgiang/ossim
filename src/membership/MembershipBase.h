//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
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
    virtual IPvXAddress getARandPeer() = 0;
    virtual IPvXAddress getARandPeer(IPvXAddress address) = 0;

    virtual void addPeerAddress(const IPvXAddress &address, int maxNOP=0) = 0;
    virtual void addSourceAddress(const IPvXAddress &address, int maxNOP=0) = 0;

    virtual bool deletePeerAddress(const IPvXAddress &address) = 0;

    virtual void incrementNPartner(const IPvXAddress &addr) = 0;
    virtual void decrementNPartner(const IPvXAddress &addr) = 0;

//protected:
//    virtual void writeToMcache(void) = 0;
};

#endif /* MEMBERSHIPBASE_H_ */
