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
// ExpParetoChurnModel.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "ExpParetoChurnModel.h"

Define_Module(ExpParetoChurnModel);

double ExpParetoChurnModel::m_absoluteInterval = 0.0;

ExpParetoChurnModel::ExpParetoChurnModel() {
    // TODO Auto-generated constructor stub
}

ExpParetoChurnModel::~ExpParetoChurnModel() {
    // TODO Auto-generated destructor stub
}

void ExpParetoChurnModel::initialize()
{
    // -- Get parameters
    param_rng           = par("rng");
    param_arrivalRate   = par("arrivalRate");
    param_a             = par("a");
    param_b             = par("b");

    WATCH(param_rng);
    WATCH(param_arrivalRate);
    WATCH(param_a);
    WATCH(param_b);
}

void ExpParetoChurnModel::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

double ExpParetoChurnModel::getArrivalTime()
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

double ExpParetoChurnModel::getSessionDuration()
{
    //if (param_a <= 0 || param_b <= 0) return -1.0;
    if (param_a <= 0)
       return -1.0;
//    else if (param_b <= 0)
//       return -1.0;

    m_absoluteInterval = (m_absoluteInterval < 0.0) ? simTime().dbl() : m_absoluteInterval;

    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    //double deltaT = pareto_shifted(param_a, param_b, 0, param_rng);;
    double duration = pareto_shifted(param_a, param_b, 0);

    return duration;
}

double ExpParetoChurnModel::getDepartureTime()
{
   if (m_leave == false)
      return (-1.0);

    if (param_a <= 0 || param_b <= 0) return -1.0;

    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    //double deltaT = pareto_shifted(param_a, param_b, 0, param_rng);;
    double duration = pareto_shifted(param_a, param_b, 0);

    return (duration + m_joinTime);
}
