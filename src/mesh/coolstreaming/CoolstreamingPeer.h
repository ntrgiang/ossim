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
// CoolstreamingPeer.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief CoolstreamingPeer based on CoolstreamingBase
// @ingroup mesh
// @ingroup coolstreaming

#include "CoolstreamingBase.h"

#ifndef COOLSTREAMINGPEER_H_
#define COOLSTREAMINGPEER_H_

class CoolstreamingPeer : public CoolstreamingBase
{
public:
    CoolstreamingPeer();
    virtual ~CoolstreamingPeer();

protected:
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);

    virtual void handleTimerMessage(cMessage *msg);

    // @brief overrides the checkPartners function of the base-class to check if new partners are needed and to query them
    void checkPartners();

private:
//   void handleTimerJoin(void);
   void bindToGlobalModule();

    // parent managment ->
private:
    // @brief continue to query other peers for partnership until the number is reached
    unsigned int param_minNOP;

    // @brief to check if there are any partners receiving new chunks
    int* stalemateDetection;

    // @brief defines the maximum number of tolerated chunk delay
    int param_coolstreaming_Ts;
    // @brief defines the maximum number of chunks which a parent can be behind other partners
    int param_coolstreaming_Tp;
    // @brief interval to check parents
    double param_coolstreaming_Ta;

    cMessage* timer_CheckParents;

    // @brief checks if the current parents deliver the substreams fast enough
    void checkParents();
    // @brief checks if a partner would be good for a given substream
    bool satisfiesInequalitys(CoolstreamingPartner* partner, int substream);
    // @brief checks if a partner would be good for a given substream
    bool satisfiesInequalityTwo(CoolstreamingPartner* partner, int substream);
    // <- parent managment

private:
    cMessage *timer_getJoinTime;
    cMessage *timer_join;
};

#endif /* COOLSTREAMINGPEER_H_ */
