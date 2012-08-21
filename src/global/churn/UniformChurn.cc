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

#include "UniformChurn.h"

Define_Module(UniformChurn);
//Define_Module(ActivePeerTable);

double UniformChurn::m_absoluteInterval = -1.0;

UniformChurn::UniformChurn() {
    // TODO Auto-generated constructor stub

}

UniformChurn::~UniformChurn() {
    // TODO Auto-generated destructor stub
}

void UniformChurn::initialize()
{
    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Reading parameters
    param_lowerBoundAT = par("lowerBoundAT");
    param_upperBoundAT = par("upperBoundAT");
    param_lowerBoundDT = par("lowerBoundDT");
    param_upperBoundDT = par("upperBoundDT");

}

void UniformChurn::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void UniformChurn::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

double UniformChurn::getArrivalTime()
{
    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    double arrivalTime = uniform(param_lowerBoundAT, param_upperBoundAT);

    m_absoluteInterval += arrivalTime;

    return m_absoluteInterval;
}

double UniformChurn::getSessionDuration()
{
    double duration = uniform(param_lowerBoundDT, param_upperBoundDT);

    return duration;
}
