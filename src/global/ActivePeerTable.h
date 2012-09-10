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

#include <vector>
#include "IPvXAddress.h"
#include "IInterfaceTable.h"
#include "NotificationBoard.h"

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

    /**
     * Raises an error.
     */
    virtual void handleMessage(cMessage *);

    /**
     * Called by the NotificationBoard whenever a change of a category
     * occurs to which this client has subscribed.
     */
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

public:
    /**
     * For debugging
     */
    virtual void printActivePeerTable() const;

    void setName(std::string s);
    /** @name Routing functions (query the route table) */
    //@{
    /**
     * Checks if a peer, given its address, is an active one the overlay.
     */
    virtual bool isActivePeer(const IPvXAddress& dest) const;

    /** @name Peer address table manipulation */
    //@{

    /**
     * Returns the total number of active peers
     */
    virtual int getNumActivePeer() const;


    /**
     * Adds a peer address to the table. Note that once added, addresses
     * cannot be modified; you must delete and re-add them instead.
     */
//    virtual void addPeerAddress(const IPvXAddress *address);
    virtual void addPeerAddress(const IPvXAddress &address);

    /**
     * Deletes the given peer address from the table.
     * Returns true if the address was deleted correctly, false if it was
     * not in the table.
     */
//    virtual bool deletePeerAddress(const IPvXAddress *address);
    virtual bool deletePeerAddress(const IPvXAddress &address);


    /**
     * Utility function: Returns a vector of /N/ addresses of the active nodes.
     */
    virtual std::vector<IPvXAddress> getNPeer() const;

    /**
     * Utility function: Returns a vector of /N/ addresses of the active nodes.
     */
    virtual std::vector<IPvXAddress> getNPeer(long int N) const;

    /**
     * Utility function: Returns exactly one random address of the active nodes.
     * A wrapper of the getNPeer(long int N)
     */
    //virtual IPvXAddress getARandPeer() const;
    virtual IPvXAddress getARandPeer();

    /**
     * Utility function: Returns a vector of all addresses of the active nodes.
     */
//    virtual std::vector<IPAddress> gatherPeerAddresses() const;
    //@}

protected:
    IInterfaceTable *ift; // cached pointer
    NotificationBoard *nb; // cached pointer

    // local addresses cache (to speed up isLocalAddress())
//    typedef std::set<IPAddress> AddressSet;

    // version1: set (binary tree) to speed up the looking-up of an address (key)
    // But it is difficult to get a /random/ list of N active peers
//        typedef std::set<IPvXAddress> AddressSet;
//        mutable AddressSet activePeerList;

    // version2: vector (or list) to make it easier for accessing the list via indices
    // Using vector is not easier at all (when have to check for existence of an element
    typedef std::vector<IPvXAddress> AddressSet;
    mutable AddressSet activePeerList;

    // just a test comment

    std::string m_name;

    // -- Signal
    simsignal_t sig_size;

};

#endif /* ACTIVEPEERTABLE_H_ */
