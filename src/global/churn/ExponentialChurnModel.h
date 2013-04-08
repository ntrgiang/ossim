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
// ExponentialChurnModel.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "IChurnGenerator.h"
#include "NotificationBoard.h"

#ifndef EXPONENTIAL_CHURN_H_
#define EXPONENTIAL_CHURN_H_

//class ExponentialChurnModel : public IChurnGenerator, public cSimpleModule, protected INotifiable
class ExponentialChurnModel : public IChurnGenerator, public cSimpleModule
{
public:
    ExponentialChurnModel();
    virtual ~ExponentialChurnModel();

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

public:
    double getArrivalTime();
    double getSessionDuration();
    double getDepartureTime();

private:
    // -- Parameters
    int param_rng;

    // -- For the Exponential distribution of inter-arrival times
    double param_arrivalRate;

    // -- For the Exponential distribution of session durations
    double param_meanDuration;

    // Accumulated value to referback to the origine
    static double m_absoluteInterval;
};

#endif /* EXPONENTIAL_CHURN_H_ */
