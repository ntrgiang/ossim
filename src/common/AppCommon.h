/*
 * common.h
 *
 *  Created on: Apr 12, 2012
 *      Author: giang
 */

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
