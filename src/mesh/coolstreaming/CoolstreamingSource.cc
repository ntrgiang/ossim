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
// CoolstreamingSource.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief CoolstreamingSource based on CoolstreamingBase
// @ingroup mesh
// @ingroup coolstreaming

#include "CoolstreamingSource.h"

Define_Module(CoolstreamingSource)

CoolstreamingSource::CoolstreamingSource()
{
    // TODO Auto-generated constructor stub
}

CoolstreamingSource::~CoolstreamingSource()
{
    // TODO Auto-generated destructor stub
}

void CoolstreamingSource::initialize(int stage)
{
    if (stage != 3)
        return;

    initBase();

    m_apTable->addAddress(getNodeAddress());
    m_apTable->printActivePeerTable();

    m_memManager->addSourceAddress(getNodeAddress(), param_maxNOP);

    m_nActive = -1;

    m_nActive = m_memManager->getActivePeerNumber();

    scheduleAt(simTime() + param_CheckPartnersInterval, timer_CheckPartners);

    WATCH(param_maxNOP);
    WATCH(m_localAddress);
    WATCH(m_nActive);
    WATCH(m_memManager);
}
