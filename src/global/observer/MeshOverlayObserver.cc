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

#include "MeshOverlayObserver.h"

Define_Module(MeshOverlayObserver);

//MeshOverlayObserver::MeshOverlayObserver() {
//    // TODO Auto-generated constructor stub
//
//}
//
//MeshOverlayObserver::~MeshOverlayObserver() {
//    // TODO Auto-generated destructor stub
//}

void MeshOverlayObserver::initialize(int stage)
{
    if (stage == 0)
    {
    }

    if (stage != 3)
        return;

//    m_meshOverlayLogFile = par("meshOverlayLogFile");

    m_outFile.open(par("meshOverlayLogFile").stringValue(), fstream::out);

    //  m_outFile << "test \n";
//    WATCH(m_meshOverlayLogFile);
}

void MeshOverlayObserver::handleMessage(cMessage *)
{
    throw cException("MeshOverlayObserver does NOT process messages!");
}

void MeshOverlayObserver::finish()
{
    m_outFile.close();
}

void MeshOverlayObserver::writeToFile(const Link &link)
{
    m_outFile << link.timeStamp << " ";
    m_outFile << link.linkType << " ";
    m_outFile << link.root << " ";
    m_outFile << link.head << endl;
}

void MeshOverlayObserver::writeToFile(const Partnership &p)
{
    m_outFile << p.address << " ";
    m_outFile << p.arrivalTime << " ";
    m_outFile << p.joinTime << " ";
    m_outFile << p.nPartner << endl;
}
