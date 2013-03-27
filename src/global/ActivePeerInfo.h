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
// ActivePeerInfo.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef ACTIVE_PEER_INFO_H_
#define ACTIVE_PEER_INFO_H_

#include <vector>
#include <IPvXAddress.h>
using namespace std;

struct Struct_ActivePeerInfo
{
//   int m_maxNOP;
//   int m_current_nPartner;
   double m_joinTime;
//   bool m_joined_membershipOverlay;
//   bool m_joined_meshOverlay;
//   bool m_joined_treeOverlay;

   //std::vector<IPvXAddress> m_partner_waiting_list;
};

/*
class ActivePeerInfo
{
public:
    ActivePeerInfo();
    ActivePeerInfo(int maxNOP, int current_nPartner, double joinTime);
    ~ActivePeerInfo();

public:
    // -- For debugging --
    void printStatus(void);
//    void incrementNPartner(void);
//    void decrementNPartner(void);

    //void setMaxNop(int maxNOP);
    int getMaxNop(void) { return m_maxNOP; }
    int getCurrentNumberOfPartner(void) { return m_current_nPartner; }
    double getJoinTime(void) { return m_joinTime; }

// Data member
private:
    int m_maxNOP;
    int m_current_nPartner;
    double m_joinTime;
    //vector<IPvXAddress> m_partnerList;
};
*/

#endif /* ACTIVE_PEER_INFO_H_ */
