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
// ScampBase.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "ScampBase.h"
#include "Contact.h"

#include "DpControlInfo_m.h"

#include <string>

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
    if (m_dispatcher != NULL)
//    if (true)
    {
        m_localPort = m_dispatcher->getLocalPort();
        m_destPort  = m_dispatcher->getDestPort();
    }

    m_c = par("c");

    param_isoCheckInterval      = par("isoCheckInterval");
    param_heartbeatInterval     = par("heartbeatInterval");
    param_appMessageInterval    = par("appMessageInterval");

}

void ScampBase::bindToGlobalModule(void)
{
    CommBase::bindToGlobalModule();

    // -- Churn
    cModule *temp = simulation.getModuleByPath("churnModerator");
    m_churn = check_and_cast<IChurnGenerator *>(temp);

}

void ScampBase::bindToParentModule(void)
{
    // -- Dispatcher
    cModule *temp = getParentModule()->getModuleByRelativePath("dispatcher");
    m_dispatcher = check_and_cast<Dispatcher *>(temp);
    if (m_dispatcher == NULL) throw cException("m_dispatcher == NULL is invalid");
}

void ScampBase::bindToStatisticModule()
{
   Enter_Method("bindToStatisticModule");

   cModule *temp = simulation.getModuleByPath("globalStatistic");
   m_gstat = check_and_cast<ScampStatistic *>(temp);
   EV << "Binding to globalStatistic is completed successfully" << endl;
}

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
        GossipSubscriptionPacket *sub_pkt = check_and_cast<GossipSubscriptionPacket *>(pkt);

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

        handleAckPacket(pkt);
        //delete pkt; pkt = NULL;

        break;
    }
    default:
    {
        break;
    }
    };
}

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


    }
    else
    {
    // -- Forwarding --
        EV << "Subscription will be forwarded!" << endl;

        // -- Update the new type of the Packet
        sub_pkt->setFwdSubscription(true);

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

        }

        // -- Forward other c copies of the Packet
        for (int i = 0; i < m_c; ++i)
        {
           EV << "hi there ************************************************" << endl;

            Contact aRandomContact = m_partialView.getOneRandomContact();
            IPvXAddress destAddress = IPvXAddress(aRandomContact.getAddress());
            int destPort = aRandomContact.getPort();

            GossipSubscriptionPacket *copy_pkt = sub_pkt->dup();
            sendToDispatcher(copy_pkt, m_localPort, destAddress, destPort);

        }
    }

    delete pkt;
}

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

    double random = dblrand();
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
    Enter_Method("subscribe");

    IPvXAddress addressRandPeer;

    // -- Get my own address
    IPvXAddress myAddress = getNodeAddress();
    EV << "Node's own address: " << myAddress << endl;

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

    // -- Add this 'contact' to node's own PartialView as the first entry
    m_partialView.addContact(addressRandPeer, m_destPort);

    GossipSubscriptionPacket *gossip_sub_pkt = new GossipSubscriptionPacket("GOSSIP_SUBSCRIPTION_NEW");
        gossip_sub_pkt->setFwdSubscription(false);
        gossip_sub_pkt->setSubscriberAddress(myAddress.str().c_str());
        gossip_sub_pkt->setSubscriberPort(m_localPort);

    sendToDispatcher(gossip_sub_pkt, m_localPort, addressRandPeer, m_destPort);

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

    } // for

    delete gossipApp_pkt;
}

void ScampBase::handleAppPacket(cPacket *pkt)
{
    GossipApplicationPacket *appPkt = dynamic_cast<GossipApplicationPacket *>(pkt);
    if (appPkt == NULL) throw cException("appPkt == NULL is invalid");
    IPvXAddress rootAddress = appPkt->getRootAddress();
    MESSAGE_ID_TYPE msgId = appPkt->getMessageId();

    if (rootAddress == getNodeAddress())
    {
        delete pkt;
        return;
    }

    int count = 0;
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
        }

    }

    delete pkt;

    // -- For getting an idea of what's going on in logged list:
    m_msgLogger.printNodeList();
}

void ScampBase::finish()
{

}

double ScampBase::getSimDuration(void)
{
    double sime_time_limit;
    std::stringstream ss(ev.getConfig()->getConfigValue("sim-time-limit"));
    ss >> sime_time_limit;

    return sime_time_limit;
}

void ScampBase::handleAckPacket(cPacket *pkt)
{
//    switch(m_state)
//    {
//    case SCAMP_STATE_JOINING:
//    {
        // -- Extract the IP and port of the responder
        DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());

        // -- Add the IP address and the port number of the sender to the InView list
        m_inView.addContact(controlInfo->getSrcAddr(), controlInfo->getSrcPort());

        // m_inView.print();

        // -- Self check to decide whether to update the Active Peer Table
        if (m_active == false)
        {
            m_active = true;
            m_apTable->addAddress(this->getNodeAddress());

            // -- Start the timer for isolation check
            scheduleAt(simTime() + param_isoCheckInterval, timer_isolationCheck);

            m_apTable->printActivePeerTable();
        }

        // -- Update state
//        m_state = SCAMP_STATE_JOINED;

//        break;
//    }
//    default:
//    {
//        throw cException("Invalid message or unsuitable state machine");
//    }
//    } // switch

    delete pkt;
}

//IPvXAddress ScampBase::getARandPeer()
//{
//   // Dummy implementation
//   return m_apTable->getARandPeer();
//}

IPvXAddress ScampBase::getRandomPeer(IPvXAddress address)
{
   // Dummy implementation
   // TODO: implementation should be placed here
   return m_apTable->getARandPeer(address);
}

void ScampBase::addPeerAddress(const IPvXAddress &address, int maxNOP)
{
   // Dummy implementation
   // TODO: implementation should be placed here
   //m_apTable->addAddress(address, maxNOP);
   m_apTable->addAddress(address);
}

void ScampBase::addSourceAddress(const IPvXAddress &address, int maxNOP)
{
   // Dummy implementation
   // TODO: implementation should be placed here
   //m_apTable->addSourceAddress(address, maxNOP);
   m_apTable->addAddress(address);
}

bool ScampBase::deletePeerAddress(const IPvXAddress &address)
{
   // Dummy implementation
   // TODO: implementation should be placed here
   return true;
}

void ScampBase::incrementNPartner(const IPvXAddress &addr)
{
   // TODO: implementation should be placed here
   return;
}

void ScampBase::decrementNPartner(const IPvXAddress &addr)
{
   // TODO: implementation should be placed here
   return;
}
