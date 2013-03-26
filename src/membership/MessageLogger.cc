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
// MessageLogger.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "MessageLogger.h"
#include "GossipMembershipPacket_m.h"
//#include <map>
//#include <algorithm>

MessageLogger::MessageLogger() {
    // TODO Auto-generated constructor stub

}

MessageLogger::~MessageLogger() {
    // TODO Auto-generated destructor stub
}

void MessageLogger::initialize()
{
}

void MessageLogger::finish()
{

}

bool MessageLogger::nodeLogged(IPvXAddress addr)
{
    std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator it;
    it = m_logList.find(addr);
    if (it == m_logList.end())
        return false;
    else
        return true;
}

bool MessageLogger::isUniqueMessage(cPacket *pkt, int& count)
{
    // printNodeList();
    bool isUnique = true;
    bool isFound = false;
    std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator itNode;
    MESSAGE_ID_LIST::iterator itMsg;

    GossipApplicationPacket *appPkt = check_and_cast<GossipApplicationPacket *>(pkt);
    IPvXAddress rootAddress = appPkt->getRootAddress();
    MESSAGE_ID_TYPE msgId = appPkt->getMessageId();

    itNode = m_logList.find(rootAddress);
    if (itNode != m_logList.end())
    {
        isFound = true;
    }

    if (isFound == true)
    {
        for (itMsg = itNode->second.begin(); itMsg != itNode->second.end(); ++itMsg)
        {
            if (*itMsg == msgId)
            {
                isUnique = false;
            }
        }
    }

    if (isUnique == true)
    {
        if (isFound == false)
        {
            // -- Insert a new node_id into the map
            std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator it;
            it = m_logList.begin();
            MESSAGE_ID_LIST initialList;
            initialList.push_back(msgId);
            m_logList.insert(it, std::pair<IPvXAddress, MESSAGE_ID_LIST>(rootAddress, initialList));
            // printNodeList();
        }
        else
        {
            itNode->second.push_back(msgId);
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool MessageLogger::isUniqueMessage(IPvXAddress &rootAddress, MESSAGE_ID_TYPE &msgId, int& count)
{
    // printNodeList();
    bool isUnique = true;
    bool isFound = false;
    std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator itNode;

    itNode = m_logList.find(rootAddress);
    if (itNode != m_logList.end())
    {
        isFound = true;
    }

    if (isFound == true)
    {
        for (MESSAGE_ID_LIST::iterator itMsg = itNode->second.begin(); itMsg != itNode->second.end(); ++itMsg)
        {
            if (*itMsg == msgId)
            {
                isUnique = false;
            }
        }
    }

    if (isUnique == true)
    {
        if (isFound == false)
        {
            // -- Insert a new node_id into the map
            std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator it;
            it = m_logList.begin();
            MESSAGE_ID_LIST initialList;
            initialList.push_back(msgId);
            m_logList.insert(it, std::pair<IPvXAddress, MESSAGE_ID_LIST>(rootAddress, initialList));
            // printNodeList();
        }
        else
        {
            itNode->second.push_back(msgId);
        }
    }
    else
    {
        return false;
    }
    return true;
}

void MessageLogger::printNodeList(void)
{
    EV << "Current list of " << m_logList.size() << " logged nodes:" << endl;
    std::map<IPvXAddress, MESSAGE_ID_LIST>::iterator iter;
    for (iter = m_logList.begin(); iter != m_logList.end(); ++iter)
    {
        EV << (*iter).first << " | ";
    }
    EV << endl;
}
