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

#include "PartnerList.h"
#include "AppSettingDonet.h"
#include <omnetpp.h>
#include <map>

Define_Module(PartnerList);

PartnerList::PartnerList() {
    // TODO Auto-generated constructor stub

}

PartnerList::~PartnerList() {
    // TODO Auto-generated destructor stub

}

void PartnerList::initialize(int stage)
{
    if (stage != 3)
    {
        return;
    }

    cModule *temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet *appSetting = check_and_cast<AppSettingDonet *>(temp);
    m_bufferSize = appSetting->getBufferMapSizeChunk();
}

void PartnerList::finish()
{
    // Release allocated elements
    std::map<IPvXAddress, NeighborInfo *>::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        if (iter->second)
        {
            delete iter->second;
            iter->second = NULL;
        }
    }
}

// Raise error
void PartnerList::handleMessage(cMessage *)
{
    throw cException("PartnerList does not process messages!");
}

void PartnerList::print() const
{
    if (ev.isGUI() == false)
        return;

    EV << endl;
    EV << "%%%" << endl;
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    int i = 1;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        EV << "Partner " << i++ << ": " << iter->first
           << " -- upload bw: " << iter->second->getUpBw() << endl;
    }
}

void PartnerList::printAllSendBm() const
{

    if (ev.isGUI() == false)
        return;

    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    int i = 0;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        EV << "Partner " << ++i << ": " << iter->first;
        iter->second->printSendBm();

        // -- For Donet chunk scheduling
        EV << "TimeBudget: " << iter->second->getTimeBudget() << endl;
    }


}

void PartnerList::updateBoundSendBm(SEQUENCE_NUMBER_T head,
                                    SEQUENCE_NUMBER_T start,
                                    SEQUENCE_NUMBER_T end) const
{
    Enter_Method("print()");

    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second->setSeqNumSendBmHead(head);
        iter->second->setSeqNumSendBmStart(start);
        iter->second->setSeqNumSendBmEnd(end);
    }
}

void PartnerList::updateBoundSendBm(SEQUENCE_NUMBER_T start,
                                    SEQUENCE_NUMBER_T end) const
{
    Enter_Method("print()");

    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second->setSeqNumSendBmHead(-1L);
        iter->second->setSeqNumSendBmStart(start);
        iter->second->setSeqNumSendBmEnd(end);
    }
}

/**
 * Could also be obsolete
 */
bool PartnerList::isPartner(const IPvXAddress& addr) const
{
    Enter_Method("isNeighbor()");

    AddressList::iterator it;
    it = m_map.find(addr);

    if (it == m_map.end())
        return false;

    return true;
}

bool PartnerList::have(const IPvXAddress& addr) const
{
    Enter_Method("isNeighbor()");

    AddressList::iterator it;
    it = m_map.find(addr);

    if (it == m_map.end())
        return false;

    return true;
}

int PartnerList::getSize() const
{
//    Enter_Method_Silent("isNeighbor");
    Enter_Method("getSize()");
    return m_map.size();
}


void PartnerList::addAddress(const IPvXAddress &addr)
{
    // first version
    Enter_Method("addAddress");

    if (isPartner(addr) == true)
        return;

    NeighborInfo *nbr_info = new NeighborInfo(m_bufferSize);
    m_map.insert(std::pair<IPvXAddress, NeighborInfo *>(addr, nbr_info));

    // second version
    /*
    std::pair<std::map<IPvXAddress, NeighborInfo*>::iterator, bool> ret;
    NeighborInfo *nbr_info = new NeighborInfo();
    ret = partnerList.insert(std::pair<IPvXAddress, NeighborInfo *>(addr, nbr_info));
    */
    // return ret;
//    if (ret == false)
//        return false;
//    return true
}

void PartnerList::addAddress(const IPvXAddress &addr, double upBw)
{
    // first version
//    Enter_Method_Silent("addNeighborAddress");
    Enter_Method("addAddress");

    if (isPartner(addr) == true)
        return;

    NeighborInfo *nbr_info = new NeighborInfo(m_bufferSize);
    nbr_info->setUpBw(upBw);

    // -- Insert the pair into the map
    m_map.insert(std::pair<IPvXAddress, NeighborInfo *>(addr, nbr_info));
}

