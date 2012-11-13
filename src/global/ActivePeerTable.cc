/*
 * ActivePeerTable.cc
 *
 *  Created on: Mar 30, 2012
 *      Author: giang
 */

#include "ActivePeerTable.h"
#include <algorithm>

Define_Module(ActivePeerTable);

void ActivePeerTable::updateDisplayString()
{

}

ActivePeerTable::ActivePeerTable() {}

ActivePeerTable::~ActivePeerTable() {}

void ActivePeerTable::initialize(int stage)
{
    if (stage==0)
        {
            // get a pointer to the NotificationBoard module and IInterfaceTable
            nb = NotificationBoardAccess().get();

            nb->subscribe(this, NF_INTERFACE_CREATED);
            nb->subscribe(this, NF_INTERFACE_DELETED);
            nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
            nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
            nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

            sig_size = registerSignal("Signal_Size");
        }
        else if (stage==1)
        {

        }
        else if (stage==3)
        {
            cModule *temp = simulation.getModuleByPath("globalStatistic");
            m_gstat = check_and_cast<GlobalStatistic *>(temp);
            EV << "Binding to globalStatistic is completed successfully" << endl;
        }

    //WATCH(m_activePeerList.size());
    //WATCH(m_activePeerList);

}

void ActivePeerTable::finish()
{
    m_gstat->writeActivePeerTable2File(getListActivePeer());

}

vector<IPvXAddress> ActivePeerTable::getListActivePeer()
{
    vector<IPvXAddress> storeList;

    // -- With list
//    for (AddressSet::iterator iter = activePeerList.begin();
//         iter != activePeerList.end(); ++iter)
//    {
//        storeList.push_back(*iter);
//    }

    // -- With map
    //map<IPvXAddress, ActivePeerInfo*>::iterator iter;
    Type_ActiveList::iterator iter;
    for (iter = m_activePeerList.begin(); iter != m_activePeerList.end(); ++iter)
    {
        storeList.push_back(iter->first);
    }
    return storeList;
}

/**
 * Raises an error.
 */
void ActivePeerTable::handleMessage(cMessage *)
{
    throw cException("ActivePeerTable doesn't process messages!");
}

/**
 * Called by the NotificationBoard whenever a change of a category
 * occurs to which this client has subscribed.
 */
void ActivePeerTable::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

/**
 * For debugging
 */
void ActivePeerTable::printActivePeerTable()
{
   Enter_Method("printActivePeerTable()");

    if (ev.isGUI() == false)
        return;

    EV << endl;
    EV << "%%%" << endl;
    EV << "-- Active peers :" << m_activePeerList.size() << endl;

    // -- With list
//    AddressSet::iterator iter;
//    for (iter = activePeerList.begin(); iter != activePeerList.end(); ++iter)
//    {
//        EV << iter->get4() << endl;
//    }

    // -- With map
   Type_ActiveList::iterator iter;
   for (iter = m_activePeerList.begin(); iter != m_activePeerList.end(); ++iter)
   {
      EV << iter->first.get4() << endl;
   }
}

//void ActivePeerTable::setName(std::string name)
//{
//    m_name = name;
//}

/** @name Routing functions (query the route table) */
//@{
/**
 * Checks if a peer, given its address, is an active one the overlay.
 */
/*
bool ActivePeerTable::isActivePeer(const IPvXAddress& dest) const
{
    //Enter_Method("isActivePeer()");

//    AddressSet::iterator it = activePeerList.search(dest);
    // -- With list
//    AddressSet::iterator it;
//    for (it = activePeerList.begin(); it != activePeerList.end(); it++)
//    {
//        if (*it == dest)
//        {
//            return true;
//        }
//    }

    // -- With map
    map<IPvXAddress, ActivePeerInfo*>::iterator iter;
    iter = m_activePeerList.find(dest);

    if (iter == m_activePeerList.end())
        return false;

    return true;
}
*/


/**
 * Returns the total number of active peers
 */
int ActivePeerTable::getNumActivePeer() const
{
    Enter_Method("getNumActivePeer()");

    //return activePeerList.size();
    return m_activePeerList.size();
}

/**
 * Adds a peer address to the table. Note that once added, addresses
 * cannot be modified; you must delete and re-add them instead.
 */
//void ActivePeerTable::addPeerAddress(const IPvXAddress *address)
/*
void ActivePeerTable::addPeerAddress(const IPvXAddress &address)
{
    Enter_Method("addPeerAddress()");

   Struct_ActivePeerInfo info;
    m_activePeerList[address] = info;
    //m_activePeerList[address] = 1;
    emit(sig_size, m_activePeerList.size());
}
*/

