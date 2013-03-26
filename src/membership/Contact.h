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
// Contact.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef CONTACT_H_
#define CONTACT_H_

#include "IPvXAddress.h"

class Contact {
public:
    Contact();
    Contact(IPvXAddress addr, int port);
    virtual ~Contact();

    friend class ContactView;

    // -- Getters & Setters
    inline IPvXAddress      getAddress() const              { return m_address; }
    inline void             setAddress(IPvXAddress addr)    { m_address = addr; }

    inline int              getPort() const                 { return m_port; }
    inline void             setPort(int port)               { m_port = port; }

    inline double           getLastHeartBeatTime(void)      { return m_lastHeartbeatTime; }
    inline void             setLastHeartBeatTime(double t)  { m_lastHeartbeatTime = t; }

private:
    IPvXAddress m_address;
    int m_port;

    // -- TODO: should think about another solution which is /cleaner/ & /nicer/
    double m_lastHeartbeatTime;

};

// -----------------------------------------------------------------------------

class ContactView
{
public:
    // -- For debugging purpose
    void print();

    ContactView();
    ContactView(std::string name);
    ContactView(const ContactView& rhs) { *this = rhs; };
    ContactView& operator=(const ContactView& rhs);
    ~ContactView();

    int getViewSize();

    void setName(std::string name);
    //Contact operator[] (int index);

    // Add contact to the view
//    Contact* AddContact(IPvXAddress addr, int port);
    void addContact(IPvXAddress addr, int port);

    // Remove contact from view
    void removeContact(const Contact& contact);

    bool hasContact(IPvXAddress addr, int port);

    bool isEmpty();

    // Locate a contact
//    Contact LocateContact(unsigned int addr, int port = -1);

    // Select a random contact from the list
    Contact getOneRandomContact(void);

    // Clear the list
    void clear();

    // Send a message to the view - actually queues it
    void sendMessage(cMessage* msg);

    const std::vector<Contact>& getContactList(void);

private:
    std::vector<Contact> m_contactList;
//    int m_count;

    // -- To separate between inView vs. partialView
    std::string m_name;
};


#endif /* CONTACT_H_ */
