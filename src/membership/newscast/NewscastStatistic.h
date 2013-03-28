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
// NewscastStatistic.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


// @author Thorsten Jacobi
// @brief class to output some statistics of newscast
// @ingroup membership
// @ingroup newscast


#include <fstream>
#include <iosfwd>

#include "csimplemodule.h"
#include "IPvXAddress.h"

#ifndef NEWSCASTSTATISTIC_H_
#define NEWSCASTSTATISTIC_H_

class NewscastStatistic : public cSimpleModule
{
public:
    NewscastStatistic();
    virtual ~NewscastStatistic();

    // @brief writes the address to the file
    void writeGotRandomIP(IPvXAddress addr);

    // @brief writes the connections of a peer to the file
    void writeCacheConnections(IPvXAddress src, std::vector<IPvXAddress> knownPeers);
protected:
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);
private:
    std::ofstream m_outFileRandomIPs;
    std::ofstream m_outFileConnections;
};

#endif /* NEWSCASTSTATISTIC_H_ */
