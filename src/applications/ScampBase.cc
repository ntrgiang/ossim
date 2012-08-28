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

#include "ScampBase.h"
#include "Contact.h"

//#include "UDPControlInfo_m.h"
//#include "CommControlInfo_m.h"
#include "DpControlInfo_m.h"

#include <string>
#include <assert.h>

ScampBase::ScampBase()
//: m_active(false)
{
    // TODO Auto-generated constructor stub

}

ScampBase::~ScampBase() {
    // TODO Auto-generated destructor stub

}

void ScampBase::handleMessage(cMessage* msg)
{

    if (msg->isSelfMessage() == true)
    {
        handleTimerMessage(msg);
    }
    else
    {
//        processPacket(PK(msg));
        processGossipPacket(PK(msg));
        // EV << "External message processed!" << endl;
    }

}



void ScampBase::readParameter(void)
{
    // -- Stand-alone module, based on UDPBaseApp
    //    m_localPort = par("localPort");
    //    m_destPort = par("destPort");

    // -- Module which has to use Dispatcher
//    if (m_dispatcher != NULL)
    if (true)
    {
        m_localPort = m_dispatcher->getLocalPort();
        m_destPort  = m_dispatcher->getDestPort();
    }

    m_c = par("c");

    param_isoCheckInterval      = par("isoCheckInterval");
    param_heartbeatInterval     = par("heartbeatInterval");
    param_appMessageInterval    = par("appMessageInterval");

}

//void ScampBase::bindToGlobalModule(void)
//{
//    // -- Active Peer Table
//    cModule *temp = simulation.getModuleByPath("activePeerTable");
//    m_apTable = check_and_cast<ActivePeerTable *>(temp);
//    assert(m_apTable != NULL);
//
//    // -- Churn
//    temp = simulation.getModuleByPath("churnModerator");
//    m_churn = check_and_cast<IChurnGenerator *>(temp);
//    assert(m_churn != NULL);
//
//    // -- Global Statistic
//    temp = simulation.getModuleByPath("globalStatisic");
//    m_gstat = check_and_cast<GlobalStatistic *>(temp);
//    assert(m_gstat != NULL);
//
//}

void ScampBase::bindToParentModule(void)
{
    // -- Dispatcher
    cModule *temp = getParentModule()->getModuleByRelativePath("dispatcher");
    m_dispatcher = check_and_cast<Dispatcher *>(temp);
    //assert(m_dispatcher != NULL);
    if (m_dispatcher == NULL) throw cException("m_dispatcher == NULL is invalid");
}

// -- Not used anymore
//int ScampBase::getNodeAddress(IPvXAddress &address)
//{
//    IInterfaceTable *inet_ift;
//    inet_ift = InterfaceTableAccess().get();
//
//    if (inet_ift->getNumInterfaces() < 2)
//    {
//        throw cException("not enough number of interfaces at the Video Source");
//        return -1;
//    }
//
//    //    IPvXAddress sourceAddress = (IPvXAddress)inet_ift->getInterface(1)->ipv4Data()->getIPAddress();
//    // IPvXAddress sourceAddress(inet_ift->getInterface(1)->ipv4Data()->getIPAddress());
//
//    address = inet_ift->getInterface(1)->ipv4Data()->getIPAddress();
//    EV << "IP address of the video source: " << address << endl;
//    return 0;
//}

// -- Moved to CommBase
//IPvXAddress ScampBase::getNodeAddress(void)
//{
//    IInterfaceTable *inet_ift;
//    inet_ift = InterfaceTableAccess().get();
//
//    // EV << "Number of interfaces: " << inet_ift->getNumInterfaces() << endl;
//
//    assert(inet_ift->getNumInterfaces() >= 2);
//
//    // EV << "Interface 1: " << inet_ift->getInterface(0)->ipv4Data()->getIPAddress() << endl;
//    // EV << "Interface 2: " << inet_ift->getInterface(1)->ipv4Data()->getIPAddress() << endl;
//
//    IPvXAddress myAddress = (IPvXAddress)inet_ift->getInterface(1)->ipv4Data()->getIPAddress();
//    // EV << "My address is: " << myAddress << endl;
//    // IPvXAddress sourceAddress(inet_ift->getInterface(1)->ipv4Data()->getIPAddress());
//
//    return myAddress;
//}


