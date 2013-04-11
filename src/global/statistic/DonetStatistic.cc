//
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// DonetStatistic.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "DonetStatistic.h"

Define_Module(DonetStatistic)

DonetStatistic::DonetStatistic() {
    // TODO Auto-generated constructor stub

}

DonetStatistic::~DonetStatistic() {
    // TODO Auto-generated destructor stub
    if (timer_reportCI)
    {
       delete cancelEvent(timer_reportCI);
       timer_reportCI       = NULL;
    }
}

void DonetStatistic::finish()
{
    // -- Close the log file
    m_outFile.close();

//    EV << "Everage size of InView: " << getAverageSizeInView() << endl;
//    EV << "Everage size of PartialView: " << getAverageSizePartialView() << endl;

    // -- Final Size of InViews & PartialViews
//    m_finalSizeIV.record();
//    m_finalSizePV.record();

//    EV << "InView statistics: Max: " << m_finalSizeIV.getMax()
//            << " -- Min: " << m_finalSizeIV.getMin()
//            << " -- Mean: " << m_finalSizeIV.getMean() << endl;

//    EV << "PartialView statistics: Max: " << m_finalSizePV.getMax()
//                << " -- Min: " << m_finalSizePV.getMin()
//                << " -- Mean: " << m_finalSizePV.getMean() << endl;

//    EV << "Work around with subscriptions: " << endl;
//    EV << "NEW: " << m_count_NEW << " -- IGN: " << m_count_IGN << " -- ACK: " << m_count_ACK << endl;

    // -- Record a scalar value
//    EV << "Accumulated PVsize: " << m_accumulatedSizePV << endl;
//    EV << "Number of reported nodes: " << m_totalNode << endl;
//    recordScalar("Average PartialView Size", m_accumulatedSizePV/(double)m_totalNode);

    // -- For the histogram of the partial view sizes
//    m_sizePV.recordAs("histogram of pvSize");

    // -- For the histogram of the partial view sizes
//    m_sizeIV.recordAs("histogram of ivSize");

    // -- For checking the number of new App messages created and deleted
    //EV << "The number of App message left: " << m_countAppMsgNew << endl;
    //EV << "The number of ignored messaged (because of returning to sender: " << m_countSelfAppMsg << endl;

    // -- For calculating and recording reach ratio
//    EV << "The reach ratio (it should approximate the total number of nodes): "
//            << 1 + (double)m_countReach / m_countAppMsgNew << endl;

    recordScalar("Reach Ratio", (double)m_countReach / m_countAppMsgNew);
}

