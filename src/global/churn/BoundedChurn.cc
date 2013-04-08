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

#include "BoundedChurn.h"

Define_Module(BoundedChurn);

BoundedChurn::BoundedChurn() {
    // TODO Auto-generated constructor stub
}

BoundedChurn::~BoundedChurn() {
    // TODO Auto-generated destructor stub
}

void BoundedChurn::initialize()
{
    // -- Reading parameters
    m_leave = par("leave");
    m_joinStart = par("joinStart");
    m_joinDuration = par("joinDuration");
    m_leaveStart = par("leaveStart");
    m_leaveDuration = par("leaveDuration");

    WATCH(m_leave);
    WATCH(m_joinStart);
    WATCH(m_joinDuration);
    WATCH(m_leaveStart);
    WATCH(m_leaveDuration);
}

void BoundedChurn::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

double BoundedChurn::getArrivalTime()
{
   m_joinTime = uniform(m_joinStart, m_joinStart + m_joinDuration);

   return m_joinTime;
}

double BoundedChurn::getSessionDuration()
{
   if (m_leave == false)
   {
      return (-1.0);
   }
   else
   {
      return uniform(m_leaveStart, m_leaveStart + m_leaveDuration) - m_joinTime;
   }
}

double BoundedChurn::getDepartureTime()
{
   if (m_leave == false)
   {
      return (-1.0);
   }
   else
   {
      return uniform(m_leaveStart, m_leaveStart + m_leaveDuration);
   }
}
