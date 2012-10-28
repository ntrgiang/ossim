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

        }

}

void ActivePeerTable::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void ActivePeerTable::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

void ActivePeerTable::printActivePeerTable() const
{
    if (ev.isGUI() == false)
        return;

    EV << endl;
    EV << "%%%" << endl;
    EV << "-- Active peer table:" << endl;

    AddressSet::iterator iter;
    for (iter = activePeerList.begin(); iter != activePeerList.end(); ++iter)
    {
        EV << iter->get4() << endl;
    }
}

void ActivePeerTable::setName(std::string name)
{
    m_name = name;
}

bool ActivePeerTable::isActivePeer(const IPvXAddress& dest) const
{
    //Enter_Method("isActivePeer()");

//    AddressSet::iterator it = activePeerList.search(dest);
    AddressSet::iterator it;
    for (it = activePeerList.begin(); it != activePeerList.end(); it++)
    {
        if (*it == dest)
        {
            return true;
        }
    }

    return false;
}

int ActivePeerTable::getNumActivePeer() const
{
    Enter_Method("getNumActivePeer()");

    return activePeerList.size();
}


//void ActivePeerTable::addPeerAddress(const IPvXAddress *address)
void ActivePeerTable::addPeerAddress(const IPvXAddress &address)
{
    Enter_Method("addPeerAddress()");

    if (isActivePeer(address))
        return;

    activePeerList.push_back(address);

    emit(sig_size, activePeerList.size());
}

//bool ActivePeerTable::deletePeerAddress(const IPvXAddress *address)
bool ActivePeerTable::deletePeerAddress(const IPvXAddress &address)
{
    Enter_Method("deletePeerAddress()");

    if (!isActivePeer(address))
        return false;

    AddressSet::iterator it;
    for (it = activePeerList.begin(); it != activePeerList.end(); it++)
    {
        if (*it == address)
        {
            activePeerList.erase(it);
            break;
        }
    }
    return true;
}


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

//IPvXAddress ActivePeerTable::getARandPeer() const
IPvXAddress ActivePeerTable::getARandPeer()
{
//    std::vector<IPvXAddress> resultList;
//    resultList = getNPeer(1);
//
//    return resultList[0];
    int size = activePeerList.size();
//    int aRandomNumber = rand();
//    int aRandomIndex = aRandomNumber % size;
    int aRandomIndex = (int)intrand(size);

    //emit(sig_size, size);
//    emit(sig_size, aRandomIndex);

    return activePeerList[aRandomIndex];
}
