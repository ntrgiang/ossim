/*
 * ActivePeerTable.h
 *
 *  Created on: Mar 30, 2012
 *      Author: giang
 */

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

    void addPeerAddress(const IPvXAddress &address, int maxNOP=0);
    void addSourceAddress(const IPvXAddress &address, int maxNOP=0);
    void removePeerAddress(const IPvXAddress &address);

    void incrementNPartner(const IPvXAddress &addr);
    void decrementNPartner(const IPvXAddress &addr);

//    virtual bool deletePeerAddress(const IPvXAddress *address);
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
    StatisticBase *m_gstat;

    std::string m_name;

    // New stuff from this point
    //map<IPvXAddress, ActivePeerInfo*> m_activePeerList;
    //typedef map<IPvXAddress, int> Type_ActiveList;
    //typedef map<IPvXAddress, ActivePeerInfo*> Type_ActiveList;
    typedef map<IPvXAddress, Struct_ActivePeerInfo> Type_ActiveList;
    Type_ActiveList m_activePeerList;
    vector<IPvXAddress> m_tempList;
//    int m_max_reported_nPartner;
    //map<IPvXAddress, ActivePeerInfo*> m_activePeerList;

    IPvXAddress m_sourceAddress;

    // -- Signal
    simsignal_t sig_size;

    // -- Parameters
    double param_interval_reportSize;

    // Timer
    cMessage *timer_reportSize;

};

#endif /* ACTIVEPEERTABLE_H_ */
