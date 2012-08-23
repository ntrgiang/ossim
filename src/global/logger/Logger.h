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

#ifndef LOGGER_H
#define LOGGER_H

#include "AppCommon.h"
#include <fstream>

using namespace std;

class Logger : public cSimpleModule
{
public:
    Logger() {}
    virtual ~Logger() {}

    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);

    virtual void handleMessage(cMessage *msg);
    virtual void finish();

//private:
public:
//    friend class DonetPeer;
    ofstream m_outFile;
    // string m_meshOverlayLogFile;
};

#endif /* LOGGER_H */
