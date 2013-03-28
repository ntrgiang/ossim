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
// NewscastCacheEntry.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief a object to be stored in the cache contains: address, agent identifier, timestamp and value of the agent

#include "NewscastCacheEntry.h"

NewscastCacheEntry::NewscastCacheEntry()
{
    m_value = NULL;
    m_address = IPvXAddress("0.0.0.0");
}

NewscastCacheEntry::~NewscastCacheEntry()
{
    if (m_value) delete m_value;
}

long NewscastCacheEntry::getEstimatedSizeInBits()
{
    long ret = 0;

    ret = (sizeof(m_address) + m_agent.size() + sizeof(m_timestamp))*8;

    if (m_value != NULL)
        ret += m_value->getSizeInBits();

    return ret;
}
