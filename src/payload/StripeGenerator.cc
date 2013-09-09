#include "StripeGenerator.h"

Define_Module(StripeGenerator)

StripeGenerator::StripeGenerator(){}

StripeGenerator::~StripeGenerator(){}

void StripeGenerator::initialize(int stage)
{
    if (stage == 0)
    {
        //sig_stripeSeqNum = registerSignal("stripeSeqNum");
        //return;
    }

    if (stage != 3)
        return;

    cModule *temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingMultitree *>(temp);

	temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
    if (m_videoBuffer == NULL) throw cException("m_videoBuffer == NULL is invalid");

	numStripes = m_appSetting->getNumStripes();

    nextSeqNumber = 0L;
    nextStripe = 0;
    timer_newStripe      = new cMessage("TIMER_NEW_STRIPE");

    m_interval_newStripe = m_appSetting->getIntervalNewChunk();
    m_size_stripePacket  = m_appSetting->getPacketSizeVideoChunk();

    // -- Schedule the first event for the first stripe
    scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newStripe);
}

void StripeGenerator::finish()
{
    if (timer_newStripe)
		delete cancelEvent(timer_newStripe);
}

void StripeGenerator::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage())
    {
        throw cException("This module does NOT process external messages!");
        return;
    }

    handleTimerMessage(msg);
}

void StripeGenerator::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_newStripe)
    {
        scheduleAt(simTime() + m_interval_newStripe, timer_newStripe);

        VideoStripePacket *stripe = new VideoStripePacket("VIDEO_STRIPE_PACKET");
            stripe->setSeqNumber(nextSeqNumber);
            stripe->setStripe(nextStripe);
            stripe->setTimeStamp(simTime().dbl());           // Set the time-stamp of the stripe to current time
            stripe->setBitLength(m_size_stripePacket * 8);    // convert the stripe size from Bytes --> bits

        ++nextSeqNumber;
        nextStripe= ++nextStripe % numStripes;

		m_videoBuffer->insertPacket(stripe);
    }
    else
    {
        throw cException("Wrong timer!");
    }
}
