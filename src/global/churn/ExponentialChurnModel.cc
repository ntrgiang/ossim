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
// ExponentialChurnModel.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "ExponentialChurnModel.h"

Define_Module(ExponentialChurnModel)

double ExponentialChurnModel::m_absoluteInterval = 0.0;
//ExponentialChurnModel::m_absoluteInterval = 0.0;

ExponentialChurnModel::ExponentialChurnModel() {
    // TODO Auto-generated constructor stub
}

ExponentialChurnModel::~ExponentialChurnModel() {
    // TODO Auto-generated destructor stub
}

//void ExponentialChurnModel::initialize(int stage)
void ExponentialChurnModel::initialize()
{
//    sig_arrivalTime = registerSignal("arrivalTime");
//    sig_sessionDuration = registerSignal("sessionDuration");

    // get a pointer to the NotificationBoard module and IInterfaceTable
//    nb = NotificationBoardAccess().get();

//    nb->subscribe(this, NF_INTERFACE_CREATED);
//    nb->subscribe(this, NF_INTERFACE_DELETED);
//    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
//    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
//    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Get parameters
    param_arrivalRate = par("arrivalRate");
    param_meanDuration = par("meanDuration");

    WATCH(param_arrivalRate);
    WATCH(param_meanDuration);
}

void ExponentialChurnModel::handleMessage(cMessage *)
{
    throw cException("ActivePeerTable doesn't process messages!");
}

double ExponentialChurnModel::getArrivalTime()
{
    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    if (param_arrivalRate <= 0)
        throw cException("Invalid value of arrivalRate: %6.2f", param_arrivalRate);

    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    double meanArrivalTime = 1 / param_arrivalRate;
    double deltaT = exponential(meanArrivalTime);

    // -- Accumulate the value into the origine
    m_absoluteInterval += deltaT;

    m_joinTime = m_absoluteInterval;

//    emit(sig_arrivalTime, m_absoluteInterval);

    return m_absoluteInterval;
}

double ExponentialChurnModel::getSessionDuration()
{
    return (exponential(param_meanDuration));
}

double ExponentialChurnModel::getDepartureTime()
{
   if (m_leave == false)
   {
      return (-1.0);
   }
   else
   {
      return (m_joinTime + exponential(param_meanDuration));
   }
}
