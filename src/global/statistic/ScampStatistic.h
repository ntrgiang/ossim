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

//#include "IChurnGenerator.h"
//#include "NotificationBoard.h"
//#include "AppCommon.h"
#include "ActivePeerTable.h"
//#include "IPvXAddress.h"
//#include <fstream>
//#include "ccomponent.h"
#include "StatisticBase.h"

#ifndef SCAMP_STATISTIC_H_
#define SCAMP_STATISTIC_H_

//class ActivePeerTable;

//class GlobalStatistic : public cSimpleModule, protected INotifiable
class ScampStatistic : public StatisticBase
{
public:
    ScampStatistic();
    virtual ~ScampStatistic();

protected:
    int numInitStages() const  {return 4;}
    void initialize(int stage);

    void handleMessage(cMessage *msg);
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

    void finish();

// -- Real interfaces
public:
   void reportPvSize(int size);

private:
    NotificationBoard *nb; // cached pointer
    ActivePeerTable *m_apTable; // To get the number of active node for averaging recorded results

    simsignal_t sig_pvSize;

    // -- Calculate the the "reach ratio"
    long m_countReach;

};

#endif /* SCAMP_STATISTIC_H_ */
