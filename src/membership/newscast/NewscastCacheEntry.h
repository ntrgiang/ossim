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

#ifndef NEWSCASTCACHEENTRY_H_
#define NEWSCASTCACHEENTRY_H_

#include "IPvXAddress.h"
#include "simtime_t.h"

class NewscastCacheEntry {
public:
    NewscastCacheEntry();
    virtual ~NewscastCacheEntry();

    // Getter
    IPvXAddress getAddress()  { return m_address;}
    simtime_t   getTimestamp(){ return m_timestamp;}
    std::string getAgent()    { return m_agent;}
    float       getValue()    { return m_value;} // temp

    // Setter
    void setAddress(IPvXAddress addr)       { m_address = addr;}
    void setTimestamp(simtime_t timestamp)  { m_timestamp = timestamp;}
    void setAgent(std::string agent)        { m_agent = agent;}
    void setValue(float value)              { m_value = value;}
protected:
    IPvXAddress m_address;
    simtime_t   m_timestamp;
    std::string m_agent;
    float       m_value; // temp
};

#endif /* NEWSCASTCACHEENTRY_H_ */
