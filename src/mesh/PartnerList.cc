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
// PartnerList.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "PartnerList.h"
#include "AppSettingDonet.h"
#include <omnetpp.h>
#include <map>

Define_Module(PartnerList)

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
   m_map.clear();

}

// Raise error
void PartnerList::handleMessage(cMessage *)
{
    throw cException("PartnerList does not process messages!");
}

void PartnerList::print() const
{
    Enter_Method("print()");

    if (ev.isGUI() == false)
        return;

    EV << "Partner list: " << endl;

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    int i = 1;
    for (iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
       EV << "Partner " << i++ << ": " << iter->first
           << " -- upload bw: " << iter->second.getUpBw() << endl;
    }
}

void PartnerList::printAllSendBm() const
{
    Enter_Method("printAllSendBm()");

    if (ev.isGUI() == false)
        return;

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    int i = 0;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        EV << "Partner " << ++i << ": " << iter->first;
        iter->second.printSendBm();

        // -- For Donet chunk scheduling
        EV << "TimeBudget: " << iter->second.getTimeBudget() << endl;
    }

}

void PartnerList::updateBoundSendBm(SEQUENCE_NUMBER_T head,
                                    SEQUENCE_NUMBER_T start,
                                    SEQUENCE_NUMBER_T end) const
{
    Enter_Method("updateBoundSendBm()");

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.setSeqNumSendBmHead(head);
        iter->second.setSeqNumSendBmStart(start);
        iter->second.setSeqNumSendBmEnd(end);
    }
}

void PartnerList::updateBoundSendBm(SEQUENCE_NUMBER_T start,
                                    SEQUENCE_NUMBER_T end) const
{
    Enter_Method("updateBoundSendBm()");

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.setSeqNumSendBmHead(-1L);
        iter->second.setSeqNumSendBmStart(start);
        iter->second.setSeqNumSendBmEnd(end);
    }

}

void PartnerList::resetNChunkScheduled()
{
    Enter_Method("resetNChunkScheduled()");

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for(iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.setNChunkScheduled(0);
    }
}

void PartnerList::setElementSendBm(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num, bool val)
{
//    for(std::map<IPvXAddress, NeighborInfo>::iterator iter = m_map.begin();
//        iter != m_map.end(); ++iter)
//    {
//        iter->second.setNChunkScheduled(0);
//    }

//   EV << "Address: " << addr << " -- seq_num: " << seq_num << " -- value: " << val << endl;

   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   iter = m_map.find(addr);

   if (iter != m_map.end())
   {
      iter->second.setElementSendBm(seq_num, val);
   }
   else
   {
      throw cException("Address not found from the partnerList to setElementSendBm");
   }
}

double PartnerList::getUpBw(IPvXAddress addr)
{
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   iter = m_map.find(addr);

   if (iter != m_map.end())
   {
      return iter->second.getUpBw();
   }
   else
   {
      throw cException("Address not found from the partnerList to getUpBw");
   }
}

bool PartnerList::updateChunkAvailTime(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num, double time)
{
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   iter = m_map.find(addr);

   if (iter != m_map.end())
   {
      return iter->second.updateChunkAvailTime(seq_num, time);
   }
   else
   {
      throw cException("Address not found from the partnerList to updateChunkAvailTime");
   }
}

