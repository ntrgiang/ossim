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
// Contact.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "Contact.h"
#include "UDP.h"

Contact::Contact()
{
    m_lastHeartbeatTime = -1.0;
}

Contact::Contact(IPvXAddress addr, int port)
: m_address(addr), m_port(port)
{
}

Contact::~Contact()
{

}

// -----------------------------------------------------------------------------
// ------------------------- ContactView ---------------------------------------
// -----------------------------------------------------------------------------

ContactView::ContactView()
{

}

ContactView::ContactView(std::string name)
{
    m_name = name;
}

ContactView::~ContactView()
{
    //this is important nothingness!!!
}

void ContactView::setName(std::string name)
{
    m_name = name;
}

ContactView& ContactView::operator=(const ContactView& rhs)
{
    // First remove all nodes in this list
    // Clear();
    m_contactList.clear();

    this->m_contactList = rhs.m_contactList;

    return *this;
}

int ContactView::getViewSize()
{
    return m_contactList.size();
}

void ContactView::clear()
{
    m_contactList.clear();
}


//Contact* ContactView::SelectContact(int index)
//{
//    ContactList *cl = m_head;
//    int num = 0;
//
//    while (cl != NULL)
//    {
//        if (num == index) return cl->contact;
//
//        cl = cl->next;
//        num++;
//    }
//    return NULL;
//}

//Contact ContactView::operator [] (int index)
//{
//    if (index >= m_contactList.size() || index < 0)
//        return NULL;
//    //return SelectContact(index);
//    return (m_contactList[index]);
//}

void ContactView::addContact(IPvXAddress addr, int port)
{
    // -- Find to see whether this pair exists in the list
    std::vector<Contact>::iterator iter;
    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
    {
        if (iter->getAddress() != addr)
            continue;
        if (iter->getPort() != port)
            continue;

        // EV << "This (address, port) pair exists." << endl;
        return;
        //break;
    }

    // EV << "The Contact will be added the the list!" << endl;
    m_contactList.push_back(Contact(addr, port));

     //print();
}

/*Contact ContactView::LocateContact(unsigned int addr, int port)
{
    // -- Find to see whether this pair exists in the list
    std::vector<Contact>::iterator iter;
    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
    {
        if (iter->getAddress() != addr)
            continue;
        if (iter->getPort() != port)
            continue;

        break;
    }

    return (*iter);
}*/

void ContactView::removeContact(const Contact& contact)
{
    // -- Find to see whether this pair exists in the list
    std::vector<Contact>::iterator iter;
    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
    {
        if (iter->getAddress() != contact.getAddress())
            continue;
        if (iter->getPort() != contact.getPort())
            continue;

        break;
    }

    if (iter != m_contactList.end())
    {
        m_contactList.erase(iter);
    }
}

Contact ContactView::getOneRandomContact(void)
{
//    int rand_num = rand();
//    rand_num %= m_contactList.size();

    int rand_num = (int)intrand(m_contactList.size());
    return m_contactList[rand_num];

}

//prints out the contacts in the contact view list
void ContactView::print()
{
    // Enter_Method("print()");

    EV << "List " << m_name << " has " << m_contactList.size() << " elements: " << endl;
    std::vector<Contact>::iterator iter;
    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
    {
        EV << "(" << iter->getAddress() << ":" << iter->getPort() << ")" << endl;
    }
}

//sends a message to all the contacts in the contact view
void ContactView::sendMessage(cMessage* msg)
{
//    std::vector<Contact>::iterator iter;
//    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
//    {
//        IPvXAddress destAddress = IPvXAddress(iter->getAddress());
//        int destPort = iter->getPort();
//
//        sendtoUDP();
//    }
//  printf("[-] Entering Send Message\n");
//    ContactList *cl = m_head;
//    while (cl != NULL)
//    {
//        msg->AddContact(cl->contact);
//        cl = cl->next;
//    }
//    Node::Instance()->Auth_send(msg);
}

const std::vector<Contact>& ContactView::getContactList(void)
{
    return m_contactList;
}

bool ContactView::hasContact(IPvXAddress addr, int port)
{
    // -- Find to see whether this pair exists in the list
    std::vector<Contact>::iterator iter;
    for (iter = m_contactList.begin(); iter != m_contactList.end(); ++iter)
    {
        if (iter->getAddress() != addr)
            continue;
        if (iter->getPort() != port)
            continue;

        break;
    }

    if (iter == m_contactList.end())
    {
        return false;
    }

    return true;
}

bool ContactView::isEmpty()
{
    if (m_contactList.size() == 0)
        return true;
    return false;
}
