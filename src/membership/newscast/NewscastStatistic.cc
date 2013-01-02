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

#include "NewscastStatistic.h"

Define_Module(NewscastStatistic);

NewscastStatistic::NewscastStatistic() {
    // TODO Auto-generated constructor stub


}

NewscastStatistic::~NewscastStatistic() {
    // TODO Auto-generated destructor stub
    m_outFileRandomIPs.close();
    m_outFileConnections.close();
}

void NewscastStatistic::initialize(int stage){

    if (stage != 3)
        return;
    //m_outFileRandomIPs.open(".//results//newscastGetRandomIPs.txt", std::fstream::out);
    m_outFileRandomIPs.open(par("newscastGetRandomIPsLOG").stringValue(), std::fstream::out);
    //m_outFileConnections.open(".//results//newscastConnections.txt", std::fstream::out);
    m_outFileConnections.open(par("newscastConnectionsLOG").stringValue(), std::fstream::out);
}

void NewscastStatistic::writeGotRandomIP(IPvXAddress addr){
    m_outFileRandomIPs << addr << endl;
}

void NewscastStatistic::writeCacheConnections(IPvXAddress src, std::vector<IPvXAddress> knownPeers){
    for (unsigned int i = 0; i < knownPeers.size(); i++)
        m_outFileConnections << src << ";" << knownPeers.at(i) << endl;
}
