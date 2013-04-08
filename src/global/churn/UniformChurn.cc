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
// UniformChurn.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "UniformChurn.h"

Define_Module(UniformChurn);

double UniformChurn::m_absoluteInterval = -1.0;

UniformChurn::UniformChurn() {
    // TODO Auto-generated constructor stub
}

UniformChurn::~UniformChurn() {
    // TODO Auto-generated destructor stub
}

void UniformChurn::initialize()
{
    // -- Reading parameters
    m_leave = par("leave");
    param_lowerBoundAT = par("lowerBoundAT");
    param_upperBoundAT = par("upperBoundAT");
    param_lowerBoundDT = par("lowerBoundDT");
    param_upperBoundDT = par("upperBoundDT");

    WATCH(m_leave);
    WATCH(param_lowerBoundAT);
    WATCH(param_upperBoundAT);
    WATCH(param_lowerBoundDT);
    WATCH(param_upperBoundDT);
}

void UniformChurn::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

double UniformChurn::getArrivalTime()
{
    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    double arrivalTime = uniform(param_lowerBoundAT, param_upperBoundAT);

    m_absoluteInterval += arrivalTime;

    m_joinTime = m_absoluteInterval;

    return m_absoluteInterval;
}

double UniformChurn::getSessionDuration()
{
   if (m_leave == false)
   {
      return (-1.0);
   }
   else
   {
      return (uniform(param_lowerBoundDT, param_upperBoundDT));
   }
}

double UniformChurn::getDepartureTime()
{
   if (m_leave == false)
   {
      return (-1.0);
   }
   else
   {
      return (uniform(param_lowerBoundDT, param_upperBoundDT) + m_joinTime);
   }

}
