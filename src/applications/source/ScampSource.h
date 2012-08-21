#ifndef __GOSSIP_VIDEO_SOURCE_H__
#define __GOSSIP_VIDEO_SOURCE_H__

#include "ScampBase.h"

// TODO if the peer don't respond during an interval, remove it from list
class ScampSource : public ScampBase
{
public:
//	friend class PeerConnectionThread;
    ScampSource();
	virtual ~ScampSource();

protected:
//	virtual void initialize();
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);
    virtual void finish();

// Helper functions
private:
	//! Print a debug message to the passed ostream, which defaults to clog.
	//   void printDebugMsg(std::string s);
//	virtual void processPacket(cPacket *pkt);
	void handleTimerMessage(cMessage *msg);

// Member data
private:

    SEQUENCE_NUMBER_T m_id_newChunk;

};

#endif