void ScampBase::processGossipPacket(cPacket *pkt)
{
    GossipMembershipPacket *gossipPkt = dynamic_cast<GossipMembershipPacket *>(pkt);

    switch (gossipPkt->getPacketType())
    {
    case GOSSIP_APP_PT:
    {
        EV << "An App message has been received!" << endl;
        handleAppPacket(pkt);
        break;
    }
    case GOSSIP_HEARTBEAT_PT:
    {
        EV << "A Heartbeat packet has been received!" << endl;
        handleHeartbeat(pkt);
        break;
    }
    case GOSSIP_SUBSCRIPTION_PT:
    {
        EV << "I received a subscription packet!" << endl;

        // -- Cast the packet to the desired packet type
        GossipSubscriptionPacket *sub_pkt = dynamic_cast<GossipSubscriptionPacket *>(pkt);
        if (sub_pkt == NULL)
        {
            EV << "wrong casting to packet of type GossipSubscriptionPacket!" << endl;
            return;
        }
        // -- Get the type of the subscription (new or forwarded)
        if (sub_pkt->isFwdSubscription() == false)
        {
            // ---- Process the packet as the _NEW_ Subscription packet
            handleNewSubscription(pkt);
        }
        else
        {
            // ---- Process the packet as the _forwarded_ Subscription packet
            handleForwardedSubscription(pkt);
        }
        break;
    }
    case GOSSIP_SUBS_ACK_PT:
    {
        EV << "I received an ACK for the subscription packet!" << endl;

        // -- Extract the IP and port of the responder
        DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());

        // -- Add the IP address and the port number of the sender to the InView list
        m_inView.addContact(controlInfo->getSrcAddr(), controlInfo->getSrcPort());

        delete pkt;

        // m_inView.print();

        // -- Self check to decide whether to update the Active Peer Table
        if (m_active == false)
        {
            m_active = true;
            m_apTable->addPeerAddress(this->getNodeAddress());

            // -- Start the timer for isolation check
            scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);

            m_apTable->printActivePeerTable();
        }
        break;
    }
    default:
    {
        break;
    }
    };
}

/*
void ScampBase::handleNewSubscription(cPacket *pkt)
{
    Enter_Method("handleNewSubscription(cPacket*)");

    EV << "----------------------------------------- handleNewSubscription -----------------" << endl;
    EV << "---------------------------------------------------------------------------------" << endl;

    GossipSubscriptionPacket *sub_pkt = dynamic_cast<GossipSubscriptionPacket *>(pkt);
    EV << "Subscriber:: Address: " << sub_pkt->getSubscriberAddress() << " -- Port: " << sub_pkt->getSubscriberPort() << endl;

    //    std::string str_senderAddress = sub_pkt->getSubscriberAddress();
    //    int senderPort = sub_pkt->getSubscriberPort();

    // -- Debug
//    m_partialView.print();

    if (m_partialView.getViewSize() == 0)
    {
        // -- Extract the address and the port
//        Contact *newContact = new Contact(senderAddress, senderPort);
        IPvXAddress subscriberAddress = IPvXAddress(sub_pkt->getSubscriberAddress());
        int subscriberPort = sub_pkt->getSubscriberPort();

        // -- Accept the subscribing node automatically to the inView
        m_inView.addContact(subscriberAddress, subscriberPort);

//        m_partialView.addContact(subscriberAddress, subscriberPort);

        // -- Debug
//        EV << "After adding a new contact to Partial View " << endl;
//        m_partialView.print();

        // -- Prepare a ACK packet for the subscription and send it back to the subscriber
//        GossipSubsAckPacket *ack_pkt = new GossipSubsAckPacket("GOSSIP_SUBSCRIPTION_ACK");
//        sendToDispatcher(ack_pkt, m_localPort, subscriberAddress, subscriberPort);

//        m_gstat->increaseACK();
    }
    else
    {
        EV << "Subscription will be forwarded!" << endl;

        // -- Update the new type of the Packet
        sub_pkt->setFwdSubscription(true);
//        sub_pkt

        // -- Debug
        // m_partialView.print();

        // -- Get the list of contacts in the partialView
        std::vector<Contact> contactList = m_partialView.getContactList();

        EV << "Subscription will be forwarded to " << contactList.size() << " peers" << endl;
        // -- Forward to all contacts in the partialView
        std::vector<Contact>::iterator iter;
        for (iter = contactList.begin(); iter != contactList.end(); ++iter)
        {
            IPvXAddress destAddress = IPvXAddress(iter->getAddress());
            int destPort = iter->getPort();

//            if (destAddress != getNodeAddress())
//            {
            GossipSubscriptionPacket *copy_pkt = sub_pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

            EV << "--- Forwarded to " << destAddress << endl;

            // m_gstat->increaseNEW();
//            }
        }
        m_gstat->decreaseNEW();

        // -- Forward other c copies of the Packet
        for (int i = 0; i < m_c; ++i)
        {
            Contact aRandomContact = m_partialView.getOneRandomContact();
            IPvXAddress destAddress = IPvXAddress(aRandomContact.getAddress());
            int destPort = aRandomContact.getPort();

            GossipSubscriptionPacket *copy_pkt = sub_pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

            // m_gstat->increaseNEW();
        }

        m_gstat->decreaseNEW();

    } // else
}
*/

