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
// ScampPeer.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef SCAMPPEER_H_
#define SCAMPPEER_H_

#define __DONET_PEER_DEBUG__ true

#include <UDPBasicApp.h>
#include "IChurnGenerator.h"

#include "ActivePeerTable.h"
#include "GossipMembershipPacket_m.h"
//#include "PartnerList.h"
//#include "VideoBuffer.h"

#include "Contact.h"
#include "ScampBase.h"

class ScampPeer : public ScampBase {
public:
    /** @name Constructor, Destructor */
    // @{
    /**
     * Constructor
     */
    ScampPeer();

    /*
     * Destructor
     */
    virtual ~ScampPeer();
    //@}

    /** @name Redefined cSimpleModule functions */
    // @{
    /*
     * This function set the number of stages to the value of 4
     */
    virtual int numInitStages() const { return 5; }

    //    virtual void initialize();
    /*
     * Initialize.
     */
    virtual void initialize(int stage);

protected:
    /**
     *
     */
    virtual void finish();

    /*
     * Main function to distribute self- and external messages
     * - self message will be processed in handleTimerMessage()
     * - messages from other modules will be processed in processPacket()
     */
     // virtual void handleMessage(cMessage *msg);
    // @}


// Helper functions
private:
    /** @name Redefine UDPBasicApp function */
    // @{
    /*
     * This function processes messages/packets from external modules
     */
//    virtual void processPacket(cPacket *pkt);
    // @}
    /** */
    /*
     * This function processes Timer messages
     */
    void handleTimerMessage(cMessage *msg);

    void handleExternalMessage(cMessage *msg);

    /*
     * This method starts the join process:
     * - Get random list of bootstrap partners
     * - Send them the join request
     */
    void join();

//    void processGossipPacket(cPacket *pkt);
//    void handleNewSubscription(cPacket *pkt);
//    void handleFwdSubscription(cPacket *pkt);

//    void unsubscribe(void);

private:
    // -- Helper data members for /DEBUGGING/ data collection
    bool param_moduleDebug;

    // -- Timers
    cMessage *timer_getJoinTime;
    cMessage *timer_join;
//    cMessage *timer_heartBeat;

private:
    // -- State variables
    // bool m_active;


//    cLongHistogram stat_pvSize;
//    cLongHistogram stat_ivSize;

};

#endif /* SCAMPPEER_H_ */
