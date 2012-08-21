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

#ifndef COMMBASE_H_
#define COMMBASE_H_

#include "IPvXAddress.h"

#include "ActivePeerTable.h"
#include "IChurnGenerator.h"
#include "GlobalStatistic.h"

class CommBase : public cSimpleModule {
public:
    CommBase();
    virtual ~CommBase();

protected:
    void sendToDispatcher(cPacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort);
    void printPacket(cPacket *pkt);

    // -- Get parameters from the AppSetting module
    int getLocalPort(void);
    int getDestPort(void);

    // -- Get parameters from the AppSettingDonet module
    int getVideoStreamBitRate(void);
    int getChunkSize(void);
    int getBufferMapSize(void);

    // -- Utility functions
    IPvXAddress getNodeAddress(void);

    // -- Binding to external modules
    void bindToGlobalModule(void);

protected:
    IChurnGenerator *m_churn;
    ActivePeerTable *m_apTable;
    GlobalStatistic *m_gstat;
};

#endif /* COMMBASE_H_ */
