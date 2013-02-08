/*
 * NewscastAgentInterface.h
 *
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

class NewscastAgentInterface {
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
