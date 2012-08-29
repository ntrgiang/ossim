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

#include "Player.h"

Define_Module(Player);

Player::Player() {
    // TODO Auto-generated constructor stub
}

Player::~Player() {
    // TODO Auto-generated destructor stub
}

void Player::initialize(int stage)
{
    if (stage == 0)
    {
        sig_chunkHit            = registerSignal("Signal_ChunkHit");
        sig_chunkMiss           = registerSignal("Signal_ChunkMiss");
        sig_rebuffering_local   = registerSignal("rebuffering_Local");
        return;
    }

    if (stage != 3)
        return;

    // -- pointing to the Video Buffer
    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
    if (!m_videoBuffer) throw cException("Null pointer for the VideoBuffer module");

    temp = simulation.getModuleByPath("appSetting");
    m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
    if (!m_videoBuffer) throw cException("Null pointer for the AppSetting module");

    temp = simulation.getModuleByPath("globalStatistic");
    m_stat = dynamic_cast<GlobalStatistic *>(temp);
    if (!m_stat) throw cException("Null pointer for the GlobalStatistic module");

    timer_nextChunk      = new cMessage("MESH_SOURCE_TIMER_NEW_CHUNK");

    param_interval_recheckVideoBuffer = par("interval_recheckVideoBuffer");

    m_interval_newChunk = m_appSetting->getIntervalNewChunk();
    m_chunkSize  = m_appSetting->getChunkSize();

    // -- Schedule the first event for the first chunk
//    scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newChunk);

    WATCH(m_videoBuffer);
    WATCH(m_appSetting);
    WATCH(m_chunkSize);
    WATCH(m_interval_newChunk);
}

void Player::finish()
{
    if (timer_nextChunk) delete cancelEvent(timer_nextChunk);
}

void Player::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage())
    {
        throw cException("This module does NOT process external messages!");
        return;
    }

    handleTimerMessage(msg);
}

void Player::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_nextChunk)
    {
        scheduleAt(simTime() + m_interval_newChunk, timer_nextChunk);

        m_stat->reportChunkSeek(m_id_nextChunk);

        if (m_videoBuffer->isInBuffer(m_id_nextChunk))
        {
            emit(sig_chunkHit, m_id_nextChunk);
            m_stat->reportChunkHit(m_id_nextChunk);
        }
        else
        {
            emit(sig_chunkMiss, m_id_nextChunk);
            m_stat->reportChunkMiss(m_id_nextChunk);

            if (m_id_nextChunk >= m_videoBuffer->getBufferEndSeqNum())
            {
                cancelEvent(timer_nextChunk);
                scheduleAt(simTime() + param_interval_recheckVideoBuffer, timer_nextChunk);

                // -- recording rebuffering events
                emit(sig_rebuffering_local, m_id_nextChunk);
                m_stat->reportRebuffering(m_id_nextChunk);
            }
        }

        ++m_id_nextChunk;

    }
    else
    {
        throw cException("Wrong timer at Player module!");
    }
}

void Player::startPlayer()
{
    Enter_Method("startPlayer");
    scheduleAt(simTime(), timer_nextChunk);

    m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();

}
