#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "VideoBuffer.h"
#include "VideoChunkPacket_m.h"
#include "AppSettingDonet.h"
#include "GlobalStatistic.h"

/*
#define PLAYER_STATE_IDLE       0
#define PLAYER_STATE_BUFFERING  1
#define PLAYER_STATE_PLAYING    2
#define PLAYER_STATE_STALLED    3

#define EVENT_CHUNK_NOT_IN_BUFFER   0
#define EVENT_CHUNK_IN_BUFFER       1
*/

class PlayerBase : public cSimpleModule {
//class PlayerBase {
public:
    PlayerBase() {};
    virtual ~PlayerBase() {};

public:
    virtual void activate(void) = 0;
    virtual void startPlayer(void) = 0;
    virtual SEQUENCE_NUMBER_T getCurrentPlaybackPoint(void) = 0;
    virtual bool playerStarted(void) = 0;

    virtual long int getCountChunkHit(void) = 0;
    virtual long int getCountChunkMiss(void) = 0;

    //inline long int getCountChunkHit(void) { return m_countChunkHit; }
    //inline long int getCountChunkMiss(void) { return m_countChunkMiss; }

/*
protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    void handleTimerMessage(cMessage *msg);

private:
    bool shouldResumePlaying(SEQUENCE_NUMBER_T seq_num);

protected:
    // Pointer to other modules
    //DonetPeer *m_

    // -- State variable
    bool m_playerStarted;
    cMessage *timer_nextChunk;
    cMessage *timer_playerStart;
    SEQUENCE_NUMBER_T m_id_nextChunk;

    // -- Module (secondary) parameters
    double m_interval_newChunk;
    double param_interval_recheckVideoBuffer;
    int m_chunkSize; // Bytes

   // -- Some new parameters for the Finite State Machine
    int m_state;
    double param_percent_buffer_low, param_percent_buffer_high;
    int param_max_skipped_chunk;
    double param_interval_probe_playerStart;

    // -- Statistics locally
    long int m_countChunkHit, m_countChunkMiss;

    int m_skip;

    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    AppSettingDonet *m_appSetting;
    GlobalStatistic *m_stat;

    // -- Signal for data collection
    simsignal_t sig_chunkHit;
    simsignal_t sig_chunkMiss;
    simsignal_t sig_chunkSeek;
    simsignal_t sig_rebuffering_local;

    simsignal_t sig_rebuffering;
    simsignal_t sig_stall;
*/
};

#endif /* PLAYER_H_ */
