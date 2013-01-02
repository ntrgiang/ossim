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

#ifndef GOSSIPUSERDATA_H_
#define GOSSIPUSERDATA_H_

#include "cobject.h"

class GossipUserData : cObject {
public:
    GossipUserData();
    virtual ~GossipUserData();

    virtual GossipUserData* dup() const;

    virtual long getSizeInBits();
};

#endif /* GOSSIPUSERDATA_H_ */
