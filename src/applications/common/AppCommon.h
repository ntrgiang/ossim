/*
 * common.h
 *
 *  Created on: Apr 12, 2012
 *      Author: giang
 */

#ifndef APPCOMMON_H_
#define APPCOMMON_H_

#include "IPvXAddress.h"

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
