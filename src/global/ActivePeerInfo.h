/*
 * ActivePeerInfo.h
 *
 *  Created on: May 3, 2012
 *      Author: giang
 */

#ifndef ACTIVE_PEER_INFO_H_
#define ACTIVE_PEER_INFO_H_

#include <vector>
//#include <IPvXAddress.h>
using namespace std;

struct Struct_ActivePeerInfo
{
   int m_maxNOP;
   int m_current_nPartner;
   double m_joinTime;
};

/*
class ActivePeerInfo
{
public:
    ActivePeerInfo();
    ActivePeerInfo(int maxNOP, int current_nPartner, double joinTime);
    ~ActivePeerInfo();

public:
    // -- For debugging --
    void printStatus(void);
//    void incrementNPartner(void);
//    void decrementNPartner(void);

    //void setMaxNop(int maxNOP);
    int getMaxNop(void) { return m_maxNOP; }
    int getCurrentNumberOfPartner(void) { return m_current_nPartner; }
    double getJoinTime(void) { return m_joinTime; }

// Data member
private:
    int m_maxNOP;
    int m_current_nPartner;
    double m_joinTime;
    //vector<IPvXAddress> m_partnerList;
};
*/

#endif /* ACTIVE_PEER_INFO_H_ */
