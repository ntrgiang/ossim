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

#ifndef UNIFORM_CHURN_H_
#define UNIFORM_CHURN_H_

class UniformChurn : public IChurnGenerator, public cSimpleModule, protected INotifiable
{
public:
    UniformChurn();
    virtual ~UniformChurn();

    virtual void initialize();
//    virtual void initialize(int stage);
//    virtual int numInitStages() {return 4;}
    virtual void handleMessage(cMessage *msg);
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

public:
    virtual double getArrivalTime();
    virtual double getSessionDuration();
private:
    NotificationBoard *nb; // cached pointer

    // -- Parameters
    double param_lowerBoundAT;
    double param_upperBoundAT;
    double param_lowerBoundDT;
    double param_upperBoundDT;

    static double m_absoluteInterval;

};

#endif /* UNIFORM_CHURN_H_ */
