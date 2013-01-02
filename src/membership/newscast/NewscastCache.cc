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
    max = size;
}

NewscastCache::~NewscastCache(){
    EV << "~NewscastCache()" << endl;
    NewscastCacheEntry* entry;
    while (currentCache.size() > 0){
        entry = currentCache.at(currentCache.size()-1);
        currentCache.pop_back();
        if (entry != NULL) delete entry;
    }

    currentCache.clear(); // TODO: delete objects
}

cOwnedObject* NewscastCache::dup() const{
    NewscastCache* ret = new NewscastCache(max);
    CacheSet::iterator it;
    for (it = currentCache.begin(); it != currentCache.end(); it++){
        ret->setEntry( (*it)->getAgent(), (*it)->getAddress(), (*it)->getTimestamp(), (*it)->getValue() );
    }

    return ret;
}

void NewscastCache::setEntry(std::string agent, IPvXAddress addr, simtime_t timestamp, float value){
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
    entry->setValue(value);

    //EV << "[set-entry-end]my name: " << entry->getAgent() << " value: " << entry->getValue() <<  endl;
    entry = findEntryForAgent(agent);
    //EV << "[set-entry-end2]my name: " << entry->getAgent() << " value: " << entry->getValue() <<  endl;
}

void NewscastCache::merge(NewscastCache* cache){

    // insert the entries from the new cache into the current
    CacheSet::iterator it;
    for (it = cache->currentCache.begin(); it != cache->currentCache.end(); it++){
        setEntry( (*it)->getAgent(), (*it)->getAddress(), (*it)->getTimestamp(), (*it)->getValue() );
    }

    // TODO: now check maximum count and remove oldest ...
    int lol = 0;
    while ((currentCache.size() > max) && (lol < 10)){
        EV << "WHILE" << endl;
        int oldest = 0; simtime_t time = currentCache.at(0)->getTimestamp();
           for (unsigned int i = 1; i < currentCache.size(); i++)
               if (currentCache.at(i)->getTimestamp() < time){
                   time = currentCache.at(i)->getTimestamp();
                   oldest = i;
               }
           it = currentCache.begin();
           it += oldest;
           printCache();
           EV << "Deleting: " << (*it)->getAgent() << endl;
           currentCache.erase(it);
       lol++;
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
    int aRandomIndex = rand() % currentCache.size();

    return currentCache.at(aRandomIndex);
}

NewscastCacheEntry* NewscastCache::getEntry(IPvXAddress addr){
    for (unsigned int i = 0; i < currentCache.size(); i++)
        if (currentCache.at(i)->getAddress().equals(addr))
            return currentCache.at(i);

    return NULL;
}
