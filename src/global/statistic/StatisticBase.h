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
// StatisticBase.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//


//#include "IChurnGenerator.h"
#include "NotificationBoard.h"
//#include "AppCommon.h"
//#include "ActivePeerTable.h"
//#include "IPvXAddress.h"
//#include <fstream>

#ifndef STATISTIC_BASE_H_
#define STATISTIC_BASE_H_

//class ActivePeerTable;

class StatisticBase : public cSimpleModule, protected INotifiable
{
public:
//    StatisticBase();
    virtual ~StatisticBase() {};

protected:
//    virtual int numInitStages() const  {return 4;}
//    virtual void initialize(int stage);

//    virtual void handleMessage(cMessage *msg) = 0;
//    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

//    virtual void finish() = 0;

};

#endif /* STATISTIC_BASE_H_ */
