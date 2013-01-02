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

#include "NewscastCache.h"

NewscastCache::NewscastCache(int size) : cOwnedObject(){
    m_maxEntries = size;
    EV << "NewscastCache-> " << size << endl;
}

NewscastCache::~NewscastCache(){
    EV << "~NewscastCache()" << endl;
    //while(!currentCache.empty()) delete currentCache.back(), currentCache.pop_back();
    /*NewscastCacheEntry* entry;
    while (currentCache.size() > 0){
        entry = currentCache.at(currentCache.size()-1);
        currentCache.pop_back();
        //if (entry != NULL) delete entry;
    }*/
    EV << "~NewscastCache() -> DONE" << endl;
    currentCache.clear(); // TODO: delete objects
    EV << "~NewscastCache() -> DONE 2" << endl;
}

cOwnedObject* NewscastCache::dup() const{
    NewscastCache* ret = new NewscastCache(m_maxEntries);
    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        ret->setEntry( (*it)->getAgent(), (*it)->getAddress(), (*it)->getTimestamp(), (*it)->getValue() );
    }

    return ret;
}

void NewscastCache::setEntry(std::string agent, IPvXAddress addr, simtime_t timestamp, GossipUserData* value){
    //EV << "[set-entry]my name: " << agent << " value: " << value <<  endl;
    NewscastCacheEntry* entry = findEntryForAgent(agent);
    if (entry == NULL){ // generate new entry
        entry = new NewscastCacheEntry();
        entry->setAgent(agent);
        entry->setTimestamp(0);
        currentCache.push_back(entry);
    }

    // check timestamp ... new one < old? -> return
    if (timestamp < entry->getTimestamp()) return;

    // set entry
    entry->setAddress(addr); // should i update this everytime? hmm
    entry->setTimestamp(timestamp);
    if (value == NULL)
        entry->setValue(NULL);
    else
        entry->setValue(value->dup());

    //EV << "[set-entry-end]my name: " << entry->getAgent() << " value: " << entry->getValue() <<  endl;
    //entry = findEntryForAgent(agent);
    //EV << "[set-entry-end2]my name: " << entry->getAgent() << " value: " << entry->getValue() <<  endl;
}

void NewscastCache::merge(NewscastCache* cache){

    // insert the entries from the new cache into the current
    CacheSet::iterator it;
    for (it = cache->currentCache.begin(); it != cache->currentCache.end(); it++){
        setEntry( (*it)->getAgent(), (*it)->getAddress(), (*it)->getTimestamp(), (*it)->getValue() );
    }

    while (currentCache.size() > m_maxEntries){  // while we have more than m_maxEntries ...
        int oldest = 0; simtime_t time = currentCache.at(0)->getTimestamp();

        // find the oldest one
        for (unsigned int i = 1; i < currentCache.size(); i++)
            if (currentCache.at(i)->getTimestamp() < time){
                time = currentCache.at(i)->getTimestamp();
                oldest = i;
            }

        // set the pointer to the oldest one
        it = currentCache.begin();
        it += oldest;

        printCache();
        EV << "Deleting: " << (*it)->getAgent() << endl;

        // delete it
        delete *it;
        currentCache.erase(it);
    }
}

NewscastCacheEntry* NewscastCache::findEntryForAgent(std::string agent){
    for (unsigned int i = 0; i < currentCache.size(); i++)
        if (currentCache.at(i)->getAgent().compare(agent) == 0)
            return currentCache.at(i);

    return NULL;
}


void NewscastCache::printCache(){
    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        EV << (*it)->getAgent() <<","<< (*it)->getAddress() <<","<< (*it)->getTimestamp() <<","<< (*it)->getValue() << endl;
    }
}

NewscastCacheEntry* NewscastCache::getRandomEntry(){
    int aRandomIndex = (int)intrand(currentCache.size());//rand() % currentCache.size();

    return currentCache.at(aRandomIndex);
}

NewscastCacheEntry* NewscastCache::getEntry(IPvXAddress addr){
    for (unsigned int i = 0; i < currentCache.size(); i++)
        if (currentCache.at(i)->getAddress().equals(addr))
            return currentCache.at(i);

    return NULL;
}

std::vector<IPvXAddress> NewscastCache::getAllAddresses(){
    std::vector<IPvXAddress> ret;

    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++)
        ret.push_back( (*it)->getAddress());

    return ret;
}


long NewscastCache::getEstimatedSizeInBits(){

    long ret = 0;

    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        ret += (*it)->getEstimatedSizeInBits();
    }

    return ret;

}