//void DonetStatistic::initialize()
void DonetStatistic::initialize(int stage)
{
    if (stage == 0)
    {
        // -- Performance-critical Signals
        // sig_dummy_chunkHit = registerSignal("Signal_ChunkHit");

        sig_chunkHit    = registerSignal("Signal_ChunkHit");
        sig_chunkMiss   = registerSignal("Signal_ChunkMiss");
        sig_chunkNeed   = registerSignal("Signal_ChunkNeed");

        sig_requestedChunk      = registerSignal("Signal_RequestedChunk");
        sig_receivedChunk       = registerSignal("Signal_ReceivedChunk");
        sig_lateChunk           = registerSignal("Signal_LateChunk");
        sig_inrangeChunk        = registerSignal("Signal_InrangeChunk");
        sig_duplicatedChunk     = registerSignal("Signal_DuplicatedChunk");

        sig_skipChunk         = registerSignal("Signal_SkipChunk");
        sig_rebuffering       = registerSignal("Signal_Rebuffering");
        sig_stallDuration     = registerSignal("Signal_StallDuration");
        sig_ci                = registerSignal("Signal_CI");
        sig_systemSize        = registerSignal("Signal_SystemSize");

        // should be obsolete
        sig_chunkSeek       = registerSignal("Signal_ChunkSeek");

        sig_meshJoin    = registerSignal("Signal_MeshJoin");
        sig_nPartner    = registerSignal("Signal_NumberOfPartner");
        sig_nJoin       = registerSignal("Signal_nJoin");

        timer_reportCI = new cMessage("GLOBAL_STATISTIC_REPORT_CI");
        timer_reportSystemSize = new cMessage("GLOBAL_STATISTIC_REPORT_SYSTEM_SIZE");

        param_interval_reportCI = par("interval_reportCI").doubleValue();
        param_interval_reportSystemSize = par("interval_reportSystemSize");

        m_count_allChunk = 0L;
        m_count_chunkHit = 0L;
        m_count_chunkMiss = 0L;

//        std::vector<cIListener*> localListener;
//        localListener = getLocalSignalListeners(sig_dummy_chunkHit);
//
//        for (std::vector<cIListener*>::iterator iter = localListener.begin(); iter != localListener.end(); ++iter)
//        {
//            simulation.getSystemModule()->subscribe("Signal_ChunkHit", *iter);
//        }
    }

    if (stage != 3)
        return;

    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Binding to Active Peer Table
    cModule *temp = simulation.getModuleByPath("activePeerTable");
    m_apTable = check_and_cast<ActivePeerTable *>(temp);
    //if (m_apTable == NULL) throw cException("NULL pointer to module activePeerTable");
    EV << "Binding to activePeerTable is completed successfully" << endl;

    m_outFile.open(par("gstatLog").stringValue(), fstream::out);
    //m_outFile << "test" << endl;

    scheduleAt(simTime() + param_interval_reportCI, timer_reportCI);
    scheduleAt(simTime() + param_interval_reportSystemSize, timer_reportSystemSize);

//    cNumericResultRecorder *listener = new cNumericResultRecorder;
//    simulation.getSystemModule()->subscribe("chunkHit_Global", listener);

    // -- Initialize count variables
//    m_sizeInView = 0L;
//    m_sizePartialView = 0L;

//    m_countIV = 0L;
//    m_countPV = 0L;

    m_joinTime.setName("JoinTime");

//    m_finalSizeIV.setName("Final Size of InViews");
//    m_finalSizePV.setName("Final Size of PartialViews");

    m_count_NEW = 0L;
    m_count_IGN = 0L;
    m_count_ACK = 0L;

    // -- data to be processed with R
//    m_accumulatedSizePV = 0L;
//    m_totalNode = 0;

    // -- For collecting the sizes of Partial View
    // m_sizePV.setName("pvSize");

    // -- For drawing the histogram of partial view sizes
    m_allFinalPVsizes.setName("allFinalPVsizes");

    // -- For checking the number of new App messages created and deleted
    m_countAppMsgNew = 0L;
    m_countSelfAppMsg = 0L;

    m_countReach = 0L;



    WATCH(m_outFile);
}

void DonetStatistic::handleMessage(cMessage *msg)
{
    //EV << "ActivePeerTable doesn't process messages!" << endl;
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        throw cException("ActivePeerTable doesn't process messages!");
    }
}

void DonetStatistic::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_reportCI)
    {
        collectCI();
        collectSkipChunk();
        collectStallDuration();
        collectRebuffering();

        scheduleAt(simTime() + param_interval_reportCI, timer_reportCI);
    }
    else if (msg == timer_reportSystemSize)
    {
       reportSystemSize();
       scheduleAt(simTime() + param_interval_reportSystemSize, timer_reportSystemSize);
    }
}

void DonetStatistic::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}


// ----------------- Interface in effect -------------------------
void DonetStatistic::writeActivePeerTable2File(vector<IPvXAddress> activePeerList)
{
    m_outFile << "List of active peers" << endl;

    for (vector<IPvXAddress>::iterator iter = activePeerList.begin();
         iter != activePeerList.end(); ++iter)
    {
        m_outFile << *iter << endl;
    }
}

void DonetStatistic::writePartnerList2File(IPvXAddress node, vector<IPvXAddress> pList)
{
   //m_outFile << "---------------------------------------------------------------" << endl;
   //m_outFile << "At " << simTime().dbl() << "(s) " << endl;
   //m_outFile << "Peer " << node << " has " << pList.size() << " partners: " << endl;

    for (vector<IPvXAddress>::iterator iter = pList.begin();
         iter != pList.end(); ++iter)
    {
        //m_outFile << "\t" << *iter << endl;
       m_outFile << node << " " << *iter << endl;
    }
    m_outFile << endl;
}