void ActivePeerTable::addPeerAddress(const IPvXAddress &address, int maxNOP)
{
    Enter_Method("addPeerAddress()");

    //ActivePeerInfo *temp = new ActivePeerInfo(maxNOP, 1, simTime().dbl());
    Struct_ActivePeerInfo info;
      info.m_maxNOP = maxNOP;
      info.m_current_nPartner = 1;
      info.m_joinTime = simTime().dbl();

    m_activePeerList.insert(pair<IPvXAddress, Struct_ActivePeerInfo>(address, info));
//    m_activePeerList[address] = info;
    emit(sig_size, m_activePeerList.size());
}

/*
void ActivePeerTable::addSourceAddress(const IPvXAddress &address)
{
    Enter_Method("addSourceAddress()");

    //m_activePeerList.insert(pair<IPvXAddress, int>(address, 0));
    //m_activePeerList[address] = 0;


    emit(sig_size, m_activePeerList.size());
}
*/

void ActivePeerTable::addSourceAddress(const IPvXAddress &address, int maxNOP)
{
   Enter_Method("addSourceAddress()");

   //m_activePeerList.insert(pair<IPvXAddress, int>(address, 0));
   //m_activePeerList[address] = 0;

   Struct_ActivePeerInfo info;
     info.m_maxNOP = maxNOP;
     info.m_current_nPartner = 0;
     info.m_joinTime = simTime().dbl();

   m_activePeerList.insert(pair<IPvXAddress, Struct_ActivePeerInfo>(address, info));
   //m_activePeerList[address] = info;
    emit(sig_size, m_activePeerList.size());
}

void ActivePeerTable::incrementNPartner(const IPvXAddress &addr)
{
   Enter_Method("incrementNPartner()");

   //m_activePeerList[addr] += 1;

   EV << "Increment for address " << addr << endl;

   EV << "***********************************************************************************" << endl;
   EV << "***********************************************************************************" << endl;
   EV << "Before increment: current_nPartner = " << m_activePeerList[addr].m_current_nPartner << endl;

   m_activePeerList[addr].m_current_nPartner += 1;

   EV << "After increment: current_nPartner = " << m_activePeerList[addr].m_current_nPartner << endl;
   EV << "***********************************************************************************" << endl;
   EV << "***********************************************************************************" << endl;
}

void ActivePeerTable::decrementNPartner(const IPvXAddress &addr)
{
  //m_activePeerList[addr] -= 1;
   m_activePeerList[addr].m_current_nPartner -= 1;
}

/**
 * Deletes the given peer address from the table.
 * Returns true if the address was deleted correctly, false if it was
 * not in the table.
 */
//bool ActivePeerTable::deletePeerAddress(const IPvXAddress *address)
bool ActivePeerTable::deletePeerAddress(const IPvXAddress &address)
{
    Enter_Method("deletePeerAddress()");

    // -- With list
//    if (!isActivePeer(address))
//        return false;
//    AddressSet::iterator it;
//    for (it = activePeerList.begin(); it != activePeerList.end(); it++)
//    {
//        if (*it == address)
//        {
//            activePeerList.erase(it);
//            break;
//        }
//    }
//    return true;

    // -- With map
    //map<IPvXAddress, ActivePeerInfo*>::iterator iter;
    Type_ActiveList::iterator iter;
    iter = m_activePeerList.find(address);

    if (iter == m_activePeerList.end())
        return false;

    //delete iter->second;
    m_activePeerList.erase(iter);
    return true;

}

/**
 * Utility function: Returns a vector of /N/ addresses of the active nodes.
 */
/*
std::vector<IPvXAddress> ActivePeerTable::getNPeer() const
{
    std::vector<IPvXAddress> resultActivePeer;

    long int table_size = getNumActivePeer();
    long int N = std::min(table_size, (long int)LIST_SIZE);

    random_shuffle(activePeerList.begin(), activePeerList.end());

    AddressSet::iterator it = activePeerList.begin();
    for (int i = 0; i< N; i++)
    {
        resultActivePeer.push_back(*it);
        it++;
    }

    return resultActivePeer;
}
*/

/**
 * Utility function: Returns a vector of /N/ addresses of the active nodes.
 */
