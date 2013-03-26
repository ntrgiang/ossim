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
// PoissonChurnModel.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "PoissonChurnModel.h"

Define_Module(PoissonChurnModel);

double PoissonChurnModel::m_absoluteInterval = 0.0;

PoissonChurnModel::PoissonChurnModel() {
    // TODO Auto-generated constructor stub

}

PoissonChurnModel::~PoissonChurnModel() {
    // TODO Auto-generated destructor stub
}

//void PoissonChurnModel::initialize(int stage)
void PoissonChurnModel::initialize()
{
    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Get parameters
    param_rng               = par("rng");
    param_arrivalRate       = par("arrivalRate");
    param_meanSessionTime   = par("meanSessionTime");
}

void PoissonChurnModel::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void PoissonChurnModel::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

double PoissonChurnModel::getArrivalTime()
{
    if (param_arrivalRate == 0) return -1;

    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    double deltaT = exponential(1/param_arrivalRate, param_rng);

    // -- Accumulate the value into the origine
    m_absoluteInterval += deltaT;

    return m_absoluteInterval;
}

double PoissonChurnModel::getSessionDuration()
{
    double deltaT = exponential(param_meanSessionTime, param_rng);

    return deltaT;
}