void DonetStatistic::writePartnership2File(IPvXAddress local, IPvXAddress remote)
{
   m_outFile << simTime().dbl() << " " << local.str() << " " << remote.str() << endl;
}

/**
 * ****************************************************************************
 * Recording the sizes of InView & PartialView
 * ****************************************************************************
 */

//void DonetStatistic::recordSizeInView(int size)
//{
//    m_sizeInView += size;
//    ++m_countIV;
//}

//void DonetStatistic::recordSizePartialView(int size)
//{
//    m_sizePartialView += size;
//    ++m_countPV;
//}

//double DonetStatistic::getAverageSizeInView(void)
//{
//    return (double)m_sizeInView / m_countIV;
//}

//double DonetStatistic::getAverageSizePartialView(void)
//{
//    return (double)m_sizePartialView / m_countPV;
//}



void DonetStatistic::recordJoinTime(double time)
{
    m_joinTime.record(time);
}

/**
 * ****************************************************************************
 * Collect sizes of InView & PartialView
 * ****************************************************************************
 */

void DonetStatistic::collectSizeIV(int size)
{
//    m_finalSizeIV.collect(size);
}

void DonetStatistic::collectSizePV(int size)
{
//    m_finalSizePV.collect(size);
}

/**
 * ****************************************************************************
 * Work-around with counting the subscriptions
 * ****************************************************************************
 */

void DonetStatistic::increaseNEW(void)
{
    ++m_count_NEW;
}

void DonetStatistic::decreaseNEW(void)
{
    --m_count_NEW;
}

void DonetStatistic::increaseIGN(void)
{
    ++m_count_IGN;
}

void DonetStatistic::increaseACK(void)
{
    ++m_count_ACK;
}

long DonetStatistic::getNEW(void)
{
    return m_count_NEW;
}

long DonetStatistic::getIGN(void)
{
    return m_count_IGN;
}

long DonetStatistic::getACK(void)
{
    return m_count_ACK;
}

//void DonetStatistic::recordPartialViewSize(int size)
//{
//    m_accumulatedSizePV += (long)size;
//    ++m_totalNode;
//}

//void DonetStatistic::collectPartialViewSize(int size)
//{
//    m_sizePV.collect(size);
//}

//void DonetStatistic::collectInViewSize(int size)
//{
//    m_sizeIV.collect(size);
//}

void DonetStatistic::collectAllPVsizes(int size)
{
    m_allFinalPVsizes.record(size);
}

// ---------------------- Signal --------------------------
//void DonetStatistic::receiveSignal(cComponent *src, simsignal_t id, long l){
//        if(registerSignal("BeaconTx") == id){
//                int idx = (int) l ;
//                txBeacons[idx]++ ;
//        }
//        else if(registerSignal("AppTx") == id){
//                appTx++ ;
//        }
//        else opp_error("Unknown signal received. 2") ;
//}
//
//void DonetStatistic::receiveSignal(cComponent *src, simsignal_t id, cObject* obj){
//
//        if(registerSignal("BeaconRx") == id){
//                beaconRx* brx = check_and_cast<beaconRx*>(obj) ;
//                rxBeacons[index(brx->id,brx->from,totNodes)]++ ;
//        }
//        else opp_error("Unknown signal.") ;
//}

//void DonetStatistic::reportChunkHit(SEQUENCE_NUMBER_T seq_num)
void DonetStatistic::reportChunkHit(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkHit, seq_num);
    ++m_count_chunkHit;
    ++m_count_allChunk;
}

void DonetStatistic::reportChunkMiss(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkMiss, seq_num);
    emit(sig_chunkNeed, seq_num);
    ++m_count_chunkMiss;
    ++m_count_allChunk;
}

void DonetStatistic::reportChunkSeek(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkSeek, seq_num);
    emit(sig_chunkNeed, seq_num);
