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
// AppCommon.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// ------------------------------------------------------------------------------
//


#ifndef APPCOMMON_H_
#define APPCOMMON_H_

#include "IPvXAddress.h"
using namespace std;

// For Timer of type: MESH_PEER_TIMER
#define TIMER_MESH_GET_ARRIVAL_TIME    0
#define TIMER_MESH_START_JOINING       1

#define RVALUE_NO_ERROR 0
#define RVALUE_ERROR    -1

#define __DEBUG__ true

#define FORWARDED_SUBSCRIPTION  1
// #define NEW_SUBSCRIPTION        0

// For messages of type: MESH_PEER_JOIN
/*
#define MESH_PEER_JOIN_REQUEST  0
#define MESH_PEER_JOIN_ACCEPTED 1
*/

//#define MESH_STATE_JOIN_IDLE                    0
//#define MESH_STATE_JOIN_WAITING_ACCEPT          1
//#define MESH_STATE_JOIN_ALL_ACCEPT_RECEIVED     2
//#define MESH_STATE_JOIN_WAITING_ACCEPT_ACK      3

//enum Mesh_State {
//    MESH_STATE_IDLE = 0,
//    MESH_STATE_IDLE_WAITING = 1,
//    MESH_STATE_JOINED = 2,
//    MESH_STATE_JOINED_WAITING = 3
//};

struct PendingPartnershipRequest
{
    IPvXAddress address;
    int port;
    double upBW;
};

typedef long SEQUENCE_NUMBER_T;
typedef double SIM_TIME_T;

typedef long MESSAGE_ID_TYPE;
typedef std::vector<long> MESSAGE_ID_LIST;

struct Link
{
    simtime_t timeStamp;
    int linkType; // 0 -> no link; 1 -> link established
    IPvXAddress root;
    IPvXAddress head;
};

struct Partnership
{
    IPvXAddress address;
    double arrivalTime;
    double joinTime;
    int nPartner;
    double video_startTime;
    SEQUENCE_NUMBER_T head_videoStart;
    SEQUENCE_NUMBER_T begin_videoStart;
    int threshold_videoStart;
    long nChunkRequestReceived;
    long nChunkSent;
    long nBMrecv;
    vector<IPvXAddress> partnerList;
};

// ----------------------- Packet sizes ---------------------------------
// --- Membership Overlay
//#define PACKET_SIZE_MESH_PARTNERSHIP_REQUEST    5
    // 1 byte for Packet type
    // 4 bytes for the upload bandwidth information
//#define PACKET_SIZE_MESH_PARTNERSHIP_ACCEPT     5
    // 1 byte for Packet type
    // 4 bytes for the upload bandwidth information
//#define PACKET_SIZE_MESH_PARTNERSHIP_REJECT     1
    // 1 byte for the Packet type


// --- Mesh Overlay


// --- Tree Overlay


#endif /* APPCOMMON_H_ */