void ScampBase::handleNewSubscription(cPacket *pkt)
{
    Enter_Method("handleNewSubscription(cPacket*)");

    EV << "----------------------------------------- handleNewSubscription -----------------" << endl;
    EV << "---------------------------------------------------------------------------------" << endl;

    GossipSubscriptionPacket *sub_pkt = dynamic_cast<GossipSubscriptionPacket *>(pkt);
    EV << "Subscriber:: Address: " << sub_pkt->getSubscriberAddress() << " -- Port: " << sub_pkt->getSubscriberPort() << endl;

    // -- Debug
    //    m_partialView.print();

    // -- Extract the address and the port
    IPvXAddress subscriberAddress = IPvXAddress(sub_pkt->getSubscriberAddress());
    int subscriberPort = sub_pkt->getSubscriberPort();

    // -- Accept the subscribing node automatically to the inView
    m_inView.addContact(subscriberAddress, subscriberPort);

    if (m_partialView.getViewSize() == 0)
    {
        // -- add the subscriber to the partialView
        m_partialView.addContact(subscriberAddress, subscriberPort);

        // -- Debug
//        EV << "After adding a new contact to Partial View " << endl;
//        m_partialView.print();

        // -- Prepare a ACK packet for the subscription and send it back to the subscriber
        GossipSubsAckPacket *ack_pkt = new GossipSubsAckPacket("GOSSIP_SUBSCRIPTION_ACK");
        sendToDispatcher(ack_pkt, m_localPort, subscriberAddress, subscriberPort);

//        m_gstat->increaseACK();

    }
    else
    {
    // -- Forwarding --
        EV << "Subscription will be forwarded!" << endl;

        // -- Update the new type of the Packet
        sub_pkt->setFwdSubscription(true);
//        sub_pkt

        // -- Debug
        // m_partialView.print();

        // -- Get the list of contacts in the partialView
        std::vector<Contact> contactList = m_partialView.getContactList();

        EV << "Subscription will be forwarded to " << contactList.size() << " peers" << endl;
        // -- Forward to all contacts in the partialView
        std::vector<Contact>::iterator iter;
        for (iter = contactList.begin(); iter != contactList.end(); ++iter)
        {
            IPvXAddress destAddress = IPvXAddress(iter->getAddress());
            int destPort = iter->getPort();

            GossipSubscriptionPacket *copy_pkt = sub_pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

            EV << "--- Forwarded to " << destAddress << endl;

            // m_gstat->increaseNEW();
        }
        m_gstat->decreaseNEW();

        // -- Forward other c copies of the Packet
        for (int i = 0; i < m_c; ++i)
        {
            Contact aRandomContact = m_partialView.getOneRandomContact();
            IPvXAddress destAddress = IPvXAddress(aRandomContact.getAddress());
            int destPort = aRandomContact.getPort();

            GossipSubscriptionPacket *copy_pkt = sub_pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

            // m_gstat->increaseNEW();
        }
//        m_gstat->decreaseNEW();
    }

    delete pkt;
}