//    ++m_count_allChunk;
}

void DonetStatistic::increaseChunkHit(const int &delta)
{
   m_count_chunkHit += delta;
   m_count_allChunk += delta;
}

void DonetStatistic::increaseChunkMiss(const int &delta)
{
   m_count_chunkMiss += delta;
   m_count_allChunk += delta;
}


void DonetStatistic::reportRequestedChunk(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_requestedChunk, seq_num);
}

void DonetStatistic::reportDuplicatedChunk(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_duplicatedChunk, seq_num);
}

void DonetStatistic::reportLateChunk(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_lateChunk, seq_num);
    emit(sig_receivedChunk, seq_num);
}

void DonetStatistic::reportInrangeChunk(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_inrangeChunk, seq_num);
    emit(sig_receivedChunk, seq_num);
}

void DonetStatistic::reportRebuffering(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_rebuffering, seq_num);
}

void DonetStatistic::reportSystemSize()
{
   Enter_Method("reportSystemSize");

   EV << "report system size: " << endl;
   int count = 0;
   for (map<IPvXAddress, int>::iterator iter = m_peerList.begin();
        iter != m_peerList.end(); ++iter)
   {
      EV << iter->second << endl;
      if (iter->second > 0) count++;
   }

   EV << "Number of reported peers: " << m_peerList.size() << endl;
   EV << "Current system size: " << count << endl;
   emit(sig_systemSize, count+1); // plus one for the streaming server
}

void DonetStatistic::printActivePeerList(void)
{
   for (std::map<IPvXAddress, int>::iterator iter = m_peerList.begin();
        iter != m_peerList.end(); ++iter)
   {
      EV << iter->first << " with " << iter->second << " partner(s)" << endl;
   }
}

// should be obsolete
void DonetStatistic::reportStall()
{
    emit(sig_stall, 1);
}

// --------------------------------------------------- ON-GOING ----------------
void DonetStatistic::collectCI(void)
{
   if (m_count_allChunk)
   {
      emit(sig_ci, (long double)m_count_chunkHit/m_count_allChunk);
   }
   else
   {
      emit(sig_ci, 0.0);
   }

}

void DonetStatistic::collectSkipChunk(void)
{
   emit(sig_skipChunk, (long double)m_count_skipChunk / m_apTable->getNumActivePeer());
}

void DonetStatistic::collectStallDuration(void)
{
   emit(sig_stallDuration, (long double)m_count_stallDuration_chunk / m_apTable->getNumActivePeer());
}

void DonetStatistic::collectRebuffering(void)
{
   emit(sig_rebuffering, (long double)m_count_rebuffering / m_apTable->getNumActivePeer());
}

void DonetStatistic::reportSkipChunk()
{
   ++m_count_skipChunk;
}

void DonetStatistic::reportStallDuration(double dur)
{
    m_count_stallDuration += dur;
}

void DonetStatistic::reportStallDuration(void)
{
    ++m_count_stallDuration_chunk;
    // the stall duration will be normalized to the length of a video chunk
}

void DonetStatistic::reportRebuffering()
{
    //emit(sig_rebuffering, 1);
    ++m_count_rebuffering;
}
// -------------------------------------------- END OF ON-GOING ----------------

void DonetStatistic::reportMeshJoin()
{
    emit(sig_meshJoin, 1);
}

void DonetStatistic::reportNumberOfPartner(int nPartner)
{
    emit(sig_nPartner, nPartner);
}

void DonetStatistic::reportNumberOfPartner(const IPvXAddress &addr, const int &nPartner)
{
   Enter_Method("reportNumberOfPartner");

   EV << "Report number of partners" << endl;
   EV << "Peer " << addr << " with " << nPartner << " partners" << endl;
   printActivePeerList();
   EV << endl << endl;

   m_peerList[addr] = nPartner;
    //emit(sig_nPartner, nPartner);

   printActivePeerList();
}

void DonetStatistic::reportNumberOfJoin(int val)
{
    emit(sig_nJoin, val);
}
