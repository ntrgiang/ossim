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

#ifndef GOSSIPPROTOCOLWITHUSERDATA_H_
#define GOSSIPPROTOCOLWITHUSERDATA_H_

#include "GossipProtocol.h"
#include "GossipUserData.h"

class GossipProtocolWithUserData : public GossipProtocol {
public:
    GossipProtocolWithUserData();
    virtual ~GossipProtocolWithUserData();


    /*
     * sets the data which should be send to other peers
     */
    virtual void setOwnData(GossipUserData* data);

    /*
     * should return the known data of the peer
     * or NULL if unknown
     */
    virtual GossipUserData* getPeerData(IPvXAddress addr);
};

#endif /* GOSSIPPROTOCOLWITHUSERDATA_H_ */
