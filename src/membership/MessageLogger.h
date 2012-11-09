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

#ifndef MESSAGELOGGER_H_
#define MESSAGELOGGER_H_

#include "IPvXAddress.h"
#include "AppCommon.h"
//#include <map>

class MessageLogger : public cSimpleModule {
public:
    MessageLogger();
    virtual ~MessageLogger();

    virtual void initialize();
    virtual void finish();

    // -- Working Interface
    bool nodeLogged(IPvXAddress addr);
    bool isUniqueMessage(cPacket *pkt, int& count);
    bool isUniqueMessage(IPvXAddress &addr, MESSAGE_ID_TYPE &msgId, int& count);
    void printNodeList(void);

private:
    std::map<IPvXAddress, MESSAGE_ID_LIST> m_logList;
};

#endif /* MESSAGELOGGER_H_ */
