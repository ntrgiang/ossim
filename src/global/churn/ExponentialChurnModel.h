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

#include "IChurnGenerator.h"
#include "NotificationBoard.h"

#ifndef EXPONENTIAL_CHURN_H_
#define EXPONENTIAL_CHURN_H_

class ExponentialChurnModel : public IChurnGenerator, public cSimpleModule, protected INotifiable
{
public:
    ExponentialChurnModel();
    virtual ~ExponentialChurnModel();

    virtual void initialize();
//    virtual void initialize(int stage);
//    virtual int numInitStages() {return 4;}
    virtual void handleMessage(cMessage *msg);
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

public:
    double getArrivalTime();
    double getSessionDuration();

private:
    NotificationBoard *nb; // cached pointer

    // -- Parameters
    int param_rng;

    // -- For the Exponential distribution of inter-arrival times
    double param_arrivalRate;

    // -- For the Exponential distribution of session durations
    double param_meanDuration;

    // Accumulated value to referback to the origine
    static double m_absoluteInterval;

    // -- Signals
    simsignal_t sig_arrivalTime;
    simsignal_t sig_sessionDuration;
};

#endif /* EXPONENTIAL_CHURN_H_ */
