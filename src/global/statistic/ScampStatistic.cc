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

#include "ScampStatistic.h"

Define_Module(ScampStatistic);

ScampStatistic::ScampStatistic() {
    // TODO Auto-generated constructor stub
}

ScampStatistic::~ScampStatistic() {
    // TODO Auto-generated destructor stub
}

void ScampStatistic::initialize(int stage)
{
    if (stage == 0)
    {
       sig_pvSize = registerSignal("Signal_pvSize");
    }

    if (stage != 3)
        return;

    // get a pointer to the NotificationBoard module and IInterfaceTable
//    nb = NotificationBoardAccess().get();

//    nb->subscribe(this, NF_INTERFACE_CREATED);
//    nb->subscribe(this, NF_INTERFACE_DELETED);
//    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
//    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
//    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Binding to Active Peer Table
    cModule *temp = simulation.getModuleByPath("activePeerTable");
    m_apTable = check_and_cast<ActivePeerTable *>(temp);
    //if (m_apTable == NULL) throw cException("NULL pointer to module activePeerTable");
    EV << "Binding to activePeerTable is completed successfully" << endl;

    // -- For checking the number of new App messages created and deleted
    m_countReach = 0L;

}

void ScampStatistic::handleMessage(cMessage *msg)
{
   throw cException("ActivePeerTable doesn't process messages!");
}

void ScampStatistic::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

void ScampStatistic::finish()
{

}

void ScampStatistic::reportPvSize(int size)
{
   emit(sig_pvSize, size);
}
