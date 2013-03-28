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
// NewscastCache.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief Implementation of a cache to store the entries of a peer

#include "NewscastCache.h"

NewscastCache::NewscastCache(int size) : cOwnedObject()
{
    m_maxEntries = size;
}

NewscastCache::~NewscastCache()
{
    EV << "~NewscastCache()" << endl;
    //while(!currentCache.empty()) delete currentCache.back(), currentCache.pop_back();
    /*NewscastCacheEntry* entry;
    while (currentCache.size() > 0){
        entry = currentCache.at(currentCache.size()-1);
        currentCache.pop_back();
        //if (entry != NULL) delete entry;
    }*/
    EV << "~NewscastCache() -> DONE" << endl;
    // -- Giang
//    std::vector<NewscastCacheEntry*>::iterator iter, old_iter;
//    old_iter = currentCache.begin();
//    for (iter=old_iter+1; iter != currentCache.end(); ++iter)
//    {
//       if (*old_iter) delete (*old_iter);
//       old_iter = iter;
//    }
//    std::vector<NewscastCacheEntry*>::iterator iter;
//    for (iter = currentCache.begin(); iter != currentCache.end(); ++iter)
//    {
//       if (*iter) delete *iter;
//    }
    EV << "~NewscastCacghe() -> DONE 2" << endl;
    currentCache.clear();
}

void NewscastCache::setMaxSize(int size)
{
    m_maxEntries = size;

    while ((currentCache.size() > m_maxEntries) && (currentCache.size() > 0)){  // while we have more than m_maxEntries ...
        removeOldestEntry();
    }
}


NewscastCache NewscastCache::dup()
{
    NewscastCache ret = NewscastCache(m_maxEntries);
    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        EV << "dup2: " << (*it).getAddress() << endl;
        ret.setEntry( (*it).getAgent(), (*it).getAddress(), (*it).getTimestamp(), (*it).getValue() );
    }

    return ret;
}

void NewscastCache::setEntry(std::string agent, IPvXAddress addr, simtime_t timestamp, GossipUserData* value)
{
    for (unsigned int i = 0; i < currentCache.size(); i++)
        if (currentCache.at(i).getAgent().compare(agent) == 0){
            currentCache.at(i).setAddress(addr);
            currentCache.at(i).setTimestamp(timestamp);
            currentCache.at(i).setValue(value);
            return;
        }

    NewscastCacheEntry entry;
        entry.setAgent(agent);
        entry.setAddress(addr);
        entry.setTimestamp(timestamp);
        entry.setValue(value);
    currentCache.push_back(entry);

}

void NewscastCache::merge(NewscastCache* cache)
{
    EV << "merge: " << cache->getSize() << endl;
    // insert the entries from the new cache into the current
    CacheSet::iterator it;
    for (it = cache->currentCache.begin(); it != cache->currentCache.end(); it++){
        setEntry( (*it).getAgent(), (*it).getAddress(), (*it).getTimestamp(), (*it).getValue() );
    }

    while ((currentCache.size() > m_maxEntries) && (currentCache.size() > 0)){  // while we have more than m_maxEntries ...
        removeOldestEntry();
    }
}

void NewscastCache::removeOldestEntry()
{
    int oldest = 0; simtime_t time = currentCache.at(0).getTimestamp();

    // find the oldest one
    for (unsigned int i = 1; i < currentCache.size(); i++)
        if (currentCache.at(i).getTimestamp() < time){
            time = currentCache.at(i).getTimestamp();
            oldest = i;
        }

    // set the pointer to the oldest one
    CacheSet::iterator it = currentCache.begin();
    it += oldest;

    currentCache.erase(it);
}

void NewscastCache::printCache()
{
    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        EV << (*it).getAgent() <<","<< (*it).getAddress() <<","<< (*it).getTimestamp() <<","<< (*it).getValue() << endl;
    }
}

NewscastCacheEntry NewscastCache::getRandomEntry()
{
    NewscastCacheEntry ret;

    if (currentCache.size() > 0){
        int aRandomIndex = (int)intrand(currentCache.size());
        ret = currentCache.at(aRandomIndex);
    }

    return ret;
}

NewscastCacheEntry NewscastCache::getEntry(IPvXAddress addr)
{
    NewscastCacheEntry ret;

    for (unsigned int i = 0; i < currentCache.size(); i++)
        if (currentCache.at(i).getAddress().equals(addr)){
            ret = currentCache.at(i);
            break;
        }

    return ret;
}

std::vector<IPvXAddress> NewscastCache::getAllAddresses()
{
    while ((currentCache.size() > m_maxEntries) && (currentCache.size() > 0)){  // while we have more than m_maxEntries ...
        removeOldestEntry();
    }

    std::vector<IPvXAddress> ret;

    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++)
        if (!(*it).getAddress().isUnspecified())
            ret.push_back( (*it).getAddress());

    return ret;
}


long NewscastCache::getEstimatedSizeInBits()
{

    long ret = 0;

    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        ret += (*it).getEstimatedSizeInBits();
    }

    return ret;

}
