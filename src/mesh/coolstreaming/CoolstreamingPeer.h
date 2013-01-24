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

#include "CoolstreamingBase.h"

#ifndef COOLSTREAMINGPEER_H_
#define COOLSTREAMINGPEER_H_

class CoolstreamingPeer : public CoolstreamingBase{
public:
    CoolstreamingPeer();
    virtual ~CoolstreamingPeer();

protected:
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);

    virtual void handleTimerMessage(cMessage *msg);

    void checkPartners();

    // parent managment ->
private:
    unsigned int param_minNOP;

    int param_coolstreaming_Ts;
    int param_coolstreaming_Tp;
    double param_coolstreaming_Ta;

    cMessage* timer_CheckParents;

    void checkParents();
    bool satisfiesInequalitys(CoolstreamingPartner* partner, int substream);
    // <- parent managment
};

#endif /* COOLSTREAMINGPEER_H_ */