/*
void ScampBase::handleForwardedSubscription(cPacket *pkt)
{
    EV << "---------------------------------------------------------------------------------------" << endl;
    EV << "----------------------------------------- handleForwardedSubscription -----------------" << endl;
    EV << "---------------------------------------------------------------------------------------" << endl;
    // -- Extract the Contact from the received packet
    GossipSubscriptionPacket *sub_pkt = dynamic_cast<GossipSubscriptionPacket *>(pkt);
    IPvXAddress subscriberAddress = sub_pkt->getSubscriberAddress();
    int subscriberPort = sub_pkt->getSubscriberPort();

    EV << "Subscriber:: Address: " << subscriberAddress << " -- Port: " << subscriberPort << endl;

    // -- Debug
    // m_partialView.print();

    // -- Check if the Contact was already in the PartialView
    if (!m_partialView.hasContact(subscriberAddress, subscriberPort))
    {
        // -- if NO --> Calculate the probability

        //probability math
        float probability = 1.0 + m_partialView.getViewSize();
        probability = 1.0 / probability;

        float random = (float)rand() / (float)RAND_MAX;
        if (m_partialView.isEmpty()) probability = 1.0;
        //probability = probability * 100.0;
        //int random = (rand() % 100);
        //if (m_partialView.isEmpty()) probability = 100;

        // -- Debug
        EV << "random = " << random << " -- probability = " << probability << endl;

        // -- Compare the probability with a random number
        if (random < probability)
        {
            // -- if probability satisfies --> keep the Contact, send back the ACK
            m_partialView.addContact(subscriberAddress, subscriberPort);

            // -- Debug
            EV << "After adding a new contact into Partial View: " << endl;
            m_partialView.print();

            // -- Prepare a ACK packet for the subscription and send it back to the subscriber
            GossipSubsAckPacket *ack_pkt = new GossipSubsAckPacket("GOSSIP_SUBSCRIPTION_ACK");
            sendToDispatcher(ack_pkt, m_localPort, subscriberAddress, subscriberPort);

            m_gstat->increaseACK();

        }
        else
        {
            // -- if not satisfies --> forward the Subscription to a random contact
//            for (int i = 0; i<2; ++i)
//            {
            Contact aRandomContact = m_partialView.getOneRandomContact();

//            if (aRandomContact.getAddress() != getNodeAddress())
//            {
                sendToDispatcher(sub_pkt->dup(), m_localPort, aRandomContact.getAddress(), aRandomContact.getPort());
                EV << "Subscription was forwarded to a random contact " << aRandomContact.getAddress() << endl;
//            }
//            }

//            // m_gstat->increaseNEW();
        }

    } // if
    else
    {
        // -- if YES --> do nothing at all

        // -- if not satisfies --> forward the Subscription to a random contact

        // -- Commented since it does not help a lot
//        Contact aRandomContact = m_partialView.getOneRandomContact();
//        if (aRandomContact.getAddress() != subscriberAddress && aRandomContact.getPort() != subscriberPort)
//        {
//            sendToDispatcher(sub_pkt->dup(), m_localPort, aRandomContact.getAddress(), aRandomContact.getPort());
//            EV << "Subscription was forwarded to a random contact " << aRandomContact.getAddress() << endl;
//        }
//        else
//        {
//            m_gstat->increaseIGN();
//        }
        //// m_gstat->increaseNEW();
    }

}
*/

