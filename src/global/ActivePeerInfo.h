/*
 * ActivePeerInfo.h
 *
 *  Created on: May 3, 2012
 *      Author: giang
 */

#ifndef ACTIVE_PEER_INFO_H_
#define ACTIVE_PEER_INFO_H_

#include <vector>
#include <IPvXAddress.h>
using namespace std;

class ActivePeerInfo
{
public:
    ActivePeerInfo(int bmSize=0);
    virtual ~ActivePeerInfo();

public:
//    inline void setTimeBudget(double time) { m_timeBudget = time; }
//    inline double getTimeBudget(void) { return m_timeBudget; }

    // -- For debugging --
    void printStatus(void);

// Data member
private:
    // -- Available time to transmit a specific chunk
    vector<IPvXAddress> m_partnerList;
    double m_joinTime;
};


#endif /* ACTIVE_PEER_INFO_H_ */
