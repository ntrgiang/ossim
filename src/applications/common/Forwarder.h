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

#ifndef FORWARDER_H_
#define FORWARDER_H_

#include "CommBase.h"
#include "VideoBuffer.h"
#include "Dispatcher.h"

class Forwarder : public CommBase {
public:
    Forwarder();
    virtual ~Forwarder();

    virtual void handleMessage(cMessage* msg);
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    //virtual void initialize();
    virtual void finish();

    void sendChunk(SEQUENCE_NUMBER_T seq, IPvXAddress destAddress, int destPort);
//
//protected:
//    void sendToDispatcher(cPacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort);
//    void printPacket(cPacket *pkt);

private:
    // -- Utility functions
    // int getLocalPort();

protected:
    // -- Pointers to external modules
    VideoBuffer *m_videoBuffer;
    Dispatcher *m_dispatcher;
};

#endif /* FORWARDER_H_ */
