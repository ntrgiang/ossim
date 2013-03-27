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
// GossipMembershipPacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


cplusplus {{
#include <PeerStreamingPacket_m.h>
#include "IPvXAddress.h"
//#include <string>
}}

class noncobject IPvXAddress;
class PeerStreamingPacket;

enum GossipMembershipPacketType
{
    GOSSIP_SUBSCRIPTION_PT      = 0;
    GOSSIP_SUBS_ACK_PT          = 1;
    GOSSIP_UNSUBS_FROM_PVIEW_PT = 2;
    GOSSIP_UNSUBS_FROM_IVIEW_PT = 3;
    GOSSIP_HEARTBEAT_PT         = 4;
    GOSSIP_APP_PT               = 5;
}

//
// Base class for all messages handled by protocol's modules
//
// @author Giang Nguyen
//

packet GossipMembershipPacket extends PeerStreamingPacket
{
    packetGroup = PACKET_GROUP_GOSSIP_OVERLAY;
    short packetType @getter(getPacketType) @setter(setPacketType) @enum(GossipMembershipPacketType) ;
}

packet GossipSubscriptionPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_SUBSCRIPTION_PT;
    bool fwdSubscription        @getter(isFwdSubscription)      @setter(setFwdSubscription);
    string subscriberAddress    @getter(getSubscriberAddress)   @setter(setSubscriberAddress);
    int subscriberPort          @getter(getSubscriberPort)      @setter(setSubscriberPort);
    // long messageId
}

packet GossipSubsAckPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_SUBS_ACK_PT;
}

packet GossipUnSubsFromPartialViewPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_UNSUBS_FROM_PVIEW_PT;
}

packet GossipUnSubsFromInViewPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_UNSUBS_FROM_IVIEW_PT;
}

packet GossipHeartbeatPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_HEARTBEAT_PT;
}

packet GossipApplicationPacket extends GossipMembershipPacket
{
    packetType = GOSSIP_APP_PT;
    IPvXAddress rootAddress     @getter(getRootAddress)         @setter(setRootAddress);
    long messageId              @getter(getMessageId)           @setter(setMessageId);
    double timeStamp            @getter(getTimeStamp)           @setter(setTimeStamp);
}