double PartnerList::getChunkAvailTime(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num)
{
   std::map<IPvXAddress, NeighborInfo>::iterator iter;
   iter = m_map.find(addr);

   if (iter != m_map.end())
   {
      return iter->second.getChunkAvailTime(seq_num);
   }
   else
   {
      throw cException("Address %s not found from the partnerList to getChunkAvailTime", addr.str().c_str());
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
    Enter_Method("getSize()");
    return m_map.size();
}


void PartnerList::addAddress(const IPvXAddress &addr)
{
    // first version
    Enter_Method("addAddress()");

    if (isPartner(addr) == true)
        return;

    NeighborInfo nbr_info(m_bufferSize);
    m_map.insert(std::pair<IPvXAddress, NeighborInfo>(addr, nbr_info));

}

void PartnerList::addAddress(const IPvXAddress &addr, double upBw)
{
    Enter_Method("addAddress()");

    if (isPartner(addr) == true)
        return;

    NeighborInfo nbr_info(m_bufferSize);
    nbr_info.setUpBw(upBw);
    nbr_info.setUploadRate_Chunk(0);

    // -- Insert the pair into the map
    m_map.insert(std::pair<IPvXAddress, NeighborInfo>(addr, nbr_info));
}

void PartnerList::addAddress(const IPvXAddress &addr, double upBw, int nChunk)
{
    Enter_Method("addAddress()");

    if (isPartner(addr) == true)
        return;

    NeighborInfo nbr_info(m_bufferSize);
    nbr_info.setUpBw(upBw);
    nbr_info.setUploadRate_Chunk(nChunk);

    // Partnership management
    nbr_info.setCountChunkReceived(0L);
    nbr_info.setCountChunkSent(0L);

    // -- Insert the pair into the map
    m_map.insert(std::pair<IPvXAddress, NeighborInfo>(addr, nbr_info));
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


    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    iter = m_map.find(address);

    if (iter == m_map.end())
    {
        // address is not in the partnerList
        return -RVALUE_ERROR;
    }

    iter->second.copyFrom(bm);
    iter->second.setLastRecvBmTime(simTime().dbl());

    return RVALUE_NO_ERROR;
}


bool PartnerList::deleteAddress(const IPvXAddress &addr)
{
    Enter_Method("deleteAddress()");

    m_map.erase(addr);

    return true;
}


std::vector<IPvXAddress> PartnerList::getAddressList() const
{
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

    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); ++iter)
    {
        //NeighborInfo nbr_info = iter->second;
        if (iter->second.getLastRecvBmTime() != -1)
        {
            //EV << "  -- At peer " << iter->first << ": ";
            if (iter->second.isInRecvBufferMap(seq_num))
            {
                if (iter->second.getNChunkScheduled() < iter->second.getUploadRate_Chunk())
                {
                   holderList.push_back(iter->first);
                   // EV << "\tPartner " << iter->first << " HAS the chunk " << seq_num << endl;
                }
            }
            else
            {
                // EV << "\tPartner " << iter->first << " does NOT have chunk " << seq_num << endl;
            }
        }
        else
        {
            // EV << "\tBufferMap from " << iter->first << " is too old!" << endl;
        }
    } // for

}

int PartnerList::getNumberOfHolder(SEQUENCE_NUMBER_T seq_num)
{
   int nHolder = 0;
   for (std::map<IPvXAddress, NeighborInfo>::iterator iter = m_map.begin();
      iter != m_map.end(); ++iter)
   {
      if (iter->second.getLastRecvBmTime() != -1)
      {
         //EV << "  -- At peer " << iter->first << ": ";
         if (iter->second.isInRecvBufferMap(seq_num))
         {
            if (iter->second.getNChunkScheduled() < iter->second.getUploadRate_Chunk())
            {
               //holderList.push_back(iter->first);
               ++nHolder;
               EV << "\tPartner " << iter->first << " HAS the chunk " << seq_num << endl;
            }
         }
         else
         {
            // EV << "\tPartner " << iter->first << " does NOT have chunk " << seq_num << endl;
         }
      }
      else
      {
         // EV << "\tBufferMap from " << iter->first << " is too old!" << endl;
      }
   } // for
   return nHolder;
}

bool PartnerList::hasAddress(const IPvXAddress & address) const
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
       return false;

    return true;
}

NeighborInfo* PartnerList::getNeighborInfo(const IPvXAddress &address) const
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
    {
       return NULL;
    }
    return &(iter->second);
}

void PartnerList::printRecvBm(const IPvXAddress &address)
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
    {
        EV << "The address " << address << " is not found!" << endl;
    }
    else
    {
        EV << "\t\tPartner " << address << " sent: " << endl;
        iter->second.printRecvBm();
    }

}
void PartnerList::printSendBm(const IPvXAddress &address)
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    iter = m_map.find(address);
    if (iter == m_map.end())
    {
        EV << "The address " << address << " is not found!" << endl;
    }
    else
    {
        EV << "Partner " << address << " will receive this: ";
        iter->second.printSendBm();
    }
}

void PartnerList::clearAllSendBm(void)
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.clearSendBm();
    }
}

void PartnerList::clearAllTimeBudget(void)
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.setTimeBudget(0.0);
    }

}

void PartnerList::resetAllAvailableTime(SEQUENCE_NUMBER_T vb_start, SEQUENCE_NUMBER_T win_start, double chunkInterval)
{
    std::map<IPvXAddress, NeighborInfo>::iterator iter;
    for (iter=m_map.begin(); iter != m_map.end(); ++iter)
    {
        iter->second.resetVectorAvailableTime(vb_start, win_start, chunkInterval);
    }

}

