#ifndef APPSETTINGMULTITREE_H_
#define APPSETTINGMULTITREE_H_

#include "AppSetting.h"

class AppSettingMultitree : public AppSetting {
public:
    AppSettingMultitree(void);
    virtual ~AppSettingMultitree(void);

   inline unsigned int getNumStripes(void){ return param_numStripes; }

   virtual inline int getBufferMapSizeChunk(void) { return param_bufferMapSizeChunk; }
   virtual inline double getIntervalNewChunk(void) { return param_intervalNewChunk; }
   virtual inline int getChunkSize(void) { return param_chunkSize; }
   virtual inline int getPacketSizeVideoChunk(void) { return param_chunkSize + 8; } // 4 for seq_num, 4 for stripe_num
   inline int getVideoStreamBitRate(void) { return param_videoStreamBitRate; }


protected:
    void handleMessage(cMessage* msg);
    virtual void initialize(void);
    virtual void finish(void);

    unsigned int param_numStripes;

    int param_chunkSize;
    int param_videoStreamBitRate;

    int param_bufferMapSizeChunk;
    double param_intervalNewChunk;

    int param_bufferMapSize_seconds;
    int m_videoStreamChunkRate;
};

#endif /* APPSETTINGMULTITREE_H_ */
