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
// CommBase.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//

#include "CommBase.h"
#include "DpControlInfo_m.h"
#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"
#include "InterfaceTable.h"
#include "assert.h"

#ifndef debugOUT
#define debugOUT (!m_debug) ? std::cout : std::cout << "::" << getFullName() << ": "
#endif

CommBase::CommBase() {
    // TODO Auto-generated constructor stub
}

CommBase::~CommBase() {
    // TODO Auto-generated destructor stub
}


int CommBase::getLocalPort(void)
{
    // return getParentModule()->getModuleByRelativePath("appSetting")->par("localPort").longValue();
    return simulation.getModuleByPath("appSetting")->par("localPort").longValue();
}

int CommBase::getDestPort(void)
{
//    return getParentModule()->getModuleByRelativePath("appSetting")->par("destPort").longValue();
    return simulation.getModuleByPath("appSetting")->par("destPort").longValue();
}

int CommBase::getVideoStreamBitRate(void)
{
//    return getParentModule()->getModuleByRelativePath("appSetting")->par("videoStreamBitRate").longValue();
    return simulation.getModuleByPath("appSetting")->par("videoStreamBitRate").longValue();
}

int CommBase::getChunkSize(void)
{
//    return getParentModule()->getModuleByRelativePath("appSetting")->par("chunkSize").longValue();
    return simulation.getModuleByPath("appSetting")->par("chunkSize").longValue();
}

int CommBase::getBufferMapSize(void)
{
//    return getParentModule()->getModuleByRelativePath("appSetting")->par("bufferMapSize").longValue();
    return simulation.getModuleByPath("appSetting")->par("bufferMapSize").longValue();
}

void CommBase::printPacket(cPacket *pkt)
{
    DpControlInfo *ctrl = check_and_cast<DpControlInfo *>(pkt->getControlInfo());

    EV  << ctrl->getSrcAddr() << ":"
        << ctrl->getSrcPort() << " --> "
        << ctrl->getDestAddr() << ":"
        << ctrl->getDestPort() << " :: ";

    EV  << pkt->getName() << "  (" << pkt->getByteLength() << " bytes)" << endl;
}

void CommBase::sendToDispatcher(cPacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort)
{
    EV << "Sending a packet to Dispatcher ... ";

    DpControlInfo *ctrl = new DpControlInfo();
        ctrl->setSrcAddr(getNodeAddress());
        ctrl->setSrcPort(srcPort);
        ctrl->setDestAddr(destAddr);
        ctrl->setDestPort(destPort);
    pkt->setControlInfo(ctrl);

    printPacket(pkt);

    send(pkt, "dpOut");
}

void CommBase::findNodeAddress(void)
{
   Enter_Method("findNodeAddress()");

   cModule* module = getParentModule()->getParentModule();
   InterfaceTable* interface = check_and_cast<InterfaceTable*>(module->getSubmodule("interfaceTable"));
   EV << "Number of interfaces: " << interface->getNumInterfaces() << endl;

   assert(interface->getNumInterfaces() > 0);

   if (interface->getNumInterfaces() > 1)
   {
      m_localAddress = IPvXAddress(interface->getInterface(interface->getNumInterfaces()-1)->ipv4Data()->getIPAddress());
   }
   else
   {
      m_localAddress = IPvXAddress(interface->getInterface(0)->ipv4Data()->getIPAddress());
   }

   EV << "Node address found: " << m_localAddress << endl;

}

IPvXAddress CommBase::getNodeAddress(void)
{
    if(m_localAddress.isUnspecified()) findNodeAddress();
    return m_localAddress;
}

void CommBase::bindToGlobalModule(void)
{
    // -- Active Peer Table
    cModule *temp = simulation.getModuleByPath("activePeerTable");
    m_apTable = check_and_cast<ActivePeerTable *>(temp);
    //if (m_apTable == NULL) throw cException("NULL pointer to module activePeerTable");
    EV << "Binding to activePeerTable is completed successfully" << endl;

    // -- Global Statistic
//    temp = simulation.getModuleByPath("globalStatistic");
//    //m_gstat = check_and_cast<GlobalStatistic *>(temp);
//    m_gstat = check_and_cast<StatisticBase *>(temp);
//    //if (m_gstat == NULL) throw cException("NULL pointer to module globalStatistic");
//    EV << "Binding to globalStatistic is completed successfully" << endl;
}

double CommBase::getSimTimeLimit(void)
{
   string s = ev.getConfig()->getConfigValue("sim-time-limit");
   s = s.erase(s.find('s'));
   double sim_time_limit = atof(s.c_str());

   return sim_time_limit;
}
