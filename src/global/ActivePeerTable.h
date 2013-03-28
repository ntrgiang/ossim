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
// ActivePeerTable.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

//TODO: it might be nicer to implement this table as a vector instead of a set (for easier accessing elements using indices)

#ifndef ACTIVEPEERTABLE_H_
#define ACTIVEPEERTABLE_H_

#define LIST_SIZE 5

//#include <vector>
//#include <map>
#include "IPvXAddress.h"
#include "IInterfaceTable.h"
#include "NotificationBoard.h"

//#include "GlobalStatistic.h"
#include "StatisticBase.h"
#include "ActivePeerInfo.h"

#define PARTNER_ASSIGNMENT_IMMEDIATE 0
#define PARTNER_ASSIGNMENT_REQUEST_REPLY 1
//#define PARTNER_ASSIGNMENT PARTNER_ASSIGNMENT_IMMEDIATE
#define PARTNER_ASSIGNMENT PARTNER_ASSIGNMENT_REQUEST_REPLY

using namespace std;

class GlobalStatistic;

class ActivePeerTable: public cSimpleModule, protected INotifiable
{
protected:
    // displays summary above the icon
    virtual void updateDisplayString();

public:
    ActivePeerTable();
    virtual ~ActivePeerTable();

protected:
    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *);

    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

public:
    void printActivePeerTable();

    //void setName(std::string s);
    //virtual bool isActivePeer(const IPvXAddress& dest);

    /** @name Peer address table manipulation */
    //@{
    int getNumActivePeer() const;

//    virtual void addPeerAddress(const IPvXAddress *address);

    // -- Temporary, for Scamp
//    void addPeerAddress(const IPvXAddress &address);
//    void addSourceAddress(const IPvXAddress &address);

    void addAddress(const IPvXAddress &address);
    void removeAddress(const IPvXAddress &address);

// -- will be removed
//    void addPeerAddress(const IPvXAddress &address, int maxNOP=0);
//    void addSourceAddress(const IPvXAddress &address, int maxNOP=0);
//    void removePeerAddress(const IPvXAddress &address);

// -- will be moved to Dummy module
//    void incrementNPartner(const IPvXAddress &addr);
//    void decrementNPartner(const IPvXAddress &addr);

//    virtual bool deletePeerAddress(const IPvXAddress *address);

// -- keep or not?
    bool deletePeerAddress(const IPvXAddress &address);

    void printActivePeerInfo(const IPvXAddress &address);

//    virtual std::vector<IPvXAddress> getNPeer() const;

//    virtual std::vector<IPvXAddress> getNPeer(long int N) const;

    IPvXAddress getARandPeer();
    IPvXAddress getARandPeer(IPvXAddress address);

    vector<IPvXAddress> getListActivePeer(void);
//    virtual std::vector<IPAddress> gatherPeerAddresses() const;

   void handleTimerMessage(cMessage *msg);

    //@}

protected:
    IInterfaceTable *ift; // cached pointer
    NotificationBoard *nb; // cached pointer

    typedef std::vector<IPvXAddress> AddressSet;
    //mutable AddressSet activePeerList;

    //GlobalStatistic *m_gstat;
    //StatisticBase *m_gstat;

    std::string m_name;

    // New stuff from this point
    typedef map<IPvXAddress, Struct_ActivePeerInfo> Type_ActiveList;
    Type_ActiveList m_activePeerList;
    vector<IPvXAddress> m_tempList;
    // int m_max_reported_nPartner;
    // map<IPvXAddress, ActivePeerInfo*> m_activePeerList;

    IPvXAddress m_sourceAddress;

    // -- Signal
    simsignal_t sig_size;

    // -- Parameters
    double param_interval_reportSize;

    // Timer
    cMessage *timer_reportSize;

};

#endif /* ACTIVEPEERTABLE_H_ */
