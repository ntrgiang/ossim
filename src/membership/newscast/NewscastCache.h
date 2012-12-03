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

#ifndef NEWSCASTCACHE_H_
#define NEWSCASTCACHE_H_

#include "NewscastCacheEntry.h"
#include "cownedobject.h"

class NewscastCache : public ::cOwnedObject{
public:
    NewscastCache(int size);
    NewscastCache(){max = 5;};
    virtual ~NewscastCache();

    void setEntry(std::string agent, IPvXAddress addr, simtime_t timestamp, cObject* value);
    void merge(NewscastCache* cache);
    int getSize(){ return currentCache.size(); }
    NewscastCacheEntry* getEntry(int index){ return currentCache.at(index);}
    NewscastCacheEntry* getEntry(IPvXAddress addr);
    NewscastCacheEntry* getRandomEntry();

    virtual cOwnedObject *dup() const;

    unsigned int max;

    void printCache();

    /*
     * returns an estimation of the size of this cache in bytes
     */
    long getEstimatedSize();
protected:
    typedef std::vector<NewscastCacheEntry*> CacheSet;
    mutable CacheSet currentCache;

    NewscastCacheEntry* findEntryForAgent(std::string agent);
};

#endif /* NEWSCASTCACHE_H_ */
