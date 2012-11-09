#ifndef CHUNKGENERATOR_H_
#define CHUNKGENERATOR_H_

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"

class ChunkGenerator : public cSimpleModule {
public:
    ChunkGenerator();
    virtual ~ChunkGenerator();

protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

protected:
    //VideoChunkPacket *generateNewVideoChunk(SEQUENCE_NUMBER_T seq_num);

protected:
    cMessage *timer_newChunk;
    SEQUENCE_NUMBER_T m_id_newChunk;

    // -- Module (secondary) parameters
    double m_interval_newChunk;
    int m_size_chunkPacket;

    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    AppSettingDonet *m_appSetting;

    // -- Signal for data collection
    simsignal_t sig_chunkSeqNum;

    // -- Objects for aggregated data
    cTimestampedValue m_tsValue;

};

#endif /* CHUNKGENERATOR_H_ */
