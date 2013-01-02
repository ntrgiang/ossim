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

#ifndef GOSSIPPROTOCOL_H_
#define GOSSIPPROTOCOL_H_

#include "IPvXAddress.h"

class GossipProtocol {
public:
    GossipProtocol();
    virtual ~GossipProtocol();

    virtual bool joinNetwork(IPvXAddress bootstrap = "0.0.0.0");
    virtual void leaveNetwork();

    virtual IPvXAddress getRandomPeer();
    virtual IPvXAddress getRandomPeer(IPvXAddress notThisAddress);
    virtual std::vector<IPvXAddress> getKnownPeers();
};

#endif /* GOSSIPPROTOCOL_H_ */
