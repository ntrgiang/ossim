//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "IChurnGenerator.h"
#include "NotificationBoard.h"
#include "AppCommon.h"
//#include "ccomponent.h"

#ifndef GLOBAL_STATISTIC_H_
#define GLOBAL_STATISTIC_H_

//class GlobalStatistic : public cSimpleModule, cListener, protected INotifiable
//class GlobalStatistic : public cSimpleModule, cNumericResultFilter, cNumericResultRecorder, protected INotifiable
class GlobalStatistic : public cSimpleModule, protected INotifiable
{
public:
    GlobalStatistic();
    virtual ~GlobalStatistic();

//    virtual void initialize();
    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);

    virtual void handleMessage(cMessage *msg);
    virtual void receiveChangeNotification(int category, const cPolymorphic *details);

    virtual void finish();

    // -- For cListener (maybe!)
//    virtual void receiveSignal(cComponent *src, simsignal_t id, long l) ;
//    virtual void receiveSignal(cComponent *src, simsignal_t id, cObject* obj)

// -- Real interfaces
public:
    void recordSizeInView(int size);
    void recordSizePartialView(int size);

    double getAverageSizeInView(void);
    double getAverageSizePartialView(void);

    void recordJoinTime(double time);

    void collectSizeIV(int size);
    void collectSizePV(int size);

    void increaseNEW(void);
    void decreaseNEW(void);
    void increaseIGN(void);
    void increaseACK(void);

    long getNEW(void);
    long getIGN(void);
    long getACK(void);

    // -- Interface to get the final average of each run
    void recordPartialViewSize(int size);

    // -- Interface to get the histogram of view sizes
    void collectPartialViewSize(int size);
    void collectInViewSize(int size);

    // -- For drawing the histogram of partial view sizes
    void collectAllPVsizes(int size);

    // -- For checking the number of new App messages created and deleted
    inline void incrementCountAppMsg(void) { ++m_countAppMsgNew; }
    inline void decrementCountAppMsg(void) { --m_countAppMsgNew; }
    inline void incrementCoundSelfMsg(void) { ++m_countSelfAppMsg; }

    // -- "Reach ratio"
    inline void incrementCountReach(void) { ++m_countReach; }

    // -- Interface to emit signal for global statistic collection
    //void reportChunkHit(SEQUENCE_NUMBER_T seq_num);
    void reportChunkHit(const SEQUENCE_NUMBER_T &seq_num);
    void reportChunkMiss(const SEQUENCE_NUMBER_T &seq_num);
    void reportChunkSeek(const SEQUENCE_NUMBER_T &seq_num);
    void reportRebuffering(const SEQUENCE_NUMBER_T &seq_num);

    void reportMeshJoin();

    void reportNumberOfPartner(int nPartner);

private:
    NotificationBoard *nb; // cached pointer

//    long m_sizeInView;
//    long m_countIV;
//    long m_sizePartialView;
//    long m_countPV;

//    cStdDev m_finalSizeIV;
//    cStdDev m_finalSizePV;

    cOutVector m_joinTime;

    // -- Counting subscription messages
    long m_count_NEW;
    long m_count_ACK;
    long m_count_IGN;

    // -- Getting the final average of each run
//    long m_accumulatedSizePV;
//    int m_totalNode;

    // -- Getting the histogram of view sizes
//    cLongHistogram m_sizePV;
//    cLongHistogram m_sizeIV;
//     cStdDev m_sizePV;

    // -- For drawing the histogram of partial view sizes
    cOutVector m_allFinalPVsizes;

    // -- For checking the number of new App messages created and deleted
    long m_countAppMsgNew;
    long m_countSelfAppMsg;

    // -- Calculate the the "reach ratio"
    long m_countReach;

    // -- Signals
    simsignal_t sig_dummy_chunkHit;

    simsignal_t sig_chunkHit;
    simsignal_t sig_chunkMiss;
    simsignal_t sig_chunkNeed;

    simsignal_t sig_chunkSeek;
    simsignal_t sig_rebuffering;

    simsignal_t sig_meshJoin;

    simsignal_t sig_nPartner;
};

#endif /* GLOBAL_STATISTIC_H_ */
