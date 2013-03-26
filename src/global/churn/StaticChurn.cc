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
// StaticChurn.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "StaticChurn.h"

Define_Module(StaticChurn);

double StaticChurn::m_absoluteInterval = -1.0;

StaticChurn::StaticChurn() {
    // TODO Auto-generated constructor stub

}

StaticChurn::~StaticChurn() {
    // TODO Auto-generated destructor stub
}

void StaticChurn::initialize()
{
    sig_arrivalTime = registerSignal("arrivalTime");
    sig_sessionDuration = registerSignal("sessionDuration");

    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Reading parameters
    param_arrivalTime       = par("arrivalTime");
    param_sessionDuration   = par("sessionDuration");

    param_lowerBoundAT = par("lowerBoundAT");
    param_upperBoundAT = par("upperBoundAT");

    WATCH(param_arrivalTime);
    WATCH(param_sessionDuration);
    WATCH(param_lowerBoundAT);
    WATCH(param_upperBoundAT);

}
/*
void StaticChurn::initialize(int stage)
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
        }
        else if (stage==1)
        {

        }
        else if (stage==3)
        {

        }
}
*/

void StaticChurn::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void StaticChurn::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

double StaticChurn::getArrivalTime()
{
    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    double arrivalTime = uniform(param_lowerBoundAT, param_upperBoundAT);

    m_absoluteInterval += arrivalTime;

    // -- emitting signal for statistical collection
    emit(sig_arrivalTime, arrivalTime);

    return m_absoluteInterval;
}

double StaticChurn::getSessionDuration()
{
    double sessionDuration = par("sessionDuration").doubleValue();

    // -- emitting signals for statistical collection
    emit(sig_sessionDuration, sessionDuration);

    return sessionDuration;
}
