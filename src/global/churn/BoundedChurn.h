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
// UniformChurn.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "IChurnGenerator.h"
#include "NotificationBoard.h"

#ifndef BOUNDED_CHURN_H_
#define BOUNDED_CHURN_H_

class BoundedChurn : public IChurnGenerator, public cSimpleModule
{
public:
    BoundedChurn();
    virtual ~BoundedChurn();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
//    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

public:
    virtual double getArrivalTime();
    virtual double getSessionDuration();
    virtual double getDepartureTime();

private:
    // -- Parameters
    double m_joinStart;
    double m_joinDuration;
    double m_leaveStart;
    double m_leaveDuration;

};

#endif // BOUNDED_CHURN_H_
