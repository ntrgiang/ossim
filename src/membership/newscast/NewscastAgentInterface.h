/*
 * NewscastAgentInterface.h
 *
 *      Author: Thorsten Jacobi
 */

#ifndef NEWSCASTAGENTINTERFACE_H_
#define NEWSCASTAGENTINTERFACE_H_

#include "NewscastCache.h"
#include "IPvXAddress.h"

class NewscastAgentInterface {
public:
    NewscastAgentInterface();
    virtual ~NewscastAgentInterface();

    /*
     * should return an unique identifier for the agent
     * if this is NULL this agent wont be added to the cache
     */
    virtual std::string getAgentName();
    /*
     * should return the Data to be added to the local cache
     */
    virtual GossipUserData*    getOwnData();

    /*
     * this function gets called whenever a cache from another peer has been received
     */
    virtual void onCacheReceived(IPvXAddress from, NewscastCache* cache);
};

#endif /* NEWSCASTAGENTINTERFACE_H_ */
