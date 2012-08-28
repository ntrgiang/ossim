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

#include "Forwarder.h"
#include "assert.h"

Define_Module(Forwarder);

Forwarder::Forwarder() {}

Forwarder::~Forwarder() {}

void Forwarder::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage())
    {
        throw cException("No timer is used in this module");
        return;
    }

    PeerStreamingPacket *appMsg = dynamic_cast<PeerStreamingPacket *>(msg);
    EV << "PacketGroup = " << appMsg->getPacketGroup() << endl;
    if (appMsg->getPacketGroup() != PACKET_GROUP_VIDEO_CHUNK)
    {
        //EV << "Wrong packet type!" << endl;
        throw cException("Wrong packet type!");
        return;
    }

    EV << "A video chunk has just been received from " << endl;
    //MeshVideoChunkPacket *chunkPkt = check_and_cast<MeshVideoChunkPacket *>(appMsg);
    VideoChunkPacket *chunkPkt = check_and_cast<VideoChunkPacket *>(appMsg);

    //////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
    EV << "Video chunk, before inserting ------------";
    m_videoBuffer->printStatus();
    #endif
    ///////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

    m_videoBuffer->insertPacket(chunkPkt);

    ///////////////////////////////////////////////////////////////////////////////////////// DEBUG_START //////////////////////////
    #if (__DONET_PEER_DEBUG__)
    EV << "ID of inserted chunk: " << chunkPkt->getSeqNumber() << " from " << senderAddress << endl;

    EV << "Video chunk, after inserting ------------ ";
    m_videoBuffer->printStatus();
    #endif
    ///////////////////////////////////////////////////////////////////////////////////////// DEBUG_END //////////////////////////

}

/*
void Forwarder::initialize()
{

}
*/

void Forwarder::initialize(int stage)
{
    if (stage != 3)
        return;

    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
    //assert(m_videoBuffer != NULL);
    if (m_videoBuffer == NULL) throw cException("m_videoBuffer == NULL is invalid");
}

void Forwarder::finish()
{

}

void Forwarder::sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort)
{
    Enter_Method("sendChunk");

    //MeshVideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(seq)->dup();
    VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(seq)->dup();

    sendToDispatcher(chunkPkt, getLocalPort(), destAddress, destPort);
}

//int Forwarder::getLocalPort()
//{
//
//}