/*
std::vector<IPvXAddress> ActivePeerTable::getNPeer(long int N) const
{
    std::vector<IPvXAddress> resultActivePeer;

    //long int table_size = getNumActivePeer();
    long int table_size = activePeerList.size();
    long int M = std::min(table_size, N);

    random_shuffle(activePeerList.begin(), activePeerList.end());

//    AddressSet::iterator it = activePeerList.begin();
//    for (int i = 0; i<M; i++)
//    {
//        resultActivePeer.push_back(*it);
//        it++;
//    }

    int i = 0;
    for (AddressSet::iterator it = activePeerList.begin(); it != activePeerList.end(); ++it)
    {
        resultActivePeer.push_back(*it);
        if (i >= M-1)
            break;
        ++i;
    }

    return resultActivePeer;
}
*/

/**
 * Utility function: Returns exactly one random address of the active nodes.
 * A wrapper of the getNPeer(long int N)
 */

//IPvXAddress ActivePeerTable::getARandPeer() const
IPvXAddress ActivePeerTable::getARandPeer()
{
   Enter_Method("getARandPeer()");

//    std::vector<IPvXAddress> resultList;
//    resultList = getNPeer(1);
//
//    return resultList[0];
    //int size = activePeerList.size();
//    int aRandomNumber = rand();
//    int aRandomIndex = aRandomNumber % size;

   if (m_activePeerList.size() == 1)
      return m_activePeerList.begin()->first;

   // -- Clear ALL content of the the tempList
   m_tempList.clear();

   // -- Find out the max value of nPartner
//   int max = 1;
//   Type_ActiveList::iterator iter;
//   for (iter = m_activePeerList.begin(); iter != m_activePeerList.end(); ++iter)
//   {
//      max = (max < iter->second)?iter->second:max;
//   }

//   EV << "max = " << max << endl;

   //vector<IPvXAddress> tempList;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   Type_ActiveList::iterator iter;
   for (iter = m_activePeerList.begin(); iter != m_activePeerList.end(); ++iter)
   {
      //for (int i = iter->second->getCurrentNumberOfPartner(); i <= iter->second->getMaxNop(); ++i)
      for (int i = iter->second.m_current_nPartner; i < iter->second.m_maxNOP; ++i)
      {
         m_tempList.push_back(iter->first);
         EV << "Address: " << iter->first << endl;
      }
   }
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;

   int size = m_tempList.size();

   if (size <= 0) throw cException("Wrong size of the tempList %d", size);
    int aRandomIndex = (int)intrand(size);

    //IPvXAddress randAddr = tempList[aRandomIndex];
    return m_tempList[aRandomIndex];

    //emit(sig_size, size);
//    emit(sig_size, aRandomIndex);

    //return activePeerList[aRandomIndex];
    //return m_activePeerList[aRandomIndex].first;
}


IPvXAddress ActivePeerTable::getARandPeer(IPvXAddress address)
{
   Enter_Method("getARandPeer()");

   if (m_activePeerList.size() == 1)
      return m_activePeerList.begin()->first;

   // -- Clear ALL content of the the tempList
   m_tempList.clear();

   //vector<IPvXAddress> tempList;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   Type_ActiveList::iterator iter;
   for (iter = m_activePeerList.begin(); iter != m_activePeerList.end(); ++iter)
   {
      //for (int i = iter->second->getCurrentNumberOfPartner(); i <= iter->second->getMaxNop(); ++i)
      if (iter->first == address)
         continue;

      for (int i = iter->second.m_current_nPartner; i < iter->second.m_maxNOP; ++i)
      {
         m_tempList.push_back(iter->first);
         EV << "Address: " << iter->first << endl;
      }
   }
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;
   EV << "**********************************************************************" << endl;

   int size = m_tempList.size();

   if (size <= 0)
   {
      //throw cException("Wrong size of the tempList %d", size);

      // Hacking !!! (return the address of the source
      return m_activePeerList.begin()->first;
   }

   int aRandomIndex = (int)intrand(size);

   return m_tempList[aRandomIndex];
}

void ActivePeerTable::printActivePeerInfo(const IPvXAddress &address)
{
   Enter_Method("printActivePeerInfo()");

   EV << endl << "***********************************************************" << endl;
   Type_ActiveList::iterator iter;
   iter = m_activePeerList.find(address);

   if (iter == m_activePeerList.end())
   {
      EV << "Address " << address << " doesn't exist in the Active Peer Table" << endl;
      return;
   }

   EV << "Peer " << iter->first << "has " << iter->second.m_current_nPartner << " partners" << endl;
}

/**
 * Utility function: Returns a vector of all addresses of the active nodes.
 */
