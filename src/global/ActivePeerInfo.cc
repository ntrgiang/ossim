/*
 * ActivePeerInfo.cc
 *
 *  Created on: May 3, 2012
 *      Author: giang
 */
#include "ActivePeerInfo.h"

using namespace std;

ActivePeerInfo::ActivePeerInfo()
    : m_current_nPartner(0), m_joinTime(0.0)
{

}

ActivePeerInfo(int maxNOP, int current_nPartner, double joinTime)
   : m_maxNOP (maxNOP), m_current_nPartner(current_nPartner), m_joinTime(joinTime)
{
}

//ActivePeerInfo::ActivePeerInfo(int bmSize)
//{
//    m_current_nPartner = 0;
//    m_joinTime = 0.0;
//}

ActivePeerInfo::~ActivePeerInfo()
{
}

ActivePeerInfo::printStatus()
{
    Enter_Method("printStatus()");
    EV << "Info: " << endl;
    EV << " -- m_current_nPartner = " << m_current_nPartner << endl;
    EV << " -- m_joinTime = " << m_joinTime << endl;
}
