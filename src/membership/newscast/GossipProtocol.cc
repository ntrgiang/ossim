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

// @author Thorsten Jacobi
// @brief Interface class for gossip protocols

#include "GossipProtocol.h"

GossipProtocol ::GossipProtocol(){

}
GossipProtocol::~GossipProtocol(){

}

bool GossipProtocol::joinNetwork(IPvXAddress bootstrap){
    return false;
}
void GossipProtocol::leaveNetwork(){

}

IPvXAddress GossipProtocol::getRandomPeer(){
    return "0.0.0.0";
}
IPvXAddress GossipProtocol::getRandomPeer(IPvXAddress notThisAddress){
    return "0.0.0.0";
}

std::vector<IPvXAddress> GossipProtocol::getKnownPeers(){
    std::vector<IPvXAddress> nul;
    return nul;
}
