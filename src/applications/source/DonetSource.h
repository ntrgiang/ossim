#ifndef __DONETSOURCE_H__
#define __DONETSOURCE_H__

#include "DonetBase.h"
#include "AppCommon.h"
//#include "ActivePeerTable.h"
//#include "BufferMap.h"

// TODO if the peer doesn't respond during an interval, remove it from list
class DonetSource : public DonetBase
{
public:
    DonetSource();
	virtual ~DonetSource();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

// Helper functions
private:
	virtual void processPacket(cPacket *pkt);
	void handleTimerMessage(cMessage *msg);

	// -- Partnership Management
	void processPartnershipRequest(cPacket *pkt);
//	void acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress);
	void acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress, double bw);
//	void sendBufferMap();
	bool canHaveMorePartner(void);

// Member data
private:
    // -- Timer
    cMessage *timer_startVideo;
    cMessage *timer_newChunk;
//    MeshBufferMapPacket *m_bmPacket;

    int totalBytesUploaded;
    int totalBytesDownloaded;

    // new ones
//    SEQUENCE_NUMBER_T m_id_bmStart; // smallest id in the BM
//    SEQUENCE_NUMBER_T m_id_bmEnd; // biggest id in the BM
    SEQUENCE_NUMBER_T m_id_newChunk;

//   BufferMap *m_bufferMap;

   // Timer, getting value from parameters or calculated from parameters
   double m_newChunkTimerInterval;

};

#endif

