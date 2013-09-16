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
// DonetStatistic.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


package so.global.statistic;

//import package so.global.StreamingStatistic;

//simple DonetStatistic like IStatisticBase
//simple DonetStatistic extends StreamingStatistic
simple DonetStatistic like IStreamingStatistic
{
   parameters:
      bool debug = default(false);
      string gstatLog = default("..//results//gstatlog.txt");
      double interval_reportCI @unit(s) = default(0.5s);
      double interval_reportSystemSize @unit(s) = default(0.5s);
      int numPeers = default(1);

   // -- Statistics till the end of the simulation
   //
   @statistic[numActivePeer](source="Signal_NumActivePeer";       record=last,vector);
   @statistic[numPartner](source="Signal_NumPartner";             record=last,vector);
   @statistic[globalCI](source="Signal_GlobalCI";                 record=last,vector);
   @statistic[globalLoss](source="Signal_GlobalLoss";             record=last,vector);
   @statistic[overlayHopCount](source="Signal_OverlayHopCount";   record=last,vector);
   @statistic[s2pDelay](source="Signal_S2pDelay";                 record=last,vector);

   @statistic[playback](source="Signal_Playback"; record=min,max,mean,count);
           // count: to see how many active peers are there till the end of the simulation



}
