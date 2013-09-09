#ifndef STRIPEGENERATOR_H_
#define STRIPEGENERATOR_H_

#include "VideoBuffer.h"
#include "AppSettingMultitree.h"
#include "VideoChunkPacket_m.h"

class StripeGenerator : public cSimpleModule {
public:
    StripeGenerator();
    virtual ~StripeGenerator();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

    cMessage *timer_newStripe;
    int numStripes;

    long nextSeqNumber;
    int nextStripe;

    // -- Module (secondary) parameters
    double m_interval_newStripe;
    int m_size_stripePacket;

	AppSettingMultitree *m_appSetting;
	//PushForwarder		*m_forwarder;
	VideoBuffer			*m_videoBuffer;

};

#endif /* STRIPEGENERATOR_H_ */
