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
// MeshOverlayObserver.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "MeshOverlayObserver.h"
#include <iomanip>

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

    //m_outFile.open(par("meshOverlayLogFile").stringValue(), fstream::out);

    //  m_outFile << "test \n";
    //  WATCH(m_meshOverlayLogFile);
}

void MeshOverlayObserver::handleMessage(cMessage *)
{
    throw cException("MeshOverlayObserver does NOT process messages!");
}

void MeshOverlayObserver::finish()
{
//    m_outFile.close();
}

void MeshOverlayObserver::writeToFile(const Link &link)
{
    /*m_outFile*/ EV << link.timeStamp << " " \
     << link.linkType << " " \
     << link.root << " " \
     << link.head << " " \
     << endl;
}

void MeshOverlayObserver::writeToFile(const Partnership &p)
{
    // -- Names of fields
    EV \
        << setw(15) << "IP address" << " " \
        << setw(6)  << "T_arr"      << " " \
        << setw(6)  << "T_join"     << " " \
        << setw(3)  << "nPner"      << " " \
        << setw(6)  << "T_st"       << " " \
        << setw(6)  << "head"       << " " \
        << setw(6)  << "begin"      << " " \
        << setw(5)  << "th_hold"    << " " \
        << setw(8)  << "n_Req_Recv" << " " \
        << setw(8)  << "n_sent"     << " " \
        << setw(8)  << "n_BMrecv"   << " " \
        << endl;

    EV \
        << setw(15) << p.address                << " " \
        << setw(6) << p.arrivalTime             << " " \
        << setw(6) << p.joinTime                << " " \
        << setw(3) << p.nPartner                << " " \
        << setw(6) << p.video_startTime         << " " \
        << setw(6) << p.head_videoStart         << " " \
        << setw(6) << p.begin_videoStart        << " " \
        << setw(5) << p.threshold_videoStart    << " " \
        << setw(8) << p.nChunkRequestReceived   << " " \
        << setw(8) << p.nChunkSent              << " " \
        << setw(8) << p.nBMrecv                 << " " \
        << endl;

    EV << "Partner list: " << endl;
    int listSize = p.partnerList.size();
    for (int i=0; i<listSize; ++i)
    {
        EV << p.partnerList[i] << endl;
    }
    //for (vector<IPvXAddress>::iterator iter = p.partnerList.begin(); iter != p.partnerList.end(); ++iter)
//    {
//        EV << *iter << endl;
//    }
}
