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
#include "DpControlInfo_m.h"

Define_Module(Forwarder);

Forwarder::Forwarder() {}

Forwarder::~Forwarder() {}

void Forwarder::handleMessage(cMessage* msg)
{
    Enter_Method("handleMessage");

    if (msg->isSelfMessage())
    {
        throw cException("No timer is used in this module");
        return;
    }

    IPvXAddress senderAddress;
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(msg->getControlInfo());
    senderAddress = controlInfo->getSrcAddr();

    PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(msg);
    EV << "PacketGroup = " << appMsg->getPacketGroup() << endl;
    if (appMsg->getPacketGroup() != PACKET_GROUP_VIDEO_CHUNK)
        throw cException("Wrong packet type!");

    EV << "A video chunk has just been received from " << senderAddress << endl;
    VideoChunkPacket *chunkPkt = check_and_cast<VideoChunkPacket *>(appMsg);

    m_videoBuffer->insertPacket(chunkPkt);
}

void Forwarder::initialize(int stage)
{
    if (stage != 3)
        return;

    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
}

void Forwarder::finish()
{

}

void Forwarder::sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort)
{
    Enter_Method("sendChunk");

    VideoChunkPacket *chunkPkt = m_videoBuffer->getChunk(seq)->dup();

    sendToDispatcher(chunkPkt, getLocalPort(), destAddress, destPort);
}