void ScampBase::handleForwardedSubscription(cPacket *pkt)
{
    EV << "---------------------------------------------------------------------------------------" << endl;
    EV << "----------------------------------------- handleForwardedSubscription -----------------" << endl;
    EV << "---------------------------------------------------------------------------------------" << endl;
    // -- Extract the Contact from the received packet
    GossipSubscriptionPacket *sub_pkt = dynamic_cast<GossipSubscriptionPacket *>(pkt);
    IPvXAddress subscriberAddress = sub_pkt->getSubscriberAddress();
    int subscriberPort = sub_pkt->getSubscriberPort();

    EV << "Subscriber:: Address: " << subscriberAddress << " -- Port: " << subscriberPort << endl;

    // -- Debug
    // m_partialView.print();

    // -- Check if the Contact was already in the PartialView
    // -- if NO --> Calculate the probability

    //probability math
    float probability = 1.0 + m_partialView.getViewSize();
    probability = 1.0 / probability;

    float random = (float)rand() / (float)RAND_MAX;
    if (m_partialView.isEmpty()) probability = 1.0;
    //probability = probability * 100.0;
    //int random = (rand() % 100);
    //if (m_partialView.isEmpty()) probability = 100;

    // -- Debug
    EV << "random = " << random << " -- probability = " << probability << endl;

    // -- Compare the probability with a random number
    if (random < probability && !m_partialView.hasContact(subscriberAddress, subscriberPort))
    {
        // -- if probability satisfies --> keep the Contact, send back the ACK
        m_partialView.addContact(subscriberAddress, subscriberPort);

        // -- Debug
        EV << "After adding a new contact into Partial View: " << endl;
        m_partialView.print();

        // -- Prepare a ACK packet for the subscription and send it back to the subscriber
        GossipSubsAckPacket *ack_pkt = new GossipSubsAckPacket("GOSSIP_SUBSCRIPTION_ACK");
        sendToDispatcher(ack_pkt, m_localPort, subscriberAddress, subscriberPort);

//        m_gstat->increaseACK();
    }
    else
    {      // -- if not satisfies --> forward the Subscription to a random contact
        Contact aRandomContact = m_partialView.getOneRandomContact();

        sendToDispatcher(sub_pkt->dup(), m_localPort, aRandomContact.getAddress(), aRandomContact.getPort());
        EV << "Subscription was forwarded to a random contact " << aRandomContact.getAddress() << endl;

    }

    delete pkt;

}


void ScampBase::subscribe(void)
{
    IPvXAddress addressRandPeer;

    // -- Get my own address
    IPvXAddress myAddress = getNodeAddress();

    if (m_apTable->size() == 1)
    {
        // -- Get a random peer address from the active list
        addressRandPeer = m_apTable->getARandPeer();

        if (addressRandPeer == myAddress) return;
    }
    else if (m_apTable->size() > 1)
    {
        do
        {
            // -- Get a random peer address from the active list
            addressRandPeer = m_apTable->getARandPeer();

        } while (addressRandPeer == myAddress);
    }

    // -- Add this 'contact' to node's own PartialView
    m_partialView.addContact(addressRandPeer, m_destPort);

    GossipSubscriptionPacket *gossip_sub_pkt = new GossipSubscriptionPacket("GOSSIP_SUBSCRIPTION_NEW");
        gossip_sub_pkt->setFwdSubscription(false);
        gossip_sub_pkt->setSubscriberAddress(myAddress.str().c_str());
        gossip_sub_pkt->setSubscriberPort(m_localPort);

    sendToDispatcher(gossip_sub_pkt, m_localPort, addressRandPeer, m_destPort);

    // m_gstat->increaseNEW();
}

void ScampBase::unsubscribe(void)
{
    // -- Inform all nodes in the PartialView
    // -- Create an unscription message
    GossipUnSubsFromInViewPacket *unsubs_iv_pkt = new GossipUnSubsFromInViewPacket("GOSSIP_UNSUBSCRIPTION_FROM_IVIEW");

    // -- Get the list of contacts in the partialView
    std::vector<Contact> contactList = m_partialView.getContactList();

    // -- Send the Unscription message to all contacts in the partialView

    std::vector<Contact>::iterator iter;
    for (iter = contactList.begin(); iter != contactList.end(); ++iter)
    {
        IPvXAddress destAddress = IPvXAddress(iter->getAddress());
        int destPort = iter->getPort();

        GossipUnSubsFromInViewPacket *copy_pkt = unsubs_iv_pkt->dup();
        sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);
    }

    // -- Inform all nodes in the InView
    GossipUnSubsFromPartialViewPacket *unsubs_pv_pkt = new GossipUnSubsFromPartialViewPacket("GOSSIP_UNSUBSCRIPTION_FROM_PVIEW");

    contactList = m_inView.getContactList();
    for (iter = contactList.begin(); iter != contactList.end(); ++iter)
    {
        IPvXAddress destAddress = IPvXAddress(iter->getAddress());
        int destPort = iter->getPort();

        GossipUnSubsFromPartialViewPacket *copy_pkt = unsubs_pv_pkt->dup();
        sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);
    }

    // Remove itself from the active list
    m_apTable->deletePeerAddress(getNodeAddress());

    delete unsubs_iv_pkt;
    delete unsubs_pv_pkt;

}

void ScampBase::resubscribe(void)
{
    subscribe();
}

