#ifndef PLAYER_H_
#define PLAYER_H_

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"
#include "GlobalStatistic.h"

class Player : public cSimpleModule {
public:
    Player();
    virtual ~Player();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

public:
    void startPlayer(void);

protected:
    cMessage *timer_nextChunk;
    SEQUENCE_NUMBER_T m_id_nextChunk;

    // -- Module (secondary) parameters
    double m_interval_newChunk;
    double param_interval_recheckVideoBuffer;
    int m_chunkSize; // Bytes

    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    AppSettingDonet *m_appSetting;
    GlobalStatistic *m_stat;

    // -- Signal for data collection
    simsignal_t sig_chunkHit;
    simsignal_t sig_chunkMiss;
    simsignal_t sig_chunkSeek;
    simsignal_t sig_rebuffering_local;

    // -- Objects for aggregated data
    cTimestampedValue m_tsValue;

};

#endif /* PLAYER_H_ */
