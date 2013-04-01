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
// NewscastStatistic.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief class to output some statistics of newscast

#include "NewscastStatistic.h"

Define_Module(NewscastStatistic)

NewscastStatistic::NewscastStatistic()
{
}

NewscastStatistic::~NewscastStatistic()
{
    m_outFileRandomIPs.close();
    m_outFileConnections.close();
}

void NewscastStatistic::initialize(int stage)
{
    if (stage != 3)
        return;
    //m_outFileRandomIPs.open(".//results//newscastGetRandomIPs.txt", std::fstream::out);
    m_outFileRandomIPs.open(par("newscastGetRandomIPsLOG").stringValue(), std::fstream::out);
    //m_outFileConnections.open(".//results//newscastConnections.txt", std::fstream::out);
    m_outFileConnections.open(par("newscastConnectionsLOG").stringValue(), std::fstream::out);
}

void NewscastStatistic::writeGotRandomIP(IPvXAddress addr)
{
    m_outFileRandomIPs << addr << endl;
}

void NewscastStatistic::writeCacheConnections(IPvXAddress src, std::vector<IPvXAddress> knownPeers)
{
    for (unsigned int i = 0; i < knownPeers.size(); i++)
        m_outFileConnections << src << ";" << knownPeers.at(i) << endl;
    //m_outFileConnections << src << ";" << knownPeers.size() << endl;
}
