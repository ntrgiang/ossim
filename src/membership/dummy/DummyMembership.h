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

#ifndef DUMMYMEMBERSHIP_H_
#define DUMMYMEMBERSHIP_H_

#include "MembershipBase.h"

class DummyMembership : public MembershipBase {
public:
    DummyMembership();
    virtual ~DummyMembership();

    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);

public:
    //IPvXAddress getARandPeer();
    IPvXAddress getARandPeer(IPvXAddress address);

    void addPeerAddress(const IPvXAddress &address, int maxNOP=0);
    void addSourceAddress(const IPvXAddress &address, int maxNOP=0);

    bool deletePeerAddress(const IPvXAddress &address);

    void incrementNPartner(const IPvXAddress &addr);
    void decrementNPartner(const IPvXAddress &addr);
};

#endif /* DUMMYMEMBERSHIP_H_ */
