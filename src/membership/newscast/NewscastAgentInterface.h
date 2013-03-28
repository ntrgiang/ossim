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
// NewscastAgentInterface.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

/*
 * @author Thorsten Jacobi
 *
 * @brief Interface which can be used to add multiply Agents to one Newscast-Peer
 */

// @ingroup membership
// @ingroup newscast

#ifndef NEWSCASTAGENTINTERFACE_H_
#define NEWSCASTAGENTINTERFACE_H_

#include "NewscastCache.h"
#include "IPvXAddress.h"

class NewscastAgentInterface
{
public:
    NewscastAgentInterface();
    virtual ~NewscastAgentInterface();

    /*
     * @brief should return an unique identifier for the agent. If this is NULL this agent wont be added to the cache
     */
    virtual std::string getAgentName();
    /*
     * @brief should return the Data to be added to the local cache
     */
    virtual GossipUserData*    getOwnData();

    /*
     * @brief this function gets called whenever a cache from another peer has been received
     */
    virtual void onCacheReceived(IPvXAddress from, NewscastCache* cache);
};

#endif /* NEWSCASTAGENTINTERFACE_H_ */
