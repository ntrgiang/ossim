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
// VelosoChurnModel.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "VelosoChurnModel.h"

Define_Module(VelosoChurnModel);

double VelosoChurnModel::m_absoluteInterval = 0.0;

VelosoChurnModel::VelosoChurnModel() {
    // TODO Auto-generated constructor stub
}

VelosoChurnModel::~VelosoChurnModel() {
    // TODO Auto-generated destructor stub
}

void VelosoChurnModel::initialize()
{
    // -- Get parameters
    param_rng       = par("rng");
    param_a         = par("a");
    param_b         = par("b");
    param_mu        = par("mu");
    param_lambda    = par("lambda");

    WATCH(param_rng);
    WATCH(param_a);
    WATCH(param_b);
    WATCH(param_mu);
    WATCH(param_lambda);
}

void VelosoChurnModel::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

double VelosoChurnModel::getArrivalTime()
{
    if (param_a <= 0 || param_b <= 0) return -1.0;

    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    //double deltaT = pareto_shifted(param_a, param_b, 0, param_rng);;
    double deltaT = pareto_shifted(param_a, param_b, 0);

    // -- Accumulate the value into the origine
    m_absoluteInterval += deltaT;

    m_joinTime = m_absoluteInterval;

    return m_absoluteInterval;
}

double VelosoChurnModel::getSessionDuration()
{
    double mean = exp(param_mu + 0.5 * param_lambda * param_lambda);
    double variance =  (exp(param_lambda * param_lambda) - 1) * (exp(2 * param_mu + param_lambda * param_lambda));

    if (variance < 0) return -1.0;

    double stddev = sqrt(variance);

    //double duration = lognormal(mean, stddev, param_rng);
    double duration = lognormal(mean, stddev);

    return duration;
}

double VelosoChurnModel::getDepartureTime()
{
   if (m_leave == false)
      return (-1.0);

    double mean = exp(param_mu + 0.5 * param_lambda * param_lambda);
    double variance =  (exp(param_lambda * param_lambda) - 1) * (exp(2 * param_mu + param_lambda * param_lambda));

    if (variance < 0) return -1.0;

    double stddev = sqrt(variance);

    return (lognormal(mean, stddev) + m_joinTime);
}