void ScampBase::checkIsolation(void)
{
    if (isIsolated())
    {
        resubscribe();
    }
}

bool ScampBase::isIsolated(void)
{
    // -- If the InView is empty --> isolated
    if (m_inView.getViewSize() <= 0)
        return true;

    return false;
}

void ScampBase::handleHeartbeat(cPacket *pkt)
{
    //delete pkt;
}

void ScampBase::sendGossipAppMessage()
{
    if (m_partialView.getViewSize() <= 0)
        return;

    ++m_messageId;
    GossipApplicationPacket *gossipApp_pkt = new GossipApplicationPacket("GOSSIP_APP_MESSAGE");
        gossipApp_pkt->setRootAddress(getNodeAddress());
        gossipApp_pkt->setMessageId(m_messageId);
        gossipApp_pkt->setTimeStamp(simTime().dbl());
    m_gstat->incrementCountAppMsg();

    // -- Create an entry in the log for itself


    std::vector<Contact> pvList = m_partialView.getContactList();

    EV << "Application message will be forwarded to " << pvList.size() << " peers" << endl;
    // -- Forward to all contacts in the partialView
    std::vector<Contact>::iterator iter;
    for (iter = pvList.begin(); iter != pvList.end(); ++iter)
    {
        IPvXAddress destAddress = IPvXAddress(iter->getAddress());
        int destPort = iter->getPort();

        GossipApplicationPacket *copy_pkt = gossipApp_pkt->dup();
        sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

        EV << "--- sent to " << destAddress << endl;

//        m_gstat->incrementCountAppMsg();
    } // for

    delete gossipApp_pkt;
}

void ScampBase::handleAppPacket(cPacket *pkt)
{
    GossipApplicationPacket *appPkt = dynamic_cast<GossipApplicationPacket *>(pkt);
    //assert(appPkt != NULL);
    if (appPkt == NULL) throw cException("appPkt == NULL is invalid");
    IPvXAddress rootAddress = appPkt->getRootAddress();
    MESSAGE_ID_TYPE msgId = appPkt->getMessageId();

    if (rootAddress == getNodeAddress())
    {
        delete pkt;
//        m_gstat->incrementCoundSelfMsg();
        return;
    }

    int count = 0;
//    if (m_msgLogger.isUniqueMessage(pkt, count) == true)
//    if (m_msgLogger.isUniqueMessage(appPkt, count) == true)
    if (m_msgLogger.isUniqueMessage(rootAddress, msgId, count) == true)
    {
        // -- Get the list of peers from PartialView
        std::vector<Contact> contactList = m_partialView.getContactList();
        std::vector<Contact>::iterator iter;
        for (iter = contactList.begin(); iter != contactList.end(); ++iter)
        {
            IPvXAddress destAddress = IPvXAddress(iter->getAddress());
            int destPort = iter->getPort();

            cPacket *copy_pkt = pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

//            m_gstat->incrementCountAppMsg();
        }

        m_gstat->incrementCountReach();
    }

    delete pkt;

    // -- For getting an idea of what's going on in logged list:
    m_msgLogger.printNodeList();
}

void ScampBase::finish()
{

}


//void ScampBase::sendToDispatcher(cPacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort)
//{
//    DpControlInfo *ctrl = new DpControlInfo();
//    ctrl->setSrcPort(srcPort);
//    ctrl->setDestAddr(destAddr);
//    ctrl->setDestPort(destPort);
//    pkt->setControlInfo(ctrl);
//
//    EV << "Sending packet: ";
//    printPacket(pkt);
//
//    EV << "Packet size: " << sizeof(*pkt) << endl;;
//
//    send(pkt, "dpOut");
//}
//
//void ScampBase::printPacket(cPacket *pkt)
//{
//    DpControlInfo *ctrl = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
//
//    IPvXAddress srcAddr = ctrl->getSrcAddr();
//    IPvXAddress destAddr = ctrl->getDestAddr();
//    int srcPort = ctrl->getSrcPort();
//    int destPort = ctrl->getDestPort();
//
//    ev  << pkt << "  (" << pkt->getByteLength() << " bytes)" << endl;
//    ev  << srcAddr << " :" << srcPort << " --> " << destAddr << ":" << destPort << endl;
//}
//
