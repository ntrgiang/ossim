#ifndef __DONETSOURCE_H__
#define __DONETSOURCE_H__

#include "DonetBase.h"
#include "AppCommon.h"

// TODO-Giang if the peer doesn't respond during an interval, remove it from list
class DonetSource : public DonetBase
{
public:
    DonetSource();
	virtual ~DonetSource();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void processPacket(cPacket *pkt);
    void handleTimerMessage(cMessage *msg);
    void handleTimerPartnerlistCleanup(void);

private:
   void processPeerBufferMap(cPacket *pkt);

	// -- Partnership Management
//	void processPartnershipRequest(cPacket *pkt);
	void acceptJoinRequestFromPeer(IPvXAddress &reqPeerAddress, double bw);

	//bool canHaveMorePartner(void);

private:
    // -- Timer
    cMessage *timer_startVideo;
    cMessage *timer_newChunk;

    int totalBytesUploaded;
    int totalBytesDownloaded;

    // -- Signals
    simsignal_t sig_nPartner;

};

#endif