/*
void PartnerList::updateNeighborInfo(NeighborInfo *)
{

}
*/

// -- Check if the address is already in the partnerList
// -- if YES: delete the OLD NeighborInfo
// -- if NO: something wrong!
int PartnerList::updateStoredBufferMap(IPvXAddress address, BufferMap *bm)
{
    Enter_Method("updateStoredBufferMap()");
    std::map<IPvXAddress, NeighborInfo *>::iterator iter;
    iter = m_map.find(address);

    if (iter == m_map.end())
    {
        // address is not in the partnerList
        return -RVALUE_ERROR;
    }

    iter->second->copyFrom(bm);
    iter->second->setLastRecvBmTime(simTime().dbl());

/*
    if (iter->second) delete iter->second;

    NeighborInfo *neighborInfo = new NeighborInfo;
    neighborInfo->setLastRecvBmTime(simTime().dbl());
    //neighborInfo->setBufferMap(bm);
    neighborInfo->updateRecvBufferMap(bm);
    iter->second = neighborInfo;
*/

    return RVALUE_NO_ERROR;
}


bool PartnerList::deleteAddress(const IPvXAddress &addr)
{
//    Enter_Method_Silent("deleteNeighborAddress");
    Enter_Method("deleteAddress()");

    m_map.erase(addr);

    return true;
}


std::vector<IPvXAddress> PartnerList::getAddressList() const
{
//    Enter_Method_Silent("getpartnerList");
    Enter_Method("getAddressList()");

    std::vector<IPvXAddress>resultAddressList;
    AddressList::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        resultAddressList.push_back(iter->first);
    }
    return resultAddressList;
}

/*
void PartnerList::updateKeepAliveTimestamp(IPvXAddress addr, long timestamp)
{
//    Enter_Method_Silent("updateKeepAliveTimestamp");
//    Enter_Method("updateKeepAliveTimestamp");

    std::map<IPvXAddress, NeighborInfo *>::iterator iter;
    iter = partnerList.find(addr);
    iter->second->lastKeepAliveTime(timestamp);
}
*/

void PartnerList::getHolderList(SEQUENCE_NUMBER_T seq_num, std::vector<IPvXAddress> &holderList)
{
    Enter_Method("getHolderList()");
    // -- Debug
    // EV << "Enter getHolderList!!!!" << endl;

    holderList.clear();
    std::map<IPvXAddress, NeighborInfo *>::iterator iter;

    for (iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        NeighborInfo *nbr_info = iter->second;
        if (nbr_info->getLastRecvBmTime() != -1)
        {
            if (nbr_info->isInRecvBufferMap(seq_num))
            {
                holderList.push_back(iter->first);
                // -- Debug
                // EV << "\tPartner " << iter->first << " HAS the chunk " << seq_num << endl;
            }
            else
            {
                // -- Debug
                // EV << "\tPartner " << iter->first << " does NOT have chunk " << seq_num << endl;
            }
        }
        else
        {
            // -- Debug
            // EV << "\tBufferMap from " << iter->first << " is too old!" << endl;
        }
    }
}

NeighborInfo* PartnerList::getNeighborInfo(const IPvXAddress &address) const
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end()) return NULL;

    return iter->second;
}

void PartnerList::printRecvBm(const IPvXAddress &address)
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
    {
        EV << "The address " << address << " is not found!" << endl;
    }
    else
    {
        EV << "\t\tPartner " << address << " sent: " << endl;
        iter->second->printRecvBm();
    }

}
void PartnerList::printSendBm(const IPvXAddress &address)
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
    {
        EV << "The address " << address << " is not found!" << endl;
    }
    else
    {
        EV << "Partner " << address << " will receive this: ";
        iter->second->printSendBm();
    }
}

void PartnerList::clearAllSendBm(void)
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second->clearSendBm();
    }
}

void PartnerList::clearAllTimeBudget(void)
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second->setTimeBudget(0.0);
    }
}

void PartnerList::resetAllAvailableTime(SEQUENCE_NUMBER_T vb_start, SEQUENCE_NUMBER_T win_start, double chunkInterval)
{
    std::map<IPvXAddress, NeighborInfo*>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second->resetVectorAvailableTime(vb_start, win_start, chunkInterval);
    }
}

