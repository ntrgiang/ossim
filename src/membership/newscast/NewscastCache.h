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
// NewscastCache.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief Implementation of a cache to store the entries of a peer
// @ingroup membership
// @ingroup newscast

#ifndef NEWSCASTCACHE_H_
#define NEWSCASTCACHE_H_

#include "NewscastCacheEntry.h"
#include "cownedobject.h"

class NewscastCache : public ::cOwnedObject
{
public:
    // @brief intiates a cache with a fixed size
    NewscastCache(int size);
    NewscastCache(){m_maxEntries = 5;}
    virtual ~NewscastCache();

    // @brief adds or updates a entry for the agent
    void setEntry(std::string agent, IPvXAddress addr, simtime_t timestamp, GossipUserData* value);
    // @brief merges an other cache into this one. Only the newest entries will be stored
    void merge(NewscastCache* cache);
    // @brief returns the number of entries in the cache
    int getSize(){ return currentCache.size(); }
    // @brief returns a entry at a specified position
    NewscastCacheEntry getEntry(int index){ return currentCache.at(index);}
    // @brief returns a entry for a specified address
    NewscastCacheEntry getEntry(IPvXAddress addr);
    // @brief returns a random entry
    NewscastCacheEntry getRandomEntry();
    // @brief returns a list of IPvXAddress containing all entries
    std::vector<IPvXAddress> getAllAddresses();

    // @brief sets a new maximum size of the cache. If there are to many entries the oldest entries are getting dropped
    void setMaxSize(int size);

    // @brief generates a duplicate of this cache
    NewscastCache dup();

    // @brief displays the content of this cache via EV
    void printCache();

    // @brief returns an estimation of the size of this cache in bits
    long getEstimatedSizeInBits();
protected:
    typedef std::vector<NewscastCacheEntry> CacheSet;
    mutable CacheSet currentCache;
//CacheSet currentCache;

    // @brief removes the oldest entry
    void removeOldestEntry();

private:
    unsigned int m_maxEntries;
};

#endif /* NEWSCASTCACHE_H_ */
